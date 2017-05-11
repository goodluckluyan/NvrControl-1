//@file:CppMySQL3DB.h
//@brief: ������CppMySQL3DB������Ϊ�ⲿ���á�
//��װ�����ݿ�ĸ��ַ��ʲ�����
//@������:wangzhongping@oristartech.com
//dade:2012-07-12
#ifndef _CPP_MYSQL_3DB
#define _CPP_MYSQL_3DB
#include <string>
#include "CppMySQLQuery.h"

#define BUFSIZE 1024

class CppMySQL3DB
{
public:

    CppMySQL3DB();

    virtual ~CppMySQL3DB();
		//�������ݿ⡣
    int open(const char* host, const char* user, const char* passwd, const char* db,
		unsigned int port = 0, unsigned long client_flag = 0);
		int openTMS();
		//�ر����ݿ⡣
    void close();

	/* ���ؾ�� */
	MYSQL* getMysql();

	/* �����ض��еĲ�ѯ������Ӱ������� */
	//������������Ϊ��CppMySQLQuery�ĸ�ֵ���캯����Ҫ�ѳ�Ա����_mysql_res��Ϊ��
	CppMySQLQuery& querySQL(const char *sql,int &iResult);
	/* ִ�зǷ��ؽ����ѯ */
	int execSQL(const char* sql);
	/* ����mysql�������Ƿ��� */
	int ping();
	/* �ر�mysql ������ */
	int shutDown();
	/* ��Ҫ����:��������mysql ������ */
	int reboot();
	/*
	* ˵��:����֧��InnoDB or BDB������
    */
	/* ��Ҫ����:��ʼ���� */
	int startTransaction();
	/* ��Ҫ����:�ύ���� */
	int commit();
	/* ��Ҫ����:�ع����� */
	int rollback();
	/* �õ��ͻ���Ϣ */
	const char * getClientInfo();
	/* ��Ҫ����:�õ��ͻ��汾��Ϣ */
	const unsigned long  getClientVersion();
	/* ��Ҫ����:�õ�������Ϣ */
	const char * getHostInfo();
	/* ��Ҫ����:�õ���������Ϣ */
	const char * getServerInfo();
	/*��Ҫ����:�õ��������汾��Ϣ*/
	const unsigned long  getServerVersion();
	/*��Ҫ����:�õ� ��ǰ���ӵ�Ĭ���ַ���*/
	const char *  getCharacterSetName();
	/* �õ�ϵͳʱ�� */
	const char * getSysTime();
	/* ���������ݿ� */
	int createDB(const char* name);
	/* ɾ���ƶ������ݿ�*/
	int dropDB(const char* name);

	bool tableExists(const char* table);

    u_int lastRowId();

    void setBusyTimeout(int nMillisecs){};

	
	std::string errmsg()
	{
		std::string sError;
		char buffer[BUFSIZE]="";
		unsigned int errcode = mysql_errno(_db_ptr);
		const char* pstr = mysql_error(_db_ptr);
		if ( errcode == 0 && pstr!=NULL && pstr[0] == '\0' )
		{
			return sError;
		}
		sprintf( buffer , "mysql DB error msg:errno(%u) \n errormsg:(%s)\n" , errcode , pstr );
		sError = buffer;
		return sError;
	}

private:

    CppMySQL3DB(const CppMySQL3DB& db);
    CppMySQL3DB& operator=(const CppMySQL3DB& db);

    void checkDB();

private:
	/* msyql ���Ӿ�� */
	MYSQL* _db_ptr;
	CppMySQLQuery _db_query;
};
#endif //_CPP_MYSQL_3DB

#define WRITE_DBELOG
#if defined(WRITE_DBELOG) 
#ifndef TMS20_LOG
#include "../log/C_LogManage.h"
#endif
#endif

#ifdef WRITE_DBELOG
#define DBLOG( errorCode ,str) MySqlDB_WriteErrorLog(errorCode,str);
int MySqlDB_WriteErrorLog( int& errorCode ,const std::string& strError );
#else
#define DBLOG( errCode ,str ) 
#endif // WRITE_DBELOG



