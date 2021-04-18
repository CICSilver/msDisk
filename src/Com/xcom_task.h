#ifndef XCOMTASK_H
#define XCOMTASK_H

#include "xtask.h"
#include "xmsg.h"
#include <string>
class COM_API XComTask :
    public XTask
{
public:
	virtual bool Init();
	void SetServerIp(std::string ip) { this->server_ip_ = ip; }
	void SetPort(int port) { this->server_port_ = port; }

	// 事件回调
	virtual void EventCB(short what);

	// 发送消息
	virtual bool Write(const XMsg* msg);

	// 连接成功时调用，用于发送通知消息
	virtual void ConnectedCB() {};

	// 接收到消息的回调，由具体业务重载
	virtual void ReadCB(const XMsg* msg);

	// 读取数据
	virtual void ReadCB();


protected:

	// 读取缓存
	char read_buf_[4096] = { 0 };

private:
    struct bufferevent* bev_ = 0;
	/// 服务器IP
	std::string server_ip_ = "";
	/// 服务器端口
	int server_port_ = 0;

	// 数据包缓存
	XMsg msg_;
};


#endif // !XCOMTASK_H