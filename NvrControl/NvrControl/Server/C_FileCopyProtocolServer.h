/****************************************************************************
*@file: C_FileCopyProtocolServer.h
*@brief: AQ33_CS���Եȷ������ļ�ͨ��Э��ʵ����
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-12
****************************************************************************/
#ifndef C_FILECOPYPROTOCOLSERVER_H
#define C_FILECOPYPROTOCOLSERVER_H

#include <string>
#include <map>
#include "C_ITcpSserver.h"
#include "C_RecvDataObserverImp.h"
#include "ConstDef.h"
#include "C_WriteDataThread.h"
namespace FileCopyProtocol
{
	class CFileCopyProtocolServer
	{
	public:
		CFileCopyProtocolServer();
		~CFileCopyProtocolServer();
	public:
		void SetServerPort(const unsigned short& port );
		void SetLogPath(const std::string& path );
		bool Start();
		bool Close();
	public:
		void Process(const NetData& data );
	private:
		void Init();
		/**
		*brief���ڶԵȻ����ϴ����ļ�
		*/
		void CreateFile(const NetData& data );

		/**
		*brief�������ļ�
		*/
		void TransferFile(const NetData& data );

		/**
		*brief���ļ�������ɹر��ļ�
		*/
		void TransferFileFinish(const NetData& data );

		/**
		*brief���ļ��������ʱɾ������·��
		*/
		void RemoveDirectoryAll(const NetData& data );

		/**
		*brief��ɾ������Ŀ¼�µ������ļ�
		*/
		void RemoveDirectory(const NetData& data );
		
		/**
		*brief������Ŀ¼�����Ŀ��Ŀ¼�������ٴ���
		*/
		void CreateDirectoryNotRm(const NetData& data );
		
		/**
		*brief������Ŀ¼,������ɾ�����ٴ���
		*/
		void CreateDirectoryRm(const NetData& data );

		/**
		*brief���޸�Ŀ¼Ȩ��
		*/
		void ChangeDirectoryOwn(const NetData& data );

		/**
		*brief�������޸Ľ��
		*/
		void RecToClient(const unsigned int& typeNo, const Errorno& erno );

	private:
		ICTcpServer *m_pTcpServer;
		CRecvDataObserverImp *m_pRecvDataObserver;
		unsigned short m_serverPort;
		std::string m_curpath;//��ǰ�ļ�·��
		std::map<int, std::string> m_errNoErrMsgMap;
		std::string m_LogPath;//��־�ļ�����ļ���
		CWriteDataThread *m_pWriteThread;
	};
}
#endif // C_FILECOPYPROTOCOLSERVER_H
