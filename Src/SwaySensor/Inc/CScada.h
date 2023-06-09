#pragma once

#include "CTaskObj.h"
#include "CShared.h"

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
#define DISP_IMG_SCROLL_SIZE    20      // 表示画像スクロールバーサイズ

#define SCALAR_WHITE            cv::Scalar(255, 255, 255)
#define SCALAR_BLUE             cv::Scalar(255, 0, 0)
#define SCALAR_GREEN            cv::Scalar(0, 255, 0)
#define SCALAR_RED              cv::Scalar(0, 0, 255)
#define SCALAR_YELLOW           cv::Scalar(0, 255, 255)
#define SCALAR_MAGENTA          cv::Scalar(255, 0, 255)
#define SCALAR_LT_BLUE          cv::Scalar(255, 255, 0)

#define LINE_TKN                1   // ライン表示太さ
#define CURSOR_LEN              10  // カーソル表示長さ
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CScada

/// <summary>
/// Scadaタスクでは、以下の処理を担当します
/// <para>ユーザインタフェイス処理（モニタリングウィンドウ処理：表示,パラメータ変更）</para>
/// </summary>

class CScada : public CTaskObj
{
public:
    CScada();
    ~CScada();

    // メンバー変数

    // メンバー関数

protected:

private:
    // メンバー変数
    CShared* m_shared;  // CSharedクラス

    static CONFIG_COMMON    m_cnfgcmn;      // 共通設定
    static CONFIG_CAMERA    m_cnfgcam;      // カメラ設定
    static CONFIG_IMGPROC   m_cnfgprc;      // 画像処理条件設定
    static INFO_TILTMT_DATA m_infotlt_data; // 傾斜計情報データ
    static INFO_CLIENT_DATA m_infoclt_data; // クライアント情報データ
    static INFO_ADJUST_DATA m_infoajs_data; // 調整情報データ
    static INFO_IMGPRC_DATA m_infoprc_data; // 画像処理情報データ
    static INFO_SYSTEM_DATA m_infosys_data; // システム情報データ

    static cv::Mat m_img_src;   // 元画像

    static HWND       m_cam_dlg_hndl;
    static uint32_t   m_sel_img;
    static BOOL       m_cursor;
    static POINT      m_cursor_pt;
    static BOOL       m_scale_img_src;
    static SCROLLINFO m_scrlinf_img_src_h;
    static SCROLLINFO m_scrlinf_img_src_v;
    static BOOL       m_sway_pos;
    static BOOL       m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
    static BOOL       m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
    static BOOL       m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];

    // メンバー関数
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // 初期化処理
    void close(void);           // 終了処理
    void proc_main(void);       // メイン処理

    HWND create_dlg_wnd();  // ダイアログのオープン

    void set_tab_func_txt(void);    // タブパネルのRadio ButtonのStaticテキストを設定
    void set_tab_tip_txt(void);     // タブパネルの説明用Staticテキストを設定
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // タブパネルの設定

    static LRESULT CALLBACK cb_dlg_wnd(HWND hwnd,
                                       uint32_t msg,
                                       WPARAM wp,
                                       LPARAM lp);  // ダイアログ画面用コールバック関数
};
