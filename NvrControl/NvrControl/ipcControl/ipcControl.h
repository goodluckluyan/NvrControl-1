#ifndef IPCCONTROL_H
#define IPCCONTROL_H
#include <iostream>
#include "../so/dhnetsdk.h"
#include "../so/dhconfigsdk.h"
//#include "ipcControl_Table.h"
#include "ipcControl_Struct.h"
#include "../Nvr/Nvr_Control.h"

#include <vector>
using namespace std;
#if 0
typedef struct DM_IpcInfo
{
	int id;
	int HallNo;
	int pos;
	int frameRate;
	int stream;
	int ResoluRatio;
	string codeMode;
	string IpcIP;
	string IpcPort;
	string mode;
	string username;
	string password;
}DM_IpcInfo;

typedef vector<DM_IpcInfo> DM_IpcInfos;
typedef vector<DM_IpcInfo> * pDM_IpcInfos;
#endif

typedef vector<string> ResoluRatios;

class ipccontrol
{
public:
	/************************************************************\
	�������ܣ���������ͷ��Ϣ
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3;ͨ����
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\************************************************************/
	int DM_SetIpcInfo(int nAudiNU,int nDevicePo,int nChannelId);

	/************************************************************\
	�������ܣ���ȡ����ͷͨ���Ÿ���Ӱ���ţ�ǰ��������ͷ
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3��ͨ����
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\************************************************************/
	int DM_GetIpcInfo(int nAudiNU,int nDevicePo,int &nChannelID);

	/************************************************************\
	�������ܣ���������ͷ�ֱ��ʣ�֡�ʣ������ʽ������
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3���ֱ���
	����4��֡��
	����5�������ʽ
	����6������
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**********i**************************************************/
	int DM_SetIpcVedioQuality(int nAudiNU,int nDevicePo,string ResoluRatio,int frame,string CodeMode,int stream);

	/*************************************************************\
	�������ܣ���ȡ����ͷ�豸֧�ֵķֱ���
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3���ֱ�������
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*************************************************************/
	int DM_GetIpcResoluRatio(int nAudiNU,int nDevicePo,ResoluRatios & resoluratios);

	/*************************************************************\
	�������ܣ���ȡ����ͷ�ֱ��ʣ�֡�ʣ������ʽ������
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3���ֱ���
	����4��֡��
	����5�������ʽ
	����6������
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*************************************************************/
	int DM_GetIpcVedioQuality(int nAudiNU,int nDevicePo,string &ResoluRatio,int &frame,string &CodeMode,int &stream);

	/**************************************************************\
	�������ܣ���������ͷ����ҹģʽ
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3��ģʽֵ,��modeΪ0ʱ��Ϊ����ģʽ����ʼ��δ��ɫ
			��modeΪ1ʱΪ�Զ��л�����modeΪ2ʱ��Ϊҹģʽ����ʼ��Ϊ
			�ڰ�ɫ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************************/
	int DM_SetIPCDayOrNeightMode(int nAudiNU,int nDevicePo,int Mode);

	/****************************************************************\
	�������ܣ���������ͷ�Զ��۽�
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3��ʹ��
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\****************************************************************/
	int DM_SetAutoFocus(int nAudiNU,int nDevicePo,bool isEnable);

	/**************************************************************\
	�������ܣ���������ͷ�佹
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3����������ÿ�ε�������ͷ����ʱ������ֵ
	����4���ӱ����߼���(DH_PTZ_ZOOM_ADD_CONTROL�ӱ���DH_PTZ_ZOOM_DEC_CONTROL����)
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************************/
	int DM_SetIPCZoom(int nAudiNU,int nDevicePo,int nStep,int nType);

	/**************************************************************\
	�������ܣ���ȡ������Ϣ
	����1��������
	����2��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************************/
	int GetIPCErrorString(int errorcode,REPORT_STATUS *preportstatus);

	//
	int NewSpaceDeviceIPCInfo(DEV_IPC_INFO **pdeviceipcinfo);
	//
	int DeleteSpaceDeviceIPCInfo(DEV_IPC_INFO **pdeviceipcinfo);
	//
    int ZeroSpaceDeviceIPCInfo(DEV_IPC_INFO * pdeviceipcinfo);
	//
	int DeleteSpaceIPCInfos(DEV_IPC_INFOS *pipcinfos);
	//
	int ZeroSpaceIPCInfos(DEV_IPC_INFOS *pipcinfos);
};

#endif