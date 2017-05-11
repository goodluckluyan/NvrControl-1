#ifndef _FILEMANAGER_LINUX_H
#define _FILEMANAGER_LINUX_H
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <dirent.h> 
#include <memory.h>
#include <ctype.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/time.h>

using namespace std;

#ifndef LPVOID
#define LPVOID void *
#endif

#ifndef BUFF_SIZE_50
#define BUFF_SIZE_50 50
#endif

/// GKM:Ӧ�ó������� DLL EXE
typedef enum _EXE_TYPE { EXE, DLL }EXE_TYPE, *pEXE_TYPE;

/// GKM:�ļ���������
typedef enum { FIND_DIR, FIND_FILE, FIND_ALL }FINDFILE_TYPE, *pFINDFILE_TYPE;

/// GKM:�ļ���Ϣ����
typedef enum { FILEINFO_DIR, FILEINFO_FILE, FILEINFO_LINK,FILEINFO_OTHRER }FILEINFO_TYPE, *pFILEINFO_TYPE;

/// �ļ�����״̬
typedef enum { COPY_STOP,COPY_COPYING,COPY_FINISHED,COPY_FINISHED_ERROR}FILE_COPY_STATE;

/// GKM:�ļ���Ϣ�ṹ����
#if 0
typedef struct _FILE_INFO
{
	char *pfilepath;
	char *pfilename;
	char *pfilefullname;
	FILEINFO_TYPE filetype;
	long long filesize;
	struct stat64 file_st;
}FILE_INFO,*pFILE_INFO;

typedef vector<FILE_INFO *>FILE_INFOS;
#endif

//////////////////////////
typedef struct _REPORT_STATUS REPORT_STATUS;

#define FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE						31000
#define ERROR_INPUT_PARMETER_INVALID									1
#define ERROR_FILE_NOT_EXIST											2
#define ERROR_READFILE_NOT_INITIALIZE									3
#define ERROR_FILE_ISEMPTY												4
#define ERROR_READFILE_READ_OVER										5
#define ERROR_READFILE_READ_ERROR										6
const int ERROR_FILEMANAGER_MODULE_INPUT_PARMETER_INVALID				= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_INPUT_PARMETER_INVALID;
const int ERROR_FILEMANAGER_MODULE_FILE_NOT_EXIST						= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_FILE_NOT_EXIST;
const int ERROR_FILEMANAGER_MODULE_READFILE_NOT_INITIALIZE				= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_READFILE_NOT_INITIALIZE;
const int ERROR_FILEMANAGER_MODULE_FILE_ISEMPTY							= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_FILE_ISEMPTY;
const int ERROR_FILEMANAGER_MODULE_READFILE_READ_OVER					= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_READFILE_READ_OVER;
const int ERROR_FILEMANAGER_MODULE_READFILE_READ_ERROR					= FILEMANAGER_MODULE_ERROR_CODE_BASE_VALUE + ERROR_READFILE_READ_ERROR;
///////////////////////////

class CFileManager
{
public:
	CFileManager(void);
	~CFileManager(void);
	
//////////////////		
	/// �����Ƿ���pingͨ
	int PingIsOk(const char *pipaddress,int *pisok);

#if 0
	/// ·���Ƿ�ΪĿ¼
	int IsDir(const char *pdirpath,int *pisdir);

	/// �ļ�·���Ƿ����
	int IsExistPath(const char *ppath,int *pisexist);

	/// ����Ŀ¼
	int CreateDir(const char *pdirpath);

	/// �����ļ�
	int CreateFile(const char *pfilepath);

	/// ɾ��Ŀ¼
	int DeleteDirOrFile(const char *ppath);

	/// ��ȡ·���µ������ļ���Ŀ¼
	int GetFileFullNamesByPath(const char *ppath,char (*pfilefullnames)[BUFF_SIZE_50],int maxcount,int max_len,int *pfilefullname_count);

	/// ��ȡ·���µ������ļ���Ŀ¼
	int IsEmptyDir(const char *ppath,int *pisempty);

////////////

	/// GKM:����ָ��·�������ļ�
	int FindFileByPath(const char *pfilepath, FILE_INFOS *pfileinfos, FINDFILE_TYPE findFileType, int isSubDir);
	
	/// GKM:��ȡ��ִ�г����·��
	int GetLocalPath(char *ppath,int maxlen);
	
	/// GKM:��ȡ�ļ���Ϣ
	int GetFileInfo(const char *pfileFullName, FILE_INFO *pfileinfo);

	/// GKM:��ȡ�ļ����ڵ�·��
	int GetFilePath(const char *pfileFullName,int type,char *pfilePath,int maxlen_filepath);

	/// GKM:��ȡ�ļ�������
	int GetFileName(const char *pfileFullName,int type,char *pfileName,int maxlen_filename);

	/// GKM:��Դ�ַ�����ȥ�������ַ���
	int GetFilterString(const char *strSrc, char *strFilter, char *strDest);

	/// @brief ��ȡ�ļ���С
	int GetFileSize(const char *pfullpath,long long *pfilesize);

	/// @brief ��ȡ�ļ���С
	int GetFileSizeS(const char *pfullpath,char *psize,int maxlen);
	
		/// ��ȡ������Ϣ�����̴�С�����ÿռ䣬���ÿռ䣬���ðٷֱȣ�
	int GetDiskInfo_S(const char *pdiskdir,
					char *pdisksize,
					int maxlen_disksize,
					char *pdiskused,
					int maxlen_diskused,
					char *pdiskavail,
					int maxlen_diskavail,
					char *pdiskusedpercent,
					int maxlen_diskusedpercent
					);

	/// ��ȡ���λ����
	int GetMaxUnitSize(char *pmaxunit,int maxlen_maxunit,long long maxsize);

	/// �ļ��Ƿ����
	bool IsExistFile(const char *pfullpath);

	/// ·���Ƿ����
	bool IsExistPath(const char *ppath);

	/// ����·���ַ���Ϊ�Ϸ������·���г����������·���ָ��������,ʹ������һ��
	int CorrectPathString(char *ppath,int maxlen,int type);

	/// ����·���ַ���
	int GeneratePathString(char *ppath,int type,int maxlen,const char *pathmsg,...);

	/// ��·�����Ϸָ�����windows "\\" linux "/"��������л��������ָ����������һ��,
	/// flag = 0,��·����ʼλ�������ң�flag = 1,��·������λ����ǰ���ң�flag = other,��·���������м���ҡ�
	int AddSeparator(char *ppath,int flag,int type);
	/// ��·����ȥ�ָ�����windows "\\" linux "/"��������л��������ָ�������ȫ����ȥ,
	/// flag = 0,��·����ʼλ�������ң�flag = 1,��·������λ����ǰ���ң�flag = other,��·���������м���ҡ�
	int SubtractSeparator(char *ppath,int flag,int type);
	
	/// ���ת���ַ�'\'����Ӧmysql�����������·�������
	int ConrrectToHaveESCPathString(char *ppath,int maxlen,int type);

	/// ��ȡ�ļ�����׺,���һ������Ϊ�ָ���
	int GetPostFixInFileName(char *ppostfix,int maxlen,const char *pfilename);

	/// ��ȡ�ļ�������������׺�����һ������Ϊ�ָ���
	int GetNameInFileName(char *pname,int maxlen,const char *pfilename);
	
	/// ������Ժ���
	int EmptyLang();
	
	/// �ַ��滻���� ���ַ����е�ĳ�ַ�ȫ���滻��ָ���ַ�
	int ReplaceCharInString(char *pbuff,int max_len,char ch_from,char ch_to);
	
	/// �����ʽת��
	int code_convert(const char *pcharset_from,
						const char *pcharset_to,
						char *pbuff_in,
						size_t len_in,
						char *pbuff_out,
						size_t maxlen_out,
						size_t *plen_out);
						
	int ReplaceStr(char *pbuff_src,const char *pbuff_from,const char *pbuff_to);


    int DeleteCharA(char *pbuff_src,char ch);


	int testsplit();
	int split_parse(char *pbuffer,int buffer_len,char (*split_buffers)[20],int maxcount,int max_len,int *pbuffer_count,char *psplit_tmp,int split_tmp_len);

	int killmountlast(const char *pmountdest);
	
	int get_exe_path( char * buf, int count);

	int GetImbSM_time(const char *pipaddress,const char *pchain,const char *pprivate,char *presult,int maxlen);
	int SetImbSM_time(const char *pipaddress,const char *pchain,const char *pprivate,int diff_seconds);

public:
	int DeleteFIle_M(const char *pfilefullpath);

	/// GKM:�����ļ������������̣߳�������hash
	int CopyFile_M(const char *pdestfullpath,const char *psrcfullpath);
	/// GKM:�����ļ��������������̣߳�������hash����������Ҫ��֤hash��С�ļ���
	int CopyFile_M1(const char *pdestfullpath,const char *psrcfullpath);
	/// GKM:ֹͣ�ļ�����
	int StopCopyFile_M();

	/// GKM:�ƶ��ļ�
	int MoveFile_M(const char *pdestfullpath,const char *psrcfullpath);

	/// �����߳�
	int copy_procedure();

	/// ��ȡ��������
	int GetCopyingState(FILE_COPY_STATE *pstate,
							long long *psize_total,
							long long *psize_elpased,
							long long *psize_remain,
							int *pcopyingbytespersecond);
							
	/// �Ƿ����ڿ����������߳��Ƿ��������У�
	int IsCopying(int *piscopying);	
	
	int StopCopy();	
	
	int ResetCopyState();

    /// gkm add 20141216
    int SetCopyRate(int copyrate_m);

    /// gkm add 20141216
    int GetCopyRate(int *pcopyrate_m);
	
#if 0	
	/// ��ȡhashֵ
	int GetHash(char *pbuff_hash,int *plen_hash,int max_len);
#endif	

#if 0
	/// ��ȡhashֵ
	int GetHash(char *pbuff_hash,int *plen_hash,int max_len);
#endif

	/// ��ȡ�����ٶȣ��ֽ�/�룩
	int GetCopyingSpeed(int *pcopyingbytespersecond);

	/// ��ȡ�Ѿ�����������
	int GetCopiedElapsedSeconds(int *pelapsedseconds);

	/// �����ļ���С��ȡ��������Ҫ��ʱ��
	int GetCopySeondsBySize(int *pcopiedseconds,long long filesize);

	void file_seek(const char *dir, int depth,FILE_INFOS *pfileinfos);
#endif
public:
	int InitReadFile(const char *pfilefullpath,long long &file_size);
	int ReadFile(unsigned char *pbuff,int max_len,int &len,long long &elapsed_size,int &isfinished);
	int UninitReadFile();
	//int GetErrorString(int errorcode,REPORT_STATUS *preportstatus);

public:
	//int NewSpaceFileInfo(FILE_INFO **pfileinfo);
	//int DeleteSpaceFileInfo(FILE_INFO **pfileinfo);
	//int ClearSpaceFileInfo(FILE_INFO *pfileinfo);
	//int DeleteSpaceFileInfos(FILE_INFOS *pfileinfos);	
	
private:
	char m_descfullpath[255];
	char m_sourcefullpath[255];
//	char m_buff_filehash[255];
//	int m_len_filehash;
	long long m_size_copyingfile;
	long long m_size_copiedelapsed;
	FILE_COPY_STATE m_copy_state;	/// 0 schedule 1 copying  2 finished  3 finished_error
	int m_bcopying;	
	int m_copy_control_stop;	
	time_t m_time_copystart;
	int m_isrunning;
    /// gkm add 20141216 start
    int m_wait_loop_counter;
    int m_min_loop_counter;
    int m_max_loop_counter;
    int m_copy_rate;
    int m_copy_rate_real;
    long long m_size_copiedelapsed_real;
    time_t m_time_elapsed_start_real;
    /// gkm add 20141216 end
	int m_thread_isexist;

	FILE *m_pf_read;
	char m_fullpath_read[255];
	long long m_totalsize_readfile;
	long long m_elapsed_readsize;
	int m_isfinished_read;
	int m_errorcode_read;

		
};
#endif
