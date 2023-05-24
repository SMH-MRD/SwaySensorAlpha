#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include <format>

#include "CANopen\lib_canopen.h"

//============================================================================
// define定義
#pragma region CONSTANT_DEFINITION
//----------------------------------------------------------------------------
// CANopen library
#if defined(_M_X64) || defined(__amd64__)
#define LIB_EMUC_CANOPEN L"lib_canopen_64.dll"
#else
#define LIB_EMUC_CANOPEN L"lib_canopen_32.dll"
#endif
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum定義
#pragma region ENUM_DEFINITION
//----------------------------------------------------------------------------
// EMUC CANopne定義
// CANopen API
enum class ENUM_EMUC_CANOPEN_API : uint32_t {
    SUCCESS = 0 // success
} ;

// CANopenステータス
enum class ENUM_EMUC_CANOPEN_STAT : uint32_t {
    DEFAULT     = 0x00000000,   // デフォルト
    NW_EABLE    = 0x00000001,   // ネットワーク有効
    ERR_LIBLOAD = 0x00000010,   // ライブラリロード失敗
    ERR_MODADRS = 0x00000020    // モジュールアドレス異常
} ;

//----------------------------------------------------------------------------
// TILTIX CANopen定義
// COB-ID
enum class CANOPEN_TILTIX_CAN_COB_ID : uint32_t {
    NMT     = 0x00000000,   // NMT
    SYNC    = 0x00000080,   // SYNC
    EMCY    = 0x00000080,   // EMCY + Node-ID
    PDO1    = 0x00000180,   // PDO1 (Tx) + Node-ID
    PDO2    = 0x00000280,   // PDO2 (Tx) + Node-ID
    SDO_RX  = 0x00000600,   // SDO (Rx) + Node-ID
    SDO_TX  = 0x00000580    // SDO (Tx) + Node-ID
} ;

// NMT command
enum class CANOPEN_NMT_COMMAND : uint8_t {
    OPERATIONAL     = 0x01, // Operational Mode
    STOPPED         = 0x02, // Stopped Mode
    PRE_OPERATIONAL = 0x80, // Pre-Operational Mode
    RESET_NODE      = 0x81, // Reset Node
    RESET_COMM      = 0x82  // Reset Communication
} ;

// NMT state
enum class CANOPEN_NMT_STATE : uint8_t {
    BOOT_UP         = 0x00, // 
    STOPPED         = 0x04, // 
    OPERATIONAL     = 0x05, // 
    PRE_OPERATIONAL = 0x7F, // 
    MEANINGLESS     = 0xFF  // 
} ;

// CAN command
enum class CANOPEN_TILTIX_CAN_COMMAND : uint8_t {
    DOWNLOAD_REQUEST      = 0x22,   // Parameter to TILTIX
    DOWNLOAD_CONFIRMATION = 0x60,   // Parameter Received
    UPLOAD_REQUEST        = 0x40,   // Parameter Request
    UPLOAD_REPLY_4BYTE    = 0x43,   // Parameter to Master (Data lengh:4byte)
    UPLOAD_REPLY_2BYTE    = 0x4B,   // Parameter to Master (Data lengh:2byte)
    UPLOAD_REPLY_1BYTE    = 0x4F,   // Parameter to Master (Data lengh:1byte)
    WARNING_REPLY         = 0x80    // Transmission Error
} ;

// OD index
enum class CANOPEN_TILTIX_OD_INDEX : uint16_t {
    DEVICETYPE                          = 0x1000,   // Device Type
    ERROR_REGISTER                      = 0x1001,   // Erroe Register
    PREDEFINED_ERROR_FIELD              = 0x1003,   // Pre-Defined Error Field
    COBID_SYNC                          = 0x1005,   // COB-ID SYNC
    COMMUNICATION_CYCLE_PERIOD          = 0x1006,   // Communication Cycle Period
    SYNCHRONOUS_WINDOW_LENGTH           = 0x1007,   // Synchronous Window Length
    DEVICENAME                          = 0x1008,   // Device Name
    DARDWARE_VERSION                    = 0x1009,   // Hardware Version
    SOFTWARE_VERSION                    = 0x100A,   // Software Version
    GUAR_TIME                           = 0x100C,   // Guard Time
    LIFE_TIME_FACTOR                    = 0x100D,   // Life Time Factor
    STORE_PARAMETERS_RW                 = 0x1010,   // Store Parameters RW
    RESTORE_DEFAULT_PARAMETERS          = 0x1011,   // Restore Default Parameters
    COBID_EMERGENCY                     = 0x1014,   // COB-ID Emergency
    INHIBIT_TIME_EMERGENCY              = 0x1015,   // Inhibit Time Emergency
    CONSUMER_HEARTBEAT_TIME             = 0x1016,   // Consumer Heartbeat Time
    PRODUCER_HEARTBEAT_TIME             = 0x1017,   // Producer Heartbeat Time
    IDENTITY_OBJECT                     = 0x1018,   // Identity Object
    VERIFY_CONFIGURATION                = 0x1020,   // Verify Configuration
    ERROR_BEHAVIOUR                     = 0x1029,   // Error behaviour
    TPDO1_COMM_PARAMETER                = 0x1800,   // TPDO1 Comm Parameter
    TPDO2_COMM_PARAMETER                = 0x1801,   // TPDO2 Comm Parameter
    TPDO1_MAPPING                       = 0x1A00,   // TPDO1 Mapping
    TPDO2_MAPPING                       = 0x1A01,   // TPDO2 Mapping
    CYCLE_TIME                          = 0x2200,   // Cyclic Time
    STORE_PARAMETERS_WO                 = 0x2300,   // Store Parameters WO
    PRESET_X_AXIS                       = 0x2600,   // Preset X-Axis
    PRESET_Y_AXIS                       = 0x2601,   // Preset Y-Axis
    NODE_NUMBER                         = 0x3000,   // Node number
    BAUD_RATE                           = 0x3001,   // Baud Rate
    TERMINATION_RESISTOR                = 0x3002,   // Termination Resistor
    DIGITAL_RECURSIVE_FILTER            = 0x3022,   // Digital recursive filter
    MOVING_AGERAGE_FILTER               = 0x3100,   // Moving Average Filter
    RESOLUTION                          = 0x6000,   // Resolution
    SLOPE_LONG16                        = 0x6010,   // Slope long16
    SLOPE_LONG16_OPERATING_PARAMETER    = 0x6011,   // Slope long16 operating parameter
    SLOPE_LONG16_PRESET_VALUE           = 0x6012,   // Slope long16 preset value
    SLOPE_LONG16_OFFSET                 = 0x6013,   // Slope long16 offset
    DIFFERENTIAL_SLOPE_LONG16_OFFSET    = 0x6014,   // Differential slope long16 offset
    SLOPE_LATERAL16                     = 0x6020,   // Slope lateral16
    SLOPE_LATERAL16_OPERATING_PARAMETER = 0x6021,   // Slope lateral16 operating parameter
    SLOPE_LATERAL16_PRESET_VALUE        = 0x6022,   // Slope lateral16 preset value
    SLOPE_LATERAL16_OFFSET              = 0x6023,   // Slope lateral16 offset
    DIFFERENTIAL_SLOPE_LATERAL16_OFFSET = 0x6024,   // Differential slope lateral16 offset
    SLOPE_LONG32                        = 0x6110,   // Slope long32
    SLOPE_LONG32_OPERATING_PARAMETER    = 0x6111,   // Slope long32 operating parameter
    SLOPE_LONG32_PRESET_VALUE           = 0x6112,   // Slope long32 preset value
    SLOPE_LONG32_OFFSET                 = 0x6113,   // Slope long32 offset
    DIFFERENTIAL_SLOPE_LONG32           = 0x6114,   // Differential slope long32 offset
    SLOPE_LATERAL32                     = 0x6120,   // Slope lateral32
    SLOPE_LATERAL32_OPERATING_PARAMETER = 0x6121,   // Slope lateral32 operating parameter
    SLOPE_LATERAL32_PRESET_VALUE        = 0x6122,   // Slope lateral32 preset value
    SLOPE_LATERAL32_OFFSET              = 0x6123,   // Slope lateral32 offset
    DIFFERENTIAL_SLOPE_LATERAL32        = 0x6124    // Differential slope lateral32 offset
} ;

// OD sub-index
enum class CANOPEN_TILTIX_OD_SUB_INDEX : uint8_t {
    VARIABLE                                                    = 0,    // Variable
    PREDEFINED_ERROR_FIELD_NUMBER_ERRORS                        = 0,    // Pre-Defined Error Field (Number of errors)
    PREDEFINED_ERROR_FIELD_NEW_ERROR                            = 1,    // Pre-Defined Error Field (New error)
    STORE_PARAMETERS_RW_HIGHEST_SUBINDEX_SUPPORTED              = 0,    // Store Parameters RW (Highest sub-index supported)
    STORE_PARAMETERS_RW_SAVE_ALL_PARAMETERS                     = 1,    // Store Parameters RW (Save all parameters)
    STORE_PARAMETERS_RW_SAVE_COMMUNICATION_PARAMETERS           = 2,    // Store Parameters RW (Save communication parameters)
    STORE_PARAMETERS_RW_SAVE_APPLICATION_PARAMETERS             = 3,    // Store Parameters RW (Save Application parameters)
    STORE_PARAMETERS_RW_SAVE_MANUFACTURER_PARAMETERS            = 4,    // Store Parameters RW (Save Manufacturer parameters)
    RESTORE_DEFAULT_PARAMETERS_HIGHEST_SUBINDEX_SUPPORTED       = 0,    // Restore Default Parameters (Highest sub-index supported)
    RESTORE_DEFAULT_PARAMETERS_RESTORE_ALL_PARAMETERS           = 1,    // Restore Default Parameters (Restore all parameters)
    RESTORE_DEFAULT_PARAMETERS_RESTORE_COMMUNICATION_PARAMETERS = 2,    // Restore Default Parameters (Restore communication parameters)
    RESTORE_DEFAULT_PARAMETERS_RESTORE_APPLICATION_PARAMETERS   = 3,    // Restore Default Parameters (Restore application parameters)
    RESTORE_DEFAULT_PARAMETERS_RESTORE_MANUFACTURER_PARAMETERS  = 4,    // Restore Default Parameters (Restore manufacturer parameters)
    CONSUMER_HEARTBEAT_TIME_NUMBER_ENTRIES                      = 0,    // Consumer Heartbeat Time (Number of Entries)
    CONSUMER_HEARTBEAT_TIME_CONSUMER_HEARTBEAT_TIME             = 1,    // Consumer Heartbeat Time (Consumer Heartbeat Time)
    IDENTITY_OBJECT_NUMBER_ENTRIES                              = 0,    // Identity Object (Number of Entries)
    IDENTITY_OBJECT_VENDOR_ID                                   = 1,    // Identity Object (Vendor ID)
    IDENTITY_OBJECT_PRODUCT_CODE                                = 2,    // Identity Object (Product Code)
    IDENTITY_OBJECT_REVISION_NUMBER                             = 3,    // Identity Object (Revision Number)
    IDENTITY_OBJECT_SERIAL_NUMBER                               = 4,    // Identity Object (Serial Number)
    VERIFY_CONFIGURATION_NUMBER_ENTRIES                         = 0,    // Verify Configuration (Number of Entries)
    VERIFY_CONFIGURATION_CONFIGURATION_DATE                     = 1,    // Verify Configuration (Configuration date)
    VERIFY_CONFIGURATION_CONFIGURATION_TIME                     = 2,    // Verify Configuration (Configuration time)
    ERROR_BEHAVIOUR_NUMBER_ERROR_CLASSES                        = 0,    // Error behaviour (Number of Error Classes)
    ERROR_BEHAVIOUR_COMMUNICATION_ERROR                         = 1,    // Error behaviour (Communication Error)
    TPDO1_COMM_PARAMETER_NUMBER_ENTRIES                         = 0,    // TPDO1 Comm Parameter (Number of Entries)
    TPDO1_COMM_PARAMETER_COBID                                  = 1,    // TPDO1 Comm Parameter (COB-ID)
    TPDO1_COMM_PARAMETER_TRANSMISSION_TYPE                      = 2,    // TPDO1 Comm Parameter (Transmission Type)
    TPDO1_COMM_PARAMETER_INHIBIT_TIME                           = 3,    // TPDO1 Comm Parameter (Inhibit Time)
    TPDO1_COMM_PARAMETER_EVENT_TIMER                            = 5,    // TPDO1 Comm Parameter (Event Timer)
    TPDO2_COMM_PARAMETER_NUMBER_ENTRIES                         = 0,    // TPDO2 Comm Parameter (Number of Entries)
    TPDO2_COMM_PARAMETER_COBID                                  = 1,    // TPDO2 Comm Parameter (COB-ID)
    TPDO2_COMM_PARAMETER_TRANSMISSION_TYPE                      = 2,    // TPDO2 Comm Parameter (Transmission Type)
    TPDO2_COMM_PARAMETER_INHIBIT_TIME                           = 3,    // TPDO2 Comm Parameter (Inhibit Time)
    TPDO2_COMM_PARAMETER_EVENT_TIMER                            = 5,    // TPDO2 Comm Parameter (Event Timer)
    TPDO1_MAPPING_NUMBER_ENTRIES                                = 0,    // TPDO1 Mapping (Number of entries)
    TPDO1_MAPPING_1ST_MAPPED_OBJECT                             = 1,    // TPDO1 Mapping (1st mapped Object)
    TPDO1_MAPPING_2ND_MAPPED_OBJECT                             = 2,    // TPDO1 Mapping (2nd mapped Object)
    TPDO2_MAPPING_NUMBER_ENTRIES                                = 0,    // TPDO2 Mapping (Number of entries)
    TPDO2_MAPPING_1ST_MAPPED_OBJECT                             = 1,    // TPDO2 Mapping (1st mapped Object)
    TPDO2_MAPPING_2ND_MAPPED_OBJECT                             = 2     // TPDO2 Mapping (2nd mapped Object)
} ;

// TILTIX Resolution value
enum class CANOPEN_TILTIX_RESOLUTION_VALUE : uint16_t {
    DEG_001 = 10,   // 0.01°
    DEG_01  = 100,  // 0.1°
    DEG_1   = 1000  // 1°
} ;

// TILTIX Slope axis
enum class CANOPEN_TILTIX_SLOPE_AXIS : uint32_t {
    X = 0,  //
    Y,      //
    E_MAX   //
} ;
#pragma endregion ENUM_DEFINITION

//============================================================================
// 構造体/共用体定義
#pragma region STRUCTURE_DEFINITION
//----------------------------------------------------------------------------
// 傾斜計データ情報
// 傾斜計データ
typedef struct TAG_CANOPEN_TILTIX_SLOPE_DATA {
    uint32_t count;
    int16_t  val;

    TAG_CANOPEN_TILTIX_SLOPE_DATA()
        : count(0)
        , val(0)
    {}
} CANOPEN_TILTIX_SLOPE_DATA, *PCANOPEN_TILTIX_SLOPE_DATA;
// 傾斜計データ情報
typedef struct TAG_CANOPEN_TILTIX_SLOPE {
    CANOPEN_TILTIX_SLOPE_DATA data[static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::E_MAX)];  // 傾斜計データ
} CANOPEN_TILTIX_SLOPE, *PCANOPEN_TILTIX_SLOPE;
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CCANopenLib

class CCANopenLib
{
public:
    CCANopenLib(void);
    virtual ~CCANopenLib(void);

    // メンバー変数

    // メンバー関数
    int32_t initialize(uint8_t port, uint8_t node_id);  // 初期化処理
    void close(void);                                   // 終了処理

    int32_t network_enable(wchar_t* file_name); // ネットワークの有効化
    int32_t network_disable(void);              // ネットワークの無効化
    int32_t send_frame(uint32_t cob_id,
                       uint8_t command,
                       uint16_t od_idx,
                       uint8_t od_sub_idx);     // CANopenフレームの送信
    int32_t send_frame(uint32_t cob_id,
                       uint8_t command,
                       uint16_t od_idx,
                       uint8_t od_sub_idx,
                       uint16_t val);           // CANopenフレームの送信
    int32_t write_od(uint16_t index,
                     uint8_t sub_index,
                     uint8_t* buf,
                     uint8_t buf_len);                  // オブジェクトディクショナリアドレス指定インデックスおよびサブインデックスのデータ値の変更
    int32_t set_state(CANOPEN_NMT_COMMAND command);     // NMT通信オブジェクトの送信
    int32_t request_state(void);                        // RTR CAN SIDフレームの送信
    int32_t check_node_state(NSD_INFO* nsd_info);       // Temp

    int32_t get_canopen_stat(void);                     // CANopenステータスの取得
    std::wstring get_libver(void);                      // APIライブラリのバージョンの取得
    std::wstring get_infomsg(void);                     // メッセージの取得
    CANOPEN_TILTIX_SLOPE get_tiltix_slope(void);        // 傾斜計データ情報の取得

protected:

private:
    // メンバー変数
    HMODULE m_dll_hndl;

    EMUC_CANOPEN_ENABLE           m_EMUCCANOpenEnable;
    EMUC_CANOPEN_DISABLE          m_EMUCCANOpenDisable;
    EMUC_WRITE_OD                 m_EMUCWriteOD;
    EMUC_CANOPEN_TX               m_EMUCCANOpenTx;
    EMUC_CANOPEN_SET_STATE        m_EMUCCANOpenSetState;
    EMUC_CANOPEN_RQST_STATE       m_EMUCCANOpenRqstState;
    EMUC_CANOPEN_CHECK_NODE_STATE m_EMUCCANOpenCheckNodeState;
    EMUC_CANOPEN_GET_VER          m_EMUCCANOpenGetVer;

    CANOPEN_CB_INFO m_cb_info;  // CANOPEN_CB_INFO構造体情報

    int32_t m_canopen_stat;     // CANopenステータス
 
    static uint8_t m_port;      // ポート
    static uint8_t m_node_id;   // ノードID

    static CANOPEN_TILTIX_SLOPE m_tiltix_slope;  // 傾斜計データ情報

    std::wstring        m_libver;   // CANopen library version
    static std::wstring m_infomsg;  // メッセージ

    // メンバー関数
    static void cb_handle_sdo_abort(uint8_t port,
                                    uint8_t node_id,
                                    uint16_t index,
                                    uint8_t sub_index,
                                    uint8_t cs,
                                    uint32_t abort_code);   // SDOアボートを処理するコールバック関数
    static void cb_handle_sdo(uint8_t port,
                              uint8_t node_id,
                              uint16_t index,
                              uint8_t sub_index,
                              uint8_t* buf,
                              uint8_t buf_len);             // SDO応答を処理するコールバック関数
    static void cb_cob_handle(uint8_t port,
                              uint8_t fcode,
                              uint8_t src,
                              uint8_t* buf,
                              uint8_t buf_len);             // PDOとEMCYを処理するためのコールバック関数
    static void cb_update_handle(int port);                 // CANopenスタックから定期的に呼び出されるコールバック関数
    static void cb_set_state(uint8_t port,
                             uint8_t state);                // アプリケーション層の動作状態の変更を処理するためのコールバック関数
    static void cb_time(uint8_t port,
                        uint32_t milliseconds,
                        uint16_t days);                     // すべてのTIMEメッセージを処理するコールバック関数
    static void cb_sync(uint8_t port,
                        uint8_t counter);                   // すべてのSYNCメッセージを処理するコールバック関数
    static void cb_init_device(uint8_t port);               // 外部デバイスを一度初期化するためのコールバック関数
    static void cb_can_rx(uint8_t port,
                          uint32_t id,
                          uint8_t* buf,
                          uint8_t buf_len);                 // 受信したCANフレームを処理するコールバック関数
    static void cb_can_tx(uint8_t port,
                          uint32_t id,
                          uint8_t* buf,
                          uint8_t buf_len);                 // 送信されたCANフレームを処理するコールバック関数
};
