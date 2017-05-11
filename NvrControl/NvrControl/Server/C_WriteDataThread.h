/****************************************************************************
*@file: C_WriteDataThread.h
*@brief: д���ļ��߳���
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-11
****************************************************************************/
#ifndef CWRITEDATATHREAD_H
#define CWRITEDATATHREAD_H

#include <pthread.h>
#include <string>
#include "CircularQueue.h"
class CWriteDataThread
{
public:
    CWriteDataThread();
    ~CWriteDataThread();

public:
	/**
	*brief������д���ļ�·��
	*/
	void SetFilePath(const std::string& path );

	/**
	*brief������ˢ�´�����ֵ
	*/
	void SetSyncSz(const int& sz );

	/**
	*brief������д������
	*/
	bool SetWriteData(const element_Info& elemInfo ,const int& syncFlag =0 );

	/**
	*brief�����ùر��ļ���ʶ
	*/
	void SetCloseFlag(const bool& flag );

	/**
	*brief�������߳�
	*/
	void StartThread();

	/**
	*brief���ļ��Ƿ��ڹر�״̬
	*/
	bool IsFileClosed( );

	/**
	*brief��д���ļ��̺߳���
	*/
	void WriteToFile();

public:
	bool m_bWriteWrong;
private:
	unsigned char m_syncFlag;//ˢ�´��̱�ʶ
	std::string m_filePath;//�ļ�·��
	bool m_bColseFile;//�ر��ļ���ʶ
	pthread_t  m_threadId;//�߳�Id
	CircularQueue* m_pCircularQueue;//ѭ�����л���
	int m_syncSz;//���������ֵ
	int m_recDataSz; // �����ļ���С
	bool m_bFileClosed; //�ļ��Ƿ��Ѿ��رձ�ʶ
};

#endif // CWRITEDATATHREAD_H
