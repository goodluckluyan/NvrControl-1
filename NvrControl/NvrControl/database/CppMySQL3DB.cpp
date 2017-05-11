//@file:CppMySQL3DB.cpp
//@brief: CppMySQL3DB������ʵ�֡�
//@author:wangzhongping@oristartech.com
//dade:2012-07-12

#include "CppMySQL3DB.h"
#include "../para/C_Para.h"
#include "../C_constDef.h"
#include <iostream>
using namespace std;
CppMySQL3DB::CppMySQL3DB()
{
	_db_ptr = NULL;
}

CppMySQL3DB::~CppMySQL3DB()
{
	if ( _db_ptr != NULL )
	{
		close();
	}
}

int CppMySQL3DB::open(const char* host, const char* user, const char* passwd, const char* db,
		unsigned int port /*= 0*/, unsigned long client_flag /*= 0*/)
{
	int ret = -1;

	_db_ptr = mysql_init(NULL);
	if( NULL == _db_ptr ) 
		goto EXT;
	
	//�������ʧ�ܣ�����NULL�����ڳɹ������ӣ�����ֵ���1��������ֵ��ͬ��
	if ( NULL == mysql_real_connect( _db_ptr, host, user, passwd, db,port, NULL, client_flag) )
		goto EXT;

	/* �������ݿ�Ĭ���ַ��� */ 	
	if ( mysql_set_character_set( _db_ptr, "utf8" )  != 0 )
	{
		fprintf ( stderr , "����, %s/n" , mysql_error( _db_ptr) ) ;
	}

	//ѡ���ƶ������ݿ�ʧ��
	//0��ʾ�ɹ�����0ֵ��ʾ���ִ���
	if ( mysql_select_db( _db_ptr, db ) != 0 ) 
	{
		mysql_close(_db_ptr);
		_db_ptr = NULL;
		goto EXT;
	}

	ret = 0;
EXT:
	//��ʼ��mysql�ṹʧ��
	if ( ret == -1 && _db_ptr != NULL )
	{
		//--zhangmiao begin:11/28/2012
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param: host(%s),user(%s),passwd(%s),DB(%s),port(%d),client_flag(%lu)\n" ,host ,user, passwd, db, port, client_flag );
		strError = errmsg();
		strError += buffer;
		DBLOG((int &)ERROR_OPEN_DATABASE,strError);
		//printf("%s\n",strError.c_str());
		//--zhangmiao end:11/28/2012

		mysql_close( _db_ptr );
		_db_ptr = NULL;
	}
	//--zhangmiao begin:11/28/2012
	else if ( ret == -1 && _db_ptr == NULL )
	{
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param: host(%s),user(%s),passwd(%s),DB(%s),port(%d),client_flag(%lu)\n" ,host ,user, passwd, db, port, client_flag );
		strError = errmsg();
		strError += buffer;
		DBLOG((int &)ERROR_OPEN_DATABASE,strError);
		//printf("%s\n",strError.c_str());
	}
	//--zhangmiao end:11/28/2012

	return ret;
}


int CppMySQL3DB::openTMS()
{
	C_Para *pPara = C_Para::GetInstance();
	return open(pPara->m_strDBServiceIP.c_str(), pPara->m_strDBUserName.c_str(),
						           pPara->m_strDBPWD.c_str(),pPara->m_strDBName.c_str(),(unsigned int)pPara->m_usDBServicePort);
	
}	
void CppMySQL3DB::close()
{
	if ( _db_ptr != NULL )
	{
		mysql_close( _db_ptr );
		_db_ptr = NULL;
	}
}

MYSQL* CppMySQL3DB::getMysql()
{
	return _db_ptr;
}

/* �����ض��еĲ�ѯ������Ӱ������� */
CppMySQLQuery& CppMySQL3DB::querySQL(const char *sql,int &iResult)
{
	//wzp 2012-09-11 modify;
/*	if ( !mysql_real_query( _db_ptr, sql, strlen(sql) ) )
	{
		_db_query._mysql_res = mysql_store_result( _db_ptr );
// 		_db_query._row =  mysql_fetch_row( _db_query._mysql_res );
// 		_db_query._row_count = mysql_num_rows( _db_query._mysql_res ); 
// 		//�õ��ֶ�����
// 		_db_query._field_count = mysql_num_fields( _db_query._mysql_res );
	}

	return _db_query;*/
	iResult = mysql_real_query( _db_ptr, sql, strlen(sql));
	if(iResult == 0)
	{
		//wzp add in 2012-9-24
		if(_db_query._mysql_res != NULL)
		{
			mysql_free_result(_db_query._mysql_res);
			_db_query._mysql_res = NULL;
		} 
		//wzp add end;
		_db_query._mysql_res = mysql_store_result( _db_ptr );
	}
	//--zhangmiao begin:11/28/2012
	else
	{
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param: sql=(%s)\n" , sql );
		strError = errmsg();
		strError += buffer;
		DBLOG((int &)ERROR_QUERY_TABLE,strError);
	}
	//--zhangmiao end:11/28/2012
	return _db_query;
}

/* ִ�зǷ��ؽ����ѯ */
int CppMySQL3DB::execSQL(const char* sql)
{
	if( !mysql_real_query( _db_ptr, sql, strlen(sql) ) )
	{
		//�õ���Ӱ�������
		return (int)mysql_affected_rows(_db_ptr) ;
	}
	else
	{
		//ִ�в�ѯʧ��
		//--zhangmiao begin:11/28/2012
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param: sql=(%s)\n" , sql );
		strError = errmsg();
		strError += buffer;
		DBLOG( (int &)ERROR_EXEC_TABLE , strError);
		//--zhangmiao end:11/28/2012
		return -1;
	}
}

/* ����mysql�������Ƿ��� */
int CppMySQL3DB::ping()
{
	if( mysql_ping(_db_ptr) == 0 )
		return 0;
	else 
		return -1; 
}

/* �ر�mysql ������ */
int CppMySQL3DB::shutDown()
{
	if( mysql_shutdown(_db_ptr,SHUTDOWN_DEFAULT) == 0 )
		return 0;
	else 
		return -1;
}

/* ��Ҫ����:��������mysql ������ */
int CppMySQL3DB::reboot()
{
	if(!mysql_reload(_db_ptr))
		return 0;
	else
		return -1;
}

/*
* ˵��:����֧��InnoDB or BDB������
*/
/* ��Ҫ����:��ʼ���� */
int CppMySQL3DB::startTransaction()
{
	if(!mysql_real_query(_db_ptr, "START TRANSACTION" ,
		(unsigned long)strlen("START TRANSACTION") ))
	{
		return 0;
	}
	else
	{
		//ִ�в�ѯʧ��
		//--zhangmiao begin:11/28/2012
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param: START TRANSACTION\n" );
		strError = errmsg();
		strError += buffer;
		DBLOG( (int &)ERROR_START_TRANSACTION , strError);
		//--zhangmiao end:11/28/2012
		return -1;
	}
}

/* ��Ҫ����:�ύ���� */
int CppMySQL3DB::commit()
{
	if(!mysql_real_query( _db_ptr, "COMMIT",
		(unsigned long)strlen("COMMIT") ) )
	{
		return 0;
	}
	else
	{
		//ִ�в�ѯʧ��
		//--zhangmiao begin:11/28/2012
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param:\"COMMIT\" \n" );
		strError = errmsg();
		strError += buffer;
		DBLOG( (int &)ERROR_COMMIT_TRANSACTION , strError);
		//--zhangmiao end:11/28/2012
		return -1;
	}
}

/* ��Ҫ����:�ع����� */
int CppMySQL3DB::rollback()
{
	if(!mysql_real_query(_db_ptr, "ROLLBACK",
		(unsigned long)strlen("ROLLBACK") ) )
		return 0;
	else
	{
		//ִ�в�ѯʧ��
		//--zhangmiao begin:11/28/2012
		string strError;
		char buffer[BUFSIZE]="";
		sprintf( buffer , "mysql param:\"ROLLBACK\" \n" );
		strError = errmsg();
		strError += buffer;
		DBLOG( (int &)ERROR_ROLL_BACK_TRANSACTION , strError);
		//--zhangmiao end:11/28/2012
		return -1;
	}
}

/* �õ��ͻ���Ϣ */
const char * CppMySQL3DB::getClientInfo()
{
	return mysql_get_client_info();
}

/* ��Ҫ����:�õ��ͻ��汾��Ϣ */
const unsigned long  CppMySQL3DB::getClientVersion()
{
	return mysql_get_client_version();
}

/* ��Ҫ����:�õ�������Ϣ */
const char * CppMySQL3DB::getHostInfo()
{
	return mysql_get_host_info(_db_ptr);
}

/* ��Ҫ����:�õ���������Ϣ */
const char * CppMySQL3DB::getServerInfo()
{
	return mysql_get_server_info( _db_ptr ); 

}
/*��Ҫ����:�õ��������汾��Ϣ*/
const unsigned long  CppMySQL3DB::getServerVersion()
{
	return mysql_get_server_version(_db_ptr);

}

/*��Ҫ����:�õ� ��ǰ���ӵ�Ĭ���ַ���*/
const char *  CppMySQL3DB::getCharacterSetName()
{
	return mysql_character_set_name(_db_ptr);

}

/* �õ�ϵͳʱ�� */
const char * CppMySQL3DB::getSysTime()
{
	//return ExecQueryGetSingValue("select now()");
	return NULL;

}

/* ���������ݿ� */
int CppMySQL3DB::createDB(const char* name)
{
	char temp[1024];

	sprintf(temp, "CREATE DATABASE %s", name);

	if(!mysql_real_query( _db_ptr, temp, strlen(temp)) )
		return 0;
	
	else
		//ִ�в�ѯʧ��
		return -1;
}

/* ɾ���ƶ������ݿ�*/
int CppMySQL3DB::dropDB(const char*  name)
{
	char temp[1024];
	
	sprintf(temp, "DROP DATABASE %s", name);

	if(!mysql_real_query( _db_ptr, temp, strlen(temp)) )
		return 0;
	else
		//ִ�в�ѯʧ��
		return -1;
}

bool CppMySQL3DB::tableExists(const char* table)
{
	return false;
}

u_int CppMySQL3DB::lastRowId()
{
	return 0;
}

#ifdef WRITE_DBELOG
int MySqlDB_WriteErrorLog( int& errorCode ,const std::string& strError )
{
	int ret = 0;
	int iLevel=ULOG_FATAL, iModule=LOG_MODEL_DB, iSubModule=0;
	//int errorCode = 0;
	ret = C_LogManage::GetInstance()->WriteLog( iLevel,  iModule, iSubModule, errorCode, strError);
	return ret;
}
#endif // WRITE_DBELOG
