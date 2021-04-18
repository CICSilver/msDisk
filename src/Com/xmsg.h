#ifndef XMSG_H
#define XMSG_H

#ifdef _WIN32
#ifdef COM_EXPORTS
#define COM_API __declspec(dllexport)
#else
#define COM_API __declspec(dllimport)
#endif
#else
#define COM_API
#endif
enum MsgType
{
	MSG_NONE     = 0,
	MSG_GETDIR	 = 1,	//< ����Ŀ¼
	MSG_DIRLIST  = 2,	//< ����Ŀ¼�б�
	MSG_MAX_TYPE = 3,	//< ��֤�����Ƿ���ȷ
};

// ��Ϣ����ֽ���
#define MSG_MAX_SIZE 1000000

// ��Ϣͷ
struct COM_API XMsgHead
{
	MsgType type;
	int size = 0;
};

//�ٶ���Ϣȫ���������ݣ�������OK
struct COM_API XMsg : public XMsgHead
{
	char* data = 0;		//< �洢��Ϣ����
	int recved = 0;		//< �Ѿ����ܵ���Ϣ�ֽ���
};

#endif // ! XMSG_H
