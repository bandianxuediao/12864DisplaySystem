//=================================================================================
// 版权: bangiunion
//---------------------------------------------------------------------------------
// 作者：     时间：      版本：V0.0.4
//---------------------------------------------------------------------------------
// 修改历史：
// 修改人：  		 修改时间：			 修改内容：	     检查人：
//---------------------------------------------------------------------------------
// 文件：display.h
//---------------------------------------------------------------------------------
// 介绍：显示界面的显示和处理相关定义
//---------------------------------------------------------------------------------
// 备注：	
//=================================================================================
#ifndef __DISPLAY_H__
	#define __DISPLAY_H__ 
	
	#define  TITLE_X 60			 //标题栏起始坐标-原来是80
	#define  TITLE_Y 5
	#define  RIGHT_BUTTON_X  (OLED_X_MAX-24)//右功能键起始横坐标
	#define  LEFT_BUTTON_X   0	//左功能键起始横坐标
	#define  BUTTON_Y        52		//功能键纵坐标
    #define  IBUS_STATUS_X0 25 
	#define  IBUS_STATUS_X1 101
	#define  IBUS_STATUS_Y0 51
	#define  IBUS_STATUS_Y1 63
	//12864可以显示12X12大小的汉字五行
	#define  LINE0		     0	      //12864第一行的起始位置
	#define  LINE1           13       //12864第二行的起始位置 
	#define  LINE2		     	 26       //12864第三行的起始位置
	#define  LINE3           39		  //12864第四行的起始位置
	#define  LINE4           52		  //12864第五行的起始位置
	#define  LINE_MARGIN     1        //行与行之间间隔的像素数
	#define  TOP_RIGHT    	 98       //顶行右边
	#define  CWIDTH          12       //汉字宽度
	#define  LWIDTH			 			6		  //字母宽度
	#define  CHEIGHT         12       //字符高度

	//内容显示区域的矩形框,可用于显示1行或者2行内容
    #define  CONTENT_RECT_X0 8
    #define  CONTENT_RECT_Y0 (LINE1+1)
    #define  CONTENT_RECT_X1 120
    #define  CONTENT_RECT_Y1 (LINE4-4)

	//内容显示区域的矩形框,可用于显示3行内容
    #define  CONTENT_RECT3_X0 8
    #define  CONTENT_RECT3_Y0 (LINE0+1)
    #define  CONTENT_RECT3_X1 120
    #define  CONTENT_RECT3_Y1 (LINE4-4)

	#define  ALIGN_LEFT      0        //水平对齐方式，居左
	#define  ALIGN_CENTER    1        //水平对齐方式，居中
	#define  ALIGN_RIGHT     2        //水平对齐方式，居右
	
	#define  CURSOR_WIDTH  0	   //用于显示光标
	#define  CURSOR_HEIGHT 9
	
	#define  CURSOR_START_X  84
	#define  CURSOR_START_Y	 48

	#define STEP_SX  21	  //进度度起始坐标
    #define STEP_SY  LINE2

	//使用宏计算全屏水平居中对齐显示横坐标,汉字作为2个字符
    #define  FSCREEN_HC(charnum)    ((128-charnum*LWIDTH)/2)  
	
	//使用宏计算矩形框内水平对齐显示横坐标，汉字作为2个字符
	//单行
	#define  RECT_SL_X(charnum)				    (CONTENT_RECT_X0+(CONTENT_RECT_X1-CONTENT_RECT_X0-(charnum)*LWIDTH)/2 +1)	   //为了便于理解,没有优化，编译器会自动优化
	#define  RECT_SL_Y      				    (CONTENT_RECT_Y0+(CONTENT_RECT_Y1-CONTENT_RECT_Y0-CHEIGHT)/2 +1)

	//两行
	#define  RECT_DL0_X(charnum)				(CONTENT_RECT_X0+(CONTENT_RECT_X1-CONTENT_RECT_X0-(charnum)*LWIDTH)/2 +1)
	//#define  RECT_DL0_Y         				(CONTENT_RECT_Y0+(CONTENT_RECT_Y1-CONTENT_RECT_Y0-CHEIGHT*2)/2 +1)
    #define  RECT_DL0_Y         				(CONTENT_RECT_Y0+(CONTENT_RECT_Y1-CONTENT_RECT_Y0-CHEIGHT*2)/2)
	#define  RECT_DL1_X(charnum)				(CONTENT_RECT_X0+(CONTENT_RECT_X1-CONTENT_RECT_X0-(charnum)*LWIDTH)/2 +1)
	//#define  RECT_DL1_Y         				(CONTENT_RECT_Y0+(CONTENT_RECT_Y1-CONTENT_RECT_Y0-CHEIGHT*2)/2+CHEIGHT +1)
    #define  RECT_DL1_Y         				(CONTENT_RECT_Y0+(CONTENT_RECT_Y1-CONTENT_RECT_Y0-CHEIGHT*2)/2+CHEIGHT + 2)

	//三行
	#define  RECT_TL0_X(charnum)				(CONTENT_RECT3_X0+(CONTENT_RECT3_X1-CONTENT_RECT3_X0-(charnum)*LWIDTH)/2)
	#define  RECT_TL0_Y         				(CONTENT_RECT3_Y0+(CONTENT_RECT3_Y1-CONTENT_RECT3_Y0-CHEIGHT*3)/2 +1)
	#define  RECT_TL1_X(charnum)				(CONTENT_RECT3_X0+(CONTENT_RECT3_X1-CONTENT_RECT3_X0-(charnum)*LWIDTH)/2 +1)
	#define  RECT_TL1_Y         				(CONTENT_RECT3_Y0+(CONTENT_RECT3_Y1-CONTENT_RECT3_Y0-CHEIGHT*3)/2+CHEIGHT +1)
	#define  RECT_TL2_X(charnum)				(CONTENT_RECT3_X0+(CONTENT_RECT3_X1-CONTENT_RECT3_X0-(charnum)*LWIDTH)/2 +1)
	#define  RECT_TL2_Y         				(CONTENT_RECT3_Y0+(CONTENT_RECT3_Y1-CONTENT_RECT3_Y0-CHEIGHT*3)/2+2*CHEIGHT +1)

	//光标坐标结构体
	typedef struct 
	{
	    unsigned char x, y;
	} CURSOR;
	
	#define INPUT_DATA_MAX_LENGTH  20
	typedef struct
	{
		u8 area_num;              //要输入的区域数
		u8 area_current;          //当前正在输入的区域
		CURSOR area_pos[4];       //每个区域对应的起始坐标
		u8 length[4];             //每个区域最大可输入长度
		u8 buffer[4][INPUT_DATA_MAX_LENGTH+1];   //每个区域的输入数据
	}INPUT_DATA;
	extern INPUT_DATA input_data;
	
	
	extern void AddCurrentInputData(u8* inputdata, u8 length, u8 show_type);
	extern ErrorStatus DeleteCurrentInputData(u8 length, u8 show_type);
	extern void InputAreaNext(void);
	extern void InputAreaBack(void);
	extern u8 GetCurrentInputAreaNum(void);
	extern u32 GetCurrentInputAreaValue(void);
	extern u8* GetCurrentInputAreaString(void);
	
	extern void RestoreBackupDisplay(void);
	extern void BackupCurrentDisplay(void);
	
	extern void ShowCursor(unsigned char flash);
	extern void FlashCursor(void);
	
	extern void ShowMainMenu(void);
	extern void ShowDeviceType(unsigned char type);
	extern u8 ShowProgress(u8 label,u8 state);
	extern void ShowRegState(u8 state);
	extern void ShowRegHoleSelect(u16 index);
	extern void ShowBusVoltage(void);
	extern void ShowBusHighCurrent(void);
	extern void ShowBusLowCurrent(void);
	extern void ShowBatteryVoltage(void);
	extern void ShowBusError(u8 state);
	extern void ShowRegFinish(void);
	extern void ShowSystemStateInfo(u8 state);
	extern void ShowNetTestState(u8 state);
	extern void ShowTestDetErrorList(u8 up_down, u8 disdel);
	extern void ShowDeleteDetInfoState(u8 state);
	extern void ShowDelayModifyState(u8 state);
	extern void ShowNetListInfo(u16 hole_index, u8 type);
	extern void ShowSystemSetFunState(u8 state);
	extern void ShowDeviceNameModify(u8 name);
	extern void ShowSelectDetTypeMakeSure (u8 n_num);
	extern void ShowDkeyTransState(u8 state);
	extern void ShowDeviceCurrentDetState(void);
	extern void ShowBlastProcessState(u8 state);
	extern void ShowConnectState(u8 state);
	extern void ShowDetRegTypeError(unsigned char type);
	extern void ShowBattaryLow(u16 voltage);
	extern void ShowDeviceNetConnectCommState(u8 name, u8 retry);
	extern void ShowDeviceNetConnectlist(u8 up_down);
	extern void ShowBlastUserPassWordValidate(u8 reply);
	extern void ShowHVButtonState(u8 type);
	extern void ShowBlasteFireNumber(u8 number);
	extern void DebugTest(u8 state);

	extern void ShowGPSOrGPRSState(u8 state);
	extern void ShowGPRSInitResult(u8 state, u8 result1, u8 result2);
	extern void ShowSystemInfoSafeMonitorFun(u8 num);
	extern ErrorStatus ShowBlasterFireRecorder(u16 index);
	extern void ShowBlasteValidityErrorInfo(u8 errnum);
	extern void Dis_menu(void);
	
#endif
