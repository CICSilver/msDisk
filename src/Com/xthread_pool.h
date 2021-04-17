#ifndef XTHREAD_POOL_H
#define XTHREAD_POOL_H

#ifdef _WIN32
#ifdef COM_EXPORTS
#define COM_API __declspec(dllexport)
#else
#define COM_API __declspec(dllimport)
#endif
#else 
#define COM_API
#endif
#include <vector>
class XThread;
class XTask;
class COM_API XThreadPool
{
public:
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ȡXThreadPool�ľ�̬���� ����̬������
    /// @return XThreadPool ��̬�����ָ��
    ///////////////////////////////////////////////////////////////////////////
	static XThreadPool* Get()
	{
		static XThreadPool p;
		return &p;
	}
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ʼ�������̲߳������̣߳�������event_base ,�����߳��п�ʼ������Ϣ
	void Init(int thread_count);

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �ַ������߳���ִ�У������task��Init���������ʼ��
    ///        �������ѯ�ַ����̳߳��еĸ����߳�
    /// @param task ����ӿڶ���XTask��Ҫ�û��Լ��̳в�����Init����
	void Dispatch(XTask *task);

    XThreadPool() {};
private:
	///�߳�����
	int thread_count_ = 0;

    ///��һ�ηַ��õ��̣߳�������ѯ
	int last_thread_ = -1;

	///�̳��̶߳���
	std::vector<XThread *>threads_;
	

};

#endif