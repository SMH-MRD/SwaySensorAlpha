#pragma once

#include <windows.h>
#include <string>

//============================================================================
// define定義
//----------------------------------------------------------------------------
// .iniファイル関連設定
#define FOLDER_OF_INIFILE                               L"Data\\"                       // .iniファイルフォルダ
#define EXT_OF_INIFILE                                  L"ini"                          // .iniファイル拡張子
#define PATH_OF_INIFILE                                 m_path                          // アプリケーション.iniファイルパス

#define INI_SCT_COMMON                                  L"COMMON"                       // 共通設定
#define INI_KEY_COMMON_SYSTEM_CYCLE_TIME                L"CMN_SYSTEM_CYCLE_TIME"        // メインスレッド実行周期
#define INI_KEY_COMMON_IMAGE_SOURCE                     L"CMN_IMAGE_SOURCE"             // 画像取込み元
#define INI_KEY_COMMON_IMAGE_FILENAME_SOURCE            L"CMN_IMAGE_FILENAME_SOURCE"    // 取込み画像ファイル名
#define INI_KEY_COMMON_IMAGE_FILENAME_OUTPUT            L"CMN_IMAGE_FILENAME_OUTPUT"    // 画像保存ファイル名

#define INI_SCT_TASK                                    L"TASK"                         // タスク設定
#define INI_KEY_TASK_COLUMN_NUM                         3                               // 
#define INI_KEY_TASK_ENVIRONMENT                        L"TSK_ENVIRONMENT"              // Environment
#define INI_KEY_TASK_POLICY                             L"TSK_POLICY"                   // Policy
#define INI_KEY_TASK_AGENT                              L"TSK_AGENT"                    // Agent
#define INI_KEY_TASK_SCADA                              L"TSK_SCADA"                    // Scada

#define INI_SCT_ETHERNET                                L"ETHERNET"                     // 通信設定
#define INI_KEY_ETHERNET_COLUMN_NUM                     2                               // 
#define INI_KEY_ETHERNET_LOCAL                          L"NET_LOCAL"                    // Local設定
#define INI_KEY_ETHERNET_REMOTE                         L"NET_REMOTE"                   // Remote設定

#define INI_SCT_CAMERA                                  L"CAMERA"                       // カメラ設定
#define INI_KEY_CAMERA_BASIS_IP_ADDRESS                 L"CAM_BASIS_IP_ADDRESS"         // カメラのIPアドレス
#define INI_KEY_CAMERA_BASIS_ROI_COLUMN_NUM             4                               // 
#define INI_KEY_CAMERA_BASIS_ROI                        L"CAM_BASIS_ROI"                // ROI(領域)
#define INI_KEY_CAMERA_BASIS_PACKET_SIZE                L"CAM_BASIS_PACKET_SIZE"        // ドライバが受け取るパケットの最大サイズ
#define INI_KEY_CAMERA_BASIS_FRAME_RATE                 L"CAM_BASIS_FRAME_RATE"         // フレームレート
#define INI_KEY_CAMERA_BASIS_BLACKLEVEL                 L"CAM_BASIS_BLACKLEVEL"         // 黒レベル設定
#define INI_KEY_CAMERA_BASIS_GAMMA                      L"CAM_BASIS_GAMMA"              // ガンマ補正
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_COLUMN_NUM   3                               // 
#define INI_KEY_CAMERA_BASIS_WBALANCE_AUTO              L"CAM_BASIS_WBALANCE_AUTO"      // ホワイトバランスゲイン設定
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE_COLUMN_NUM      2                               // 
#define INI_KEY_CAMERA_BASIS_VIEW_ANGLE                 L"CAM_BASIS_VIEW_ANGLE"         // カメラ視野角
#define INI_KEY_CAMERA_GAIN_COLUMN_NUM                  3                               // 
#define INI_KEY_CAMERA_GAIN                             L"CAM_GAIN"                     // ゲイン設定
#define INI_KEY_CAMERA_GAIN_AUTO_COLUMN_NUM             2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO                        L"CAM_GAIN_AUTO"                // 自動ゲイン設定
#define INI_KEY_CAMERA_GAIN_AUTO_LIMIT                  2                               // 
#define INI_KEY_CAMERA_GAIN_AUTO_START                  L"CAM_GAIN_AUTO_START"          // 自動ゲイン判定輝度(開始)
#define INI_KEY_CAMERA_GAIN_AUTO_STOP                   L"CAM_GAIN_AUTO_STOP"           // 自動ゲイン判定輝度(停止)
#define INI_KEY_CAMERA_EXPSTIME_COLUMN_NUM              3                               // 
#define INI_KEY_CAMERA_EXPSTIME                         L"CAM_EXPSTIME"                 // 露光時間設定
#define INI_KEY_CAMERA_EXPSTIME_AUTO_COLUMN_NUM         2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO                    L"CAM_EXPSTIME_AUTO"            // 自動露光設定
#define INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM   2                               // 
#define INI_KEY_CAMERA_EXPSTIME_AUTO_START              L"CAM_EXPSTIME_AUTO_START"      // 自動露光判定輝度(開始)
#define INI_KEY_CAMERA_EXPSTIME_AUTO_STOP               L"CAM_EXPSTIME_AUTO_STOP"       // 自動露光判定輝度(停止)
#define INI_KEY_CAMERA_ERROR_COLUMN_NUM                 2                               // 
#define INI_KEY_CAMERA_ERROR                            L"CAM_ERROR"                    // カメラの異常判定設定
#define INI_KEY_CAMERA_BOOT_COLUMN_NUM                  3                               // 
#define INI_KEY_CAMERA_BOOT                             L"CAM_BOOT"                     // カメラの起動設定

#define INI_SCT_MOUNTING                                L"MOUNTING"                     // 取付寸法設定
#define INI_KEY_MOUNTING_OFFSET_COLUMN_NUM              2                               // 
#define INI_KEY_MOUNTING_OFFSET_LX0                     L"MNT_OFFSET_LX0"               // 吊具吊点～BOX吊点距離LX0
#define INI_KEY_MOUNTING_OFFSET_LY0                     L"MNT_OFFSET_LY0"               // 吊具吊点～BOX吊点距離LY0
#define INI_KEY_MOUNTING_OFFSET_L0                      L"MNT_OFFSET_L0"                // BOX吊点～BOX可動部中心距離L0
#define INI_KEY_MOUNTING_OFFSET_LC                      L"MNT_OFFSET_LC"                // BOX可動部中心～カメラ中心距離LC
#define INI_KEY_MOUNTING_OFFSET_A0                      L"MNT_OFFSET_A0"                // BOX内カメラ取付角度θ0
#define INI_KEY_MOUNTING_OFFSET_AC                      L"MNT_OFFSET_AC"                // BOX可動部中心～カメラ中心角度θc

#define INI_SCT_INCLINOMETER                            L"INCLINOMETER"                 // 傾斜計設定
#define INI_KEY_INCLINOMETER_DEVICE_COLUMN_NUM          4                               // 
#define INI_KEY_INCLINOMETER_DEVICE                     L"INC_DEVICE"                   // デバイス設定
#define INI_KEY_INCLINOMETER_PORT_NUMBER_COLUMN_NUM     2                               // 
#define INI_KEY_INCLINOMETER_PORT_NUMBER                L"INC_PORT_NUMBER"              // 傾斜計データ接続ポート番号

#define INI_SCT_IMGPROC                                 L"IMGPROC"                      // 画像処理条件設定
#define INI_KEY_IMGPROC_ROI_COLUMN_NUM                  2                               // 
#define INI_KEY_IMGPROC_ROI                             L"IMG_ROI"                      // 画像ROI設定
#define INI_KEY_IMGPROC_MASK                            L"IMG_MASK"                     // マスク画像選択
#define INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM           3                               // 
#define INI_KEY_IMGPROC_MASK1_LOW                       L"IMG_MASK1_LOW"                // 画像1マスク下限
#define INI_KEY_IMGPROC_MASK1_UP                        L"IMG_MASK1_UP"                 // 画像1マスク上限
#define INI_KEY_IMGPROC_MASK2_LOW                       L"IMG_MASK2_LOW"                // 画像2マスク下限
#define INI_KEY_IMGPROC_MASK2_UP                        L"IMG_MASK2_UP"                 // 画像2マスク上限
#define INI_KEY_IMGPROC_FILTER_COLUMN_NUM               2                               // 
#define INI_KEY_IMGPROC_FILTER1                         L"IMG_FILTER1"                  // フィルタ1(ゴマ塩ノイズ)
#define INI_KEY_IMGPROC_FILTER2                         L"IMG_FILTER2"                  // フィルタ2(穴埋めノイズ)
#define INI_KEY_IMGPROC_PROC                            L"IMG_PROC"                     // ターゲット検出アルゴリズム
#define INI_KEY_IMGPROC_SWAY_SPEED_TC                   L"IMG_SWAY_SPEED_TC"            // 振れ速度フィルタ時定数

//----------------------------------------------------------------------------
// 
// H, S, V
#define IMAGE_HSV_H_MIN                                 0
#define IMAGE_HSV_H_MAX                                 179
#define IMAGE_HSV_S_MIN                                 0
#define IMAGE_HSV_S_MAX                                 255
#define IMAGE_HSV_V_MIN                                 0
#define IMAGE_HSV_V_MAX                                 255

// RIOエラービット
#define RIO_ERR_NONE                                    0x00000000  // エラーなし
#define RIO_ERR_INIT_INCOMPLETE                         0x00000001  // RIO初期化未完了
#define RIO_ERR_SET_IOLINKMODE                          0x00000010  // IO LINK MODE設定エラー
#define RIO_ERR_SET_PARAM_VALID                         0x00000020  // 有効化設定エラー
#define RIO_ERR_SET_PARAM_AI                            0x00000040  // AI設定エラー
#define RIO_ERR_GET_AI_READ                             0x00000100  // データ読み込みエラー応答

// Process Data Error
#define S7CMPTBL_FORMAT_OVERRANGE                       0x7FFF      // Overrange
#define S7CMPTBL_FORMAT_UNDERRANGE                      0x8000      // Open circuit/short circuit/underrange
#define S7CMPTBL_FORMAT_SIGNBIT                         0x8000      // Sign bit

// 外部入力
#define EXTN_ROPELEN_MIN                                1000.0      // ロープ長(最小)
#define EXTN_ROPELEN_MAX                                32000.0     // ロープ長(最大)

#define EXTN_CAMBOXANGL_MIN                             -4500.0     // カメラBOX傾き(最小)
#define EXTN_CAMBOXANGL_MAX                             4500.0      // カメラBOX傾き(最大)

// シャッタコントロールモード
#define	EXPOSURE_CONTROL_HOLD                           0           // シャッタコントロール:Hold
#define	EXPOSURE_CONTROL_LIGHT                          1           // シャッタコントロール:明るく
#define	EXPOSURE_CONTROL_DARKEN                         -1          // シャッタコントロール:暗く

//============================================================================
// enum定義
//----------------------------------------------------------------------------
// カメラ画像配列 INDEX
enum {
    IMAGE_ID_RAW_A = 0, // 元画像A
    IMAGE_ID_RAW_B,     // 元画像B
    IMAGE_ID_MASK1_A,   // マスク画像1A
    IMAGE_ID_MASK1_B,   // マスク画像1B
    IMAGE_ID_MASK2_A,   // マスク画像2A
    IMAGE_ID_MASK2_B,   // マスク画像2B
    IMAGE_ID_PROC_A,    // 処理画像A
    IMAGE_ID_PROC_B,    // 処理画像B
    IMAGE_ID_MAX
};

// 画像処理配列 INDEX
enum {
    IMGPROC_ID_IMG_1 = 0,   // 画像1処理データ
    IMGPROC_ID_IMG_2,       // 画像2処理データ
    IMGPROC_ID_MAX
};

// 傾斜計入力データ
enum {
    RIO_PORT_1 = 0, // RIO入力ポート1 
    RIO_PORT_2,     // RIO入力ポート2 
    RIO_PORT_MAX
};

// カメラ制御用パラメータ
enum {
    GRAB_IMG_STOP = 0,      // 画像取込み(停止)
    GRAB_IMG_GRAB_CAMERA,   // 画像取込み(カメラ)
    GRAB_IMG_GRAB_FILE      // 画像取込み(ファイル)
};

// マスク画像選択
enum {
    MASK_IMG_ALL = 0,   // マスク画像選択(両方)
    MASK_IMG_IMAGE1 ,   // マスク画像選択(画像1のみ)
    MASK_IMG_IMAGE2,    // マスク画像選択(画像2のみ)
};

// ノイズフィルタ
enum {
    NOISEFILTER1_NONE = 0,  // ノイズフィルタ:なし
    NOISEFILTER1_MEDIAN,    // ノイズフィルタ:中央値フィルタ
    NOISEFILTER1_OPENNING,  // ノイズフィルタ:オープニング処理
    NOIZEFILTER1_MAX
};

// ノイズフィルタ
enum {
    NOISEFILTER2_NONE = 0,  // ノイズフィルタ:なし
    NOISEFILTER2_CLOSING,   // ノイズフィルタ:クロージング処理
    NOIZEFILTER2_MAX
};

// 重心位置算出アルゴリズム
enum {
    COG_ALGORITHM_ALL = 0,  // 重心位置算出アルゴリズム(全輪郭点)
    COG_ALGORITHM_AREA,     // 重心位置算出アルゴリズム(最大輪郭面積)
    COG_ALGORITHM_LEN,      // 重心位置算出アルゴリズム(最大輪郭長)
    COG_ALGORITHM_MAX
};

// 軸
enum {
    AXIS_X = 0, // X軸
    AXIS_Y,     // Y軸
    AXIS_MAX
};

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
// .iniファイル情報の構造体
//----------------------------------------------------------------------------
// 共通設定
typedef struct TAG_CONFIG_COMMON {  // 共通設定
    unsigned int cycletime_sys;     // メインスレッド実行周期[msec]
    unsigned int img_source;        // 画像取込み元(カメラ[0]/画像ファイル[1]) 
    std::wstring imgfname_source;   // 取込み画像ファイル名(IMAGE_SOURCE=1のときの画像)
    std::wstring imgfname_output;   // 画像保存ファイル名

    TAG_CONFIG_COMMON()
        : cycletime_sys(0)
        , img_source(0) 
        , imgfname_source(L"")
        , imgfname_output(L"")
    {}
} CONFIG_COMMON, *PCONFIG_COMMON;

// タスク設定
typedef struct TAG_CONFIG_TASK_DATA {   // タスク設定(データ)
    unsigned int cycletime;             // メインスレッド実行周期[msec]
    std::wstring name;                  // タスク名
    std::wstring sname;                 // タスク略称

    TAG_CONFIG_TASK_DATA()
        : cycletime(0)
        , name(L"")
        , sname(L"")
    {}
} CONFIG_TASK_DATA, *PCONFIG_TASK_DATA;
typedef struct TAG_CONFIG_TASK {    // タスク設定
    CONFIG_TASK_DATA environment;   // Environment
    CONFIG_TASK_DATA policy;        // Policy
    CONFIG_TASK_DATA agent;         // Agent
    CONFIG_TASK_DATA scada;         // Scada
} CONFIG_TASK, *PCONFIG_TASK;

// 通信設定
typedef struct TAG_CONFIG_ETHERNET_DATA {   // 通信設定(データ)
    std::wstring ipaddress;                 // IPアドレス
    unsigned int port;                      // ポート番号

    TAG_CONFIG_ETHERNET_DATA()
        : ipaddress(L"")
        , port(0)
    {}
} CONFIG_ETHERNET_DATA, *PCONFIG_ETHERNET_DATA;
typedef struct TAG_CONFIG_SYS_ETHERNET {    // 通信設定
    TAG_CONFIG_ETHERNET_DATA local;         // Local設定
    TAG_CONFIG_ETHERNET_DATA remote;        // Remote設定
} CONFIG_ETHERNET, *PCONFIG_ETHERNET;

// カメラ設定
// ROI設定
typedef struct TAG_CONFIG_CAMERA_ROI {  // ROI設定
    uint32_t offset;                    // ROI(領域)の開始位置(X, Y)
    uint32_t size;                      // ROI(領域)のサイズ(X(32の倍数, 2592以下), Y(2の倍数, 2048以下))

    TAG_CONFIG_CAMERA_ROI()
        : offset(0)
        , size(0)
    {}
} CONFIG_CAMERA_ROI, *PCONFIG_CAMERA_ROI;
// ホワイトバランスゲイン設定
typedef struct TAG_CONFIG_CAMERA_BALANCE_WHITE {    // ホワイトバランスゲイン設定
    unsigned int wb_auto;                           // ホワイトバランスゲイン自動調整モー設定(自動調整OFF[1]/連続自動調整[2]/一回のみ自動調整[3])
    double       wb_ratio_red;                      // ホワイトバランスゲイン設定(Red Gain)
    double       wb_ratio_blue;                     // ホワイトバランスゲイン設定(Blue Gain)

    TAG_CONFIG_CAMERA_BALANCE_WHITE()
        : wb_auto(0)
        , wb_ratio_red(0.0)
        , wb_ratio_blue(0.0)
    {}
} CONFIG_CAMERA_BALANCE_WHITE, *PCONFIG_CAMERA_BALANCE_WHITE;
// カメラの基本設定
typedef struct TAG_CONFIG_CAMERA_BASIS {                // カメラの基本設定
    std::wstring                ipaddress;              // IPアドレス
    CONFIG_CAMERA_ROI           roi[AXIS_MAX];          // ROI(領域)
    uint32_t                    packetsize;             // ドライバが受け取るパケットの最大サイズ[byte]
    double                      framerate;              // フレームレート[fps]
    double                      blacklevel;             // 黒レベル設定
    double                      gamma;                  // ガンマ補正
    CONFIG_CAMERA_BALANCE_WHITE wb;                     // ホワイトバランスゲイン設定
    double                      viewangle[AXIS_MAX];    // 視野角[deg]

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
typedef struct TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL {   // 輝度コントロール設定
    double val;                                         // 設定値
    double minval;                                      // 最小値
    double maxval;                                      // 最大値

    BOOL   atcontrol;                                   // 自動コントロール(固定[0]/自動[1])
    double atrate;                                      // 自動コントロール変化量
    double atstart_h;                                   // 自動コントロール判定輝度(開始上限:この値より輝度が高い場合、暗くする)   [0 - 255]
    double atstart_l;                                   // 自動コントロール判定輝度(開始下限:この値より輝度が高い場合、明るくする) [0 - 255]
    double atstop_h;                                    // 自動コントロール判定輝度(停止上限:この値より輝度が低い場合、停止する)   [0 - 255]
    double atstop_l;                                    // 自動コントロール判定輝度(停止下限:この値より輝度が高い場合、停止する)   [0 - 255]

    TAG_CONFIG_CAMERA_BRIGHTNESS_CONTROL()
        : val(0.0)
        , minval(0.0)
        , maxval(0.0)
        , atcontrol(FALSE)
        , atrate(0.0)
        , atstart_h(0.0)
        , atstart_l(0.0)
        , atstop_h(0.0)
        , atstop_l(0.0)
    {}
} CONFIG_CAMERA_BRIGHTNESS_CONTROL, *PCONFIG_CAMERA_BRIGHTNESS_CONTROL;
// カメラの異常判定設定
typedef struct TAG_CONFIG_CAMERA_ERROR {    // カメラの異常判定設定
    double       framedrop;                 // フレームレート低下の判定値[fps]
    unsigned int errtime;                   // 異常判定時間[msec]

    TAG_CONFIG_CAMERA_ERROR()
        : framedrop(0.0)
        , errtime(0)
    {}
} CONFIG_CAMERA_ERROR, *PCONFIG_CAMERA_ERROR;
// カメラの起動設定
typedef struct TAG_CONFIG_CAMERA_BOOT { // カメラの起動設定
    unsigned int bootwaittimer;         // システム起動後のカメラ起動待ち時間[msec]
    unsigned int rebootcount;           // システム起動後のカメラ起動失敗時にカメラリを再起動する回数[count]
    unsigned int rebootingtimer;        // カメラ再起動中であることを通知する時間[msec]

    TAG_CONFIG_CAMERA_BOOT()
        : bootwaittimer(0)
        , rebootcount(0)
        , rebootingtimer(0)
    {}
} CONFIG_CAMERA_BOOT, *PCONFIG_CAMERA_BOOT;
typedef struct TAG_CONFIG_CAMERA {              // カメラ設定
    CONFIG_CAMERA_BASIS              basis;     // カメラの基本設定
    CONFIG_CAMERA_BRIGHTNESS_CONTROL gain;      // 輝度コントロール設定(ゲイン)
    CONFIG_CAMERA_BRIGHTNESS_CONTROL expstime;  // 輝度コントロール設定(露光時間)
    CONFIG_CAMERA_ERROR              error;     // カメラの異常判定設定
    CONFIG_CAMERA_BOOT               boot;      // カメラの起動設定
} CONFIG_CAMERA, *PCONFIG_CAMERA;

// 構造定義設定
// 取付オフセット設定
typedef struct TAG_CONFIG_MOUNTING_OFFSET { // 取付オフセット設定
    double lx0;                             // 吊具吊点～BOX吊点距離LX0[mm]
    double ly0;                             // 吊具吊点～BOX吊点距離LY0[mm]
    double l0;                              // BOX吊点～BOX可動部中心距離L0[mm]
    double lc;                              // BOX可動部中心～カメラ中心距離LC[mm]
    double a0;                              // BOX内カメラ取付角度θ0[deg]
    double ac;                              // BOX可動部中心～カメラ中心角度θc[deg]

    TAG_CONFIG_MOUNTING_OFFSET()
        : lx0(0.0)
        , ly0(0.0)
        , l0(0.0)
        , lc(0.0)
        , a0(0.0)
        , ac(0.0)
    {}
} CONFIG_MOUNTING_OFFSET, *PCONFIG_MOUNTING_OFFSET;
typedef struct TAG_CONFIG_MOUNTING {            // 構造定義設定
    CONFIG_MOUNTING_OFFSET offset[AXIS_MAX];    // 取付オフセット設定
} CONFIG_MOUNTING, *PCONFIG_MOUNTING;

// 傾斜計設定
typedef struct TAG_CONFIG_INCLINOMETER {    // 傾斜計設定
    std::wstring ipaddress;                 // IPアドレス
    unsigned int tcpport;                   // TCPポート番号
    unsigned int slaveaddress;              // スレーブアドレス
    unsigned int timeout;                   // 通信タイムアウト[msec]
    unsigned int port[AXIS_MAX];            // 傾斜計データ接続ポート番号[1～8] (X, Y) 

    TAG_CONFIG_INCLINOMETER()
        : ipaddress(L"")
        , tcpport(0)
        , slaveaddress(0)
        , timeout(0)
        , port{0, 0}
    {}
} CONFIG_INCLINOMETER, *PCONFIG_INCLINOMETER;

// 画像処理条件設定
// ROI設定
typedef struct TAG_CONFIG_IMGPROC_ROI { // ROI設定
    BOOL   valid;                       // ROI有効設定(無効[FALSE]/有効[TRUE])
    double scale;                       // ROIスケール(検出したターゲットに対する倍率)

    TAG_CONFIG_IMGPROC_ROI()
        : valid(FALSE)
        , scale(0.0)
    {}
} CONFIG_IMGPROC_ROI, *PCONFIG_IMGPROC_ROI;
// HSVマスク判定値
typedef struct TAG_CONFIG_IMGPROC_HSV { // HSVマスク判定値
    unsigned int h;                     // H
    unsigned int s;                     // S
    unsigned int v;                     // V

    TAG_CONFIG_IMGPROC_HSV()
        : h(0)
        , s(0)
        , v(0)
    {}
} CONFIG_IMGPROC_HSV, *PCONFIG_IMGPROC_HSV;
// ノイズフィルタ
typedef struct TAG_CONFIG_IMGPROC_FILTER {  // ノイズフィルタ
    unsigned int type;                      // フィルタ種類
    unsigned int val;                       // フィルタ値

    TAG_CONFIG_IMGPROC_FILTER()
        : type(0)
        , val(0)
    {}
} CONFIG_IMGPROC_FILTER, *PCONFIG_IMGPROC_FILTER;
// マスク設定
typedef struct TAG_CONFIG_IMGPROC_MASK {    // マスク設定
    BOOL               valid;               // マスク選択(無効[FALSE]/有効[TRUE])
    CONFIG_IMGPROC_HSV hsv_l;               // HSVマスク判定値(下限)
    CONFIG_IMGPROC_HSV hsv_h;               // HSVマスク判定値(上限)

    TAG_CONFIG_IMGPROC_MASK()
        : valid(FALSE)
    {}
} CONFIG_IMGPROC_MASK, *PCONFIG_IMGPROC_MASK;
typedef struct TAG_CONFIG_IMGPROC {             // 画像処理条件設定
    CONFIG_IMGPROC_ROI    roi;                  // ROI設定
    CONFIG_IMGPROC_MASK   mask[IMGPROC_ID_MAX]; // マスク設定
    CONFIG_IMGPROC_FILTER fliter1;              // ゴマ塩ノイズフィルタ(0=なし, 1=中央値フィルタ, 2=オープニング処理), フィルタ値(中央値フィルタ=1,3,5,... オープニング処理=1,2,...)
    CONFIG_IMGPROC_FILTER fliter2;              // 穴埋めノイズフィルタ(0=なし, 1=クロージング処理), フィルタ値(1,2,...)
    unsigned int          imgproc;              // ターゲット検出アルゴリズム(0=全輪郭点, 1=最大輪郭面積, 2=最大輪郭長)
    double                swayspdfliter;        // 振れ速度フィルタ時定数

    TAG_CONFIG_IMGPROC()
        : imgproc(0)
        , swayspdfliter(0.0)
    {}
} CONFIG_IMGPROC, *PCONFIG_IMGPROC;

// システム設定
typedef struct TAG_APP_CONFIG {         // システム設定
    CONFIG_COMMON       common;         // 共通設定
    CONFIG_TASK         task;           // タスク設定
    CONFIG_ETHERNET     ethernet;       // 通信設定
    CONFIG_CAMERA       camera;         // 通信設定
    CONFIG_MOUNTING     mounting;       // 取付寸法設定
    CONFIG_INCLINOMETER inclinometer;   // 傾斜計設定
    CONFIG_IMGPROC      imageprocess;   // 画像処理条件設定
} APP_CONFIG, *PAPP_CONFIG;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@

#if 0
// 外部入力設定
typedef struct _stExtnParamData                         // 外部入力設定データ
{
    int    ropelenfixed;                                // ロープ長固定
    double ropelen;                                     // ロープ長

    int    boxanglefixed;                               // BOX傾き固定
    double boxangle[AXIS_MAX];                          // BOX傾き
} stExtnParamData;
typedef struct _stExtnParam                             // 外部入力設定
{
    CRITICAL_SECTION    cs;
    stExtnParamData     data;                           // 外部入力設定データ
} stExtnParam;

// 画像情報
typedef struct _stImageData
{
    cv::Mat image;
    BOOL    update;                                     // image setでtrue getでfalse
} stImageData;
typedef struct _stImageInfo
{
    CRITICAL_SECTION cs;
    stImageData      data;
} stImageInfo;

// カメラ情報
typedef struct _stCameraInfoData
{
    BOOL    valid;                              // カメラ状態
    double  cycleTime;                          // 画像取込み間隔[ms]
} stCameraInfoData;
typedef struct _stCameraInfo
{
    CRITICAL_SECTION    cs;
    stCameraInfoData    data;
} stCameraInfo;

// RIO情報
typedef struct _stInclinometerData              // 傾斜計データ
{
    int16_t dig;
    double  cur;                                // 入力データ変換値(mA)
    double  deg;                                // 入力データ変換値(deg.)
} stInclinometerData;
typedef struct _stRIOInfoData                   // RIOデータ
{
    int32_t             error;                  // エラー情報
    stInclinometerData  incldata[AXIS_MAX];     // 傾斜計データ
} stRIOInfoData;
typedef struct _stRIOInfo
{
    CRITICAL_SECTION    cs;
    stRIOInfoData       data;
} stRIOInfo;

// 検出情報
typedef struct _stImageProcData
{
    double      maxv;                           // 最大輝度
    double      posx;                           // 検出位置X
    double      posy;                           // 検出位置Y
    int         tgtsize;                        // 検出サイズ
    cv::Rect    roi;                            // ROI
    BOOL        valid;                          // 検出状態
} stImageProcData;
typedef struct _stSwayData                      // 振れ検出データ
{
    double  pos;                                // 振れ位置
    double  deg;                                // 振れ角[deg]
    double  rad;                                // 振れ角[rad]
    double  spd;                                // 振れ角速度
} stSwayData;
typedef struct _stProcInfoData
{
    stImageProcData imgprocdata[IMGPROC_ID_MAX];    // 画像処理結果
    stSwayData      swaydata[AXIS_MAX];             // 振れ検出データ 
    BOOL            valid;                          // 検出状態
    double          imgvalue ;                      // 明度
    int             expmode;                        // 自動露光コマンド(0:停止 1:Up -1:Down)
    double          exptime;                        // 露光時間[us]
    double          proctime;                       // 処理時間[ms]
} stProcInfoData;
typedef struct _stProcInfo
{
    CRITICAL_SECTION    cs;
    stProcInfoData      data;
} stProcInfo;

// 外部入力
typedef struct _stExtnInfoData                      // 外部入力データ
{
    double  ropelen;                                // ロープ長
    double  boxangle[AXIS_MAX];                     // BOX傾き
} stExtnInfoData;
typedef struct _stExtnInfo
{
    CRITICAL_SECTION    cs;
    stExtnInfoData      data;                       // 外部入力データ
} stExtnInfo;

// 外部入出力
#define ETHER_RECV_WORD_MAX     3                   // Number of Word (receive data)
#define ETHER_SEND_WORD_MAX     5                   // Number of Word (send    data)
#pragma pack(push, 1)                               // Set 1 byte boundary
typedef struct _stEtherRecvData                     // Network reveive data
{
    short word[ETHER_RECV_WORD_MAX];                // Receive data
} stEtherRecvData;
typedef struct _stEtherSendData                     // Network send data
{
    short word[ETHER_SEND_WORD_MAX];                // Send data
} stEtherSendData;
#pragma pack(pop)

// 受信データ情報
typedef struct _stEtherRecvInfo
{
    CRITICAL_SECTION    cs;
    stEtherRecvData     data;
} stEtherRecvInfo;

// 送信データ情報
typedef struct _stEtherSendInfo
{
    CRITICAL_SECTION    cs;
    stEtherSendData     data;
} stEtherSendInfo;

#endif

//////////////////////////////////////////////////////////////////////////////
// CShared

class CShared
{
public:
    CShared();
    ~CShared();

    BOOL set_appconfig(void);
    BOOL get_appconfig(PCONFIG_COMMON data);
    BOOL get_appconfig(PCONFIG_TASK data);
    BOOL get_appconfig(PCONFIG_ETHERNET data);
    BOOL get_appconfig(PCONFIG_CAMERA data);
    BOOL get_appconfig(PCONFIG_MOUNTING data);
    BOOL get_appconfig(PCONFIG_INCLINOMETER data);
    BOOL get_appconfig(PCONFIG_IMGPROC data);

protected:

private:
    WCHAR             m_path[_MAX_PATH];    // アプリケーション.iniファイルのパス
    static APP_CONFIG m_appcnfig;
};
