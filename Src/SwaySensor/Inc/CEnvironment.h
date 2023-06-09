#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "CTeliCamLib.h"
#include "CCANopenLib.h"

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
// 画像データ状態
enum class ENUM_IMAGE_STATUS : uint32_t {
    DEFAULT = 0x00000000,   // デフォルト
    ENABLED = 0x00000001    // 画像有効
} ;
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
#pragma pack(push, 1)
 // 元画像データ
typedef struct TAG_IMAGE_DATA {
	uint32_t status;        // 画像ステータス
	uint32_t width;         // 画像サイズ(水平画素) [pixel]
	uint32_t height;        // 画像サイズ(垂直画素) [pixel]
    double   fps;           // 画像フレームレート [fps]
	uint8_t* data_bgr;      // 画像データバッファのポインタ(BGR 24bit)
    cv::Mat  data_opencv;   // 画像データ(OpenCV変換画像)

    TAG_IMAGE_DATA()
        : status(static_cast<uint32_t>(ENUM_IMAGE_STATUS::DEFAULT))
	    , width(0)
	    , height(0)
        , fps(0.0)
	    , data_bgr(NULL)
    {}
} IMAGE_DATA, *PIMAGE_DATA;
#pragma pack(pop)
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CEnvironment

/// <summary>
/// Environmentタスクでは、以下の処理を担当します
/// <para>カメラの再起動処理</para>
/// <para>カメラの露光時間の設定(APIへの設定はスレッドで実行される)</para>
/// <para>傾斜計プリセット処理</para>
/// <para>傾斜計データの取得</para>
/// </summary>

class CEnvironment : public CTaskObj
{
public:
    CEnvironment();
    ~CEnvironment();

    // メンバー変数

    // メンバー関数
    IMAGE_DATA get_opencv_image(void);  // 画像データ取得

protected:

private:
    // メンバー変数
    CShared*     m_shared;      // CSharedクラス
    CTeliCamLib* m_telicam_lib; // CTeliCamLibクラス
    CCANopenLib* m_canopen_lib; // CCANopenLibクラス

    HANDLE m_td_camera_start_hndl;          // スレッドハンドル
    BOOL   m_td_camera_start_stat;          // スレッドステータス (FALSE:Exit TRUE:Run)
    HANDLE m_td_camera_restart_hndl;        // スレッドハンドル
    BOOL   m_td_camera_restart_stat;        // スレッドステータス (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_start_hndl;         // スレッドハンドル
    BOOL   m_td_canopen_start_stat;         // スレッドステータス (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_tilt_preset_hndl;   // スレッドハンドル
    BOOL   m_td_canopen_tilt_preset_stat;   // スレッドステータス (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_tilt_get_hndl;      // スレッドハンドル
    BOOL   m_td_canopen_tilt_get_stat;      // スレッドステータス (FALSE:Exit TRUE:Run)

    CONFIG_COMMON    m_cnfgcmn;         // 共通設定
    CONFIG_CAMERA    m_cnfgcam;         // カメラ設定
    CONFIG_TILTMETER m_cnfgtlt;         // 傾斜計設定
    INFO_TILTMT_DATA m_infotlt_data;    // 傾斜計情報データ
    INFO_IMGPRC_DATA m_infoprc_data;    // 画像処理情報データ
    INFO_SYSTEM_DATA m_infosys_data;    // システム情報データ

    IMAGE_DATA m_img_src;   // 元画像データ

    LARGE_INTEGER m_cycle_time_counter; // パフォーマンスカウンター現在値
    int32_t       m_cycle_time;         // 実行周期[ms]

    // メンバー関数
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // 初期化処理
    void close(void);           // 終了処理
    void proc_main(void);       // メイン処理

    HRESULT set_telicam_property(void); // カメラの基本設定

    void set_tab_func_txt(void);    // タブパネルのRadio ButtonのStaticテキストを設定
    void set_tab_tip_txt(void);     // タブパネルの説明用Staticテキストを設定
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // タブパネルの設定

    static unsigned WINAPI td_camera_start(LPVOID lpParam);         // カメラ初回起動処理
    static unsigned WINAPI td_camera_restart(LPVOID lpParam);       // カメラ再起動処理
    static unsigned WINAPI td_canopen_start(LPVOID lpParam);        // CANopen起動処理
    static unsigned WINAPI td_canopen_tilt_preset(LPVOID lpParam);  // 傾斜計プリセット処理
    static unsigned WINAPI td_canopen_tilt_get(LPVOID lpParam);     // 傾斜計データ取得処理
};
