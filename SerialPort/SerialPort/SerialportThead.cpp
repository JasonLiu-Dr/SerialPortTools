#include "StdAfx.h"
#include "SerialportThead.h"
#include<iostream>

static const int MAX_SIEZ_BUFFER_SIZE = 2048;

CSerialportThread::CSerialportThread(void)
:m_com("Com1"),m_baudRate(2400),m_byteSize(8),m_parity(NOPARITY),m_stopBits(ONESTOPBIT), m_bInit(false)
{
}

CSerialportThread::~CSerialportThread(void)
{
}

CSerialportThread * CSerialportThread::getInstance()
{
	static CSerialportThread instance_;
	return &instance_;
}

//
void CSerialportThread::setCommConfig(const char* com, int baudRate, char byteSize, char parity, char stopBits)
{
	m_com		= com;
	m_baudRate	= baudRate;
	m_byteSize	= byteSize;
	m_parity	= parity;
	m_stopBits	= stopBits;
	
}

//
void CSerialportThread::initComm()
{
	if (m_serialport.openComm(m_com) && m_serialport.setupComm(MAX_SIEZ_BUFFER_SIZE, MAX_SIEZ_BUFFER_SIZE))//�������������������С
	{
		DCB dcb;
		COMMTIMEOUTS TimeOuts;
		TimeOuts.ReadIntervalTimeout = MAXDWORD;							//�������ʱ
		TimeOuts.ReadTotalTimeoutMultiplier = 0;							//��ʱ��ϵ��
		TimeOuts.ReadTotalTimeoutConstant = 0;								//��ʱ�䳣��
		TimeOuts.WriteTotalTimeoutMultiplier = 10;							//дʱ��ϵ��
		TimeOuts.WriteTotalTimeoutConstant = 50;							//дʱ�䳣��
		if (m_serialport.setCommTimeouts(TimeOuts) && m_serialport.getCommState(dcb))
		{
			dcb.BaudRate = m_baudRate;										//������2400
			dcb.ByteSize = m_byteSize;										//ÿ���ַ���8λ
			dcb.Parity	 = m_parity;										//��У��
			dcb.StopBits = m_stopBits;										//һ��ֹͣλ
																			//
			if (m_serialport.setCommState(dcb) && m_serialport.purgeComm()) //�ж϶�д��������ջ�����
				m_bInit = true;
		}
	}
}


//
void CSerialportThread::runTask()
{
	if (!m_bInit)
	{
		initComm();
	}
	//
	while (1)
	{
		if (WaitForSingleObject(m_hEndEvent, 50) == WAIT_OBJECT_0)
			break;
		vector<char> readbuffer;
		readbuffer.resize(MAX_SIEZ_BUFFER_SIZE);
		DWORD dwRead;
		int nRet = m_serialport.readFile(readbuffer, MAX_SIEZ_BUFFER_SIZE, dwRead, NULL);
		if (nRet == -1 || nRet == 0)
			continue;
		else
		{
			list<char> tmp(readbuffer.begin(), readbuffer.begin()+dwRead);
			m_addDataCallBack(tmp);
		}
		if (WaitForSingleObject(m_hEndEvent, 50) == WAIT_OBJECT_0)
			break;
	}
	SetEvent(m_hExitEvent);
}

//
int CSerialportThread::writeFile(const char * writeBuffer, int size)
{
	DWORD ret = 0;
	vector<char> buffer;
	int nSize = strlen(writeBuffer) < size ? strlen(writeBuffer): size;
	buffer.resize(nSize);
	buffer.assign(writeBuffer, writeBuffer + nSize);
	return m_serialport.writeFile(buffer, nSize, ret, NULL);

}

//
void CSerialportThread::initCallBack(void * pCallBack)
{
	m_addDataCallBack = (pGET_DATA_CAAL_BACK)pCallBack;
}
