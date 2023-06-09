#pragma once

#include <windows.h>
#include <string>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world455d.lib")
#else
#pragma comment(lib, "opencv_world455.lib")
#endif

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
//----------------------------------------------------------------------------
// .iniファイル関連設定
#define FOLDER_OF_INIFILE                               L"Data\\"                       // .iniファイルフォルダ
#define EXT_OF_INIFILE                                  L"ini"                          // .iniファイル拡張子

#define INI_SCT_COMMON                                  L"COMMON"                       // 共通設定
#define INI_KEY_COMMON_IMAGE_SOURCE_CAMERA              L"CMN_IMAGE_SOURCE_CAMERA"      // カメラ画像取込み
#define INI_KEY_COMMON_IMAGE_SOURCE_CAMERA_ITEM_NUM     1                               // 
#define INI_KEY_COMMON_IMAGE_SOURCE_FILENAME            L"CMN_IMAGE_SOURCE_FILENAME"    // 取込み画像ファイル名
#define INI_KEY_COMMON_IMAGE_SOURCE_FILENAME_ITEM_NUM   1                               // 
#define INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME            L"CMN_IMAGE_OUTPUT_FILENAME"    // 画像保存ファイル名
#define INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME_ITEM_NUM   1                               // 
#define INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT              L"CMN_IMAGE_SCREEN_LAYOUT"      // 画像表示レイアウト
#define INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT_ITEM_NUM     4                               // 

#define INI_SCT_SYSTEM                                  L"SYSTEM"                       // システム設定
#define INI_KEY_SYSTEM_CYCLE_TIME                       L"SYS_CYCLE_TIME"               // メインスレッド実行周期
#define INI_KEY_SYSTEM_CYCLE_TIME_ITEM_NUM              1                               // 
#define INI_KEY_SYSTEM_DEVICE_ID                        L"SYS_DEVICE_ID"                // 機器ID(半角英数字4文字)
#define INI_KEY_SYSTEM_DEVICE_ID_ITEM_NUM               2                               // 

#define INI_SCT_TASK                                    L"TASK"                         // タスク設定
#define INI_KEY_TASK_ENVIRONMENT                        L"TSK_ENVIRONMENT"              // Environment
#define INI_KEY_TASK_POLICY                             L"TSK_POLICY"                   // Policy
#define INI_KEY_TASK_AGENT                              L"TSK_AGENT"                    // Agent
#define INI_KEY_TASK_SCADA                              L"TSK_SCADA"                    // Scada
#define INI_KEY_TASK_ITEM_NUM                           3                               // 

#define INI_SCT_ETHERNET                                L"ETHERNET"                     // 通信設定
#define INI_KEY_ETHERNET_LOCAL                          L"NET_LOCAL"                    // Local設定
#define INI_KEY_ETHERNET_REMOTE                         L"NET_REMOTE"                   // Remote設定
#define INI_KEY_ETHERNET_ITEM_NUM                       2                               // 

#define INI_SCT_CAMERA                                  L"CAMERA"                       // カメラ設定
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS                 L"CAM_BASIS_IP_ADDRESS"         // カメラのIPアドレス
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_ROI                        L"CAM_BASIS_ROI"                // ROI(領域)
#define INI_KEY_CAMERA_BASIS_ROI_ITEM_NUM               4                               // 
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE                L"CAM_BASIS_PACKET_SIZE"        // ドライバが受け取るパケットの最大サイズ
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE_ITEM_NUM       1                               // 
#define INI_KEY_CAMERA_BASIS_FRAME_RATE                 L"CAM_BASIS_FRAME_RATE"         // フレームレート
#define INI_KEY_CAMERA_BASIS_FRAME_RATE_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL                 L"CAM_BASIS_BLACKLEVEL"         // 黒レベル設定
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL_ITEM_NUM        1                               // 
#define INI_KEY_CAMERA_BASIS_GAMMA                      L"CAM_BASIS_GAMMA"              // ガンマ補正
#define INI_KEY_CAMERA_BASIS_GAMMA_ITEM_NUM             1                               // 
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO              L"CAM_BASIS_WBALANCE_AUTO"      // ホワイトバランスゲイン設定
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_ITEM_NUM     3                               // 
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE                 L"CAM_BASIS_VIEW_ANGLE"         // カメラ視野角
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE_ITEM_NUM        2                               // 
#define INI_KEY_CAMERA_GAIN                             L"CAM_GAIN"                     // ゲイン設定
#define INI_KEY_CAMERA_GAIN_ITEM_NUM                    3                               // 
#define INI_KEY_CAMERA_GAIN_AUTO                        L"CAM_GAIN_AUTO"                // 自動ゲイン設定
#define INI_KEY_CAMERA_GAIN_AUTO_ITEM_NUM               2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_START                  L"CAM_GAIN_AUTO_START"          // 自動ゲイン判定輝度(開始)
#define INI_KEY_CAMERA_GAIN_AUTO_START_ITEM_NUM         2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_STOP                   L"CAM_GAIN_AUTO_STOP"           // 自動ゲイン判定輝度(停止)
#define INI_KEY_CAMERA_GAIN_AUTO_STOP_ITEM_NUM          2                               // 
#define INI_KEY_CAMERA_EXPSTIME                         L"CAM_EXPSTIME"                 // 露光時間設定
#define INI_KEY_CAMERA_EXPSTIME_ITEM_NUM                3                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO                    L"CAM_EXPSTIME_AUTO"            // 自動露光設定
#define INI_KEY_CAMERA_EXPSTIME_AUTO_ITEM_NUM           2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START              L"CAM_EXPSTIME_AUTO_START"      // 自動露光判定輝度(開始)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START_ITEM_NUM     2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP               L"CAM_EXPSTIME_AUTO_STOP"       // 自動露光判定輝度(停止)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP_ITEM_NUM      2                               // 
#define INI_KEY_CAMERA_ERROR                            L"CAM_ERROR"                    // カメラの異常判定設定
#define INI_KEY_CAMERA_ERROR_ITEM_NUM                   2                               // 
#define INI_KEY_CAMERA_BOOT                             L"CAM_BOOT"                     // カメラの起動設定
#define INI_KEY_CAMERA_BOOT_ITEM_NUM                    3                               // 

#define INI_SCT_TILTMETER                               L"TILTMETER"                    // 傾斜計設定
#define INI_KEY_TILTMETER_CANOPEN                       L"TLT_CANOPEN"                  // CANope設定
#define INI_KEY_TILTMETER_CANOPEN_ITEM_NUM              3                               // 
#define INI_KEY_TILTMETER_FILTER                        L"TLT_FILTER"                   // 傾斜計フィルター
#define INI_KEY_TILTMETER_FILTER_ITEM_NUM               2                               // 
#define INI_KEY_TILTMETER_SPEED_TC                      L"TLT_SPEED_TC"                 // 傾斜角速度フィルター時定数
#define INI_KEY_TILTMETER_SPEED_TC_ITEM_NUM             1                               // 

#define INI_SCT_MOUNTING                                L"MOUNTING"                     // 取付寸法設定
#define INI_KEY_MOUNTING_OFFSET_L0                      L"MNT_OFFSET_L0"                // カメラ回転軸距離L0[mm]
#define INI_KEY_MOUNTING_OFFSET_T0                      L"MNT_OFFSET_T0"                // カメラ取付角度θ0(*10^6)[rad]
#define INI_KEY_MOUNTING_OFFSET_P0                      L"MNT_OFFSET_P0"                // カメラ設置角度φ0(*10^6)[rad]
#define INI_KEY_MOUNTING_OFFSET_ITEM_NUM                2                               // 

#define INI_SCT_IMGPROC                                 L"IMGPROC"                      // 画像処理条件設定
#define INI_KEY_IMGPROC_ROI                             L"IMG_ROI"                      // 画像ROI設定
#define INI_KEY_IMGPROC_ROI_ITEM_NUM                    2                               // 
#define INI_KEY_IMGPROC_MASK                            L"IMG_MASK"                     // マスク画像選択
#define INI_KEY_IMGPROC_MASK_ITEM_NUM                   1                               // 
#define INI_KEY_IMGPROC_MASK1_H                         L"IMG_MASK1_H"                  // 画像1マスクH
#define INI_KEY_IMGPROC_MASK1_S                         L"IMG_MASK1_S"                  // 画像1マスクS
#define INI_KEY_IMGPROC_MASK1_V                         L"IMG_MASK1_V"                  // 画像1マスクV
#define INI_KEY_IMGPROC_MASK1_ITEM_NUM                  2                               // 
#define INI_KEY_IMGPROC_MASK2_H                         L"IMG_MASK2_H"                  // 画像2マスクH
#define INI_KEY_IMGPROC_MASK2_S                         L"IMG_MASK2_S"                  // 画像2マスクS
#define INI_KEY_IMGPROC_MASK2_V                         L"IMG_MASK2_V"                  // 画像2マスクV
#define INI_KEY_IMGPROC_MASK2_ITEM_NUM                  2                               // 
#define INI_KEY_IMGPROC_FILTER1                         L"IMG_FILTER1"                  // フィルター1(ゴマ塩ノイズ)
#define INI_KEY_IMGPROC_FILTER1_ITEM_NUM                2                               // 
#define INI_KEY_IMGPROC_FILTER2                         L"IMG_FILTER2"                  // フィルター2(穴埋めノイズ)
#define INI_KEY_IMGPROC_FILTER2_ITEM_NUM                2                               // 
#define INI_KEY_IMGPROC_PROC                            L"IMG_PROC"                     // ターゲット検出アルゴリズム
#define INI_KEY_IMGPROC_PROC_ITEM_NUM                   1                               // 
#define INI_KEY_IMGPROC_SWAY_SPEED_TC                   L"IMG_SWAY_SPEED_TC"            // 振れ速度フィルター時定数
#define INI_KEY_IMGPROC_SWAY_SPEED_TC_ITEM_NUM          1                               // 
#define INI_KEY_IMGPROC_SWAY_ZERO_SET                   L"IMG_SWAY_ZERO_SET"            // 振れゼロ点設定
#define INI_KEY_IMGPROC_SWAY_ZERO_SET_ITEM_NUM          1                               // 

//----------------------------------------------------------------------------
// 
// H, S, V
#define IMAGE_HSV_H_MIN                 0           // H最小値
#define IMAGE_HSV_H_MAX                 179         // H最大値
#define IMAGE_HSV_S_MIN                 0           // S最小値
#define IMAGE_HSV_S_MAX                 255         // S最大値
#define IMAGE_HSV_V_MIN                 0           // V最小値
#define IMAGE_HSV_V_MAX                 255         // V最大値

// 調整
#define EXTN_TARGETDIST_MIN             100.0       // ターゲット距離(最小)
#define EXTN_TARGETDIST_MAX             80000.0     // ターゲット距離(最大)

// シャッタコントロールモード
#define	EXPOSURE_CONTROL_HOLD           0           // シャッタコントロール:Hold
#define	EXPOSURE_CONTROL_LIGHT          1           // シャッタコントロール:明るく
#define	EXPOSURE_CONTROL_DARKEN         -1          // シャッタコントロール:暗く

//----------------------------------------------------------------------------
// 
#define PI              3.14159     // π
#define CONV_DEG_RAD    (PI/180.0)  // deg→rad
#define CONV_MSEC_SEC   0.001       // msec→sec
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
// 傾斜計状態
enum class ENUM_TILT_STATUS : uint32_t {
    DEFAULT              = 0x00000000,  // デフォルト
    CANOPEN_STARTED      = 0x00000001,  // CANopen起動
    ERR_CANOPEN_LIB      = 0x00000010,  // CANopen Libエラー
    ERR_CANOPEN_READ     = 0x00000020   // データ読み込みエラー
} ;

// 画像処理状態
enum class ENUM_PROCCESS_STATUS : uint32_t {
    DEFAULT            = 0x00000000,    // デフォルト
    IMAGE_ENABLE       = 0x00000001,    // 画像データ有効
    TARGET_ENABLE      = 0x00000002,    // ターゲット検出有効
    ZERO_SET_RUN       = 0x00000010,    // 0点設定中(カメラ)
    ZERO_SET_MCC_RUN   = 0x00000020,    // 0点設定中(カメラ)(MCC)
    ZERO_SET_COMPLETED = 0x00000100     // 0点設定完了(カメラ)
} ;

// システム状態
enum class ENUM_SYSTEM_STATUS : uint32_t {
    DEFAULT                = 0x00000000,    // デフォルト
    CAMERA_RESET_REQ       = 0x00000010,    // カメラ再接続要求
    CAMERA_RESET_REQ_MCC   = 0x00000020,    // カメラ再接続要求(MCC)
    CAMERA_RESET_RUN       = 0x00000040,    // カメラ再接続中
    CAMERA_RESET_COMPLETED = 0x00000080,    // カメラ再接続完了
    TILT_PRESET_REQ        = 0x00000100,    // 傾斜計プリセット要求
    TILT_PRESET_REQ_MCC    = 0x00000200,    // 傾斜計プリセット要求(MCC)
    TILT_PRESET_RUN        = 0x00000400,    // 傾斜計プリセット実行中
    TILT_PRESET_COMPLETED  = 0x00000800     // 傾斜計プリセット完了
} ;

// カメラ画像取込み
enum class ENUM_GRAB_IMAGE : uint32_t {
    GRAB_FILE = 0,  // 画像取込み(ファイル)
    GRAB_CAMERA,    // 画像取込み(カメラ)
    E_MAX
} ;

// 振れセンサー
enum class ENUM_SWAY_SENSOR : uint32_t {
    SENSOR_1 = 0,   // 振れセンサー1
    SENSOR_2,       // 振れセンサー2
    E_MAX
} ;

// ターゲット検出
enum class ENUM_TARGET : uint32_t {
    TARGET_1 = 0,   // ターゲット検出1
    TARGET_2,       // ターゲット検出2
    E_MAX
} ;

// HSV色空間
enum class ENUM_HSV_MODEL : uint32_t {
    H = 0,  // 色相(Hue)
    S,      // 彩度(Saturation)
    V,      // 明度(Value)
    E_MAX
} ;

// カメラ画像配列 INDEX
enum class ENUM_IMAGE : uint32_t {
    SOURCE = 0, // 元画像
    MASK_1,     // マスク画像1
    MASK_2,     // マスク画像2
    PROCESS,    // 加工画像
    E_MAX
} ;

// 画像処理配列 INDEX
enum class ENUM_IMAGE_MASK : uint32_t {
    MASK_1 = 0, // マスク画像1
    MASK_2,     // マスク画像2
    E_MAX
} ;

// マスク画像選択
enum class ENUM_IMAGE_MASK_SEL : uint32_t {
    ALL = 0,    // マスク画像選択(両方)
    IMAGE_1,    // マスク画像選択(画像1のみ)
    IMAGE_2,    // マスク画像選択(画像2のみ)
    E_MAX
} ;

// ノイズフィルター
enum class ENUM_NOISE_FILTER : uint32_t {
    FILTER_1 = 0,   // フィルター1
    FILTER_2,       // フィルター2
    E_MAX
} ;

// ノイズフィルター1
enum class ENUM_NOISE_FILTER1 : uint32_t {
    NONE = 0,   // なし
    MEDIAN,     // 中央値フィルター
    OPENNING,   // オープニング処理
    E_MAX
} ;

// ノイズフィルター2
enum class ENUM_NOISE_FILTER2 : uint32_t {
    NONE = 0,   // なし
    CLOSING,    // クロージング処理
    E_MAX
} ;

// 重心位置算出アルゴリズム
enum class ENUM_COG : uint32_t {
    MAX_CONTOUR_AREA = 0,   // 最大輪郭面積
    MAX_CONTOUR_LENGTH,     // 最大輪郭長
    E_MAX
} ;

// 軸
enum class ENUM_AXIS : uint32_t {
    X = 0,  // X軸
    Y,      // Y軸
    E_MAX
} ;
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
#pragma region STRUCTURE_DEFINITION
//----------------------------------------------------------------------------
// アプリケーション設定の構造体
// 共通設定
// 画像表示レイアウト設定
typedef struct TAG_CONFIG_COMMON_IMAGE_SCREEN_LAYOUT {
    uint32_t x0;        // 原点座標X
    uint32_t y0;        // 原点座標Y
    uint32_t width;     // 横幅サイズ
    uint32_t height;    // 高さサイズ

    TAG_CONFIG_COMMON_IMAGE_SCREEN_LAYOUT()
        : x0(0)
        , y0(0)
        , width(0)
        , height(0)
    {}
} CONFIG_COMMON_IMAGE_SCREEN_LAYOUT, *PCONFIG_COMMON_IMAGE_SCREEN_LAYOUT;
// 共通設定
typedef struct TAG_CONFIG_COMMON {
    uint32_t     img_source_camera; // カメラ画像取込み(カメラ[1]/画像ファイル[0])
    std::wstring img_source_fname;  // 取込み画像ファイル名(CMN_IMAGE_SOURCE_CAMERA=0のときの画像)
    std::wstring img_output_fname;  // 画像保存ファイル名

    CONFIG_COMMON_IMAGE_SCREEN_LAYOUT img_screen_layout;    // 画像表示レイアウト設定

    TAG_CONFIG_COMMON()
        : img_source_camera(0)
        , img_source_fname(L"")
        , img_output_fname(L"")
    {}
} CONFIG_COMMON, *PCONFIG_COMMON;

// システム設定
typedef struct TAG_CONFIG_SYSTEM {
    uint32_t     cycletime_sys; // メインスレッド実行周期[msec]
    std::wstring mcc_id;        // 機器ID(制御PC)[半角英数字4文字]
    std::wstring my_id;         // 機器ID(振れセンサーPC)[半角英数字4文字]

    TAG_CONFIG_SYSTEM()
        : cycletime_sys(0)
        , mcc_id(L"")
        , my_id(L"")
    {}
} CONFIG_SYSTEM, *PCONFIG_SYSTEM;

// タスク設定
// タスク設定(データ)
typedef struct TAG_CONFIG_TASK_DATA {
    uint32_t     cycletime; // メインスレッド実行周期[msec]
    std::wstring name;      // タスク名
    std::wstring sname;     // タスク略称

    TAG_CONFIG_TASK_DATA()
        : cycletime(0)
        , name(L"")
        , sname(L"")
    {}
} CONFIG_TASK_DATA, *PCONFIG_TASK_DATA;
// タスク設定
typedef struct TAG_CONFIG_TASK {
    CONFIG_TASK_DATA environment;   // Environment
    CONFIG_TASK_DATA policy;        // Policy
    CONFIG_TASK_DATA agent;         // Agent
    CONFIG_TASK_DATA scada;         // Scada
} CONFIG_TASK, *PCONFIG_TASK;

// 通信設定
// 通信設定(データ)
typedef struct TAG_CONFIG_ETHERNET_DATA {
    std::wstring ipaddress; // IPアドレス
    unsigned int port;      // ポート番号

    TAG_CONFIG_ETHERNET_DATA()
        : ipaddress(L"")
        , port(0)
    {}
} CONFIG_ETHERNET_DATA, *PCONFIG_ETHERNET_DATA;
// 通信設定
typedef struct TAG_CONFIG_SYS_ETHERNET {
    TAG_CONFIG_ETHERNET_DATA local;     // Local設定
    TAG_CONFIG_ETHERNET_DATA remote;    // Remote設定
} CONFIG_ETHERNET, *PCONFIG_ETHERNET;

// カメラ設定
// ROI設定
typedef struct TAG_CONFIG_CAMERA_ROI {
    uint32_t offset;    // ROI(領域)の開始位置(X, Y)
    uint32_t size;      // ROI(領域)のサイズ(X(32の倍数, 2592以下), Y(2の倍数, 2048以下))

    TAG_CONFIG_CAMERA_ROI()
        : offset(0)
        , size(0)
    {}
} CONFIG_CAMERA_ROI, *PCONFIG_CAMERA_ROI;
// ホワイトバランスゲイン設定
typedef struct TAG_CONFIG_CAMERA_BALANCE_WHITE {
    unsigned int wb_auto;       // ホワイトバランスゲイン自動調整モー設定(自動調整OFF[1]/連続自動調整[2]/一回のみ自動調整[3])
    double       wb_ratio_red;  // ホワイトバランスゲイン設定(Red Gain)
    double       wb_ratio_blue; // ホワイトバランスゲイン設定(Blue Gain)

    TAG_CONFIG_CAMERA_BALANCE_WHITE()
        : wb_auto(0)
        , wb_ratio_red(0.0)
        , wb_ratio_blue(0.0)
    {}
} CONFIG_CAMERA_BALANCE_WHITE, *PCONFIG_CAMERA_BALANCE_WHITE;
// カメラの基本設定
typedef struct TAG_CONFIG_CAMERA_BASIS {
    std::wstring                ipaddress;  // IPアドレス
    CONFIG_CAMERA_ROI           roi[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];   // ROI(領域)
    uint32_t                    packetsize; // ドライバが受け取るパケットの最大サイズ[byte]
    double                      framerate;  // フレームレート[fps]
    double                      blacklevel; // 黒レベル設定
    double                      gamma;      // ガンマ補正
    CONFIG_CAMERA_BALANCE_WHITE wb;         // ホワイトバランスゲイン設定
    double                      viewangle[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // 視野角[deg]

    TAG_CONFIG_CAMERA_BASIS()
        : ipaddress(L"")
        , packetsize(0)
        , framerate(0.0)
        , blacklevel(0.0)
        , gamma(0.0)
        , viewangle{0.0, 0.0}
    {}
} CONFIG_CAMERA_BASIS, *PCONFIG_CAMERA_BASIS;
// 輝度コントロール設定
typedef struct TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL {
    double val;     // 設定値
    double val_min; // 最小値
    double val_max; // 最大値

    BOOL   auto_control;    // 自動コントロール(自動[TRUE]/固定[FALSE])
    double auto_rate;       // 自動コントロール変化量
    double auto_start_h;    // 自動コントロール判定輝度(開始上限:この値より輝度が高い場合、暗くする)   [0 - 255]
    double auto_start_l;    // 自動コントロール判定輝度(開始下限:この値より輝度が高い場合、明るくする) [0 - 255]
    double auto_stop_h;     // 自動コントロール判定輝度(停止上限:この値より輝度が低い場合、停止する)   [0 - 255]
    double auto_stop_l;     // 自動コントロール判定輝度(停止下限:この値より輝度が高い場合、停止する)   [0 - 255]

    TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL()
        : val(0.0)
        , val_min(0.0)
        , val_max(0.0)
        , auto_control(FALSE)
        , auto_rate(0.0)
        , auto_start_h(0.0)
        , auto_start_l(0.0)
        , auto_stop_h(0.0)
        , auto_stop_l(0.0)
    {}
} CONFIG_CAMERA_BRIGHTNESS_CONTROL, *PCONFIG_CAMERA_BRIGHTNESS_CONTROL;
// カメラの異常判定設定
typedef struct TAG_CONFIG_CAMERA_ERROR {
    double   framedrop; // フレームレート低下の判定値[fps]
    uint32_t errtime;   // 異常判定時間[msec]

    TAG_CONFIG_CAMERA_ERROR()
        : framedrop(0.0)
        , errtime(0)
    {}
} CONFIG_CAMERA_ERROR, *PCONFIG_CAMERA_ERROR;
// カメラの起動設定
typedef struct TAG_CONFIG_CAMERA_BOOT {
    uint32_t bootwaittimer;     // システム起動後のカメラ起動待ち時間[msec]
    uint32_t rebootcount;       // システム起動後のカメラ起動失敗時にカメラリを再起動する回数[count]
    uint32_t rebootingtimer;    // カメラ再起動中であることを通知する時間[msec]

    TAG_CONFIG_CAMERA_BOOT()
        : bootwaittimer(0)
        , rebootcount(0)
        , rebootingtimer(0)
    {}
} CONFIG_CAMERA_BOOT, *PCONFIG_CAMERA_BOOT;
// カメラ設定
typedef struct TAG_CONFIG_CAMERA {
    CONFIG_CAMERA_BASIS              basis;     // カメラの基本設定
    CONFIG_CAMERA_BRIGHTNESS_CONTROL gain;      // 輝度コントロール設定(ゲイン)
    CONFIG_CAMERA_BRIGHTNESS_CONTROL expstime;  // 輝度コントロール設定(露光時間)
    CONFIG_CAMERA_ERROR              error;     // カメラの異常判定設定
    CONFIG_CAMERA_BOOT               boot;      // カメラの起動設定
} CONFIG_CAMERA, *PCONFIG_CAMERA;

// 傾斜計設定
// CANopen設定
typedef struct TAG_CONFIG_CANOPEN {
    std::wstring setup_fname;   // CANopen set-up .ini file name
    uint8_t      port;          // CANopen port (port1(=0) or port2(=1))
    uint8_t      node_id;       // CANopen node-ID

    TAG_CONFIG_CANOPEN()
        : setup_fname(L"")
        , port(0)
        , node_id(0x00)
    {}
} CONFIG_CANOPEN, *PCONFIG_CANOPEN;
// 傾斜計設定
typedef struct TAG_CONFIG_TILTMETER {
    CONFIG_CANOPEN canopen;         // CANopen設定
    uint16_t       dr_filter;       // 傾斜計フィルター(再帰形ディジタルフィルター)
    uint16_t       ma_filter;       // 傾斜計フィルター(移動平均フィルター)
    double         tiltspdfilter;   // 傾斜角速度フィルター時定数

    TAG_CONFIG_TILTMETER()
        : dr_filter(0)
        , ma_filter(0)
        , tiltspdfilter(0.0)
    {}
} CONFIG_TILTMETER, *PCONFIG_TILTMETER;

// 構造定義設定
// 取付オフセット設定
typedef struct TAG_CONFIG_MOUNTING_OFFSET {
    double l0;  // カメラ回転軸距離L0[mm]
    double t0;  // カメラ取付角度θ0(*10^6)[rad]
    double p0;  // カメラ設置角度φ0(*10^6)[rad]

    TAG_CONFIG_MOUNTING_OFFSET()
        : l0(0.0)
        , t0(0.0)
        , p0(0.0)
    {}
} CONFIG_MOUNTING_OFFSET, *PCONFIG_MOUNTING_OFFSET;
// 構造定義設定
typedef struct TAG_CONFIG_MOUNTING {
    CONFIG_MOUNTING_OFFSET offset[static_cast<uint32_t>(ENUM_AXIS::E_MAX)]; // 取付オフセット設定
} CONFIG_MOUNTING, *PCONFIG_MOUNTING;

// 画像処理条件設定
// ROI設定
typedef struct TAG_CONFIG_IMGPROC_ROI {
    BOOL   valid;   // ROI有効設定(無効[FALSE]/有効[TRUE])
    double scale;   // ROIスケール(検出したターゲットに対する倍率)

    TAG_CONFIG_IMGPROC_ROI()
        : valid(FALSE)
        , scale(0.0)
    {}
} CONFIG_IMGPROC_ROI, *PCONFIG_IMGPROC_ROI;
// ノイズフィルター
typedef struct TAG_CONFIG_IMGPROC_FILTER {
    uint32_t type;  // フィルター種類
    uint32_t val;   // フィルター値

    TAG_CONFIG_IMGPROC_FILTER()
        : type(0)
        , val(0)
    {}
} CONFIG_IMGPROC_FILTER, *PCONFIG_IMGPROC_FILTER;
// マスク設定
typedef struct TAG_CONFIG_IMGPROC_MASK {
    BOOL     valid;                                                 // マスク選択(無効[FALSE]/有効[TRUE])
    uint32_t hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];   // HSVマスク判定値(下限)
    uint32_t hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];   // HSVマスク判定値(上限)

    TAG_CONFIG_IMGPROC_MASK()
        : valid(FALSE)
        , hsv_l{0,0,0}
        , hsv_u{0,0,0}
    {}
} CONFIG_IMGPROC_MASK, *PCONFIG_IMGPROC_MASK;
// 画像処理条件設定
typedef struct TAG_CONFIG_IMGPROC {
    CONFIG_IMGPROC_ROI    roi;                                                      // ROI設定
    CONFIG_IMGPROC_MASK   mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];      // マスク設定
    CONFIG_IMGPROC_FILTER filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::E_MAX)];  // ノイズフィルター(0=ゴマ塩, 1=穴埋め)
    uint32_t              imgprc;                                                   // ターゲット検出アルゴリズム(0=最大輪郭面積, 1=最大輪郭長)
    double                swayspdfilter;                                            // 振れ速度フィルター時定数
    uint32_t              swayzeroset_time;                                         // 振れゼロ点設定(計測時間[ms])

    TAG_CONFIG_IMGPROC()
        : imgprc(0)
        , swayspdfilter(0.0)
        , swayzeroset_time(0)
    {}
} CONFIG_IMGPROC, *PCONFIG_IMGPROC;

// ***アプリケーション設定
typedef struct TAG_APP_CONFIG {
    CONFIG_COMMON    common;        // 共通設定
    CONFIG_SYSTEM    system;        // システム設定
    CONFIG_TASK      task;          // タスク設定
    CONFIG_ETHERNET  ethernet;      // 通信設定
    CONFIG_CAMERA    camera;        // カメラ設定
    CONFIG_TILTMETER tiltmeter;     // 傾斜計設定
    CONFIG_MOUNTING  mounting;      // 取付寸法設定
    CONFIG_IMGPROC   imageprocess;  // 画像処理条件設定
} APP_CONFIG, *PAPP_CONFIG;

//----------------------------------------------------------------------------
// アプリケーション情報の構造体
//----------------------------------------------------------------------------
// 画像情報
// 画像情報データ
typedef struct TAG_INFO_IMGBUF_DATA {
    cv::Mat cv_mat; // CpenCV
} INFO_IMGBUF_DATA, *PINFO_IMGBUF_DATA;
// 画像情報
typedef struct TAG_INFO_IMGBUF {
    CRITICAL_SECTION cs;
    INFO_IMGBUF_DATA data;  // 画像情報データ
} INFO_IMGBUF, *PINFO_IMGBUF;

// 傾斜計情報
// 傾斜計データ
typedef struct TAG_TILTMT_DATA {
    double deg;         // 傾斜角[deg]
    double rad;         // 傾斜角[rad]
    double speed_deg;   // 傾斜角速度[deg/s]
    double speed_rad;   // 傾斜角速度[rad/s]

    TAG_TILTMT_DATA()
        : deg(0.0)
        , rad(0.0)
        , speed_deg(0.0)
        , speed_rad(0.0)
    {}
} TILT_DATA, *PTILT_DATA;
// 傾斜計情報データ
typedef struct TAG_INFO_TILTMT_DATA {
    int32_t   status;                                           // ステータス情報
    TILT_DATA tilt[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];    // 傾斜計データ

    TAG_INFO_TILTMT_DATA()
        : status(static_cast<uint32_t>(ENUM_TILT_STATUS::DEFAULT))
    {}
} INFO_TILTMT_DATA, *PINFO_TILTMT_DATA;
// 傾斜計情報
typedef struct TAG_INFO_TILTMT {
    CRITICAL_SECTION cs;
    INFO_TILTMT_DATA data;  // 傾斜計情報データ
} INFO_TILTMT, *PINFO_TILTMT;

// クライアント情報
// クライアント情報データ
typedef struct TAG_INFO_CLIENT_DATA {
    std::wstring device_id;                                                 // 機器ID
    uint32_t     command;                                                   // コマンド
    uint32_t     mode;                                                      // モード
    uint32_t     cycle;                                                     // 最小受信周期[ms]
    double       tgt_len[static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX)];   // ターゲット距離[mm]

    TAG_INFO_CLIENT_DATA()
        : device_id(L"")
        , command(0)
        , mode(0)
        , cycle(0)
        , tgt_len{0.0,0.0}
    {}
} INFO_CLIENT_DATA, *PINFO_CLIENT_DATA;
// クライアント情報
typedef struct TAG_INFO_CLIENT {
    CRITICAL_SECTION cs;
    INFO_CLIENT_DATA data;  // クライアント情報データ
} INFO_CLIENT, *PINFO_CLIENT;

// 調整情報
// 調整情報データ
typedef struct TAG_INFO_ADJUST_DATA {
    BOOL   target_distance_fixed;   // 吊点～ターゲット間距離固定
    double target_distance;         // 吊点～ターゲット間距離[mm]

    TAG_INFO_ADJUST_DATA()
        : target_distance_fixed(FALSE)
        , target_distance(EXTN_TARGETDIST_MIN)
    {}
} INFO_ADJUST_DATA, *PINFO_ADJUST_DATA;
// 調整情報
typedef struct _stExtnInfo {
    CRITICAL_SECTION cs;
    INFO_ADJUST_DATA data;  // 調整情報データ
} INFO_ADJUST, *PINFO_ADJUST;

// 画像処理情報
// ターゲット検出データ
typedef struct TAG_TARGET_DATA {
    BOOL     valid;                                         // 検出状態
    double   max_val;                                       // 最大輝度
    double   pos[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];  // 検出位置[pixel]
    int32_t  size;                                          // 検出サイズ
    cv::Rect roi;                                           // ROI

    TAG_TARGET_DATA()
        : valid(FALSE)
        , max_val(0.0)
        , pos{0.0, 0.0}
        , size(0)
    {}
} TARGET_DATA, *PTARGET_DATA;
// 振れ検出データ(ターゲット検出データの中心)
typedef struct TAG_SWAY_DATA {
    double target_pos;  // ターゲット位置[pixel]
    double target_tilt; // ターゲット傾き[pixel]
    double sway_angle;  // 振れ角[pixel]
    double sway_speed;  // 振れ速度[pixel/s]
    double sway_zero;   // 振れ中心[pixel]

    TAG_SWAY_DATA()
        : target_pos(0.0)
        , target_tilt(0.0)
        , sway_angle(0.0)
        , sway_speed(0.0)
        , sway_zero(0.0)
    {}
} SWAY_DATA, *PSWAY_DATA;
// 画像処理情報データ
typedef struct TAG_INFO_IMGPRC_DATA {
    std::vector<std::vector<cv::Point>> contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];    // 輪郭座標
    TARGET_DATA target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];                         // ターゲット検出データ
    SWAY_DATA   sway_data[static_cast<uint32_t>(ENUM_AXIS::E_MAX)];                                 // 振れ検出データ(ターゲット検出データの中心)
    double      target_size;                                                                        // ターゲットサイズ(ターゲット検出データの平均)
    uint32_t    status;                                                                             // 状態
    double      img_fps;                                                                            // フレームレート[fps]
    double      img_val;                                                                            // 明度
    int32_t     exps_mode;                                                                          // 自動露光コマンド(0:停止 1:Up -1:Down)
    double      exps_time;                                                                          // 露光時間[us]

    TAG_INFO_IMGPRC_DATA()
        : target_size(0.0)
        , status(static_cast<uint32_t>(ENUM_PROCCESS_STATUS::DEFAULT))
        , img_fps(0.0)
        , img_val(0.0)
        , exps_mode(0)
        , exps_time(0.0)
    {}
} INFO_IMGPRC_DATA, *PINFO_IMGPRC_DATA;
// 画像処理情報
typedef struct TAG_INFO_IMGPRC {
    CRITICAL_SECTION cs;
    INFO_IMGPRC_DATA data;  // 画像処理情報データ
} INFO_IMGPRC, *PINFO_IMGPRC;

// システム情報
// システム情報データ
typedef struct TAG_INFO_SYSTEM_DATA {
    uint32_t status;    // ステータス

    TAG_INFO_SYSTEM_DATA()
        : status(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::DEFAULT))
    {}
} INFO_SYSTEM_DATA, *PINFO_SYSTEM_DATA;
// システム情報
typedef struct TAG_INFO_SYSTEM {
    CRITICAL_SECTION cs;
    INFO_SYSTEM_DATA data;  // システム情報データ
} INFO_SYSTEM, *PINFO_SYSTEM;

// ***アプリケーション情報
typedef struct TAG_APP_INFO {
    INFO_IMGBUF imgbuf[static_cast<uint32_t>(ENUM_IMAGE::E_MAX)];   // 画像情報
    INFO_TILTMT tiltmt;                                             // 傾斜計情報
    INFO_CLIENT client;                                             // クライアント情報
    INFO_ADJUST adjust;                                             // 調整情報
    INFO_IMGPRC imgprc;                                             // 画像処理情報
    INFO_SYSTEM system;                                             // システム情報
} APP_INFO, *PAPP_INFO;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CShared

/// <summary>
/// タスク間共有情報ハンドリングクラス
/// メインスレッド及び各タスクでインスタンス作成
/// </summary>

class CShared
{
public:
    CShared(BOOL init = FALSE);
    ~CShared();

    // メンバー変数

    // メンバー関数
    BOOL set_app_config_ini(wchar_t* file_name);    // ini file読み込みパラメータ設定

    BOOL set_app_config(CONFIG_COMMON data);        // 共通設定書込み
    BOOL get_app_config(PCONFIG_COMMON data);       // 共通設定読込み
    BOOL set_app_config(CONFIG_SYSTEM data);        // システム設定書込み
    BOOL get_app_config(PCONFIG_SYSTEM data);       // システム設定読込み
    BOOL set_app_config(CONFIG_TASK data);          // タスク設定書込み
    BOOL get_app_config(PCONFIG_TASK data);         // タスク設定読込み
    BOOL set_app_config(CONFIG_ETHERNET data);      // 通信設定書込み
    BOOL get_app_config(PCONFIG_ETHERNET data);     // 通信設定読込み
    BOOL set_app_config(CONFIG_CAMERA data);        // カメラ設定書込み
    BOOL get_app_config(PCONFIG_CAMERA data);       // カメラ設定読込み
    BOOL set_app_config(CONFIG_TILTMETER data);     // 傾斜計設定書込み
    BOOL get_app_config(PCONFIG_TILTMETER data);    // 傾斜計設定読込み
    BOOL set_app_config(CONFIG_MOUNTING data);      // 取付寸法設定書込み
    BOOL get_app_config(PCONFIG_MOUNTING data);     // 取付寸法設定読込み
    BOOL set_app_config(CONFIG_IMGPROC data);       // 画像処理条件設定書込み
    BOOL get_app_config(PCONFIG_IMGPROC data);      // 画像処理条件設定読込み

    BOOL set_app_info_data(uint8_t id, cv::Mat cv_mat);     // 画像データ書込み
    BOOL get_app_info_data(uint8_t id, cv::Mat* cv_mat);    // 画像データ読込み
    BOOL set_app_info_data(INFO_TILTMT_DATA data);          // 傾斜計情報データ書込み
    BOOL get_app_info_data(PINFO_TILTMT_DATA data);         // 傾斜計情報データ読込み
    BOOL set_app_info_data(INFO_CLIENT_DATA data);          // クライアント情報データ書込み
    BOOL get_app_info_data(PINFO_CLIENT_DATA data);         // クライアント情報データ読込み
    BOOL set_app_info_data(INFO_ADJUST_DATA data);          // 調整情報データ書込み
    BOOL get_app_info_data(PINFO_ADJUST_DATA data);         // 調整情報データ読込み
    BOOL set_app_info_data(INFO_IMGPRC_DATA data);          // 画像処理情報データ書込み
    BOOL get_app_info_data(PINFO_IMGPRC_DATA data);         // 画像処理情報データ読込み
    BOOL set_app_info_data(INFO_SYSTEM_DATA data);          // システム情報データ書込み
    BOOL get_app_info_data(PINFO_SYSTEM_DATA data);         // システム情報データ読込み

protected:

private:
    // メンバー変数
    static APP_CONFIG m_app_cnfg;   // アプリケーション設定
    static APP_INFO   m_app_info;   // アプリケーション情報

    // メンバー関数
};
