#pragma once

#include "framework.h"
#include <windows.h>
#include <string>
#include <format>
#include <process.h>
#include <sstream>
#include <iostream>
#include "CHelper.h"

//============================================================================
// define定義
// スレッドで実行する関数選択用定義
#define THREAD_WORK_IDLE                0       // アイドル処理
#define THREAD_WORK_ROUTINE             1       // 定常処理

#define TASK_EVENT_MAX                  8       // タスクスレッド起動イベント最大数
#define DEFAUT_TASK_CYCLE               50      // 標準タスク周期

#define REPEAT_INFINIT                  0       // 永久ループ
#define TERMINATE_THREAD                1       // 中断

#define LISTVIEW_ROW_MAX                1000    // リストビュー行数
#define LISTVIEW_COLUMN_MAX             2       // リストビュー列数
#define LISTVIEW_COLUMN_WIDTH_TIME      90      // リストビュー列幅(Time)
#define LISTVIEW_COLUMN_WIDTH_MESSAGE   450     // リストビュー列幅(Message)

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
// タスクオブジェクトインデックス構造体    
typedef struct TAG_TASK_INDEX {
    int32_t environment;
    int32_t scada;
    int32_t agent;
    int32_t policy;
    int32_t device;
    int32_t client;
    int32_t manage;
    int32_t dummy;
} TASK_INDEX, *PTASK_INDEX;

//----------------------------------------------------------------------------
// タスクオブジェクトの個別管理情報構造体
typedef struct TAG_THREAD_INFO {
    // オブジェクト識別情報
    std::wstring name;  // オブジェクト名
    std::wstring sname; // オブジェクト略称
    HBITMAP      hbmp;  // オブジェクト識別用ビットマップ

    // スレッド設定内容
    int32_t  index;                     // スレッドIndex
    uint32_t id;                        // スレッドID
    HANDLE   hndl;                      // スレッドハンドル
    HANDLE   hevents[TASK_EVENT_MAX];   // イベントハンドル
    int32_t  n_active_events;           // 有効なイベント数
    int32_t  event_triggered;           // 発生したイベント番号
    uint32_t cycle_ms;                  // スレッド実行設定周期
    uint32_t cycle_count;               // スレッド実行設定周期 Tick count(ms/system tick)
    int32_t  trigger_type;              // スレッド起動条件 定周期 or イベント
    int32_t  priority;                  // スレッドのプライオリティ
    int32_t  thread_com;                // スレッド制御フラグ 0:繰り返し 1:中断 それ以外:逐次処理:

    // スレッドモニタ情報
    DWORD    start_time;        // 現スキャンのスレッド開始時間
    DWORD    act_time;          // 1スキャンのスレッド実処理時間
    DWORD    period;            // スレッドループ周期実績
    DWORD    act_count;         // スレッドループカウンタ
    DWORD    total_act;         // 起動積算カウンタ
    uint32_t time_over_count;   // 予定周期をオーバーした回数

    // 関連ウィンドウハンドル
    HWND hwnd_parent;       // 親ウィンドウのハンドル
    HWND hwnd_msgstatics;   // 親ウィンドウメッセージ表示用ウィンドウへのハンドル
    HWND hwnd_panel;        // 自メインウィンドウのハンドル(メインフレーム上に配置）
    HWND hwnd_msglist;      // 自メインウィンドウのメッセージ表示用リストコントロールへのハンドル
    HWND hwnd_work;         // 自専用作業用ウインドウのハンドル

    HINSTANCE hinstance;    // アプリケーションのインスタンス

    // 操作パネル関連
    int32_t panel_msglist_count;    // パネルメッセージリストのカウント数
    int32_t panel_func_id;          // パネルfunctionボタンの選択内容
    int32_t panel_type_id;          // パネルtypeボタンの選択内容

    // 外部インターフェース
    unsigned long* psys_counter;    // メインシステムカウンターの参照先ポインタ
    uint32_t       work_select;     // スレッド実行の関数の種類を指定

    TAG_THREAD_INFO()
        : name(L"")
        , sname(L"")
        , hbmp(NULL)
        , index(0)
        , id(0)
        , hndl(NULL)
        , hevents{}
        , n_active_events(1)
        , event_triggered(0)
        , cycle_ms(DEFAUT_TASK_CYCLE)
        , cycle_count(0)
        , trigger_type(0)
        , priority(THREAD_PRIORITY_NORMAL)
        , thread_com(REPEAT_INFINIT)
        , start_time(0)
        , act_time(0)
        , period(0)
        , act_count(0)
        , total_act(0)
        , time_over_count(0)
        , hwnd_parent(NULL)
        , hwnd_msgstatics(NULL)
        , hwnd_panel(NULL)
        , hwnd_msglist(NULL)
        , hwnd_work(NULL)
        , hinstance(NULL)
        , panel_msglist_count(0)
        , panel_func_id(0)
        , panel_type_id(0)
        , psys_counter(NULL)
        , work_select(THREAD_WORK_IDLE)
    {}
} THREAD_INFO, *PTHREAD_INFO;

//////////////////////////////////////////////////////////////////////////////
// CTaskObj

class CTaskObj
{
public:
    CTaskObj();
    virtual ~CTaskObj();

    // メンバー変数
    THREAD_INFO thread_info;

    // メンバー関数
    virtual void init_task(void* pobj);
    unsigned __stdcall run(void* param);    // スレッド実行対象関数

    virtual void set_tab_func_txt(void);    // タブパネルのRadio ButtonのStaticテキストを設定
    virtual void set_tab_tip_txt(void);     // タブパネルの説明用Staticテキストを設定
    virtual void set_tab(HWND hDlg,
                         UINT msg,
                         WPARAM wp,
                         LPARAM lp);        // タブパネルの設定

protected:
    // メンバー変数

    // メンバー関数
    void outp_msg_to_mainwnd(const std::string& src);   // メインウィンドウへのショートメッセージ表示(string)
    void outp_msg_to_mainwnd(const std::wstring& src);  // メインウィンドウへのショートメッセージ表示(wstring)
    void outp_msg_to_listview(const std::string str);   // リストコントロールへのショートメッセージ表示(string)
    void outp_msg_to_listview(const std::wstring str);  // リストコントロールへのショートメッセージ表示(wstring)

private:
    // メンバー変数
    CRITICAL_SECTION cs_listview;

    CHelper             tool;
    std::ostringstream  s;
    std::wostringstream ws;
    std::wstring        wstr;
    std::string         str;

    // メンバー関数
    virtual void routine_work(void* param);
    virtual void default_work(void* param);
};
