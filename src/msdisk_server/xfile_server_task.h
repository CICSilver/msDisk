#pragma once
#include "xcom_task.h"
class XFileServerTask :
    public XComTask
{
public:
    virtual void ReadCB(const XMsg* msg);

private:
    // ����ͻ��˻�ȡĿ¼����Ϣ�� ����Ŀ¼�б�
    void GetDir(const XMsg* msg);
};

