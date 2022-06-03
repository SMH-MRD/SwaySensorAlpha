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
    ThreadInfo.hndl            = NULL;
    ThreadInfo.cycle_ms        = DEFAUT_TASK_CYCLE;
    ThreadInfo.thread_com      = REPEAT_INFINIT;
    wsprintf(ThreadInfo.name, L"?");
    ThreadInfo.act_count       = 0;
    ThreadInfo.act_time        = 0;
    ThreadInfo.priority        = THREAD_PRIORITY_NORMAL;
    ThreadInfo.work_select     = THREAD_WORK_IDLE;
    ThreadInfo.hWnd_parent     = NULL;
    ThreadInfo.hWnd_work       = NULL;
    ThreadInfo.hWnd_opepane    = NULL;
    ThreadInfo.hWnd_msgStatics = NULL;
    ThreadInfo.hBmp            = NULL;
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
void CTaskObj::InitTask(void* pobj)
{
//@@@    set_panel_tip_txt();
    return;
}

/// @brief スレッドメイン関数
/// @param
/// @return 
/// @note
unsigned CTaskObj::Run(void* param)
{
    while (this->ThreadInfo.thread_com != TERMINATE_THREAD) {
        ThreadInfo.event_triggered = WaitForMultipleObjects(ThreadInfo.n_active_events, ((CTaskObj*)param)->ThreadInfo.hevents, FALSE, INFINITE);   // メインスレッドからのSIGNAL状態待ち

        // 処理周期確認用
        DWORD start_time = timeGetTime();
        ThreadInfo.period     = start_time - ThreadInfo.start_time;
        ThreadInfo.start_time = start_time;

        switch (ThreadInfo.work_select) {
        case THREAD_WORK_ROUTINE:
            RoutineWork(param);
            break;
        case THREAD_WORK_OPTION1:
            OptionalWork1(param);
            break;
        case THREAD_WORK_OPTION2:
            OptionalWork2(param);
            break;
        default:
            DefaultWork(param);
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
void CTaskObj::RoutineWork(void* param)
{
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::OptionalWork1(void* param)
{
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::OptionalWork2(void* param)
{
}

/// @brief
/// @param
/// @return
/// @note
void CTaskObj::DefaultWork(void* param)
{
}

/// @brief
/// @param
/// @return
/// @note
unsigned int CTaskObj::SetWork(int work_id)
{
    ThreadInfo.work_select = work_id;

    return ThreadInfo.work_select;
}

/// @brief
/// @param
/// @return
/// @note
void CTaskObj::SetPanelPbTxt(void)
{
    return;
}

/// @brief 
/// @param
/// @return 
/// @note 
LRESULT CALLBACK CTaskObj::PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    return 0;
}
