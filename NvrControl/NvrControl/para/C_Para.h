//@file:C_Para.h
//@brief: ������C_Para��
//C_Para:��ȡϵͳ�����ļ��еĸ���������
//@author:wangzhongping@oristartech.com
//dade:2012-07-12


#ifndef _TMS20_PARA
#define _TMS20_PARA
#include <string>
#include <pthread.h>
#include "../threadManage/C_CS.h"
using namespace std;

// ������ɫ������ֻ��MAINROLEһ��״̬��������STDBYROLE��TMPMAINROLE����״̬
enum enHOSTROLE{MAINROLE = 1,ONLYMAINROLE=2,STDBYROLE = 3,TMPMAINROLE = 4};
class C_Para
{
public:
 //method:
    static C_Para* GetInstance();
	static void DestoryInstance();
    ~C_Para();
    //��ȡ���ò�����
    int ReadPara();

	bool IsMain();
	int GetRole();
	bool SetRoleFlag(int nRole);
protected:
     C_Para();
public:    
//Property:  
	//���ݿ��������IP  
    string m_strDBServiceIP;

   	//���ݿ�������Ķ˿ں�
    unsigned short m_usDBServicePort;

    //���ݿ��������½���û�����
    string m_strDBUserName;

    //���ݿ��������½���û�����
    string m_strDBPWD;
    string m_strDBName;

    //ϵͳ�̳߳����̵߳ĸ�����
    unsigned int m_uiThreadCount;

	//��־���·��
    string m_strLogPath;

	//�����������ȳ���
	enHOSTROLE m_enRole ;

	//����webservice����򿪵Ķ˿�
	//std::string m_strWebServiceIP;
	int m_nWebServicePort;

	//���ص��ȳ����IP
	std::string m_strLIP;

	//�Զ˵ĵ��ȳ����IP
	std::string m_strOIP;

	//�Զ˵ĵ��ȳ���Ķ˿�
	int m_nOPort ;

	//TMS��Webservice�˿�
	int m_nTMSWSPort;

	//�Զ˵ĵ��ȳ���wsdl��URI
	std::string m_strOURI;

	//tms����Ŀ¼
	std::string m_strTMSPath;

	//tomcat����Ŀ¼
	//std::string m_strTOMCATPath;

	//��Ŀ¼
	std::string m_strInipath;

	//����sms�ķ�ʽ��1Ϊͬһ�ն�����  2Ϊ���ն�����
	int m_nStartSMSType ;

	//�ȴ��Զ˵������������ⳬʱʱ��,��λ��
	int m_nTimeOutWaitOtherIMonitor;

	//д��־����
	int m_nWirteLogLevel;

	// ������״̬ʱ����
	int m_nDiskCheckDelay;

	// �������״̬ʱ����
	int m_nEthCheckDelay;

	// �������״̬ʱ����
	int m_nHallListCheckDelay;

	// �������״̬ʱ����
	int m_nTMSCheckDelay;

	// ���Զ˵��ȳ���״̬ʱ����
	int m_nOtherMonitorCheckDelay;

	// ���Զ�TMS״̬ʱ����
	int m_nOtherTMSCheckDelay;

	// ���Զ�SMS״̬ʱ����
	int m_nOtherSMSCheckDelay;

	// ���Զ˴���״̬ʱ����
	int m_nOtherRAIDCheckDelay;

	// ���Զ�����״̬ʱ����
	int m_nOtherEthCheckDelay;

	// ���Զ˽�����ʱ����
	int m_nOtherSwitchCheckDelay;

	// ���Զ��ٶ�����ʱ����
	int m_nOtherSpeedLmtCheckDelay;

	// ���Զ��쳣״̬ʱ����
	int m_nOtherEWCheckDelay;

	// SSD RAID����
	int m_nSSD_Raid_Num;

	// Sata RAID����
	int m_nSATA_Raid_Num;

    // ���ݿ�ͬ�����
	int m_nDBSynchCheckDelay;
private:
    static C_Para *m_pInstance;
	pthread_rwlock_t m_rwlk_main;
};


class GlobalStatus
{
public:
	static GlobalStatus *GetInstinct()
	{
		return m_globalstatus;
	}

	static void DestoryInstinct()
	{
		if(m_globalstatus)
		{
			delete m_globalstatus;
			m_globalstatus = NULL;
		}
	}

	~GlobalStatus()
	{
		pthread_mutex_destroy(&m_mutx);
		pthread_cond_destroy(&m_cond);
		
	}

	int GetStatus()
	{
		pthread_mutex_lock(&m_mutx);
		int nRet = m_RunState;
		pthread_mutex_unlock(&m_mutx);
		return nRet;
	}

	int SetStatus(int stat)
	{
		int nRet = 0;
		if(stat == 1)
		{
			pthread_mutex_lock(&m_mutx);
			m_RunState = 1;
			pthread_mutex_unlock(&m_mutx);
			pthread_cond_signal(&m_cond);
		}
		else
		{
			// ״̬��Ϊ������״̬ʱ��Ҫ��ǰ״̬Ϊ����״̬������ȴ���
			pthread_mutex_lock(&m_mutx);
			while(m_RunState != 1)
			{
				pthread_cond_wait(&m_cond,&m_mutx);
			}
			m_RunState = stat;
			pthread_mutex_unlock(&m_mutx);
		}
		

	}

	C_CS m_mutxSignal;
private:
	GlobalStatus()
	{
		pthread_mutex_init(&m_mutx,NULL);
		pthread_cond_init(&m_cond,NULL);
		m_RunState = 0;

	}


	int m_RunState;// 0Ϊ����������1Ϊ�������� 2Ϊ�ڽӹܻ�ָ��ӹܽ׶� 3Ϊ�����ͻ�׶�
	static GlobalStatus * m_globalstatus;
	pthread_mutex_t m_mutx;
	pthread_cond_t m_cond;

	
};
#endif //_TMS20_PARA
