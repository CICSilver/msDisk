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

	// �¼��ص�
	virtual void EventCB(short what);

	// ������Ϣ
	virtual bool Write(const XMsg* msg);

	// ���ӳɹ�ʱ���ã����ڷ���֪ͨ��Ϣ
	virtual void ConnectedCB() {};

	// ���յ���Ϣ�Ļص����ɾ���ҵ������
	virtual void ReadCB(const XMsg* msg);

	// ��ȡ����
	virtual void ReadCB();


protected:

	// ��ȡ����
	char read_buf_[4096] = { 0 };

private:
    struct bufferevent* bev_ = 0;
	/// ������IP
	std::string server_ip_ = "";
	/// �������˿�
	int server_port_ = 0;

	// ���ݰ�����
	XMsg msg_;
};


#endif // !XCOMTASK_H