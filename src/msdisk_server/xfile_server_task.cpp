#include "xfile_server_task.h"
#include "xtools.h"
#include <iostream>
using namespace std;

void XFileServerTask::GetDir(const XMsg* msg)
{
	if (!msg->data) return;
	string root = msg->data;
	if (root.empty())
	{
		root = "./";
	}
	string dir = GetDirData(msg->data);
	XMsg retMsg;
	retMsg.type = MSG_DIRLIST;
	retMsg.size = dir.size() + 1;
	retMsg.data = (char*)dir.c_str();
	Write(&retMsg); 
}
void XFileServerTask::ReadCB(const XMsg* msg)
{
	switch (msg->type)
	{
	case MSG_GETDIR:
	{
		cout << "MSG_GETDIR" << endl;
		GetDir(msg);
		break;
	}
	default:
		break;
	}
}

