#ifndef TIMECONTROL_H
#define TIMECONTROL_H
#include <iostream>
#include "../so/dhnetsdk.h"
#include "../Nvr/Nvr_Control.h"
#include "../Nvr_Types.h"
#include "../Nvr_Ret.h"
using namespace std;


typedef struct timeStruct
{
	unsigned int unYear;
	unsigned int unMonth;
	unsigned int unDay;
	unsigned int unHour;
	unsigned int unMinute;
	unsigned int unSecond;
}timeStruct;
class timeControl
{
public:
	timeControl();
	~timeControl();
	/*******************************************************\
	�������ܣ���ѯ�豸��ǰʱ��(�ú�����������ϵͳʱ��ǰ��ȡϵͳʱ��)
	����1����ѯ���ĵ�ǰʱ��
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************/
	 int GetSystemTime(timeStruct & timeInfo);

	 /******************************************************\
	 �������ܣ������豸ʱ��
	 ����1��ʱ����Ϣ
	 ����ֵ�ɹ�����0,ʧ�ܷ�������ֵ
	 \******************************************************/
	 int SetSystemTime(timeStruct  timeInfo);

	 /******************************************************\
	 �������ܣ���ʼ������
	 \******************************************************/
	 int DM_Init();
	 /******************************************************\
	 �������ܣ��ͷŲ���
	 \******************************************************/
	 int DM_Release();

	/*******************************************************\
	�������ܣ�����NTP
	����1��ʹ�ܿ���
	����2��IP��ַ��������	
	����3���˿ں�
	����4����������
	����ֵ���ɹ�����0��ʧ�ܷ�������ֵ
	\*******************************************************/
	int SetNvtServer(bool isOK,string ip,int port,int updatePeriod);
public:
	int GetTimeControlErrorString(int errorcode,REPORT_STATUS *preportstatus);
protected:
private:
	NvrControl m_nvrcontrol;

};


#endif