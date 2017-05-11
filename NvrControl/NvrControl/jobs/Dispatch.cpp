#include "Dispatch.h"
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include "../log/C_LogManage.h"
#include "../para/C_Para.h"
#include "Invoke.h"

#define  LOG(errid,msg)  C_LogManage::GetInstance()->WriteLog(ULOG_FATAL,LOG_MODEL_JOBS,0,errid,msg)
#define  LOGERRFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_ERROR,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)
#define  LOGINFFMT(errid,fmt,...)  C_LogManage::GetInstance()->WriteLogFmt(ULOG_INFO,LOG_MODEL_JOBS,0,errid,fmt,##__VA_ARGS__)


using namespace xercesc;
CDispatch::CDispatch(void * ptrInvoker)
:m_ptrInvoker(ptrInvoker)
{
	pthread_cond_init(&cond,NULL);
}

CDispatch::~CDispatch()
{
	struct DispatchTask nodeTask;
	nodeTask.nDTriggerType = NULLTask;
	m_csLDTask.EnterCS();
	m_lstDTask.push_back(nodeTask);
	m_csLDTask.LeaveCS();
	pthread_cond_signal(&cond);

	pthread_cond_destroy(&cond);
//	xercesc::XMLPlatformUtils::Terminate();
}

// ��ʼ��
bool CDispatch::Init(std::string strPath)
{
// 	try
// 	{
// 		xercesc::XMLPlatformUtils::Initialize();
// 	}
// 	catch( xercesc::XMLException& e )
// 	{
// 		char* message = xercesc::XMLString::transcode( e.getMessage() );
// 		xercesc::XMLString::release( &message );
// 	}

	if(access(strPath.c_str(),R_OK) < 0)
	{
		LOGERRFMT(0,"CDispatch Init Read %s Failed!",strPath.c_str());
		//xercesc::XMLPlatformUtils::Terminate();
		return false;
	}

	ParsePolicy(strPath);
	

	
	return true;
}

// �������Ȳ���xml
bool CDispatch::ParsePolicy(std::string strPath)
{
	XercesDOMParser *ptrParser = new  XercesDOMParser;
	ptrParser->setValidationScheme(  XercesDOMParser::Val_Never );
	ptrParser->setDoNamespaces( true );
	ptrParser->setDoSchema( false );
	ptrParser->setLoadExternalDTD( false );

	try
	{
		ptrParser->parse(strPath.c_str());
		DOMDocument* ptrDoc = ptrParser->getDocument();	
		std::map<int,std::string> mapTmp;
		mapTmp[RAIDTask] = "Raid";
		mapTmp[ETHTask] = "Eth";
		mapTmp[SMSTask] = "SMS";
		mapTmp[TMSTask] = "TMS";
		mapTmp[IMonitorTask] = "IMonitor";
		mapTmp[ETH] = "Eth";

 
		std::map<int,std::string>::iterator it = mapTmp.begin();
		for(;it != mapTmp.end();it++)
		{
			struct PolicyInfo stPI;
			stPI.nPolicyDevType = it->first;
			if(GetPolicyNode(ptrDoc,it->second,stPI.mapPInfo))
			{
				m_mapPolicy[it->first] = stPI;
			}
		}
	}
	catch(  XMLException& e )
	{
		char* message =  XMLString::transcode(e.getMessage());
		XMLString::release( &message );
		LOG(ERROR_PARSE_MONITORSTATE_XML,message);
		delete ptrParser;
		ptrParser = NULL;
	}
	delete ptrParser;
	ptrParser = NULL;
	return true;
}

// �����ڲ��ڵ�
bool CDispatch::GetPolicyNode(DOMDocument* ptrDoc,std::string strNodeName,std::map<std::string,PolicyInfoEle> &mapPInfo)
{
	DOMNodeList *ptrDevNode = ptrDoc->getElementsByTagName(C2X(strNodeName.c_str()));
	if(ptrDevNode->getLength() == 0)
	{
		return false;
	}

	DOMNodeList *ptrDevChildNode = ptrDevNode->item(0)->getChildNodes();
	int nLen = ptrDevChildNode->getLength();	
	for(int i =0; i < nLen ;i++)
	{
		DOMNode *node = ptrDevChildNode->item(i);
		if (node->getNodeType() != DOMNode::ELEMENT_NODE) 
		{
			continue;
		}
		
		PolicyInfoEle pinfo;
		char * nodeName = XMLString::transcode(node->getNodeName());//��ȡ�ýڵ������
		std::string nodeName_str = nodeName;
		pinfo.strErrName = nodeName_str;

		//����ýڵ��������ԣ������������
		if (node->hasAttributes()) 
		{
			DOMNamedNodeMap *pAttributes = node->getAttributes();
			const XMLSize_t nSize = pAttributes->getLength();

			//�����ýڵ��µ���������
			for (XMLSize_t i = 0; i < nSize; ++i) 
			{
				DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(i);
				char *name = XMLString::transcode(pAttributeNode->getName());//��ȡ������
				std::string name_str = name;
				if (name_str == "type") 
				{
					char *attrValue = XMLString::transcode(pAttributeNode->getValue());//��ȡ����ֵ
					pinfo.strType = attrValue;
					XMLString::release(&attrValue);//�ͷ���Դ
				}
				if (name_str == "priority") 
				{
					char *attrValue = XMLString::transcode(pAttributeNode->getValue());//��ȡ����ֵ
					pinfo.nPriority = atoi(attrValue);
					XMLString::release(&attrValue);//�ͷ���Դ
				}
				XMLString::release(&name);
			}
		}
		XMLString::release(&nodeName);

		if (node->hasChildNodes()) 
		{
			DOMNode * child;
			for (child = node->getFirstChild(); child != 0; child = child->getNextSibling()) 
			{
				if (child->getNodeType() != DOMNode::ELEMENT_NODE) 
				{
					continue;
				}

				char * nodeName = XMLString::transcode(child->getNodeName());//��ȡ�ýڵ������
				std::string nodeName_str = nodeName;
				if(!child->hasAttributes())
				{
					continue;
				}
				DOMNamedNodeMap *pAttributes = child->getAttributes();
				DOMAttr *pAttributeNode = (DOMAttr*) pAttributes->item(0);
				char *name = XMLString::transcode(pAttributeNode->getName());//��ȡ������
				std::string name_str = name;
				if (name_str == "value") 
				{
					char *attrValue = XMLString::transcode(pAttributeNode->getValue());//��ȡ����ֵ
					if(nodeName_str == "fault")
					{
						pinfo.strFault = attrValue;
					}
					else if(nodeName_str == "action")
					{
						pinfo.vecAct.push_back(attrValue);
					}
					XMLString::release(&attrValue);//�ͷ���Դ
				}
				XMLString::release(&name);
				XMLString::release(&nodeName);
			}
		}
		mapPInfo[pinfo.strErrName] = pinfo;
	}
	return true;
}

// ���raid������������
bool CDispatch::TriggerDispatch(int nTaskType,std::vector<stError> &vecErr)
{
	struct DispatchTask nodeTask;
	nodeTask.nDTriggerType = (enDTriggerType)nTaskType;
	nodeTask.vecErr = vecErr;
	m_csLDTask.EnterCS();
	m_lstDTask.push_back(nodeTask);
	m_csLDTask.LeaveCS();
	pthread_cond_signal(&cond);
	

	return true;
}



// ִ�е�������
bool CDispatch::Routine()
{
	struct DispatchTask nodeTask;
	m_csLDTask.EnterCS();
	while(m_lstDTask.size() == 0)
	{
		pthread_cond_wait(&cond,&(m_csLDTask.m_CS));
	}

	
	nodeTask = m_lstDTask.front();
	m_lstDTask.pop_front();
	
	m_csLDTask.LeaveCS();

	switch(nodeTask.nDTriggerType)
	{
	case RAIDTask:
	case ETHTask:	
	case SMSTask:	
	case TMSTask:
	case IMonitorTask:
	case ETH:
		{
			std::map<int,std::vector<std::string> > mapAction;
			ApplyPolicy(nodeTask.nDTriggerType,nodeTask,mapAction);
			ExeCmd(mapAction);
		}
		break;
	}
}

// ִ�ж���
void CDispatch::ExeCmd(std::map<int,std::vector<std::string> > &mapAction)
{
	std::map<int,std::vector<std::string> >::iterator it = mapAction.begin();
	for(;it != mapAction.end();it++)
	{
		switch(it->first)
		{
		case LOGCmd:
			{
				std::vector<std::string> &vecStr = it->second;
				for(int i = 0;i < vecStr.size();i++)
				{
					LOGERRFMT(ERROR_DEVSTATUS_FAULT,"%s",vecStr[i].c_str());
				}
			}
			break;
		case POLICYCmd:
			{
				std::vector<std::string> &vecStr = it->second;
				vecStr.erase(std::unique(vecStr.begin(),vecStr.end()),vecStr.end());
				for(int i = 0;i < vecStr.size();i++)
				{
					TrimSpace(vecStr[i]);
					if(vecStr[i] == "SwitchALLSMS" && m_ptrInvoker)	
					{
						((CInvoke*)m_ptrInvoker)->SwitchAllSMS();	
					}
					else if(vecStr[i] == "StartTMS" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->StartTMS();
					}
					else if(vecStr[i] == "Exit" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->Exit();
					}
					else if(vecStr[i] == "StartALLSMS" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->StartALLSMS();
					}
					else if(vecStr[i] == "TakeOverMain" && m_ptrInvoker)
					{
// 						if(!C_Para::GetInstance()->IsMain())
// 						{
							((CInvoke*)m_ptrInvoker)->TakeOverMain();
//						}
					}
					else if(vecStr[i] == "ChangeToStdby" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->ChangeToStdby();
					}
					else if(vecStr[i] == "TakeOverStdby" && m_ptrInvoker)
					{
//						if(C_Para::GetInstance()->IsMain())
//						{
							((CInvoke*)m_ptrInvoker)->TakeOverStdby();
//						}
					}
					else if(vecStr[i] == "ChangeToMain" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->ChangeToMain();
					}
					else if(vecStr[i] == "SolveConflict" && m_ptrInvoker)
					{
						((CInvoke*)m_ptrInvoker)->SolveConflict(m_vecConflict);
						m_vecConflict.clear();
					}
					
				}
			}
			break;
		}

	}
}

// ɾ������ո�
void CDispatch::TrimSpace(std::string &str)
{
	std::string tmp=str;
	str = tmp.assign(tmp.begin()+tmp.find_first_not_of(' '),tmp.begin()+tmp.find_last_not_of(' ')+1);
}

// �����Խ��е���
bool CDispatch::ApplyPolicy(int nTaskType,struct DispatchTask &nodeTask,std::map<int,std::vector<std::string> >& mapAction)
{	
	if(nodeTask.nDTriggerType != nTaskType)
	{
		return false;
	}

	std::map<int,PolicyInfo>::iterator fit = m_mapPolicy.find(nTaskType);
	if(fit == m_mapPolicy.end())
	{
		return false;
	}

	std::vector<PolicyInfoEle> vecPIEAct;
	PolicyInfo& stPI = fit->second;
	int nLen = nodeTask.vecErr.size();
	for(int i = 0 ;i < nLen ;i++)
	{
		stError &err = nodeTask.vecErr[i];
		std::map<std::string,PolicyInfoEle>::iterator fele = stPI.mapPInfo.find(err.ErrorName);
		if(fele == stPI.mapPInfo.end())
		{
			continue;
		}

		PolicyInfoEle &stPE = fele->second;
		if(stPE.strType == "value" && stPE.strFault == err.ErrorVal)
		{
			std::string strAct;
			for(int i=0 ;i<stPE.vecAct.size();i++)
			{
				strAct+=stPE.vecAct[i];
			}
			char buf[128];
			snprintf(buf,sizeof(buf),"***Event Trigger:%s%d value: %s = Policy:%s->%s",err.ErrorName.c_str(),err.nOrdinal,
				err.ErrorVal.c_str(),stPE.strFault.c_str(),strAct.c_str());
			mapAction[LOGCmd].push_back(std::string(buf));
		
			//��ȡ����
			vecPIEAct.push_back(stPE);
		}

		if(stPE.strType == "exrange")
		{
			int nPos = stPE.strFault.find(',');
			if(nPos != std::string::npos)
			{
				int nMin = atoi(stPE.strFault.substr(0,nPos).c_str());
				int nMax = atoi(stPE.strFault.substr(nPos+1).c_str());
				if(nMin>nMax)
				{
					int tmp = nMin;
					nMin = nMax;
					nMax = tmp;
				}
				int errvalue = atoi(err.ErrorVal.c_str());
				if(errvalue < nMin || errvalue > nMax)
				{
					std::string strAct;
					for(int i=0 ;i<stPE.vecAct.size();i++)
					{
					  strAct+=stPE.vecAct[i];
					}
					char buf[128];
					snprintf(buf,sizeof(buf),"*** Event Trigger:%s value = %s  = Policy:%s -> %s ***",err.ErrorName.c_str(),
						err.ErrorVal.c_str(),stPE.strFault.c_str(),strAct.c_str());
					mapAction[LOGCmd].push_back(std::string(buf));

					//��ȡ����
					vecPIEAct.push_back(stPE);
				}
			}
		}

		if(stPE.strType == "inrange")
		{
			int nPos = stPE.strFault.find(',');
			if(nPos != std::string::npos)
			{
				int nMin = atoi(stPE.strFault.substr(0,nPos).c_str());
				int nMax = atoi(stPE.strFault.substr(nPos+1).c_str());
				if(nMin>nMax)
				{
					int tmp = nMin;
					nMin = nMax;
					nMax = tmp;
				}
				int errvalue = atoi(err.ErrorVal.c_str());
				if(errvalue >= nMin && errvalue <= nMax)
				{
					std::string strAct;
					for(int i=0 ;i<stPE.vecAct.size();i++)
					{
						strAct+=stPE.vecAct[i];
					}
					char buf[128];
					snprintf(buf,sizeof(buf),"*** Event Trigger:%s value = %s  = Policy:%s -> %s ***",err.ErrorName.c_str(),
						err.ErrorVal.c_str(),stPE.strFault.c_str(),strAct.c_str());
					mapAction[LOGCmd].push_back(std::string(buf));

					//��ȡ����
					vecPIEAct.push_back(stPE);
				}
			}
		}

		if(stPE.strType == "asis")
		{
			if(stPE.strFault.find(err.ErrorVal) != std::string::npos)
			{
				std::string strAct;
				for(int i=0 ;i<stPE.vecAct.size();i++)
				{
					strAct+=stPE.vecAct[i];
				}
				char buf[128];
				snprintf(buf,sizeof(buf),"*** Event Trigger::%s value = %s  = Policy:%s->%s ***",err.ErrorName.c_str(),
					err.ErrorVal.c_str(),stPE.strFault.c_str(),strAct.c_str());
				mapAction[LOGCmd].push_back(std::string(buf));

				//��ȡ����
				vecPIEAct.push_back(stPE);
			}
		}
	}

	if(vecPIEAct.size()>0)
	{
		std::sort(vecPIEAct.begin(),vecPIEAct.end());
		for(int i = 0;i < vecPIEAct.size() ;i++)
		{
			for(int j=0;j<vecPIEAct[i].vecAct.size();j++)
			{
				mapAction[POLICYCmd].push_back(vecPIEAct[i].vecAct[j]);
			}
		}
	}
	return true;
}

void CDispatch::AddConflictInfo(std::vector<ConflictInfo> &vecCI)
{
	m_vecConflict=vecCI;
}

