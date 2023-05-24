// SwaySensor.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "SwaySensor.h"

#include "CTaskObj.h"       // TaskObjクラス
#include "CHelper.h"        // Helperクラス
#include "CEnvironment.h"   // Environmentクラス
#include "CPolicy.h"        // Policyクラス
#include "CAgent.h"         // Agentクラス
#include "CScada.h"         // Scadaクラス
#include "CShared.h"        // 

#include <string>
#include <time.h>
#include <vector>

#include <windowsx.h>
#include <commctrl.h>
#include <mmsystem.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                            // 現在のインターフェイス
wchar_t   szTitle[MAX_LOADSTRING];          // タイトルバーのテキスト
wchar_t   szWindowClass[MAX_LOADSTRING];    // メインウィンドウクラス名

SYSTEMTIME g_app_start_time;                // アプリケーション開始時間

std::vector<void*> VectpCTaskObj;           // TaskObjのポインタ
TASK_INDEX         g_task_index;            // TaskObjのインデックス
CShared*           g_shared = NULL;         // タスク間共有データのポインタ

// スタティック変数:
static HWND                hwnd_status_bar; // ステータスバーのウィンドウのハンドル
static HWND                hwnd_tab;        // 操作パネル用タブコントロールウィンドウのハンドル
static KNL_MANAGE_SET      knl_manage_set;  // マルチスレッド管理用構造体
static std::vector<HWND>   VectTweetHandle; // メインウィンドウのスレッドツイートメッセージ表示Staticハンドル
static std::vector<HANDLE> VectHevent;      // マルチスレッド用イベントのハンドル

static HIMAGELIST hImgListTaskIcon;         // タスクアイコン用イメージリスト

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// ここにアプリケーション専用の関数を追加しています:
static unsigned __stdcall thread_gate_func(void* pObj);                                     // スレッド実行のためのゲート関数
int32_t          init_tasks(HWND hWnd);                                                     // アプリケーション毎のタスクオブジェクトの初期設定
DWORD            knl_task_startup(void);                                                    // 実行させるタスクの起動処理
VOID CALLBACK    alarm_handlar(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);    // マルチメディアタイマー処理関数 スレッドのイベントオブジェクト処理
HWND             create_statusbar_main(HWND);                                               // メインウィンドウステータスバー作成関数
HWND             create_task_setting_wnd(HWND hWnd);                                        // タブ付タスクウィンドウ作成
LRESULT CALLBACK task_tab_dlg_proc(HWND, UINT, WPARAM, LPARAM);                             // 個別タスク設定タブウィンドウ用メッセージハンドリング関数

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SWAYSENSOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SWAYSENSOR));

    //----------------------------------------------------------------------------
    // メイン メッセージ ループ:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //----------------------------------------------------------------------------
    // マルチメディアタイマーの停止
    if (knl_manage_set.knl_tick_timer_id != 0) {
        timeKillEvent(knl_manage_set.knl_tick_timer_id);
    }

    //----------------------------------------------------------------------------
    // オブジェクトの開放
    for (int32_t idx = (knl_manage_set.num_of_task - 1); idx >= 0; idx--) {
        CTaskObj* task = reinterpret_cast<CTaskObj*>(VectpCTaskObj[idx]);
        if (task != NULL) {
            task->thread_info.thread_com = TERMINATE_THREAD;
            PulseEvent(VectHevent[idx]);
            Sleep(1000);
            delete task;
        }
    }

    //----------------------------------------------------------------------------
    // タスク間共有データ
    if (g_shared != NULL) {
        delete g_shared;
    }

    return static_cast<int>(msg.wParam);
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SWAYSENSOR));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_SWAYSENSOR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;  // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(szWindowClass,
                              szTitle,
                              WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                              MAIN_WND_INIT_POS_X,
                              MAIN_WND_INIT_POS_Y,
                              TAB_DIALOG_W + 40,
                              (MSG_WND_H + MSG_WND_Y_SPACE) * TASK_NUM + TAB_DIALOG_H + 110,
                              nullptr,
                              nullptr,
                              hInstance,
                              nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    setlocale(LC_ALL, "");

    //----------------------------------------------------------------------------
    // タスク間共有データ
    g_shared = NULL;
    if ((g_shared = new CShared(TRUE)) == NULL) {
        return FALSE;
    }

    wchar_t path[_MAX_PATH], szDrive[_MAX_DRIVE], szPath[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    GetModuleFileName(NULL, path, sizeof(path) / sizeof(*path));    // exe failのpathを取得
    _wsplitpath_s(path,
                  szDrive, sizeof(szDrive) / sizeof(*szDrive),
                  szPath,  sizeof(szPath) / sizeof(*szPath),
                  szFName, sizeof(szFName) / sizeof(*szFName),
                  szExt,   sizeof(szExt) / sizeof(*szExt)); // 取得したpathを分割
    swprintf_s(path, sizeof(path) / sizeof(*path),
               L"%s%s%s%s.%s",
               szDrive, szPath, FOLDER_OF_INIFILE, szFName, EXT_OF_INIFILE);    // フォルダのパスとiniファイルのパスに合成
    // ini file読み込みパラメータ設定
    g_shared->set_app_config_ini(path);

    //----------------------------------------------------------------------------
    // タスク設定
    init_tasks(hWnd);   // タスク個別設定

    InvalidateRect(hWnd, NULL, FALSE);  // WM_PAINTを発生させてアイコンを描画させる
    UpdateWindow(hWnd);
 
    //----------------------------------------------------------------------------
    // タスク起動
    knl_task_startup();

    //----------------------------------------------------------------------------
    // マルチメディアタイマー起動
    {
        // マルチメディアタイマー精度設定
        TIMECAPS wTc;   // マルチメディアタイマー精度構造体
        if (timeGetDevCaps(&wTc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
            return static_cast<UINT>(FALSE);
        }
        knl_manage_set.mmt_resolution = MIN(MAX(wTc.wPeriodMin, TARGET_RESOLUTION), wTc.wPeriodMax);
        if (timeBeginPeriod(knl_manage_set.mmt_resolution) != TIMERR_NOERROR) {
            return static_cast<UINT>(FALSE);
        }

        _RPT1(_CRT_WARN, ">>>MMTimer Period = %d\n", knl_manage_set.mmt_resolution);

        // マルチメディアタイマーセット
        knl_manage_set.knl_tick_timer_id = timeSetEvent(knl_manage_set.cycle_base,
                                                        knl_manage_set.mmt_resolution,
                                                        reinterpret_cast<LPTIMECALLBACK>(alarm_handlar),
                                                        0,
                                                        TIME_PERIODIC);

        // マルチメディアタイマー起動失敗判定 メッセージBOX出してFALSE returen
        if (knl_manage_set.knl_tick_timer_id == 0) {    // 失敗確認表示
            LPVOID lpMsgBuf;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                          0, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
            MessageBox(NULL, (LPCTSTR)lpMsgBuf, L"MMT Failed!!", MB_OK | MB_ICONINFORMATION);   // Display the string.
            LocalFree(lpMsgBuf);    // Free the buffer.
            return static_cast<BOOL>(FALSE);
        }
    }

    return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE: 
        {
            // メインウィンドウにステータスバー付加
            hwnd_status_bar = create_statusbar_main(hWnd);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // タスクツイートメッセージアイコン描画
            for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
                ImageList_Draw(hImgListTaskIcon, i, hdc, 0, i * (MSG_WND_H + MSG_WND_Y_SPACE), ILD_NORMAL);
            }
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE: 
        {
            SendMessage(hwnd_status_bar, WM_SIZE, wParam, lParam);  // ステータスバーにサイズ変更を通知付加

            RECT rct;
            GetClientRect(hWnd, &rct);
            TabCtrl_AdjustRect(hwnd_tab, FALSE, &rct);
            MoveWindow(hwnd_tab, TAB_POS_X, TAB_POS_Y, TAB_DIALOG_W, TAB_DIALOG_H, TRUE);

            for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
                CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
                MoveWindow(task_obj->thread_info.hwnd_panel,
                           TAB_POS_X,
                           TAB_POS_Y + TAB_SIZE_H,
                           TAB_DIALOG_W,
                           TAB_DIALOG_H - TAB_SIZE_H,
                           TRUE);
            }
        }
        break;
    case WM_NOTIFY: 
        {
            switch ((reinterpret_cast<NMHDR*>(lParam))->code) {
            case TCN_SELCHANGE: // タブの切り替え
                {
                    int tab_index = TabCtrl_GetCurSel((reinterpret_cast<NMHDR*>(lParam))->hwndFrom);

                    for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
                        CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
                        MoveWindow(task_obj->thread_info.hwnd_panel,
                                   TAB_POS_X,
                                   TAB_POS_Y + TAB_SIZE_H,
                                   TAB_DIALOG_W,
                                   TAB_DIALOG_H - TAB_SIZE_H,
                                   TRUE);
                        if (task_obj->thread_info.index == tab_index) {
                            ShowWindow(task_obj->thread_info.hwnd_panel, SW_SHOW);
                            HWND hname_static = GetDlgItem(task_obj->thread_info.hwnd_panel, IDC_STATIC_TASKNAME);
                            SetWindowText(hname_static, task_obj->thread_info.name.c_str());
                            task_obj->set_tab_func_txt();   // タブパネルのRadio ButtonのStaticテキストを設定

                            // ウィンドウをフォアグランドに持ってくる
                            SetForegroundWindow(task_obj->thread_info.hwnd_panel);
                        }
                        else {
                            ShowWindow(task_obj->thread_info.hwnd_panel, SW_HIDE);
                        }
                    }
                }
                break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/// @brief スレッド実行のためのゲート関数
/// @param pObj:タスククラスインスタンスのポインタ
/// @return
/// @note
static unsigned __stdcall thread_gate_func(void* pObj)
{
    CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(pObj);

    return pTaskObj->run(pObj);
}

/// @brief スレッドタスクの登録、設定
/// @param
/// @return 
/// @note
int32_t init_tasks(HWND hWnd)
{
    HBITMAP   hbmp;
    CTaskObj* task_obj;
    int32_t   task_index = 0;

    //----------------------------------------------------------------------------
    // コモンコントロール初期化
    InitCommonControls();

    //----------------------------------------------------------------------------
    // タスクアイコン表示用イメージリスト設定
    hImgListTaskIcon = ImageList_Create(32, 32, ILC_COLOR | ILC_MASK, 2, 0);

    //----------------------------------------------------------------------------
    // システム設定
    CONFIG_SYSTEM cnfg_system;
    g_shared->get_app_config(&cnfg_system);
    (cnfg_system.cycletime_sys > 0) ? knl_manage_set.cycle_base = cnfg_system.cycletime_sys :   // メインスレッド周期
                                      knl_manage_set.cycle_base = SYSTEM_TICK_MSEC;             // メインスレッド周期

    //----------------------------------------------------------------------------
    // タスク設定
    CONFIG_TASK cnfg_task;
    g_shared->get_app_config(&cnfg_task);
    // Task1 設定 ENVIRONMENT
    {
        // タスクインスタンス作成->リスト登録
        task_obj = dynamic_cast<CTaskObj*>(new CEnvironment);
        VectpCTaskObj.push_back(reinterpret_cast<void*>(task_obj));
        g_task_index.environment = task_index;

        // タスクインデックスセット
        task_obj->thread_info.index = task_index++;

        // イベントオブジェクトCreate->リスト登録
        VectHevent.push_back(task_obj->thread_info.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL)); // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hbmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_ENVIRONMENT));    // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hbmp, RGB(255, 255, 255));
        DeleteObject(hbmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.environment;
        task_obj->thread_info.cycle_ms = task->cycletime;
        task_obj->thread_info.name     = task->name;
        task_obj->thread_info.sname    = task->sname;

        // 実行関数選択
        task_obj->thread_info.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        task_obj->thread_info.n_active_events = 1;
    }
    // Task2 設定 POLICY
    {
        // タスクインスタンス作成->リスト登録
        task_obj = dynamic_cast<CTaskObj*>(new CPolicy);
        VectpCTaskObj.push_back(reinterpret_cast<void*>(task_obj));
        g_task_index.policy = task_index;

        // タスクインデックスセット
        task_obj->thread_info.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(task_obj->thread_info.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hbmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_POLICY)); // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hbmp, RGB(255, 255, 255));
        DeleteObject(hbmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.policy;
        task_obj->thread_info.cycle_ms = task->cycletime;
        task_obj->thread_info.name     = task->name;
        task_obj->thread_info.sname    = task->sname;

        // 実行関数選択
        task_obj->thread_info.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        task_obj->thread_info.n_active_events = 1;
    }
    // Task3 設定 AGENT
    {
        // タスクインスタンス作成->リスト登録
        task_obj = dynamic_cast<CTaskObj*>(new CAgent);
        VectpCTaskObj.push_back(reinterpret_cast<void*>(task_obj));
        g_task_index.agent = task_index;

        // タスクインデックスセット
        task_obj->thread_info.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(task_obj->thread_info.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hbmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_AGENT));  // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hbmp, RGB(255, 255, 255));
        DeleteObject(hbmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.agent;
        task_obj->thread_info.cycle_ms = task->cycletime;
        task_obj->thread_info.name     = task->name;
        task_obj->thread_info.sname    = task->sname;

        // 実行関数選択
        task_obj->thread_info.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        task_obj->thread_info.n_active_events = 1;
    }
    // Task4 設定 SCADA
    {
        // タスクインスタンス作成->リスト登録
        task_obj = dynamic_cast<CTaskObj*>(new CScada);
        VectpCTaskObj.push_back(reinterpret_cast<void*>(task_obj));
        g_task_index.scada = task_index;

        // タスクインデックスセット
        task_obj->thread_info.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(task_obj->thread_info.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hbmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_SCADA));  // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hbmp, RGB(255, 255, 255));
        DeleteObject(hbmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.scada;
        task_obj->thread_info.cycle_ms = task->cycletime;
        task_obj->thread_info.name     = task->name;
        task_obj->thread_info.sname    = task->sname;

        // 実行関数選択
        task_obj->thread_info.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        task_obj->thread_info.n_active_events = 1;
    }

    // タスク数登録
    knl_manage_set.num_of_task = static_cast<uint32_t>(VectpCTaskObj.size());

    // 各タスク用設定ウィンドウ作成
    InitCommonControls();   // コモンコントロール初期化
    hwnd_tab = create_task_setting_wnd(hWnd);   // タブウィンドウ作成

    // 各タスク残パラメータセット
    for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
        task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
        task_obj->thread_info.index       = i;       // タスクインデックスセット
        task_obj->thread_info.hwnd_parent = hWnd;    // 親ウィンドウのハンドルセット
        task_obj->thread_info.hinstance   = hInst;   // 親ウィンドウのハンドルセット

        // ツイートメッセージ用Static window作成->リスト登録
        task_obj->thread_info.hwnd_msgstatics = CreateWindow(L"STATIC",
                                                             L"...",
                                                             WS_CHILD | WS_VISIBLE,
                                                             MSG_WND_ORG_X,
                                                             MSG_WND_ORG_Y + (MSG_WND_H * i) + (i * MSG_WND_Y_SPACE),
                                                             MSG_WND_W,
                                                             MSG_WND_H,
                                                             hWnd,
                                                             (HMENU)ID_STATIC_MAIN,
                                                             hInst,
                                                             NULL);
        VectTweetHandle.push_back(task_obj->thread_info.hwnd_msgstatics);

        // その他設定
        task_obj->thread_info.psys_counter = &knl_manage_set.sys_counter;
        task_obj->thread_info.act_count    = 0; // 起動チェック用カウンタリセット
        // 起動周期カウント値
        if (task_obj->thread_info.cycle_ms >= knl_manage_set.cycle_base) {
            task_obj->thread_info.cycle_count = task_obj->thread_info.cycle_ms / knl_manage_set.cycle_base;
        }
        else {
            task_obj->thread_info.cycle_count = 1;
        }

        // 最後に初期化関数呼び出し
        task_obj->init_task(task_obj);
    }

    return 1;
}

/// @brief マルチタスクスタートアップ処理関数
/// @param
/// @return 
/// @note 実行させるオブジェクトのリストのスレッドを起動
DWORD knl_task_startup(void)
{
    // 機能  ：[KNL]システム/ユーザータスクスタートアップ関数
    // 処理  ：自プロセスのプライオリティ設定,カーネルの初期設定,タスク生成,基本周期設定
    // 戻り値：Win32APIエラーコード

    HANDLE myPrcsHndl;  // 本プログラムのプロセスハンドル
    // 自プロセスプライオリティ設定処理
    // プロセスハンドル取得
    if ((myPrcsHndl = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION,
                                  FALSE,
                                  _getpid())) == NULL) {
        return GetLastError();
    }
    _RPTWN(_CRT_WARN, L">>>KNL Priority For Windows(before) = %d \n", GetPriorityClass(myPrcsHndl));

    // 自プロセスのプライオリティを最優先ランクに設定
    if (SetPriorityClass(myPrcsHndl, REALTIME_PRIORITY_CLASS) == 0) {
        return GetLastError();
    }
    _RPTWN(_CRT_WARN, L">>>KNL Priority For NT(after) = %d \n", GetPriorityClass(myPrcsHndl));

    // アプリケーションタスク数が最大数を超えた場合は終了
    if (knl_manage_set.num_of_task >= MAX_APP_TASK) {
        return static_cast<DWORD>(ERROR_BAD_ENVIRONMENT);
    }

    // アプリケーションスレッド生成処理
    for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
        CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);

        // タスク生成(スレッド生成)
        // 他プロセスとの共有なし,スタック初期サイズ デフォルト,スレッド実行関数 引数で渡すオブジェクトで対象切り替え,スレッド関数の引数(対象のオブジェクトのポインタ),即実行Createflags,スレッドID取り込み
        task_obj->thread_info.hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,                                      //_Security
                                                                             0,                                                          //_StackSize
                                                                             thread_gate_func,                                           //_StartAddress
                                                                             VectpCTaskObj[i],                                           //_ArgList
                                                                             0,                                                          //_InitFlag
                                                                             reinterpret_cast<unsigned*>(&(task_obj->thread_info.id)))); //_ThrdAddr

        // タスクプライオリティ設定
        if (task_obj->thread_info.hndl != NULL) {
            HANDLE hTh = task_obj->thread_info.hndl;
            if (SetThreadPriority(hTh, task_obj->thread_info.priority) == 0) {
                return GetLastError();
            }
        }
        else {
            return GetLastError();
        }
        _RPTWN(_CRT_WARN, L">>>Task[%d]_priority = %d\n", i, GetThreadPriority(task_obj->thread_info.hndl));

        task_obj->thread_info.act_count       = 0;  // 基本Tickのカウンタ変数クリア
        task_obj->thread_info.time_over_count = 0;  // 予定周期オーバーカウントクリア
    }

    return 0;
}

/// @brief マルチメディアタイマーイベント処理関数
/// @param
/// @return 
/// @note
VOID CALLBACK alarm_handlar(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    int64_t tmttl;

    knl_manage_set.sys_counter++;

    // スレッド再開イベントセット処理
    for (uint32_t i = 0; i < knl_manage_set.num_of_task; i++) {
        CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
        task_obj->thread_info.act_count++;
        if (task_obj->thread_info.act_count >= task_obj->thread_info.cycle_count) {
            PulseEvent(VectHevent[i]);
            task_obj->thread_info.act_count = 0;
            task_obj->thread_info.total_act++;
        }
    }

    // Statusバーに経過時間表示
    // 1sec毎
    if ((knl_manage_set.sys_counter % 40) == 0) {
        // 起動後経過時間計算
        tmttl = static_cast<int64_t>(knl_manage_set.sys_counter) * static_cast<int64_t>(knl_manage_set.cycle_base); // アプリケーション起動後の経過時間 msec
        knl_manage_set.knl_time.wMilliseconds = static_cast<WORD>(tmttl % 1000); tmttl /= 1000;
        knl_manage_set.knl_time.wSecond       = static_cast<WORD>(tmttl % 60);   tmttl /= 60;
        knl_manage_set.knl_time.wMinute       = static_cast<WORD>(tmttl % 60);   tmttl /= 60;
        knl_manage_set.knl_time.wHour         = static_cast<WORD>(tmttl % 60);   tmttl /= 24;
        knl_manage_set.knl_time.wDay          = static_cast<WORD>(tmttl % 24);

        std::wstring time;
        time = std::format(L"{:03d} {:02d}:{:02d}:{:02d}", knl_manage_set.knl_time.wDay,
                                                           knl_manage_set.knl_time.wHour,
                                                           knl_manage_set.knl_time.wMinute,
                                                           knl_manage_set.knl_time.wSecond);
        SendMessage(hwnd_status_bar, SB_SETTEXT, 5, reinterpret_cast<LPARAM>(time.c_str()));
    }
}

/// @brief ステータスバー作成
/// @param
/// @return 
/// @note
HWND create_statusbar_main(HWND hWnd)
{
    HWND    hSBWnd;
    int32_t sb_size[] = {100, 200, 300, 400, 525, 615};    // ステータス区切り位置

    InitCommonControls();
    hSBWnd = CreateWindowEx(0,                                  // 拡張スタイル
                            STATUSCLASSNAME,                    // ウィンドウクラス
                            NULL,                               // タイトル
                            WS_CHILD | SBS_SIZEGRIP,            // ウィンドウスタイル
                            0, 0,                               // 位置
                            0, 0,                               // 幅、高さ
                            hWnd,                               // 親ウィンドウ
                            reinterpret_cast<HMENU>(ID_STATUS), // ウィンドウのID
                            hInst,                              // インスタンスハンドル
                            NULL);
    SendMessage(hSBWnd,
                SB_SETPARTS,
                static_cast<WPARAM>(6),
                reinterpret_cast<LPARAM>(static_cast<LPINT>(sb_size))); // 6枠で各枠の仕切り位置をパラメータ指定
    ShowWindow(hSBWnd, SW_SHOW);

    return hSBWnd;
}

/// @brief タブ付タスクウィンドウ作成
/// @param
/// @return 
/// @note
HWND create_task_setting_wnd(HWND hWnd)
{
    RECT    rc;
    TC_ITEM tc[MAX_APP_TASK];

    GetClientRect(hWnd, &rc);
    HWND hTab = CreateWindowEx(0,
                               WC_TABCONTROL,
                               NULL,
                               WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
                               rc.left + TAB_POS_X,
                               rc.top + TAB_POS_Y,
                               TAB_DIALOG_W,
                               TAB_DIALOG_H,
                               hWnd,
                               reinterpret_cast<HMENU>(ID_TASK_SET_TAB),
                               hInst,
                               NULL);
    // Task Setting用タブ作成
    for (int32_t idx = (knl_manage_set.num_of_task - 1); idx >= 0; idx--) {
        CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[idx]);

        tc[idx].mask    = (TCIF_TEXT | TCIF_IMAGE);
        tc[idx].pszText = const_cast<LPWSTR>(task_obj->thread_info.sname.c_str());
        tc[idx].iImage  = task_obj->thread_info.index;
        SendMessage(hTab,
                    TCM_INSERTITEM,
                    static_cast<WPARAM>(0),
                    reinterpret_cast<LPARAM>(&tc[idx]));
        task_obj->thread_info.hwnd_panel = CreateDialog(hInst,
                                                        MAKEINTRESOURCE(IDD_DIALOG_TASKSET),
                                                        hWnd,
                                                        reinterpret_cast<DLGPROC>(task_tab_dlg_proc));
        task_obj->set_tab_func_txt();   // タブパネルのRadio ButtonのStaticテキストを設定
        MoveWindow(task_obj->thread_info.hwnd_panel,
                   TAB_POS_X,
                   TAB_POS_Y + TAB_SIZE_H,
                   TAB_DIALOG_W,
                   TAB_DIALOG_H - TAB_SIZE_H,
                   TRUE);
        ShowWindow(task_obj->thread_info.hwnd_panel, SW_HIDE);
    }

    // タブの表示
    int32_t idx = 0;
    CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[idx]);
    ShowWindow(task_obj->thread_info.hwnd_panel, SW_SHOW);
    SetWindowText(GetDlgItem(task_obj->thread_info.hwnd_panel, IDC_STATIC_TASKNAME),
                  task_obj->thread_info.name.c_str());  // タスク名をスタティックテキストに表示
    TabCtrl_SetCurSel(hTab, idx);

    // タブコントロールにイメージリストセット
    SendMessage(hTab, TCM_SETIMAGELIST, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(hImgListTaskIcon));

    return hTab;
}

static LVCOLUMNA lvcol;

/// @brief 
/// @param
/// @return 
/// @note
LRESULT CALLBACK task_tab_dlg_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        {
            InitCommonControls();

            // リストビュー
            LVCOLUMN lvcol;
            LPTSTR   title[] = {const_cast<LPTSTR>(L"Time"), const_cast<LPTSTR>(L"Message")};   // 列ラベル
            int32_t  cx[] = {LISTVIEW_COLUMN_WIDTH_TIME, LISTVIEW_COLUMN_WIDTH_MESSAGE};        // 列幅

            // リストコントロール設定
            HWND hwnd_list = GetDlgItem(hDlg, IDC_LIST);
            lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvcol.fmt  = LVCFMT_LEFT;
            for (int32_t clmn = 0; clmn < LISTVIEW_COLUMN_MAX; clmn++) {
                lvcol.cx       = cx[clmn];      // 表示位置
                lvcol.pszText  = title[clmn];   // 見出し
                lvcol.iSubItem = clmn;          // サブアイテムの番号
                ListView_InsertColumn(hwnd_list, clmn, &lvcol);
            }
//          // リスト行追加
//          LVITEM lvitem;
//          lvitem.mask = LVIF_TEXT;
//          for (int32_t row = 0; row < LISTVIEW_ROW_MAX; row++) {
//              lvitem.pszText  = const_cast<LPWSTR>(L"");  // テキスト
//              lvitem.iItem    = row;                      // 番号
//              lvitem.iSubItem = 0;                        // サブアイテムの番号
//              ListView_InsertItem(hwnd_list, &lvitem);
//          }
            return TRUE;
        }
        break;
    case WM_COMMAND:
        {
            CTaskObj* task_obj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[TabCtrl_GetCurSel(hwnd_tab)]);
            task_obj->set_tab(hDlg, msg, wp, lp);   // タブパネルの設定
        }
        break;
    default:
        break;
    }
    return FALSE;
}
