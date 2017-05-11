/****************************************************************************
*@file: C_ITcpServer.h
*@brief: Tcp��������Э�����ӿڶ���
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-11
****************************************************************************/
#ifndef C_ITCPSERVER
#define C_ITCPSERVER

//���ջ��͵�����
const int MAXBUFFERSZ =90000; /*9000;*/
typedef struct NetData
{
    int sz;
    char buffer[MAXBUFFERSZ];
}NetData;

//�������ݹ۲��߽ӿ�
class IRecvDataObserver
{
public:
    virtual void RecvNetData(const NetData& netData ) =0;
};

class ICTcpServer
{
public:
    virtual ~ICTcpServer( ){}
	/**
	*brief������TCP�����
	*/
    virtual bool StartServer() =0;

	/**
	*brief������TCP�����
	*/
    virtual bool StartServer(const unsigned short& port ) =0;

	/**
	*brief���ر�TCP�����
	*/
    virtual bool CloseServer() =0;

	/**
	*brief�����÷���˶˿�
	*/
    virtual void SetServerPort(const unsigned short& port ) =0;

	/**
	*brief��д��Ҫ���͵�����
	*/
    virtual bool WriteData(const NetData&netData ) =0;

	/**
	*brief�����ý������ݹ۲���
	*/
    virtual void SetRecvDataObserver(IRecvDataObserver*pObserver ) =0;
};
extern "C"
{
	ICTcpServer* createTcpServer();
	void releaseTcpServer(ICTcpServer* pServer );
};
#endif // C_ITCPSERVER

