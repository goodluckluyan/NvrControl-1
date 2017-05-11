
#include "../para/C_Para.h"
class IPMgr
{
public:
	static IPMgr* GetInstance()
	{
		if(m_ptrIpMgr==NULL)
		{
			m_ptrIpMgr = new IPMgr();
		}
		return m_ptrIpMgr;
	}

	bool AddIP(std::string strLocal,std::string strOther)
	{

		if(strLocal.empty()||strOther.empty())
		{
			return false;
		}

		int nRole = C_Para::GetInstance()->GetRole();
		if(nRole==1 || nRole ==2)
		{
			m_mapIPPool_MS.insert(std::pair<std::string,std::string>(strLocal,strOther));

		}
		else if(nRole==3 || nRole ==4)
		{
			m_mapIPPool_MS.insert(std::pair<std::string,std::string>(strOther,strLocal));
		}
		else
		{
			return false;
		}

		// �γɽ������
		//10.7.75.78->192.168.1.10
		//192.168.1.10->10.7.75.78
		//10.7.75.79->192.168.1.20
		//192.168.1.20->10.7.75.79
		std::vector<std::string> vecMaster;
		std::vector<std::string> vecSlave;

		std::map<std::string,std::string>::iterator it = m_mapIPPool_MS.begin();
		for(;it!=m_mapIPPool_MS.end();it++)
		{
			vecMaster.push_back(it->first);
			vecSlave.push_back(it->second);
		}

		int nLen = vecMaster.size();
		for(int i =0 ;i<nLen ;i++)
		{
			for(int j = 0 ;j<nLen;j++)
			{
				if(i==j)
				{
					continue;
				}
				m_mapIPPool.insert(std::pair<std::string,std::string>(vecMaster[i],vecMaster[j]));
			}
		}

		nLen = vecSlave.size();
		for(int i =0 ;i<nLen ;i++)
		{
			for(int j = 0 ;j<nLen;j++)
			{
				if(i==j)
				{
					continue;
				}
				m_mapIPPool.insert(std::pair<std::string,std::string>(vecSlave[i],vecSlave[j]));
			}
		}

		return true;


	}
	bool GetNextOtherIP(std::string& strIP)
	{
		m_csLock.EnterCS();
		std::map<std::string,std::string>  tmpmap = m_mapIPPool;
		m_csLock.LeaveCS();

		std::map<std::string,std::string>::iterator fit = tmpmap.find(strIP);
		if(fit!=tmpmap.end())
		{
			strIP=fit->second;
		}
		else
		{
			return false;
		}
	}

private:
	IPMgr()
	{};

	static IPMgr * m_ptrIpMgr;

	std::map<std::string,std::string>  m_mapIPPool_MS;// keyΪ��ip
	std::map<std::string,std::string>  m_mapIPPool;//�滻��ϵ
	C_CS m_csLock;

};


