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
	MSG_GETDIR	 = 1,	//< 请求目录
	MSG_DIRLIST  = 2,	//< 返回目录列表
	MSG_MAX_TYPE = 3,	//< 验证类型是否正确
};

// 消息最大字节数
#define MSG_MAX_SIZE 1000000

// 消息头
struct COM_API XMsgHead
{
	MsgType type;
	int size = 0;
};

//假定消息全部包含内容，否则发送OK
struct COM_API XMsg : public XMsgHead
{
	char* data = 0;		//< 存储消息正文
	int recved = 0;		//< 已经接受的消息字节数
};

#endif // ! XMSG_H
