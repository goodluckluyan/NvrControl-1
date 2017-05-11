
#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#include <algorithm>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include"../log/C_LogManage.h"
#include "../para/C_Para.h"
#define  LOG(errid,msg)  C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_JOBS,0,errid,msg)
#define  LOGERRFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_ERROR,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)
#define  LOGINFFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_INFO,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)

class CWatchdog
{
public:
	CWatchdog(std::string strExe,std::string strPath)
	{
		m_strExe = strExe;
		m_strPath = strPath;
		m_pid = -1;
	}

	~CWatchdog()
	{

	}

	// ��ȡָ�����̵�pid
	int GetPID(std::string strName,std::vector<int>& vecPID)
	{
		char acExe[256]={'\0'};
		snprintf(acExe,sizeof(acExe),"ps -ef|grep %s|grep -v \"grep %s\"|awk '{print $2}'",strName.c_str(),strName.c_str());
		FILE *pp = popen(acExe,"r");
		if(!pp)
		{
			LOG(0,"popen fail\n");
			return -1;
		}
		char tmpbuf[128]={'\0'};
		std::vector<std::string> vecBuf;
		while(fgets(tmpbuf,sizeof(tmpbuf),pp)!=NULL)
		{
			vecBuf.push_back(tmpbuf);
		}

		int nLen = vecBuf.size();
		for(int i = 0 ;i < nLen ;i++)
		{
			std::string &strtmp=vecBuf[i];
			int nStart = 0;
			int nPos = strtmp.find(' ',nStart);
			while(nPos != std::string::npos)
			{
				vecPID.push_back(atoi(strtmp.substr(nStart,nPos-nStart).c_str()));
				nStart = nPos+1;
				nPos = strtmp.find(' ',nStart);
			}
			vecPID.push_back(atoi(strtmp.substr(nStart).c_str()));
		}

		pclose(pp);
		return 0;
	}

	// ��̨����
	bool StartInCurTerminal(int &nPid)
	{
		LOGINFFMT(0,"Start_CurTerminal!");
		if(m_strPath.empty())
		{
			LOGERRFMT(0,"StartInCurTerminal:strExepath is Empty!");
			return false;
		}

		std::vector<int> vecCurPID;
		if(GetPID(m_strExe,vecCurPID) < 0)
		{
			LOGERRFMT(0,"StartInCurTerminal Getpid Failed !");
			return false;
		}

		struct rlimit rl;
		if(getrlimit(RLIMIT_NOFILE,&rl)<0)
		{
			LOGERRFMT(0,"StartInCurTerminal:getrlimit failed!");
			return false;
		}

		GlobalStatus::GetInstinct()->m_mutxSignal.EnterCS();

		pid_t cpid ;
		if((cpid = fork()) < 0)
		{
			perror("StartInCurTerminal:failed to create process!");
			return false;
		}
		else if(cpid == 0)
		{
			//��ò�Ҫ��fork��ʹ��LOGINFFMT��־�������Ϊ��־��ʹ����mutx���ܻ���forkʱ��������
//			LOGINFFMT(0,"Fork Process(%d) Start %s ... \n",getpid(),m_strExe.c_str());

			// �ر����и����̴򿪵��ļ��������������ӽ��̼̳и����̴򿪵Ķ˿ڡ�
			if(rl.rlim_max == RLIM_INFINITY)
			{
				rl.rlim_max = 1024;
			}
			for(int i = 3 ;i < rl.rlim_max;i++)
			{
				close(i);
			}


			// �ٴ�fork��ֹ���ֽ�ʬ����
			int ccpid=0;
			if((ccpid = fork()) < 0)
			{
				perror("StartInCurTerminal:failed to create process!");
				return false;
			}
			else if(ccpid > 0)
			{
				exit(0);
			}

			// Ϊ�˷�ֹSMSҪ��ȡ���ӽ��̵�״̬ʱʧ�ܣ����԰�SIGCHLD�źŴ������Ĭ�ϴ���ʽ��
			// ��ΪSMS��̳е���������źŴ���ʽ,���������SIGCHLD�źŴ������Ǻ��ԡ�
// 			struct sigaction sa;
// 			sa.sa_handler=SIG_DFL;
// 			sigemptyset(&sa.sa_mask);
// 			sa.sa_flags = 0;
// 			if(sigaction(SIGCHLD,&sa,NULL)<0)
// 			{
// 				perror("Start_CurTerminal::Cannot Set SIGCHLD Signal Catchfun! ");
// 			}

			chdir(m_strPath.c_str());
			std::string strEXE = m_strExe + "&";
			

			std::string tmp;
			if(m_strPath.rfind('/')!=m_strPath.size()-1)
			{
				tmp = m_strPath+"/"+m_strExe;
			}
			else
			{
				tmp = m_strPath+m_strExe;
			}

			if(!strEXE.empty() && execl(tmp.c_str(),strEXE.c_str(),NULL) < 0)
			{
				perror("execl error");
				exit(0);
			}
		}

		// �ļ�Ǩ��ʱ����ź�SIGCHID����ʽ��ΪĬ�ϣ�Ϊ�˷�ֹ��ʱ��������
		// ���Խ����ж�SIGCHID����ʽ
// 		struct sigaction cursa;
// 		if(sigaction(SIGCHLD,NULL,&cursa)<0)
// 		{
// 			LOGINFFMT(ULOG_ERROR,"StartInCurTerminal::Cannot Set SIGCHLD Signal Catchfun! ");
// 		}

// 		if(cursa.sa_handler==SIG_DFL)
// 		{
// 			int nStatus;
// 			waitpid(cpid,&nStatus,NULL);
// 		}

		GlobalStatus::GetInstinct()->m_mutxSignal.LeaveCS();
		bool bRun = false;
		int exepid = 0;
		time_t tm1;
		time(&tm1);
		while(!bRun)
		{
			sleep(1);
			std::vector<int> vecNowPID;
			if(GetPID(m_strExe,vecNowPID) < 0)
			{
				LOGERRFMT(0,"StartInCurTerminal Getpid Failed (Start %s)!",m_strExe.c_str());
				return false;
			}

			std::vector<int>::iterator fit;
			for(int i = 0 ;i <vecNowPID.size();i++)
			{
				fit = std::find(vecCurPID.begin(),vecCurPID.end(),vecNowPID[i]);
				if(fit == vecCurPID.end())
				{
					exepid = vecNowPID[i];
					bRun = true;
					LOGINFFMT(0,"Fork Process(%d) Start %s ... \n",exepid,m_strExe.c_str());
					break;
				}
			}

			time_t tm2;
			time(&tm2);
			if( tm2-tm1 > 5)
			{
				LOGINFFMT(0,"waiting 5 sec ,but %s not run..\n",m_strExe.c_str());
				break;
			}
		}

		if(exepid > 0)
		{	
			m_pid = exepid;
			nPid = exepid;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsRun()
	{
		std::vector<int> vecPID;
		GetPID(m_strExe,vecPID);
		if(vecPID.size()>0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
 

private:
	std::string m_strExe;
	std::string m_strPath;
	int m_pid;
};


#endif