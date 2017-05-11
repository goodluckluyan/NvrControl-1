//@file:C_ThreadData.h
//@brief: ����C_ThreadData���ڲ������߳�������Ҫ�����ݣ������߳����С�
//@author: wangzhongping@oristartech.com
//date: 2012-05-23
#ifndef THREAD_DATA
#define THREAD_DATA

#include "C_CS.h"
#include "../timeTask/C_Task.h"
#include <pthread.h>
#include "../C_constDef.h"
#include "../C_ErrorDef.h"
#include "../log/C_LogManage.h"
#include "../para/C_RunPara.h"
class C_ThreadData
{
public:
	C_ThreadData()
	{
		m_iState = INIT_STATE;
		m_iResumeCount = 0;
		m_iCommand = -1;
		m_pGsoap = NULL;
		m_pCommandPara = NULL;
		m_iErrorNumber = 0;
		m_strError = "";
		m_iRunType = RUN_UNKNOWN_TYPE;
		m_pReturn = NULL;
		m_iStartTime = -1;
		m_iWorkTime = 1800;
		m_pTask = NULL;
		m_bQuit = false;
		pthread_cond_init(&cond,NULL);
	}

	~C_ThreadData()
	{
		pthread_cond_destroy(&cond);
	}

	//�����̡߳�
	//����ֵ���ޡ�
	//������newState �̹߳�������õ�״̬��iResumeCount ��Ҫ����resume�Ĵ���
	void suspend(Thread_State newState, int iResumeCount = 1)
	{
		m_ThreadCS.EnterCS();
		if(m_iState == INIT_STATE || m_iState == RUN_STATE || m_iState == RUN_LOCKED_STATE)
		{
			m_iResumeCount = iResumeCount;
			m_iState = newState;
			pthread_cond_wait(&cond,&(m_ThreadCS.m_CS));

		}
		else
		{
			C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_THREADMGR,0,ERROR_THREAD_STATE,"�߳�״̬����");
			//printf("suspend error pThreadData:%x\n",(unsigned int)this);
		}
		m_ThreadCS.LeaveCS();
			
	}

	//�����߳����С�
	void resume()
	{
		m_ThreadCS.EnterCS();
		//wzp on 2012-12-13 modify
	/*	if(--m_iResumeCount == 0 && m_iState == SUSPEND_LOCKED_STATE)
		{
			pthread_cond_broadcast(&cond);
		}
		m_iState = RUN_STATE;*/
	  if(--m_iResumeCount == 0 && m_iState == SUSPEND_LOCKED_STATE)
		{
			
			pthread_cond_signal(&cond) ;
			
			m_iState = RUN_STATE;	

			// when bug��use.
			C_RunPara *pRunPara = C_RunPara::GetInstance();
			m_iStartTime = pRunPara->GetCurTime();
			
		}		
		//wzp end.
		m_ThreadCS.LeaveCS();
	}
	
	//�жϵ�ǰ�߳��Ƿ���У�
	Thread_State IsIdlState()
	{
		m_CS.EnterCS();
		Thread_State oldState = m_iState;
		if(m_iState == SUSPEND_IDL_STATE)
		{
			m_iState = SUSPEND_LOCKED_STATE;
		}
		m_CS.LeaveCS();
		return oldState;
	}
	
	//�жϵ�ǰ�߳��Ƿ�ʱ
	int IsTimeoutState(int iCurTime)
	{
		m_CS.EnterCS();
		int iResult = -1;
		if(m_iState == RUN_STATE && iCurTime - m_iStartTime > m_iWorkTime)
		{
			//m_iState = RUN_LOCKED_STATE; //wzp delete on 2012-12-13 
			C_LogManage *pLogManage = C_LogManage::GetInstance();
			char tmp[1024];
			memset(tmp, 0, 1024);
			if(m_iRunType == 1)
			{
				sprintf(tmp, "Thread Timeout threadId:%u m_iRunType:%d TimeTaskId:%d",(unsigned int)m_hThread, m_iRunType,
					m_pTask->m_iCommandNumber);
			}
			else
			{
				sprintf(tmp, "Thread Timeout threadId:%u m_iRunType:%d ",(unsigned int)m_hThread, m_iRunType);
			}
			iResult = pLogManage->CreateLogNumber(ULOG_FATAL,LOG_MODEL_THREADMGR,0,ERROR_THREAD_TIMEOUT);
			pLogManage->WriteLog(iResult,tmp);
			//printf("%s\n",tmp);
			iResult = 0;
		}
		m_CS.LeaveCS();
		return iResult;
	}
	Thread_State SetState(Thread_State state)
	{
		Thread_State oldState;
		m_CS.EnterCS();
		oldState = m_iState;
		m_iState = state;
		m_CS.LeaveCS();
		return oldState;
	}

	Thread_State GetState()
	{
		return m_iState;
	}

	C_ThreadData* GetThreadData(pthread_t &threadId)
	{
		C_ThreadData*  pData = NULL;
		m_CS.EnterCS();
		if(pthread_equal(m_hThread, threadId) != 0)
		{
			pData =  this;
		}
		m_CS.LeaveCS();
		return 	pData;			
	}
	
public:

  C_Task * m_pTask;
  int m_iCommand;
  void *m_pGsoap;
  void *m_pCommandPara;
  void *m_pReturn;

  //��ǰ�����Ŀ�ʼʱ��
  int m_iStartTime;

  // ��ǰ������Ԥ��ִ��ʱ�䳤�� ��λ����
  int m_iWorkTime;
  int m_iErrorNumber;
  std::string m_strError;

  //0:webservice; 1:timeTask
  int m_iRunType;
  C_CS m_CS;
  C_CS m_ThreadCS;
  volatile int m_iResumeCount;
  pthread_t  m_hThread;
  pthread_cond_t cond;
  bool m_bQuit;
 

private:

	volatile Thread_State m_iState; 
};
#endif //THREAD_DATA;

