#include "CShared.h"

//////////////////////////////////////////////////////////////////////////////
// CShared

APP_CONFIG CShared::m_app_cnfg; // �A�v���P�[�V�����ݒ�
APP_INFO   CShared::m_app_info; // �A�v���P�[�V�������

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CShared::CShared(BOOL init)
{
    if (init) {
        //----------------------------------------------------------------------------
        // �f�[�^������
        // �X�Όv���f�[�^
        PINFO_TILTMT_DATA info_tiltmt_data = &m_app_info.tiltmt.data;   // �X�Όv���f�[�^
        info_tiltmt_data->status = static_cast<uint32_t>(ENUM_TILT_STATUS::DEFAULT);    // �X�e�[�^�X���
        for (UINT i = 0; i < static_cast<uint32_t>(ENUM_AXIS::E_MAX); i++) {
            info_tiltmt_data->tilt[i].deg       = 0.0;  // �X�Ίp[deg]
            info_tiltmt_data->tilt[i].rad       = 0.0;  // �X�Ίp[rad]
            info_tiltmt_data->tilt[i].speed_deg = 0.0;  // �X�Ίp���x[deg/s]
            info_tiltmt_data->tilt[i].speed_rad = 0.0;  // �X�Ίp���x[rad/s]
        }

        // �N���C�A���g���f�[�^
        PINFO_CLIENT_DATA info_client_data = &m_app_info.client.data;   // �N���C�A���g���f�[�^
        info_client_data->device_id = L"";  // �@��ID
        info_client_data->command   = 0;    // �R�}���h
        info_client_data->mode      = 0;    // ���[�h
        info_client_data->cycle     = 0;    // �ŏ���M����[ms]
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
            info_client_data->tgt_len[idx] = 0.0;   // �^�[�Q�b�g����[mm]
        }

        // �������f�[�^
        PINFO_ADJUST_DATA info_adjust_data = &m_app_info.adjust.data;   // �������f�[�^
        info_adjust_data->target_distance_fixed = FALSE;                // �ݓ_�`�^�[�Q�b�g�ԋ����Œ�
        info_adjust_data->target_distance       = EXTN_TARGETDIST_MIN;  // �ݓ_�`�^�[�Q�b�g�ԋ���

        // �摜�������f�[�^
        PINFO_IMGPRC_DATA info_imgprc_data = &m_app_info.imgprc.data;   // �摜�������f�[�^
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            info_imgprc_data->target_data[idx].valid   = FALSE; // ���o���
            info_imgprc_data->target_data[idx].max_val = 0.0;   // �ő�P�x
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                info_imgprc_data->target_data[idx].pos[axis] = 0.0; // ���o�ʒu[pixel]
            }
            info_imgprc_data->target_data[idx].size       = 0;  // ���o�T�C�Y
            info_imgprc_data->target_data[idx].roi.x      = 0;  // ROI:x coordinate of the top-left corner
            info_imgprc_data->target_data[idx].roi.y      = 0;  // ROI:y coordinate of the top-left corner
            info_imgprc_data->target_data[idx].roi.width  = 0;  // ROI:width of the rectangle
            info_imgprc_data->target_data[idx].roi.height = 0;  // ROI:height of the rectangle
        }
        for (UINT axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            info_imgprc_data->sway_data[axis].target_pos  = 0.0;    // �^�[�Q�b�g�ʒu[pixel]
            info_imgprc_data->sway_data[axis].target_tilt = 0.0;    // �^�[�Q�b�g�X��[pixel]
            info_imgprc_data->sway_data[axis].sway_angle  = 0.0;    // �U��p[pixel]
            info_imgprc_data->sway_data[axis].sway_speed  = 0.0;    // �U�ꑬ�x[pixel/s]
            info_imgprc_data->sway_data[axis].sway_zero   = 0.0;    // �U�ꒆ�S[pixel]
        }
        info_imgprc_data->target_size = 0.0;                                                    // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
        info_imgprc_data->status      = static_cast<uint32_t>(ENUM_PROCCESS_STATUS::DEFAULT);   // ���
        info_imgprc_data->img_fps     = 0.0;                                                    // �t���[�����[�g[fps]
        info_imgprc_data->img_val     = 0.0 ;                                                   // ���x
        info_imgprc_data->exps_mode   = EXPOSURE_CONTROL_HOLD;                                  // �����I���R�}���h(0:��~ 1:Up -1:Down)
        info_imgprc_data->exps_time   = 0.0;                                                    // �I������[us]

        // �V�X�e�����f�[�^
        PINFO_SYSTEM_DATA info_system_data = &m_app_info.system.data;   // �V�X�e�����f�[�^
        info_system_data->status = static_cast<uint32_t>(ENUM_SYSTEM_STATUS::DEFAULT);  // �X�e�[�^�X

        //----------------------------------------------------------------------------
        // ���L�f�[�^�A�N�Z�X�p�N���e�B�J���Z�N�V�����̏�����
        for (UINT idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE::E_MAX); idx++) {
            InitializeCriticalSection(&m_app_info.imgbuf[idx].cs);  // �摜���
        }
        InitializeCriticalSection(&m_app_info.tiltmt.cs);   // �X�Όv���
        InitializeCriticalSection(&m_app_info.client.cs);   // �N���C�A���g���
        InitializeCriticalSection(&m_app_info.adjust.cs);   // �������
        InitializeCriticalSection(&m_app_info.imgprc.cs);   // �摜�������
        InitializeCriticalSection(&m_app_info.system.cs);   // �V�X�e�����
    }   // if (init)
}

/// @brief
/// @param
/// @return
/// @note
CShared::~CShared()
{
}

/// @brief ini file�ǂݍ��݃p�����[�^�ݒ�
/// @param file_name:�t�@�C����
/// @return 
/// @note 
BOOL CShared::set_app_config_ini(wchar_t* file_name)
{
    wchar_t str[256]  = {0};
    wchar_t note[256] = {0};

    //----------------------------------------------------------------------------
    // ���ʐݒ�
#pragma region SET_CONFIG_COMMON
    PCONFIG_COMMON cnfg_common = &m_app_cnfg.common;    // ���ʐݒ�

    // �J�����摜�捞��
    if (GetPrivateProfileString(INI_SCT_COMMON,
                                INI_KEY_COMMON_IMAGE_SOURCE_CAMERA,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_COMMON_IMAGE_SOURCE_CAMERA_ITEM_NUM + 1) != swscanf_s(str,
                                                                           L"%lu,%[^,]",
                                                                           &cnfg_common->img_source_camera,
                                                                           note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �捞�݉摜�t�@�C����
    if (GetPrivateProfileString(INI_SCT_COMMON,
                                INI_KEY_COMMON_IMAGE_SOURCE_FILENAME,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t fname[256] = {0};
        if ((INI_KEY_COMMON_IMAGE_SOURCE_FILENAME_ITEM_NUM + 1) != swscanf_s(str,
                                                                             L"%[^,],%[^,]",
                                                                             fname, (unsigned int)(sizeof(fname) / sizeof(*fname)),
                                                                             note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_common->img_source_fname = fname;
            cnfg_common->img_source_fname.erase(remove(cnfg_common->img_source_fname.begin(), cnfg_common->img_source_fname.end(),' '),
                                                cnfg_common->img_source_fname.end());
        }
    }

    // �摜�ۑ��t�@�C����
    if (GetPrivateProfileString(INI_SCT_COMMON,
                                INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t fname[256] = {0};
        if ((INI_KEY_COMMON_IMAGE_OUTPUT_FILENAME_ITEM_NUM + 1) != swscanf_s(str,
                                                                             L"%[^,],%[^,]",
                                                                             fname, (unsigned int)(sizeof(fname) / sizeof(*fname)),
                                                                             note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_common->img_output_fname = fname;
            cnfg_common->img_output_fname.erase(remove(cnfg_common->img_output_fname.begin(), cnfg_common->img_output_fname.end(),' '),
                                                cnfg_common->img_output_fname.end());
        }
    }

    // �摜�\�����C�A�E�g
    if (GetPrivateProfileString(INI_SCT_COMMON,
                                INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_COMMON_IMAGE_SCREEN_LAYOUT_ITEM_NUM + 1)!= swscanf_s(str,
                                                                          L"%lu,%lu,%lu,%lu,%[^,]",
                                                                          &cnfg_common->img_screen_layout.x0,
                                                                          &cnfg_common->img_screen_layout.y0,
                                                                          &cnfg_common->img_screen_layout.width,
                                                                          &cnfg_common->img_screen_layout.height,
                                                                          note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
#pragma endregion SET_CONFIG_COMMON

    //----------------------------------------------------------------------------
    // �V�X�e���ݒ�
#pragma region SET_CONFIG_SYSTEM
    PCONFIG_SYSTEM cnfg_system = &m_app_cnfg.system;    // �V�X�e���ݒ�

    // ���C���X���b�h���s����
    if (GetPrivateProfileString(INI_SCT_SYSTEM,
                                INI_KEY_SYSTEM_CYCLE_TIME,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_SYSTEM_CYCLE_TIME_ITEM_NUM + 1) != swscanf_s(str,
                                                                  L"%lu,%[^,]",
                                                                  &cnfg_system->cycletime_sys,
                                                                  note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �@��ID
    if (GetPrivateProfileString(INI_SCT_SYSTEM,
                                INI_KEY_SYSTEM_DEVICE_ID,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t mcc_id[256] = {0};
        wchar_t my_id[256]  = {0};
        if ((INI_KEY_SYSTEM_DEVICE_ID_ITEM_NUM + 1) != swscanf_s(str,
                                                                 L"%[^,],%[^,],%[^,]",
                                                                 mcc_id, (unsigned int)(sizeof(mcc_id) / sizeof(*mcc_id)),
                                                                 my_id, (unsigned int)(sizeof(my_id) / sizeof(*my_id)),
                                                                 note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_system->mcc_id = mcc_id;
            cnfg_system->mcc_id.erase(remove(cnfg_system->mcc_id.begin(), cnfg_system->mcc_id.end(),' '),
                                             cnfg_system->mcc_id.end());
            cnfg_system->my_id = my_id;
            cnfg_system->my_id.erase(remove(cnfg_system->my_id.begin(), cnfg_system->my_id.end(),' '),
                                            cnfg_system->my_id.end());
        }
    }
#pragma endregion SET_CONFIG_SYSTEM

    //----------------------------------------------------------------------------
    // �^�X�N�ݒ�
#pragma region SET_CONFIG_TASK
    PCONFIG_TASK cnfg_task = &m_app_cnfg.task;  // �^�X�N�ݒ�

    // Environment
    if (GetPrivateProfileString(INI_SCT_TASK,
                                INI_KEY_TASK_ENVIRONMENT,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t name[256]  = {0};
        wchar_t sname[256] = {0};
        if ((INI_KEY_TASK_ITEM_NUM + 1)!= swscanf_s(str,
                                                    L"%lu,%[^,],%[^,],%[^,]",
                                                    &cnfg_task->environment.cycletime,
                                                    name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                    sname, (unsigned int)(sizeof(sname) / sizeof(*sname)),
                                                    note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_task->environment.name = name;
            cnfg_task->environment.name.erase(remove(cnfg_task->environment.name.begin(), cnfg_task->environment.name.end(),' '),
                                              cnfg_task->environment.name.end());
            cnfg_task->environment.sname = sname;
            cnfg_task->environment.sname.erase(remove(cnfg_task->environment.sname.begin(), cnfg_task->environment.sname.end(),' '),
                                               cnfg_task->environment.sname.end());
        }
    }

    // Policy
    if (GetPrivateProfileString(INI_SCT_TASK,
                                INI_KEY_TASK_POLICY,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t name[256]  = {0};
        wchar_t sname[256] = {0};
        if ((INI_KEY_TASK_ITEM_NUM + 1)!= swscanf_s(str,
                                                    L"%lu,%[^,],%[^,],%[^,]",
                                                    &cnfg_task->policy.cycletime,
                                                    name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                    sname, (unsigned int)(sizeof(sname) / sizeof(*sname)),
                                                    note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_task->policy.name = name;
            cnfg_task->policy.name.erase(remove(cnfg_task->policy.name.begin(), cnfg_task->policy.name.end(),' '),
                                         cnfg_task->policy.name.end());
            cnfg_task->policy.sname = sname;
            cnfg_task->policy.sname.erase(remove(cnfg_task->policy.sname.begin(), cnfg_task->policy.sname.end(),' '),
                                          cnfg_task->policy.sname.end());
        }
    }

    // Agent
    if (GetPrivateProfileString(INI_SCT_TASK,
                                INI_KEY_TASK_AGENT,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t name[256]  = {0};
        wchar_t sname[256] = {0};
        if ((INI_KEY_TASK_ITEM_NUM + 1)!= swscanf_s(str,
                                                    L"%lu,%[^,],%[^,],%[^,]",
                                                    &cnfg_task->agent.cycletime,
                                                    name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                    sname, (unsigned int)(sizeof(sname) / sizeof(*sname)),
                                                    note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_task->agent.name = name;
            cnfg_task->agent.name.erase(remove(cnfg_task->agent.name.begin(), cnfg_task->agent.name.end(),' '),
                                        cnfg_task->agent.name.end());
            cnfg_task->agent.sname = sname;
            cnfg_task->agent.sname.erase(remove(cnfg_task->agent.sname.begin(), cnfg_task->agent.sname.end(),' '),
                                         cnfg_task->agent.sname.end());
        }
    }

    // Scada
    if (GetPrivateProfileString(INI_SCT_TASK,
                                INI_KEY_TASK_SCADA,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t name[256]  = {0};
        wchar_t sname[256] = {0};
        if ((INI_KEY_TASK_ITEM_NUM + 1)!= swscanf_s(str,
                                                    L"%lu,%[^,],%[^,],%[^,]",
                                                    &cnfg_task->scada.cycletime,
                                                    name, (unsigned int)(sizeof(name) / sizeof(*name)),
                                                    sname, (unsigned int)(sizeof(sname) / sizeof(*sname)),
                                                    note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_task->scada.name = name;
            cnfg_task->scada.name.erase(remove(cnfg_task->scada.name.begin(), cnfg_task->scada.name.end(),' '),
                                        cnfg_task->scada.name.end());
            cnfg_task->scada.sname = sname;
            cnfg_task->scada.sname.erase(remove(cnfg_task->scada.sname.begin(), cnfg_task->scada.sname.end(),' '),
                                         cnfg_task->scada.sname.end());
        }
    }
#pragma endregion SET_CONFIG_TASK

    //----------------------------------------------------------------------------
    // �ʐM�ݒ�
#pragma region SET_CONFIG_ETHERNET
    PCONFIG_ETHERNET cnfg_ethernet = &m_app_cnfg.ethernet;  // �ʐM�ݒ�

    // Local�ݒ�
    if (GetPrivateProfileString(INI_SCT_ETHERNET,
                                INI_KEY_ETHERNET_LOCAL,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t ipaddress[256] = {0};
        if ((INI_KEY_ETHERNET_ITEM_NUM + 1)!= swscanf_s(str,
                                                        L"%[^,],%lu,%[^,]",
                                                        ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                        &cnfg_ethernet->local.port,
                                                        note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_ethernet->local.ipaddress = ipaddress;
            cnfg_ethernet->local.ipaddress.erase(remove(cnfg_ethernet->local.ipaddress.begin(), cnfg_ethernet->local.ipaddress.end(),' '),
                                                 cnfg_ethernet->local.ipaddress.end());
        }
    }

    // Remote�ݒ�
    if (GetPrivateProfileString(INI_SCT_ETHERNET,
                                INI_KEY_ETHERNET_REMOTE,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t ipaddress[256] = {0};
        if ((INI_KEY_ETHERNET_ITEM_NUM + 1)!= swscanf_s(str,
                                                        L"%[^,],%lu,%[^,]",
                                                        ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                        &cnfg_ethernet->remote.port,
                                                        note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_ethernet->remote.ipaddress = ipaddress;
            cnfg_ethernet->remote.ipaddress.erase(remove(cnfg_ethernet->remote.ipaddress.begin(), cnfg_ethernet->remote.ipaddress.end(),' '),
                                                  cnfg_ethernet->remote.ipaddress.end());
        }
    }
#pragma endregion SET_CONFIG_ETHERNET

    //----------------------------------------------------------------------------
    // �J�����ݒ�
#pragma region SET_CONFIG_CAMERA
    // �J�����̊�{�ݒ�
    PCONFIG_CAMERA cnfg_camera = &m_app_cnfg.camera;    // �J�����ݒ�

    // IP�A�h���X
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_IP_ADDRESS,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t ipaddress[256] = {0};
        if ((INI_KEY_CAMERA_BASIS_IP_ADDRESS_ITEM_NUM + 1) != swscanf_s(str,
                                                                        L"%[^,],%[^,]",
                                                                        ipaddress, (unsigned int)(sizeof(ipaddress) / sizeof(*ipaddress)),
                                                                        note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_camera->basis.ipaddress = ipaddress;
            cnfg_camera->basis.ipaddress.erase(remove(cnfg_camera->basis.ipaddress.begin(), cnfg_camera->basis.ipaddress.end(),' '),
                                               cnfg_camera->basis.ipaddress.end());
        }
    }
    // ROI(�̈�)
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_ROI,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_ROI_ITEM_NUM + 1)!= swscanf_s(str,
                                                                L"%lu,%lu,%lu,%lu,%[^,]",
                                                                &cnfg_camera->basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].offset,
                                                                &cnfg_camera->basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].offset,
                                                                &cnfg_camera->basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size,
                                                                &cnfg_camera->basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_PACKET_SIZE,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_PACKET_SIZE_ITEM_NUM + 1)!= swscanf_s(str,
                                                                        L"%lu,%[^,]",
                                                                        &cnfg_camera->basis.packetsize,
                                                                        note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �t���[�����[�g
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_FRAME_RATE,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_FRAME_RATE_ITEM_NUM + 1)!= swscanf_s(str,
                                                                       L"%lf,%[^,]",
                                                                       &cnfg_camera->basis.framerate,
                                                                       note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����x���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_BLACKLEVEL,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_BLACKLEVEL_ITEM_NUM + 1)!= swscanf_s(str,
                                                                       L"%lf,%[^,]",
                                                                       &cnfg_camera->basis.blacklevel,
                                                                       note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �K���}�␳
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_GAMMA,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_GAMMA_ITEM_NUM + 1)!= swscanf_s(str,
                                                                  L"%lf,%[^,]",
                                                                  &cnfg_camera->basis.gamma,
                                                                  note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �z���C�g�o�����X�Q�C���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_WBALANCE_AUTO,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_WBALANCE_AUTO_ITEM_NUM +1)!= swscanf_s(str,
                                                                         L"%lu,%lf,%lf,%[^,]",
                                                                         &cnfg_camera->basis.wb.wb_auto,
                                                                         &cnfg_camera->basis.wb.wb_ratio_red,
                                                                         &cnfg_camera->basis.wb.wb_ratio_blue,
                                                                         note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // ����p
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BASIS_VIEW_ANGLE,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BASIS_VIEW_ANGLE_ITEM_NUM + 1)!= swscanf_s(str,
                                                                       L"%lf,%lf,%[^,]",
                                                                       &cnfg_camera->basis.viewangle[static_cast<uint32_t>(ENUM_AXIS::X)],
                                                                       &cnfg_camera->basis.viewangle[static_cast<uint32_t>(ENUM_AXIS::Y)],
                                                                       note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �P�x�R���g���[���ݒ�(�Q�C��)
    // �Q�C���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_GAIN,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_GAIN_ITEM_NUM + 1)!= swscanf_s(str,
                                                           L"%lf,%lf,%lf,%[^,]",
                                                           &cnfg_camera->gain.val,
                                                           &cnfg_camera->gain.val_min,
                                                           &cnfg_camera->gain.val_max,
                                                           note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����Q�C���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_GAIN_AUTO,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_GAIN_AUTO_ITEM_NUM + 1)!= swscanf_s(str,
                                                                L"%lu,%lf,%[^,]",
                                                                &cnfg_camera->gain.auto_control,
                                                                &cnfg_camera->gain.auto_rate,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����Q�C������P�x(�J�n���)
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_GAIN_AUTO_START,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_GAIN_AUTO_START_ITEM_NUM + 1)!= swscanf_s(str,
                                                                      L"%lf,%lf,%[^,]",
                                                                      &cnfg_camera->gain.auto_start_h,
                                                                      &cnfg_camera->gain.auto_start_l,
                                                                      note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����Q�C������P�x(��~���)
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_GAIN_AUTO_STOP,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_GAIN_AUTO_STOP_ITEM_NUM + 1)!= swscanf_s(str,
                                                                     L"%lf,%lf,%[^,]",
                                                                     &cnfg_camera->gain.auto_stop_h,
                                                                     &cnfg_camera->gain.auto_stop_l,
                                                                     note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �P�x�R���g���[���ݒ�(�I������)
    // �I�����Ԑݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_EXPSTIME,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_EXPSTIME_ITEM_NUM + 1) != swscanf_s(str,
                                                                L"%lf,%lf,%lf,%[^,]",
                                                                &cnfg_camera->expstime.val,
                                                                &cnfg_camera->expstime.val_min,
                                                                &cnfg_camera->expstime.val_max,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����I���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_EXPSTIME_AUTO,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_EXPSTIME_AUTO_ITEM_NUM + 1) != swscanf_s(str,
                                                                     L"%lu,%lf,%[^,]",
                                                                     &cnfg_camera->expstime.auto_control,
                                                                     &cnfg_camera->expstime.auto_rate,
                                                                     note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����I������P�x(�J�n���)
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_EXPSTIME_AUTO_START,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_EXPSTIME_AUTO_START_ITEM_NUM + 1) != swscanf_s(str,
                                                                           L"%lf,%lf,%[^,]",
                                                                           &cnfg_camera->expstime.auto_start_h,
                                                                           &cnfg_camera->expstime.auto_start_l,
                                                                           note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����I������P�x(��~���)
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_EXPSTIME_AUTO_STOP,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_EXPSTIME_AUTO_STOP_ITEM_NUM + 1) != swscanf_s(str,
                                                                          L"%lf,%lf,%[^,]",
                                                                          &cnfg_camera->expstime.auto_stop_h,
                                                                          &cnfg_camera->expstime.auto_stop_l,
                                                                          note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �J�����ُ̈픻��ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_ERROR,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_ERROR_ITEM_NUM + 1) != swscanf_s(str,
                                                             L"%lf,%lu,%[^,]",
                                                             &cnfg_camera->error.framedrop,
                                                             &cnfg_camera->error.errtime,
                                                             note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }

    // �J�����̋N���ݒ�
    if (GetPrivateProfileString(INI_SCT_CAMERA,
                                INI_KEY_CAMERA_BOOT,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_CAMERA_BOOT_ITEM_NUM + 1) != swscanf_s(str,
                                                            L"%lu,%lu,%lu,%[^,]",
                                                            &cnfg_camera->boot.bootwaittimer,
                                                            &cnfg_camera->boot.rebootcount,
                                                            &cnfg_camera->boot.rebootingtimer,
                                                            note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
#pragma endregion SET_CONFIG_CAMERA

    //----------------------------------------------------------------------------
    // �X�Όv�ݒ�
#pragma region SET_CONFIG_TILTMETER
    PCONFIG_TILTMETER cnfg_tiltmeter = &m_app_cnfg.tiltmeter;   // �X�Όv�ݒ�

    // �f�o�C�X�ݒ�
    if (GetPrivateProfileString(INI_SCT_TILTMETER,
                                INI_KEY_TILTMETER_CANOPEN,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t setup_fname[256] = {0};
        if ((INI_KEY_TILTMETER_CANOPEN_ITEM_NUM + 1) != swscanf_s(str,
                                                                  L"%[^,],%hhu,%hhx,%[^,]",
                                                                  setup_fname, (unsigned int)(sizeof(setup_fname) / sizeof(*setup_fname)),
                                                                  &cnfg_tiltmeter->canopen.port,
                                                                  &cnfg_tiltmeter->canopen.node_id,
                                                                  note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        else {
            cnfg_tiltmeter->canopen.setup_fname = setup_fname;
            cnfg_tiltmeter->canopen.setup_fname.erase(remove(cnfg_tiltmeter->canopen.setup_fname.begin(), cnfg_tiltmeter->canopen.setup_fname.end(),' '),
                                                      cnfg_tiltmeter->canopen.setup_fname.end());
        }
    }
    // �X�Όv�t�B���^�[
    if (GetPrivateProfileString(INI_SCT_TILTMETER,
                                INI_KEY_TILTMETER_FILTER,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        wchar_t setup_fname[256] = {0};
        if ((INI_KEY_TILTMETER_FILTER_ITEM_NUM + 1) != swscanf_s(str,
                                                                 L"%hu,%hu,%[^,]",
                                                                 &cnfg_tiltmeter->dr_filter,
                                                                 &cnfg_tiltmeter->ma_filter,
                                                                 note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �X�Ίp���x�t�B���^�[���萔
    if (GetPrivateProfileString(INI_SCT_TILTMETER,
                                INI_KEY_TILTMETER_SPEED_TC,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_TILTMETER_SPEED_TC_ITEM_NUM + 1) != swscanf_s(str,
                                                                  L"%lf,%[^,]",
                                                                  &cnfg_tiltmeter->tiltspdfilter,
                                                                  note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
#pragma endregion SET_CONFIG_TILTMETER

    //----------------------------------------------------------------------------
    // ��t���@�ݒ�
#pragma region SET_CONFIG_MOUNTING
    PCONFIG_MOUNTING cnfg_mounting = &m_app_cnfg.mounting;  // ��t���@�ݒ�

    // �J������]������L0[mm]
    if (GetPrivateProfileString(INI_SCT_MOUNTING,
                                INI_KEY_MOUNTING_OFFSET_L0,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_MOUNTING_OFFSET_ITEM_NUM + 1) != swscanf_s(str,
                                                                L"%lf,%lf,%[^,]",
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::X)].l0,
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::Y)].l0,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �J������t�p�x��0(*10^6)[rad]
    if (GetPrivateProfileString(INI_SCT_MOUNTING,
                                INI_KEY_MOUNTING_OFFSET_T0,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_MOUNTING_OFFSET_ITEM_NUM + 1) != swscanf_s(str,
                                                                L"%lf,%lf,%[^,]",
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::X)].t0,
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::Y)].t0,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �J�����ݒu�p�x��0(*10^6)[rad]
    if (GetPrivateProfileString(INI_SCT_MOUNTING,
                                INI_KEY_MOUNTING_OFFSET_P0,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_MOUNTING_OFFSET_ITEM_NUM + 1) != swscanf_s(str,
                                                                L"%lf,%lf,%[^,]",
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::X)].p0,
                                                                &cnfg_mounting->offset[static_cast<uint32_t>(ENUM_AXIS::Y)].p0,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
#pragma endregion SET_CONFIG_MOUNTING

    //----------------------------------------------------------------------------
    // �摜���������ݒ�
#pragma region SET_CONFIG_IMGPROC
    PCONFIG_IMGPROC cnfg_imgprc = &m_app_cnfg.imageprocess;

    // �摜ROI�ݒ�
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_ROI,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_ROI_ITEM_NUM + 1) != swscanf_s(str,
                                                            L"%lu,%lf,%[^,]",
                                                            &cnfg_imgprc->roi.valid,
                                                            &cnfg_imgprc->roi.scale,
                                                            note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �}�X�N�摜�I��
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        uint32_t mask = 0;
        if ((INI_KEY_IMGPROC_MASK_ITEM_NUM + 1) != swscanf_s(str,
                                                            L"%lu,%[^,]",
                                                            &mask,
                                                            note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
        switch (mask) {
        case static_cast<uint32_t>(ENUM_IMAGE_MASK_SEL::IMAGE_1):
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid = TRUE;
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid = FALSE;
            break;
        case static_cast<uint32_t>(ENUM_IMAGE_MASK_SEL::IMAGE_2):
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid = FALSE;
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid = TRUE;
            break;
        default:
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid = TRUE;
            cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid = TRUE;
            break;
        }
    }
    // �摜1�}�X�NH
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK1_H,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK1_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �摜1�}�X�NS
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK1_S,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK1_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �摜1�}�X�NV
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK1_V,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK1_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �摜2�}�X�NH
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK2_H,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK2_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �摜2�}�X�NS
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK2_S,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK2_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �摜2�}�X�NV
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_MASK2_V,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_MASK2_ITEM_NUM + 1) != swscanf_s(str,
                                                              L"%lu,%lu,%[^,]",
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)],
                                                              &cnfg_imgprc->mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)],
                                                              note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �S�}���m�C�Y�t�B���^�[
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_FILTER1,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_FILTER1_ITEM_NUM + 1) != swscanf_s(str,
                                                                L"%lu,%lu,%[^,]",
                                                                &cnfg_imgprc->filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type,
                                                                &cnfg_imgprc->filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val,
                                                                note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �����߃m�C�Y�t�B���^�[
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_FILTER2,
                                L"-", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_FILTER2_ITEM_NUM + 1)!= swscanf_s(str,
                                                               L"%lu,%lu,%[^,]",
                                                               &cnfg_imgprc->filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type,
                                                               &cnfg_imgprc->filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val,
                                                               note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �^�[�Q�b�g���o�A���S���Y��
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_PROC,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_PROC_ITEM_NUM + 1) != swscanf_s(str,
                                                             L"%lu,%[^,]",
                                                             &cnfg_imgprc->imgprc,
                                                             note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �U�ꑬ�x�t�B���^�[���萔
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_SWAY_SPEED_TC,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_SWAY_SPEED_TC_ITEM_NUM + 1) != swscanf_s(str,
                                                                      L"%lf,%[^,]",
                                                                      &cnfg_imgprc->swayspdfilter,
                                                                      note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
    // �U��[���_�ݒ�
    if (GetPrivateProfileString(INI_SCT_IMGPROC,
                                INI_KEY_IMGPROC_SWAY_ZERO_SET,
                                L"", str, sizeof(str) / sizeof(*str),
                                file_name) <= 0) {
        return FALSE;
    }
    else {
        if ((INI_KEY_IMGPROC_SWAY_ZERO_SET_ITEM_NUM + 1) != swscanf_s(str,
                                                                      L"%lu,%[^,]",
                                                                      &cnfg_imgprc->swayzeroset_time,
                                                                      note, (unsigned int)(sizeof(note) / sizeof(*note)))) {
            return FALSE;
        }
    }
#pragma endregion SET_CONFIG_IMGPROC

    return TRUE;
}

/// @brief ���ʐݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_COMMON data)
{
    PCONFIG_COMMON cnfg_data = &m_app_cnfg.common;  // ���ʐݒ�

    cnfg_data->img_source_camera = data.img_source_camera;  // �J�����摜�捞��(�J����[1]/�摜�t�@�C��[0])
    cnfg_data->img_source_fname  = data.img_source_fname;   // �捞�݉摜�t�@�C����CMN_IMAGE_SOURCE_CAMERA=0�̂Ƃ��̉摜)
    cnfg_data->img_output_fname  = data.img_output_fname;   // �摜�ۑ��t�@�C����

    cnfg_data->img_screen_layout.x0     = data.img_screen_layout.x0;        // ���_���WX
    cnfg_data->img_screen_layout.y0     = data.img_screen_layout.y0;        // ���_���WY
    cnfg_data->img_screen_layout.width  = data.img_screen_layout.width;     // �����T�C�Y
    cnfg_data->img_screen_layout.height = data.img_screen_layout.height;    // �����T�C�Y

    return TRUE;
}

/// @brief ���ʐݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_COMMON data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_COMMON cnfg_data = &m_app_cnfg.common;  // ���ʐݒ�

    data->img_source_camera = cnfg_data->img_source_camera; // �J�����摜�捞��(�J����[1]/�摜�t�@�C��[0])
    data->img_source_fname  = cnfg_data->img_source_fname;  // �捞�݉摜�t�@�C����CMN_IMAGE_SOURCE_CAMERA=0�̂Ƃ��̉摜)
    data->img_output_fname  = cnfg_data->img_output_fname;  // �摜�ۑ��t�@�C����

    data->img_screen_layout.x0     = cnfg_data->img_screen_layout.x0;       // ���_���WX
    data->img_screen_layout.y0     = cnfg_data->img_screen_layout.y0;       // ���_���WY
    data->img_screen_layout.width  = cnfg_data->img_screen_layout.width;    // �����T�C�Y
    data->img_screen_layout.height = cnfg_data->img_screen_layout.height;   // �����T�C�Y

    return TRUE;
}

/// @brief �V�X�e���ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_SYSTEM data)
{
    PCONFIG_SYSTEM cnfg_data = &m_app_cnfg.system;  // ���ʐݒ�

    cnfg_data->cycletime_sys = data.cycletime_sys;  // ���C���X���b�h���s����[msec]
    cnfg_data->mcc_id        = data.mcc_id;         // �@��ID(����PC)[���p�p����4����]
    cnfg_data->my_id         = data.my_id;          // �@��ID(�U��Z���T�[PC)[���p�p����4����]

    return TRUE;
}

/// @brief �V�X�e���ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_SYSTEM data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_SYSTEM cnfg_data = &m_app_cnfg.system;  // ���ʐݒ�

    data->cycletime_sys = cnfg_data->cycletime_sys; // ���C���X���b�h���s����[msec]
    data->mcc_id        = cnfg_data->mcc_id;        // �@��ID(����PC)[���p�p����4����]
    data->my_id         = cnfg_data->my_id;         // �@��ID(�U��Z���T�[PC)[���p�p����4����]

    return TRUE;
}

/// @brief �^�X�N�ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_TASK data)
{
    PCONFIG_TASK cnfg_data = &m_app_cnfg.task;  // �^�X�N�ݒ�

    // Environment
    cnfg_data->environment.cycletime = data.environment.cycletime;  // ���C���X���b�h���s����[msec]
    cnfg_data->environment.name      = data.environment.name;       // �^�X�N��
    cnfg_data->environment.sname     = data.environment.sname;      // �^�X�N����
    // Policy
    cnfg_data->policy.cycletime      = data.policy.cycletime;       // ���C���X���b�h���s����[msec]
    cnfg_data->policy.name           = data.policy.name;            // �^�X�N��
    cnfg_data->policy.sname          = data.policy.sname;           // �^�X�N����
    // Agent
    cnfg_data->agent.cycletime       = data.agent.cycletime;        // ���C���X���b�h���s����[msec]
    cnfg_data->agent.name            = data.agent.name;             // �^�X�N��
    cnfg_data->agent.sname           = data.agent.sname;            // �^�X�N����
    // Scada
    cnfg_data->scada.cycletime       = data.scada.cycletime;        // ���C���X���b�h���s����[msec]
    cnfg_data->scada.name            = data.scada.name;             // �^�X�N��
    cnfg_data->scada.sname           = data.scada.sname;            // �^�X�N����

    return TRUE;
}

/// @brief �^�X�N�ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_TASK data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_TASK cnfg_data = &m_app_cnfg.task;  // �^�X�N�ݒ�

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

/// @brief �ʐM�ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_ETHERNET data)
{
    PCONFIG_ETHERNET cnfg_data = &m_app_cnfg.ethernet;  // �ʐM�ݒ�

    // Local
    cnfg_data->local.ipaddress  = data.local.ipaddress;     // IP�A�h���X
    cnfg_data->local.port       = data.local.port;          // �|�[�g�ԍ�
    // Remote
    cnfg_data->remote.ipaddress = data.remote.ipaddress;    // IP�A�h���X
    cnfg_data->remote.port      = data.remote.port;         // �|�[�g�ԍ�

    return TRUE;
}

/// @brief �ʐM�ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_ETHERNET data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_ETHERNET cnfg_data = &m_app_cnfg.ethernet;  // �ʐM�ݒ�

    // Local
    data->local.ipaddress  = cnfg_data->local.ipaddress;    // IP�A�h���X
    data->local.port       = cnfg_data->local.port;         // �|�[�g�ԍ�
    // Remote
    data->remote.ipaddress = cnfg_data->remote.ipaddress;   // IP�A�h���X
    data->remote.port      = cnfg_data->remote.port;        // �|�[�g�ԍ�

    return TRUE;
}

/// @brief �J�����ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_CAMERA data)
{
    PCONFIG_CAMERA cnfg_data = &m_app_cnfg.camera;  // �J�����ݒ�

    // �J�����̊�{�ݒ�
    cnfg_data->basis.ipaddress = data.basis.ipaddress;  // IP�A�h���X
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        cnfg_data->basis.roi[idx].offset = data.basis.roi[idx].offset;  // ROI(�̈�)�̊J�n�ʒu
        cnfg_data->basis.roi[idx].size   = data.basis.roi[idx].size;    // ROI(�̈�)�̃T�C�Y
    }
    cnfg_data->basis.packetsize       = data.basis.packetsize;          // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
    cnfg_data->basis.framerate        = data.basis.framerate;           // �t���[�����[�g[fps]
    cnfg_data->basis.blacklevel       = data.basis.blacklevel;          // �����x���ݒ�
    cnfg_data->basis.gamma            = data.basis.gamma;               // �K���}�␳
    cnfg_data->basis.wb.wb_auto       = data.basis.wb.wb_auto;          // �z���C�g�o�����X�Q�C�������������[�ݒ�(��������OFF[1]/�A����������[2]/���̂ݎ�������[3])
    cnfg_data->basis.wb.wb_ratio_red  = data.basis.wb.wb_ratio_red;     // �z���C�g�o�����X�Q�C���ݒ�(Red Gain)
    cnfg_data->basis.wb.wb_ratio_blue = data.basis.wb.wb_ratio_blue;    // �z���C�g�o�����X�Q�C���ݒ�(Blue Gain)
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        cnfg_data->basis.viewangle[idx] = data.basis.viewangle[idx];    // ����p[deg]
    }

    // �P�x�R���g���[���ݒ�(�Q�C��)
    cnfg_data->gain.val          = data.gain.val;           // �ݒ�l
    cnfg_data->gain.val_min      = data.gain.val_min;       // �ŏ��l
    cnfg_data->gain.val_max      = data.gain.val_max;       // �ő�l
    cnfg_data->gain.auto_control = data.gain.auto_control;  // �����R���g���[��(�Œ�[0]/����[1])
    cnfg_data->gain.auto_rate    = data.gain.auto_rate;     // �����R���g���[���ω���
    cnfg_data->gain.auto_start_h = data.gain.auto_start_h;  // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    cnfg_data->gain.auto_start_l = data.gain.auto_start_l;  // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    cnfg_data->gain.auto_stop_h  = data.gain.auto_stop_h;   // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    cnfg_data->gain.auto_stop_l  = data.gain.auto_stop_l;   // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �P�x�R���g���[���ݒ�(�I������)
    cnfg_data->expstime.val          = data.expstime.val;           // �ݒ�l
    cnfg_data->expstime.val_min      = data.expstime.val_min;       // �ŏ��l
    cnfg_data->expstime.val_max      = data.expstime.val_max;       // �ő�l
    cnfg_data->expstime.auto_control = data.expstime.auto_control;  // �����R���g���[��(�Œ�[0]/����[1])
    cnfg_data->expstime.auto_rate    = data.expstime.auto_rate;     // �����R���g���[���ω���
    cnfg_data->expstime.auto_start_h = data.expstime.auto_start_h;  // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    cnfg_data->expstime.auto_start_l = data.expstime.auto_start_l;  // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    cnfg_data->expstime.auto_stop_h  = data.expstime.auto_stop_h;   // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    cnfg_data->expstime.auto_stop_l  = data.expstime.auto_stop_l;   // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �J�����ُ̈픻��ݒ�
    cnfg_data->error.framedrop = data.error.framedrop;  // �t���[�����[�g�ቺ�̔���l[fps]
    cnfg_data->error.errtime   = data.error.errtime;    // �ُ픻�莞��[msec]

    // �J�����̋N���ݒ�
    cnfg_data->boot.bootwaittimer  = data.boot.bootwaittimer;   // �V�X�e���N����̃J�����N���҂�����[msec]
    cnfg_data->boot.rebootcount    = data.boot.rebootcount;     // �V�X�e���N����̃J�����N�����s���ɃJ���������ċN�������[count]
    cnfg_data->boot.rebootingtimer = data.boot.rebootingtimer;  // �J�����ċN�����ł��邱�Ƃ�ʒm���鎞��[msec]

    return TRUE;
}

/// @brief �J�����ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_CAMERA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_CAMERA cnfg_data = &m_app_cnfg.camera;  // �J�����ݒ�

    // �J�����̊�{�ݒ�
    data->basis.ipaddress = cnfg_data->basis.ipaddress;                 // IP�A�h���X
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
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
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        data->basis.viewangle[idx] = cnfg_data->basis.viewangle[idx];   // ����p[deg]
    }

    // �P�x�R���g���[���ݒ�(�Q�C��)
    data->gain.val          = cnfg_data->gain.val;          // �ݒ�l
    data->gain.val_min      = cnfg_data->gain.val_min;      // �ŏ��l
    data->gain.val_max      = cnfg_data->gain.val_max;      // �ő�l
    data->gain.auto_control = cnfg_data->gain.auto_control; // �����R���g���[��(�Œ�[0]/����[1])
    data->gain.auto_rate    = cnfg_data->gain.auto_rate;    // �����R���g���[���ω���
    data->gain.auto_start_h = cnfg_data->gain.auto_start_h; // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    data->gain.auto_start_l = cnfg_data->gain.auto_start_l; // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    data->gain.auto_stop_h  = cnfg_data->gain.auto_stop_h;  // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    data->gain.auto_stop_l  = cnfg_data->gain.auto_stop_l;  // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �P�x�R���g���[���ݒ�(�I������)
    data->expstime.val          = cnfg_data->expstime.val;          // �ݒ�l
    data->expstime.val_min      = cnfg_data->expstime.val_min;      // �ŏ��l
    data->expstime.val_max      = cnfg_data->expstime.val_max;      // �ő�l
    data->expstime.auto_control = cnfg_data->expstime.auto_control; // �����R���g���[��(�Œ�[0]/����[1])
    data->expstime.auto_rate    = cnfg_data->expstime.auto_rate;    // �����R���g���[���ω���
    data->expstime.auto_start_h = cnfg_data->expstime.auto_start_h; // �����R���g���[������P�x(�J�n���:���̒l���P�x�������ꍇ�A�Â�����)   [0 - 255]
    data->expstime.auto_start_l = cnfg_data->expstime.auto_start_l; // �����R���g���[������P�x(�J�n����:���̒l���P�x�������ꍇ�A���邭����) [0 - 255]
    data->expstime.auto_stop_h  = cnfg_data->expstime.auto_stop_h;  // �����R���g���[������P�x(��~���:���̒l���P�x���Ⴂ�ꍇ�A��~����)   [0 - 255]
    data->expstime.auto_stop_l  = cnfg_data->expstime.auto_stop_l;  // �����R���g���[������P�x(��~����:���̒l���P�x�������ꍇ�A��~����)   [0 - 255]

    // �J�����ُ̈픻��ݒ�
    data->error.framedrop = cnfg_data->error.framedrop; // �t���[�����[�g�ቺ�̔���l[fps]
    data->error.errtime   = cnfg_data->error.errtime;   // �ُ픻�莞��[msec]

    // �J�����̋N���ݒ�
    data->boot.bootwaittimer  = cnfg_data->boot.bootwaittimer;  // �V�X�e���N����̃J�����N���҂�����[msec]
    data->boot.rebootcount    = cnfg_data->boot.rebootcount;    // �V�X�e���N����̃J�����N�����s���ɃJ���������ċN�������[count]
    data->boot.rebootingtimer = cnfg_data->boot.rebootingtimer; // �J�����ċN�����ł��邱�Ƃ�ʒm���鎞��[msec]

    return TRUE;
}

/// @brief �X�Όv�ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_TILTMETER data)
{
    PCONFIG_TILTMETER cnfg_data = &m_app_cnfg.tiltmeter;    // �X�Όv�ݒ�

    cnfg_data->canopen.setup_fname = data.canopen.setup_fname;  // CANopen set-up .ini file name
    cnfg_data->canopen.port        = data.canopen.port;         // CANopen port (port1(=0) or port2(=1))
    cnfg_data->canopen.node_id     = data.canopen.node_id;      // CANopen node-ID

    cnfg_data->dr_filter = data.dr_filter;  // �X�Όv�t�B���^�[(�ċA�`�f�B�W�^���t�B���^�[)
    cnfg_data->ma_filter = data.ma_filter;  // �X�Όv�t�B���^�[(�ړ����σt�B���^�[)

    cnfg_data->tiltspdfilter = data.tiltspdfilter;  // �X�Ίp���x�t�B���^�[���萔

    return TRUE;
}

/// @brief �X�Όv�ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_TILTMETER data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_TILTMETER cnfg_data = &m_app_cnfg.tiltmeter;    // �X�Όv�ݒ�

    data->canopen.setup_fname = cnfg_data->canopen.setup_fname;  // CANopen set-up .ini file name
    data->canopen.port        = cnfg_data->canopen.port;         // CANopen port (port1(=0) or port2(=1))
    data->canopen.node_id     = cnfg_data->canopen.node_id;      // CANopen node-ID

    data->dr_filter = cnfg_data->dr_filter; // �X�Όv�t�B���^�[(�ċA�`�f�B�W�^���t�B���^�[)
    data->ma_filter = cnfg_data->ma_filter; // �X�Όv�t�B���^�[(�ړ����σt�B���^�[)

    data->tiltspdfilter = cnfg_data->tiltspdfilter; // �X�Ίp���x�t�B���^�[���萔

    return TRUE;
}

/// @brief ��t���@�ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_MOUNTING data)
{
    PCONFIG_MOUNTING cnfg_data = &m_app_cnfg.mounting;  // ��t���@�ݒ�

    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        cnfg_data->offset[idx].l0 = data.offset[idx].l0;    // �J������]������L0[mm]
        cnfg_data->offset[idx].t0 = data.offset[idx].t0;    // �J������t�p�x��0(*10^6)[rad]
        cnfg_data->offset[idx].p0 = data.offset[idx].p0;    // �J�����ݒu�p�x��0(*10^6)[rad]
    }

    return TRUE;
}

/// @brief ��t���@�ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_MOUNTING data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_MOUNTING cnfg_data = &m_app_cnfg.mounting;  // ��t���@�ݒ�

    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        data->offset[idx].l0 = cnfg_data->offset[idx].l0;   // �J������]������L0[mm]
        data->offset[idx].t0 = cnfg_data->offset[idx].t0;   // �J������t�p�x��0(*10^6)[rad]
        data->offset[idx].p0 = cnfg_data->offset[idx].p0;   // �J�����ݒu�p�x��0(*10^6)[rad]
    }

    return TRUE;
}

/// @brief �摜���������ݒ菑����
/// @param
/// @return 
/// @note 
BOOL CShared::set_app_config(CONFIG_IMGPROC data)
{
    PCONFIG_IMGPROC cnfg_data = &m_app_cnfg.imageprocess;   // �摜���������ݒ�

    // ROI�ݒ�
    cnfg_data->roi.valid = data.roi.valid;  // ROI�L���ݒ�(����[FALSE]/�L��[TRUE])
    cnfg_data->roi.scale = data.roi.scale;  // ROI�X�P�[��(���o�����^�[�Q�b�g�ɑ΂���{��)

    // �}�X�N�ݒ�
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        cnfg_data->mask[idx].valid   = data.mask[idx].valid;    // �}�X�N�I��(����[FALSE]/�L��[TRUE])
        for (int hsv = 0; hsv < static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX); hsv++) {
            cnfg_data->mask[idx].hsv_l[hsv] = data.mask[idx].hsv_l[hsv];    // HSV�}�X�N����l(����)
            cnfg_data->mask[idx].hsv_u[hsv] = data.mask[idx].hsv_u[hsv];    // HSV�}�X�N����l(���)
        }
    }

    // �t�B���^�[
    for (uint32_t filter = 0; filter < static_cast<uint32_t>(ENUM_NOISE_FILTER::E_MAX); filter++) {
        cnfg_data->filter[filter].type = data.filter[filter].type;  // �t�B���^�[���
        cnfg_data->filter[filter].val  = data.filter[filter].val;   // �t�B���^�[�l
    }

    cnfg_data->imgprc        = data.imgprc;         // �^�[�Q�b�g���o�A���S���Y��(0=�ő�֊s�ʐ�, 1=�ő�֊s��)
    cnfg_data->swayspdfilter = data.swayspdfilter;  // �U�ꑬ�x�t�B���^�[���萔

    cnfg_data->swayzeroset_time = data.swayzeroset_time;    // �U��[���_�ݒ�(�v������[ms])

    return TRUE;
}

/// @brief �摜���������ݒ�Ǎ���
/// @param
/// @return 
/// @note 
BOOL CShared::get_app_config(PCONFIG_IMGPROC data)
{
    if (data == NULL) {
        return FALSE;
    }

    PCONFIG_IMGPROC cnfg_data = &m_app_cnfg.imageprocess;   // �摜���������ݒ�

    // ROI�ݒ�
    data->roi.valid = cnfg_data->roi.valid; // ROI�L���ݒ�(����[FALSE]/�L��[TRUE])
    data->roi.scale = cnfg_data->roi.scale; // ROI�X�P�[��(���o�����^�[�Q�b�g�ɑ΂���{��)

    // �}�X�N�ݒ�
    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        data->mask[idx].valid   = cnfg_data->mask[idx].valid;           // �}�X�N�I��(����[FALSE]/�L��[TRUE])
        for (int hsv = 0; hsv < static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX); hsv++) {
            data->mask[idx].hsv_l[hsv] = cnfg_data->mask[idx].hsv_l[hsv];   // HSV�}�X�N����l(����)
            data->mask[idx].hsv_u[hsv] = cnfg_data->mask[idx].hsv_u[hsv];   // HSV�}�X�N����l(���)
        }
    }

    // �S�}���m�C�Y�t�B���^�[
    for (uint32_t filter = 0; filter < static_cast<uint32_t>(ENUM_NOISE_FILTER::E_MAX); filter++) {
        data->filter[filter].type = cnfg_data->filter[filter].type; // �t�B���^�[���
        data->filter[filter].val  = cnfg_data->filter[filter].val;  // �t�B���^�[�l
    }

    data->imgprc        = cnfg_data->imgprc;        // �^�[�Q�b�g���o�A���S���Y��(0=�ő�֊s�ʐ�, 1=�ő�֊s��)
    data->swayspdfilter = cnfg_data->swayspdfilter; // �U�ꑬ�x�t�B���^�[���萔

    data->swayzeroset_time = cnfg_data->swayzeroset_time;   // �U��[���_�ݒ�(�v������[ms])

    return TRUE;
}

/// @brief �摜�f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(uint8_t id, cv::Mat cv_mat)
{
    if (id >= static_cast<uint32_t>(ENUM_IMAGE::E_MAX)) {
        return FALSE;
    }

    PINFO_IMGBUF_DATA info_data = &m_app_info.imgbuf[id].data;  // �摜���f�[�^

    EnterCriticalSection(&m_app_info.imgbuf[id].cs);

    cv_mat.copyTo(info_data->cv_mat);   // CpenCV

    LeaveCriticalSection(&m_app_info.imgbuf[id].cs);

    return TRUE;
}

/// @brief �摜�f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(uint8_t id, cv::Mat* cv_mat)
{
    if (id >= static_cast<uint32_t>(ENUM_IMAGE::E_MAX)) {
        return FALSE;
    }
    if (cv_mat == NULL) {
        return FALSE;
    }

    PINFO_IMGBUF_DATA info_data = &m_app_info.imgbuf[id].data;  // �摜���f�[�^

    EnterCriticalSection(&m_app_info.imgbuf[id].cs);

    info_data->cv_mat.copyTo(*cv_mat);  // CpenCV

    LeaveCriticalSection(&m_app_info.imgbuf[id].cs);

    return TRUE;
}

/// @brief �X�Όv���f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(INFO_TILTMT_DATA data)
{
    PINFO_TILTMT_DATA info_data = &m_app_info.tiltmt.data;  // �X�Όv���f�[�^

    EnterCriticalSection(&m_app_info.tiltmt.cs);

    info_data->status = data.status;    // �X�e�[�^�X���
    for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        info_data->tilt[axis].deg       = data.tilt[axis].deg;          // �X�Ίp[deg]
        info_data->tilt[axis].rad       = data.tilt[axis].rad;          // �X�Ίp[rad]
        info_data->tilt[axis].speed_deg = data.tilt[axis].speed_deg;    // �X�Ίp���x[deg/s]
        info_data->tilt[axis].speed_rad = data.tilt[axis].speed_rad;    // �X�Ίp���x[rad/s]
    }

    LeaveCriticalSection(&m_app_info.tiltmt.cs);

    return TRUE;
}

/// @brief �X�Όv���f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(PINFO_TILTMT_DATA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PINFO_TILTMT_DATA info_data = &m_app_info.tiltmt.data;  // �X�Όv���f�[�^

    EnterCriticalSection(&m_app_info.tiltmt.cs);

    data->status = info_data->status;   // �X�e�[�^�X���
    for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        data->tilt[axis].deg       = info_data->tilt[axis].deg;         // �X�Ίp[deg]
        data->tilt[axis].rad       = info_data->tilt[axis].rad;         // �X�Ίp[deg]
        data->tilt[axis].speed_deg = info_data->tilt[axis].speed_deg;   // �X�Ίp���x[deg/s]
        data->tilt[axis].speed_rad = info_data->tilt[axis].speed_rad;   // �X�Ίp���x[rad/s]
    }

    LeaveCriticalSection(&m_app_info.tiltmt.cs);

    return TRUE;
}

/// @brief �N���C�A���g���f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(INFO_CLIENT_DATA data)
{
    PINFO_CLIENT_DATA info_data = &m_app_info.client.data;  // �N���C�A���g���f�[�^

    EnterCriticalSection(&m_app_info.client.cs);

    info_data->device_id = data.device_id;  // �@��ID
    info_data->command   = data.command;    // �R�}���h
    info_data->mode      = data.mode;       // ���[�h
    info_data->cycle     = data.cycle;      // �ŏ���M����[ms]
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
        info_data->tgt_len[idx] = data.tgt_len[idx];    // �^�[�Q�b�g����[mm]
    }

    LeaveCriticalSection(&m_app_info.client.cs);

    return TRUE;
}

/// @brief �N���C�A���g���f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(PINFO_CLIENT_DATA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PINFO_CLIENT_DATA info_data = &m_app_info.client.data;  // �N���C�A���g���f�[�^

    EnterCriticalSection(&m_app_info.client.cs);

    data->device_id = info_data->device_id; // �@��ID
    data->command   = info_data->command;   // �R�}���h
    data->mode      = info_data->mode;      // ���[�h
    data->cycle     = info_data->cycle;     // �ŏ���M����[ms]
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
        data->tgt_len[idx] = info_data->tgt_len[idx];   // �^�[�Q�b�g����[mm]
    }

    LeaveCriticalSection(&m_app_info.client.cs);

    return TRUE;
}

/// @brief �������f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(INFO_ADJUST_DATA data)
{
    PINFO_ADJUST_DATA info_data = &m_app_info.adjust.data;  // �������f�[�^

    EnterCriticalSection(&m_app_info.adjust.cs);

    info_data->target_distance_fixed = data.target_distance_fixed;  // �ݓ_�`�^�[�Q�b�g�ԋ����Œ�
    info_data->target_distance       = data.target_distance;        // �ݓ_�`�^�[�Q�b�g�ԋ���

    LeaveCriticalSection(&m_app_info.adjust.cs);

    return TRUE;
}

/// @brief �������f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(PINFO_ADJUST_DATA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PINFO_ADJUST_DATA info_data = &m_app_info.adjust.data;  // �������f�[�^

    EnterCriticalSection(&m_app_info.adjust.cs);

    data->target_distance_fixed = info_data->target_distance_fixed; // �ݓ_�`�^�[�Q�b�g�ԋ����Œ�
    data->target_distance       = info_data->target_distance;       // �ݓ_�`�^�[�Q�b�g�ԋ���

    LeaveCriticalSection(&m_app_info.adjust.cs);

    return TRUE;
}

/// @brief �摜�������f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(INFO_IMGPRC_DATA data)
{
    PINFO_IMGPRC_DATA info_data = &m_app_info.imgprc.data;  // �摜�������f�[�^
 
    EnterCriticalSection(&m_app_info.imgprc.cs);
   
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        info_data->contours[idx] = data.contours[idx];  // �֊s���W
    }

    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        info_data->target_data[idx].valid   = data.target_data[idx].valid;      // ���o���
        info_data->target_data[idx].max_val = data.target_data[idx].max_val;    // �ő�P�x
        for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            info_data->target_data[idx].pos[axis] = data.target_data[idx].pos[axis];    // ���o�ʒu[pixel]
        }
        info_data->target_data[idx].size       = data.target_data[idx].size;        // ���o�T�C�Y
        info_data->target_data[idx].roi.x      = data.target_data[idx].roi.x;       // ROI:x coordinate of the top-left corner
        info_data->target_data[idx].roi.y      = data.target_data[idx].roi.y;       // ROI:y coordinate of the top-left corner
        info_data->target_data[idx].roi.width  = data.target_data[idx].roi.width;   // ROI:width of the rectangle
        info_data->target_data[idx].roi.height = data.target_data[idx].roi.height;  // ROI:height of the rectangle
    }

    for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        info_data->sway_data[axis].target_pos  = data.sway_data[axis].target_pos;   // �^�[�Q�b�g�ʒu[pixel]
        info_data->sway_data[axis].target_tilt = data.sway_data[axis].target_tilt;  // �^�[�Q�b�g�X��[pixel]
        info_data->sway_data[axis].sway_angle  = data.sway_data[axis].sway_angle;   // �U��p[pixel]
        info_data->sway_data[axis].sway_speed  = data.sway_data[axis].sway_speed;   // �U�ꑬ�x[pixel/s]
        info_data->sway_data[axis].sway_zero   = data.sway_data[axis].sway_zero;    // �U�ꒆ�S[pixel]
    }

    info_data->target_size = data.target_size;  // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
    info_data->status      = data.status;       // ���
    info_data->img_fps     = data.img_fps;      // �t���[�����[�g[fps]
    info_data->img_val     = data.img_val;      // ���x
    info_data->exps_mode   = data.exps_mode;    // �V���b�^�R���g���[�����[�h(0:��~ 1:Up -1:Down)
    info_data->exps_time   = data.exps_time;    // �I������[us]

    LeaveCriticalSection(&m_app_info.imgprc.cs);

    return TRUE;
}

/// @brief �摜�������f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(PINFO_IMGPRC_DATA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PINFO_IMGPRC_DATA info_data = &m_app_info.imgprc.data;  // �摜�������f�[�^

    EnterCriticalSection(&m_app_info.imgprc.cs);

    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        data->contours[idx] = info_data->contours[idx]; // �֊s���W
    }

    for (int idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        data->target_data[idx].valid   = info_data->target_data[idx].valid;     // ���o���
        data->target_data[idx].max_val = info_data->target_data[idx].max_val;   // �ő�P�x
        for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            data->target_data[idx].pos[axis] = info_data->target_data[idx].pos[axis];   // ���o�ʒuX[pixel]
        }
        data->target_data[idx].size       = info_data->target_data[idx].size;       // ���o�T�C�Y
        data->target_data[idx].roi.x      = info_data->target_data[idx].roi.x;      // ROI:x coordinate of the top-left corner
        data->target_data[idx].roi.y      = info_data->target_data[idx].roi.y;      // ROI:y coordinate of the top-left corner
        data->target_data[idx].roi.width  = info_data->target_data[idx].roi.width;  // ROI:width of the rectangle
        data->target_data[idx].roi.height = info_data->target_data[idx].roi.height; // ROI:height of the rectangle
    }

    for (int axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        data->sway_data[axis].target_pos  = info_data->sway_data[axis].target_pos;  // �^�[�Q�b�g�ʒu[pixel]
        data->sway_data[axis].target_tilt = info_data->sway_data[axis].target_tilt; // �^�[�Q�b�g�X��[pixel]
        data->sway_data[axis].sway_angle  = info_data->sway_data[axis].sway_angle;  // �U��p[pixel]
        data->sway_data[axis].sway_speed  = info_data->sway_data[axis].sway_speed;  // �U�ꑬ�x[pixel/s]
        data->sway_data[axis].sway_zero   = info_data->sway_data[axis].sway_zero;   // �U�ꒆ�S[pixel]
    }

    data->target_size = info_data->target_size; // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
    data->status      = info_data->status;      // ���
    data->img_fps     = info_data->img_fps;     // �t���[�����[�g[fps]
    data->img_val     = info_data->img_val;     // ���x
    data->exps_mode   = info_data->exps_mode;   // �V���b�^�R���g���[�����[�h(0:��~ 1:Up -1:Down)
    data->exps_time   = info_data->exps_time;   // �I������[us]
 
    LeaveCriticalSection(&m_app_info.imgprc.cs);

    return TRUE;
}

/// @brief �V�X�e�����f�[�^������
/// @param
/// @return 
/// @note
BOOL CShared::set_app_info_data(INFO_SYSTEM_DATA data)
{
    PINFO_SYSTEM_DATA info_data = &m_app_info.system.data;  // �V�X�e�����f�[�^

    EnterCriticalSection(&m_app_info.system.cs);

    info_data->status = data.status;    // �X�e�[�^�X

    LeaveCriticalSection(&m_app_info.system.cs);

    return TRUE;
}

/// @brief �V�X�e�����f�[�^�Ǎ���
/// @param
/// @return 
/// @note
BOOL CShared::get_app_info_data(PINFO_SYSTEM_DATA data)
{
    if (data == NULL) {
        return FALSE;
    }

    PINFO_SYSTEM_DATA info_data = &m_app_info.system.data;  // �V�X�e�����f�[�^

    EnterCriticalSection(&m_app_info.system.cs);

    data->status = info_data->status;   // �X�e�[�^�X

    LeaveCriticalSection(&m_app_info.system.cs);

    return TRUE;
}
