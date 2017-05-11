//@file:C_Task.cpp
//@brief: ...
//@author: wangzhongping@oristartech.com
//date: 2012-05-23

#include "C_Task.h"
#include "../para/C_RunPara.h"
C_Task::C_Task()
{
	m_iCommandNumber = 0;
	m_iTaskState = TASK_IDLE_STATE;
	m_pPara = NULL;
	m_iStartTime = -1;
	m_ptrInvoker = NULL;
	m_emTaskType = NULL_TASK;
}

C_Task::C_Task(CInvoke * ptr)
{
	m_iCommandNumber = 0;
	m_iTaskState = TASK_IDLE_STATE;
	m_pPara = NULL;
	m_iStartTime = -1;
	m_ptrInvoker = ptr;
	m_emTaskType = NULL_TASK;
}


C_Task::~C_Task()
{
}

/*******************************************************************************
* �������ƣ�	IsIdle
* ����������	��ȡ����״̬
* ���������	
* ���������	
* �� �� ֵ��	����״̬��
* ����˵����	��� m_iTaskState ���Ϊ TASK_IDLE_STATE�� ��m_iTaskState
				����ΪTASK_NO_STATE ����ֵΪTASK_IDLE_STATE ʱ����ʾ���óɹ�����Ϊʧ�ܡ�
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
TASK_STATE C_Task::IsIdle()
{
	m_CS.EnterCS();
	TASK_STATE oldState = m_iTaskState;
	if(m_iTaskState == TASK_IDLE_STATE)
	{
		m_iTaskState = TASK_LOCKED_STATE;
	}
	m_CS.LeaveCS();
	return oldState;
}

/*******************************************************************************
* �������ƣ�	ISDelete
* ����������	��������Ϊɾ��״̬
* ���������	
* ���������	
* �� �� ֵ��	����״̬
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
TASK_STATE C_Task::ISDelete()
{
	
	TASK_STATE oldState = TASK_IDLE_STATE;
	m_CS.EnterCS();

	oldState = m_iTaskState;

	if(oldState != TASK_RUNNING_STATE)
	{
		ReInit();
	}
	else
	{
		m_iTaskState = TASK_DELETE_STATE;
	}

	m_CS.LeaveCS();

	return oldState;		
}


/*******************************************************************************
* �������ƣ�	IsEnableRun
* ����������	�ж������Ƿ��������
* ���������	
* ���������	
* �� �� ֵ��	����ֵ0Ϊ���������ɹ����� ����0Ϊ������������
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
int C_Task::IsEnableRun(int iCurTime)
{
	if(m_iCommandNumber == 0)
	{
		return 1;
	}

	if(m_iTaskState == TASK_NO_STATE && m_emTaskType == TIME_TASK 
		&& m_iStartTime <= iCurTime)
	{
		m_iTaskState = TASK_RUNNING_STATE;
		return 0;
	}

	if(m_iTaskState == TASK_NO_STATE && m_emTaskType == ONCE_TASK
		&& m_iStartTime <= iCurTime)
	{
		m_iTaskState = TASK_RUNNING_STATE;
		return 0;
	}

	if(m_iTaskState == TASK_NO_STATE && m_emTaskType == ALWAYS_TASK)
	{
		m_iTaskState = TASK_RUNNING_STATE;
		return 0;
	}

	return 1;
}

// ��ȡ������
TASK_STATE C_Task::GetTaskState()
{
	return m_iTaskState;
}

// m_iTaskState �ڷ� TASK_IDLE_STATE ���Ե��ô˺�����
void C_Task::SetTaskState(TASK_STATE newState)
{
	m_iTaskState = newState;
}

// ����������
void C_Task::SetCommandNumber(int nCmd)
{
	m_iCommandNumber = nCmd;
}


/*******************************************************************************
* �������ƣ�	ReInit
* ����������	�����������Ͷ���������س�ʼ��
* ���������	
* ���������	
* �� �� ֵ��	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
void C_Task::ReInit(int nDelaySec)
{
	if(TIME_TASK == m_emTaskType && nDelaySec != 0)
	{
		C_RunPara *pRunPara = C_RunPara::GetInstance();
		m_iStartTime  = pRunPara->GetCurTime() + nDelaySec;
		m_iTaskState = TASK_NO_STATE;
	}
	else if( ONCE_TASK == m_emTaskType && 0 == nDelaySec )
	{
		m_iTaskState = TASK_LOCKED_STATE;
		m_iCommandNumber = 0;
		m_iStartTime = -1;
		m_iTaskState = TASK_IDLE_STATE;
		m_emTaskType = NULL_TASK;
	}
	else if( ALWAYS_TASK == m_emTaskType)
	{
		m_iTaskState = TASK_NO_STATE;
	}
	
}

 
/*******************************************************************************
* �������ƣ�	Exec
* ����������	����ִ�У��ɲ�ͬ��ģ�����
* ���������	
* ���������	
* �� �� ֵ��	0Ϊ�ɹ���-1Ϊʧ�ܣ�2Ϊû����Ӧ����������
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
int  C_Task::Exec()
{
	int nRet = 0;
	if(GetTaskState() == TASK_DELETE_STATE)
	{
		ReInit();
		return 0;
	}
#if 0
	if(m_ptrInvoker == NULL)
	{
		return -1;
	}

	SetTaskState(TASK_RUNNING_STATE);
	nRet = m_ptrInvoker->Exec(m_iCommandNumber,m_pPara);
#endif
	ExeFinal();
	return nRet;
}

/*******************************************************************************
* �������ƣ�	ExeFinal
* ����������	ִ�н�����Ĵ���Ϊ�´�������׼��
* ���������	
* ���������	
* �� �� ֵ��	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-09-01					����
*******************************************************************************/
void C_Task::ExeFinal()
{
#if 0
	C_RunPara *pRunPara = C_RunPara::GetInstance();
	switch(m_emTaskType)
	{
	case TIME_TASK:
		{
			int nDelay = m_ptrInvoker->GetCheckDelay(m_iCommandNumber);
			ReInit(nDelay);
			SetTaskState(TASK_NO_STATE);
		}
		break;
	case ONCE_TASK:
	case ALWAYS_TASK:
		ReInit();
		break;
	}

	if(GetTaskState() == TASK_DELETE_STATE)
	{
		ReInit();
	}
	
#endif	
}
