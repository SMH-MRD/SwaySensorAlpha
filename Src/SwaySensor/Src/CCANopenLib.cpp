#include "CCANopenLib.h"
#include "CHelper.h"

//////////////////////////////////////////////////////////////////////////////
// CCANopenLib

uint8_t              CCANopenLib::m_port;           // ポート
uint8_t              CCANopenLib::m_node_id;        // ノードID
CANOPEN_TILTIX_SLOPE CCANopenLib::m_tiltix_slope;   //
std::wstring         CCANopenLib::m_infomsg;        // メッセージ

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief Constructor
/// @param
/// @return
/// @note
CCANopenLib::CCANopenLib(void)
{
    m_dll_hndl = NULL;                      //

    m_EMUCCANOpenEnable         = NULL;     //
    m_EMUCCANOpenDisable        = NULL;     //
    m_EMUCWriteOD               = NULL;     //
    m_EMUCCANOpenTx             = NULL;     //
    m_EMUCCANOpenSetState       = NULL;     //
    m_EMUCCANOpenRqstState      = NULL;     //
    m_EMUCCANOpenCheckNodeState = NULL;     //
    m_EMUCCANOpenGetVer         = NULL;     //

    m_canopen_stat = static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::DEFAULT);    // CANopenステータス:デフォルト

    m_port    = 0;  // ポート
    m_node_id = 0;  // ノードID

    for (uint32_t idx = 0; idx < static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::E_MAX); idx++) {
        m_tiltix_slope.data[idx].count = 0;
        m_tiltix_slope.data[idx].val   = 0;
    }

    m_libver  = L"";    // CANopen library version
    m_infomsg = L"";    // メッセージ
}

/// @brief Destructor
/// @param
/// @return
/// @note
CCANopenLib::~CCANopenLib(void)
{
    //----------------------------------------------------------------------------
    // 終了処理
    close();
}

/// @brief 初期化処理
/// @param
/// @return
/// @note
int32_t CCANopenLib::initialize(uint8_t port, uint8_t node_id)
{
    //----------------------------------------------------------------------------
    // メンバー変数初期化
    m_cb_info.canopen_handle_sdo_abort_cb = cb_handle_sdo_abort;    // SDOアボートを処理するコールバック関数ポインタ
    m_cb_info.canopen_handle_sdo_cb       = cb_handle_sdo;          // SDO応答を処理するコールバック関数ポインタ
    m_cb_info.canopen_cob_cb              = cb_cob_handle;          // PDOとEMCYを処理するためのコールバック関数ポインタ
    m_cb_info.canopen_update_cb           = cb_update_handle;       // CANopenスタックから定期的に呼び出されるコールバック関数ポインタ
    m_cb_info.canopen_set_state_cb        = cb_set_state;           // アプリケーション層の動作状態の変更を処理するためのコールバック関数ポインタ
    m_cb_info.canopen_time_cb             = cb_time;                // すべてのTIMEメッセージを処理するコールバック関数ポインタ
    m_cb_info.canopen_sync_cb             = cb_sync;                // すべてのSYNCメッセージを処理するコールバック関数ポインタ
    m_cb_info.canopen_init_device_cb      = cb_init_device;         // 外部デバイスを一度初期化するためのコールバック関数ポインタ
    m_cb_info.canopen_can_rx_cb           = cb_can_rx;              // 受信したCANフレームを処理するコールバック関数ポインタ
    m_cb_info.canopen_can_tx_cb           = cb_can_tx;              // 送信されたCANフレームを処理するコールバック関数ポインタ

    m_canopen_stat = static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::DEFAULT);    // CANopenステータス:デフォルト

    m_port    = port;       // ポート
    m_node_id = node_id;    // ノードID

    //----------------------------------------------------------------------------
    // DLLモジュールハンドルの取得
    if (m_dll_hndl == NULL) {
        if ((m_dll_hndl = LoadLibrary(LIB_EMUC_CANOPEN)) == NULL) {
            //if ((m_dll_hndl = LoadLibrary(L"kernel32.dll")) == NULL) {
            DWORD	dwError = GetLastError();
            LPVOID lpMsgBuf;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                0, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

            std::wstring wstr = (LPCTSTR)lpMsgBuf;
            LocalFree(lpMsgBuf);

            m_canopen_stat |= static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD);   // CANopenステータス:ライブラリロード失敗
            return -1;
        }
    }
    m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD));    // CANopenステータス:ライブラリロード失敗クリア

    //----------------------------------------------------------------------------
    // DLLモジュール関数アドレスの取得
    m_EMUCCANOpenEnable         = reinterpret_cast<EMUC_CANOPEN_ENABLE>          (GetProcAddress(m_dll_hndl, "EMUCCANOpenEnable"));
    m_EMUCCANOpenDisable        = reinterpret_cast<EMUC_CANOPEN_DISABLE>         (GetProcAddress(m_dll_hndl, "EMUCCANOpenDisable"));
    m_EMUCWriteOD               = reinterpret_cast<EMUC_WRITE_OD>                (GetProcAddress(m_dll_hndl, "EMUCWriteOD"));
    m_EMUCCANOpenTx             = reinterpret_cast<EMUC_CANOPEN_TX>              (GetProcAddress(m_dll_hndl, "EMUCCANOpenTx"));
    m_EMUCCANOpenSetState       = reinterpret_cast<EMUC_CANOPEN_SET_STATE>       (GetProcAddress(m_dll_hndl, "EMUCCANOpenSetState"));
    m_EMUCCANOpenRqstState      = reinterpret_cast<EMUC_CANOPEN_RQST_STATE>      (GetProcAddress(m_dll_hndl, "EMUCCANOpenRqstState"));
    m_EMUCCANOpenCheckNodeState = reinterpret_cast<EMUC_CANOPEN_CHECK_NODE_STATE>(GetProcAddress(m_dll_hndl, "EMUCCANOpenCheckNodeState"));
    m_EMUCCANOpenGetVer         = reinterpret_cast<EMUC_CANOPEN_GET_VER>         (GetProcAddress(m_dll_hndl, "EMUCCANOpenGetVer"));

    if (!m_EMUCCANOpenEnable || !m_EMUCCANOpenDisable || !m_EMUCWriteOD || !m_EMUCCANOpenTx ||
        !m_EMUCCANOpenSetState || !m_EMUCCANOpenRqstState || !m_EMUCCANOpenCheckNodeState || !m_EMUCCANOpenGetVer) {
        m_canopen_stat |= static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS);   // CANopenステータス:モジュールアドレス異常
        return -2;
    }
    m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));    // CANopenステータス:モジュールアドレス異常

    //----------------------------------------------------------------------------
    // CANopen libraryのバージョンの取得
    char ver[VER_LEN];

    m_EMUCCANOpenGetVer(ver);
    m_libver = CHelper::conv_string(ver);   // CANopen library version

    return 0;
}

/// @brief 終了処理
/// @param
/// @return
/// @note
void CCANopenLib::close(void)
{
    if (m_dll_hndl != NULL) {
        //----------------------------------------------------------------------------
        // CANopenネットワークの無効化    
        network_disable();

        //----------------------------------------------------------------------------
        // DLLモジュールのハンドルを破棄
        FreeLibrary(m_dll_hndl);
    }

    return ;
}

/// @brief ネットワークの有効化
/// @param
/// @return
/// @note
int32_t CCANopenLib::network_enable(wchar_t* file_name)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常
     
    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE));   // CANopenステータス:ネットワーク有効
        return -1;
    }

    //----------------------------------------------------------------------------
    // ネットワークの有効化
    int32_t stat = 0;
    char    fname[_MAX_PATH];
    size_t  num;

    //wcharのCANopen.iniのファイルパスをマルチバイトに変換（wcのs to mbs）
    if (wcstombs_s(&num, fname, _MAX_PATH, file_name, _MAX_PATH - 1) != 0) {
        m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE));   // CANopenステータス:ネットワーク有効
        return -2;
    }
    //wcharのCANopen.iniのファイルパスをマルチバイトに変換（wcのs to mbs）
    // dllの"EMUCCANOpenEnable"のモジュール実行
    if ((stat = m_EMUCCANOpenEnable(fname, m_cb_info)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE));   // CANopenステータス:ネットワーク有効
        return stat;
    }
    m_canopen_stat |= static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE);  // CANopenステータス:ネットワーク有効

    return 0;
}

/// @brief ネットワークの無効化
/// @param
/// @return
/// @note
int32_t CCANopenLib::network_disable(void)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE));   // CANopenステータス:ネットワーク有効
        return -1;
    }

    //----------------------------------------------------------------------------
    // ネットワークの無効化
    m_EMUCCANOpenDisable();
    m_canopen_stat &= (~static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE));   // CANopenステータス:ネットワーク有効

    return 0;
}

/// @brief CANopenの送信フレームのセット
/// @param
/// @return
/// @note
int32_t CCANopenLib::send_frame(uint32_t cob_id, uint8_t command, uint16_t od_idx, uint8_t od_sub_idx)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // 
    CAN_TX_INFO tx_info;    //

    for (uint32_t idx = 0; idx < MAX_CAN_FRAME_DATA_LEN; idx++) {
        tx_info.buf[idx] = 0;
    }

    uint32_t idx = 0;   //

    tx_info.id         = cob_id;
    tx_info.buf[idx++] = command;
    tx_info.buf[idx++] = static_cast<uint8_t>((od_idx)      & 0x00FF);
    tx_info.buf[idx++] = static_cast<uint8_t>((od_idx >> 8) & 0x00FF);
    tx_info.buf[idx++] = od_sub_idx;
    tx_info.buf_len    = MAX_CAN_FRAME_DATA_LEN;

    //----------------------------------------------------------------------------
    // 
    int32_t stat = 0;

    if ((stat = m_EMUCCANOpenTx(m_port, tx_info)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief CANopenの送信フレームのセット
/// @param
/// @return
/// @note
int32_t CCANopenLib::send_frame(uint32_t cob_id, uint8_t command, uint16_t od_idx, uint8_t od_sub_idx, uint16_t val)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // 
    CAN_TX_INFO tx_info;    //

    for (uint32_t idx = 0; idx < MAX_CAN_FRAME_DATA_LEN; idx++) {
        tx_info.buf[idx] = 0;
    }

    uint32_t idx = 0;   //

    tx_info.id         = cob_id;
    tx_info.buf[idx++] = command;
    tx_info.buf[idx++] = static_cast<uint8_t>((od_idx)      & 0x00FF);
    tx_info.buf[idx++] = static_cast<uint8_t>((od_idx >> 8) & 0x00FF);
    tx_info.buf[idx++] = od_sub_idx;
    tx_info.buf[idx++] = static_cast<uint8_t>((val)      & 0x00FF);
    tx_info.buf[idx++] = static_cast<uint8_t>((val >> 8) & 0x00FF);
    tx_info.buf_len    = MAX_CAN_FRAME_DATA_LEN;

    //----------------------------------------------------------------------------
    // 
    int32_t stat = 0;

    if ((stat = m_EMUCCANOpenTx(m_port, tx_info)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief オブジェクトディクショナリアドレス指定インデックスおよびサブインデックスのデータ値の変更
/// @param
/// @return
/// @note
int32_t CCANopenLib::write_od(uint16_t index, uint8_t sub_index, uint8_t* buf, uint8_t buf_len)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // 
    int32_t stat = 0;

    if ((stat = m_EMUCWriteOD(m_port, index, sub_index, buf, buf_len)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief NMT通信オブジェクトの送信
/// @param
/// @return
/// @note
int32_t CCANopenLib::set_state(CANOPEN_NMT_COMMAND command)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // NMT通信オブジェクトの送信
    int32_t stat = 0;

    if ((stat = m_EMUCCANOpenSetState(m_port, m_node_id,
                                      static_cast<uint8_t>(command))) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief RTR CAN SIDフレームの送信
/// @param
/// @return
/// @note
int32_t CCANopenLib::request_state(void)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // RTR CAN SIDフレームの送信
    int32_t stat = 0;

    if ((stat = m_EMUCCANOpenRqstState(m_port, m_node_id)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief Temp
/// @param
/// @return
/// @note
int32_t CCANopenLib::check_node_state(NSD_INFO* nsd_info)
{
    uint32_t err_stat = (static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_LIBLOAD) |   // CANopenステータス:ライブラリロード失敗
                         static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::ERR_MODADRS));   // CANopenステータス:モジュールアドレス異常

    if (m_canopen_stat & err_stat) {    // CANopenステータス:異常
        return -1;
    }

    if (!(m_canopen_stat & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {  // CANopenステータス:ネットワーク有効
        return -2;
    }

    //----------------------------------------------------------------------------
    // 
    int32_t stat = 0;

    if ((stat = m_EMUCCANOpenCheckNodeState(m_port, m_node_id, nsd_info)) != static_cast<uint32_t>(ENUM_EMUC_CANOPEN_API::SUCCESS)) {
        return stat;
    }

    return 0;
}

/// @brief CANopenステータスの取得
/// @param
/// @return
/// @note
int32_t CCANopenLib::get_canopen_stat(void)
{
    return m_canopen_stat;
}

/// @brief APIライブラリのバージョンの取得
/// @param
/// @return
/// @note
std::wstring CCANopenLib::get_libver(void)
{
    return std::format(L"CANopen library ver.{:s}", m_libver);
}

/// @brief メッセージの取得
/// @param
/// @return
/// @note
std::wstring CCANopenLib::get_infomsg(void)
{
    return m_infomsg;
}

/// @brief 傾斜計データ情報の取得
/// @param
/// @return
/// @note
CANOPEN_TILTIX_SLOPE CCANopenLib::get_tiltix_slope(void)
{
    return m_tiltix_slope;
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief SDOアボートを処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_handle_sdo_abort(uint8_t port, uint8_t node_id, uint16_t index, uint8_t sub_index, uint8_t cs, uint32_t abort_code)
{
    return ;
}

/// @brief  SDO応答を処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_handle_sdo(uint8_t port, uint8_t node_id, uint16_t index, uint8_t sub_index, uint8_t* buf, uint8_t buf_len)
{
    return ;
}

/// @brief PDOとEMCYを処理するためのコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_cob_handle(uint8_t port, uint8_t fcode, uint8_t src, uint8_t* buf, uint8_t buf_len)
{
    return ;
}

/// @brief CANopenスタックから定期的に呼び出されるコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_update_handle(int port)
{
    return ;
}

/// @brief アプリケーション層の動作状態の変更を処理するためのコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_set_state(uint8_t port, uint8_t state)
{
    return ;
}

/// @brief すべてのTIMEメッセージを処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_time(uint8_t port, uint32_t milliseconds, uint16_t days)
{
    return ;
}

/// @brief すべてのSYNCメッセージを処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_sync(uint8_t port, uint8_t counter)
{
    return ;
}

/// @brief 外部デバイスを一度初期化するためのコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_init_device(uint8_t port)
{
    return ;
}

/// @brief 受信したCANフレームを処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_can_rx(uint8_t port, uint32_t id, uint8_t* buf, uint8_t buf_len)
{
    uint32_t cobid = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_TX) + m_node_id;  // COB-ID(Com Object ID)

    if (id == cobid) {
        //----------------------------------------------------------------------------
        // TILTIX Slope Data
        uint8_t  command;
        uint16_t od_idx;
        uint8_t  od_sub_idx;
        uint32_t idx = 0;

        command    = buf[idx++];
        od_idx     = ((buf[idx++])      & 0x00FF);
        od_idx    += ((buf[idx++] << 8) & 0xFF00);
        od_sub_idx = buf[idx++];
        if ((od_idx     == static_cast<uint16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LONG16)) &&
            (od_sub_idx == static_cast<uint8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) {
            int16_t val = 0;
            for (uint32_t count = 0; count < sizeof(uint16_t); count++) {//2byteデータをwordに合成
                val += (static_cast<int16_t>(buf[idx++]) << (8 * count)) & (0x00FF << (8 * count));
            }
            m_tiltix_slope.data[static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::X)].val = val; //受信データ
            m_tiltix_slope.data[static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::X)].count++;   //受信カウンタ
        }
        else if ((od_idx     == static_cast<uint16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LATERAL16)) &&
                 (od_sub_idx == static_cast<uint8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) {
            int16_t val = 0;
            for (uint32_t count = 0; count < sizeof(uint16_t); count++) {
                val += (static_cast<int16_t>(buf[idx++]) << (8 * count)) & (0x00FF << (8 * count));
            }
            m_tiltix_slope.data[static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::Y)].val = val;
            m_tiltix_slope.data[static_cast<uint32_t>(CANOPEN_TILTIX_SLOPE_AXIS::Y)].count++;
        }
        else {
            ;
        }
    }

    return ;
}

/// @brief 送信されたCANフレームを処理するコールバック関数
/// @param
/// @return
/// @note
void CCANopenLib::cb_can_tx(uint8_t port, uint32_t id, uint8_t* buf, uint8_t buf_len)
{
    return ;
}
