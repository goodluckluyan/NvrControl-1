//@file:C_HallList.h
//@brief: ������C_HallList��
//C_HallList:�ڲ���װ��ӰԺ��Ӱ���������ṩ��ӡ�ɾ�����޸�Ӱ���豸�Ȳ�����
//ͬʱ��װ��Ӱ�������豸�Ĳ�����
//@author:wangzhongping@oristartech.com
//dade:2012-07-12



 #ifndef HALL_LIST
 #define HALL_LIST
#include <string>
#include <list>
#include <pthread.h>
#include "../threadManage/C_CS.h"
#include "../C_constDef.h"
#include "DataManager.h"
#include "TMSSensor.h"
#include "C_Hall.h"


//using namespace std;

struct stConditionSwitch
{
	std::string strHallID;
	std::string strCond;
	int nVal;
	int nTriaggerCnt;
};


class C_HallList
{
public:
	C_HallList()
		:m_ptrDM(NULL)
		,m_ptrTMS(NULL)
	{
		pthread_cond_init(&cond,NULL);
	};
   
	
    ~C_HallList();

   	// ��ʼ������Ӱ����
	int Init(CTMSSensor * ptrTMS,bool bCheckOtherSMS = false);
   
	// ��ȡSMS����״̬
	bool GetSMSWorkState();

	// �л�sms
	bool SwitchSMS(bool bDelaySwitch,std::string strHallID,int &nState);


	// �ڱ�����������sms
	bool StartAllSMS(bool bCheckOtherSMSRun,std::vector<std::string>& vecHallid,bool bLocalHost=false);

	// �����������������л�
	int SwitchSMSByStdby(bool bDelaySwitch,std::string strHallID);

	// ��ȡ�ڱ������е�hallid
	void GetAllLocalRunHallID(std::vector<std::string>& vecHallID);

	// ��ȡ���е�hallid
	void GetAllRunHallID(std::vector<std::string> &vecHallID);

	// ��ȡ�ɱ����ӹ��������hallid
	void GetTakeOverSMS(std::vector<std::string> &vecHallID);

	// ��ȡ����������webservice�˿�
	bool GetSMSRunHost(std::string strHallID,std::string &strIP,int &nPort);

	// ��������ȴ��л�����
	bool AddCondSwitchTask(std::string strHallID,std::string strCond,int nVal);

	// ִ�������ȴ��л�����
	bool ProcessCondSwitchTask();

	// �ж��л�sms�����Ѿ������������д���
	bool IsHaveCondSwitchTask(std::string strHallID);

	// �رձ�����sms
	bool CloseSMS(std::string strHallID);

	// �ڱ�������sms
	bool StartSMS(std::string strHallID);

	// ������رմ�sms bSoC true������  false �ر� 
	bool StartOrCloseStdBySMS(bool bSoC,std::string strHallID);

	// �������ݿ�sms���е�λ��
	bool UpdateDataBase(std::string strHallID,int nPosition);

	// ��ȡsms������λ��
	bool GetSMSPosition(std::string strHallID,std::string &strIP,int& nPort);

	// �ı�sms����λ�� pos: 1���� 2���� ֻ�������Ż�����������
	bool ChangeSMSHost(std::string strHallID,int nPos);

	// ��ȡWebServiceIP
	void GetWebServiceIP(std::string &IP1,std::string &IP2);

private:

	// ���µ������ִ�нű�
	bool ExeShell_Fork(std::string strPath,std::string strShell);

	// ��ȡָ�����̵�pid
	int GetPID(std::string strName,std::vector<int>& vecPID);

	// ��ȡ���̵�����Ŀ¼
	bool GetPIDExeDir(int ,std::string &);

	// ͨ��service ���������Լ�
	bool RebootMyself();


    C_CS m_csHallCurState;//����m_mapHallCurState
	std::map<std::string,int> m_mapHallCurState;

	std::map<std::string,C_Hall *> m_mapHall;
    static C_HallList *m_pInstance; 
	std::string m_WebServiceLocalIP;
	std::string m_WebServiceOtherIP;
	CDataManager *m_ptrDM;
	CTMSSensor * m_ptrTMS;

	std::list<stConditionSwitch>  m_lstCondSwitch;
	C_CS m_csCondTaskLst;// ��������
	pthread_cond_t cond; 

	std::map<std::string,C_CS*> m_mapCS;// ���л��л��ͻ�ȡ״̬
	

};
#endif //HALL_LIST;
