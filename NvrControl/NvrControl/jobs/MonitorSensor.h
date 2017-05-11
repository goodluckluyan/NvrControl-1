//@file:MonitorSensor.h
//@brief: ��һ̨���ȳ����״̬��⡣
//@author:luyan@oristartech.com
//date:2014-09-17

#ifndef MONITORSENSOR_INC
#define MONITORSENSOR_INC

#include "DataManager.h"
#include "../threadManage/C_CS.h"
#include <map>
#include "parser_xml.h"


class CMonitorSensor
{
public:
	CMonitorSensor();

	~CMonitorSensor();

	// ��ʼ��
	bool Init(std::string strIP,int nPort);
	
	// ��ȡ��һ̨�����ĵ��ȳ����״̬
	bool GetOtherMonitorState(int nStateType,bool bNoticeDM = true);

	// ������һ�˽ӿ���������ͬ�����
	bool SetOtherDBSynch(std::string dbsynch);

	// ��������SetOtherDBSynch����
	int Parser_SetOtherDBSynch(std::string &content,int &nRet);

	// ���ñ����������ӿ�
	bool SlaveReboot(int nType,int &nState,std::string strDesc);

	// ���ñ����Ķ�ʱ�����ӿڣ�ֻ��Ϊ���ڱ�����¼timer.xml�ļ�
	bool InvokeSlaveTimingReboot(int nDay,int nWeek,int nHour,int nMinute,int nRepeatType,
		int nRepeatCnt,int &nState,std::string &strDesc);


	bool FixRebootXmlAttribute(std::string strAttr ,int nValue);

	int ReadRebootTask(int &nEnable,int& nDay,int& nWeek,int& nHour,int& nMinute,int &nType,int &nRepeatCnt,int &nExecnt);

	// ������xml�ļ�
    bool WriteTimerFile(int nDay,int nWeek,int nHour,int nMinute,int nRepeatType,int nRepeatCnt,int &nState,std::string &strDesc);

	int WriteXmlFile(xercesc::DOMDocument * ptrDoc,std::string xmlFile);

	bool ParseAskAboutSlaveRestoreSwitch(std::string &retXml,int &nState);

	bool AskAboutSlaveRestoreSwitch(int &nIsRestoreSwitch);


private:

	// ��ȡhttp�е�xml 
	int GetHttpContent(const std::string &http, std::string &response);

	// �ѵ���xml����http��ʽ���͵�����˲����շ���xml
	int SendAndRecvResponse(const std::string &request, std::string &response, int delayTime=2);

	// ����webservice�ӿ�
	int InvokerWebServer(std::string &xml,std::string &strResponse);

	// ����Monitor״̬
	bool ParseOtherMonitorState(std::string &retXml,bool &bMain,int &nState ,long &lSynch);

	// ����TMS״̬
	bool ParseOtherMonitorTMSState(std::string &retXml,bool &bRun,int &nWorkState,int &nState);

	// ����SMS״̬
	bool ParseOtherMonitorSMSState(std::string &retXml,std::vector<SMSStatus>& vecSMSStatus);

	// �������̳���״̬
	bool ParseOtherMonitorRaidState(std::string &retXml,int &nState,int &nReadSpeed,int &nWriteSpeed,
		std::vector<int> &vecDiskState);

	// ����SMS�����쳣״̬
	bool ParseOtherMonitorSMSEWState(std::string &retXml,int &nState,
		std::string & strInfo, std::string &strHall);

	// �����ٶ�����״̬
	bool ParseOtherMonitorSpeedLmtState(std::string &retXml,bool &bEnableIngest,int &nSpeedLimit);

	// ����������״̬
	bool ParseOtherMonitorSwitchState(std::string &retXml,int &nSwitch1State,int & nSwitch2State);

	// ��������״̬
	bool ParseOtherMonitorEthState(std::string &retXml,std::vector<EthStatus> &vecEthStatus);

	// ������������״̬
	bool ParseSlaveRebootState(std::string &retXml,int &nState,std::string &strDesc);
	
private:
	std::string m_strURI;
	std::string m_strIP;
	int m_nPort;
	CDataManager * m_ptrDM;

	std::map<int,std::string> m_mapStateType;
	C_CS m_csMap;
};

#endif