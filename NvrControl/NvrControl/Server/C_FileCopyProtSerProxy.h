/****************************************************************************
*@file: C_FileCopyProtSerProxy.h
*@brief: AQ33_CS���Եȷ������ļ�ͨ��Э�����˴�����
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-12
****************************************************************************/
#ifndef CFILECOPYPROTOCOLSERVERPROXY_H
#define CFILECOPYPROTOCOLSERVERPROXY_H

#include <string>

namespace FileCopyProtocol
{
	class CFileCopyProtocolServer;
}
class CFileCopyProtSerProxy
{
public:
	CFileCopyProtSerProxy();
	~CFileCopyProtSerProxy();

	/**
	*brief:���÷���˿ں�
	*/
	void SetServerPort(const unsigned short& port );

	/**
	*brief:������־���·��(Ŀ¼·��)
	*/
	void SetLogPath(const std::string& path );

	/**
	*brief:��������
	*/
	bool Start();

	/**
	*brief:�رշ���
	*/
	bool Close();
private:
	FileCopyProtocol::CFileCopyProtocolServer *m_pServer;
};

#endif // CFILECOPYPROTOCOLSERVERPROXY_H
