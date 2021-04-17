#ifndef MSSERVERTASK_H
#define MSSERVERTASK_H

#include "xtask.h"
#ifdef _WIN32
#ifdef COM_EXPORTS
#define COM_API __declspec(dllexport)
#else
#define COM_API __declspec(dllimport)
#endif
#else 
#define COM_API
#endif
typedef void (*ListenCBFunc)(int sock, struct sockaddr* addr, int socklen, void* arg);
class COM_API msServerTask : public XTask
{
public:
	// 接受客户端的连接
	virtual bool Init();
	ListenCBFunc ListenCB = 0;
	void set_server_port(int port) { this->server_port_ = port; }

private:
	int server_port_;
};


#endif // !1