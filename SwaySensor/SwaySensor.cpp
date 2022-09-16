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
WCHAR     szTitle[MAX_LOADSTRING];          // タイトルバーのテキスト
WCHAR     szWindowClass[MAX_LOADSTRING];    // メインウィンドウクラス名

SYSTEMTIME g_AppStartTime;                  // アプリケーション開始時間

std::vector<void*> VectpCTaskObj;           // TaskObjのポインタ
TASK_INDEX         g_TaskIndex;             // TaskObjのインデックス
APP_MANAGE         g_AppManage;             // アプリケーション管理
CShared*           g_pShared = NULL;        // タスク間共有データのポインタ

// スタティック変数:
static HWND                hWnd_status_bar; // ステータスバーのウィンドウのハンドル
static HWND                hTabWnd;         // 操作パネル用タブコントロールウィンドウのハンドル
static KNL_MANAGE_SET      KNLManageSet;    // マルチスレッド管理用構造体
static std::vector<HWND>   VectTweetHandle; // メインウィンドウのスレッドツイートメッセージ表示Staticハンドル
static std::vector<HANDLE> VectHevent;      // マルチスレッド用イベントのハンドル

static HIMAGELIST hImgListTaskIcon;         // タスクアイコン用イメージリスト

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// ここにアプリケーション専用の関数を追加しています:
static unsigned __stdcall ThreadGateFunc(void* pObj);                                   // スレッド実行のためのゲート関数
int              InitTasks(HWND hWnd);                                                  // アプリケーション毎のタスクオブジェクトの初期設定
DWORD            KNLTaskStartUp();                                                      // 実行させるタスクの起動処理
VOID CALLBACK    AlarmHandlar(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2); // マルチメディアタイマー処理関数 スレッドのイベントオブジェクト処理
HWND             CreateStatusbarMain(HWND);                                             // メインウィンドウステータスバー作成関数
HWND             CreateTaskSettingWnd(HWND hWnd);                                       // タブ付タスクウィンドウ作成
LRESULT CALLBACK TaskTabDlgProc(HWND, UINT, WPARAM, LPARAM);                            // 個別タスク設定タブウィンドウ用メッセージハンドリング関数

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

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //
    for (int idx = (KNLManageSet.num_of_task - 1); idx >= 0; idx--) {
        CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[idx]);
        if (pTaskObj != NULL) {
            pTaskObj->ThreadInfo.thread_com = TERMINATE_THREAD;
            PulseEvent(VectHevent[idx]);
            Sleep(100);
            delete pTaskObj;
        }
    }

    // タスク間共有データ
    if (g_pShared != NULL) {
        delete g_pShared;
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SWAYSENSOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SWAYSENSOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
    // タスク間共有データ設定
    g_pShared = new CShared();
    g_pShared->set_appconfig();

    //----------------------------------------------------------------------------
    // タスク設定
    InitTasks(hWnd);    // タスク個別設定

    InvalidateRect(hWnd, NULL, FALSE);  // WM_PAINTを発生させてアイコンを描画させる
    UpdateWindow(hWnd);
 
    //----------------------------------------------------------------------------
    // タスク起動
    KNLTaskStartUp();

    //----------------------------------------------------------------------------
    // マルチメディアタイマー起動
    {
        // マルチメディアタイマー精度設定
        TIMECAPS wTc;   // マルチメディアタイマー精度構造体
        if (timeGetDevCaps(&wTc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
            return((DWORD)FALSE);
        }
        KNLManageSet.mmt_resolution = MIN(MAX(wTc.wPeriodMin, TARGET_RESOLUTION), wTc.wPeriodMax);
        if (timeBeginPeriod(KNLManageSet.mmt_resolution) != TIMERR_NOERROR) {
            return((DWORD)FALSE);
        }

        _RPT1(_CRT_WARN, ">>>MMTimer Period = %d\n", KNLManageSet.mmt_resolution);

        // マルチメディアタイマーセット
        KNLManageSet.KnlTick_TimerID = timeSetEvent(KNLManageSet.cycle_base, KNLManageSet.mmt_resolution, (LPTIMECALLBACK)AlarmHandlar, 0, TIME_PERIODIC);

        // マルチメディアタイマー起動失敗判定 メッセージBOX出してFALSE returen
        if (KNLManageSet.KnlTick_TimerID == 0) {    // 失敗確認表示
            LPVOID lpMsgBuf;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                          0, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
            MessageBox(NULL, (LPCTSTR)lpMsgBuf, L"MMT Failed!!", MB_OK | MB_ICONINFORMATION);   // Display the string.
            LocalFree(lpMsgBuf);    // Free the buffer.
            return((DWORD)FALSE);
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
            hWnd_status_bar = CreateStatusbarMain(hWnd);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // タスクツイートメッセージアイコン描画
            for (unsigned int i = 0; i < KNLManageSet.num_of_task; i++) {
                ImageList_Draw(hImgListTaskIcon, i, hdc, 0, i * (MSG_WND_H + MSG_WND_Y_SPACE), ILD_NORMAL);
            }
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE: 
        {
            SendMessage(hWnd_status_bar, WM_SIZE, wParam, lParam);  // ステータスバーにサイズ変更を通知付加

            RECT rc;
            GetClientRect(hWnd, &rc);
            TabCtrl_AdjustRect(hTabWnd, FALSE, &rc);
            MoveWindow(hTabWnd, TAB_POS_X, TAB_POS_Y, TAB_DIALOG_W, TAB_DIALOG_H, TRUE);

            for (unsigned int i = 0; i < VectpCTaskObj.size(); i++) {
                CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
                MoveWindow(pTaskObj->ThreadInfo.hWnd_opepane, TAB_POS_X, TAB_POS_Y + TAB_SIZE_H, TAB_DIALOG_W, TAB_DIALOG_H - TAB_SIZE_H, TRUE);
            }
        }
        break;
    case WM_NOTIFY: 
        {
            int tab_index = TabCtrl_GetCurSel(((NMHDR*)lParam)->hwndFrom);

            for (unsigned int i = 0; i < VectpCTaskObj.size(); i++) {
                CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);
                MoveWindow(pTaskObj->ThreadInfo.hWnd_opepane, TAB_POS_X, TAB_POS_Y + TAB_SIZE_H, TAB_DIALOG_W, TAB_DIALOG_H - TAB_SIZE_H, TRUE);
                if ((VectpCTaskObj.size() - 1 - pTaskObj->ThreadInfo.index) == tab_index) {
                    ShowWindow(pTaskObj->ThreadInfo.hWnd_opepane, SW_SHOW);
                    HWND hname_static = GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_STATIC_TASKNAME);
                    SetWindowText(hname_static, pTaskObj->ThreadInfo.name.c_str());
                    pTaskObj->set_window();

//////////////////////////////////////////////////////////////////////////////
#if 0
                    // 実行関数の設定状況に応じてOption Checkボタンセット
                    if (pTaskObj->ThreadInfo.work_select == THREAD_WORK_OPTION1) {
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK1), BM_SETCHECK, BST_CHECKED,   0L);
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK2), BM_SETCHECK, BST_UNCHECKED, 0L);
                    }
                    else if (pTaskObj->ThreadInfo.work_select == THREAD_WORK_OPTION2) {
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0L);
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK2), BM_SETCHECK, BST_CHECKED,   0L);
                    }
                    else {
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0L);
                        SendMessage(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_TASK_OPTION_CHECK2), BM_SETCHECK, BST_UNCHECKED, 0L);
                    }
#endif
//////////////////////////////////////////////////////////////////////////////

                    // ウィンドウをフォアグランドに持ってくる
                    SetForegroundWindow(pTaskObj->ThreadInfo.hWnd_opepane);
                }
                else {
                    ShowWindow(pTaskObj->ThreadInfo.hWnd_opepane, SW_HIDE);
                }
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
static unsigned __stdcall ThreadGateFunc(void* pObj)
{
    CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(pObj);
    return pTaskObj->run(pObj);
}

/// @brief スレッドタスクの登録、設定
/// @param
/// @return 
/// @note
int InitTasks(HWND hWnd)
{
    HBITMAP   hBmp;
    CTaskObj* pTaskObj;
    int       task_index = 0;

    //----------------------------------------------------------------------------
    // コモンコントロール初期化
    InitCommonControls();

    //----------------------------------------------------------------------------
    // タスクアイコン表示用イメージリスト設定
    hImgListTaskIcon = ImageList_Create(32, 32, ILC_COLOR | ILC_MASK, 2, 0);

    //----------------------------------------------------------------------------
    // タスク設定
    CONFIG_TASK cnfg_task;
    g_pShared->get_appconfig(&cnfg_task);
    // Task1 設定 ENVIRONMENT
    {
        // タスクインスタンス作成->リスト登録
        pTaskObj = dynamic_cast<CTaskObj*>(new CEnvironment);
        VectpCTaskObj.push_back((void*)pTaskObj);
        g_TaskIndex.environment = task_index;

        // タスクインデックスセット
        pTaskObj->ThreadInfo.index = task_index++;

        // イベントオブジェクトCreate->リスト登録
        VectHevent.push_back(pTaskObj->ThreadInfo.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL)); // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hBmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_ENVIRONMENT));    // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hBmp, RGB(255, 255, 255));
        DeleteObject(hBmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.environment;
        pTaskObj->ThreadInfo.cycle_ms = task->cycletime;
        pTaskObj->ThreadInfo.name     = task->name;
        pTaskObj->ThreadInfo.sname    = task->sname;

        // 実行関数選択
        pTaskObj->ThreadInfo.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        pTaskObj->ThreadInfo.n_active_events = 1;
    }
    // Task2 設定 POLICY
    {
        // タスクインスタンス作成->リスト登録
        pTaskObj = dynamic_cast<CTaskObj*>(new CPolicy);
        VectpCTaskObj.push_back((void*)pTaskObj);
        g_TaskIndex.policy = task_index;

        // タスクインデックスセット
        pTaskObj->ThreadInfo.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(pTaskObj->ThreadInfo.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hBmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_POLICY)); // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hBmp, RGB(255, 255, 255));
        DeleteObject(hBmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.policy;
        pTaskObj->ThreadInfo.cycle_ms = task->cycletime;
        pTaskObj->ThreadInfo.name     = task->name;
        pTaskObj->ThreadInfo.sname    = task->sname;

        // 実行関数選択
        pTaskObj->ThreadInfo.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        pTaskObj->ThreadInfo.n_active_events = 1;
    }
    // Task3 設定 SCADA
    {
        // タスクインスタンス作成->リスト登録
        pTaskObj = dynamic_cast<CTaskObj*>(new CScada);
        VectpCTaskObj.push_back((void*)pTaskObj);
        g_TaskIndex.scada = task_index;

        // タスクインデックスセット
        pTaskObj->ThreadInfo.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(pTaskObj->ThreadInfo.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hBmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_SCADA));  // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hBmp, RGB(255, 255, 255));
        DeleteObject(hBmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.scada;
        pTaskObj->ThreadInfo.cycle_ms = task->cycletime;
        pTaskObj->ThreadInfo.name     = task->name;
        pTaskObj->ThreadInfo.sname    = task->sname;

        // 実行関数選択
        pTaskObj->ThreadInfo.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        pTaskObj->ThreadInfo.n_active_events = 1;
    }
    // Task4 設定 AGENT
    {
        // タスクインスタンス作成->リスト登録
        pTaskObj = dynamic_cast<CTaskObj*>(new CAgent);
        VectpCTaskObj.push_back((void*)pTaskObj);
        g_TaskIndex.agent = task_index;

        // タスクインデックスセット
        pTaskObj->ThreadInfo.index = task_index++;

        // イベントオブジェクトCreate->リスト登録    
        VectHevent.push_back(pTaskObj->ThreadInfo.hevents[ID_TIMER_EVENT] = CreateEvent(NULL, FALSE, FALSE, NULL));    // 自動リセット,初期値非シグナル

        // ツイートメッセージ用iconセット
        hBmp = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BMP_AGENT));  // ビットマップ割り当て
        ImageList_AddMasked(hImgListTaskIcon, hBmp, RGB(255, 255, 255));
        DeleteObject(hBmp);

        // オブジェクト情報のセット
        PCONFIG_TASK_DATA task = &cnfg_task.agent;
        pTaskObj->ThreadInfo.cycle_ms = task->cycletime;
        pTaskObj->ThreadInfo.name     = task->name;
        pTaskObj->ThreadInfo.sname    = task->sname;

        // 実行関数選択
        pTaskObj->ThreadInfo.work_select = THREAD_WORK_ROUTINE;

        // スレッド起動に使うイベント数(定周期タイマーのみの場合1)
        pTaskObj->ThreadInfo.n_active_events = 1;
    }

    // 各タスク用設定ウィンドウ作成
    InitCommonControls();   // コモンコントロール初期化
    hTabWnd = CreateTaskSettingWnd(hWnd);   // タブウィンドウ作成

    // 各タスク残パラメータセット
    KNLManageSet.num_of_task = (unsigned int)VectpCTaskObj.size();  // タスク数登録
    for (unsigned i = 0; i < KNLManageSet.num_of_task; i++) {
        pTaskObj = (CTaskObj*)VectpCTaskObj[i];
        pTaskObj->ThreadInfo.index       = i;       // タスクインデックスセット
        pTaskObj->ThreadInfo.hWnd_parent = hWnd;    // 親ウィンドウのハンドルセット
        pTaskObj->ThreadInfo.hInstance   = hInst;   // 親ウィンドウのハンドルセット

        // ツイートメッセージ用Static window作成->リスト登録
        pTaskObj->ThreadInfo.hWnd_msgStatics = CreateWindow(L"STATIC",
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
        VectTweetHandle.push_back(pTaskObj->ThreadInfo.hWnd_msgStatics);

        // その他設定
        pTaskObj->ThreadInfo.psys_counter = &KNLManageSet.sys_counter;
        pTaskObj->ThreadInfo.act_count    = 0;  // 起動チェック用カウンタリセット
        // 起動周期カウント値
        if (pTaskObj->ThreadInfo.cycle_ms >= SYSTEM_TICK_MSEC) {
            pTaskObj->ThreadInfo.cycle_count = pTaskObj->ThreadInfo.cycle_ms / SYSTEM_TICK_MSEC;
        }
        else {
            pTaskObj->ThreadInfo.cycle_count = 1;
        }

        // 最後に初期化関数呼び出し
        pTaskObj->init_task(pTaskObj);
    }

    return 1;
}

/// @brief マルチタスクスタートアップ処理関数
/// @param
/// @return 
/// @note 実行させるオブジェクトのリストのスレッドを起動
DWORD KNLTaskStartUp()
{
    // 機能  ：[KNL]システム/ユーザータスクスタートアップ関数
    // 処理  ：自プロセスのプライオリティ設定,カーネルの初期設定,タスク生成,基本周期設定
    // 戻り値：Win32APIエラーコード

    HANDLE myPrcsHndl;  // 本プログラムのプロセスハンドル
    // 自プロセスプライオリティ設定処理
    // プロセスハンドル取得
    if ((myPrcsHndl = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_INFORMATION, FALSE, _getpid())) == NULL) {
        return(GetLastError());
    }
    _RPT1(_CRT_WARN, ">>>KNL Priority For Windows(before) = %d \n", GetPriorityClass(myPrcsHndl));

    // 自プロセスのプライオリティを最優先ランクに設定
    if (SetPriorityClass(myPrcsHndl, REALTIME_PRIORITY_CLASS) == 0) {
        return(GetLastError());
    }
    _RPT1(_CRT_WARN, ">>>KNL Priority For NT(after) = %d \n", GetPriorityClass(myPrcsHndl));

    // アプリケーションタスク数が最大数を超えた場合は終了
    if (VectpCTaskObj.size() >= MAX_APP_TASK) {
        return((DWORD)ERROR_BAD_ENVIRONMENT);
    }

    // アプリケーションスレッド生成処理
    for (unsigned int i = 0; i < VectpCTaskObj.size(); i++) {
        CTaskObj* pobj = (CTaskObj*)VectpCTaskObj[i];

        // タスク生成(スレッド生成)
        // 他プロセスとの共有なし,スタック初期サイズ デフォルト,スレッド実行関数 引数で渡すオブジェクトで対象切り替え,スレッド関数の引数(対象のオブジェクトのポインタ),即実行Createflags,スレッドID取り込み
        pobj->ThreadInfo.hndl = (HANDLE)_beginthreadex((void*)NULL,                         //_Security
                                                       0,                                   //_StackSize
                                                       ThreadGateFunc,                      //_StartAddress
                                                       VectpCTaskObj[i],                    //_ArgList
                                                       (unsigned)0,                         //_InitFlag
                                                       (unsigned*)&(pobj->ThreadInfo.ID));  //_ThrdAddr

        // タスクプライオリティ設定
        if (pobj->ThreadInfo.hndl != 0) {
            HANDLE hTh = pobj->ThreadInfo.hndl;
            if (SetThreadPriority(hTh, pobj->ThreadInfo.priority) == 0) {
                return(GetLastError());
            }
        }
        else {
            return(GetLastError());
        }
        _RPT2(_CRT_WARN, ">>>Task[%d]_priority = %d\n", i, GetThreadPriority(pobj->ThreadInfo.hndl));

        pobj->ThreadInfo.act_count       = 0;   // 基本Tickのカウンタ変数クリア
        pobj->ThreadInfo.time_over_count = 0;   // 予定周期オーバーカウントクリア
    }

    return 1;
}

/// @brief マルチメディアタイマーイベント処理関数
/// @param
/// @return 
/// @note
VOID CALLBACK AlarmHandlar(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    LONG64 tmttl;

    KNLManageSet.sys_counter++;

    // スレッド再開イベントセット処理
    for (unsigned int i = 0; i < KNLManageSet.num_of_task; i++) {
        CTaskObj* pobj = (CTaskObj*)VectpCTaskObj[i];
        pobj->ThreadInfo.act_count++;
        if (pobj->ThreadInfo.act_count >= pobj->ThreadInfo.cycle_count) {
            PulseEvent(VectHevent[i]);
            pobj->ThreadInfo.act_count = 0;
            pobj->ThreadInfo.total_act++;
        }
    }

    // Statusバーに経過時間表示
    // 1sec毎
    if ((KNLManageSet.sys_counter % 40) == 0) {
        // 起動後経過時間計算
        tmttl = (long long)KNLManageSet.sys_counter * KNLManageSet.cycle_base;  // アプリケーション起動後の経過時間 msec
        KNLManageSet.Knl_Time.wMilliseconds = (WORD)(tmttl % 1000); tmttl /= 1000;
        KNLManageSet.Knl_Time.wSecond       = (WORD)(tmttl % 60);   tmttl /= 60;
        KNLManageSet.Knl_Time.wMinute       = (WORD)(tmttl % 60);   tmttl /= 60;
        KNLManageSet.Knl_Time.wHour         = (WORD)(tmttl % 60);   tmttl /= 24;
        KNLManageSet.Knl_Time.wDay          = (WORD)(tmttl % 24);

        WCHAR tbuf[32];
        swprintf_s(tbuf, (unsigned int)(sizeof(tbuf) / sizeof(*tbuf)),
                   L"%3d %02d:%02d:%02d",
                   KNLManageSet.Knl_Time.wDay,
                   KNLManageSet.Knl_Time.wHour,
                   KNLManageSet.Knl_Time.wMinute,
                   KNLManageSet.Knl_Time.wSecond);
        SendMessage(hWnd_status_bar, SB_SETTEXT, 5, (LPARAM)tbuf);
    }
}

/// @brief ステータスバー作成
/// @param
/// @return 
/// @note
HWND CreateStatusbarMain(HWND hWnd)
{
    HWND hSBWnd;
    int  sb_size[] = {100, 200, 300, 400, 525, 615};    // ステータス区切り位置

    InitCommonControls();
    hSBWnd = CreateWindowEx(0,                          // 拡張スタイル
                            STATUSCLASSNAME,            // ウィンドウクラス
                            NULL,                       // タイトル
                            WS_CHILD | SBS_SIZEGRIP,    // ウィンドウスタイル
                            0, 0,                       // 位置
                            0, 0,                       // 幅、高さ
                            hWnd,                       // 親ウィンドウ
                            (HMENU)ID_STATUS,           // ウィンドウのID
                            hInst,                      // インスタンスハンドル
                            NULL);
    SendMessage(hSBWnd, SB_SETPARTS, (WPARAM)6, (LPARAM)(LPINT)sb_size);    // 6枠で各枠の仕切り位置をパラメータ指定
    ShowWindow(hSBWnd, SW_SHOW);

    return hSBWnd;
}

/// @brief タブ付タスクウィンドウ作成
/// @param
/// @return 
/// @note
HWND CreateTaskSettingWnd(HWND hWnd)
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
                               (HMENU)ID_TASK_SET_TAB,
                               hInst,
                               NULL);
    // Task Setting用タブ作成
    for (unsigned i = 0; i < VectpCTaskObj.size(); i++) {
        CTaskObj* pTaskObj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[i]);

        tc[i].mask    = (TCIF_TEXT | TCIF_IMAGE);
        tc[i].pszText = const_cast<LPWSTR>(pTaskObj->ThreadInfo.sname.c_str());
        tc[i].iImage  = pTaskObj->ThreadInfo.index;
        SendMessage(hTab, TCM_INSERTITEM, (WPARAM)0, (LPARAM)&tc[i]);
        pTaskObj->ThreadInfo.hWnd_opepane = CreateDialog(hInst, L"IDD_DIALOG_TASKSET1", hWnd, (DLGPROC)TaskTabDlgProc);
        pTaskObj->set_window();
        MoveWindow(pTaskObj->ThreadInfo.hWnd_opepane, TAB_POS_X, TAB_POS_Y + TAB_SIZE_H, TAB_DIALOG_W, TAB_DIALOG_H - TAB_SIZE_H, TRUE);

        // 初期値はindex 0のウィンドウを表示
        if (i == 0) {
            ShowWindow(pTaskObj->ThreadInfo.hWnd_opepane, SW_SHOW);
            SetWindowText(GetDlgItem(pTaskObj->ThreadInfo.hWnd_opepane, IDC_STATIC_TASKNAME), pTaskObj->ThreadInfo.name.c_str());   // タスク名をスタティックテキストに表示
        }
        else {
            ShowWindow(pTaskObj->ThreadInfo.hWnd_opepane, SW_HIDE);
        }
    }

    // タブコントロールにイメージリストセット
    SendMessage(hTab, TCM_SETIMAGELIST, (WPARAM)0, (LPARAM)hImgListTaskIcon);

    return hTab;
}

static LVCOLUMNA lvcol;

/// @brief 
/// @param
/// @return 
/// @note
LRESULT CALLBACK TaskTabDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        {
            InitCommonControls();

            // メッセージ用リスト
            LVCOLUMN lvcol;
            LPTSTR   strItem0[] = {(LPTSTR)(L"time"), (LPTSTR)(L"message")};    // 列ラベル
            int      CX[] = {60, 600};                                          // 列幅

            // リストコントロール設定
            HWND hList = GetDlgItem(hDlg, IDC_LIST);
            lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvcol.fmt  = LVCFMT_LEFT;
            for (int i = 0; i < 2; i++) {
                lvcol.cx       = CX[i];         // 表示位置
                lvcol.pszText  = strItem0[i];   // 見出し
                lvcol.iSubItem = i;             // サブアイテムの番号
                ListView_InsertColumn(hList, i, &lvcol);
            }
            // リスト行追加
            LVITEM item;
            item.mask = LVIF_TEXT;
            for (int i = 0; i < MSG_LIST_MAX; i++) {
                item.pszText  = (LPTSTR)L".";   // テキスト
                item.iItem    = i;              // 番号
                item.iSubItem = 0;              // サブアイテムの番号
                ListView_InsertItem(hList, &item);
            }
            return TRUE;
        }
        break;
    case WM_COMMAND:
        {
            CTaskObj* pObj = reinterpret_cast<CTaskObj*>(VectpCTaskObj[VectpCTaskObj.size() - TabCtrl_GetCurSel(hTabWnd) - 1]);
            pObj->cb_panel_proc(hDlg, msg, wp, lp);
        }
        break;
    default:
        break;
    }
    return FALSE;
}
