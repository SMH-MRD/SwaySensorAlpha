#include "CTaskObj.h"
#include <windowsx.h>
#include <commctrl.h>
#include <mmsystem.h>

/// @brief
/// @param
/// @return
/// @note
CTaskObj::CTaskObj()
{
//@@@    swprintf_s(ThreadInfo.name, sizeof(ThreadInfo.name) / sizeof(*ThreadInfo.name), L"%s", L"?");
    ThreadInfo.name  = L"";
    ThreadInfo.sname = L"";
    ThreadInfo.hBmp  = NULL;
    //
    ThreadInfo.index = 0;
    ThreadInfo.ID    = 0;
    ThreadInfo.hndl  = NULL;
    for (int idx = 0; idx < TASK_EVENT_MAX; idx++) {
        ThreadInfo.hevents[idx] = NULL;
    }
    ThreadInfo.n_active_events = 1;
    ThreadInfo.event_triggered = 0;
    ThreadInfo.cycle_ms        = DEFAUT_TASK_CYCLE;
    ThreadInfo.cycle_count     = 0;
    ThreadInfo.trigger_type    = 0;
    ThreadInfo.priority        = THREAD_PRIORITY_NORMAL;
    ThreadInfo.thread_com      = REPEAT_INFINIT;
    //
    ThreadInfo.start_time      = 0;
    ThreadInfo.act_time        = 0;
    ThreadInfo.period          = 0;
    ThreadInfo.act_count       = 0;
    ThreadInfo.total_act       = 0;
    ThreadInfo.time_over_count = 0;
    ThreadInfo.hWnd_parent     = NULL;
    ThreadInfo.hWnd_msgStatics = NULL;
    ThreadInfo.hWnd_opepane    = NULL;
    ThreadInfo.hWnd_msgList    = NULL;
    ThreadInfo.hWnd_work       = NULL;
    ThreadInfo.hInstance       = NULL;
    ThreadInfo.cnt_PNLlist_msg = 0;
    ThreadInfo.panel_func_id   = 0;
    ThreadInfo.panel_type_id   = 0;
    ThreadInfo.psys_counter    = NULL;
    ThreadInfo.work_select     = THREAD_WORK_IDLE;
}

/// @brief
/// @param
/// @return
/// @note
CTaskObj::~CTaskObj()
{
    if (ThreadInfo.hBmp != NULL) {
        delete ThreadInfo.hBmp;
    }
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::init_task(void* pobj)
{
//@@@    set_panel_tip_txt();
    return;
}

/// @brief スレッドメイン関数
/// @param
/// @return 
/// @note
unsigned CTaskObj::run(void* param)
{
    while (this->ThreadInfo.thread_com != TERMINATE_THREAD) {
        ThreadInfo.event_triggered = WaitForMultipleObjects(ThreadInfo.n_active_events, ((CTaskObj*)param)->ThreadInfo.hevents, FALSE, INFINITE);   // メインスレッドからのSIGNAL状態待ち

        // 処理周期確認用
        DWORD start_time = timeGetTime();
        ThreadInfo.period     = start_time - ThreadInfo.start_time;
        ThreadInfo.start_time = start_time;

        switch (ThreadInfo.work_select) {
        case THREAD_WORK_ROUTINE:
            routine_work(param);
            break;
        case THREAD_WORK_OPTION1:
            optional_work1(param);
            break;
        case THREAD_WORK_OPTION2:
            optional_work2(param);
            break;
        default:
            default_work(param);
            break;
        }

        ThreadInfo.act_time = timeGetTime() - start_time;       // 実処理時間測定用
        if (this->ThreadInfo.thread_com != REPEAT_INFINIT) {    // 逐次処理の場合は抜ける
            break;
        }
    }
    return 0;
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::routine_work(void* param)
{
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::optional_work1(void* param)
{
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::optional_work2(void* param)
{
}

/// @brief
/// @param
/// @return
/// @note
void CTaskObj::default_work(void* param)
{
}

/// @brief
/// @param
/// @return
/// @note
unsigned int CTaskObj::set_work(int work_id)
{
    ThreadInfo.work_select = work_id;

    return ThreadInfo.work_select;
}

/// @brief
/// @param
/// @return
/// @note
void CTaskObj::set_window(void)
{
    return;
}

/// @brief 
/// @param
/// @return 
/// @note 
LRESULT CALLBACK CTaskObj::cb_panel_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    return 0;
}
