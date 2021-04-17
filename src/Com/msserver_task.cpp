#include "msserver_task.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <iostream>
#include <string.h>
using namespace std;
static void SListenCB(struct evconnlistener* evc, evutil_socket_t client_socket, struct sockaddr* addr, int socklen, void *arg)
{
    cout << "SListenCB" << endl;
    auto task = (msServerTask *)arg;
    if (task->ListenCB)
    {
        task->ListenCB(client_socket, addr, socklen, arg);
    }
    else
    {
        cerr << "no callback function" << endl;
    }
}
bool msServerTask::Init()
{
    if (server_port_ <= 0)
    {
        cerr << "MsServerTask::Init failed! server_port_ hasnt been set!";
        return false;
    }
    //�����˿�
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_port = htons(server_port_);
    sin.sin_family = AF_INET;
    //���ûص�����
    auto evc = evconnlistener_new_bind(base(), SListenCB, this,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        10,  ///listen back
        (sockaddr*)&sin,
        sizeof(sin)
    );
    if (!evc)
    {
        cout << "listen port" << server_port_ << "failed!" << endl;
        return false;
    }
    else
    {
        cout << "listen port" << server_port_ << "success!" << endl;
        return true;
    }

    // ���ûص�����
    return false;
}
