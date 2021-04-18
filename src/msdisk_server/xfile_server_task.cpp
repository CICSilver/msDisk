#include "xfile_server_task.h"
#include "xtools.h"
#include <iostream>
#include <string.h>
using namespace std;
string XFileServerTask::cur_dir_ = "./";
std::mutex XFileServerTask::cur_dir_mux_;
//����Ŀ¼��ȡ����Ϣ������Ŀ¼�б�
void XFileServerTask::GetDir(const XMsg *msg)
{
    if (!msg->data)return;
    string root = msg->data;
    if (root.empty())
    {
        root = "./";
    }
    set_cur_dir(root);
    //string dir = "file1,1024;file2,4096;file3.zip,10240";
    string dir = GetDirData(root);
    XMsg resmsg;
    resmsg.type = MSG_DIRLIST;
    resmsg.size = dir.size() + 1; // +1����\0
    resmsg.data = (char*)dir.c_str();
    Write(&resmsg);
}
//����ͻ��˵��ϴ�����
void XFileServerTask::Upload(const XMsg *msg)
{
    //1 ��ȡ�ļ������ļ���С
    if (!msg->data || !msg || msg->size <= 0)return;
    string str = msg->data;
    if (str.empty())return;
    //filename.zip,1024
    int pos = str.find_last_of(',');
    if (pos <= 0)return;
    string filename = str.substr(0, pos);
    int sizepos = pos + 1;
    if (sizepos >= str.size())
        return;
    string tmp = str.substr(sizepos, str.size() - sizepos);
    cout << filename << ":" << tmp << endl;
    filesize_ = atoi(tmp.c_str());
    if (filesize_ <= 0)
        return;

    //2 �򿪱����ļ�
    string filepath = cur_dir() + filename;
    ofs_.open(filepath, ios::out | ios::binary);
    if (!ofs_.is_open())
    {
        cout << "open file " << filepath << " failed!" << endl;
        return;
    }
    cout << "open file " << filepath << " success!" << endl;
    //3 �ظ�accept
    XMsg resmsg;
    resmsg.type = MSG_UPLOAD_ACCEPT;
    resmsg.size = 3; // +1����\0
    resmsg.data = (char*)"OK";
    Write(&resmsg);

    //��������Ϣ����ʼ�����ļ�
    set_is_recv_msg(false);
    recv_size_ = 0;
}

//����ͻ��˵���������
void XFileServerTask::Download(const XMsg* msg)
{
    if (!msg->data || !msg || msg->size <= 0)return;
    //���ļ�
    filepath_ = msg->data;
    if (filepath_.empty())
        return;
    //��ȡ�ļ���
    ifs_.open(filepath_.c_str(), ios::in | ios::binary | ios::ate);
    if (!ifs_.is_open())
    {
        cerr << "open file " << filepath_ << " failed!" << endl;
        return;
    }
    //2 ��ȡ�ļ����� �ļ���С  filename.zip,1024
    filesize_ = ifs_.tellg();
    ifs_.seekg(0, ios::beg);
    cout << "open file " << filepath_ << " success!" << endl;

    //�ظ���Ϣ MSG_DOWNLOAD_ACCEPT
    char buf[32] = { 0 };
    //_CRT_SECURE_NO_WARNINGS
    #ifdef _WIN32
    sprintf_s(buf, "%d", filesize_);
    #else
    sprintf(buf, "%d", filesize_);
    #endif
    XMsg resmsg;
    resmsg.type = MSG_DOWNLOAD_ACCEPT;
    resmsg.size = strlen(buf) + 1; // +1����\0
    resmsg.data = buf;
    Write(&resmsg);

}

///д�����ݻص�����
void XFileServerTask::WriteCB()
{
    if (!ifs_.is_open())return;
    ifs_.read(read_buf_, sizeof(read_buf_));
    int len = ifs_.gcount();
    if (len <= 0)
    {
        ifs_.close();
        return;
    }
    Write(read_buf_, len);
    if (ifs_.eof())
    {
        ifs_.close();
    }
}

//���ر���Ϣ����ʱ�����ݽ����͵��˺�������ҵ��ģ������
void XFileServerTask::ReadCB(void *data, int size)
{
    if (!data || size <= 0 || !ofs_.is_open())return;
    ofs_.write((char*)data, size);
    recv_size_ += size;
    if (recv_size_ == filesize_)
    {
        cout << "file write end" << endl;
        ofs_.close();
        XMsg resmsg;
        resmsg.type = MSG_UPLOAD_COMPLETE;
        resmsg.size = 3; // +1����\0
        resmsg.data = (char*)"OK";
        Write(&resmsg);
    }
}
bool XFileServerTask::ReadCB(const XMsg *msg)
{
    switch (msg->type)
    {
    case MSG_GETDIR:
        cout << "MSG_GETDIR" << endl;
        GetDir(msg);
        break;
    case MSG_UPLOAD_INFO:
        cout << "MSG_UPLOAD_INFO" << endl;
        Upload(msg);
        break;
    case MSG_DOWNLOAD_INFO:
        cout << "MSG_DOWNLOAD_INFO" << endl;
        Download(msg);
        break;
    case MSG_DOWNLOAD_COMPLETE:
        cout << "MSG_DOWNLOAD_COMPLETE" << endl;
        //����������Դ
        Close();
        return false;
        break;
    default:
        break;
    }
    return true;
}

