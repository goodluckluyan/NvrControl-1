#ifndef IPCCONTROL_TABLE_H
#define IPCCONTROL_TABLE_H
#include "../database/pub.h"
#include <mysql.h>

#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef struct IPC_ITEM_TBL
{
	 char *	id;							////idֵ
	 char * cinemaName;					////ӰԺ����
	 char * HallNo;						////Ӱ����
	 char * ipcPosition;				////����ͷλ��
	 char * frameRate;					////֡��
	 char * CBR;						////
	 char * resolvingRate;				////
	 char * decodeMode;					////����ģʽ
	 char * ip;							////ip��ַ
	 char * port;						////�˿ں�
	 char * username;					////�û���
	 char * password;					////����
	 char * mode;						////��ҹģʽ
	 char * channelID;					////ͨ����
}IPC_ITEM_TBL;

typedef vector<IPC_ITEM_TBL *> IPC_ITEMS_TBL;

class IpcTable:public PubFun
{
public:
	IpcTable();
	~IpcTable();
	/*************************************************\
	�������ܣ���ȡ����ipc��Ϣ
	����1����ȡ����ipc��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*************************************************/
	int GetIPCInfos(IPC_ITEMS_TBL * pIpcs);

	/*************************************************\
	�������ܣ�����Ӱ���ţ�ǰ��������ͷ��ȡipc��Ϣ
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3��ipc��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*************************************************/
	int GetIpcInfoByHallNoAndPosition(int HallNO,int position,IPC_ITEM_TBL * pIpcInfo);

	/**************************************************\
	�������ܣ�����id��ȡipc��Ϣ
	����1��idֵ
	����2��ipc��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\**************************************************/
	int GetIpcInfoById(int id,IPC_ITEM_TBL * pIpcInfo);

	/***************************************************\
	�������ܣ�����ipc��Ϣ
	����1��ipc��Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\***************************************************/
	int InsertIpcInfo(IPC_ITEM_TBL *ipcinfo);

	/***************************************************\
	�������ܣ�����id����ipc��Ϣ
	����1��idֵ
	����2������
	����3����ֵ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\***************************************************/
	int UpdateIpcInfo(int id,string columnName,string coumnValue);

	/***************************************************\
	�������ܣ�����Ӱ���ţ�ǰ��λ��ɾ��ָ��������ͷ
	����1��Ӱ����
	����2��λ��
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\***************************************************/
	int DeleteIpcInfoByHallNoAndPosition(int HallNo,int position);
	
	/****************************************************\
	�������ܣ�����idɾ��ָ��������ͷ
	����1��id��
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\****************************************************/
	int DeleteIpcInfoById(int id);

	/****************************************************\
	�������ܣ�ɾ��ȫ������ͷ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\****************************************************/
	int DeleteAllIpc();

public:
	////�����ڴ�ռ�
	void NewSpaceIpc(IPC_ITEM_TBL**ipcinfo);
	////�ͷ��ڴ�ռ�
	void DeleteSpaceIpc(IPC_ITEM_TBL**ipcinfo); 
	////����ڴ�ռ�
	void ZeroSpaceIpc(IPC_ITEM_TBL*ipcinfo);
protected:
private:
	int MapItemVarToArray(IPC_ITEM_TBL *pItem);
	COLUMN_VALUE_MAP  m_ipc_column_value_map[14];
};

#endif