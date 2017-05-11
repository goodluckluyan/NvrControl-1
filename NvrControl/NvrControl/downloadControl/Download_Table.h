#ifndef DOWNLOAD_TABLE_H
#define DOWNLOAD_TABLE_H
#include "../database/pub.h"
#include <mysql.h>

#include <vector>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef struct DOWNLOAD_ITEM_TBL
{
	 char *	id;							////idֵ
	 char * cinemaName;					////ӰԺ����
	 char * HallNo;						////Ӱ����
	 char * IPCPosition;				////����ͷλ��
	 char * state;						////����״̬
	 char * starttime;					////��ʼ����ʱ��
	 char * endtime;					////��������ʱ��
	 char * ipcIP;						////ipc��IP��ַ
	 char * downloadID;					////���ص�ID
	 char * filename;					////���ص�ԭʼ�ļ����ļ���
	 char * mp4filename;				////mp4�ļ���
	 char * covertstate;				////ת����װ��ʽ��״̬
	 char * avifilename;                ////avi�ļ���
	 char * loginHandle;				////��¼id
}DOWNLOAD_ITEM_TBL;

typedef vector<DOWNLOAD_ITEM_TBL *> DOWNLOAD_ITEMS_TBL;




class downloadtable:public PubFun
{
public:
	downloadtable();
	~downloadtable();

	/*****************************************************\
	�������ܣ�����������Ϣ
	����1��������Ϣ
	�ɹ�����0��ʧ�ܷ�������ֵ
	\*****************************************************/
	int insertDownloadInfo(DOWNLOAD_ITEM_TBL * pItem);

	/*****************************************************\
	�������ܣ�����������Ϣ����������id��
	����1������
	����2����ֵ
	����3������id
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*****************************************************/
	int updateDownloadInfoByDownloadID(string columnName,string columnValue,long long downloadID);

	/******************************************************\
	�������ܣ�����IDֵ����������Ϣ
	����1��idֵ
	����2������
	����3����ֵ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int updateDownloadInfoByid(int id,string columnName,string columnValue);

	/******************************************************\
	�������ܣ���ȡȫ��������Ϣ
	����1��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int GetAllDownloadInfo(DOWNLOAD_ITEMS_TBL & downloadinfos);

	/******************************************************\
	�������ܣ�����Ӱ���ţ�ǰ��������ͷ��ȡ������Ϣ
	����1��Ӱ����
	����2��ǰ��������ͷ
	����3��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int GetDownloadInfoByHallNoAndIpcPo(int HallNo,int ipcPo,DOWNLOAD_ITEMS_TBL & downloadinfos);

	/******************************************************\
	�������ܣ���������id��ȡ������Ϣ
	����1������id
	����2��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int GetDownloadInfoByDownloadID(int downloadId,DOWNLOAD_ITEM_TBL & downloadinfo);

	/******************************************************\
	�������ܣ���������״̬��ȡ������Ϣ
	����1��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int GetDownloadInfoBystate(DOWNLOAD_ITEMS_TBL & downloadinfos);

	/******************************************************\
	�������ܣ���ȡ���²������
	����1��idֵ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\******************************************************/
	int GetLatestInsertId(int &id);

	/*******************************************************\
	�������ܣ�����sql��ȡ������Ϣ
	����1��sql���
	����2��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************/
	int GetDownloadInfoBySql(string sql,DOWNLOAD_ITEMS_TBL & downloadinfos);

	/*******************************************************\
	�������ܣ�����sql����ȡ������Ϣ
	����1��sql���
	����2��������Ϣ
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************/
	int GetDownloadInfoBySql(string sql,DOWNLOAD_ITEM_TBL *downloadinfo);

public:
	////�����ڴ�ռ�
	void NewSpaceDownload(DOWNLOAD_ITEM_TBL**downloadinfo);
	////�ͷ��ڴ�ռ�
	void DeleteSpaceDownload(DOWNLOAD_ITEM_TBL**downloadinfo); 
	////����ڴ�ռ�
	void ZeroSpaceDownload(DOWNLOAD_ITEM_TBL*downloadinfo);

	void DeleteSpaceDownloads(DOWNLOAD_ITEMS_TBL * downloadinfos);
protected:
private:
	int MapItemVarToArray(DOWNLOAD_ITEM_TBL *pItem);
	COLUMN_VALUE_MAP  m_download_column_value_map[14];
};

#endif