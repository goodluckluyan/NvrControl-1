//@file:C_constDef.h
//@brief: ��������״̬��Ϣ���塣
//@author:luyan@oristartech.com
//dade:2014-09-12

#ifndef IMONITOR_CONST_DEFINE
#define IMONITOR_CONST_DEFINE
#include "threadManage/C_CS.h"
#include <vector>
#include <string>
#include <map>


//const define

// �̳߳����̵߳ĸ�����
const int THREAD_COUNT_IN_POOL = 20;

//����ID
const int TASK_NUMBER_GET_DISK_STATUS = 0x0101;
const int TASK_NUMBER_GET_NET_STATUS = 0x0102;
const int TASK_NUMBER_DISPATCH_ROUTINE = 0x0201;
const int TASK_NUMBER_GET_HALL_STATUS = 0x0301;
const int TASK_NUMBER_GET_TMS_STATUS = 0x0401;

const int TASK_NUMBER_GET_OTHERMONITOR_STATUS = 0x0501;
const int TASK_NUMBER_GET_OTHERMONITOR_TMS_STATUS = 0x0502;
const int TASK_NUMBER_GET_OTHERMONITOR_SMS_STATUS = 0x0503;
const int TASK_NUMBER_GET_OTHERMONITOR_RAID_STATUS = 0x0504;
const int TASK_NUMBER_GET_OTHERMONITOR_ETH_STATUS = 0x0505;
const int TASK_NUMBER_GET_OTHERMONITOR_SWITCH_STATUS = 0x0506;
const int TASK_NUMBER_GET_OTHERMONITOR_SPEEDLIMIT_STATUS = 0x0507;
const int TASK_NUMBER_GET_OTHERMONITOR_SMSEW_STATUS = 0x0508;
const int TASK_NUMBER_GET_DB_SYNCN_STATUS = 0x0509;

const int TASK_NUMBER_PROCESS_USERINPUT = 0x0601;
const int TASK_NUMBER_CONDSWITCH_ROUTINE = 0x0701;
const int TASK_NUMBER_HASHCHECK_ROUTINE = 0x0801;
const int TASK_NUMBER_FILEOPERATION_ROUTINE = 0x0901;

const int TASK_NUMBER_REBOOT = 0x0902;
const int TASK_NUMBER_SHUTDOWN = 0x0903;
const int TASK_NUMBER_CHECKWATCHDOG = 0x0904;

// sms ״̬
const int SMS_STATE_SUCCESS = 101 ;    // ��������
const int SMS_STATE_ERROR = 102;       // �����쳣 / ���ӳ�ʱ
const int SMS_STATE_SYSERROR =103 ;    // ϵͳ�쳣

// sms ����״̬��
const int SMS_STATE_PLAYING = 201;     // ���ڲ���
const int SMS_STATE_PAUSING = 202;     // ��ͣ����	
const int SMS_STATE_FINISHED = 203;    // ֹͣ����
const int SMS_STATE_FINISHED_ABORT = 204; // �ֶ�ֹͣ
const int SMS_STATE_FINISHED_ERROR = 205; // ����ʧ��
const int SMS_STATE_NOT_EXECUTED = 206;   // ����ִ��

// sms ingest job ״̬
const int SMS_STATE_INGEST_SCHEULED = 301;    // �ȴ�����
const int SMS_STATE_INGEST_RUNNING = 302;     // ���ڵ���
const int SMS_STATE_INGEST_PAUSING = 303;     // ��ͣ����
const int SMS_STATE_INGEST_CHECK = 304;       // ������
const int SMS_STATE_INGEST_FINISHED = 305;    // ����ɹ�
const int SMS_STATE_INGEST_FINISHED_ERROR = 306;// ����ʧ��

// sms cpl check״̬
const int SMS_STATE_CPL = 401; 			// �ȴ���֤
const int SMS_STATE_CPL_RUNNING = 402;	// ������֤
const int SMS_STATE_CPL_PAUSING = 403;	// ��ͣ��֤
const int SMS_STATE_CPL_FINISHED = 404; // ��֤�ɹ�
const int SMS_STATE_CPL_FINISHED_ERROR = 405; // ��֤ʧ��

//��־����
const int ULOG_DEBUG	= 0;
const int ULOG_INFO	= 1;
const int ULOG_ERROR	= 2;
const int ULOG_FATAL	= 3;
const int UDEFAULT_LOG_LEVEL = 0;

//��־����ģ��
const int LOG_MODEL_THREADMGR = 1;
const int LOG_MODEL_TIMETASK = 2;
const int LOG_MODEL_DB = 3;
const int LOG_MODEL_WEBS = 4;
const int LOG_MODEL_JOBS = 5;
const int LOG_MODEL_OTHER = 6;
const int LOG_MODEL_LOGMGR = 7;

// �߳�״̬��
enum Thread_State
{
	INIT_STATE = 0, //��ʼ
	RUN_STATE = 1,  //����
	SUSPEND_IDL_STATE = 2, //����
	SUSPEND_FINISH_STATE = 3,//����ִ����ɶ���ͣ��
	RUN_LOCKED_STATE = 4,  // ����ʱ������
	SUSPEND_LOCKED_STATE = 5 ,// ��ͣʱ������
	QUIT_STATE =6	// �߳̽�����
};

// �߳��������ͣ����������߳�ִ�в��������͡�
enum Thread_Run_Type
{
	RUN_WEBSERVICE_TYPE = 0,	//�߳�����ִ��Webservice���á�
	RUN_TIMETASK_TYPE = 1,		// �߳�����ִ�ж�ʱ����
	RUN_UNKNOWN_TYPE = 2		// �߳�δִ�в�����
};

// ��������
enum TASK_TYPE
{
	NULL_TASK,// ��
	TIME_TASK,// ��ʱ����
	ONCE_TASK,// һ������
	ALWAYS_TASK// �̶�����
};

// ����״̬��
enum TASK_STATE
{
	//����
	TASK_IDLE_STATE = 0,

	// ����δ��ʼִ��
	TASK_NO_STATE =1,

	// ��������ִ�С�
	TASK_RUNNING_STATE = 2,

	// ����ִ����ɡ�
	TASK_FINISH_STATE  = 3,

	// ������Ҫ��ɾ��
	TASK_DELETE_STATE  = 4,

	//����ʱ������
	TASK_LOCKED_STATE  = 5
};


// RAID״̬
typedef struct DiskInfo
{
	std::string diskGroup;
	std::string diskSize;
	std::string diskState;
	std::string diskNumOfDrives;
	std::map<std::string ,struct DiskDriveInfo> diskDrives;
}DiskInfo;

typedef struct DiskDriveInfo
{
	int group ;
	std::string driveID;
	std::string drivePosition;
	std::string driveSlotNum;
	std::string driveErrorCount;
	std::string driveSize;
	std::string driveFirmwareState;
	std::string driveSpeed;
	std::string driveType;
	DiskDriveInfo():group(-1)
	{}

	DiskDriveInfo(const DiskDriveInfo &obj)
	{
		group=obj.group;
		driveID=obj.driveID;
		drivePosition=obj.drivePosition;
		driveSlotNum=obj.driveSlotNum;
		driveErrorCount=obj.driveErrorCount;
		driveSize=obj.driveSize;
		driveFirmwareState=obj.driveFirmwareState;
		driveSpeed=obj.driveSpeed;
		driveType=obj.driveType;
	}

	DiskDriveInfo & operator =(const DiskDriveInfo &obj)
	{
		if(this != &obj)
		{
			group=obj.group;
			driveID=obj.driveID;
			drivePosition=obj.drivePosition;
			driveSlotNum=obj.driveSlotNum;
			driveErrorCount=obj.driveErrorCount;
			driveSize=obj.driveSize;
			driveFirmwareState=obj.driveFirmwareState;
			driveSpeed=obj.driveSpeed;
			driveType=obj.driveType;
		}
		return *this;
	}
}DiskDriveInfo;


// ����
struct stError
{
	stError():
		nOrdinal(-1)
	{}
	int nOrdinal;
	std::string ErrorName;
	std::string ErrorVal;
};


// ����״̬
struct EthStatus
{
	std::string strName;
	int nTaskType;
	int nConnStatue;
	unsigned long long  nRxSpeed;
	unsigned long long nTxSpeed;

	EthStatus():
	nConnStatue(0),
		nRxSpeed(0),
		nTxSpeed(0),
		nTaskType(-1)
	{
	}
};



// SMS״̬
struct SMSStatus
{
	std::string hallid;
	int nStatus;
	int nRun;				//0������û������ 1���������У�2����һ̨���� 3:��һ̨Ҳû������
	int nPosition;
	std::string strSPLUuid;

	SMSStatus():
	nStatus(-1)
		,nRun(0)
		,nPosition(-1)
	{

	}

	SMSStatus(const SMSStatus &obj)
	{
		hallid = obj.hallid;
		nStatus = obj.nStatus;
		nRun = obj.nRun;
		nPosition = obj.nPosition;
		strSPLUuid = obj.strSPLUuid;
	}

	SMSStatus& operator=(const SMSStatus &obj)
	{
		if(this != &obj)
		{   
			hallid = obj.hallid;
			nStatus = obj.nStatus;
			nRun = obj.nRun;
			nPosition = obj.nPosition;
			strSPLUuid = obj.strSPLUuid;
		}
		return *this;
	}

};


// SMS����
struct SMSInfo
{
	std::string strId;		// ��ʶid�����к�
	std::string strIp;		// ͨ�ŵ�ip��ַ
	int nPort ;				// ͨ�ŵĶ˿�
	int nRole;				// ��ɫ������Ҫ���н�ɫ���Ǻ�ѡ��ɫ 1:Ϊ�������У�2:Ϊ�������� 3:�ӹ����� 
	std::string strExepath;// smsִ���ļ�·��
	std::string strConfpath;// �����ļ�·��
	SMSStatus stStatus;


	SMSInfo():
	nPort(-1)
		,nRole(-1)
	{}

	SMSInfo(const SMSInfo &obj)
	{
		strId = obj.strId;
		strIp = obj.strIp;
		nPort = obj.nPort;
		nRole = obj.nRole;
		strExepath = obj.strExepath;
		strConfpath = obj.strConfpath;
		stStatus = obj.stStatus;
	}

	SMSInfo& operator =(const SMSInfo &obj)
	{
		if(this != &obj)
		{
			strId = obj.strId;
			strIp = obj.strIp;
			nPort = obj.nPort;
			nRole = obj.nRole;
			strExepath = obj.strExepath;
			strConfpath = obj.strConfpath;
			stStatus = obj.stStatus;
		}
		return *this;
	}
};



#endif 
