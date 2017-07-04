#pragma once
#include <Windows.h>

class CBaseThread
{
public:
	CBaseThread(void);
	virtual ~CBaseThread(void);
public:
	virtual void start();										//�����߳�
	virtual void end();											//�����߳�
	virtual void resume();										//�����߳�
	virtual void suspend();										//��ͣ�߳�
	//
	virtual int  getThreadID() const;							//����߳�ID
	virtual BOOL isRun() const;									//�ж��߳��Ƿ�����
	virtual void runTask() = 0;									//����ʵ�ִ˺������ҵ���߼�
	//
	static unsigned _stdcall threadFunc(void* pParam);			//�̺߳���,��RunTask�߼�

protected:
	HANDLE m_hEndEvent;                                         //�����˳��̴߳���
	HANDLE m_hExitEvent;                                        //�߳�RunTask����ʱ����,ȷ���߳������˳�                                     
	HANDLE m_hThreadHandle;
	unsigned int m_uThreadID;
	//
};
