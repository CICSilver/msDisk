#pragma once
#include "xcom_task.h"
class XFileServerTask :
    public XComTask
{
public:
    virtual void ReadCB(const XMsg* msg);

private:
    // 处理客户端获取目录的消息， 返回目录列表
    void GetDir(const XMsg* msg);
};

