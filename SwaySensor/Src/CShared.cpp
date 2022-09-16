#include "CShared.h"

APP_CONFIG CShared::m_appcnfig;

/// @brief
/// @param
/// @return
/// @note
CShared::CShared()
{
}

/// @brief
/// @param
/// @return
/// @note
CShared::~CShared()
{
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::set_appconfig(void)
{
    //----------------------------------------------------------------------------
    // ini file path設定
    WCHAR path[_MAX_PATH], szDrive[_MAX_DRIVE], szPath[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    // exe failのpathを取得
    GetModuleFileName(NULL, path, sizeof(path) / sizeof(*path));
    // 取得したpathを分割
    _wsplitpath_s(path,
                  szDrive, sizeof(szDrive) / sizeof(*szDrive),
                  szPath,  sizeof(szPath) / sizeof(*szPath),
                  szFName, sizeof(szFName) / sizeof(*szFName),
                  szExt,   sizeof(szExt) / sizeof(*szExt));
    // フォルダのパスとiniファイルのパスに合成
    swprintf_s(path, sizeof(path) / sizeof(*path),
               L"%s%s%s%s.%s",
               szDrive,
               szPath,
               FOLDER_OF_INIFILE,
               szFName,
               EXT_OF_INIFILE);
    memcpy(m_path, path, sizeof(m_path));

    //----------------------------------------------------------------------------
    WCHAR  str[256] = {0};
	WCHAR* endp;

    //----------------------------------------------------------------------------
    // 共通設定
    {
        PCONFIG_COMMON common = &m_appcnfig.common;

        // メインスレッド実行周期
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_SYSTEM_CYCLE_TIME,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->cycletime_sys = wcstoul(str, &endp, 0);
        }

        // 画像取込み元
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_IMAGE_SOURCE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->img_source = wcstoul(str, &endp, 0);
        }

        // 取込み画像ファイル名
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_IMAGE_FILENAME_SOURCE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->imgfname_source = str;
        }

        // 画像保存ファイル名
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_IMAGE_FILENAME_OUTPUT,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->imgfname_output = str;
        }
    }

    //----------------------------------------------------------------------------
    // タスク設定
    {
        PCONFIG_TASK_DATA task;

        // Environment
        task = &m_appcnfig.task.environment;
        if (GetPrivateProfileString(INI_SCT_TASK, INI_KEY_TASK_ENVIRONMENT,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            unsigned int cycletime  = 0;
            WCHAR        name[256]  = {0};
            WCHAR        sname[256] = {0};
            if (INI_KEY_TASK_COLUMN_NUM != swscanf_s(str,
                                                     L"%lu,%[^,],%[^,]",
                                                     &cycletime,
                                                     name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                     sname, (unsigned int)(sizeof(sname) / sizeof(*sname)))) {
                return FALSE;
            }
            else {
                task->cycletime = cycletime;
                task->name      = name;
                task->sname     = sname;
            }
        }

        // Policy
        task = &m_appcnfig.task.policy;
        if (GetPrivateProfileString(INI_SCT_TASK, INI_KEY_TASK_POLICY,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            unsigned int cycletime  = 0;
            WCHAR        name[256]  = {0};
            WCHAR        sname[256] = {0};
            if (INI_KEY_TASK_COLUMN_NUM != swscanf_s(str,
                                                     L"%lu,%[^,],%[^,]",
                                                     &cycletime,
                                                     name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                     sname, (unsigned int)(sizeof(sname) / sizeof(*sname)))) {
                return FALSE;
            }
            else {
                task->cycletime = cycletime;
                task->name      = name;
                task->sname     = sname;
            }
        }

        // Agent
        task = &m_appcnfig.task.agent;
        if (GetPrivateProfileString(INI_SCT_TASK, INI_KEY_TASK_AGENT,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            unsigned int cycletime  = 0;
            WCHAR        name[256]  = {0};
            WCHAR        sname[256] = {0};
            if (INI_KEY_TASK_COLUMN_NUM != swscanf_s(str,
                                                     L"%lu,%[^,],%[^,]",
                                                     &cycletime,
                                                     name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                     sname, (unsigned int)(sizeof(sname) / sizeof(*sname)))) {
                return FALSE;
            }
            else {
                task->cycletime = cycletime;
                task->name      = name;
                task->sname     = sname;
            }
        }

        // Scada
        task = &m_appcnfig.task.scada;
        if (GetPrivateProfileString(INI_SCT_TASK, INI_KEY_TASK_ENVIRONMENT,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            unsigned int cycletime  = 0;
            WCHAR        name[256]  = {0};
            WCHAR        sname[256] = {0};
            if (INI_KEY_TASK_COLUMN_NUM != swscanf_s(str,
                                                     L"%lu,%[^,],%[^,]",
                                                     &cycletime,
                                                     name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                     sname, (unsigned int)(sizeof(sname) / sizeof(*sname)))) {
                return FALSE;
            }
            else {
                task->cycletime = cycletime;
                task->name      = name;
                task->sname     = sname;
            }
        }
    }

    //----------------------------------------------------------------------------
    // 通信設定
    {
        PCONFIG_ETHERNET_DATA ethernet;

        // Local設定
        ethernet = &m_appcnfig.ethernet.local;
        if (GetPrivateProfileString(INI_SCT_ETHERNET, INI_KEY_ETHERNET_LOCAL,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            WCHAR        ipaddress[256] = {0};
            unsigned int port = 0;
            if (INI_KEY_ETHERNET_COLUMN_NUM != swscanf_s(str,
                                                         L"%[^,],%lu",
                                                         ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                         &port)) {
                return FALSE;
            }
            else {
                ethernet->ipaddress = ipaddress;
                ethernet->port      = port;
            }
        }

        // Remote設定
        ethernet = &m_appcnfig.ethernet.remote;
        if (GetPrivateProfileString(INI_SCT_ETHERNET, INI_KEY_ETHERNET_REMOTE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            WCHAR        ipaddress[256] = {0};
            unsigned int port = 0;
            if (INI_KEY_ETHERNET_COLUMN_NUM != swscanf_s(str,
                                                         L"%[^,],%lu",
                                                         ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                         &port)) {
                return FALSE;
            }
            else {
                ethernet->ipaddress = ipaddress;
                ethernet->port      = port;
            }
        }
    }

    //----------------------------------------------------------------------------
    // カメラ設定
    {
        // カメラの基本設定
        PCONFIG_CAMERA_BASIS basis = &m_appcnfig.camera.basis;
        // IPアドレス
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_IP_ADDRESS,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->ipaddress = str;
        }
        // ROI(領域)
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_ROI,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_BASIS_ROI_COLUMN_NUM != swscanf_s(str,
                                                                 L"%lu,%lu,%lu,%lu",
                                                                 &basis->roi[AXIS_X].offset, &basis->roi[AXIS_Y].offset,
                                                                 &basis->roi[AXIS_X].size, &basis->roi[AXIS_Y].size)) {
                return FALSE;
            }
        }
        // ドライバが受け取るパケットの最大サイズ
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_PACKET_SIZE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->packetsize = wcstoul(str, &endp, 0);
        }
        // フレームレート
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_FRAME_RATE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->framerate = wcstold(str, &endp);
        }
        // 黒レベル設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_BLACKLEVEL,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->blacklevel = wcstold(str, &endp);
        }
        // ガンマ補正
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_GAMMA,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->gamma = wcstold(str, &endp);
        }
        // ホワイトバランスゲイン設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_WBALANCE_AUTO,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_COLUMN_NUM != swscanf_s(str,
                                                                           L"%lu,%lf,%lf",
                                                                           &basis->wb.wb_auto, &basis->wb.wb_ratio_red, &basis->wb.wb_ratio_blue)) {
                return FALSE;
            }
        }
        // 視野角
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_VIEW_ANGLE,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_BASIS_VIEW_ANGLE_COLUMN_NUM != swscanf_s(str,
                                                                        L"%lf,%lf",
                                                                        &basis->viewangle[AXIS_X], &basis->viewangle[AXIS_Y])) {
                return FALSE;
            }
        }

        // 輝度コントロール設定(ゲイン)
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL gain = &m_appcnfig.camera.gain;
        // ゲイン設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_GAIN,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_GAIN_COLUMN_NUM != swscanf_s(str,
                                                            L"%lf,%lf,%lf",
                                                            &gain->val, &gain->minval, &gain->maxval)) {
                return FALSE;
            }
        }
        // 自動ゲイン設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_GAIN_AUTO,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_GAIN_AUTO_COLUMN_NUM != swscanf_s(str,
                                                                 L"%lu,%lf",
                                                                 &gain->atcontrol, &gain->atrate)) {
                return FALSE;
            }
        }
        // 自動ゲイン判定輝度(開始上限)
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_GAIN_AUTO_START,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                           L"%lf,%lf",
                                                                           &gain->atstart_h, &gain->atstart_l)) {
                return FALSE;
            }
        }
        // 自動ゲイン判定輝度(停止上限)
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_GAIN_AUTO_STOP,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                           L"%lf,%lf",
                                                                           &gain->atstop_h, &gain->atstop_l)) {
                return FALSE;
            }
        }

        // 輝度コントロール設定(露光時間)
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL expstime = &m_appcnfig.camera.expstime;
        // 露光時間設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_EXPSTIME,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf,%lf",
                                                                &expstime->val, &expstime->minval, &expstime->maxval)) {
                return FALSE;
            }
        }
        // 自動露光設定
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_EXPSTIME_AUTO,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_AUTO_COLUMN_NUM != swscanf_s(str,
                                                                     L"%lu,%lf",
                                                                     &expstime->atcontrol, &expstime->atrate)) {
                return FALSE;
            }
        }
        // 自動露光判定輝度(開始上限)
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_EXPSTIME_AUTO_START,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                           L"%lf,%lf",
                                                                           &expstime->atstart_h, &expstime->atstart_l)) {
                return FALSE;
            }
        }
        // 自動露光判定輝度(停止上限)
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_EXPSTIME_AUTO_STOP,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_EXPSTIME_AUTO_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                           L"%lf,%lf",
                                                                           &expstime->atstop_h, &expstime->atstop_l)) {
                return FALSE;
            }
        }

        // カメラの異常判定設定
        PCONFIG_CAMERA_ERROR err = &m_appcnfig.camera.error;
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_ERROR,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_BOOT_COLUMN_NUM != swscanf_s(str,
                                                            L"%lf,%lu",
                                                            &err->framedrop, &err->errtime)) {
                return FALSE;
            }
        }

        // カメラの起動設定
        PCONFIG_CAMERA_BOOT boot = &m_appcnfig.camera.boot;
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BOOT,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_CAMERA_BOOT_COLUMN_NUM != swscanf_s(str,
                                                            L"%lu,%lu,%lu",
                                                            &boot->bootwaittimer, &boot->rebootcount, &boot->rebootingtimer)) {
                return FALSE;
            }
        }
    }

    //----------------------------------------------------------------------------
    // 取付寸法設定
    {
        PCONFIG_MOUNTING mounting = &m_appcnfig.mounting;
        // 吊具吊点～BOX吊点距離 LX0
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_LX0,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].lx0, &mounting->offset[AXIS_Y].lx0)) {
                return FALSE;
            }
        }
        // 吊具吊点～BOX吊点距離 LY0
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_LY0,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].ly0, &mounting->offset[AXIS_Y].ly0)) {
                return FALSE;
            }
        }
        // BOX吊点～BOX可動部中心距離 L0
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_L0,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].l0, &mounting->offset[AXIS_Y].l0)) {
                return FALSE;
            }
        }
        // BOX可動部中心～カメラ中心距離 LC
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_LC,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].lc, &mounting->offset[AXIS_Y].lc)) {
                return FALSE;
            }
        }
        // BOX内カメラ取付角度 θ0
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_A0,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].a0, &mounting->offset[AXIS_Y].a0)) {
                return FALSE;
            }
        }
        // BOX可動部中心～カメラ中心角度 θc
        if (GetPrivateProfileString(INI_SCT_MOUNTING, INI_KEY_MOUNTING_OFFSET_AC,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_MOUNTING_OFFSET_COLUMN_NUM != swscanf_s(str,
                                                                L"%lf,%lf",
                                                                &mounting->offset[AXIS_X].ac, &mounting->offset[AXIS_Y].ac)) {
                return FALSE;
            }
        }
    }

    //----------------------------------------------------------------------------
    // 傾斜計設定
    {
        PCONFIG_INCLINOMETER inclinometer = &m_appcnfig.inclinometer;
        // デバイス設定
        if (GetPrivateProfileString(INI_SCT_INCLINOMETER, INI_KEY_INCLINOMETER_DEVICE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            WCHAR        ipaddress[256] = {0};
            unsigned int tcpport = 0;
            unsigned int slaveaddress = 0;
            unsigned int timeout = 0;    
            if (INI_KEY_INCLINOMETER_DEVICE_COLUMN_NUM != swscanf_s(str,
                                                                    L"%[^,],%lu,%lu,%lu",
                                                                    ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                                    &tcpport, &slaveaddress, &timeout)) {
                return FALSE;
            }
            else {
                inclinometer->ipaddress    = ipaddress;
                inclinometer->tcpport      = tcpport;
                inclinometer->slaveaddress = slaveaddress;
                inclinometer->timeout      = timeout;
            }
        }
        // 傾斜計データ接続ポート番号
        if (GetPrivateProfileString(INI_SCT_INCLINOMETER, INI_KEY_INCLINOMETER_PORT_NUMBER,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_INCLINOMETER_PORT_NUMBER_COLUMN_NUM != swscanf_s(str,
                                                                         L"%lu,%lu",
                                                                         &inclinometer->port[AXIS_X], &inclinometer->port[AXIS_Y])) {
                return FALSE;
            }
        }
    }

    //----------------------------------------------------------------------------
    // 画像処理条件設定
    {
        PCONFIG_IMGPROC imgproc = &m_appcnfig.imageprocess;
        // 画像ROI設定
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_ROI,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_ROI_COLUMN_NUM != swscanf_s(str,
                                                            L"%lu,%lf",
                                                            &imgproc->roi.valid, &imgproc->roi.scale)) {
                return FALSE;
            }
        }
        // マスク画像選択
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_MASK,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            unsigned int mask = wcstoul(str, &endp, 0);
            switch (mask)
            {
            case MASK_IMG_IMAGE1:
                imgproc->mask[IMGPROC_ID_IMG_1].valid = TRUE;
                imgproc->mask[IMGPROC_ID_IMG_2].valid = FALSE;
                break;
            case MASK_IMG_IMAGE2:
                imgproc->mask[IMGPROC_ID_IMG_1].valid = FALSE;
                imgproc->mask[IMGPROC_ID_IMG_2].valid = TRUE;
                break;
            default:
                imgproc->mask[IMGPROC_ID_IMG_1].valid = TRUE;
                imgproc->mask[IMGPROC_ID_IMG_2].valid = TRUE;
                break;
            }
        }
        // 画像1マスク下限
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_MASK1_LOW,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                   L"%lu,%lu,%lu",
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_l.h,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_l.s,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_l.v)) {
                return FALSE;
            }
        }
        // 画像1マスク上限
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_MASK1_UP,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                   L"%lu,%lu,%lu",
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_h.h,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_h.s,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_1].hsv_h.v)) {
                return FALSE;
            }
        }
        // 画像2マスク下限
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_MASK2_LOW,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                   L"%lu,%lu,%lu",
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_l.h,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_l.s,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_l.v)) {
                return FALSE;
            }
        }
        // 画像2マスク上限
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_MASK2_UP,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_MASK_LIMIT_COLUMN_NUM != swscanf_s(str,
                                                                   L"%lu,%lu,%lu",
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_h.h,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_h.s,
                                                                   &imgproc->mask[IMGPROC_ID_IMG_2].hsv_h.v)) {
                return FALSE;
            }
        }
        // ゴマ塩ノイズフィルタ
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_FILTER1,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_FILTER_COLUMN_NUM != swscanf_s(str,
                                                               L"%lu,%lu",
                                                               &imgproc->fliter1.type,
                                                               &imgproc->fliter1.val)) {
                return FALSE;
            }
        }
        // 穴埋めノイズフィルタ
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_FILTER2,
                                    L"-", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            if (INI_KEY_IMGPROC_FILTER_COLUMN_NUM != swscanf_s(str,
                                                               L"%lu,%lu",
                                                               &imgproc->fliter2.type,
                                                               &imgproc->fliter2.val)) {
                return FALSE;
            }
        }
        // ターゲット検出アルゴリズム
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_PROC,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            imgproc->imgproc = wcstoul(str, &endp, 0);
        }
        // 振れ速度フィルタ時定数
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_SWAY_SPEED_TC,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            imgproc->swayspdfliter = wcstold(str, &endp);
        }
    }

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_COMMON data)
{
    PCONFIG_COMMON cnfg_data = &m_appcnfig.common;      // 共通設定

    data->cycletime_sys   = cnfg_data->cycletime_sys;   // メインスレッド実行周期[msec]
    data->img_source      = cnfg_data->img_source;      // 画像取込み元(カメラ[0]/画像ファイル[1]) 
    data->imgfname_source = cnfg_data->imgfname_source; // 取込み画像ファイル名(IMAGE_SOURCE=1のときの画像)
    data->imgfname_output = cnfg_data->imgfname_output; // 画像保存ファイル名

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_TASK data)
{
    PCONFIG_TASK cnfg_data = &m_appcnfig.task;                      // タスク設定

    // Environment
    data->environment.cycletime = cnfg_data->environment.cycletime; // メインスレッド実行周期[msec]
    data->environment.name      = cnfg_data->environment.name;      // タスク名
    data->environment.sname     = cnfg_data->environment.sname;     // タスク略称
    // Policy
    data->policy.cycletime      = cnfg_data->policy.cycletime;      // メインスレッド実行周期[msec]
    data->policy.name           = cnfg_data->policy.name;           // タスク名
    data->policy.sname          = cnfg_data->policy.sname;          // タスク略称
    // Agent
    data->agent.cycletime       = cnfg_data->agent.cycletime;       // メインスレッド実行周期[msec]
    data->agent.name            = cnfg_data->agent.name;            // タスク名
    data->agent.sname           = cnfg_data->agent.sname;           // タスク略称
    // Scada
    data->scada.cycletime       = cnfg_data->scada.cycletime;       // メインスレッド実行周期[msec]
    data->scada.name            = cnfg_data->scada.name;            // タスク名
    data->scada.sname           = cnfg_data->scada.sname;           // タスク略称

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_ETHERNET data)
{
    PCONFIG_ETHERNET cnfg_data = &m_appcnfig.ethernet;      // 通信設定

    // Local
    data->local.ipaddress  = cnfg_data->local.ipaddress;    // IPアドレス
    data->local.port       = cnfg_data->local.port;         // ポート番号
    // Remote
    data->remote.ipaddress = cnfg_data->remote.ipaddress;   // IPアドレス
    data->remote.port      = cnfg_data->remote.port;        // ポート番号

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_CAMERA data)
{
    PCONFIG_CAMERA cnfg_data = &m_appcnfig.camera;                      // カメラ設定

    // カメラの基本設定
    data->basis.ipaddress = cnfg_data->basis.ipaddress;                 // IPアドレス
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->basis.roi[idx].offset = cnfg_data->basis.roi[idx].offset; // ROI(領域)の開始位置
        data->basis.roi[idx].size   = cnfg_data->basis.roi[idx].size;   // ROI(領域)のサイズ
    }
    data->basis.packetsize       = cnfg_data->basis.packetsize;         // ドライバが受け取るパケットの最大サイズ(通常は0を指定)[byte]
    data->basis.framerate        = cnfg_data->basis.framerate;          // フレームレート[fps]
    data->basis.blacklevel       = cnfg_data->basis.blacklevel;         // 黒レベル設定
    data->basis.gamma            = cnfg_data->basis.gamma;              // ガンマ補正
    data->basis.wb.wb_auto       = cnfg_data->basis.wb.wb_auto;         // ホワイトバランスゲイン自動調整モー設定(自動調整OFF[1]/連続自動調整[2]/一回のみ自動調整[3])
    data->basis.wb.wb_ratio_red  = cnfg_data->basis.wb.wb_ratio_red;    // ホワイトバランスゲイン設定(Red Gain)
    data->basis.wb.wb_ratio_blue = cnfg_data->basis.wb.wb_ratio_blue;   // ホワイトバランスゲイン設定(Blue Gain)
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->basis.viewangle[idx] = cnfg_data->basis.viewangle[idx];   // 視野角[deg]
    }

    // 輝度コントロール設定(ゲイン)
    data->gain.val       = cnfg_data->gain.val;                         // 設定値
    data->gain.minval    = cnfg_data->gain.minval;                      // 最小値
    data->gain.maxval    = cnfg_data->gain.maxval;                      // 最大値
    data->gain.atcontrol = cnfg_data->gain.atcontrol;                   // 自動コントロール(固定[0]/自動[1])
    data->gain.atrate    = cnfg_data->gain.atrate;                      // 自動コントロール変化量
    data->gain.atstart_h = cnfg_data->gain.atstart_h;                   // 自動コントロール判定輝度(開始上限:この値より輝度が高い場合、暗くする)   [0 - 255]
    data->gain.atstart_l = cnfg_data->gain.atstart_l;                   // 自動コントロール判定輝度(開始下限:この値より輝度が高い場合、明るくする) [0 - 255]
    data->gain.atstop_h  = cnfg_data->gain.atstop_h;                    // 自動コントロール判定輝度(停止上限:この値より輝度が低い場合、停止する)   [0 - 255]
    data->gain.atstop_l  = cnfg_data->gain.atstop_l;                    // 自動コントロール判定輝度(停止下限:この値より輝度が高い場合、停止する)   [0 - 255]

    // 輝度コントロール設定(露光時間)
    data->expstime.val       = cnfg_data->expstime.val;                 // 設定値
    data->expstime.minval    = cnfg_data->expstime.minval;              // 最小値
    data->expstime.maxval    = cnfg_data->expstime.maxval;              // 最大値
    data->expstime.atcontrol = cnfg_data->expstime.atcontrol;           // 自動コントロール(固定[0]/自動[1])
    data->expstime.atrate    = cnfg_data->expstime.atrate;              // 自動コントロール変化量
    data->expstime.atstart_h = cnfg_data->expstime.atstart_h;           // 自動コントロール判定輝度(開始上限:この値より輝度が高い場合、暗くする)   [0 - 255]
    data->expstime.atstart_l = cnfg_data->expstime.atstart_l;           // 自動コントロール判定輝度(開始下限:この値より輝度が高い場合、明るくする) [0 - 255]
    data->expstime.atstop_h  = cnfg_data->expstime.atstop_h;            // 自動コントロール判定輝度(停止上限:この値より輝度が低い場合、停止する)   [0 - 255]
    data->expstime.atstop_l  = cnfg_data->expstime.atstop_l;            // 自動コントロール判定輝度(停止下限:この値より輝度が高い場合、停止する)   [0 - 255]

    // カメラの異常判定設定
    data->error.framedrop = cnfg_data->error.framedrop;                 // フレームレート低下の判定値[fps]
    data->error.errtime   = cnfg_data->error.errtime;                   // 異常判定時間[msec]

    // カメラの起動設定
    data->boot.bootwaittimer  = cnfg_data->boot.bootwaittimer;          // システム起動後のカメラ起動待ち時間[msec]
    data->boot.rebootcount    = cnfg_data->boot.rebootcount;            // システム起動後のカメラ起動失敗時にカメラリを再起動する回数[count]
    data->boot.rebootingtimer = cnfg_data->boot.rebootingtimer;         // カメラ再起動中であることを通知する時間[msec]

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_MOUNTING data)
{
    PCONFIG_MOUNTING cnfg_data = &m_appcnfig.mounting;      // 取付寸法設定

    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->offset[idx].lx0 = cnfg_data->offset[idx].lx0; // 吊具吊点～BOX吊点距離LX0[mm]
        data->offset[idx].ly0 = cnfg_data->offset[idx].ly0; // 吊具吊点～BOX吊点距離LY0[mm]
        data->offset[idx].l0  = cnfg_data->offset[idx].l0;  // BOX吊点～BOX可動部中心距離L0[mm]
        data->offset[idx].lc  = cnfg_data->offset[idx].lc;  // BOX可動部中心～カメラ中心距離LC[mm]
        data->offset[idx].a0  = cnfg_data->offset[idx].a0;  // BOX内カメラ取付角度θ0[deg]
        data->offset[idx].ac  = cnfg_data->offset[idx].ac;  // BOX可動部中心～カメラ中心角度θc[deg]
    }

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_INCLINOMETER data)
{
    PCONFIG_INCLINOMETER cnfg_data = &m_appcnfig.inclinometer;  // 傾斜計設定

    data->ipaddress    = cnfg_data->ipaddress;                  // IPアドレス
    data->tcpport      = cnfg_data->tcpport;                    // TCPポート番号
    data->slaveaddress = cnfg_data->slaveaddress;               // スレーブアドレス
    data->timeout      = cnfg_data->timeout;                    // 通信タイムアウト[msec]
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->port[idx] = cnfg_data->port[idx];                 // 傾斜計データ接続ポート番号[1～8] (X, Y) 
    }

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_IMGPROC data)
{
    PCONFIG_IMGPROC cnfg_data = &m_appcnfig.imageprocess;           // 画像処理条件設定

    // ROI設定
    data->roi.valid = cnfg_data->roi.valid;                         // ROI有効設定(無効[FALSE]/有効[TRUE])
    data->roi.scale = cnfg_data->roi.scale;                         // ROIスケール(検出したターゲットに対する倍率)

    // マスク設定
    for (int idx = 0; idx < IMGPROC_ID_MAX; idx++) {
        data->mask[idx].valid   = cnfg_data->mask[idx].valid;       // マスク選択(無効[FALSE]/有効[TRUE])
        data->mask[idx].hsv_l.h = cnfg_data->mask[idx].hsv_l.h;     // HSVマスク判定値(下限)H
        data->mask[idx].hsv_l.s = cnfg_data->mask[idx].hsv_l.s;     // HSVマスク判定値(下限)S
        data->mask[idx].hsv_l.v = cnfg_data->mask[idx].hsv_l.v;     // HSVマスク判定値(下限)V
        data->mask[idx].hsv_h.h = cnfg_data->mask[idx].hsv_h.h;     // HSVマスク判定値(上限)H
        data->mask[idx].hsv_h.s = cnfg_data->mask[idx].hsv_h.s;     // HSVマスク判定値(上限)S
        data->mask[idx].hsv_h.v = cnfg_data->mask[idx].hsv_h.v;     // HSVマスク判定値(上限)V
    }

    // ゴマ塩ノイズフィルタ
    data->fliter1.type = cnfg_data->fliter1.type;                   // フィルタ種類
    data->fliter1.val  = cnfg_data->fliter1.val;                    // フィルタ値

    // 穴埋めノイズフィルタ
    data->fliter2.type = cnfg_data->fliter2.type;                   // フィルタ種類
    data->fliter2.val  = cnfg_data->fliter2.val;                    // フィルタ値

    data->imgproc       = cnfg_data->imgproc;                       // ターゲット検出アルゴリズム(0=全輪郭点, 1=最大輪郭面積, 2=最大輪郭長)
    data->swayspdfliter = cnfg_data->swayspdfliter;                 // 振れ速度フィルタ時定数

    return TRUE;
}
