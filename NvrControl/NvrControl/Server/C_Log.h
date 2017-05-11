/****************************************************************************
*@file: C_Log.h
*@brief: ��־��ӡ��
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-09
****************************************************************************/
#ifndef C_LOG_H
#define C_LOG_H
#include <string>
enum Level
{
	Normal =0,
	Waring,
	Error
};
class CLog
{
public:
	CLog();
	~CLog();
	static void SetFilePath(const std::string& path );
	static void Write(const Level& lev, const std::string& text );
private:
	static std::string m_path;//�����ļ������ļ���·��
};
#endif // C_LOG_H
