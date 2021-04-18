#include "xcom_task.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
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
    // 收到连接先进行异常判断
    task->EventCB(what);
    
}
bool XComTask::Init()
{
    // 区分客户端和服务端
    int sock = this->sock();
    if (sock <= 0)
    {
        sock = -1;
    }
    // 建立连接, socket传入-1即自动创建
    this->bev_ = bufferevent_socket_new(base(), sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev_)
    {
        cerr << "bufferevent_socket_new" << endl;
    }

    // 设置回调函数
    bufferevent_setcb(bev_, SReadCB, SWriteCB, SEventCB, this);
    bufferevent_enable(bev_, EV_READ | EV_WRITE);

    // 设置超时时间
    timeval tv = { 10,0 };
    bufferevent_set_timeouts(bev_, &tv, &tv);

    // 连接服务器
    if (server_ip_.empty())
    {
        // 若不指定服务端ip则仅初始化bufferevent，不执行连接
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    evutil_inet_pton(AF_INET, server_ip_.c_str(), &sin.sin_addr.s_addr);
    // 建立连接
    int ret = bufferevent_socket_connect(bev_, (sockaddr*)&sin, sizeof(sin));

    if (ret != 0)
    {
        return false;
    }

    return false;
}

void XComTask::EventCB(short what)
{
    // 判断是否连接成功
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "BEV_EVENT_CONNECTED" << endl;
        ConnectedCB();
    }
    // 错误处理
    if (what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT)
    {
        cout << "BEV_EVENT_ERROR or BEV_EVENT_TIMEOUT" << endl;

        bufferevent_free(bev_);
    }
    // 连接断开处理 (注意缓冲内容)
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
    // 写消息头
    int ret = bufferevent_write(bev_, msg, sizeof(XMsgHead));
    if (ret != 0)
    {
        return false;
    }
    // 写入消息正文
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
    for (;;)     //< 防止边缘触发时无法全部读取
    {
        // 接收消息头，获取消息类型和长度   
        if (!msg_.data)
        {
            int headSize = sizeof(XMsgHead);
            int len = bufferevent_read(bev_, &msg_, headSize);
            if (len <= 0)
            {
                // 上一次循环已经完成消息读取，退出
                return;
            }
            if (len != headSize)
            {
                cerr << "msg head recv error" << endl;
                return;
            }
            // 验证消息头是否有效
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
            // 已接受的消息大于总长度，消息格式有问题
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
            // 处理消息正文
            cout << "recved msg" << msg_.size << endl;
            ReadCB(&msg_);
            delete msg_.data;
            memset(&msg_, 0, sizeof(msg_));
        }
    }
}
