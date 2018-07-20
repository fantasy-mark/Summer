#ifndef _MAGETHERNETSDK_H_
#define _MAGETHERNETSDK_H_

#ifndef _TYPEDEF_H_

enum SDStorageFileType
{
	SDFileBMP=0,
	SDFileDDT=1,
	SDFileMGT=2,
	SDFileAVI=3,
	SDFileMGS=4,
};

typedef struct 
{
	DWORD dwFPAWidth;
	DWORD dwFPAHeight;
	DWORD dwBMPWidth;
	DWORD dwBMPHeight;
	DWORD dwColorBarWidth;
	DWORD dwColorBarHeight;
}OutputPara;

typedef struct 
{
	UINT intFPAWidth;//̽��������
	UINT intFPAHeight;

	enum Cam1stClass C1Type;//һ������
	enum Cam2ndClass C2Type;//��������

	char charName[CAMNAME_PROTOCOLLEN];
	char charType[TYPENAMELEN];

	UINT intMaxFPS;//���ͺŵ����֡��
	UINT intCurrentFPS;//��ǰʵ�����֡��

	UINT intVideoWidth;//(��HDMI, H.264��MPEG�����)������Ƶ����
	UINT intVideoHeight;
}struct_CamInfo;

#endif


//callback function for new frame arrival
typedef void (CALLBACK * MAG_FRAMECALLBACK)(UINT intChannelIndex, int intCameraTemperature, DWORD dwFFCCounterdown, DWORD dwCamState, DWORD dwStreamType, void * pUserData);

//callback function for serial receive
typedef void (CALLBACK * MAG_SERIALCALLBACK)(UINT intChannelIndex, void * pData, UINT intDataLength, void * pUserData);

//callback function for re-connect operation
typedef void (CALLBACK * MAG_RECONNECTCALLBACK)(UINT intChannelIndex, UINT intRecentHeartBeatTick, int intState, void * pUserData);

//callback function for ddt compress
typedef void (CALLBACK * MAG_DDTCOMPRESSCALLBACK)(UINT intChannelIndex, void * pData, UINT intDataLength, void * pUserData);

#ifdef MAG_EXPORTS
#define MAG_API extern __declspec(dllexport)
#else
#define MAG_API extern __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" 
{
#endif


//----------------------------------------------------
MAG_API BOOL WINAPI MAG_NewChannel(UINT intChannelIndex);

MAG_API void WINAPI MAG_DelChannel(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_IsChannelAvailable(UINT intChannelIndex);


//----------------------------------------------------
MAG_API BOOL WINAPI MAG_IsLanConnected();

MAG_API DWORD WINAPI MAG_GetLocalIp();

MAG_API void WINAPI MAG_SetFilter(UINT intFilter);

MAG_API void WINAPI MAG_EnableAutoReConnect(BOOL bEnable);

MAG_API BOOL WINAPI MAG_EnumCameras();

MAG_API DWORD WINAPI MAG_GetTerminalList(struct_TerminalList * pList, DWORD dwBufferSize);

//----------------------------------------------------
MAG_API BOOL WINAPI MAG_IsInitialized(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_Initialize(UINT intChannelIndex, HWND hWndMsg);

MAG_API void WINAPI MAG_Free(UINT intChannelIndex);

//----------------------------------------------------
MAG_API BOOL WINAPI MAG_IsUsingStaticIp();

MAG_API BOOL WINAPI MAG_IsDHCPServerRunning();

MAG_API BOOL WINAPI MAG_StartDHCPServer(HWND hWndMsg);

MAG_API void WINAPI MAG_StopDHCPServer();

//----------------------------------------------------

MAG_API BOOL WINAPI MAG_IsLinked(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_LinkCamera(UINT intChannelIndex, UINT intIP, UINT intTimeoutMS);
MAG_API BOOL WINAPI MAG_LinkCameraEx(UINT intChannelIndex, UINT IndexOrIP, USHORT shortPort, const char * charUser, const char * charPwd, UINT intTimeoutMS);

MAG_API void WINAPI MAG_DisLinkCamera(UINT intChannelIndex);

MAG_API UINT WINAPI MAG_GetRecentHeartBeat(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_SetReConnectCallBack(UINT intChannelIndex, MAG_RECONNECTCALLBACK pCallBack, void * pUserData);

MAG_API BOOL WINAPI MAG_ResetCamera(UINT intChannelIndex);

MAG_API void WINAPI MAG_GetCamInfo(UINT intChannelIndex, struct_CamInfo * pInfo, UINT intSize);

MAG_API BOOL WINAPI MAG_GetCameraTemperature(UINT intChannelIndex, int intT[4], UINT intTimeoutMS);

MAG_API BOOL WINAPI MAG_TriggerFFC(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_SetIoAlarmState(UINT intChannelIndex, BOOL bAlarm);

MAG_API BOOL WINAPI MAG_ReadCameraRegContent(UINT intChannelIndex, struct_CeRegContent * pContent, UINT intTimeoutMS, BOOL bReadDefaultValue);

MAG_API BOOL WINAPI MAG_SetCameraRegContent(UINT intChannelIndex, const struct_CeRegContent * pContent);

MAG_API BOOL WINAPI MAG_SetUserROIs(UINT intChannelIndex, const struct_UserROIs * pROI);

//----------------------------------------------------

MAG_API BOOL WINAPI MAG_IsProcessingImage(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_StartProcessImage(UINT intChannelIndex, const OutputPara * paraOut, MAG_FRAMECALLBACK funcFrame, DWORD dwStreamType, void * pUserData);

MAG_API BOOL WINAPI MAG_StartProcessPulseImage(UINT intChannelIndex, const OutputPara * paraOut, MAG_FRAMECALLBACK funcFrame, DWORD dwStreamType, void * pUserData);

MAG_API BOOL WINAPI MAG_TransferPulseImage(UINT intChannelIndex);

MAG_API void WINAPI MAG_StopProcessImage(UINT intChannelIndex);

MAG_API void WINAPI MAG_SetColorPalette(UINT intChannelIndex, enum ColorPalette ColorPaletteIndex);

MAG_API BOOL WINAPI MAG_SetFixedEnlargePara(UINT intChannelIndex, int intX1, int intX2, UCHAR byteY1, UCHAR byteY2);

MAG_API BOOL WINAPI MAG_SetSubsectionEnlargePara(UINT intChannelIndex, int intX1, int intX2, UCHAR byteY1, UCHAR byteY2);

MAG_API void WINAPI MAG_SetAutoEnlargePara(UINT intChannelIndex, DWORD dwAutoEnlargeRange, int intBrightOffset, int intContrastOffset);

MAG_API void WINAPI MAG_SetIsothermalPara(UINT intChannelIndex, int intLowerLimit, int intUpperLimit);

MAG_API void WINAPI MAG_GetFixPara(UINT intChannelIndex, struct_FixPara * pBuffer);

MAG_API float WINAPI MAG_SetFixPara(UINT intChannelIndex, const struct_FixPara * pBuffer, BOOL bEnableCameraCorrect);

MAG_API BOOL WINAPI MAG_GetOutputBMPdata(UINT intChannelIndex, UCHAR const **  pData, BITMAPINFO const ** pInfo);
MAG_API BOOL WINAPI MAG_GetOutputBMPdata_copy(UINT intChannelIndex, UCHAR * pBmp, UINT intBufferSize);

MAG_API BOOL WINAPI MAG_GetOutputColorBardata(UINT intChannelIndex, UCHAR const ** pData, BITMAPINFO const ** pInfo);
MAG_API BOOL WINAPI MAG_GetOutputColorBardata_copy(UINT intChannelIndex, UCHAR * pColorBar, UINT intBufferSize);

MAG_API BOOL WINAPI MAG_GetOutputVideoData(UINT intChannelIndex, UCHAR const **  pData, BITMAPINFO const ** pInfo);
MAG_API BOOL WINAPI MAG_GetOutputVideoData_copy(UINT intChannelIndex, UCHAR * pBmp, UINT intBufferSize);

MAG_API BOOL WINAPI MAG_GetTemperatureData(UINT intChannelIndex, int * pData, UINT intBufferSize, BOOL bEnableExtCorrect);

MAG_API BOOL WINAPI MAG_GetTemperatureData_Raw(UINT intChannelIndex, int * pData, UINT intBufferSize, BOOL bEnableExtCorrect);

MAG_API void WINAPI MAG_SetEXLevel(UINT intChannelIndex, enum EX ExLevel, int intCenterX, int intCenterY);

MAG_API enum EX WINAPI MAG_GetEXLevel(UINT intChannelIndex);

MAG_API void WINAPI MAG_SetDetailEnhancement(UINT intChannelIndex, int intDDE, BOOL bQuickDDE);

MAG_API int WINAPI MAG_FixTemperature(UINT intChannelIndex, int intT, float fEmissivity, DWORD dwPosX, DWORD dwPosY);

MAG_API const struct_State * WINAPI MAG_GetFrameStatisticalData(UINT intChannelIndex);

MAG_API int WINAPI MAG_GetTemperatureProbe(UINT intChannelIndex, DWORD dwPosX, DWORD dwPosY, UINT intSize);

MAG_API int WINAPI MAG_GetLineTemperatureInfo(UINT intChannelIndex, int * buffer, UINT intBufferSizeByte, int info[3], UINT x0, UINT y0, UINT x1, UINT y1);

MAG_API BOOL WINAPI MAG_GetRectTemperatureInfo(UINT intChannelIndex, UINT x0, UINT y0, UINT x1, UINT y1, int info[5]);

MAG_API BOOL WINAPI MAG_GetEllipseTemperatureInfo(UINT intChannelIndex,UINT x0, UINT y0, UINT x1, UINT y1, int info[5]);

MAG_API BOOL WINAPI MAG_GetRgnTemperatureInfo(UINT intChannelIndex, const UINT * Pos, UINT intPosNumber, int info[5]);

MAG_API BOOL WINAPI MAG_UseTemperatureMask(UINT intChannelIndex, BOOL bUse);

MAG_API BOOL WINAPI MAG_IsUsingTemperatureMask(UINT intChannelIndex);

MAG_API BOOL WINAPI MAG_SaveBMP(UINT intChannelIndex, DWORD dwIndex, const WCHAR * charFilename);

MAG_API BOOL WINAPI MAG_SaveMGT(UINT intChannelIndex, const WCHAR * charFilename);

MAG_API int WINAPI MAG_SaveDDT2Buffer(UINT intChannelIndex, void * pBuffer, UINT intBufferSize);

MAG_API BOOL WINAPI MAG_SaveDDT(UINT intChannelIndex, const WCHAR * charFilename);

MAG_API BOOL WINAPI MAG_LoadDDT(UINT intChannelIndex, OutputPara * paraOut, const WCHAR * charFilename, MAG_FRAMECALLBACK funcFrame, void * pUserData);

MAG_API BOOL WINAPI MAG_LoadBufferedDDT(UINT intChannelIndex, OutputPara * paraOut, const void * pBuffer, UINT intBufferSize, MAG_FRAMECALLBACK funcFrame, void * pUserData);

MAG_API int WINAPI MAG_CompressDDT(void * pDstBuffer, UINT intDstBufferSize, const void * pSrcBuffer, UINT intSrcBufferSize, UINT intQuality);

MAG_API int WINAPI MAG_DeCompressDDT(void * pDstBuffer, UINT intDstBufferSize, const void * pSrcBuffer, UINT intSrcBufferSize);

MAG_API BOOL WINAPI MAG_SetAsyncCompressCallBack(UINT intChannelIndex, MAG_DDTCOMPRESSCALLBACK pCallBack, int intQuality);

MAG_API BOOL WINAPI MAG_GrabAndAsyncCompressDDT(UINT intChannelIndex, void * pUserData);

MAG_API BOOL WINAPI MAG_SetPTZCmd(UINT intChannelIndex, enum PTZCmd cmd, DWORD dwPara);

MAG_API BOOL WINAPI MAG_QueryPTZState(UINT intChannelIndex, enum PTZQuery query, int * intValue, UINT intTimeoutMS);

MAG_API BOOL WINAPI MAG_SetSerialCmd(UINT intChannelIndex, const BYTE * buffer, UINT intBufferLen);

MAG_API BOOL WINAPI MAG_SetSerialCallBack(UINT intChannelIndex, MAG_SERIALCALLBACK pCallBack, void * pUserData);

MAG_API BOOL WINAPI MAG_SetVideoContrast(UINT intChannelIndex, int intContrastOffset);

MAG_API BOOL WINAPI MAG_SetVideoBrightness(UINT intChannelIndex, int intBrightnessOffset);

MAG_API BOOL WINAPI MAG_SDCardStorage(UINT intChannelIndex, enum SDStorageFileType filetype, UINT para);

MAG_API BOOL WINAPI MAG_LocalStorageAviStart(UINT intChannelIndex, const WCHAR * charFileName, UINT intSamplePeriod);

MAG_API void WINAPI MAG_LocalStorageAviStop(UINT intChannelIndex);

MAG_API void WINAPI MAG_LockFrame(UINT intChannelIndex);

MAG_API void WINAPI MAG_UnLockFrame(UINT intChannelIndex);

//Special version for custom: ----------------------------------------------------
MAG_API BOOL WINAPI MAG_CaptureVisibleJpeg(UINT intChannelIndex, const WCHAR * charFileName);
MAG_API BOOL WINAPI MAG_GetCurrentOffset(UINT intChannelIndex, const WCHAR * charReferenceDDT, int * pOffsetX, int * pOffsetY);

//Legacy, to be removed: ------------------------------------------------------
MAG_API BOOL WINAPI MAG_ExecAutoFocus(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_MoveLens(UINT intChannelIndex, BOOL bDirectionFar, UINT intMs);
MAG_API BOOL WINAPI MAG_StopLensMotor(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageMGT(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageBMP(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageMGSStart(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageMGSStop(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageAviStart(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_SDStorageAviStop(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_GetMulticastState(UINT intTargetIp, UINT * intMulticastIp, UINT * intMulticastPort, UINT intTimeoutMS);
MAG_API BOOL WINAPI MAG_IsListening(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_ListenTo(UINT intChannelIndex, UINT intTargetIp);
MAG_API void WINAPI MAG_StopListen(UINT intChannelIndex);
MAG_API const UCHAR * WINAPI MAG_GetOutputVideoYV12(UINT intChannelIndex);
MAG_API BOOL WINAPI MAG_StartDecodeVideo(UINT intChannelIndex, const OutputPara * paraOut, MAG_FRAMECALLBACK funcFrame, void * pUserData);
MAG_API UINT WINAPI MAG_DecodeVideoFrame(UINT intChannelIndex, const char * pBuffer, UINT intBufferLen);
MAG_API UINT WINAPI MAG_GetVideoPPS(UINT intChannelIndex, char * pBuffer, UINT intBufferLen);
MAG_API UINT WINAPI MAG_GetVideoSPS(UINT intChannelIndex, char * pBuffer, UINT intBufferLen);
MAG_API UINT WINAPI MAG_GetVideoVCL(UINT intChannelIndex, char * pBuffer, UINT intBufferLen);

#ifdef __cplusplus
}
#endif

#endif