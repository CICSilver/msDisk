#include "xcom_task.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string.h>
using namespace std;

static void SReadCB(struct bufferevent* bev, void* ctx)
{
    auto task = (XComTask*)ctx;
    task->ReadCB();
}
static void SWriteCB(struct bufferevent* bev, void* ctx)
{

}
static void SEventCB(struct bufferevent* bev, short what, void* ctx)
{
    auto task = (XComTask*)ctx;
    // �յ������Ƚ����쳣�ж�
    task->EventCB(what);
    
}
bool XComTask::Init()
{
    // ���ֿͻ��˺ͷ����
    int sock = this->sock();
    if (sock <= 0)
    {
        sock = -1;
    }
    // ��������, socket����-1���Զ�����
    this->bev_ = bufferevent_socket_new(base(), sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev_)
    {
        cerr << "bufferevent_socket_new" << endl;
    }

    // ���ûص�����
    bufferevent_setcb(bev_, SReadCB, SWriteCB, SEventCB, this);
    bufferevent_enable(bev_, EV_READ | EV_WRITE);

    // ���ó�ʱʱ��
    timeval tv = { 10,0 };
    bufferevent_set_timeouts(bev_, &tv, &tv);

    // ���ӷ�����
    if (server_ip_.empty())
    {
        // ����ָ�������ip�����ʼ��bufferevent����ִ������
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    evutil_inet_pton(AF_INET, server_ip_.c_str(), &sin.sin_addr.s_addr);
    // ��������
    int ret = bufferevent_socket_connect(bev_, (sockaddr*)&sin, sizeof(sin));

    if (ret != 0)
    {
        return false;
    }

    return false;
}

void XComTask::EventCB(short what)
{
    // �ж��Ƿ����ӳɹ�
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "BEV_EVENT_CONNECTED" << endl;
        ConnectedCB();
    }
    // ������
    if (what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT)
    {
        cout << "BEV_EVENT_ERROR or BEV_EVENT_TIMEOUT" << endl;

        bufferevent_free(bev_);
    }
    // ���ӶϿ����� (ע�⻺������)
    if (what & BEV_EVENT_EOF)
    {

    }
}

bool XComTask::Write(const XMsg* msg)
{
    if (!bev_ || !msg || !msg->data || msg->size <= 0)
    {
        return false;
    }
    // д��Ϣͷ
    int ret = bufferevent_write(bev_, msg, sizeof(XMsgHead));
    if (ret != 0)
    {
        return false;
    }
    // д����Ϣ����
    ret = bufferevent_write(bev_, msg->data, msg->size);
    if (ret != 0)
    {
        return false;
    }

    return true;
}

void XComTask::ReadCB(const XMsg* msg)
{
    cout << "recv Msg type : " << msg->type << ", " 
        << "msg size : " << msg->size << endl;
}

void XComTask::ReadCB()
{
    for (;;)     //< ��ֹ��Ե����ʱ�޷�ȫ����ȡ
    {
        // ������Ϣͷ����ȡ��Ϣ���ͺͳ���   
        if (!msg_.data)
        {
            int headSize = sizeof(XMsgHead);
            int len = bufferevent_read(bev_, &msg_, headSize);
            if (len <= 0)
            {
                // ��һ��ѭ���Ѿ������Ϣ��ȡ���˳�
                return;
            }
            if (len != headSize)
            {
                cerr << "msg head recv error" << endl;
                return;
            }
            // ��֤��Ϣͷ�Ƿ���Ч
            if (msg_.type >= MSG_MAX_TYPE || msg_.size <= 0 || msg_.size > MSG_MAX_SIZE)
            {
                cerr << "msg head invalid" << endl;
                return;
            }
            msg_.data = new char[msg_.size];
        }
        int readSize = msg_.size - msg_.recved;
        if (readSize <= 0)
        {
            // �ѽ��ܵ���Ϣ�����ܳ��ȣ���Ϣ��ʽ������
            delete msg_.data;
            memset(&msg_, 0, sizeof(msg_));
            return;
        }
        int len = bufferevent_read(bev_, msg_.data + msg_.recved, msg_.size);
        if (len <= 0)
        {
            return;
        }
        msg_.recved += len;
        if (msg_.recved == msg_.size)
        {
            // ������Ϣ����
            cout << "recved msg" << msg_.size << endl;
            ReadCB(&msg_);
            delete msg_.data;
            memset(&msg_, 0, sizeof(msg_));
        }
    }
}
