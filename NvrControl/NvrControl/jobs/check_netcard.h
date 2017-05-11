//@file:check_netcard.h
//@brief: ����״̬��⡣
//@author:zhangmiao@oristartech.com
//date:2014-09-17

#ifndef CHECKNETCARD_INC
#define CHECKNETCARD_INC

#include <linux/sockios.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DataManager.h"

class Test_NetCard
{
public:
	Test_NetCard();
	~Test_NetCard();

	//��ʼ�������������ȱ�����
	int Init();

	//��ʼ�����
	bool InitAndCheck();

	// ��ȡ���п�������״̬
	bool GetAllEthStatus();
private:

	bool ReadEthinfoTable(std::map<std::string,int>& mapEthBaseInfo);

	//�������������״̬��rLinkStatus = 1,����������rLinkStatus = 0,���Ӳ����� 
	int Check_EthLinkStatus(const char* eth_name , int& rLinkStatus );

	//���������� eth_name=�������Ʊ���Ϊ"eth0,eth1,eth2,eth3,eth4,eth5"��1��;TranBPS=��ʾÿ�뷢���ֽ�����RecvBPS=��ʾÿ������ֽ���
	int Check_NewWork_Flow( std::string eth_name , unsigned long long& TranBPS , unsigned long long& RecvBPS );
	
	
private:

	bool m_bInit;
	std::map<std::string,time_t> m_old_CheckTime;	//����ÿ��������һ�εļ��ʱ���
	std::vector<std::string> m_vecEth;


	std::map<std::string,unsigned long long> m_oldTx_bytes ;
	std::map<std::string,unsigned long long> m_oldRx_bytes;	//����ÿ��������һ�εķ���/����յ����ֽ���

	CDataManager * m_ptrDM;


};

#endif



