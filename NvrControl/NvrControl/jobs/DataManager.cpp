#include<algorithm>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"../para/C_Para.h"
#include"../DataManager.h"
#include"../log/C_LogManage.h"
extern time_t g_tmDBSynch;
//extern int g_RunState;
#define  LOG(errid,msg)   C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_JOBS,0,errid,msg)
#define  LOGINF(msg)	  C_LogManage::GetInstance()->WriteLog(ULOG_INFO,LOG_MODEL_JOBS,0,0,msg)
#define  LOGINFFMT(fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_INFO,LOG_MODEL_JOBS,0,0,fmt,##__VA_ARGS__)
#define  LOGDEBFMT(fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_DEBUG,LOG_MODEL_JOBS,0,0,fmt,##__VA_ARGS__)
						

CDataManager *CDataManager::m_pinstance=NULL;
CDataManager::CDataManager()
{
	m_ptrInvoker = NULL;
	m_ptrDispatch = NULL;
	m_nOterHostFail = 0;
	time(&m_tmOtherHostFail);

	time(&m_tmCheckTMSNoRun);
	m_nCheckTMSNoRun = 0;

	m_nOtherMonitorState = -1;
	m_lSynch = 0;
	m_bSwitching=false;
	m_nSSDNum=0;
	m_nSataNum=0;
	m_nDBSynchState=-1;

	m_nOtherRaidStatus = -1;
	m_nLocalRaidStatus = -1;

	m_bRestoreSwitch = false;
}
CDataManager::~CDataManager()
{
}

// ��ʼ��
bool CDataManager::Init(void * vptr,int nSSDNum,int nSataNum)
{
	if(vptr != NULL)
	{
		m_ptrInvoker = vptr;
	}

	// ssd Ӳ������
	m_nSSDNum=nSSDNum;

	// sata Ӳ������
	m_nSataNum=nSataNum;

	return true;
}

// ��������������Ϣ
void CDataManager::SetEthBaseInfo(std::map<std::string,int> &mapEthBaseInfo)
{	
	m_csNet.EnterCS();
	std::map<std::string,int>::iterator it = mapEthBaseInfo.begin();
	for(;it != mapEthBaseInfo.end();it++)
	{
		EthStatus node;
		node.strName = it->first;
		node.nTaskType = it->second;
		m_mapEthStatus[it->first] = node;
	}
	m_csNet.LeaveCS();
}

// ����SMS������Ϣ
void CDataManager::SetSMSInfo(std::vector<SMSInfo> vecHall)
{
	m_csSMS.EnterCS();
	int nLen = vecHall.size();
	for(int i = 0 ;i < nLen ;i++)
	{
        SMSInfo & node = vecHall[i];
        LOGDEBFMT("id:(%s) role(%d) run(%d) pos(%d) stat(%d)",node.strId.c_str(),
                  node.nRole,node.stStatus.nRun,node.stStatus.nPosition,node.stStatus.nStatus);
		m_mapSmsStatus[node.strId] = node;
	}
	m_csSMS.LeaveCS();
}

// ���¸���ģ��ļ������
bool CDataManager::UpdateDevStat(std::map<int,DiskInfo> &mapdf)
{
	m_csDisk.EnterCS();
	m_mapdf = mapdf;
// 	if(m_mapdf.size()==0)
// 	{
// 		m_mapdf = mapdf;
// 	}
// 	else
// 	{
// 		std::map<int,DiskInfo>::iterator it = mapdf.begin();
// 		for(;it!=mapdf.end();it++)
// 		{
// 			int index = it->first;
// 			std::map<int,DiskInfo>::iterator fmit = m_mapdf.find(index);
// 			if(fmit!=m_mapdf.end())
// 			{
// 				DiskInfo &mdi = fmit->second;
// 				DiskInfo &di = it->second;
// 
// 				mdi.diskGroup=di.diskGroup;
// 				mdi.diskNumOfDrives=di.diskNumOfDrives;
// 				mdi.diskSize=di.diskSize;
// 				mdi.diskState=di.diskState;
// 
// 				// mapdf��m_mapdf�в��Ҷ�Ӧ�ĸ��£�û�ж�Ӧ�Ĳ����
// 				std::map<std::string ,struct DiskDriveInfo>::iterator fddit = di.diskDrives.begin();
// 				for(;fddit!=di.diskDrives.end();fddit++)
// 				{
// 					std::string strDSN=fddit->first;
// 					DiskDriveInfo &ddi=fddit->second;
// 					if(strDSN.empty())
// 					{
// 						continue;
// 					}
// 
// 					std::map<std::string ,struct DiskDriveInfo>::iterator fddmit = mdi.diskDrives.find(strDSN);
// 					if(fddmit!=mdi.diskDrives.end())
// 					{
// 						DiskDriveInfo &mddi = fddmit->second;
// 						mddi=ddi;
// 					}
// 					else
// 					{
// 						mdi.diskDrives[strDSN]=ddi;
// 					}
// 
// 				}
// 
// 				// m_mapdf��mapdf�в��Ҷ�Ӧ�ĸ��£�û�ж�Ӧ����Ϊ��offline
// 				std::map<std::string ,struct DiskDriveInfo>::iterator fddmit = mdi.diskDrives.begin();
// 				for(;fddmit!=mdi.diskDrives.end();fddmit++)
// 				{
// 					std::string strDSN=fddmit->first;
// 					DiskDriveInfo &mddi=fddmit->second;
// 					if(strDSN.empty())
// 					{
// 						continue;
// 					}
// 
// 					std::map<std::string ,struct DiskDriveInfo>::iterator fddit = di.diskDrives.find(strDSN);
// 					if(fddit!=di.diskDrives.end())
// 					{
// 						DiskDriveInfo &ddi = fddit->second;
// 						mddi=ddi;
// 					}
// 					else
// 					{
// 						mddi.driveFirmwareState="offline";
// 					}
// 
// 				}
// 
// 			}
// 		}
// 	}
	std::map<int,DiskInfo> df=m_mapdf;
	m_csDisk.LeaveCS();

	std::map<std::string ,struct DiskDriveInfo>mapSN;
	std::map<int,DiskInfo>::iterator it = df.begin();
	for(;it!=df.end();it++)
	{
		DiskInfo &di = it->second;
		std::map<std::string ,struct DiskDriveInfo>::iterator ddmit = di.diskDrives.begin();
		for(;ddmit!=di.diskDrives.end();ddmit++)
		{
			mapSN[ddmit->first] = ddmit->second;
		}	
	}
	int nSum=m_nSSDNum+m_nSataNum;
	for(int i=0 ;i < nSum ;i++)
	{
		char buf[16]={'\0'};
		snprintf(buf,16,"%d",i);
		std::map<std::string ,struct DiskDriveInfo>::iterator fit = mapSN.find(buf);
		if(fit == mapSN.end())
		{
			DiskDriveInfo node;
			node.driveSlotNum = buf;
			node.driveFirmwareState="bad";
			mapSN[buf]=node;
			LOGDEBFMT("----------------NOT FOUND(SN:%d)----------------",i);
		
		}
	}
	
	LOGDEBFMT("*****************Raid State************");
	std::map<int,DiskInfo>::iterator mit = df.begin();
	for(;mit != df.end();mit++)
	{
		DiskInfo &di = mit->second;
		
		LOGDEBFMT("diskGroup:%d",mit->first);
		LOGDEBFMT("diskSize:%s",di.diskSize.c_str());
		std::transform(di.diskState.begin(),di.diskState.end(),di.diskState.begin(),::tolower);
		LOGDEBFMT("diskState:%s",di.diskState.c_str());
		LOGDEBFMT("diskNumberOfDrives:%s",di.diskNumOfDrives.c_str());
		LOGDEBFMT("-------------------Detail--------------");
		std::map<std::string ,struct DiskDriveInfo>::iterator diit = di.diskDrives.begin();
		int i=0;
		for( ;diit != di.diskDrives.end();diit++,i++)
		{	
			DiskDriveInfo &ddi=diit->second;
			LOGDEBFMT("----------------%d----------------",i);
			LOGDEBFMT("dirveID:%s",ddi.driveID.c_str());
			LOGDEBFMT("dirveSlotNum:%s",ddi.driveSlotNum.c_str());
			LOGDEBFMT("dirveErrorCount:%s",ddi.driveErrorCount.c_str());
			LOGDEBFMT("dirveSize:%s",ddi.driveSize.c_str());
			std::transform(ddi.driveFirmwareState.begin(),
				ddi.driveFirmwareState.end(),ddi.driveFirmwareState.begin(),::tolower);
			LOGDEBFMT("dirveFirmwareState:%s",ddi.driveFirmwareState.c_str());
			LOGDEBFMT("dirveType:%s",ddi.driveType.c_str());
			LOGDEBFMT("dirveSpeed:%s",ddi.driveSpeed.c_str());
		}
		
	}
    LOGDEBFMT("---------------------------------------");
	
	
	std::vector<stError> vecRE;
	if(!CheckRaidError(vecRE,mapSN))
	{
		if(m_ptrDispatch)
		{
			m_ptrDispatch->TriggerDispatch(RAIDTask,vecRE);
		}
		m_nLocalRaidStatus = 1;
	}
	else
	{
		m_nLocalRaidStatus = 0;
	}
	
	return true;
}

// �����̳����Ƿ��д���
bool CDataManager::CheckRaidError(std::vector<stError> &vecErr,
	std::map<std::string ,struct DiskDriveInfo> &mapdf)
{
	bool bRet = true;
	
	std::map<std::string,DiskDriveInfo>::iterator it = mapdf.begin();
	for(;it != mapdf.end();it++)
	{
		DiskDriveInfo &di = it->second;
		if(di.driveFirmwareState.find("Online") == std::string::npos)
		{
			stError re;
			re.ErrorName = "driveFirmwareState";
			re.ErrorVal = "bad";
			re.nOrdinal = atoi(di.driveSlotNum.c_str());
			vecErr.push_back(re);
			bRet = false;
		}
	}
	
	return bRet;
}

// ��������״̬�������
bool CDataManager::UpdateNetStat(std::vector<EthStatus> &vecEthStatus)
{
	m_csNet.EnterCS();
	int nLen = vecEthStatus.size();
	std::string strTmp;
	for(int i = 0 ;i < nLen ;i++)
	{
		std::map<std::string,EthStatus>::iterator fit = m_mapEthStatus.find(vecEthStatus[i].strName);
		if(fit != m_mapEthStatus.end())
		{
			fit->second.nConnStatue = vecEthStatus[i].nConnStatue;
			fit->second.nRxSpeed = vecEthStatus[i].nRxSpeed;
			fit->second.nTxSpeed = vecEthStatus[i].nTxSpeed;
			char buf[128]={'\0'};
			snprintf(buf,128," %s[st:%d,RxSp:%8llu,TxSp:%8llu]",vecEthStatus[i].strName.c_str(),
				vecEthStatus[i].nConnStatue,vecEthStatus[i].nRxSpeed,vecEthStatus[i].nTxSpeed);
			strTmp+=buf;
			if((i+1)%4==0||(i+1)==nLen)
			{
				LOGDEBFMT("Local Eth%s",strTmp.c_str());
				strTmp="";
			}
			
		}
	}
	m_csNet.LeaveCS();

	std::vector<stError> vecErr;
	CheckEthError(vecErr);
	if(vecErr.size()>0)
	{
		if(m_ptrDispatch)
		{
			m_ptrDispatch->TriggerDispatch(ETH,vecErr);
		}
	}

	return true;
}

void CDataManager::CheckEthError(std::vector<stError> &vecErr)
{
	m_csNet.EnterCS();
	std::map<std::string,EthStatus>::iterator it = m_mapEthStatus.begin();
	for(;it!=m_mapEthStatus.end();it++)
	{
		EthStatus &estate = it->second;
		if(estate.nConnStatue == 0)
		{
			stError re;
			switch(estate.nTaskType)
			{
			case 1:
				re.ErrorName = "type1noconn";
				break;
			case 2:
				re.ErrorName = "type2noconn";
				break;
			case 3:
				re.ErrorName = "type3noconn";
				break;
			}
			
			re.ErrorVal = estate.strName;
			re.nOrdinal = 0;
			vecErr.push_back(re);
		}
	}
	m_csNet.LeaveCS();

}

//����SMS��״̬
/*
���ܳ��ֵĽ�ɫ������״̬
        ��ɫ��info.nRole��               ����״̬��info.stStatus.nRun��
		������MAINROLE��                     �������У�1��
		������STDBYROLE��                    ������У�2��
		�ӻ���MAINROLE��                     ������У�2��
		�ӻ���STDBYROLE��                    �������У�1��
		������TAKEOVERROLE��                 �������У�1��
		�ӻ���TAKEOVERROLE��                 �������У�1��
*/
bool CDataManager::UpdateSMSStat(std::string strHallID,int nState,std::string strInfo)
{
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo>::iterator it = m_mapSmsStatus.find(strHallID);
	if(it != m_mapSmsStatus.end())
	{
		SMSInfo &info = it->second;
		info.stStatus.nStatus = nState;
		std::string strRole = info.nRole==1?" main":info.nRole==3?"takeover":"slave";
		std::string strPos = info.stStatus.nRun == 1 ? "   local":"nonlocal";
		LOGDEBFMT("SMS:%s(%s:%s) Status:%3d  (%s)",strHallID.c_str(),
			strRole.c_str(),strPos.c_str(),nState,strInfo.c_str());
	}
	m_csSMS.LeaveCS();

	
	return true;
}

//����SMS��״̬
bool CDataManager::UpdateSMSStat(std::string strHallID,SMSInfo &stSMSInfo)
{
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo>::iterator it = m_mapSmsStatus.find(strHallID);
	if(it != m_mapSmsStatus.end())
	{
		SMSInfo &info = it->second;
        info = stSMSInfo;
        LOGDEBFMT("SMS:%s(role:%d run:%d status:%3d)",info.strId.c_str(),
            info.nRole,info.stStatus.nRun,info.stStatus.nStatus);
	}
	m_csSMS.LeaveCS();
	return true;
}


// ����TMS��״̬
bool CDataManager::UpdateTMSStat(int state)
{
	m_csTMS.EnterCS();
	m_nTMSState = state;
	m_csTMS.LeaveCS();

	std::vector<stError> vecRE;
	if(state == -1 && C_Para::GetInstance()->IsMain())
	{
		time_t tm;
		time(&tm);
		int nSec = tm-m_tmOtherHostFail;
		if(nSec > 5)
		{
			stError er;
			er.ErrorName = "state";
			er.ErrorVal = "norun";
			vecRE.push_back(er);

			if(m_ptrDispatch)
			{
				m_ptrDispatch->TriggerDispatch(TMSTask,vecRE);
			}
			time(&m_tmOtherHostFail);
		}
	}

	return true;
}

// ��ȡdisk������
bool CDataManager::GetDevStat(std::map<int,DiskDriveInfo> &df)
{
	m_csDisk.EnterCS();
	std::map<int,DiskInfo> mapdf = m_mapdf; 
	m_csDisk.LeaveCS();   

	std::map<int,DiskInfo>::iterator it = mapdf.begin();
	for(;it!=mapdf.end();it++)
	{
		DiskInfo &di = it->second;
		std::map<std::string ,struct DiskDriveInfo>::iterator ddmit = di.diskDrives.begin();
		for(;ddmit!=di.diskDrives.end();ddmit++)
		{
			int index = atoi(ddmit->first.c_str());
			df[index] = ddmit->second;
		}	
	}
	int nSum=m_nSSDNum+m_nSataNum;
	for(int i=0 ;i < nSum ;i++)
	{
		std::map<int ,struct DiskDriveInfo>::iterator fit = df.find(i);
		if(fit == df.end())
		{
			DiskDriveInfo node;
			char buf[16]={'\0'};
			snprintf(buf,16,"%d",i);
			node.driveSlotNum = buf;
			node.driveFirmwareState="bad";
			df[i]=node;
		}
	}
}

// ��ȡ����״̬
bool CDataManager::GetNetStat(std::map<std::string,EthStatus> &mapEthStatus)
{
	m_csNet.EnterCS();
	mapEthStatus = m_mapEthStatus;
	m_csNet.LeaveCS();
	return true;
}

// ��ȡSMS״̬
bool CDataManager::GetSMSStat(std::vector<SMSStatus>& vecSMSState)
{
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo> mapTmp = m_mapSmsStatus;
	m_csSMS.LeaveCS();

	std::map<std::string,SMSInfo>::iterator it = mapTmp.begin();
	for(;it != mapTmp.end();it++)
	{
		SMSStatus state;
		SMSInfo &info = it->second;
		//if(info.stStatus.nRun ==1)
		{
			state.nStatus = info.stStatus.nStatus;
			state.hallid = info.strId;
            state.nPosition = info.stStatus.nPosition;
            state.nRun = info.stStatus.nRun;
			state.strSPLUuid = info.stStatus.strSPLUuid;
			vecSMSState.push_back(state);
		}
	}
	
	return true;
}

// ��ȡSMS״̬
bool CDataManager::GetSMSStat(std::string strHallID,SMSInfo& smsinfo)
{
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo>::iterator it = m_mapSmsStatus.find(strHallID);
	if(it != m_mapSmsStatus.end())
	{
		smsinfo = it->second;
	}
	m_csSMS.LeaveCS();

	return true;
}

// ��ȡTMS��״̬
int CDataManager::GetTMSStat()
{
	return m_nTMSState;
}

// ��ȡ�Զ˵��ȵ�״̬
int CDataManager::GetOtherIMonitor()
{
	return m_nOtherMonitorState;
}

// ��ȡ�Զ�Raid��״̬
int CDataManager::GetOtherRaidStatus()
{
	return m_nOtherRaidStatus;
}

// ��ȡ�Զ�Raid��״̬
int CDataManager::GetLocalRaidStatus()
{
	return m_nLocalRaidStatus;
}


// ��ȡInvokerָ��
void * CDataManager::GetInvokerPtr()
{
	return m_ptrInvoker;
}

//���¶Զ�tms״̬
bool CDataManager::UpdateOtherTMSState(bool bRun,int nWorkState,int nState)
{
	LOGDEBFMT("Other TMS State:bRun:%d,nWorkState:%d,nState:%d",bRun,nWorkState,nState);
	return true;
}

//���¶Զ˵������״̬
bool CDataManager::UpdateOtherMonitorState(bool bMain,int nState,long lSynch)
{
	LOGDEBFMT("Other Monitor State:bMain:%d,nState:%d,lSynch:%lld",bMain,nState,lSynch);
	m_nOtherMonitorState = nState;

	int nRunState = GlobalStatus::GetInstinct()->GetStatus();
	// ֻ���ڸ�����ʱ���ܸ�ֵ���˳���������г�ͻ�ж�
	//if(0 == g_RunState && lSynch!=0 && (nState == TMPMAINROLE || nState == ONLYMAINROLE))
	if(0 == nRunState && lSynch!=0 && (nState == TMPMAINROLE || nState == ONLYMAINROLE))
	{
		m_lSynch = lSynch;
		return true;
	}
	
	// �Զ˻�״̬Ϊ0˵���Զ��������������ڽӹܡ��ָ��عܡ������ͻ
	if(0 == nState)
	{
       m_nOtherMonitorState = 0;
	   return true;
	}

	// ��ȡ״̬ʧ��
	if(-1 == nState && nRunState == 1)
	{
		time_t tm;
		time(&tm);
		int nSec = tm-m_tmOtherHostFail;
		if(nSec > 100)
		{
			m_nOterHostFail = 1;
			time(&m_tmOtherHostFail);
		}
		else
		{
			m_nOterHostFail++;
		}

		int nRole = C_Para::GetInstance()->GetRole();
		
		int nCnt=0;
		int nSmsSize=m_mapSmsStatus.size();
		std::map<std::string,SMSInfo>::iterator it=m_mapSmsStatus.begin();
		for(;it!=m_mapSmsStatus.end();it++)
		{
			if(it->second.stStatus.nRun==1)
			{
				nCnt++;
			}
		}

		bool bTrigger = (nCnt!=nSmsSize || (nRole != ONLYMAINROLE && nRole != TMPMAINROLE));
		if(m_nOterHostFail > 2 && bTrigger)
		{
			int nPerSec = nSec/m_nOterHostFail;
			char buf[16]={'\0'};
			snprintf(buf,16,"%d",nPerSec);
			
			LOGDEBFMT("Other Host Connection Fail(Interval Sec/PerCnt:%s=%d/%d)",buf,nSec,m_nOterHostFail);
			stError er;
			std::vector<stError> vecRE;
			er.ErrorName="connfailinterval";
			er.ErrorVal=buf;
			vecRE.push_back(er);
			if(m_ptrDispatch)
			{
				m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
			}
		}

		return true;
	}   

	

	// ����������nState��״̬Ϊ�Զ˻������Ľ�ɫ
	// ���˶�����
	if(C_Para::GetInstance()->IsMain() == bMain && bMain )
	{
		int nRunState = GlobalStatus::GetInstinct()->GetStatus();
		//����Ϊ����ʱ ������������ʱ����������ɫΪ1ʱ������ʱ���Ļر�
		//if(C_Para::GetInstance()->GetRole()==(int)TMPMAINROLE && g_RunState == 1)
		if(C_Para::GetInstance()->GetRole()==(int)TMPMAINROLE && nRunState == 1 && nState == MAINROLE)
		{	
			stError er;
			std::vector<stError> vecRE;
			er.ErrorName="mainback";
			er.ErrorVal="mainback";
			vecRE.push_back(er);
			if(m_ptrDispatch)
			{
				m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
			}
		}
		// ��������������������ʱ�����´���
		else if(nState == MAINROLE && nRunState == 1)
		{
			stError er;
			std::vector<stError> vecRE;
			er.ErrorName="brotherhood";
			er.ErrorVal="nostandby";
			vecRE.push_back(er);
			if(m_ptrDispatch)
			{
				m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
			}
		}
		else if(C_Para::GetInstance()->GetRole()==(int)ONLYMAINROLE &&  nRunState == 1 && nState == TMPMAINROLE)
		{
			//���ѻָ�
			stError er;
			std::vector<stError> vecRE;
			er.ErrorName="stdbyback";
			er.ErrorVal="stdbyback";
			vecRE.push_back(er);
			if(m_ptrDispatch)
			{
				m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
			}
		}
		
	}
	// ���˶��Ǳ�
	else if(C_Para::GetInstance()->IsMain() == bMain && !bMain && nRunState == 1)
	{
		stError er;
		std::vector<stError> vecRE;
		er.ErrorName="brotherhood";
		er.ErrorVal="nomain";
		vecRE.push_back(er);
		if(m_ptrDispatch)
		{
			m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
		}
	}
	else if(C_Para::GetInstance()->GetRole()==(int)ONLYMAINROLE && !bMain && nRunState == 1 && nState == STDBYROLE)
	{
		stError er;
		std::vector<stError> vecRE;
		er.ErrorName="stdbyback";
		er.ErrorVal="stdbyback";
		vecRE.push_back(er);
		if(m_ptrDispatch)
		{
			m_ptrDispatch->TriggerDispatch(IMonitorTask,vecRE);
		}
	}


// 	// ����һЩԭ�򣬿��ܻ��������������g_tmDBSynchû�лָ�0����������������ж�һ�¡�
// 	if(bMain && !C_Para::GetInstance()->IsMain() && g_tmDBSynch != 0)
// 	{
// 		g_tmDBSynch=0;
// 	}
	return true;
}

//���¶Զ�sms״̬
bool CDataManager::UpdateOtherSMSState(std::vector<SMSStatus> &vecSMSStatus)
{
	m_csOtherSMS.EnterCS();
	int nLen = vecSMSStatus.size();
	std::string strTmp;
	for(int i = 0 ;i < nLen ;i++)
	{
		std::string pos ;
		int bMain = C_Para::GetInstance()->IsMain();
		if(bMain)
		{
			pos = vecSMSStatus[i].nRun==1 ? "slave":"main";
		}
		else
		{
			pos = vecSMSStatus[i].nRun==1 ? "main":"slave";
		}
		char buf[64]={'\0'};
        snprintf(buf,64," %s:[po:%5s,st:%3d,run:%3d]",vecSMSStatus[i].hallid.c_str(),
            pos.c_str(),vecSMSStatus[i].nStatus,vecSMSStatus[i].nRun);
		strTmp+=buf;
// 		if(vecSMSStatus[i].nRun == 0)
// 		{
// 			continue;
// 		}

		std::string strID = vecSMSStatus[i].hallid;
		
		std::map<std::string,SMSInfo>::iterator it = m_mapOtherSMSStatus.find(strID);
		if(it != m_mapOtherSMSStatus.end())
		{
			SMSInfo& node = it->second;
			node.stStatus = vecSMSStatus[i];
		}
		else
		{
			m_mapOtherSMSStatus[strID].strId=vecSMSStatus[i].hallid;
			m_mapOtherSMSStatus[strID].stStatus=vecSMSStatus[i];
		}
		
		if((i+1)%4==0 || (i+1) == nLen)
		{
			LOGDEBFMT("Other Host SMS%s",strTmp.c_str());
			strTmp="";
		}
	}
	m_csOtherSMS.LeaveCS();
	

	if(nLen > 0)
	{
		time(&m_tmUpdateOSMS);
	}

	int nRunState = GlobalStatus::GetInstinct()->GetStatus();

	//�ڽӹܺͻָ��ӹ�״̬�����ڴ����ͻʱ�������жϺͽ����ͻ
	//if(g_RunState==2 || g_RunState==3 )
	if(nRunState==2 || nRunState==3 )
	{
		LOGDEBFMT("localhost run status: %d, not conflict check",nRunState);
		return true;
	}

	//ֻ�����������жϺͽ����ͻ
	if(C_Para::GetInstance()->GetRole()!=(int)MAINROLE)
	{
		return true;
	}

	// ���Է���ɫ��Ϊ������ʱ������δ֪�ʹ��ڻָ��ͳ�ͻ״̬�����ܽ��г�ͻ�ж�
	if(m_nOtherMonitorState != 3 )
	{
		LOGDEBFMT("slave host run status: %d(not eq 3), not conflict check",m_nOtherMonitorState);
		return true;
	}


	// �жϳ�ͻ
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo>maptmp = m_mapSmsStatus;
	m_csSMS.LeaveCS();

	int nStdbyRun =0;
	std::vector<ConflictInfo> vecConflict;
	m_csSwitch.EnterCS();
	int bSwitching = m_bSwitching;
	m_csSwitch.LeaveCS();

	m_csOtherSMS.EnterCS();
	std::map<std::string,SMSInfo> mapOtherSMSStatus = m_mapOtherSMSStatus;
	m_csOtherSMS.LeaveCS();

	if(!bSwitching)//���л��ڼ䲻���г�ͻ�ж�
	{
		std::map<std::string,SMSInfo>::iterator it = mapOtherSMSStatus.begin();
		for(;it != mapOtherSMSStatus.end();it++)
		{
			SMSInfo &Other = it->second;
			std::map<std::string,SMSInfo>::iterator fit= maptmp.find(Other.strId);
			if(fit == maptmp.end())
			{
				continue;	
			}

			if(Other.stStatus.nRun == 1)
			{
				nStdbyRun++;
			}

			// �����������sms
			if(Other.stStatus.nRun == 1 && fit->second.stStatus.nRun == 1 )
			{
				ConflictInfo ci;
				ci.nMainState=fit->second.stStatus.nStatus;
				ci.nStdbyState=Other.stStatus.nStatus;
				ci.nType = 1;
				ci.strHallID=Other.strId;
				time(&ci.tmTime);
				vecConflict.push_back(ci);
			}

			// ��û���������sms, �ݲ������߶����������д������л�ʱ��������������
			// Ҫ�������߶�û�������������Ҫ���˵��л�ʱ���ֵ�������ȽϷ�����Ҫ����ϱ��Żᴦ��(����)����Ҫ���л�ʱ֪ͨ������
			// ��Ҫ���г�ͻ��⡣��δ����
// 			if(Other.stStatus.nRun == 2 && fit->second.stStatus.nRun == 2)
// 			{
// 				ConflictInfo ci;
// 				ci.nMainState=fit->second.stStatus.nStatus;
// 				ci.nStdbyState=Other.stStatus.nStatus;
// 				ci.nType = 2;
// 				ci.strHallID=Other.strId;
// 				ci.tmTime=time();
// 				vecConflict.push_back(ci);
// 			}
		}
	}


	int nMainRun=0;
	std::map<std::string,SMSInfo>::iterator mit = maptmp.begin();
	for(;mit!=maptmp.end();mit++)
	{
		if(mit->second.stStatus.nRun==1)
		{
			nMainRun++;
		}
	}

	nLen = vecConflict.size();
	for(int i=0;i<nLen;i++)
	{
		vecConflict[i].nStdbySMSSum=nStdbyRun;
		vecConflict[i].nMainSMSSum=nMainRun;
//		m_maplstConfilict[vecConflict[i].strHallID].push_back(vecConflict[i]);
	}

// 	vecConflict.clear();
// 	CheckConfile(vecConflict);
	if(vecConflict.size()>0 && m_ptrDispatch!=NULL)
	{
		m_ptrDispatch->AddConflictInfo(vecConflict);
		stError er;
		std::vector<stError> vecRE;
		er.ErrorName="smsconflict";
		er.ErrorVal="smsconflict";
		vecRE.push_back(er);
		m_ptrDispatch->TriggerDispatch(SMSTask,vecRE);

		GlobalStatus::GetInstinct()->SetStatus(3);// ����ȫ��״̬Ϊ�����ͻ״̬
		//g_RunState=3; // ����ȫ��״̬Ϊ�����ͻ״̬

	}

	return true;
}


time_t CDataManager::GetOtherSMSstatus(std::vector<SMSStatus> &vecSMSStatus )
{
	m_csOtherSMS.EnterCS();
	std::map<std::string,SMSInfo>::iterator it = m_mapOtherSMSStatus.begin();
	for(;it != m_mapOtherSMSStatus.end();it++)
	{
		SMSStatus status;
		status=it->second.stStatus;
		vecSMSStatus.push_back(status);
	}
	m_csOtherSMS.LeaveCS();

	return m_tmUpdateOSMS;
}

// ���˼�⵽�ĳ�ͻ����ֹ��
bool CDataManager::CheckConfile(std::vector<ConflictInfo> &vecConflict)
{
	std::map<std::string,std::list<ConflictInfo> >::iterator it = m_maplstConfilict.begin();
	while(it!=m_maplstConfilict.end())
	{
		int nLen = it->second.size();
		if(nLen<3)
		{
			it++;
			continue;
		}
		
		
		int nInterval = it->second.back().tmTime-it->second.front().tmTime;
		int nSmsCheckSec = C_Para::GetInstance()->m_nOtherSMSCheckDelay;
		if(nInterval < nSmsCheckSec*(nLen+1))
		{
			vecConflict.push_back(it->second.front());
			m_maplstConfilict.erase(it++);
			continue;
		}
		else
		{
			it->second.pop_front();
		}
		it++;
	}
}

//���¶Զ�raid״̬
bool CDataManager::UpdateOtherRaidState(int nState,int nReadSpeed,
										int nWriteSpeed,std::vector<int> &vecDiskState)
{
	std::string raidinfo;
	for(int i=0;i<vecDiskState.size();i++)
	{
		char buf[8]={'\0'};
		snprintf(buf,sizeof(buf)," %d:%d ",i,vecDiskState[i]);
		raidinfo+=buf;
	}
	LOGDEBFMT("Other Raid:State:%d,RS:%d,WS:%d Raid[%s]",nState,nReadSpeed,nWriteSpeed,raidinfo.c_str());
	m_nOtherRaidStatus = nState;
	return true;
}

//���¶Զ�eth״̬
bool  CDataManager::UpdateOtherEthState(std::vector<EthStatus> &vecEthStatus)
{
	std::string strTmp;
	int nlen = vecEthStatus.size();
	for(int i = 0 ;i < nlen ;i++)
	{
		char buf[36]={'\0'};
		EthStatus &node = vecEthStatus[i];
		snprintf(buf,36," %s:[st:%2d,sp:%8d]",node.strName.c_str(),node.nConnStatue,node.nRxSpeed);
		strTmp += buf;
		if((i+1)%4==0 || (i+1) == nlen)
		{
			LOGDEBFMT("Other NetCard:%s",strTmp.c_str());
			strTmp="";
		}
	}
	
	return true;
}


//������Ϣ��ӡtms״̬
void CDataManager::PrintTMSState()
{
	LOGDEBFMT("TMS Current State:");
	LOGDEBFMT("bRun:%d",m_nTMSState);
}


//������Ϣ��ӡraid״̬
void CDataManager::PrintDiskState()
{
	m_csDisk.EnterCS();
	std::map<int,DiskInfo> &mapdf = m_mapdf;
	m_csDisk.LeaveCS();

	LOGDEBFMT("*****************Raid State************");
	std::map<int,DiskInfo>::iterator it = mapdf.begin();
	for(;it != mapdf.end();it++)
	{
		DiskInfo &df = it->second;

		LOGDEBFMT("diskGroup:%d",it->first);
		LOGDEBFMT("diskSize:%s",df.diskSize.c_str());
		std::transform(df.diskState.begin(),df.diskState.end(),df.diskState.begin(),::tolower);
		LOGDEBFMT("diskState:%s",df.diskState.c_str());
		LOGDEBFMT("diskNumberOfDrives:%s",df.diskNumOfDrives.c_str());
		LOGDEBFMT("-------------------Detail--------------");
		std::map<std::string ,struct DiskDriveInfo>::iterator it = df.diskDrives.begin();
		int i=0;
		for(;it != df.diskDrives.end();it++,i++)
		{	
			struct DiskDriveInfo &ddi=it->second;
			LOGDEBFMT("----------------%d----------------",i);
			LOGDEBFMT("dirveID:%s",ddi.driveID.c_str());
			LOGDEBFMT("dirveSlotNum:%s",ddi.driveSlotNum.c_str());
			LOGDEBFMT("dirveErrorCount:%s",ddi.driveErrorCount.c_str());
			LOGDEBFMT("dirveSize:%s",ddi.driveSize.c_str());
			std::transform(ddi.driveFirmwareState.begin(),
				ddi.driveFirmwareState.end(),ddi.driveFirmwareState.begin(),::tolower);
			LOGDEBFMT("dirveFirmwareState:%s",ddi.driveFirmwareState.c_str());
			LOGDEBFMT("dirveType:%s",ddi.driveType.c_str());
			LOGDEBFMT("dirveSpeed:%s",ddi.driveSpeed.c_str());
		}

	}
	LOGDEBFMT("---------------------------------------");
}

//������Ϣ��ӡsms״̬
void CDataManager::PrintSMSState()
{
	m_csSMS.EnterCS();
	std::map<std::string,SMSInfo>maptmp = m_mapSmsStatus;
	m_csSMS.LeaveCS();

	std::map<std::string,SMSInfo>::iterator it = maptmp.begin();
	for(;it != maptmp.end(); it++)
	{
		SMSInfo &info = it->second;
		LOGDEBFMT("hallid:%s",info.strId.c_str());
		LOGDEBFMT("SMS state:%d",info.stStatus.nStatus);
	}
}

//������Ϣ��ӡeth״̬
void CDataManager::PrintEthState()
{
	m_csNet.EnterCS();
	std::map<std::string,EthStatus> mapTmp = m_mapEthStatus;
	m_csNet.LeaveCS();

	std::map<std::string,EthStatus>::iterator it = mapTmp.begin();
	for(;it != mapTmp.end(); it++)
	{
		EthStatus &node = it->second;
		LOGDEBFMT("EthName:%s",node.strName.c_str());
		LOGDEBFMT("TaskType:%d",node.nTaskType);
		LOGDEBFMT("ConnState:%d",node.nConnStatue);
		LOGDEBFMT("Speed:%d",node.nRxSpeed);
	}
}


bool  CDataManager::UpdateOtherSwitchState(int nSwitch1State,int nSwitch2State)
{
	//LOGDEBFMT("Other Switch nSwitch1State:%d,nSwitch2State:%d",nSwitch1State,nSwitch2State);
	return true;
}

bool  CDataManager::UpdateOtherSpeedLmtState(bool bEnableIngest,int nSpeedLimit)
{
	//LOGDEBFMT("Other SpeedLmt bEnableIngest:%d,nSpeedLimit:%d",bEnableIngest,nSpeedLimit);
	return true;
}

bool  CDataManager::UpdateOtherSMSEWState(int nState,std::string  strInfo,std::string  strHall)
{
	//LOGDEBFMT("Other SMSEW nState:%d,strInfo:%s,strHall:%s",nState,strInfo.c_str(),strHall.c_str());
	return true;
}

bool CDataManager::GetDBSynchStatus(int &state)
{
	state = m_nDBSynchState;
	return true;
}

