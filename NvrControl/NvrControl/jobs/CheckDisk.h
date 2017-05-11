/***********************************************************
Copyright (C), 2010-2020, DADI MEDIA Co.,Ltd.
ModuleName: CheckDisk.h
FileName: CheckDisk.h
Author: chengyu
Date: 14/06/26
Version:
Description: ��ȡ������Ϣ
Others:
History:
		<Author>		<Date>		<Modification>
		chengyu			14/09/12
***********************************************************/


#ifndef _H_CHECKDISK_
#define _H_CHECKDISK_


#include <iostream>
#include <string.h>
#include <vector>
#include <cstdlib>
#include "DataManager.h"

#define COMM_FLAG		35		// '#'
#define	KEYVALUE_FLAG	58		// ':'
#define BLANK_FLAG1		32		// ' '
#define BLANK_FLAG2		9		// '	'
#define BLANK_FLAG3		13		// ����


class CheckDisk
{
public:
	//cus
	CheckDisk();
	//dis
	~CheckDisk();

	// ��ʼ�������
	bool InitAndCheck();

	//��ȡ���д�������Ϣ
	int ReadMegaSASInfo();

	//��ȡ�ܴ�������
	std::string GetDiskSize(){return DiskSize;};

	//��ȡ�ܴ���״̬
	std::string GetDiskState(){return DiskState;};

	//��ȡ�ܴ�������
	std::string GetDiskNumberOfDrives(){ return DiskNumOfDrives;};

	//��ӡ����״̬��Ϣ
	void PrintState();

private:
	
	int GetDiskInfo();

	//���ɰ���������Ϣ��MegaSAS.log�ļ�
	int GetDickInfoLog(std::string ppath);

	int GetRaidInfoFromPip();

	//ɾ���洢������Ϣ��MegaSAS.log�ļ�
	int RemoveDir( const std::string &dir);

	//�Ƿ�Ϊ�հ���
	int isblank( const char *pbuf);

	//��ȡÿ�����ƺ�ֵ
	int getkey( const char *pbuf, std::string &temp_key, std::string &temp_value);

	std::string DiskSize;//�ܴ�������
	std::string DiskState;//�ܴ���״̬
	std::string DiskNumOfDrives;//�ܴ�������

	std::map<int,DiskInfo> mapDiskInfo;//��������Ϣ

	CDataManager * m_ptrDM;

	char m_buf[20480];// Raid��־����
};

#endif //_H_CHECKDISK


