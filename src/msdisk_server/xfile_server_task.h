#ifndef XFILE_SERVER_TASK_H
#define XFILE_SERVER_TASK_H
#include "xcom_task.h"
#include <fstream>
#include <mutex>
class XFileServerTask :public XComTask
{
public:
    //���յ���Ϣ�Ļص�
    virtual bool ReadCB(const XMsg *msg);

    //���ر���Ϣ����ʱ�����ݽ����͵��˺�������ҵ��ģ������
    virtual void ReadCB(void *data, int size);

    ///д�����ݻص�����
    virtual void WriteCB();

    static void set_cur_dir(std::string dir)
    {
        cur_dir_mux_.lock();
        cur_dir_ = dir;
        cur_dir_mux_.unlock();
    }

    static std::string cur_dir()
    {
        cur_dir_mux_.lock();
        std::string dir = cur_dir_;
        cur_dir_mux_.unlock();
        return dir;
    }
private:
    //����Ŀ¼��ȡ����Ϣ������Ŀ¼�б�
    void GetDir(const XMsg *msg);

    //����ͻ��˵��ϴ�����
    void Upload(const XMsg *msg);

    //����ͻ��˵���������
    void Download(const XMsg* msg);

    //�ļ���С
    int filesize_ = 0;

    //�ͻ��Ѿ��ϴ��Ĵ�С
    int recv_size_ = 0;

    ///��ǰ·��
    static std::string cur_dir_;
    static std::mutex cur_dir_mux_;

    //д����յ��ļ�
    std::ofstream ofs_;

    //��ȡ�ļ�
    std::ifstream ifs_;

    std::string filepath_;

};

#endif 

