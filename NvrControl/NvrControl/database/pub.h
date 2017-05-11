#ifndef PUB_H
#define PUB_H
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../para/ec_config.h"
using namespace std;


#define MAX_BUFF_LEN	255

#define CONDITION_AND   1
#define CONDITION_OR    0
#define BUFF_SIZE_20    20
#define BUFF_SIZE_50	50
#define BUFF_SIZE_255	255
#define BUFF_SIZE_2048	2048
#define BUFF_SIZE_1024	1024

typedef struct _SQL_CONDITION_VALUE
{
	char    table_name[BUFF_SIZE_20];
	char    cloume_name[BUFF_SIZE_20];
	char    cloume_condition1[BUFF_SIZE_20];
	char    cloume_value1[BUFF_SIZE_20];
	char    cloume_condition2[BUFF_SIZE_20];
	char    cloume_value2[BUFF_SIZE_20];
	///value1��value2֮��Ĺ�ϵ��or������and��
	char    inside_relation[BUFF_SIZE_20];
	///����һ�������Ĺ�ϵ��or������and��
	char    outside_relation[BUFF_SIZE_20];

}SQL_CONDITION_VALUE;

typedef struct _SQL_CONDITION_RULE
{
	///���в�ѯ������ֶ�  
	char	order_cloume[BUFF_SIZE_20];
	///��ѯ�����˳��"desc"����"asc"
	char    order[BUFF_SIZE_20];
	///��ѯlimite�ķ�Χ��max��min�Ĳ�ѯ���
	char	limit_max[BUFF_SIZE_20];
	char    limit_min[BUFF_SIZE_20];
}SQL_CONDITION_RULE;

typedef vector<SQL_CONDITION_VALUE>  VEC_QUERY_VALUE;

typedef struct _COLUMN_VALUE_MAP
{
	char column_name[BUFF_SIZE_50];
	char *pvalue_var;
}COLUMN_VALUE_MAP;

class PubFun
{
public:
	~PubFun();


	PubFun();
	int GetTableState(const char *tablename,int *pstate,char *pstata_desc,int maxlen_desc);
protected:

public:
	int FillSqlBufferByCondition(char* sql ,VEC_QUERY_VALUE sql_value,SQL_CONDITION_RULE sql_rule);	
	int FillSqlBufferByStr(char *sql, char* str);
    int FillCondition(VEC_QUERY_VALUE query,char* tablename,
                      char* cloumename, char* condition1,
                      char* value1, char *conditon2,
                      char* value2, char* in_relation,
                      char* out_relation, char*  order_name,
                      char* order, char* limitemin, char* limitemax);
	
	int GetCountsBySql(char	*count, char *sql);

	/// ��ȡ������Ϣ
	int GetOneRowBySql(char *row_buffer, char *sqlbuffer);

	/// gengkeming add 20150318
	/// ����
	int UpdateColumnValue(int id,const char *ptable,const char *pcolumn,const char *pvalue);

	/// ����ӳ�������
	int CopyMapArray(COLUMN_VALUE_MAP column_map_dest[],COLUMN_VALUE_MAP column_map_src[],int column_count);

	/// mysql ���ݿ����ӳ�
#if 0
	Connection_T ConnectionPool_getConnection_pool(ConnectionPool_T mysql_pool);
	ResultSet_T Connection_executeQuery_pool(Connection_T con,const char *psql);
	void Connection_close_pool(Connection_T con);
	int ResultSet_next_pool(ResultSet_T rset);
	const char *ResultSet_getString_pool(ResultSet_T rset,int column_index);
#endif
	void assert_pool(void *pvoid);

public:
	///��ȡ���ݿ���Ϣ�������ļ���
	int GetDataBaseInfoFromConfigFile(char *pdbname,
												char *pipaddress,
												char *pusername,
												char *ppassword);


	int MysqlQuery(MYSQL **conn,const char *psql);

	MYSQL_RES *MysqlUseResult(MYSQL *conn);

	MYSQL_ROW MysqlFetchRow(MYSQL_RES *pres);

	int MysqlFreeResult(MYSQL_RES *pres);
	/// �������ݿ�
	int Connect(MYSQL **conn);
	/// �Ͽ����ݿ�����
	int DisConnected(MYSQL **conn);
};

#endif