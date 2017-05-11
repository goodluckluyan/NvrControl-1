//@file:C_TaskList.cpp
//@brief: ...
//@author: wangzhongping@oristartech.com
//date: 2012-05-23

#include "../threadManage/C_ThreadManage.h"
#include "C_TaskList.h"
#include "../log/C_LogManage.h"
#include "../C_ErrorDef.h"
#include "../para/C_RunPara.h"

#define  LOGFAT(errid,msg)  C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_TIMETASK,0,errid,msg)
#define  LOGINFFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_INFO,LOG_MODEL_TIMETASK,0,errid,fmt,##__VA_ARGS__)



C_TaskList *C_TaskList::m_pInstance = NULL;
C_TaskList* C_TaskList::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new C_TaskList;
    }
    return m_pInstance;
}

void  C_TaskList::DestoryInstance()
{

	if(m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


C_TaskList::C_TaskList()
{
	
}
C_TaskList::~C_TaskList()
{
	std::list<C_Task *>::iterator it = m_TackList.begin();
	for(;it != m_TackList.end();it++)
	{
		C_Task * ptrTask = * it;
		if(ptrTask)
		{
			delete ptrTask;
		}
	}
	m_TackList.clear();
}

/*******************************************************************************
* �������ƣ�	InitTaskList
* ����������	��ʼ���������
* ���������	
* ���������	
* �� �� ֵ��	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
int C_TaskList::InitTaskList(CInvoke * ptrInvoker)
{
	int iTaskCount = 500;
	C_Task *pTask = NULL;
	for(int i=0; i<iTaskCount; ++i)
	{
		pTask = new C_Task(ptrInvoker);
		m_TackList.push_back(pTask);
	}
	return 0;
}


/*******************************************************************************
* �������ƣ�	GetIdleTask
* ����������	��ȡ��������
* ���������	����ָ���ָ��
* ���������	
* �� �� ֵ��	0���ɹ�����0ʧ��
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
int C_TaskList::GetIdleTask(C_Task **ppTask)
{
	C_GuardCS guard(&m_cs);
	std::list<C_Task*>::iterator it = m_TackList.begin();
	for(; it != m_TackList.end(); ++it)
	{
		if((*it)->IsIdle() == TASK_IDLE_STATE)
		{
			*ppTask = (C_Task*)(*it);
			return 0;
		}			
	}
	C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_TIMETASK,0,ERROR_TASK_LIST_FULL,"��������������޿�������");
	return C_LogManage::GetInstance()->CreateLogNumber(3,18,0,ERROR_TASK_LIST_FULL); 
	
}

/*******************************************************************************
* �������ƣ�	DeleteTask
* ����������	ɾ������
* ���������	�����
* ���������	
* �� �� ֵ��	0���ɹ�����0ʧ��
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
bool C_TaskList::DeleteTask(int nCommandNumber)
{
	bool bRet = false;
	C_GuardCS guard(&m_cs);
	std::list<C_Task*>::iterator it = m_TackList.begin();
	for(; it != m_TackList.end(); ++it)
	{
		if((*it)->GetCommandNumber() == nCommandNumber)
		{
			it = m_TackList.erase(it);
			bRet = true;
			break;
		}			
	}
	
	return bRet;

}

/*******************************************************************************
* �������ƣ�	AddTask
* ����������	���������������
* ���������	iTaskNum��������
				pPara :����ָ��
				iStarTime:��ʱ����ʱʱ�䣬0Ϊ��������-1Ϊ�̶�����
* ���������	
* �� �� ֵ��	0���ɹ�����0ʧ��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01				����
*******************************************************************************/
int C_TaskList::AddTask(int iTaskNum,  void *pPara, int iStartTime)
{
	C_Task *pTask;
	int iResult = GetIdleTask(&pTask);
	if(iResult != 0)
	{
		return iResult;
	}
	pTask->m_iCommandNumber = iTaskNum;
	
	pTask->m_pPara = pPara;
	if(0 == iStartTime )
	{
		pTask->m_emTaskType= ONCE_TASK;
		pTask->m_iStartTime = 0;
	}
	else if(-1 == iStartTime)
	{
		pTask->m_emTaskType= ALWAYS_TASK;
		pTask->m_iStartTime = 0;
	}
	else
	{	
		pTask->m_emTaskType = TIME_TASK;
		pTask->m_iStartTime = iStartTime;
	}

	pTask->SetTaskState(TASK_NO_STATE);
	return 0;
}

/*******************************************************************************
* �������ƣ�	AddTask
* ����������	���������������
* ���������	iTaskNum��������
				pPara :����ָ��
				iStarTime:��ʱ����ʱʱ�䣬0Ϊ��������-1Ϊ�̶�����
* ���������	
* �� �� ֵ��	0���ɹ�����0ʧ��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01				����
*******************************************************************************/
int C_TaskList::AddTask(int iTaskNum,  void *pPara, int iStartTime,int nType)
{
	C_Task *pTask;
	int iResult = GetIdleTask(&pTask);
	if(iResult != 0)
	{
		return iResult;
	}
	
	if(nType != ONCE_TASK && nType != ALWAYS_TASK && nType != TIME_TASK )
	{
		return -1;
	}

	pTask->m_iCommandNumber = iTaskNum;
	pTask->m_pPara = pPara;
	pTask->m_emTaskType = (TASK_TYPE )nType;
	pTask->m_iStartTime = iStartTime;
	pTask->SetTaskState(TASK_NO_STATE);
	return 0;
}


/*******************************************************************************
* �������ƣ�	RunTasks
* ����������	��ȡ�̲߳�ִ�ж�ʱ������������
* ���������	iCurTime����ǰʱ�䣨�룩
* ���������	
* �� �� ֵ��	0���ɹ�����0ʧ��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01				����
*******************************************************************************/
int C_TaskList::RunTasks(int iCurTime)
{
	C_ThreadManage *pThreadManage = C_ThreadManage::GetInstance();
	C_ThreadData *pThreadData = NULL;
	int iResult = -1;

	C_GuardCS guard(&m_cs);
	std::list<C_Task*>::iterator it = m_TackList.begin();
	for(;it != m_TackList.end(); ++it)
	{
		if((*it)->IsEnableRun(iCurTime) == 0)
		{
			iResult = pThreadManage->GetIdlThread(&pThreadData);
			if(iResult != 0)
			{
				LOGINFFMT(0,"Busy! No idle thread used !");
				return -1;
			}
			pThreadData->m_iRunType = 1;
			pThreadData->m_pTask = (*it);
			pThreadData->resume();
		}
	}
	return 0;
}
