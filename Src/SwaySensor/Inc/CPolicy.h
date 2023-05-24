#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "CEnvironment.h"
#include "CAgent.h"

extern std::vector<void*> VectpCTaskObj;    // TaskObjのポインタ
extern TASK_INDEX         g_task_index;     // TaskObjのインデックス

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
#define MOVE_AVERAGE_COUNT      1   // 移動平均数
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
// 振れ中心計測データ
typedef struct TAG_SWAY_ZERO_DATA {
    LARGE_INTEGER time_counter;         // 計測時間用パフォーマンスカウンター
    double        sway_min[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // 振れ角最小値
    double        sway_max[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // 振れ角最大値
    double        sway_zero[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];  // 振れゼロ点
} SWAY_ZERO_DATA, *PSWAY_ZERO_DATA;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CPolicy

class CPolicy : public CTaskObj
{
public:
    CPolicy();
    ~CPolicy();

    // メンバー変数

    // メンバー関数

protected:

private:
    // メンバー変数
    CShared*      m_shared;         // CSharedクラス
    CEnvironment* m_environment;    // CEnvironmentクラス

    static CONFIG_COMMON    m_cnfgcmn;      // 共通設定
    static CONFIG_CAMERA    m_cnfgcam;      // カメラ設定
    static CONFIG_MOUNTING  m_cnfgmnt;      // 取付寸法設定
    static CONFIG_IMGPROC   m_cnfgprc;      // 画像処理条件設定
    static INFO_TILTMT_DATA m_infotlt_data; // 傾斜計情報データ
    static INFO_CLIENT_DATA m_infoclt_data; // クライアント情報データ
    static INFO_ADJUST_DATA m_infoajs_data; // 調整情報データ
    static INFO_IMGPRC_DATA m_infoprc_data; // 画像処理情報データ

    MOVE_AVERAGE_DATA m_move_avrg_data; // 輝度移動平均データ
    SWAY_ZERO_DATA    m_sway_zero_data; // 振れ中心計測データ

    LARGE_INTEGER m_cycle_time_counter; // パフォーマンスカウンター現在値
    int32_t       m_cycle_time;         // 実行周期[ms]

    // メンバー関数
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // 初期化処理
    void close(void);           // 終了処理
    void proc_main(void);       // メイン処理

    BOOL proc_center_gravity(std::vector<std::vector<cv::Point>> contours,
                             double* outPosX,
                             double* outPosY,
                             int* outTgtSize,
                             uint32_t sel); // 重心検出
    void proc_sway(void);                   // 振れ検出処理

    double get_sway_zero(uint32_t idx); // 振れゼロ点設定処理
    void set_expstime(void);            // シャッタコントロール

    void set_tab_func_txt(void);    // タブパネルのRadio ButtonのStaticテキストを設定
    void set_tab_tip_txt(void);     // タブパネルの説明用Staticテキストを設定
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // タブパネルの設定
};
