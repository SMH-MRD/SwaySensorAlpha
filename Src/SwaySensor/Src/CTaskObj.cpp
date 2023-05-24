#include "CTaskObj.h"
#include "Resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////////////
// CTaskObj

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CTaskObj::CTaskObj()
{
    thread_info.name  = L"";
    thread_info.sname = L"";
    thread_info.hbmp  = NULL;
    //
    thread_info.index = 0;
    thread_info.id    = 0;
    thread_info.hndl  = NULL;
    for (uint32_t idx = 0; idx < TASK_EVENT_MAX; idx++) {
        thread_info.hevents[idx] = NULL;
    }
    thread_info.n_active_events = 1;
    thread_info.event_triggered = 0;
    thread_info.cycle_ms        = DEFAUT_TASK_CYCLE;
    thread_info.cycle_count     = 0;
    thread_info.trigger_type    = 0;
    thread_info.priority        = THREAD_PRIORITY_NORMAL;
    thread_info.thread_com      = REPEAT_INFINIT;
    //
    thread_info.start_time          = 0;
    thread_info.act_time            = 0;
    thread_info.period              = 0;
    thread_info.act_count           = 0;
    thread_info.total_act           = 0;
    thread_info.time_over_count     = 0;
    thread_info.hwnd_parent         = NULL;
    thread_info.hwnd_msgstatics     = NULL;
    thread_info.hwnd_panel          = NULL;
    thread_info.hwnd_msglist        = NULL;
    thread_info.hwnd_work           = NULL;
    thread_info.hinstance           = NULL;
    thread_info.panel_msglist_count = 0;
    thread_info.panel_func_id       = 0;
    thread_info.panel_type_id       = 0;
    thread_info.psys_counter        = NULL;
    thread_info.work_select         = THREAD_WORK_IDLE;

    InitializeCriticalSection(&cs_listview);
}

/// @brief
/// @param
/// @return
/// @note
CTaskObj::~CTaskObj()
{
    if (thread_info.hbmp != NULL) {
        delete thread_info.hbmp;
    }
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::init_task(void* pobj)
{
    return ;
}

/// @brief スレッドメイン関数
/// @param
/// @return 
/// @note
unsigned CTaskObj::run(void* param)
{
    while (this->thread_info.thread_com != TERMINATE_THREAD) {
        thread_info.event_triggered = WaitForMultipleObjects(thread_info.n_active_events,
                                                             (reinterpret_cast<CTaskObj*>(param))->thread_info.hevents,
                                                             FALSE,
                                                             INFINITE); // メインスレッドからのSIGNAL状態待ち

        // 処理周期確認用
        DWORD start_time = timeGetTime();
        thread_info.period     = start_time - thread_info.start_time;
        thread_info.start_time = start_time;

        switch (thread_info.work_select) {
        case THREAD_WORK_ROUTINE:
            routine_work(param);
            break;
        default:
            default_work(param);
            break;
        }

        thread_info.act_time = timeGetTime() - start_time;      // 実処理時間測定用
        if (this->thread_info.thread_com != REPEAT_INFINIT) {   // 逐次処理の場合は抜ける
            break;
        }
    }

    return 0;
}

/// @brief タブパネルのRadio ButtonのStaticテキストを設定
/// @param
/// @return 
/// @note 
void CTaskObj::set_tab_func_txt(void)
{
    return ;
}

/// @brief タブパネルの説明用Staticテキストを設定
/// @param
/// @return 
/// @note 
void CTaskObj::set_tab_tip_txt()
{
    switch (thread_info.panel_func_id) {
    case IDC_RADIO_TASK_FUNC1:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC2:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC3:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC4:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC5:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    case IDC_RADIO_TASK_FUNC6:
        switch (thread_info.panel_type_id) {
        case IDC_BUTTON_TASK_ITEM1:
            break;
        case IDC_BUTTON_TASK_ITEM2:
            break;
        case IDC_BUTTON_TASK_ITEM3:
            break;
        case IDC_BUTTON_TASK_ITEM4:
            break;
        case IDC_BUTTON_TASK_ITEM5:
            break;
        case IDC_BUTTON_TASK_ITEM6:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return ;
}

/// @brief タブパネルの設定
/// @param
/// @return 
/// @note 
void CTaskObj::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDC_RADIO_TASK_FUNC1:
        case IDC_RADIO_TASK_FUNC2:
        case IDC_RADIO_TASK_FUNC3:
        case IDC_RADIO_TASK_FUNC4:
        case IDC_RADIO_TASK_FUNC5:
        case IDC_RADIO_TASK_FUNC6:
            thread_info.panel_func_id = LOWORD(wp);
            set_tab_tip_txt();                                  // タブパネルの説明用Staticテキストを設定
            break;
        case IDC_BUTTON_TASK_ITEM1:
        case IDC_BUTTON_TASK_ITEM2:
        case IDC_BUTTON_TASK_ITEM3:
        case IDC_BUTTON_TASK_ITEM4:
        case IDC_BUTTON_TASK_ITEM5:
        case IDC_BUTTON_TASK_ITEM6:
            thread_info.panel_type_id = LOWORD(wp);
            break;
        default:
            break;
        }
    }

    return ;
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::routine_work(void* param)
{
    return ;
}

/// @brief
/// @param
/// @return
/// @note
void CTaskObj::default_work(void* param)
{
    return ;
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief メインウィンドウへのショートメッセージ表示(string)
/// @param
/// @return 
/// @note 
void CTaskObj::outp_msg_to_mainwnd(const std::string& src)
{
    std::wstring wstr;

    wstr = CHelper::conv_string(src);
    if (thread_info.hwnd_msgstatics != NULL) {
        SetWindowText(thread_info.hwnd_msgstatics, wstr.c_str());
    }

    return ;
}

/// @brief メインウィンドウへのショートメッセージ表示(wstring)
/// @param
/// @return 
/// @note 
void CTaskObj::outp_msg_to_mainwnd(const std::wstring& src)
{
    if (thread_info.hwnd_msgstatics != NULL) {
        SetWindowText(thread_info.hwnd_msgstatics, src.c_str());
    }

    return ;
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::outp_msg_to_listview(std::string str)
{
    std::wstring wstr;
    
    wstr = CHelper::conv_string(str);
    outp_msg_to_listview(wstr);

    return ;
}

/// @brief 
/// @param
/// @return 
/// @note
void CTaskObj::outp_msg_to_listview(std::wstring str)
{
    EnterCriticalSection(&cs_listview);

    LVITEM       lvitem;
    SYSTEMTIME   systime;
    std::wstring systime_str;
    int32_t      sub_item = 0;

    ::GetLocalTime(&systime);
    systime_str = std::format(L"{:02d}:{:02d}:{:02d}.{:03d}", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);

    thread_info.hwnd_msglist = GetDlgItem(thread_info.hwnd_panel, IDC_LIST);

    if (thread_info.panel_msglist_count >= LISTVIEW_ROW_MAX) {
        ListView_DeleteItem(thread_info.hwnd_msglist, 0);
        thread_info.panel_msglist_count--;
    }

    lvitem.mask     = LVIF_TEXT;
    lvitem.pszText  = const_cast<LPWSTR>(systime_str.c_str());              // テキスト
    lvitem.iItem    = thread_info.panel_msglist_count % LISTVIEW_ROW_MAX;   // 番号
    lvitem.iSubItem = sub_item;                                             // サブアイテムの番号
    ListView_InsertItem(thread_info.hwnd_msglist, &lvitem);

    sub_item++;
    lvitem.pszText  = const_cast<LPWSTR>(str.c_str());                    // テキスト
    lvitem.iItem    = thread_info.panel_msglist_count % LISTVIEW_ROW_MAX; // 番号
    lvitem.iSubItem = sub_item;                                           // サブアイテムの番号
    ListView_SetItem(thread_info.hwnd_msglist, &lvitem);

    thread_info.panel_msglist_count++;

    LeaveCriticalSection(&cs_listview);

    return ;
}
