#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "SockUDP.h"

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
#define NW_SEND_IP_AUTO                 L"0.0.0.0"  // 送信先をSocket情報から決定する場合のIP設定値

#define NW_SIZE_DEVICE_ID               4           // 機器IDサイズ[byte]
#define NW_SIZE_STRING_INFO             32          // 文字情報サイズ[byte]
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
// 通信データ(コマンド)
enum class ENUM_NW_COMMAND : uint32_t {
    REQ_DATA_CYCLE    = 0x00000001, // データ送信要求(定周期)
    REQ_DATA_STOP     = 0x00000008, // データ送信停止要求
    SET_ZERO_CAMERA_1 = 0x00000010, // 0点設定(カメラ1)
    SET_ZERO_CAMERA_2 = 0x00000020, // 0点設定(カメラ2)
    SET_ZERO_TILT_1   = 0x00000040, // 0点設定(傾斜計1)
    SET_ZERO_TILT_2   = 0x00000080, // 0点設定(傾斜計2)
    RESET_CAMERA_1    = 0x00000100, // リセット要求(カメラ1)
    RESET_CAMERA_2    = 0x00000200, // リセット要求(カメラ2)
    RESET_TILT_1      = 0x00000400, // リセット要求(傾斜計1)
    RESET_TILT_2      = 0x00000800, // リセット要求(傾斜計2)
    REQ_SAVE_IMAGE    = 0x00001000, // 画像保存要求
    REQ_PC_REBOOT     = 0x80000000  // PC再起動要求
} ;

// 通信データ(エラー状態)
enum class ENUM_NW_ERR_SENSOR : uint32_t {
    CAMERA   = 0x00000001,  // 機器異常(カメラ)
    TILT     = 0x00000002,  // 機器異常(傾斜計)
    TARGET_1 = 0x00000010,  // 検出異常(ターゲット1)
    TARGET_2 = 0x00000020   // 検出異常(ターゲット2)
} ;

// 通信データ(検出状態)
enum class ENUM_NW_STAT_SENSOR : uint32_t {
    TARGET_1        = 0x00000001,   // 振れ検出中(ターゲット1)
    TARGET_2        = 0x00000002,   // 振れ検出中(ターゲット2)
    BOOT_CAMERA     = 0x00000010,   // 準備中(カメラ)
    BOOT_TILT       = 0x00000020,   // 準備中(傾斜計)
    SET_ZERO_CAMERA = 0x00000040,   // 0点設定中(カメラ)
    SET_ZERO_TILT   = 0x00000080    // 0点設定中(傾斜計)
};
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
//----------------------------------------------------------------------------
// クライアント通信情報の構造体
//----------------------------------------------------------------------------
// クライアント入出力
#pragma pack(push, 1)   // Set 1 byte boundary
// ***受信データ
// コマンド情報
typedef struct TAG_NW_RECV_DATA_COMMAND_INFO {
    uint32_t command;   // コマンド情報
    uint32_t mode;      // モード設定
    uint32_t cycle;     // 最小受信周期[ms]
    uint32_t tgt_len[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)];   // ターゲット距離[mm]
} NW_RECV_DATA_COMMAND_INFO, *PNW_RECV_DATA_COMMAND_INFO;
// 受信データ
typedef struct TAG_NW_RECV_DATA {
    uint8_t                   device_id[NW_SIZE_DEVICE_ID]; // 機器ID
    SOCKADDR_IN               sock;                         // sockaddrs_in構造体
    NW_RECV_DATA_COMMAND_INFO command_info;                 // コマンド情報
} NW_RECV_DATA, *PNW_RECV_DATA;

// ***送信データ
// *機器個体情報
// カメラ情報
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO {
    uint32_t image_size[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // 画像データ画素数[pixel]
    uint32_t view_angle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // 画角[pixel/rad]
} NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO, *PNW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO;
// カメラ取付情報
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO {
    uint32_t l0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // カメラ回転軸距離L0[mm]
    uint32_t t0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // カメラ取付角度θ0(*10^6)[rad]
    uint32_t p0[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // カメラ設置角度φ0(*10^6)[rad]
} NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO, *PNW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO;
// 機器個体情報
typedef struct TAG_NW_SEND_DATA_SENSOR_SPEC_INFO {
    NW_SEND_DATA_SENSOR_SPEC_CAMERA_INFO          camera;   // カメラ情報
    NW_SEND_DATA_SENSOR_SPEC_MOUNTING_OFFSET_INFO mounting; // 取付情報
} NW_SEND_DATA_SENSOR_SPEC_INFO, *PNW_SEND_DATA_SENSOR_SPEC_INFO;
// *機器状態情報
typedef struct TAG_NW_SEND_DATA_SENSOR_STAT_INFO {
    uint32_t mode;  // 検出モード
    uint32_t error; // エラー状態
    uint32_t stat;  // 検出状態
} NW_SEND_DATA_SENSOR_STAT_INFO, *PNW_SEND_DATA_SENSOR_STAT_INFO;
// *傾斜計情報
typedef struct TAG_NW_SEND_DATA_TILT_INFO {
    uint32_t angle; // 傾斜角(*10^6)[rad]
    uint32_t speed; // 傾斜角速度(*10^6)[rad/s]
} NW_SEND_DATA_TILT_INFO, *PNW_SEND_DATA_TILT_INFO;
// *振れ検出情報
// 振れ情報
typedef struct TAG_NW_SEND_DATA_SWAY_SWAY_INFO {
    uint32_t angle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // 振れ角[pixel]
    uint32_t speed[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // 振れ速度[pixel/s]
    uint32_t zero[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];     // 振れ中心位置[pixel]
} NW_SEND_DATA_SWAY_SWAY_INFO, *PNW_SEND_DATA_SWAY_SWAY_INFO;
// ターゲット情報
typedef struct TAG_NW_SEND_DATA_SWAY_TARGET_INFO {
    uint32_t tilt[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // ターゲット傾き[pixel]
    uint32_t size;                                          // ターゲットサイズ[pixel]
} NW_SEND_DATA_SWAY_TARGET_INFO, *PNW_SEND_DATA_SWAY_TARGET_INFO;
// 振れ検出情報
typedef struct TAG_NW_SEND_DATA_SWAY_INFO {
    NW_SEND_DATA_SWAY_SWAY_INFO   sway;     // 振れ情報
    NW_SEND_DATA_SWAY_TARGET_INFO target;   // ターゲット情報
} NW_SEND_DATA_SWAY_INFO, *PNW_SEND_DATA_SWAY_INFO;
// *振れセンサー情報
typedef struct TAG_NW_SEND_DATA_SWAY_SENSOR_INFO {
    NW_SEND_DATA_SENSOR_SPEC_INFO sensor_spec_info;                                     // 機器個体情報
    NW_SEND_DATA_SENSOR_STAT_INFO sensor_stat_info;                                     // 機器状態情報
    NW_SEND_DATA_TILT_INFO        tilt_info[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // 傾斜計情報
    NW_SEND_DATA_SWAY_INFO        sway_info[static_cast<uint32_t>(ENUM_TARGET::E_MAX)]; // 振れ検出情報
    uint8_t                       message[NW_SIZE_STRING_INFO];                         // 文字情報
} NW_SEND_DATA_SWAY_SENSOR_INFO, *PNW_SEND_DATA_SWAY_SENSOR_INFO;
// 送信データ
typedef struct TAG_NW_SEND_DATA {
    uint8_t                       device_id[NW_SIZE_DEVICE_ID]; // 機器ID
    SYSTEMTIME                    system_time;                  // SYSTEMTIME情報
    NW_SEND_DATA_SWAY_SENSOR_INFO sway_sensor_info[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)]; // 振れセンサー情報
} NW_SEND_DATA, *PNW_SEND_DATA;
#pragma pack(pop)
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CAgent

/// <summary>
/// Agentタスクでは、以下の処理を担当します
/// <para>センサカメラとのIF処理</para>
/// <para>傾斜計とのIF処理</para>
/// <para>制御PCとのIF処理</para>
/// </summary>

class CAgent : public CTaskObj
{
public:
    CAgent();
    ~CAgent();

    // メンバー変数

    // メンバー関数

protected:

private:
    // メンバー変数
    CShared* m_shared;  // CSharedクラス

    CONFIG_SYSTEM    m_cnfgsys;         // システム設定
    CONFIG_ETHERNET  m_cnfgnet;         // 通信設定
    CONFIG_CAMERA    m_cnfgcam;         // カメラ設定
    CONFIG_MOUNTING  m_cnfgmnt;         // 取付寸法設定
    INFO_TILTMT_DATA m_infotlt_data;    // 傾斜計情報データ
    INFO_CLIENT_DATA m_infoclt_data;    // クライアント情報データ
    INFO_IMGPRC_DATA m_infoprc_data;    // 画像処理情報データ

    HANDLE m_td_comm_send_hndl; // スレッドハンドル
    BOOL   m_td_comm_send_stat; // スレッドステータス(FALSE:Exit TRUE:Run)

    CSockUDPLib* m_sockrecv;    // Socket(受信)
    CSockUDPLib* m_socksend;    // Socket(送信)

    NW_RECV_DATA m_recv_data;   // 受信データ
    NW_SEND_DATA m_send_data;   // 送信データ

    LARGE_INTEGER m_send_cycle_counter; // パフォーマンスカウンター現在値

    // メンバー関数
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // 初期化処理
    void close(void);           // 終了処理
    void proc_main(void);       // メイン処理

    void set_tab_func_txt(void);    // タブパネルのRadio ButtonのStaticテキストを設定
    void set_tab_tip_txt(void);     // タブパネルの説明用Staticテキストを設定
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // タブパネルの設定

    static unsigned WINAPI td_comm_send(LPVOID lpParam);    // データ送信スレッド関数
};
