#include <iostream>
#include "xthread_pool.h"
#include "msserver_task.h"
#include "xfile_server_task.h"
#include <thread>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif
using namespace std;

static void ListenCB(int sock, struct sockaddr* addr, int socklen, void* arg)
{
	auto task = new XFileServerTask();
	task->set_sock(sock);
	XThreadPool::Get()->Dispatch(task);
	cout << "ListenCB in main" << endl;

}

int main(int argc, char* argv[])
{
#ifdef _WIN32
	// 初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return 1;
#endif
	int server_port = 21002;
	int thread_count = 10;
	if (argc > 1)
	{
		server_port = atoi(argv[1]);
	}
	if (argc > 2)
	{
		thread_count = atoi(argv[2]);
	}
	if (argc == 1)
	{
		cout << "msdisk_server port thread_count" << endl;
	}
	// 初始化主线程池
	XThreadPool::Get()->Init(thread_count);

	XThreadPool server_pool;
	server_pool.Init(1);
	auto task = new msServerTask();
	task->set_server_port(server_port);
	task->ListenCB = ListenCB;
	server_pool.Dispatch(task);


	for (;;)
	{
		this_thread::sleep_for(1s);
	}
	return 0;
}