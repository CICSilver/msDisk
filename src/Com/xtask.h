#ifndef XTASK_H
#define XTASK_H

#ifdef _WIN32
#ifdef COM_EXPORTS
#define COM_API __declspec(dllexport)
#else
#define COM_API __declspec(dllimport)
#endif
#else
#define COM_API
#endif

class COM_API XTask
{
public:

	//初始化任务
	virtual bool Init() = 0;

    int thread_id() { return thread_id_; }
    void set_thread_id(int thread_id) {  thread_id_ = thread_id; }

    int sock() { return sock_; }
    void set_sock(int sock) { this->sock_ = sock; }

    struct event_base *base() { return base_; }
    void set_base(struct event_base *base) { this->base_ = base; }
    
private:
    struct event_base *base_ = 0;
    int sock_ = 0;
    int thread_id_ = 0;

};
#endif
