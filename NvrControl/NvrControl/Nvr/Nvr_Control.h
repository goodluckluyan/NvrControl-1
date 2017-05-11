#ifndef NVR_CONTROL_H
#define NVR_CONTROL_H
#include <iostream>
#include "../so/dhnetsdk.h"
#include "../so/dhconfigsdk.h"
#include <vector>
#include "../Nvr_Types.h"
using namespace std;
typedef struct DM_NvrInfo
{
	int id;
	string cinemaName;
	string nvrIP;
	string NvrPort;
	string UserName;
	string PassWord;
	int IpcNo;
}DM_NvrInfo;
typedef vector<DM_NvrInfo> DM_NvrInfos;
typedef vector<DM_NvrInfo> *PDM_NvrInfos;

typedef struct Time_Info
	{
		int nYear;
		int nMonth;
		int nDay;
		int nHour;
		int nMinute;
		int nSecond;
	}Time_Info;
class NvrControl
{
public:
	
	NvrControl()
	{
		m_lLoginHandle = 0;
		m_nStDevInfoNu = 0;
	}
	~NvrControl()
	{}
	/***************************************************************\
	�������ܣ���ʼ��sdk
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\***************************************************************/
	int DM_InitSDK();
	/***************************************************************\
	�������ܣ�ע���û����豸
	����1��nvr��IP��ַ
	����2���˿ں�
	����3���û���
	����4������
	����ֵ���ɹ������豸ID��ʧ�ܷ���0
	\***************************************************************/
	//int DM_Login(char *NvrIP,WORD Port,char *UserName,char *Password);
	//test
	int DM_Login(string NvrIP,WORD Port,string UserName,string Password);
	//test
	/**************************************************************\
	�������ܣ�ע���豸�û�
	����1��Login�����ķ���ֵ
	����ֵ���ɹ�����0,ʧ�ܷ�������ֵ
	\**************************************************************/
	int DM_Logout(LLONG lLoginID);

	/*************************************************************\
	�������ܣ����SDK���ͷ�SDK��ռ�õ���Դ�������е�SDK�������ý���
	֮��
	\*************************************************************/
	void DM_Cleanup();

	/******************************************************************\
	�������ܣ�����nvr����Ϣ
	����1��nvrip
	����2��nvr�˿ں�
	����3��nvr�û���
	����4��nvr����
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************************/
	int DM_SetNvrInfo(string nvrIP,int nvrPort,string username,string password);

	/*******************************************************************\
	�������ܣ���ȡȫ��nvr��Ϣ
	����1��ȫ��nvr��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************************/
	int DM_GetAllNvrInfo(DM_NvrInfos&nvrinfos);

	/*******************************************************************\
	�������ܣ�����id��ȡnvr��Ϣ
	����1��id
	����2��nvr��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************************/
	int DM_GetNvrInfoById(int id,DM_NvrInfo & nvrinfo);

	/*******************************************************************\
	�������ܣ�����Nvr�����ٶ�
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************************/
	int DM_SetNvrDownloadSpeed();

	/**************************************************************\
	�������ܣ���ȡnvr��IPCʱ��
	����1��ʱ����Ϣ
	����ֵ���ɹ�����0.ʧ�ܷ�������ֵ
	\**************************************************************/
	int DM_GetNVRIPCTime(Time_Info &timeInfo);

	/**************************************************************\
	�������ܣ�����nvr��ipcʱ��
	����1��ʱ����Ϣ
	����ֵ:�ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************************/
	int DM_SetNVRIPCTime(Time_Info timeInfo);

public:
	/**************************************************************\
	�������ܣ���ȡ������Ϣ
	����1�������룬
	����2��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************************/
	int GetNvrErrorString(int errorcode,REPORT_STATUS *preportstatus);


	LLONG m_lLoginHandle;
	int m_nStDevInfoNu;
};

#endif