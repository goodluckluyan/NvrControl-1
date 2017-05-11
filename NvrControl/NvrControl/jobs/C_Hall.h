//@file:C_Hall.h
//@brief: ������C_Hall��
//C_Hall����װ��Ӱ����sms��ز�����
//@author:luyan@oristartech.com
//dade:2012-07-12

#ifndef HALL_DEFINE
#define HALL_DEFINE


//#include "timeTask/C_TaskList.h"
#include <pthread.h>
#include <iostream>
#include <string>
#include "parser_xml.h"
#include "../C_constDef.h"
#include "../utility/C_HttpParser.h"
#include "../utility/C_TcpTransport.h"
enum enRUNTYPE{MAINRUNTYPE=1,STDBYRUNTYPE=2,TAKEOVERRUNTYPE=3};
class C_Hall
{

 public:   
	 C_Hall(SMSInfo &stSms);
	 ~C_Hall();

public:
	// ��ʼ��
	SMSInfo & Init(bool bRun,int nPID = 0);

	 // ��ȡSMS����״̬
	int  GetSMSWorkState( int &state, std::string &info);

	 // ��ȡSMS����״̬,ָ��IP
	int GetSMSWorkState(std::string strIp,int nPort,int &state,std::string &info);
	
	// ����SMS
	bool StartSMS(int &nPid,bool bLocalHost=false);

	// �ر�SMS
	int ShutDownSMS();

	// �ı�SMS������������Ϣ	
	SMSInfo& ChangeSMSHost(std::string strIP,int nRunType,bool bLocalRun);

	// ��ȡhallid
	std::string GetHallID()
	{
		return m_SMS.strId;
	}

	// �Ƿ�Ϊ��������
	bool IsLocal()
	{
		return m_SMS.stStatus.nRun == 1;
	}

	// �Ƿ�����
	bool IsRun()
	{
		return m_SMS.stStatus.nRun == 1||m_SMS.stStatus.nRun == 2;
	}

	// ��ȡ���н�ɫ��Ϊ1:Ϊ�������У�2:Ϊ�������� 3:�ӹ�����
	int GetRunRole()
	{
		return m_SMS.nRole;
	}

	// ���öԶ˵���������л��ӿ�
	int CallStandbySwitchSMS(bool bDelaySwitch,std::string strOtherIP,int nPort,std::string strHallID);

	// ��ȡ����������webservice�˿�
	void GetRunHost(std::string &strIP,int &nPort);

	// ���ôӻ���closesms�ӿ�
	int  CallStandbyStartOrCloseSMS(bool bSoC,std::string strOtherIP,int nPort,std::string strHallID);

	bool IsRouteReboot();
private:

	// ��Ȿ�����Ƿ�������
	int ISSMSRun();

	// ��ȡ�붨���̵�pid
	int GetPID(std::string strName,std::vector<int>& vecPID);

	// ��ȡ����ִ�е�·��
	bool GetPIDExeDir(int nPID,std::string &strDir);

	// �ڵ�ǰ�ն�����SMS
	bool StartSMS_CurTerminal(int &nPid,bool bLocalHost=false);

	// �����ն�����SMS
	bool StartSMS_NewTerminal(int &nPid,bool bLocalHost=false);

	// �����ն�����SMS,ʹ��execl
	bool StartSMS_NewTerminalExe(int &nPid,bool bLocalHost=false);


	// webservice���ú���
	int UsherHttp(std::string &strURI,std::string& strIP,std::string &xml,std::string action,std::string &strRequest);
	int GetHttpContent(const std::string &http, std::string &content);
	int TcpOperator(std::string strIP,int nPort,const std::string &send, std::string &recv, int overtime);
	int SendAndRecvInfo(const std::string &send, std::string &recv, int overtime);
	int ReceiveCommand(std::string &recv, int waitTime);
	std::string ExtractXml(const std::string &response);
 	int GetRootChild( const std::string &xml,xercesc::XercesDOMParser *parser,
 		xercesc::ErrorHandler *errHandler, xercesc::DOMElement **rootChild);
 	xercesc::DOMElement *GetElementByName( const xercesc::DOMNode *elem, const std::string &name);
 	xercesc::DOMElement *FindElementByName( const xercesc::DOMNode *elem, const std::string &name);

	// ��������GetSMSWorkState�ӿڵķ���
	int Parser_GetSMSWorkState( const std::string &content, int &state, std::string &info);

	// ��������SwitchSMS�ӿڵķ���
	int Parser_SwitchSMS(std::string &content,int &nRet);

	// ��������closesms�ӿڵķ���
	int Parser_StartOrCloseSMS(bool bSoC,std::string &content,int &nRet);

	// ��ȡָ�������pid
	int Getpid(std::string strName,std::vector<int>& vecPID);

private:
	
	// SMS��Ϣ
	SMSInfo m_SMS;

	// �����ڼ�ΪSMS����
	pthread_mutex_t m_mtxSMSInfo;
 
	//for http head
	std::string m_xmlHeader;
	std::string m_xmlnsXsi;
	std::string m_xmlnsXsd;
	std::string m_xmlnsSoap;
	std::string m_envelopeBgn;
	std::string m_envelopeEnd;
	std::string m_bodyBgn;
	std::string m_bodyEnd;

	//tcp protocol
	TcpTransport m_tcp;

	pid_t m_pid;

	// ��ʼ���б�� 
	bool m_bInitRun;

	bool m_bUseLocalDB;

	time_t m_tmReboot;// ���ڼ�¼ά����������ʱ��
};
#endif //HALL_DEFINE
