/****************************************************************************
*@file: C_FileOperationsTool.h
*@brief: �����ļ����������࣬�Գ��õ��ļ����������˷�װ
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-10
****************************************************************************/
#ifndef FILEOPERATIONSTOOL_H
#define FILEOPERATIONSTOOL_H

#include <string>

class CFileOperationsTool
{
private:
	CFileOperationsTool();
	CFileOperationsTool(const CFileOperationsTool& obj);
	CFileOperationsTool& operator=(const CFileOperationsTool& obj );
public:
	static CFileOperationsTool* Instance();
public:
	/**
	*brief���ж�·���Ƿ����
	*/
	bool FileExist(const std::string& path );

	/**
	*brief��ɾ���ļ�·��,����Ŀ��Ŀ¼
	*/
	bool RemoveDirectoryPathIncFolder(const std::string& path );

	/**
	*brief��ɾ��·��
	*/
	bool RemoveDirectoryPath(const std::string& path );

	/**
	*brief��ɾ���ļ�
	*/
	bool RemoveFile(const std::string& path );

	/**
	*brief������·��
	*/
	bool CreateDirectoryPath(const std::string& path );

	/**
	*brief���޸��ļ�Ȩ��
	*/
	bool ChangeFilePermissions(const std::string& path );
private:
	bool CheckOwn(const std::string& path );
	
};

#endif // FILEOPERATIONSTOOL_H
