#ifndef _TYPEDEF_H_
#define  _TYPEDEF_H_

#ifndef u_long
typedef unsigned long u_long;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef LONG
typedef long LONG;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#define STREAM_TEMPERATURE	(2)
#define STREAM_VIDEO		(4)
#define STREAM_HYBRID		(STREAM_TEMPERATURE | STREAM_VIDEO)

enum enumInputIo {
	IoFFC=0,
	IoCaptureMGT,
	IoCaptureBMP,
	IoCustom,
};

enum enumAnalogPlot {
	AnalogPlotNone=0,
	AnalogPlotCenterCross,
	AnalogPlotMaxTemperature,
	AnalogPlotROI,
};

enum TV_STANDARD {
    NTSC_M = 0,
    NTSC_J,
    PAL_BDGHI,
    PAL_M,
    PAL_NC
};

enum EX {
	E1X = 0,
	E2X,
	E4X,
};

enum ColorPalette {
	Gray0to255=0,//白热
	Gray255to0=1,//黑热
	IronBow=2,//铁红
	RainBow=3,//彩虹
	GlowBow=4,//琥珀
	Autumn=5,//金秋
	Winter=6,//寒冬
	HotMetal=7,//热金属
	Jet=8,//喷射
	RedSaturation=9,//红饱和
	HighContrast=10,//高对比度
};

enum PTZProtocol {
	ProtocolPelcoD = 0,//Pelco_D协议
	ProtocolPelcoP,//Pelco_P协议
};

enum PTZCmd {
	PTZStop = 0,//方位或预置位停止 参数为0
	PTZRight = 1,//方位 参数为运动速度0~63
	PTZLeft,
	PTZUp,
	PTZDown,
	PTZUpRight = 5,
	PTZUpLeft,
	PTZDownRight,
	PTZDownLeft,
	PTZSetPreset = 9,//预置位 参数为预置位编号0~255
	PTZCallPreset,
	PTZClearPreset,
	PTZSetAuxiliary = 12,//辅助开关 参数为辅助开关编号0~255
	PTZClearAuxiliary,
	PTZZoomStop = 14,//镜头停止变倍 参数为0
	PTZZoomIn,//镜头放大 参数为马达运动时间
	PTZZoomOut,//镜头缩小
	PTZFocusStop = 17,//镜头停止调焦 参数为0
	PTZFocusAuto,//镜头自动对焦 参数为0
	PTZFocusFar,//镜头看远 参数为马达运动时间
	PTZFocusNear,//镜头看近
	PTZFocusGoto,//镜头调焦绝对位置 参数为调焦位置
};

enum PTZQuery {
	PTZQueryPan = 0,//查询云台上下角度
	PTZQueryTilt,//查询云台左右角度
	PTZQueryZoomPosition,//查询zoom绝对位置
	PTZQueryZoomState,//查询是否正在zoom
	PTZQueryFocusPosition,//查询focus绝对位置
	PTZQueryFocusState,//查询是否正在执行自动对焦
};

enum SDStorageFileType {
	SDFileBMP=0,
	SDFileDDT=1,
	SDFileMGT=2,
	SDFileAVI=3,
	SDFileMGS=4,
};

typedef struct {
	char	charTerminalName[32];//热像仪名称, 可包含中文字符
	UINT	intVersion;//协议版本号
	u_long	intTerminalIp;//相机ip, 高字节表示ip的最后一节
	u_long	intControllerIp;//控制端ip, 高字节表示ip的最后一节
	BYTE	charCameraMAC[6];//相机MAC地址
	char	Pad[2];
	UINT	intRefreshCounterDown;//从5开始递减，每次未响应enum指令减1，减为0时将从本list删除
	BOOL	bMulticasting;//是否正在组播图像
} struct_TerminalList;


#define CAMNAME_PROTOCOLLEN	(32)
#define TYPENAMELEN		(8)

typedef struct {
	DWORD pad0;
	
	char charName[CAMNAME_PROTOCOLLEN];//热像仪名称

	BOOL bUseStaticIp;//是否使用静态ip
	DWORD dwStaticIp;//静态ip(如使用)，以太网顺序
	DWORD dwStaticNetMask; //静态ip的子网掩码(如使用)，以太网顺序

	BOOL bMulticastImg;//图像数据是否组播
	DWORD dwMulticastIp;//图像组播目的ip(如使用)，以太网顺序

	DWORD pad1[16];

	DWORD dwSN;//序列号, read only

	DWORD pad2[2];

	DWORD dwStaticGateWay;//静态网关

	DWORD pad3[8];

	UINT intCurrentLensIndex;//当前镜头编号, 0~2
	
	DWORD pad4[2];

	UINT intFFCFrameTrigger;//FFC帧数触发器，单位帧
	UINT intFFCTemperatureTrigger;//FFC温度触发器，单位mC

	DWORD pad5;

	UINT intAccResponse;//多帧累加，单位帧, 1,2,4,8,16

	DWORD pad6[3];

	UINT intInputIoFunction;//输入Io功能, 0-FFC, 1-MGT, 2-BMP,3-自定义

	//模拟视频控制，不影响PC机上看到的效果
	ColorPalette intPaletteIndex;//调色板
	BOOL bColorBar;//是否叠加颜色条
	BOOL bSubSectionEnlarge;//是否使用人工分段灰度拉伸
	int intEnlargeX1;//人工拉伸节点X1, 单位mC
	int intEnlargeX2;//人工拉伸节点X2, 单位mC
	UINT byteEnlargeY1;//人工拉伸节点Y1, 单位为灰度，范围0~254
	UINT byteEnlargeY2;	//人工拉伸节点Y2, 单位为灰度，范围1~255
	UINT intAutoEnlargeRange;//自动拉伸最小温度范围，单位C，人工分段灰度拉伸比自动拉伸优先，必须大于2，建议不小于5
	
	//IO
	UINT intAnalogPlot;//模拟视频标记, 0-无,1-中心,2-高温,3-测温对象
	int	intAlarmTemp;//IO报警温度

	//TV standard
	TV_STANDARD intTVStandard;//模拟视频制式

	BOOL bCheckHeartBeat;//是否启用心跳检测
	BOOL bAlwaysAnalogOutput;//是否始终输出模拟视频，若否，则以太网传输图像时模拟视频关闭，以降低功耗
	
	DWORD pad8[3];

	EX EXLevel;//模拟视频电子倍焦

	DWORD dwPartnerVisibleIp;//DTXX搭档的可见光相机IP，MAGXX未定义

	UINT intDDE;//模拟视频DDE强度设置, 0~32

	DWORD pad9;

	DWORD dwSerialBaudRate;//串口波特率
	DWORD dwSerialFeature;//串口参数，格式为 (使能<<24) | (数据位<<16) | (停止位<<8) | 校验位。其中停止位 0-1, 1-1.5, 2-2，校验位0-None, 1-Odd, 2-Even, 3-Mark, 4-Space
} struct_CeRegContent;


typedef struct {
	float fDistance;//目标距离, 单位m, 范围(2*fFocalLength, ∞)
	float fEmissivity;//全局缺省发射率，范围(0,1]
	float fTemp;//气温, 单位C
	float fRH;//相对湿度，范围(0~1)
	float fVisDistance;//能见度, 单位km, 范围(0, ∞)
	float fRain;//降雨强度, 单位mm/h, 范围(0, ∞)
	float fSnow;//降雪强度, 单位mm/h, 范围(0, ∞)
	float fExtrapara1;//修正参数1 镜头相机相关
	float fExtrapara2;//修正参数2 镜头相机相关
	float fTaoAtm;//大气透过率, 只读
	float fTaoFilter;//(热像仪外部的)窗口/滤光片透过率
} struct_FixPara;

typedef struct {
	DWORD pad0[6];
	int intMaxTemperature;//全像面最高温度, 单位mC, 未经修正
	int intMinTemperature;//全像面最低温度, 单位mC, 未经修正
	int intAveTemperature;//全像面平均温度, 单位mC, 未经修正
	int intSTDTemperature;//全像面温度标准差, 单位mC，该数值对部分型号的高温热像仪不适用
	UINT intPosMax;//最高温度出现位置, y=int(intPosMax/FPAWIDTH), x=intPosMax-FPAWIDTH*y, 原点在图像左下角
	UINT intPosMin;//最低温度出现位置, y=int(intPosMin/FPAWIDTH), x=intPosMin-FPAWIDTH*y, 原点在图像左下角
	DWORD pad1[3];
	UINT intAveNETDt;//全像面温度时域标准差, 单位mC, 包含景物温度变化和相机噪声两个部分，该数值对部分型号的高温热像仪不适用
	DWORD pad2[5];
	UINT intHistTemperature[256];//红外直方图
} struct_State;

typedef struct {
	DWORD dwFPAWidth;//FPA宽度，必须为FPAWIDTH
	DWORD dwFPAHeight;//FPA高度，必须为FPAHEIGHT
	DWORD dwBMPWidth;//输出图像宽度, 必须为4的整数倍, 且不小于FPA的宽度(FPAWIDTH)
	DWORD dwBMPHeight;//输出图像高度, 不小于FPA的高度(FPAHEIGHT)
	DWORD dwColorBarWidth;//颜色条图像宽度, 必须为4的整数倍, 最小为4
	DWORD dwColorBarHeight;//颜色条图像高度, 最小为1
} OutputPara;

typedef struct {
	UINT intFPAWidth;//探测器像素
	UINT intFPAHeight;

	DWORD pad[2];

	char charName[CAMNAME_PROTOCOLLEN];
	char charType[TYPENAMELEN];

	UINT intMaxFPS;//本型号的最高帧率
	UINT intCurrentFPS;//当前实际输出帧率

	UINT intVideoWidth;//(以HDMI, H.264或MPEG输出的)数字视频像素
	UINT intVideoHeight;
}struct_CamInfo;

typedef struct {	//成像型热像仪结构体
	UINT intSize;//本结构体尺寸

	char charName[CAMNAME_PROTOCOLLEN];//热像仪名称
	
	DWORD dwReserved[12];

	DWORD dwSN;//序列号 ReadOnly

	DWORD dwReserved2[3];

	BOOL bUseStaticIp;//是否使用静态ip
	DWORD dwStaticIp;//静态网关
	DWORD dwStaticNetMask;//静态子网掩码
	DWORD dwStaticGateWay;//静态网关
	DWORD dwStaticDNS;//指定DNS

	DWORD dwReserved3[4];

	BOOL bCheckHeartBeat;//是否启用心跳检测

	DWORD dwPartnerVisibleIp;//搭档的可见光相机IP

	DWORD dwReserved4[5];

	//IO
	enum enumInputIo InputIoFunction;//输入Io功能

	DWORD dwReserved5[7];

	DWORD dwReserved6[4];

	//Digital Output
	UINT intEncoder;//数字视频编码方式
	UINT intBitRateKBPS;//码率
	UINT intIFrame;//I帧间隔

	DWORD dwReserved7[5];

	//Analog Output	
	UINT intPaletteIndex;
	enum enumAnalogPlot AnalogPlot;//模拟视频标记 0-无, 1-中心十字叉线
	UINT intTVStandard;//模拟视频制式
	UINT intDetailRatio;//模拟输出DDE强度，0~3，0为关闭
	UINT intEX;//模拟视频电子倍焦，0-none, 1-2X, 2-4X
	BOOL bSeperateMode;//海天模式

	DWORD dwReserved8[2];

	BOOL bOSDTime;//是否在视频上叠加当前时间
	BOOL bOSDCamName;//是否在视频上叠加相机名称

	DWORD dwReserved9[7];

	//serial
	DWORD dwSerialBaudRate;//串口波特率
	DWORD dwSerialByteSize;//串口数据位
	DWORD dwSerialStopBits;//串口停止位
	DWORD dwSerilParity;//串口校验位

	DWORD dwReserved10[3];

	//ptz
	enum PTZProtocol protoPTZ;//云台协议
	UINT intPTZAddress;//云台地址

	DWORD dwReserved11[8];
} struct_CfgPara;

typedef struct {
	char charROIName[32];//ROI名称
	int x0;//以左下角为原点，x0必须小于x1
	int y0;//y0必须小于y1
	int x1;
	int y1;
	int intEmissivity;//发射率*100，例如90代表0.9
	int intAlarmTemp;//报警温度，单位mC，报警方式为IO输出和画面上数字闪烁
	DWORD dwDraw;//显示选项
	DWORD dwReserved[9];
} struct_RectROI;

#define MAX_RECT_ROI_NUM	(4)

typedef struct {
	UINT intValidRectROI;//有效rect ROI数量
	struct_RectROI ROI[MAX_RECT_ROI_NUM];
} struct_UserROIs;

#endif