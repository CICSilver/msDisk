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
    MSG_NONE = 0,
    MSG_GETDIR,             //����Ŀ¼
    MSG_DIRLIST,            //����Ŀ¼�б�
    MSG_UPLOAD_INFO,        //�����ϴ��ļ�
    MSG_UPLOAD_ACCEPT,      //�����׼���ý����ļ�
    MSG_UPLOAD_COMPLETE,    //����˽����ļ�����
    MSG_DOWNLOAD_INFO,      //���������ļ�
    MSG_DOWNLOAD_ACCEPT,    //��ʼ�������ݸ��ͻ���
    MSG_DOWNLOAD_COMPLETE,  //�ͻ������سɹ�



    MSG_MAX_TYPE,       //������֤������ȷ

};
//��Ϣ����ֽ���
#define MSG_MAX_SIZE 1000000

//��Ϣͷ
struct COM_API XMsgHead
{
    MsgType type;
    int size = 0;
};
//Լ��ÿ����Ϣ����������ݣ�û������ʹ��OK
struct COM_API XMsg :public XMsgHead
{
    char *data = 0;     //�洢��Ϣ����
    int recved = 0;     //�Ѿ����յ���Ϣ�ֽ���

};

#endif