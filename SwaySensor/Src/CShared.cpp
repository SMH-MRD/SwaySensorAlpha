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
    // ini file path�ݒ�
    WCHAR path[_MAX_PATH], szDrive[_MAX_DRIVE], szPath[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    // exe fail��path���擾
    GetModuleFileName(NULL, path, sizeof(path) / sizeof(*path));
    // �擾����path�𕪊�
    _wsplitpath_s(path,
                  szDrive, sizeof(szDrive) / sizeof(*szDrive),
                  szPath,  sizeof(szPath) / sizeof(*szPath),
                  szFName, sizeof(szFName) / sizeof(*szFName),
                  szExt,   sizeof(szExt) / sizeof(*szExt));
    // �t�H���_�̃p�X��ini�t�@�C���̃p�X�ɍ���
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
    // ���ʐݒ�
    {
        PCONFIG_COMMON common = &m_appcnfig.common;

        // ���C���X���b�h���s����
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_SYSTEM_CYCLE_TIME,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->cycletime_sys = wcstoul(str, &endp, 0);
        }

        // �摜�捞�݌�
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_IMAGE_SOURCE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->img_source = wcstoul(str, &endp, 0);
        }

        // �捞�݉摜�t�@�C����
        if (GetPrivateProfileString(INI_SCT_COMMON, INI_KEY_COMMON_IMAGE_FILENAME_SOURCE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            common->imgfname_source = str;
        }

        // �摜�ۑ��t�@�C����
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
    // �^�X�N�ݒ�
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
    // �ʐM�ݒ�
    {
        PCONFIG_ETHERNET_DATA ethernet;

        // Local�ݒ�
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

        // Remote�ݒ�
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
    // �J�����ݒ�
    {
        // �J�����̊�{�ݒ�
        PCONFIG_CAMERA_BASIS basis = &m_appcnfig.camera.basis;
        // IP�A�h���X
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_IP_ADDRESS,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->ipaddress = str;
        }
        // ROI(�̈�)
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
        // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_PACKET_SIZE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->packetsize = wcstoul(str, &endp, 0);
        }
        // �t���[�����[�g
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_FRAME_RATE,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->framerate = wcstold(str, &endp);
        }
        // �����x���ݒ�
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_BLACKLEVEL,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->blacklevel = wcstold(str, &endp);
        }
        // �K���}�␳
        if (GetPrivateProfileString(INI_SCT_CAMERA, INI_KEY_CAMERA_BASIS_GAMMA,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            basis->gamma = wcstold(str, &endp);
        }
        // �z���C�g�o�����X�Q�C���ݒ�
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
        // ����p
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

        // �P�x�R���g���[���ݒ�(�Q�C��)
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL gain = &m_appcnfig.camera.gain;
        // �Q�C���ݒ�
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
        // �����Q�C���ݒ�
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
        // �����Q�C������P�x(�J�n���)
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
        // �����Q�C������P�x(��~���)
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

        // �P�x�R���g���[���ݒ�(�I������)
        PCONFIG_CAMERA_BRIGHTNESS_CONTROL expstime = &m_appcnfig.camera.expstime;
        // �I�����Ԑݒ�
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
        // �����I���ݒ�
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
        // �����I������P�x(�J�n���)
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
        // �����I������P�x(��~���)
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

        // �J�����ُ̈픻��ݒ�
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

        // �J�����̋N���ݒ�
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
    // ��t���@�ݒ�
    {
        PCONFIG_MOUNTING mounting = &m_appcnfig.mounting;
        // �݋�ݓ_�`BOX�ݓ_���� LX0
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
        // �݋�ݓ_�`BOX�ݓ_���� LY0
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
        // BOX�ݓ_�`BOX�������S���� L0
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
        // BOX�������S�`�J�������S���� LC
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
        // BOX���J������t�p�x ��0
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
        // BOX�������S�`�J�������S�p�x ��c
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
    // �X�Όv�ݒ�
    {
        PCONFIG_INCLINOMETER inclinometer = &m_appcnfig.inclinometer;
        // �f�o�C�X�ݒ�
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
        // �X�Όv�f�[�^�ڑ��|�[�g�ԍ�
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
    // �摜���������ݒ�
    {
        PCONFIG_IMGPROC imgproc = &m_appcnfig.imageprocess;
        // �摜ROI�ݒ�
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
        // �}�X�N�摜�I��
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
        // �摜1�}�X�N����
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
        // �摜1�}�X�N���
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
        // �摜2�}�X�N����
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
        // �摜2�}�X�N���
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
        // �S�}���m�C�Y�t�B���^
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
        // �����߃m�C�Y�t�B���^
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
        // �^�[�Q�b�g���o�A���S���Y��
        if (GetPrivateProfileString(INI_SCT_IMGPROC, INI_KEY_IMGPROC_PROC,
                                    L"", str, sizeof(str) / sizeof(*str),
                                    PATH_OF_INIFILE) <= 0) {
            return FALSE;
        }
        else {
            imgproc->imgproc = wcstoul(str, &endp, 0);
        }
        // �U�ꑬ�x�t�B���^���萔
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
    PCONFIG_COMMON cnfg_data = &m_appcnfig.common;      // ���ʐݒ�

    data->cycletime_sys   = cnfg_data->cycletime_sys;   // ���C���X���b�h���s����[msec]
    data->img_source      = cnfg_data->img_source;      // �摜�捞�݌�(�J����[0]/�摜�t�@�C��[1]) 
    data->imgfname_source = cnfg_data->imgfname_source; // �捞�݉摜�t�@�C����(IMAGE_SOURCE=1�̂Ƃ��̉摜)
    data->imgfname_output = cnfg_data->imgfname_output; // �摜�ۑ��t�@�C����

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_TASK data)
{
    PCONFIG_TASK cnfg_data = &m_appcnfig.task;                      // �^�X�N�ݒ�

    // Environment
    data->environment.cycletime = cnfg_data->environment.cycletime; // ���C���X���b�h���s����[msec]
    data->environment.name      = cnfg_data->environment.name;      // �^�X�N��
    data->environment.sname     = cnfg_data->environment.sname;     // �^�X�N����
    // Policy
    data->policy.cycletime      = cnfg_data->policy.cycletime;      // ���C���X���b�h���s����[msec]
    data->policy.name           = cnfg_data->policy.name;           // �^�X�N��
    data->policy.sname          = cnfg_data->policy.sname;          // �^�X�N����
    // Agent
    data->agent.cycletime       = cnfg_data->agent.cycletime;       // ���C���X���b�h���s����[msec]
    data->agent.name            = cnfg_data->agent.name;            // �^�X�N��
    data->agent.sname           = cnfg_data->agent.sname;           // �^�X�N����
    // Scada
    data->scada.cycletime       = cnfg_data->scada.cycletime;       // ���C���X���b�h���s����[msec]
    data->scada.name            = cnfg_data->scada.name;            // �^�X�N��
    data->scada.sname           = cnfg_data->scada.sname;           // �^�X�N����

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_ETHERNET data)
{
    PCONFIG_ETHERNET cnfg_data = &m_appcnfig.ethernet;      // �ʐM�ݒ�

    // Local
    data->local.ipaddress  = cnfg_data->local.ipaddress;    // IP�A�h���X
    data->local.port       = cnfg_data->local.port;         // �|�[�g�ԍ�
    // Remote
    data->remote.ipaddress = cnfg_data->remote.ipaddress;   // IP�A�h���X
    data->remote.port      = cnfg_data->remote.port;        // �|�[�g�ԍ�

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_CAMERA data)
{
    PCONFIG_CAMERA cnfg_data = &m_appcnfig.camera;                      // �J�����ݒ�

    // �J�����̊�{�ݒ�
    data->basis.ipaddress = cnfg_data->basis.ipaddress;                 // IP�A�h���X
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->basis.roi[idx].offset = cnfg_data->basis.roi[idx].offset; // ROI(�̈�)�̊J�n�ʒu
        data->basis.roi[idx].size   = cnfg_data->basis.roi[idx].size;   // ROI(�̈�)�̃T�C�Y
    }
    data->basis.packetsize       = cnfg_data->basis.packetsize;         // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
    data->basis.framerate        = cnfg_data->basis.framerate;          // �t���[�����[�g[fps]
    data->basis.blacklevel       = cnfg_data->basis.blacklevel;         // �����x���ݒ�
    data->basis.gamma            = cnfg_data->basis.gamma;              // �K���}�␳
    data->basis.wb.wb_auto       = cnfg_data->basis.wb.wb_auto;         // �z���C�g�o�����X�Q�C�������������[�ݒ�(��������OFF[1]/�A����������[2]/���̂ݎ�������[3])
    data->basis.wb.wb_ratio_red  = cnfg_data->basis.wb.wb_ratio_red;    // �z���C�g�o�����X�Q�C���ݒ�(Red Gain)
    data->basis.wb.wb_ratio_blue = cnfg_data->basis.wb.wb_ratio_blue;   // �z���C�g�o�����X�Q�C���ݒ�(Blue Gain)
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->basis.viewangle[idx] = cnfg_data->basis.viewangle[idx];   // ����p[deg]
    }

    // �P�x�R���g���[���ݒ�(�Q�C��)
    data->gain.val       = cnfg_data->gain.val;                         // �ݒ�l
    data->gain.minval    = cnfg_data->gain.minval;                      // �ŏ��l
    data->gain.maxval    = cnfg_data->gain.maxval;                      // �ő�l
    data->gain.atcontrol = cnfg_data->gain.atcontrol;                   // �����R���g���[��(�Œ�[0]/����[1])
    data->gain.atrate    = cnfg_data->gain.atrate;                      // �����R���g���[���ω���
    data->gain.atstart_h = cnfg_data->gain.atstart_h;                   // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    data->gain.atstart_l = cnfg_data->gain.atstart_l;                   // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    data->gain.atstop_h  = cnfg_data->gain.atstop_h;                    // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    data->gain.atstop_l  = cnfg_data->gain.atstop_l;                    // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �P�x�R���g���[���ݒ�(�I������)
    data->expstime.val       = cnfg_data->expstime.val;                 // �ݒ�l
    data->expstime.minval    = cnfg_data->expstime.minval;              // �ŏ��l
    data->expstime.maxval    = cnfg_data->expstime.maxval;              // �ő�l
    data->expstime.atcontrol = cnfg_data->expstime.atcontrol;           // �����R���g���[��(�Œ�[0]/����[1])
    data->expstime.atrate    = cnfg_data->expstime.atrate;              // �����R���g���[���ω���
    data->expstime.atstart_h = cnfg_data->expstime.atstart_h;           // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    data->expstime.atstart_l = cnfg_data->expstime.atstart_l;           // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    data->expstime.atstop_h  = cnfg_data->expstime.atstop_h;            // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    data->expstime.atstop_l  = cnfg_data->expstime.atstop_l;            // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �J�����ُ̈픻��ݒ�
    data->error.framedrop = cnfg_data->error.framedrop;                 // �t���[�����[�g�ቺ�̔���l[fps]
    data->error.errtime   = cnfg_data->error.errtime;                   // �ُ픻�莞��[msec]

    // �J�����̋N���ݒ�
    data->boot.bootwaittimer  = cnfg_data->boot.bootwaittimer;          // �V�X�e���N����̃J�����N���҂�����[msec]
    data->boot.rebootcount    = cnfg_data->boot.rebootcount;            // �V�X�e���N����̃J�����N�����s���ɃJ���������ċN�������[count]
    data->boot.rebootingtimer = cnfg_data->boot.rebootingtimer;         // �J�����ċN�����ł��邱�Ƃ�ʒm���鎞��[msec]

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_MOUNTING data)
{
    PCONFIG_MOUNTING cnfg_data = &m_appcnfig.mounting;      // ��t���@�ݒ�

    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->offset[idx].lx0 = cnfg_data->offset[idx].lx0; // �݋�ݓ_�`BOX�ݓ_����LX0[mm]
        data->offset[idx].ly0 = cnfg_data->offset[idx].ly0; // �݋�ݓ_�`BOX�ݓ_����LY0[mm]
        data->offset[idx].l0  = cnfg_data->offset[idx].l0;  // BOX�ݓ_�`BOX�������S����L0[mm]
        data->offset[idx].lc  = cnfg_data->offset[idx].lc;  // BOX�������S�`�J�������S����LC[mm]
        data->offset[idx].a0  = cnfg_data->offset[idx].a0;  // BOX���J������t�p�x��0[deg]
        data->offset[idx].ac  = cnfg_data->offset[idx].ac;  // BOX�������S�`�J�������S�p�x��c[deg]
    }

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_INCLINOMETER data)
{
    PCONFIG_INCLINOMETER cnfg_data = &m_appcnfig.inclinometer;  // �X�Όv�ݒ�

    data->ipaddress    = cnfg_data->ipaddress;                  // IP�A�h���X
    data->tcpport      = cnfg_data->tcpport;                    // TCP�|�[�g�ԍ�
    data->slaveaddress = cnfg_data->slaveaddress;               // �X���[�u�A�h���X
    data->timeout      = cnfg_data->timeout;                    // �ʐM�^�C���A�E�g[msec]
    for (int idx = 0; idx < AXIS_MAX; idx++) {
        data->port[idx] = cnfg_data->port[idx];                 // �X�Όv�f�[�^�ڑ��|�[�g�ԍ�[1�`8] (X, Y) 
    }

    return TRUE;
}

/// @brief
/// @param
/// @return 
/// @note 
BOOL CShared::get_appconfig(PCONFIG_IMGPROC data)
{
    PCONFIG_IMGPROC cnfg_data = &m_appcnfig.imageprocess;           // �摜���������ݒ�

    // ROI�ݒ�
    data->roi.valid = cnfg_data->roi.valid;                         // ROI�L���ݒ�(����[FALSE]/�L��[TRUE])
    data->roi.scale = cnfg_data->roi.scale;                         // ROI�X�P�[��(���o�����^�[�Q�b�g�ɑ΂���{��)

    // �}�X�N�ݒ�
    for (int idx = 0; idx < IMGPROC_ID_MAX; idx++) {
        data->mask[idx].valid   = cnfg_data->mask[idx].valid;       // �}�X�N�I��(����[FALSE]/�L��[TRUE])
        data->mask[idx].hsv_l.h = cnfg_data->mask[idx].hsv_l.h;     // HSV�}�X�N����l(����)H
        data->mask[idx].hsv_l.s = cnfg_data->mask[idx].hsv_l.s;     // HSV�}�X�N����l(����)S
        data->mask[idx].hsv_l.v = cnfg_data->mask[idx].hsv_l.v;     // HSV�}�X�N����l(����)V
        data->mask[idx].hsv_h.h = cnfg_data->mask[idx].hsv_h.h;     // HSV�}�X�N����l(���)H
        data->mask[idx].hsv_h.s = cnfg_data->mask[idx].hsv_h.s;     // HSV�}�X�N����l(���)S
        data->mask[idx].hsv_h.v = cnfg_data->mask[idx].hsv_h.v;     // HSV�}�X�N����l(���)V
    }

    // �S�}���m�C�Y�t�B���^
    data->fliter1.type = cnfg_data->fliter1.type;                   // �t�B���^���
    data->fliter1.val  = cnfg_data->fliter1.val;                    // �t�B���^�l

    // �����߃m�C�Y�t�B���^
    data->fliter2.type = cnfg_data->fliter2.type;                   // �t�B���^���
    data->fliter2.val  = cnfg_data->fliter2.val;                    // �t�B���^�l

    data->imgproc       = cnfg_data->imgproc;                       // �^�[�Q�b�g���o�A���S���Y��(0=�S�֊s�_, 1=�ő�֊s�ʐ�, 2=�ő�֊s��)
    data->swayspdfliter = cnfg_data->swayspdfliter;                 // �U�ꑬ�x�t�B���^���萔

    return TRUE;
}
