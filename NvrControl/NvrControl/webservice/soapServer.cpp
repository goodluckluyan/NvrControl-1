/* soapServer.cpp
   Generated by gSOAP 2.8.18 from webservice.h

Copyright(C) 2000-2014, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapServer.cpp ver 2.8.18 2017-05-10 05:55:14 GMT")


extern "C" SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	unsigned int k = soap->max_keep_alive;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (soap->max_keep_alive > 0 && !--k)
			soap->keep_alive = 0;
#endif
		if (soap_begin_serve(soap))
		{	if (soap->error >= SOAP_STOP)
				continue;
			return soap->error;
		}
		if (soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
extern "C" SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:GetoneInfos"))
		return soap_serve_mons__GetoneInfos(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:SetDlSpeed"))
		return soap_serve_mons__SetDlSpeed(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:DownLoadByTime"))
		return soap_serve_mons__DownLoadByTime(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:SetIpcInfo"))
		return soap_serve_mons__SetIpcInfo(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:GetSystemTimeInfo"))
		return soap_serve_mons__GetSystemTimeInfo(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:SetSystemTime"))
		return soap_serve_mons__SetSystemTime(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:DInit"))
		return soap_serve_mons__DInit(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:DRelease"))
		return soap_serve_mons__DRelease(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:GetIpcResoluRatio"))
		return soap_serve_mons__GetIpcResoluRatio(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:GetIpcVedioQuality"))
		return soap_serve_mons__GetIpcVedioQuality(soap);
	if (!soap_match_tag(soap, soap->tag, "mons:SetIpcVedioQuality"))
		return soap_serve_mons__SetIpcVedioQuality(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__GetoneInfos(struct soap *soap)
{	struct mons__GetoneInfos soap_tmp_mons__GetoneInfos;
	struct mons__get1 ret;
	soap_default_mons__get1(soap, &ret);
	soap_default_mons__GetoneInfos(soap, &soap_tmp_mons__GetoneInfos);
	if (!soap_get_mons__GetoneInfos(soap, &soap_tmp_mons__GetoneInfos, "mons:GetoneInfos", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__GetoneInfos(soap, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__get1(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__get1(soap, &ret, "mons:get1", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__get1(soap, &ret, "mons:get1", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__SetDlSpeed(struct soap *soap)
{	struct mons__SetDlSpeed soap_tmp_mons__SetDlSpeed;
	struct mons__SetDLSpeedResponse ret;
	soap_default_mons__SetDLSpeedResponse(soap, &ret);
	soap_default_mons__SetDlSpeed(soap, &soap_tmp_mons__SetDlSpeed);
	if (!soap_get_mons__SetDlSpeed(soap, &soap_tmp_mons__SetDlSpeed, "mons:SetDlSpeed", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__SetDlSpeed(soap, soap_tmp_mons__SetDlSpeed.isOK, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__SetDLSpeedResponse(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__SetDLSpeedResponse(soap, &ret, "mons:SetDLSpeedResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__SetDLSpeedResponse(soap, &ret, "mons:SetDLSpeedResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__DownLoadByTime(struct soap *soap)
{	struct mons__DownLoadByTime soap_tmp_mons__DownLoadByTime;
	struct mons__DownloadTimeResponse ret;
	soap_default_mons__DownloadTimeResponse(soap, &ret);
	soap_default_mons__DownLoadByTime(soap, &soap_tmp_mons__DownLoadByTime);
	if (!soap_get_mons__DownLoadByTime(soap, &soap_tmp_mons__DownLoadByTime, "mons:DownLoadByTime", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__DownLoadByTime(soap, soap_tmp_mons__DownLoadByTime.id, soap_tmp_mons__DownLoadByTime.nAudiNU, soap_tmp_mons__DownLoadByTime.nDevicePo, soap_tmp_mons__DownLoadByTime.tmStart, soap_tmp_mons__DownLoadByTime.tmEnd, soap_tmp_mons__DownLoadByTime.storePath, soap_tmp_mons__DownLoadByTime.mp4name, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__DownloadTimeResponse(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__DownloadTimeResponse(soap, &ret, "mons:DownloadTimeResponse", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__DownloadTimeResponse(soap, &ret, "mons:DownloadTimeResponse", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__SetIpcInfo(struct soap *soap)
{	struct mons__SetIpcInfo soap_tmp_mons__SetIpcInfo;
	struct mons__SetIpcInfoRes ret;
	soap_default_mons__SetIpcInfoRes(soap, &ret);
	soap_default_mons__SetIpcInfo(soap, &soap_tmp_mons__SetIpcInfo);
	if (!soap_get_mons__SetIpcInfo(soap, &soap_tmp_mons__SetIpcInfo, "mons:SetIpcInfo", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__SetIpcInfo(soap, soap_tmp_mons__SetIpcInfo.HallNo, soap_tmp_mons__SetIpcInfo.ipcPo, soap_tmp_mons__SetIpcInfo.channelID, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__SetIpcInfoRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__SetIpcInfoRes(soap, &ret, "mons:SetIpcInfoRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__SetIpcInfoRes(soap, &ret, "mons:SetIpcInfoRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__GetSystemTimeInfo(struct soap *soap)
{	struct mons__GetSystemTimeInfo soap_tmp_mons__GetSystemTimeInfo;
	struct mons__GetSystemTimeRes ret;
	soap_default_mons__GetSystemTimeRes(soap, &ret);
	soap_default_mons__GetSystemTimeInfo(soap, &soap_tmp_mons__GetSystemTimeInfo);
	if (!soap_get_mons__GetSystemTimeInfo(soap, &soap_tmp_mons__GetSystemTimeInfo, "mons:GetSystemTimeInfo", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__GetSystemTimeInfo(soap, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__GetSystemTimeRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__GetSystemTimeRes(soap, &ret, "mons:GetSystemTimeRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__GetSystemTimeRes(soap, &ret, "mons:GetSystemTimeRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__SetSystemTime(struct soap *soap)
{	struct mons__SetSystemTime soap_tmp_mons__SetSystemTime;
	struct mons__SetSystemInfoRes ret;
	soap_default_mons__SetSystemInfoRes(soap, &ret);
	soap_default_mons__SetSystemTime(soap, &soap_tmp_mons__SetSystemTime);
	if (!soap_get_mons__SetSystemTime(soap, &soap_tmp_mons__SetSystemTime, "mons:SetSystemTime", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__SetSystemTime(soap, soap_tmp_mons__SetSystemTime.timeInfos, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__SetSystemInfoRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__SetSystemInfoRes(soap, &ret, "mons:SetSystemInfoRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__SetSystemInfoRes(soap, &ret, "mons:SetSystemInfoRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__DInit(struct soap *soap)
{	struct mons__DInit soap_tmp_mons__DInit;
	struct mons__InitRes ret;
	soap_default_mons__InitRes(soap, &ret);
	soap_default_mons__DInit(soap, &soap_tmp_mons__DInit);
	if (!soap_get_mons__DInit(soap, &soap_tmp_mons__DInit, "mons:DInit", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__DInit(soap, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__InitRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__InitRes(soap, &ret, "mons:InitRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__InitRes(soap, &ret, "mons:InitRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__DRelease(struct soap *soap)
{	struct mons__DRelease soap_tmp_mons__DRelease;
	struct mons__ReleaseRes ret;
	soap_default_mons__ReleaseRes(soap, &ret);
	soap_default_mons__DRelease(soap, &soap_tmp_mons__DRelease);
	if (!soap_get_mons__DRelease(soap, &soap_tmp_mons__DRelease, "mons:DRelease", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__DRelease(soap, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__ReleaseRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__ReleaseRes(soap, &ret, "mons:ReleaseRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__ReleaseRes(soap, &ret, "mons:ReleaseRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__GetIpcResoluRatio(struct soap *soap)
{	struct mons__GetIpcResoluRatio soap_tmp_mons__GetIpcResoluRatio;
	struct mons__GetIpcResoluRatioInfo ret;
	soap_default_mons__GetIpcResoluRatioInfo(soap, &ret);
	soap_default_mons__GetIpcResoluRatio(soap, &soap_tmp_mons__GetIpcResoluRatio);
	if (!soap_get_mons__GetIpcResoluRatio(soap, &soap_tmp_mons__GetIpcResoluRatio, "mons:GetIpcResoluRatio", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__GetIpcResoluRatio(soap, soap_tmp_mons__GetIpcResoluRatio.nAudiNU, soap_tmp_mons__GetIpcResoluRatio.nDevicePo, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__GetIpcResoluRatioInfo(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__GetIpcResoluRatioInfo(soap, &ret, "mons:GetIpcResoluRatioInfo", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__GetIpcResoluRatioInfo(soap, &ret, "mons:GetIpcResoluRatioInfo", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__GetIpcVedioQuality(struct soap *soap)
{	struct mons__GetIpcVedioQuality soap_tmp_mons__GetIpcVedioQuality;
	struct mons__GetIpcVedioQualityInfoRes ret;
	soap_default_mons__GetIpcVedioQualityInfoRes(soap, &ret);
	soap_default_mons__GetIpcVedioQuality(soap, &soap_tmp_mons__GetIpcVedioQuality);
	if (!soap_get_mons__GetIpcVedioQuality(soap, &soap_tmp_mons__GetIpcVedioQuality, "mons:GetIpcVedioQuality", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__GetIpcVedioQuality(soap, soap_tmp_mons__GetIpcVedioQuality.nAudiNU, soap_tmp_mons__GetIpcVedioQuality.nDevicePo, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__GetIpcVedioQualityInfoRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__GetIpcVedioQualityInfoRes(soap, &ret, "mons:GetIpcVedioQualityInfoRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__GetIpcVedioQualityInfoRes(soap, &ret, "mons:GetIpcVedioQualityInfoRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_mons__SetIpcVedioQuality(struct soap *soap)
{	struct mons__SetIpcVedioQuality soap_tmp_mons__SetIpcVedioQuality;
	struct mons__SetIpcVedioQualityInfoRes ret;
	soap_default_mons__SetIpcVedioQualityInfoRes(soap, &ret);
	soap_default_mons__SetIpcVedioQuality(soap, &soap_tmp_mons__SetIpcVedioQuality);
	if (!soap_get_mons__SetIpcVedioQuality(soap, &soap_tmp_mons__SetIpcVedioQuality, "mons:SetIpcVedioQuality", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = mons__SetIpcVedioQuality(soap, soap_tmp_mons__SetIpcVedioQuality.nAudiNU, soap_tmp_mons__SetIpcVedioQuality.nDevicePo, soap_tmp_mons__SetIpcVedioQuality.vedioInfo, ret);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL;
	soap_serializeheader(soap);
	soap_serialize_mons__SetIpcVedioQualityInfoRes(soap, &ret);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mons__SetIpcVedioQualityInfoRes(soap, &ret, "mons:SetIpcVedioQualityInfoRes", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mons__SetIpcVedioQualityInfoRes(soap, &ret, "mons:SetIpcVedioQualityInfoRes", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapServer.cpp */
