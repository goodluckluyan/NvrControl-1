#ifndef NVR_RET_H
#define NVR_RET_H

#define NVR_SUCCESS                                                                 0				////�ɹ�ֵ

///���ݿ�ģ��
#define DB_CONNECTED_ERROR															420500001				///	�������ݿ�ʧ��
#define DB_DISCONNECTED_ERROR														420500002				///	�Ͽ����ݿ�����ʧ��
#define DB_COMMAND_QUERY_ERROR														420500003				///	�������ݿ��ѯ����ʧ��

///nvr ģ��
#define URL_NVRLOGIN_ERROR															420200001				////��¼nvrʧ��
#define URL_NVRLOGOUT_ERROR															420200002				////�ǳ�nvrʧ��
#define URL_NVRIPCANNOTPING_ERROR													420200003				////nvrIP��ַping��ͨ


///ipc ģ��
#define URL_IPCCONTROL_SETIPCINFO_ERROR												420300001				////����ipc��Ϣʧ��
#define URL_IPCCONTROL_GETIPCINFO_ERROR												420300002				////��ȡipc��Ϣʧ��
#define URL_IPCCONTROL_GETIPCRESOLURATIO_ERROR										420300003				////��ȡipc��֧�ֵķֱ�����Ϣʧ��

///download ģ��
#define URL_DOWNLOADCONTROL_SETDOWNLOADSPEED_ERROR									420100001				////���ø�������ʧ��
#define URL_DOWNLOADCONTROL_DOWNLOADNUM_ERROR										420100002				////���ò������ص�����������ǰ�������ص���������18ʱ��ֹͣ�ô�����
#define URL_DOWNLOADCONTROL_DOWNLOADBYTIME_ERROR									420100003				////����sdk��ʱ�����صĺ���ʧ��
#define URL_DOWNLOADCONTROL_AVAILABLESPACETOOLITTLE_ERROR							420100004				////��ǰĿ¼ʣ��ռ��С
#define URL_DOWNLOADCONTROL_STARTTIME_ERROR											420100005				////��ʼ����ʱ�����ڵ�ǰʱ��
#define URL_DOWNLOADCONTROL_ENDTIME_ERROR											420100006				////��������ʱ�����ڵ�ǰʱ��	

///timecontrol ģ��
#define URL_TIMECONTROL_GETSYSTEMTIME_ERROR											420400001				////��ȡNVRϵͳʱ��ʧ��
#define URL_TIMECONTROL_SETSYSTEMTIME_ERROR											420400002				////����NVRϵͳʱ��ʧ��
#endif  