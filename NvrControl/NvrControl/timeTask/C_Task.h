//@file:C_Task.h
//@brief: C_Task �����˶�ʱ��������ݽṹ�Ͳ�����
//@author: wangzhongping@oristartech.com
//date: 2012-05-23

#ifndef TMS20_TASK
#define TMS20_TASK

#include "../C_constDef.h"
#include "../threadManage/C_CS.h"
//#include "../jobs/Invoke.h"
#include <list>
#include <string>
#include "../log/C_LogManage.h"



class C_Task
{
public:
	C_Task();
	C_Task(class CInvoke * ptr);
	~C_Task();

	TASK_STATE IsIdle();

	TASK_STATE ISDelete();

	//����ֵ0Ϊ���������ɹ����� ����0Ϊ��������������
	int IsEnableRun(int iCurTime);

	TASK_STATE GetTaskState();

	//m_iTaskState �ڷ� TASK_IDLE_STATE ���Ե��ô˺�����
	void SetTaskState(TASK_STATE newState);
	
	void SetCommandNumber(int nCmd);
	
	void ReInit(int nDelaySec = 0);

	int Exec();

	void ExeFinal();

	int GetCommandNumber()
	{
		return  m_iCommandNumber;
	}
public:
	// Ҫִ�еĲ�����ţ�
	int m_iCommandNumber;

	// Ҫִ�еĲ����Ĳ�����
	void * m_pPara;

	// ��������
	enum TASK_TYPE m_emTaskType;


	
	// ����ʼʱ�䣬�����ڶ�ʱ����
	int m_iStartTime;

	//����״̬ 
	TASK_STATE m_iTaskState; 

private:
	C_CS m_CS;

	class CInvoke * m_ptrInvoker;


};
#endif //TMS20_TASK

