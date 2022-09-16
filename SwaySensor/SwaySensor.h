#pragma once

#include "resource.h"

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
typedef struct TAG_KNL_MANAGE_SET {
    WORD          mmt_resolution;                               // マルチメディアタイマーの分解能
    unsigned int  cycle_base;                                   // メインスレッド周期
    WORD          KnlTick_TimerID;                              // マルチメディアタイマーのID
    unsigned int  num_of_task;                                  // アプリケーションで利用するスレッド数
    unsigned long sys_counter;                                  // システムカウンタ
    SYSTEMTIME    Knl_Time;                                     // アプリケーション開始からの経過時間
    unsigned int  stackSize;                                    // タスクの初期スタックサイズﾞ

    TAG_KNL_MANAGE_SET()
        : mmt_resolution(TARGET_RESOLUTION)
        , cycle_base(SYSTEM_TICK_MSEC)
        , KnlTick_TimerID(0)
        , num_of_task(0)
        , sys_counter(0)
        , Knl_Time()
        , stackSize(INITIAL_TASK_STACK_SIZE)
    {}
} KNL_MANAGE_SET, *PKNL_MANAGE_SET;

//----------------------------------------------------------------------------
// 
typedef struct TAG_APP_MANAGE {
    DWORD proccess_status[PROC_ITEM_ID_MAX];                    // 連携ﾌﾟﾛｾｽの状態
    DWORD proccess_counter[PROC_ITEM_ID_MAX];                   // 連携ﾌﾟﾛｾｽのﾍﾙｼｰﾁｪｯｸ用

    TAG_APP_MANAGE()
        : proccess_status{}
        , proccess_counter{}
    {}
} APP_MANAGE, *PAPP_MANAGE;
