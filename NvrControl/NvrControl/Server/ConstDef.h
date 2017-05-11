/****************************************************************************
*@file: ConstDef.h
*@brief: ��Э����������ݽṹ�ͳ��������˶���
*		 
*@author: zhangchao@oristartech.com
*@dade: 2015-11-09
****************************************************************************/
#ifndef CONSTDEF_H
#define CONSTDEF_H
#include <string>

namespace FileCopyProtocol
{
	//Э��ͷ����
	typedef struct  ProtocolHeader{
		int typeno;//Э����
		int len;//Э�鳤��
	}ProtocolHeader;
	const int HEADERLENGTH =8;

	//Э����
	const unsigned int CREATEFILE =1001;//�ڶԵȻ����ϴ����ļ�
	const unsigned int CREATEFILE_REC =2001;//Server���ش����ļ����
	const unsigned int TRANSFERFILE =1002;//�����ļ�
	const unsigned int TRANSFERFILE_REC =2002;//Server���ش�����
	const unsigned int TRANSFERFILEFINISH =1003;//�ļ�������ɹر��ļ�
	const unsigned int TRANSFERFILEFINISH_REC =2003;//Server���ز������
	const unsigned int REMOVEDIRECTORYALL =1004;//�ļ��������ʱɾ������·��
	const unsigned int REMOVEDIRECTORYALL_REC =2004;//Server����ɾ������·�����
	const unsigned int REMOVEDIRECTORY =1005;//ɾ������Ŀ¼�µ������ļ�
	const unsigned int REMOVEDIRECTORY_REC =2005;//Server����ɾ������Ŀ¼�µ������ļ�����
	const unsigned int CREATEDIRECTORYNOTRM =1006;//����Ŀ¼�����Ŀ��Ŀ¼�������ٴ���
	const unsigned int CREATEDIRECTORYNOTRM_REC =2006;//Server���ؽ���Ŀ¼����
	const unsigned int CREATEDIRECTORYRM =1007;//����Ŀ¼,������ɾ�����ٴ���
	const unsigned int CREATEDIRECTORYRM_REC =2007;//Server���ؽ���Ŀ¼����
	const unsigned int CHANGEDIRECTORYOWN =1009;//�޸�Ŀ¼Ȩ��
	const unsigned int CHANGEDIRECTORYOWN_REC =2009;//Server���ؽ���Ŀ¼����

	//�����붨��
	enum Errorno
	{
		Success =0,	//�����ɹ�����ֵ
		ReveiveDataFailed=7000001, //�������ݽ���ʧ��
		CreateFileErr,	//�����ļ�ʧ��
		WriteFileFail,//�ļ�д��ʧ��
		WriteFileIncompletion,//�ļ�д�벻��ȫ
		//FileNotExist,//�ļ�������
		DirectoryNotExist,//·��������
		TransferFileFinishErr,//�ر��ļ�ʧ��
		TransferFileFinishNotOpen,//��Ҫ�رյ��ļ�δ��
		RemoveDirectoryALLErr,//ɾ������·��ʧ��
		RemoveDirectoryErr,//ɾ������Ŀ¼�µ������ļ�ʧ��
		CreateDirectoryNotRmEr,//����Ŀ¼,���ڲ�ɾ����ʧ��
		CreateDirectoryRmRmEr,//����Ŀ¼��ɾ���Ѿ����ڵ��ļ�ʧ��
		CreateDirectoryRmCreateEr,//����Ŀ¼�������ļ�ʧ��
		ChangeDirectoryOwnErr,//�޸�Ŀ¼Ȩ��ʧ��
		
	};

	//������Ϣ����
	const std::string ReveiveDataFailedStr ="Reveived the net data failed";//������������ʧ��
	const std::string CreateFileErrStr ="Create the file error";//�����ļ�ʧ��
	const std::string WriteFileFailStr ="Write data error";//�ļ�����д��ʧ��
	const std::string WriteFileIncompletionStr ="Write data incompleted";//�ļ�д�벻��ȫ
	//const std::string FileNotExistStr ="The file is not Exist";//�ļ�������
	const std::string DirectoryNotExistStr ="The directory is not Exist";//·��������
	const std::string TransferFileFinishErrStr ="Close file failed";//�ر��ļ�ʧ��
	const std::string TransferFileFinishNotOpenStr ="The file being closed is not open";//��Ҫ�رյ��ļ�δ��
	const std::string RemoveDirectoryALLErrStr ="Remove the directory which include the Folder Failed";//ɾ������·��ʧ��
	const std::string RemoveDirectoryErrStr ="Remove the directory which doesn't include the Folder Failed";//ɾ������Ŀ¼�µ������ļ�ʧ��
	const std::string CreateDirectoryNotRmErStr ="Create the directory which exist not remove Failed";//����Ŀ¼,���ڲ�ɾ����ʧ��
	const std::string CreateDirectoryRmRmErStr ="Delete the Exist directory Failed";//����Ŀ¼��ɾ���Ѿ����ڵ��ļ�ʧ��
	const std::string CreateDirectoryRmCreateErStr ="Create the directory Failed";//����Ŀ¼�������ļ�ʧ��
	const std::string ChangeDirectoryOwnErrStr ="Change the directory Permissions Failed"; //�޸�Ŀ¼Ȩ��ʧ��
}
#endif//CONSTDEF_H
