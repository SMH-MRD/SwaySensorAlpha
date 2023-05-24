#include "CPolicy.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////
// CPolicy

CONFIG_COMMON    CPolicy::m_cnfgcmn;        // 共通設定
CONFIG_CAMERA    CPolicy::m_cnfgcam;        // カメラ設定
CONFIG_MOUNTING  CPolicy::m_cnfgmnt;        // 取付寸法設定
CONFIG_IMGPROC   CPolicy::m_cnfgprc;        // 画像処理条件設定
INFO_TILTMT_DATA CPolicy::m_infotlt_data;   // 傾斜計情報データ
INFO_CLIENT_DATA CPolicy::m_infoclt_data;   // クライアント情報データ
INFO_ADJUST_DATA CPolicy::m_infoajs_data;   // 調整情報データ
INFO_IMGPRC_DATA CPolicy::m_infoprc_data;   // 画像処理情報データ

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CPolicy::CPolicy()
{
    m_shared      = NULL;   // CSharedクラス
    m_environment = NULL;   // CEnvironmentクラス
}

/// @brief
/// @param
/// @return
/// @note
CPolicy::~CPolicy()
{
    //----------------------------------------------------------------------------
    // 終了処理
    close();
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief
/// @param
/// @return
/// @note
void CPolicy::init_task(void* param)
{
    CPolicy* myclass = reinterpret_cast<CPolicy*>(param);   // 自クラスインスタンスポインタ

    //----------------------------------------------------------------------------
    // 初期化処理
    if (myclass != NULL) {
        std::wstring msg = std::format(L"<Information>Called init_task()");
        myclass->outp_msg_to_listview(msg);
        if (FAILED(myclass->initialize())) {
            return ;
        }
    }

    return ;
}

/// @brief
/// @param
/// @return
/// @note
void CPolicy::routine_work(void* param)
{
    CPolicy*      myclass = reinterpret_cast<CPolicy*>(param);  // 自クラスインスタンスポインタ
    LARGE_INTEGER frequency;                // システムの周波数
    LARGE_INTEGER start_count, end_count;   // 現在のカウント数
    LONGLONG      span_usec;                // 時間の間隔[usec]

    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CPolicy::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CPolicy::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_environment == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CPolicy::routine_work]<Error>Object pointer");
        return ;
    }

    //----------------------------------------------------------------------------
    // 処理時間計測(開始時間取得)
    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&start_count);  // 現在のカウント数

    //----------------------------------------------------------------------------
    // 画像処理
    myclass->proc_main();

    //----------------------------------------------------------------------------
    // 処理時間計測(終了時間取得)
    QueryPerformanceCounter(&end_count);    // 現在のカウント数
    span_usec = ((end_count.QuadPart - start_count.QuadPart) * 1000000L) / frequency.QuadPart; // 時間の間隔[usec]

    //----------------------------------------------------------------------------
    // メッセージ表示
    std::wstring str;
    str = std::format(L"実行ｶｳﾝﾀｰ:{:4d} 処理時間[ms]:{:4d}", (*(myclass->thread_info.psys_counter) % 1000),
                                                             static_cast<uint32_t>(span_usec / 1000));
    myclass->outp_msg_to_mainwnd(str);

    return ;
}

/// @brief 初期化処理
/// @param
/// @return 
/// @note
HRESULT CPolicy::initialize(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // 共有データ初期化
    // CSharedクラスオブジェクトの生成
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CPolicy::init_task]<Error>Creating of CShared");
            msg = std::format(L"<Error>Failed to create CShared");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CShared");
    outp_msg_to_listview(msg);

    // 共有データ読込み
    m_shared->get_app_config(&m_cnfgcmn);           // 共通設定
    m_shared->get_app_config(&m_cnfgcam);           // カメラ設定
    m_shared->get_app_config(&m_cnfgmnt);           // 取付寸法設定
    m_shared->get_app_config(&m_cnfgprc);           // 画像処理条件設定
    m_shared->get_app_info_data(&m_infotlt_data);   // 傾斜計情報データ
    m_shared->get_app_info_data(&m_infoclt_data);   // クライアント情報データ
    m_shared->get_app_info_data(&m_infoajs_data);   // 調整情報データ
    m_shared->get_app_info_data(&m_infoprc_data);   // 画像処理情報データ

    // 共有データ初期化
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        m_infoprc_data.target_data[idx].valid   = FALSE;    // 検出状態
        m_infoprc_data.target_data[idx].max_val = 0.0;      // 最大輝度
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            m_infoprc_data.target_data[idx].pos[axis] = 0.0;  // 検出位置[pixel]
        }
        m_infoprc_data.target_data[idx].size       = 0; // 検出サイズ
        m_infoprc_data.target_data[idx].roi.x      = 0; // ROI:x coordinate of the top-left corner
        m_infoprc_data.target_data[idx].roi.y      = 0; // ROI:y coordinate of the top-left corner
        m_infoprc_data.target_data[idx].roi.width  = 0; // ROI:width of the rectangle
        m_infoprc_data.target_data[idx].roi.height = 0; // ROI:height of the rectangle
    }
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // ターゲット位置[pixel]
        m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
        m_infoprc_data.sway_data[axis].sway_angle  = 0.0;   // 振れ角[pixel]
        m_infoprc_data.sway_data[axis].sway_speed  = 0.0;   // 振れ速度[pixel/s]
        m_infoprc_data.sway_data[axis].sway_zero   = 0.0;   // 振れ中心[pixel]
    }
    m_infoprc_data.target_size = 0.0;                                                   // ターゲットサイズ(ターゲット検出データの平均)
    m_infoprc_data.status      = static_cast<uint32_t>(ENUM_PROCCESS_STATUS::DEFAULT);  // 検出状態
    m_infoprc_data.img_fps     = 0.0;                                                   // フレームレート
    m_infoprc_data.img_val     = 0.0 ;                                                  // 明度
    m_infoprc_data.exps_mode   = EXPOSURE_CONTROL_HOLD;                                 // 自動露光コマンド(0:停止 1:Up -1:Down)
    m_infoprc_data.exps_time   = m_cnfgcam.expstime.val;                                // 露光時間[us]
    //@@@ファイルから読むようにする
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_infoprc_data.sway_data[axis].sway_zero = (m_cnfgcam.basis.roi[axis].offset
                                                 +  m_cnfgcam.basis.roi[axis].size) * 0.5;  //振れ中心[pixel]
    }

    // 共有データ書込み
    m_shared->set_app_info_data(m_infoprc_data);    // 画像処理情報データ

    //----------------------------------------------------------------------------
    // メンバー変数の初期化
    // 輝度移動平均データ初期化
    PMOVE_AVERAGE_DATA move_avrg_data = &m_move_avrg_data;
    ZeroMemory(move_avrg_data->data, sizeof(move_avrg_data->data)); // データ
    move_avrg_data->wptr       = 0;     // 書込みポインタ
    move_avrg_data->data_count = 0;     // データ数
    move_avrg_data->total_val  = 0;     // 輝度積算
    move_avrg_data->max_val    = 0.0;   // 最大輝度(移動平均後)

    // 振れ中心計測データ
    QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // 計測時間用パフォーマンスカウンター
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_sway_zero_data.sway_min[axis]  = static_cast<double>((m_cnfgcam.basis.roi[axis].offset
                                         + m_cnfgcam.basis.roi[axis].size));            // 振れ角最小値
        m_sway_zero_data.sway_max[axis]  = 0.0;                                         // 振れ角最大値
        m_sway_zero_data.sway_zero[axis] = m_infoprc_data.sway_data[axis].sway_zero;    // 振れゼロ点
    }

    // 
    QueryPerformanceCounter(&m_cycle_time_counter); // パフォーマンスカウンター現在値
    m_cycle_time = thread_info.cycle_ms;            // 実行周期[ms]

    //----------------------------------------------------------------------------
    // CEnvironmentクラスオブジェクトの生成
    if (m_environment == NULL) {
        if ((m_environment = (CEnvironment*)VectpCTaskObj[g_task_index.environment]) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CPolicy::init_task]<Error>Creating of CEnvironment");
            msg = std::format(L"<Error>Failed to create CEnvironment");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CEnvironment");
    outp_msg_to_listview(msg);

    return S_OK;
}

/// @brief 終了処理
/// @param
/// @return 
/// @note
void CPolicy::close(void)
{
    //----------------------------------------------------------------------------
    // CSharedクラスオブジェクトの削除
    if (m_shared != NULL) {
        delete m_shared;
        m_shared = NULL;
    }

    return ;
}

/// @brief メイン処理
/// @param
/// @return
/// @note
void CPolicy::proc_main(void)
{
    IMAGE_DATA img_src;
    cv::Mat    img_roi; // 切抜き画像
    cv::Mat    img_hsv;
    cv::Mat    img_hsv_bin;
    cv::Mat    img_mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
    cv::Mat    lut;
    uint32_t   width = 0;
    uint32_t   height = 0;
    uint32_t   mask_low[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];
    uint32_t   mask_upp[static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX)];
    BOOL       ret = FALSE;
    std::vector<cv::Mat> planes;

    //----------------------------------------------------------------------------
    // 実行周期計測
    LARGE_INTEGER frequency;    // システムの周波数
    LARGE_INTEGER start_count;  // 現在のカウント数
    LONGLONG      span_usec;    // 時間の間隔[usec]

    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&start_count);  // 現在のカウント数
    span_usec = ((start_count.QuadPart - m_cycle_time_counter.QuadPart) * 1000000L) / frequency.QuadPart;   //
    if ((m_cycle_time = static_cast<uint32_t>(span_usec / 1000)) <= 0) { // 実行周期[ms]
        m_cycle_time = thread_info.cycle_ms;
    }
    QueryPerformanceCounter(&m_cycle_time_counter); // パフォーマンスカウンター現在値

    //----------------------------------------------------------------------------
    // 共有データ読込み
    m_shared->get_app_config(&m_cnfgcam);           // カメラ設定
    m_shared->get_app_config(&m_cnfgprc);           // 画像処理条件設定
    m_shared->get_app_info_data(&m_infotlt_data);   // 傾斜計情報データ
    m_shared->get_app_info_data(&m_infoclt_data);   // クライアント情報データ
    m_shared->get_app_info_data(&m_infoajs_data);   // 調整情報データ

    //----------------------------------------------------------------------------
    // 画像取込み
#pragma region GET_IMAGE
    img_src = m_environment->get_opencv_image();
    (img_src.status & static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED)) ? (m_infoprc_data.status |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) :   // 画像処理状態:画像データ有効
                                                                           (m_infoprc_data.status &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))); // 画像処理状態:画像データ無効
    m_infoprc_data.img_fps = img_src.fps;   // フレームレート[fps]
#pragma endregion GET_IMAGE

    //----------------------------------------------------------------------------
    // 検出処理
#pragma region PROCESS_TAGET
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) {
        //----------------------------------------------------------------------------
        // 画像色をBGR→HSVに変換
#pragma region CONV_BGR_TO_HSV
        if (!m_cnfgprc.roi.valid) {
            cv::cvtColor(img_src.data_opencv, img_hsv, cv::COLOR_BGR2HSV);
        }
#pragma endregion CONV_BGR_TO_HSV

        //----------------------------------------------------------------------------
        // 各チャンネルごとに2値化(LUT変換)し、3チャンネル全てのANDを取り、マスク画像を作成する
#pragma region CREATE_MASK_IMAGE
        lut = cv::Mat(256, 1, CV_8UC3); // LUT
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // ターゲット検出データ

            if (!m_cnfgprc.mask[idx].valid) {
                target_data->max_val = 0.0; // 最大輝度
                continue;
            }

            // ROIの範囲(長方形)を設定する
            if (m_cnfgprc.roi.valid) {
                // * (x, y, width, height)で指定
                if (target_data->valid) {
                    // 水平方向
                    {
                        int32_t roi_size = static_cast<int32_t>(static_cast<double>(target_data->size) * m_cnfgprc.roi.scale);
                        if ((roi_size <= 0) || (roi_size > static_cast<int32_t>(img_src.width))) {
                            roi_size = img_src.width;
                        }
                        int32_t tmp_val = static_cast<int32_t>((static_cast<double>(roi_size) / 2.0) + 0.5);
                        if ((static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)]) - tmp_val) <= 0) {
                            target_data->roi.x = 0;
                        }
                        else if ((static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)]) + tmp_val) > static_cast<int32_t>(img_src.width)) {
                            target_data->roi.x = img_src.width - roi_size;
                        }
                        else {
                            target_data->roi.x = static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)]) - tmp_val;
                        }
                        target_data->roi.width = roi_size;
                    }

                    // 垂直方向
                    {
                        int32_t roi_size = static_cast<int32_t>(static_cast<double>(target_data->size) * m_cnfgprc.roi.scale);
                        if ((roi_size <= 0) || (roi_size > static_cast<int32_t>(img_src.height))) {
                            roi_size = img_src.height;
                        }
                        int32_t tmp_val = static_cast<int32_t>((static_cast<double>(roi_size) / 2.0) + 0.5);
                        if ((static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)]) - tmp_val) <= 0) {
                            target_data->roi.y = 0;
                        }
                        else if ((static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)]) + tmp_val) > static_cast<int32_t>(img_src.height)) {
                            target_data->roi.y = img_src.height - roi_size;
                        }
                        else {
                            target_data->roi.y = static_cast<int32_t>(target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)]) - tmp_val;
                        }
                        target_data->roi.height = roi_size;
                    }
                }   // if (target_data->valid)
                else {
                    target_data->roi.x      = 0;
                    target_data->roi.y      = 0;
                    target_data->roi.width  = img_src.width;
                    target_data->roi.height = img_src.height;
                }   // if (target_data->valid) else

                // 部分画像を生成
                // * 部分画像とその元画像は共通の画像データを参照するため、
                //   部分画像に変更を加えると、元画像も変更される。
                img_roi = img_src.data_opencv(target_data->roi);
                // 画像色をBGR→HSVに変換
                cv::cvtColor(img_roi, img_hsv, cv::COLOR_BGR2HSV);
            }   // if (m_cnfgprc.roi.valid > 0)
            else {
                target_data->roi.x      = 0;
                target_data->roi.y      = 0;
                target_data->roi.width  = img_src.width;
                target_data->roi.height = img_src.height;
            }   // if (m_cnfgprc.roi.valid > 0) else

            // 3チャンネルのLUT作成
            for (uint32_t i = 0; i < static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX); i++) {
                mask_low[i] = m_cnfgprc.mask[idx].hsv_l[i]; // HSVマスク判定値(下限)
                mask_upp[i] = m_cnfgprc.mask[idx].hsv_u[i]; // HSVマスク判定値(上限)
            }
            for (uint32_t i = 0; i < 256; i++) {
                for (uint32_t k = 0; k < static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX); k++) {
                    if (mask_low[k] <= mask_upp[k]) {
                        ((mask_low[k] <= i) && (i <= mask_upp[k])) ? lut.data[i*lut.step + k] = 255 :
                                                                     lut.data[i*lut.step + k] = 0;
                    }
                    else {
                        ((i <= mask_upp[k]) || (mask_low[k] <= i)) ? lut.data[i*lut.step + k] = 255 :
                                                                     lut.data[i*lut.step + k] = 0;
                    }
                }
            }

            // チャンネルごとのLUT変換(各チャンネルごとに2値化処理)
            cv::LUT(img_hsv, lut, img_hsv_bin);

            // マスク画像の作成
            cv::split(img_hsv_bin, planes); // チャンネルごとに2値化された画像をそれぞれのチャンネルに分解する
            cv::bitwise_and(planes[static_cast<uint32_t>(ENUM_HSV_MODEL::H)], planes[static_cast<uint32_t>(ENUM_HSV_MODEL::S)], img_mask[idx]);
            cv::bitwise_and(img_mask[idx], planes[static_cast<uint32_t>(ENUM_HSV_MODEL::V)], img_mask[idx]);

            // 最大輝度抽出
            cv::split(img_hsv, planes);
            cv::minMaxLoc(planes[static_cast<uint32_t>(ENUM_HSV_MODEL::V)], NULL, &target_data->max_val);
        }   // for (UINT idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++)
#pragma endregion CREATE_MASK_IMAGE

        //----------------------------------------------------------------------------
        // マスクされた画像の合成
#pragma region MASK_IMAGE_COMBINED
// 各マスク画像は個別に処理
#if 0
        cv::Mat imgMask;
//      imgMask = imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] + imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)];
        cv::add(imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)], imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)], imgMask);

//      // 出力
//      cv::Mat imgMasked;
//      imgSrc.copyTo(imgMasked, imgMask);
#endif
#pragma endregion MASK_IMAGE_COMBINED

        //----------------------------------------------------------------------------
        // ノイズ除去
        // ゴマ塩
#pragma region NOISE_CUT_1
        switch (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type) {
        case static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN):     // 中央値フィルター
            for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
                if (m_cnfgprc.mask[idx].valid) {
                    cv::medianBlur(img_mask[idx],
                                   img_mask[idx],
                                   m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);
                }
            }
            break;
        case static_cast<uint32_t>(ENUM_NOISE_FILTER1::OPENNING):   // オープニング処理(縮小→拡大)
            for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
                if (m_cnfgprc.mask[idx].valid) {
//                  cv::morphologyEx(img_mask[idx],
//                                   img_mask[idx],
//                                   MORPH_OPEN,
//                                   cv::Mat(),
//                                   cv::Point(-1,-1),
//                                   m_imgprcparam.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);
                    cv::erode(img_mask[idx],
                              img_mask[idx],
                              cv::Mat(),
                              cv::Point(-1, -1),
                              m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);    // 収縮
                    cv::dilate(img_mask[idx],
                               img_mask[idx],
                               cv::Mat(),
                               cv::Point(-1, -1),
                               m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);   // 膨張
                }
            }
            break;
        default:
            break;
        }
#pragma endregion NOISE_CUT_1

        // 穴埋め
#pragma region NOISE_CUT_2
        switch (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type) {
        case static_cast<uint32_t>(ENUM_NOISE_FILTER2::CLOSING):    // クロージング処理(拡大→縮小)
            for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
                if (m_cnfgprc.mask[idx].valid) {
//                  cv::morphologyEx(img_mask[idx],
//                                   img_mask[idx],
//                                   MORPH_CLOSE,
//                                   cv::Mat(),
//                                   cv::Point(-1,-1),
//                                   m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);
                    cv::dilate(img_mask[idx],
                               img_mask[idx],
                               cv::Mat(),
                               cv::Point(-1, -1),
                               m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);   // 膨張
                    cv::erode(img_mask[idx],
                              img_mask[idx],
                              cv::Mat(),
                              cv::Point(-1, -1),
                              m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);    // 収縮
                }
            }
            break;
        default:
            break;
        }
#pragma endregion NOISE_CUT_2

        //----------------------------------------------------------------------------
        // 画像処理
#pragma region IMAGE_PROC
        double  pos_x, pos_y;
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // ターゲット検出データ
            if (m_cnfgprc.mask[idx].valid) {
                // 輪郭抽出(一番外側の白の輪郭のみを取得)
                cv::findContours(img_mask[idx], m_infoprc_data.contours[idx], cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

                // 重心検出
                pos_x = 0.0;
                pos_y = 0.0;
                target_data->valid = proc_center_gravity(m_infoprc_data.contours[idx],
                                                         &pos_x,
                                                         &pos_y,
                                                         &target_data->size,
                                                         m_cnfgprc.imgprc); // 検出状態
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = pos_x + target_data->roi.x; // 検出位置X[pixel]
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = pos_y + target_data->roi.y; // 検出位置Y[pixel]
            }
            else {
                target_data->valid                                    = FALSE;  // 検出状態
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = 0.0;    // 検出位置X[pixel]
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = 0.0;    // 検出位置Y[pixel]
                target_data->size                                     = 0;      // 検出サイズ
            }
        }   // for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++)
#pragma endregion IMAGE_PROC
    }   // if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))
    else {
        //----------------------------------------------------------------------------
        // マスク画像を作成する
#pragma region CREATE_MASK_IMAGE
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // ターゲット検出データ
            // ROIの範囲(長方形)を設定する
            if (m_cnfgprc.roi.valid) {
                // * (x, y, width, height)で指定
                target_data->roi.x      = 0;
                target_data->roi.y      = 0;
                target_data->roi.width  = img_src.width;
                target_data->roi.height = img_src.height;

                // 部分画像を生成
                // * 部分画像とその元画像は共通の画像データを参照するため、
                //   部分画像に変更を加えると、元画像も変更される。
                img_roi = img_src.data_opencv(target_data->roi);
                // 画像色をBGR→HSVに変換
                cv::cvtColor(img_roi, img_hsv, cv::COLOR_BGR2HSV);
            }   // if (m_cnfgprc.roi.valid > 0)
            else {
                target_data->roi.x      = 0;
                target_data->roi.y      = 0;
                target_data->roi.width  = img_src.width;
                target_data->roi.height = img_src.height;
            }   // if (m_cnfgprc.roi.valid > 0) else

            img_hsv.copyTo(img_mask[idx]);
        }   // for (UINT idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++)
#pragma endregion CREATE_MASK_IMAGE

        //----------------------------------------------------------------------------
        // 画像処理
#pragma region IMAGE_PROC
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // ターゲット検出データ
            target_data->valid                                    = FALSE;  // 検出状態
            target_data->max_val                                  = 0.0;    // 最大輝度
            target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = 0.0;    // 検出位置X[pixel]
            target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = 0.0;    // 検出位置Y[pixel]
            target_data->size                                     = 0;      // 検出サイズ
            target_data->roi.x                                    = 0;      // ROI:x coordinate of the top-left corner
            target_data->roi.y                                    = 0;      // ROI:y coordinate of the top-left corner
            target_data->roi.width                                = 0;      // ROI:width of the rectangle
            target_data->roi.height                               = 0;      // ROI:height of the rectangle
        }
#pragma endregion IMAGE_PROC
    }   // if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) else
#pragma endregion PROCESS_TAGET

    //----------------------------------------------------------------------------
    // 画像保存
#pragma region PUT_IMAGE
    // マスク画像1
    if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
        m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_1),
                                    img_mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)]);
    }
    // マスク画像2
    if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
        m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_2),
                                    img_mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)]);
    }
    // 処理画像
    m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::PROCESS), img_src.data_opencv);
#pragma endregion PUT_IMAGE

    //----------------------------------------------------------------------------
    // 振れ検出処理
#pragma region SWAY_PROC
    proc_sway();
#pragma endregion SWAY_PROC

    //----------------------------------------------------------------------------
    // シャッタコントロール
#pragma region EXPOSURE_CONTROL
    set_expstime();
#pragma endregion EXPOSURE_CONTROL

    //----------------------------------------------------------------------------
    // 共有データ書込み
    m_shared->set_app_info_data(m_infoprc_data);    // 画像処理情報データ

    return ;
}

/// @brief 重心検出
/// @param
/// @return
/// @note
BOOL CPolicy::proc_center_gravity(std::vector<std::vector<cv::Point>> contours, double* outPosX, double* outPosY, int* outTgtSize, UINT sel)
{
    BOOL    ret         = FALSE;
    double  pos_x       = 0.0;
    double  pos_y       = 0.0;
    int32_t target_size = 0;

    switch (sel) {
    case static_cast<uint32_t>(ENUM_COG::MAX_CONTOUR_AREA):
#pragma region COG_ALGORITHM1
    //----------------------------------------------------------------------------
    // 重心位置算出アルゴリズム(最大輪郭面積)
        {
            BOOL     find             = FALSE;
            size_t   count            = 0;
            double   max_area         = 0;
            size_t   max_area_contour = 0;
            cv::Rect roi;
            for(size_t i = 0; i < contours.size(); i++) {
                double area = contourArea(contours.at(i));
                if(max_area < area) {
                    max_area         = area;
                    max_area_contour = i;
                    find             = TRUE;
                }
            }

            if (find) {
                count = contours.at(max_area_contour).size();
                for (size_t i = 0; i < count; i++) {
                    pos_x += contours.at(max_area_contour).at(i).x;
                    pos_y += contours.at(max_area_contour).at(i).y;
                }
                if (count > 0) {
                    pos_x      /= count;
                    pos_y      /= count;
                    roi         = cv::boundingRect(contours[max_area_contour]);
                    target_size = (roi.width > roi.height) ? (roi.width  + 1) : (roi.height + 1);

                    ret = TRUE;
                }
            }
        }
#pragma endregion COG_ALGORITHM1
        break;

    case static_cast<uint32_t>(ENUM_COG::MAX_CONTOUR_LENGTH):
#pragma region COG_ALGORITHM2
    //----------------------------------------------------------------------------
    // 重心位置算出アルゴリズム(最大輪郭長)
        {
            BOOL     find     = FALSE;
	        size_t   max_size = 0;
	        size_t   max_id   = 0;
            cv::Rect roi;
	        if (contours.size() > 0) {
	            for (size_t i = 0; i < contours.size(); i++) {
		            if (contours[i].size() > max_size) {
			            max_size = contours[i].size();
			            max_id   = i;
                        find     = TRUE;
		            }
	            }
                if (find) {
	                cv::Moments mu = moments(contours[max_id]);
                    if (mu.m00 > 0.0) {
	                    pos_x       = mu.m10 / mu.m00;
	                    pos_y       = mu.m01 / mu.m00;
                        roi         = cv::boundingRect(contours[max_id]);
                        target_size = (roi.width > roi.height) ? (roi.width  + 1) : (roi.height + 1);

                        ret = TRUE;
                    }
                }
            }
        }
#pragma endregion COG_ALGORITHM2
        break;

    default:
        break;
    }   // switch (sel)

    if (isnan(pos_x) || isnan(pos_y)) {
        pos_x = 0.0;
        pos_y = 0.0;

        ret = FALSE;
    }
    *outPosX    = pos_x;
    *outPosY    = pos_y;
    *outTgtSize = target_size;

    return ret;
}

/// @brief 振れ検出処理
/// @param
/// @return
/// @note
void CPolicy::proc_sway(void)
{
    //----------------------------------------------------------------------------
    // ターゲット検出(ターゲット検出データの中心)
    if ((m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) &&
        (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid)) {
        if ((m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) &&
            (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid)) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis]
                                                           +  m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis]) * 0.5;  // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis]
                                                           - m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis];          // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size
                                       +  m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].size) * 0.5;   // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // 状態
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = 0.0;                               // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // 状態
        }
    }
    else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
        if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis];  // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size;   // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // 状態
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = 0.0;   // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // 状態
        }
    }
    else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
        if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis];  // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size;   // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // 状態
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // ターゲット位置[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
            }
            m_infoprc_data.target_size = 0.0;   // ターゲットサイズ(ターゲット検出データの平均)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // 状態
        }
    }
    else {
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // ターゲット位置[pixel]
            m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // ターゲット傾き[pixel]
        }
        m_infoprc_data.target_size = 0.0;   // ターゲットサイズ(ターゲット検出データの平均)
        m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // 状態
    }

    //----------------------------------------------------------------------------
    // 振れ検出
    double dt = m_cycle_time * CONV_MSEC_SEC;   // 実行周期[s]
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PSWAY_DATA sway_data       = &m_infoprc_data.sway_data[axis];   // 振れ検出データ(ターゲット検出データの中心)
        double     last_sway_angle = sway_data->sway_angle;             // 振れ角(前回値)[pixel]

        if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)) {
            sway_data->sway_angle = sway_data->sway_zero - sway_data->target_pos;   // 振れ角[pixel]

            double sway_speed = (sway_data->sway_angle - last_sway_angle) / dt; // 振れ速度[pixel/s]
            sway_data->sway_speed = ((dt * sway_speed) + (m_cnfgprc.swayspdfilter * sway_data->sway_speed))
                                  / (m_cnfgprc.swayspdfilter + dt); // 振れ速度[pixel/s]
        }
        else {
            sway_data->sway_angle = 0.0;    // 振れ角[pixel]
            sway_data->sway_speed = 0.0;    // 振れ速度[pixel/s]
        }
    }

    //----------------------------------------------------------------------------
    // 振れゼロ点設定処理
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PSWAY_DATA sway_data = &m_infoprc_data.sway_data[axis];   // 振れ検出データ(ターゲット検出データの中心)
        sway_data->sway_zero = get_sway_zero(axis); // 振れ中心[pixel]
    }

    return ;
}

/// @brief 振れゼロ点設定処理
/// @param
/// @return
/// @note
double CPolicy::get_sway_zero(uint32_t idx)
{
    PSWAY_DATA    sway_data = &m_infoprc_data.sway_data[idx];   // 振れ検出データ(ターゲット検出データの中心)
    LARGE_INTEGER frequency;                                    // システムの周波数
    LARGE_INTEGER cur_count;                                    // 現在のカウント数
    LONGLONG      span_usec;                                    // 時間の間隔[usec]
    uint32_t      span_msec;                                    // 時間の間隔[msec]
    std::wstring  msg;
 
    QueryPerformanceFrequency(&frequency);  // システムの周波数
    QueryPerformanceCounter(&cur_count);    // 現在のカウント数
    span_usec = ((cur_count.QuadPart - m_sway_zero_data.time_counter.QuadPart) * 1000000L) / frequency.QuadPart;    // 時間の間隔[usec]
    span_msec = static_cast<uint32_t>(span_usec / 1000);    // 時間の間隔[msec]

    if (m_infoclt_data.command & static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1)) {
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN))) {
            m_infoprc_data.status  |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN);
            msg = std::format(L"<Information>Running sway zero setting");
            outp_msg_to_listview(msg);
        }
    }

    if (((m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN)) ||
         (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN))) &&
        !(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED))) {
        if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)) {
            if (span_msec >= 0) {
                if (span_msec < m_cnfgprc.swayzeroset_time) {
                    if (sway_data->target_pos < m_sway_zero_data.sway_min[idx]) {
                        m_sway_zero_data.sway_min[idx] = sway_data->target_pos;
                    }
                    if (m_sway_zero_data.sway_max[idx] < sway_data->target_pos) {
                        m_sway_zero_data.sway_max[idx] = sway_data->target_pos;
                    }
                }
                else {
                    m_sway_zero_data.sway_zero[idx] = (m_sway_zero_data.sway_min[idx] + m_sway_zero_data.sway_max[idx]) * 0.5;
                    m_infoprc_data.status          |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED);  // 
                    QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // 計測時間用パフォーマンスカウンター
                    msg = std::format(L"<Information>Completed sway zero setting");
                    outp_msg_to_listview(msg);
                }
            }
            else {
                QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // 計測時間用パフォーマンスカウンター
            }
        }
        else {
            m_infoprc_data.status |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED);   // 
            QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // 計測時間用パフォーマンスカウンター
            msg = std::format(L"<Error>Failed to sway zero setting");
            outp_msg_to_listview(msg);
        }
    }
    else {
        QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // 計測時間用パフォーマンスカウンター
    }

    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED)) {
        m_infoprc_data.status &= ~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN);            //
        if (!(m_infoclt_data.command & static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1))) {
            m_infoprc_data.status &= ~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN);    //
        }
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN)) &&
            !(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN))) {
            m_infoprc_data.status &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED));
        }
    }

    return m_sway_zero_data.sway_zero[idx];
}

/// @brief シャッタコントロール
/// @param
/// @return
/// @note
void CPolicy::set_expstime(void)
{
    if (!m_cnfgcmn.img_source_camera) {
        return ;
    }

    //----------------------------------------------------------------------------
    // シャッターコントロール禁止判定
    // 備考：画像入力異常またはシャッターコントロール固定で禁止
    if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) ||
        !(m_cnfgcam.expstime.auto_control)) {
        m_infoprc_data.exps_mode = EXPOSURE_CONTROL_HOLD;
        m_infoprc_data.exps_time = m_cnfgcam.expstime.val;

        ZeroMemory(m_move_avrg_data.data, sizeof(m_move_avrg_data.data));
        m_move_avrg_data.wptr       = 0;
        m_move_avrg_data.data_count = 0;
        m_move_avrg_data.total_val  = 0;
        m_move_avrg_data.max_val    = 0.0f;
    }   // if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) || !(m_cnfgcam.expstime.auto_control))
    else {
        //----------------------------------------------------------------------------
        // 輝度移動平均
#pragma region BrightnessMoveAverage
        double max_val;
        if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].max_val >
            m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].max_val) {
            max_val = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].max_val;
        }
        else {
            max_val = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].max_val;
        }
        m_infoprc_data.img_val = CHelper::moving_average(max_val, &m_move_avrg_data, MOVE_AVERAGE_COUNT);
#pragma endregion BrightnessMoveAverage

        //----------------------------------------------------------------------------
        switch (m_infoprc_data.exps_mode) {
        case EXPOSURE_CONTROL_LIGHT:    // 明るく
            if (m_infoprc_data.img_val > m_cnfgcam.expstime.auto_stop_l) {
                m_infoprc_data.exps_mode = EXPOSURE_CONTROL_HOLD;
            }
            break;

        case EXPOSURE_CONTROL_DARKEN:   // 暗く
            if (m_infoprc_data.img_val < m_cnfgcam.expstime.auto_stop_h) {
                m_infoprc_data.exps_mode = EXPOSURE_CONTROL_HOLD;
            }
            break;

        default:                        // Hold
            if (m_infoprc_data.img_val > m_cnfgcam.expstime.auto_start_h) {
                m_infoprc_data.exps_mode = EXPOSURE_CONTROL_DARKEN;
            }
            else if (m_infoprc_data.img_val < m_cnfgcam.expstime.auto_start_l) {
                m_infoprc_data.exps_mode = EXPOSURE_CONTROL_LIGHT;
            }
            else {
                ;
            }
            break;
        }   // switch (m_infoprc_data.exps_mode)

        //----------------------------------------------------------------------------
        switch (m_infoprc_data.exps_mode) {
        case EXPOSURE_CONTROL_LIGHT:    // 明るく
            if ((m_infoprc_data.exps_time += m_cnfgcam.expstime.auto_rate) > m_cnfgcam.expstime.val_max) {
                m_infoprc_data.exps_time = m_cnfgcam.expstime.val_max;
            }
            break;

        case EXPOSURE_CONTROL_DARKEN:   // 暗く
            if ((m_infoprc_data.exps_time -= m_cnfgcam.expstime.auto_rate) < m_cnfgcam.expstime.val_min) {
                m_infoprc_data.exps_time = m_cnfgcam.expstime.val_min;
            }
            break;

        default:
            break;
        }	// switch(m_infoprc_data.exps_mode)
    }   // else

    return ;
}

/// @brief タブパネルのRadio ButtonのStaticテキストを設定
/// @param
/// @return
/// @note
void CPolicy::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"ﾒﾆｭｰ1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), TRUE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ2";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ3";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ4";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ5";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), wstr.c_str()); wstr.clear();

    wstr = L"ﾒﾆｭｰ6";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), wstr.c_str()); wstr.clear();

    //----------------------------------------------------------------------------
    // 
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    //----------------------------------------------------------------------------
    // 
    set_tab_tip_txt();

    return ;
}

/// @brief タブパネルの説明用Staticテキストを設定
/// @param
/// @return
/// @note
void CPolicy::set_tab_tip_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM3), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM4), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM5), FALSE);
    EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM6), FALSE);

    //----------------------------------------------------------------------------
    // 
    switch (thread_info.panel_func_id) {
    case IDC_RADIO_TASK_FUNC1:
        wstr = L"ｶﾒﾗ0点設定";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), wstr.c_str());
        EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
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
void CPolicy::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    std::wstring str;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDC_RADIO_TASK_FUNC1:
        case IDC_RADIO_TASK_FUNC2:
        case IDC_RADIO_TASK_FUNC3:
        case IDC_RADIO_TASK_FUNC4:
        case IDC_RADIO_TASK_FUNC5:
        case IDC_RADIO_TASK_FUNC6:
            // 機能選択IDセット
            thread_info.panel_func_id = LOWORD(wp);
            set_tab_tip_txt();  // タブパネルの説明用Staticテキストを設定
        case IDC_BUTTON_TASK_ITEM1:
        case IDC_BUTTON_TASK_ITEM2:
        case IDC_BUTTON_TASK_ITEM3:
        case IDC_BUTTON_TASK_ITEM4:
        case IDC_BUTTON_TASK_ITEM5:
        case IDC_BUTTON_TASK_ITEM6:
            // 機能入力IDセット
            thread_info.panel_type_id = LOWORD(wp);
            // 各FUNCTION PB処理
            switch (thread_info.panel_func_id) {
            case IDC_RADIO_TASK_FUNC1:
                switch (thread_info.panel_type_id) {
                case IDC_BUTTON_TASK_ITEM1:
                    // 振れゼロ点設定
                    m_infoprc_data.status |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN);
                    str = std::format(L"<Information>Requested sway zero setting");
                    outp_msg_to_listview(str);
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
            break;
        default:
            break;
        }
        break;
    }

    return ;
}
