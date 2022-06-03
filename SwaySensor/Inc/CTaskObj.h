#pragma once

#include <windows.h>
#include <process.h>
#include <sstream>
#include <iostream>
#include "CHelper.h"

using namespace std;

//============================================================================
// define定義
// スレッドで実行する関数選択用定義
#define THREAD_WORK_IDLE                0   // アイドル処理
#define THREAD_WORK_ROUTINE             1   // 定常処理
#define THREAD_WORK_OPTION1             2   // オプション処理
#define THREAD_WORK_OPTION2             3   // オプション処理

#define TASK_EVENT_MAX                  8   // タスクスレッド起動イベント最大数
#define DEFAUT_TASK_CYCLE               50  // 標準タスク周期

#define REPEAT_INFINIT                  0   // 永久ループ
#define TERMINATE_THREAD                1   // 中断

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
// タスクオブジェクトインデックス構造体    
typedef struct tagTASK_INDEX {
    int environment;
    int scada;
    int agent;
    int policy;
    int device;
    int client;
    int manage;
    int dummy;
} ST_TASK_INDEX, *PST_TASK_INDEX;

//----------------------------------------------------------------------------
// タスクオブジェクトの個別管理情報構造体
typedef struct tagTHREAD_INFO {
    // オブジェクト識別情報
    WCHAR   name[24];   // オブジェクト名
    WCHAR   sname[8];   // オブジェクト略称
    HBITMAP hBmp;       // オブジェクト識別用ビットマップ

    // スレッド設定内容
    int          index; // スレッドIndex
    unsigned int ID;    // スレッドID
    HANDLE       hndl;  // スレッドハンドル
    HANDLE       hevents[TASK_EVENT_MAX];   // イベントハンドル
//  int          n_active_events = 1;       // 有効なイベント数
    int          n_active_events;                               // 有効なイベント数
    int          event_triggered;                               // 発生したイベント番号
    unsigned int cycle_ms;                                      // スレッド実行設定周期
    unsigned int cycle_count;                                   // スレッド実行設定周期 Tick count(ms/system tick)
    int          trigger_type;                                  // スレッド起動条件 定周期 or イベント
    int          priority;                                      // スレッドのプライオリティ
    int          thread_com;                                    // スレッド制御フラグ 0:繰り返し 1:中断 それ以外:逐次処理:

    // スレッドモニタ情報
    DWORD        start_time;                                    // 現スキャンのスレッド開始時間
    DWORD        act_time;                                      // 1スキャンのスレッド実処理時間
    DWORD        period;                                        // スレッドループ周期実績
    DWORD        act_count;                                     // スレッドループカウンタ
    DWORD        total_act;                                     // 起動積算カウンタ
    unsigned int time_over_count;                               // 予定周期をオーバーした回数

    // 関連ウィンドウハンドル
    HWND hWnd_parent;                                           // 親ウィンドウのハンドル
    HWND hWnd_msgStatics;                                       // 親ウィンドウメッセージ表示用ウィンドウへのハンドル
    HWND hWnd_opepane;                                          // 自メインウィンドウのハンドル(メインフレーム上に配置）
    HWND hWnd_msgList;                                          // 自メインウィンドウのメッセージ表示用リストコントロールへのハンドル
    HWND hWnd_work;                                             // 自専用作業用ウインドウのハンドル

    HINSTANCE hInstance;

    // 操作パネル関連
    int cnt_PNLlist_msg;                                        // パネルメッセージリストのカウント数
    int panel_func_id;                                          // パネルfunctionボタンの選択内容
    int panel_type_id;                                          // パネルtypeボタンの選択内容

    // 外部インターフェース
    unsigned long* psys_counter;                                // メインシステムカウンターの参照先ポインタ
    unsigned       work_select;                                 // スレッド実行の関数の種類を指定

    tagTHREAD_INFO() : n_active_events(1),                      // 有効なイベント数
                       cnt_PNLlist_msg(0),                      // パネルメッセージリストのカウント数
                       panel_func_id(1),                        // パネルfunctionボタンの選択内容
                       panel_type_id(1),                        // パネルtypeボタンの選択内容
                       work_select(0)                           // スレッド実行の関数の種類を指定
    {}
} ST_THREAD_INFO, *PST_THREAD_INFO;

class CTaskObj
{
public:
    CTaskObj();
    virtual ~CTaskObj();

    ST_THREAD_INFO ThreadInfo;

    virtual void InitTask(void* pobj);
    unsigned __stdcall Run(void* param);    // スレッド実行対象関数

    virtual LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    unsigned int SetWork(int work_id);
    virtual void SetPanelPbTxt(void);   // タブパネルのFunctionボタンのStaticテキストを設定

protected:
    CHelper        tool;
    ostringstream  s;
    wostringstream ws;
    wstring        wstr;
    string         str;

    virtual void RoutineWork(void* param);
    virtual void OptionalWork1(void* param);
    virtual void OptionalWork2(void* param);
    virtual void DefaultWork(void* param);

private:
};
