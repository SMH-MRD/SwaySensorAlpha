#pragma once

#include "resource.h"

using namespace std;

//============================================================================
// define定義
//----------------------------------------------------------------------------
// タスク条件
#define MAX_APP_TASK                    8                       // タスクスレッド最大数
#define TASK_NUM                        4                       // このアプリケーションのタスク数
#define TARGET_RESOLUTION               1                       // マルチメディアタイマーイベント分解能
#define SYSTEM_TICK_MSEC                25                      // メインスレッド周期
#define INITIAL_TASK_STACK_SIZE         16384                   // 各スレッド用スタックサイズ
#define ID_TIMER_EVENT                  0                       // タイマー用イベント配列の位置

//----------------------------------------------------------------------------
// ID定義 Mainスレッド用　WM_USER + 1000 + α
#define ID_STATUS                       (WM_USER + 1001)
#define ID_STATIC_MAIN                  (WM_USER + 1002)
#define ID_TASK_SET_TAB                 (WM_USER + 1003)

//----------------------------------------------------------------------------
// .ini 関連設定
#define FOLDER_OF_INIFILE               L"Data\\"               // iniファイルフォルダ
#define EXT_OF_INIFILE                  L"ini"                  // iniファイル拡張子
#define PATH_OF_INIFILE                 pszInifile              // iniファイルパス

#define INI_SCTION_COMMON               L"COMMON"               // 
#define INI_KEY_TIMER_PERIOD_SYSTEM     L"TimerPeriodSystem"    // 

#define INI_SCTION_OBJECT               L"OBJECT"               // 
#define INI_KEY_ENVIRONMENT             L"Environment"          // 
#define INI_KEY_POLICY                  L"Policy"               // 
#define INI_KEY_AGENT                   L"Agent"                // 
#define INI_KEY_SCADA                   L"Scada"                // 
#define INI_KEY_HELPER                  L"Helper"               // 

#define INI_SCTION_MOUNTING             L"MOUNTING"             // 
#define INI_KEY_MOUNT_OFFSET_LX0        L"MountOffsetLX0"       // 
#define INI_KEY_MOUNT_OFFSET_LY0        L"MountOffsetLY0"       // 
#define INI_KEY_MOUNT_OFFSET_L0         L"MountOffsetL0"        // 
#define INI_KEY_MOUNT_OFFSET_LC         L"MountOffsetLC"        // 
#define INI_KEY_MOUNT_OFFSET_A0         L"MountOffsetA0"        // 
#define INI_KEY_MOUNT_OFFSET_AC         L"MountOffsetAC"        // 

#define INI_SCTION_CAMERA               L"CAMERA"               // 
#define INI_KEY_CAM_FRAME_SIZE          L"CamFrameSize"         // 
#define INI_KEY_CAM_VIEW_ANGLE          L"CamViewAngle"         // 
#define INI_KEY_CAM_FRAME_RATE          L"CamFrameRate"         // 
#define INI_KEY_CAM_EXPOSURE_TIME       L"CamExposureTime"      // 
#define INI_KEY_CAM_EXPOSURE_AUTO       L"CamExposureAuto"      // 

#define INI_SCTION_PROCESS              L"PROCESS"              // 
#define INI_KEY_IMG_ROI                 L"ImgROI"               // 
#define INI_KEY_MASK_IMG                L"MaskImg"              // 
#define INI_KEY_MASK_IMG1_LOW           L"MaskImg1Low"          // 
#define INI_KEY_MASK_IMG1_UPP           L"MaskImg1Upp"          // 
#define INI_KEY_MASK_IMG2_LOW           L"MaskImg2Low"          // 
#define INI_KEY_MASK_IMG2_UPP           L"MaskImg2Upp"          // 
#define INI_KEY_NOISE_FILTER1           L"NoiseFilter1"         // 
#define INI_KEY_NOISE_FILTER2           L"NoiseFilter2"         // 
#define INI_KEY_ALGORITHM               L"Algorithm"            // 
#define INI_KEY_FILTER                  L"Filter"               // 
#define INI_KEY_IMG_SAVE_FILENAME       L"ImgSaveFileName"      // 

#define INI_SCTION_RIO                  L"RIO"                  // 
#define INI_KEY_IP_ADDRS                L"IPAddrs"              // 
#define INI_KEY_TCP_PORT                L"TCPPort"              // 
#define INI_KEY_SLAVE_ADDRS             L"SlaveAddrs"           // 
#define INI_KEY_TIME_OUT                L"TimeOut"              // 
#define INI_KEY_PORT_NUM_X              L"PortNumX"             // 
#define INI_KEY_PORT_NUM_Y              L"PortNumY"             // 

#define INI_SCTION_ETHER                L"ETHER"                // 
#define INI_KEY_LOCAL_IP_ADDRS          L"LocalIPAddrs"         // 
#define INI_KEY_LOCAL_PORT              L"LocalPort"            // 
#define INI_KEY_REMOTE_IP_ADDRS         L"RemoteIPAddrs"        // 
#define INI_KEY_REMOTE_PORT             L"RemotePort"           // 

//----------------------------------------------------------------------------
// 
#define MSG_LIST_MAX                    14

//----------------------------------------------------------------------------
// ベース設定
// Main Windowの初期サイズ, 位置設定
#define MAIN_WND_INIT_SIZE_W            620
#define MAIN_WND_INIT_SIZE_H            420
#define MAIN_WND_INIT_POS_X             20
#define MAIN_WND_INIT_POS_Y             20

// Tweet Message表示用Static Windowのサイズ設定値
#define MSG_WND_H                       20                      // Height of window
#define MSG_WND_W                       590                     // Width of window
#define MSG_WND_ORG_X                   35                      // Origin X of window arrangement 
#define MSG_WND_ORG_Y                   10                      // Origin Y of window arrangement 
#define MSG_WND_Y_SPACE                 12                      // Space between windows Y direction 
#define TWEET_IMG_ORG_X                 5                       // Origin X of bitmap icon arrangement

// 各タスクアイコン用イメージリスト設定値
#define ICON_IMG_W                      32                      // イメージ幅
#define ICON_IMG_H                      32                      // イメージ高さ

// 各タスク設定用タブ関連
#define TAB_DIALOG_W                    620
#define TAB_DIALOG_H                    300
#define TAB_POS_X                       10
#define TAB_POS_Y                       (MSG_WND_ORG_Y + (MSG_WND_H + MSG_WND_Y_SPACE) * TASK_NUM + 10)
#define TAB_SIZE_H                      35

//----------------------------------------------------------------------------
// アプリケーション管理
#define PROC_ITEM_ID_MAX                16

//----------------------------------------------------------------------------
#ifndef MIN
#define MIN(a,b)                        ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)                        ((a) < (b) ? (b) : (a))
#endif

//----------------------------------------------------------------------------
// 共通
#define L_ON                            0x01                    // ON
#define L_OFF                           0x00                    // OFF

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
// カーネル管理構造体
typedef struct tagKNL_MANAGE_SET {
    WORD          mmt_resolution;                               // マルチメディアタイマーの分解能
    unsigned int  cycle_base;                                   // メインスレッド周期
    WORD          KnlTick_TimerID;                              // マルチメディアタイマーのID
    unsigned int  num_of_task;                                  // アプリケーションで利用するスレッド数
    unsigned long sys_counter;                                  // システムカウンタ
    SYSTEMTIME    Knl_Time;                                     // アプリケーション開始からの経過時間
    unsigned int  stackSize;                                    // タスクの初期スタックサイズﾞ

    tagKNL_MANAGE_SET() : mmt_resolution(TARGET_RESOLUTION),    // マルチメディアタイマーの分解能
                          cycle_base(SYSTEM_TICK_MSEC),         // メインスレッド周期
                          KnlTick_TimerID(0),                   // マルチメディアタイマーのID
                          num_of_task(0),                       // アプリケーションで利用するスレッド数
                          sys_counter(0),                       // システムカウンタ
                          Knl_Time(),                           // アプリケーション開始からの経過時間
                          stackSize(INITIAL_TASK_STACK_SIZE)    // タスクの初期スタックサイズ
    {}
} ST_KNL_MANAGE_SET, *PST_MANAGE_SET;

//----------------------------------------------------------------------------
// 
typedef struct tagAPP_MANAGE {
    DWORD proccess_status[PROC_ITEM_ID_MAX];                    // 連携ﾌﾟﾛｾｽの状態
    DWORD proccess_counter[PROC_ITEM_ID_MAX];                   // 連携ﾌﾟﾛｾｽのﾍﾙｼｰﾁｪｯｸ用
} ST_APP_MANAGE, *PST_APP_MANAGE;
