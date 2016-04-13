#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <WinSock2.h>
#include <MSwsock.h>
#include <functional>
#include <vector>
#include <memory>
#include "Guard.h"

#pragma comment(lib,"ws2_32.lib")

typedef std::function<void(void* k, int flags, bool result, int transfered, byte* buf, size_t len, void* param)> iocp_proc_t;

struct IOCP_OVERLAPPED_T
{
	OVERLAPPED _olp;
	HANDLE _handle;
	std::shared_ptr<byte> _buf;
	NETOPERATE _iocptype;
	size_t _htype;
};

struct IOCP_CONTEXT_T
{
	HANDLE _handle;
	std::shared_ptr<RWLOCK> _lockptr;
	std::vector<std::shared_ptr<IOCP_OVERLAPPED_T>> _iocontext;
	__int64 _starttime;//�ػ���ʼʱ��
	unsigned long _outtimes;//�ػ���ʱʱ��
	size_t _htype;
};





#endif