#ifndef MSSERVER_TASK_H
#define MSSERVER_TASK_H

#include "xtask.h"

typedef void (*ListenCBFunc)(int sock, struct sockaddr* addr, int socklen, void* arg);
class COM_API msServerTask : public XTask
{
public:
	virtual bool Init();
	ListenCBFunc ListenCB = 0;
	void set_server_port(int port) { this->server_port_ = port; }

private:
	int server_port_;
};


#endif // MSSERVERTASK_H