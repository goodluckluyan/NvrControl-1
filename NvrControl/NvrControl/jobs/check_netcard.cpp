
#include "check_netcard.h"
#include <time.h>
#include <errno.h>
#include <string>
#include <vector>
#include <algorithm>
#include "../para/C_Para.h"
#include"../database/CppMySQL3DB.h"
#include "../log/C_LogManage.h"

#ifndef WIN32
#define O_BINARY       0x8000  
#ifndef strcmpi
#define strcmpi strcasecmp
#endif
#ifndef stricmp
#define stricmp strcasecmp
#endif
#endif

#define  LOG(errid,msg)  C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_JOBS,0,errid,msg)
#define  LOGERRFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_ERROR,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)
#define  LOGINFFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_INFO,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)


#define ETHTOOL_GLINK        0x0000000a /* Get link status (ethtool_value) */

typedef enum { IFSTATUS_UP, IFSTATUS_DOWN, IFSTATUS_ERR } interface_status_t;

typedef signed int u32;

/* for passing single values */
struct ethtool_value
{
	u32    cmd;
	u32    data;
};

interface_status_t interface_detect_beat_ethtool(int fd, char *iface)
{
	struct ifreq ifr;
	struct ethtool_value edata;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name)-1);

	edata.cmd = ETHTOOL_GLINK;
	ifr.ifr_data = (caddr_t) &edata;

	if (ioctl(fd, SIOCETHTOOL, &ifr) == -1)
	{
		perror("ETHTOOL_GLINK failed ");
		return IFSTATUS_ERR;
	}

	return edata.data ? IFSTATUS_UP : IFSTATUS_DOWN;
}

int GetIFInfo(std::vector<std::string> &vecEth)
{
	int fd;
	char buf[1024];
	struct ifconf ifc;
	ifc.ifc_len=sizeof(buf);
	ifc.ifc_buf = buf;
	
	fd = socket(AF_INET,SOCK_DGRAM,0);

	if(ioctl(fd,SIOCGIFCONF,&ifc) < 0)
	{
		LOGERRFMT(0,"get if info error: %s",strerror(errno));
		return -1;
	}

	int nEth = ifc.ifc_len/sizeof(struct ifreq);
	struct ifreq *ifr;
	ifr = (struct ifreq*)buf;
	for(int i = 0 ;i < nEth ;i++)
	{
		if(strcmp(ifr->ifr_name,"lo")== 0)
		{
			ifr++;
			continue;
		}

		char cName[4]={0};
		strncpy(cName,ifr->ifr_name,3);
		if(strcmp(cName,"eth") != 0 && strcmp(cName,"bon") != 0)
		{
			ifr++;
			continue;
		}

		LOGINFFMT(0,"%s:%s\n",ifr->ifr_name,inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr));
		vecEth.push_back(ifr->ifr_name);
		ifr++;
	}
	return  nEth;

	
}


//�������������״̬��rLinkStatus = 1,����������rLinkStatus = 0,���Ӳ����� 
int Test_NetCard::Check_EthLinkStatus(const char* eth_name , int& rLinkStatus )
{
	FILE *fp = NULL;
	interface_status_t status;
	char buf[512] = {'\0'};
	char hw_name[10] = {'\0'};
	//char *token = NULL;

	if ( eth_name == NULL )
	{
		return -1;
	}
	
	strcpy( hw_name , eth_name );

	//����һ���鿴һ���ļ��������˵�Ƚϼ�
#if 0
	char carrier_path[512] = {'\0'};

	memset(buf, 0, sizeof(buf)); 
	snprintf(carrier_path, sizeof(carrier_path), "/sys/class/net/%s/carrier", hw_name);
	if ((fp = fopen(carrier_path, "r")) != NULL)
	{
		while (fgets(buf, sizeof(buf), fp) != NULL)
		{
			if (buf[0] == '0')
			{
				status = IFSTATUS_DOWN;
			}
			else
			{
				status = IFSTATUS_UP;
			}
		}
	}
	else
	{
		perror("Open carrier ");
		return -1;
	}
	fclose(fp);
#endif

	//���������ú����ɣ��е㸴�ӣ�����Ҳ��һ����Ч�İ취
#if 1
	int fd = -1;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket ");
		return -70;
		//exit(0);
	}
	status = interface_detect_beat_ethtool(fd, hw_name);
	close(fd);
#endif

	switch ( status )
	{
	case IFSTATUS_UP:
		rLinkStatus = 1;
		//printf("%s : link up\n", hw_name);
		break;

	case IFSTATUS_DOWN:
		rLinkStatus = 0;
		//printf("%s : link down\n", hw_name);
		break;

	default:
		rLinkStatus = 0;
		//printf("Detect Error\n");
		
		return -1;
		break;
	}

	return 0;
}


int Read_NetDataBytes(const char * eth_name  , char rwMode , unsigned long long& bytes )
{
	FILE *fp = NULL;
	char buf[1024] = {'\0'};
	char path[1024] = {'\0'};

	memset(buf, 0, sizeof(buf)); 

	if ( rwMode == 'r' )
	{
		snprintf( path , sizeof(path) , "/sys/class/net/%s/statistics/rx_bytes" , eth_name );
	} 
	else
	{
		snprintf( path , sizeof(path) , "/sys/class/net/%s/statistics/tx_bytes" , eth_name );
	}

	if ((fp = fopen(path, "r")) != NULL)
	{
		while ( fgets( buf , sizeof(buf) , fp ) != NULL)
		{
			int result(0);
			result = sscanf( buf,"%llu\n", &bytes );
			if(result == 0)//sscanfʧ�ܣ��򷵻�0
			{
				LOGERRFMT(0, "Error:sscanf %s\n" , path );
				//return -1;
			}
		}
	}
	else
	{
		LOGERRFMT(0,"Open statistics ");
		return -1;
	}

	fclose(fp);

	return 0;
}


Test_NetCard::Test_NetCard()
{
	int nIndex = 0;
	m_bInit = false;
	m_ptrDM = NULL;
}

Test_NetCard::~Test_NetCard()
{

}


//��ʼ�������������ȱ�����
int Test_NetCard::Init()
{
	m_ptrDM = CDataManager::GetInstance();
	std::map<std::string,int> mapEthBaseInfo;
	if(!ReadEthinfoTable(mapEthBaseInfo))
	{
		return -1;
	}
	m_ptrDM->SetEthBaseInfo(mapEthBaseInfo);
	std::map<std::string,int>::iterator it = mapEthBaseInfo.begin();
	for(;it != mapEthBaseInfo.end();it++)
	{
		m_vecEth.push_back(it->first);
	}

	// ��ȡ����ӵ��ip��eth
	std::vector<std::string> vecEth;
	int nEth = GetIFInfo(vecEth);
	if(nEth <= 1)
	{
		return -1;
	}

	// �ҳ�bond0
	std::vector<std::string>::iterator fit;
	for(int i = 0 ;i < vecEth.size();i++)
	{
		fit = std::find(m_vecEth.begin(),m_vecEth.end(),vecEth[i]) ;
		if(fit == m_vecEth.end())
		{
			m_vecEth.push_back(vecEth[i]);
		}
	}
	
	int ret = 0;
	for ( int nIndex = 0; nIndex < m_vecEth.size() ; nIndex++ )
	{
		//��ʼ�� m_old_CheckTime��ÿ������1��ʱ��ֵ
		time_t curTime = time(NULL);
		m_old_CheckTime[m_vecEth[nIndex]] = curTime;

		unsigned long long nInitRx_bytes = 0;
		ret = Read_NetDataBytes(m_vecEth[nIndex].c_str() , 'r' ,  nInitRx_bytes );
		if ( ret == 0 )
		{
			//printf( "Read_NetDataBytes:Recv Operate nInitRx_bytes = %llu\n" , nInitRx_bytes );
			m_oldRx_bytes[m_vecEth[nIndex]] = nInitRx_bytes;
		}
		else
		{
			//printf( "Init Error:eth=%s ,Recv nInitRx_bytes\n" , m_vecEth[nIndex].c_str() );
			//return -10;
		}

		unsigned long long nInitTx_bytes = 0;
		ret = Read_NetDataBytes( m_vecEth[nIndex].c_str() , 't' ,  nInitTx_bytes );
		if ( ret == 0 )
		{
			//printf( "Read_NetDataBytes:Send Operate nInitTx_bytes = %llu\n" , nInitTx_bytes );
			m_oldTx_bytes[m_vecEth[nIndex]] = nInitTx_bytes;
		}
		else
		{
			LOGERRFMT(0, "Init Error:eth=%s ,Send nInitTx_bytes\n" , m_vecEth[nIndex].c_str() );
			//return -11;
		}
	}
	m_bInit = true;

	return 0;
}

bool Test_NetCard::ReadEthinfoTable(std::map<std::string,int>& mapEthBaseInfo)
{
	C_Para *ptrPara = C_Para::GetInstance();
	CppMySQL3DB mysql;
	if(mysql.open(ptrPara->m_strDBServiceIP.c_str(),ptrPara->m_strDBUserName.c_str(),
		ptrPara->m_strDBPWD.c_str(),ptrPara->m_strDBName.c_str()) == -1)
	{
		LOG(0,"mysql open failed!\n");
		return false;
	}

	// ��ȡethinfo ,��ʼ��������Ϣ
	int nResult;
	char strSQL[256];
	snprintf(strSQL,256,"select * from eth_set where position=%d",ptrPara->IsMain() ? 0:1);
	CppMySQLQuery query = mysql.querySQL(strSQL,nResult);
	int nRows = 0 ;
	if((nRows = query.numRow()) == 0)
	{
		LOG(0,"CDataManager Initial failed,ethinfo talbe no rows!\n");
		return false;
	}

	query.seekRow(0);
	for(int i = 0 ;i < nRows ; i++)
	{
		std::string strName = query.getStringField("eth_name");//"eth"
		int nType = atoi(query.getStringField("type"));
		mapEthBaseInfo[strName] = nType;
		query.nextRow();
	}
	return true;
	
}

//���������� eth_name=�������Ʊ���Ϊ"eth0,eth1,eth2,eth3,eth4,eth5"��1��;TranBPS=��ʾÿ�뷢���ֽ�����RecvBPS=��ʾÿ������ֽ���
int Test_NetCard::Check_NewWork_Flow( std::string eth_name , unsigned long long& TranBPS ,
									 unsigned long long& RecvBPS )
{
	//printf("Enter Check_NewWork_Flow()!\n\n");

	int ret = 0;
	int nEthNameIndex = 0;
	char hw_name[20] = {'\0'};
	int nIntervalTime = 0;
	
	if ( eth_name.empty() )
	{
		return -1;
	}

	time_t curTime = time(NULL);
	nIntervalTime = curTime - m_old_CheckTime[eth_name];
	if ( nIntervalTime <= 0 )
	{
		nIntervalTime = 1;
	}

// 	printf( "\n curTime=%d\n" , curTime );
// 	printf( "m_old_CheckTime=%d\n" , m_old_CheckTime[nEthNameIndex] );
// 	printf( "IntervalTime=%d\n\n" , nIntervalTime );

	m_old_CheckTime[eth_name] = curTime;


	//��ʼ��static���� oldTx_bytes[],oldRx_bytes[] 
	if ( m_oldRx_bytes[eth_name] == 0 )
	{
		unsigned long long nInitRx_bytes = 0;
		ret = Read_NetDataBytes( eth_name.c_str() , 'r' ,  nInitRx_bytes );
		if ( ret == 0 )
		{
			//printf( "Read_NetDataBytes:Recv Operate nInitRx_bytes = %llu\n" , nInitRx_bytes );
			m_oldRx_bytes[eth_name] = nInitRx_bytes;
		}
		else
		{
			return -10;
		}
	} 
	
	if ( m_oldTx_bytes[eth_name] == 0 )
	{
		unsigned long long nInitTx_bytes = 0;
		ret = Read_NetDataBytes( eth_name.c_str() , 't' ,  nInitTx_bytes );
		if ( ret == 0 )
		{
			//printf( "Read_NetDataBytes:Send Operate nInitTx_bytes = %llu\n" , nInitTx_bytes );
			m_oldTx_bytes[eth_name] = nInitTx_bytes;
		}
		else
		{
			return -11;
		}
	}
	//��ʼ��static���� end 

	//���ȣ���ȡ�ܵķ���/�����ֽ���
	unsigned long long newRx_bytes = 0;
	ret = Read_NetDataBytes( eth_name.c_str() , 'r' ,  newRx_bytes );
	if ( ret != 0 )
	{
		return -3;
	}
	else
	{
		//printf( "Read_NetDataBytes:Recv Operate newRx_bytes = %llu\n" , newRx_bytes );
	}
	
	unsigned long long newTx_bytes = 0;
	ret = Read_NetDataBytes(  eth_name.c_str() , 't' ,  newTx_bytes );
	if ( ret != 0 )
	{
		return -4;
	}
	else
	{
		//printf( "Read_NetDataBytes:Send Operate newTx_bytes = %llu\n" , newTx_bytes );
	}

	unsigned long long nRealRecv_bytes = 0 , nRealSend_bytes = 0;

	//�����������
	if ( newRx_bytes != 0 )
	{
		nRealRecv_bytes  = newRx_bytes - m_oldRx_bytes[eth_name];
		if( nRealRecv_bytes < 0 )
		{
			nRealRecv_bytes = newRx_bytes;
		}
	}

	RecvBPS = nRealRecv_bytes/(unsigned long long)nIntervalTime;

	//printf( "Real Recv bytes %llu B \n" , nRealRecv_bytes );
	//printf( "Net Speed RecvBPS: %llu B/S \n" , RecvBPS );
	//printf( "Net Speed Recv: %llu KB/S \n" , RecvBPS/1024 );

	if ( nRealRecv_bytes != 0 )
	{
		m_oldRx_bytes[eth_name] = newRx_bytes;
	}


	//���㷢������
	if ( newTx_bytes != 0 )
	{
		nRealSend_bytes  = newTx_bytes - m_oldTx_bytes[eth_name];
		if( nRealSend_bytes < 0 )
		{
			nRealSend_bytes = newTx_bytes;
		}
	}

	TranBPS = nRealSend_bytes/(unsigned long long)nIntervalTime;

	//printf( "Real Send bytes %llu B \n" , nRealSend_bytes );
	//printf( "Net Speed TranBPS: %llu B/S \n" , TranBPS );
	//printf( "Net Speed Send: %llu KB/S \n" , TranBPS/1024 );

	if ( nRealSend_bytes != 0 )
	{
		m_oldTx_bytes[eth_name] = newTx_bytes;
	}

	//printf("Finished Check_NewWork_Flow()!\n\n");

	return 0;
}

bool Test_NetCard::GetAllEthStatus()
{
	if(!m_bInit)
	{
		Init();
	}

	std::vector<EthStatus> vecEthStatus;
	int nLen = m_vecEth.size();
	for(int i = 0 ;i < nLen ;i++)
	{
		EthStatus node;
		node.strName = m_vecEth[i];
	
		if(Check_EthLinkStatus(m_vecEth[i].c_str(),node.nConnStatue) == -1)
		{
			vecEthStatus.push_back(node);
			continue;
		}
		if(node.nConnStatue == 1)
		{
			Check_NewWork_Flow(m_vecEth[i],node.nTxSpeed,node.nRxSpeed);
		}
		vecEthStatus.push_back(node);
	}

	if(m_ptrDM != NULL)
	{
		m_ptrDM->UpdateNetStat(vecEthStatus);
	}
	
}



bool Test_NetCard::InitAndCheck()
{
	bool bRet = true;
	if(!m_bInit)
	{
		Init();
	}
	
	int nLen = m_vecEth.size();
	for(int i = 0 ;i < nLen ;i++)
	{
		int nConnStatus = 0;
		if(Check_EthLinkStatus(m_vecEth[i].c_str(),nConnStatus) == -1)
		{
			LOG(0,"Get EthLink Status Fail!\n");
			bRet = false;
			break;
		}
		else
		{
			if(nConnStatus != 1)
			{
				LOGINFFMT(0,"%s Connect Status Down!\n",m_vecEth[i].c_str());
				break;
			}
		}
	}
	return bRet;

}

