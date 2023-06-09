#include "CAgent.h"
#include "Resource.h"
#include <time.h>
#include <mmsystem.h>

//////////////////////////////////////////////////////////////////////////////
// CAgent

#pragma comment(lib, "SockUDP.lib")

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CAgent::CAgent()
{
    m_shared = NULL;    // CShared�N���X

    m_td_comm_send_hndl = NULL;     // �X���b�h�n���h��
    m_td_comm_send_stat = FALSE;    // �X���b�h�X�e�[�^�X(FALSE:Exit TRUE:Run)

    m_sockrecv = NULL;  // Socket(��M)
    m_socksend = NULL;  // Socket(���M)
}

/// @brief
/// @param
/// @return
/// @note
CAgent::~CAgent()
{
    //----------------------------------------------------------------------------
    // �I������
    close();
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief
/// @param
/// @return
/// @note
void CAgent::init_task(void* param)
{
    CAgent* myclass = reinterpret_cast<CAgent*>(param); // ���N���X�C���X�^���X�|�C���^

    //----------------------------------------------------------------------------
    // ����������
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
void CAgent::routine_work(void* param)
{
    CAgent*       myclass = reinterpret_cast<CAgent*>(param);   // ���N���X�C���X�^���X�|�C���^
    LARGE_INTEGER frequency;                // �V�X�e���̎��g��
    LARGE_INTEGER start_count, end_count;   // ���݂̃J�E���g��
    LONGLONG      span_usec;                // ���Ԃ̊Ԋu[usec]

    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::routine_work]<Error>Object pointer");
        return ;
    }

    //----------------------------------------------------------------------------
    // �������Ԍv��(�J�n���Ԏ擾)
    QueryPerformanceFrequency(&frequency);  // �V�X�e���̎��g��
    QueryPerformanceCounter(&start_count);  // ���݂̃J�E���g��

    //----------------------------------------------------------------------------
    // ���C������
    myclass->proc_main();

    //----------------------------------------------------------------------------
    // �������Ԍv��(�I�����Ԏ擾)
    QueryPerformanceCounter(&end_count);    // ���݂̃J�E���g��
    span_usec = ((end_count.QuadPart - start_count.QuadPart) * 1000000L) / frequency.QuadPart;  // ���Ԃ̊Ԋu[usec]

    //----------------------------------------------------------------------------
    // ���b�Z�[�W�\��
    std::wstring str;
    str = std::format(L"���s�����:{:4d} ��������[ms]:{:4d}", (*(myclass->thread_info.psys_counter) % 1000),
                                                             static_cast<uint32_t>(span_usec / 1000));
    myclass->outp_msg_to_mainwnd(str);

    return ;
}

/// @brief ����������
/// @param
/// @return 
/// @note
HRESULT CAgent::initialize(void)
{
    HRESULT      ret = S_OK;
    std::wstring msg;

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    // CShared�N���X�I�u�W�F�N�g�̐���
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CShared");
            msg = std::format(L"<Error>Failed to create CShared");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CShared");
    outp_msg_to_listview(msg);

    // ���L�f�[�^�Ǎ���
    m_shared->get_app_config(&m_cnfgsys);           // �V�X�e���ݒ�
    m_shared->get_app_config(&m_cnfgnet);           // �ʐM�ݒ�
    m_shared->get_app_config(&m_cnfgcam);           // �J�����ݒ�
    m_shared->get_app_config(&m_cnfgmnt);           // ��t���@�ݒ�
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoclt_data);   // �N���C�A���g���f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^

    // ���L�f�[�^������
    m_infoclt_data.device_id = L""; // �@��ID
    m_infoclt_data.command   = 0;   // �R�}���h
    m_infoclt_data.mode      = 0;   // ���[�h
    m_infoclt_data.cycle     = 0;   // �ŏ���M����[ms]
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
        m_infoclt_data.tgt_len[idx] = EXTN_TARGETDIST_MIN;  // �^�[�Q�b�g����[mm]
    }

    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infoclt_data);    // �N���C�A���g���f�[�^

    //----------------------------------------------------------------------------
    // �����o�[�ϐ��̏�����
    ZeroMemory(&m_recv_data, sizeof(m_recv_data));  // ��M�f�[�^
    ZeroMemory(&m_send_data, sizeof(m_send_data));  // ���M�f�[�^

    QueryPerformanceCounter(&m_send_cycle_counter); // ���݂̃J�E���g��

    //----------------------------------------------------------------------------
    // �X���b�h�N��
    m_td_comm_send_stat = TRUE;
    if ((m_td_comm_send_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                       0,
                                                                       td_comm_send,
                                                                       reinterpret_cast<LPVOID>(this),
                                                                       0,
                                                                       NULL))) == NULL) {
        m_td_comm_send_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::initialize]<Error>Thread(td_comm_send) handle");
        std::wstring msg = std::format(L"<Error>Failed to start td_comm_send() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_comm_send_hndl, THREAD_PRIORITY_NORMAL);

    //--------------------------------------------------------------------------
    // Winsock�̏�����(��M)
    if (m_sockrecv == NULL) {
        if ((m_sockrecv = new CSockUDPLib()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CSockUDPLib");
            msg = std::format(L"<Error>Failed to create CSockUDPLib");
            outp_msg_to_listview(msg);

            ret = E_FAIL;
        }
        else {
            if (m_sockrecv->initialize() != SOCK_ERROR_NONE) {
                delete m_sockrecv;
                m_sockrecv = NULL;

                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>m_sockrecv->initialize()");
                msg = std::format(L"<Error>Failed in CSockUDPLib::initialize()");
                outp_msg_to_listview(msg);

                ret = E_FAIL;
            }
            else {
                //--------------------------------------------------------------------------
                // Socket�̃I�[�v��
                if (m_sockrecv->get_sock_status() != SOCK_STATUS_OPEN) {
                    std::string str = CHelper::conv_string(m_cnfgnet.local.ipaddress);
                    m_sockrecv->open(SOCK_TYPE_RECV, SOCK_NONBLOCKING,
                                     const_cast<char*>(str.c_str()), m_cnfgnet.local.port, NULL);

                    msg = std::format(L"<Information>Opened recieve socket");
                    outp_msg_to_listview(msg);
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // Winsock�̏�����(���M)
    if (m_socksend == NULL) {
        if ((m_socksend = new CSockUDPLib()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>Creating of CSockUDPLib");
            msg = std::format(L"<Error>Failed to create CSockUDPLib");
            outp_msg_to_listview(msg);

            ret = E_FAIL;
        }
        else {
            if (m_socksend->initialize() != SOCK_ERROR_NONE) {
                delete m_socksend;
                m_socksend = NULL;
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::init_task]<Error>m_socksend->initialize()");
                msg = std::format(L"<Error>Failed in CSockUDPLib::initialize()");
                outp_msg_to_listview(msg);

                ret = E_FAIL;
            }
        }
    }

    return ret;
}

/// @brief �I������
/// @param
/// @return 
/// @note
void CAgent::close(void)
{
    //----------------------------------------------------------------------------
    // �X���b�h�̏I��
    // Exposure time control thread function
    if (m_td_comm_send_hndl != NULL) {
        if (m_td_comm_send_stat) {
            m_td_comm_send_stat = FALSE;
        	ResumeThread(m_td_comm_send_hndl);
	        WaitForSingleObject(m_td_comm_send_hndl, 500);  // It waits until the thread ends.
        }
        CloseHandle(m_td_comm_send_hndl);   // Handle is closed.
        m_td_comm_send_hndl = NULL;
    }
    m_td_comm_send_stat = FALSE;

    //--------------------------------------------------------------------------
    // Socket�̃N���[�Y(���M)
    if (m_socksend != NULL) {
        m_socksend->close();
    }

    //--------------------------------------------------------------------------
    // Socket�̃N���[�Y(��M)
    if (m_sockrecv != NULL) {
        m_sockrecv->close();
    }

    //----------------------------------------------------------------------------
    // CShared�N���X�I�u�W�F�N�g�̍폜
    if (m_shared != NULL) {
        delete m_shared;
        m_shared = NULL;
    }

    return ;
}

/// @brief ���C������
/// @param
/// @return 
/// @note
void CAgent::proc_main(void)
{
    int size;
    int err;

    //----------------------------------------------------------------------------
    // ���L�f�[�^�Ǎ���
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^

    //----------------------------------------------------------------------------
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN)) {
        m_infoclt_data.command &= ~static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1);
    }

    //--------------------------------------------------------------------------
    // ���M�f�[�^�̃Z�b�g
    // �@��ID
    std::string str;
    str = CHelper::conv_string(m_cnfgsys.my_id);
    memcpy(&m_send_data.device_id, str.c_str(), sizeof(m_send_data.device_id)); // �@��ID

    // SYSTEMTIME
    GetLocalTime(&m_send_data.system_time);

    // �U��Z���T�[���
    for (uint32_t sensor = 0; sensor < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); sensor++) {
        PNW_SEND_DATA_SWAY_SENSOR_INFO sway_sensor_info = &m_send_data.sway_sensor_info[sensor];    // �U��Z���T�[���
        // �@��̏��
        // �J�������
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->sensor_spec_info.camera.image_size[axis] = m_cnfgcam.basis.roi[axis].size;    // �摜�f�[�^��f��[pixel]
            sway_sensor_info->sensor_spec_info.camera.view_angle[axis] = static_cast<uint32_t>(static_cast<double>(m_cnfgcam.basis.roi[axis].size)
                                                                       / (m_cnfgcam.basis.viewangle[axis] * CONV_DEG_RAD)); // ����\[pixel/rad]
        }
        // �J������t���
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->sensor_spec_info.mounting.l0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].l0);                // �J������]������L0[mm]
            sway_sensor_info->sensor_spec_info.mounting.t0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].t0 * pow(10, 6));   // �J������t�p�x��0(*10^6)[rad]
            sway_sensor_info->sensor_spec_info.mounting.p0[axis] = static_cast<uint32_t>(m_cnfgmnt.offset[axis].p0 * pow(10, 6));   // �J�����ݒu�p�x��0(*10^6)[rad]
        }

        // �@���ԏ��
        // ���o���[�h
        sway_sensor_info->sensor_stat_info.mode = 0;

        // �G���[���
        sway_sensor_info->sensor_stat_info.error = 0;
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::CAMERA);      // �@��ُ�(�J����)
        }
        if (m_infotlt_data.status &= (static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB) | 
                                      static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::TILT);        // �@��ُ�(�X�Όv)
        }
        if (!(m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE))) {
            sway_sensor_info->sensor_stat_info.error |= static_cast<uint32_t>(ENUM_NW_ERR_SENSOR::TARGET_1);    // ���o�ُ�(�^�[�Q�b�g1)
        }

        // ���o���
        sway_sensor_info->sensor_stat_info.stat  = 0;
        sway_sensor_info->sensor_stat_info.stat |= static_cast<uint32_t>(ENUM_NW_STAT_SENSOR::TARGET_1);    // �U�ꌟ�o��(�^�[�Q�b�g1)
        if ((m_infoprc_data.status &= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_RUN)) ||
            (m_infoprc_data.status &= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_MCC_RUN))) {
            sway_sensor_info->sensor_stat_info.stat |= static_cast<uint32_t>(ENUM_NW_STAT_SENSOR::SET_ZERO_CAMERA); // 0�_�ݒ蒆(�J����)
        }

        // �X�Όv���
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            sway_sensor_info->tilt_info[axis].angle = static_cast<uint32_t>(static_cast<double>(m_infotlt_data.tilt[axis].rad) * pow(10, 6));       // �X�Ίp(*100000)[rad]
            sway_sensor_info->tilt_info[axis].speed = static_cast<uint32_t>(static_cast<double>(m_infotlt_data.tilt[axis].speed_rad) * pow(10, 6)); // �X�Ίp���x(*100000)[rad/s]
        }

        // �U�ꌟ�o���
        for (uint32_t target = 0; target < static_cast<uint32_t>(ENUM_TARGET::E_MAX); target++) {
            PNW_SEND_DATA_SWAY_INFO sway_info = &sway_sensor_info->sway_info[target];   // �U�ꌟ�o���
            // �U����
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                sway_info->sway.angle[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_angle); // �U��p[pixel]
                sway_info->sway.speed[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_speed); // �U�ꑬ�x[pixel/s]
                sway_info->sway.zero[axis]  = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].sway_zero);  // �U�ꒆ�S�ʒu[pixel]
            }
            // �^�[�Q�b�g���
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                sway_info->target.tilt[axis] = static_cast<uint32_t>(m_infoprc_data.sway_data[axis].target_tilt);   // �^�[�Q�b�g�X��[pixel]
            }
            sway_info->target.size = static_cast<uint32_t>(m_infoprc_data.target_size); // �^�[�Q�b�g�T�C�Y[pixel]
        }

//@@@��
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.angle[static_cast<uint32_t>(ENUM_AXIS::X)] = 10;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.angle[static_cast<uint32_t>(ENUM_AXIS::Y)] = 20;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.speed[static_cast<uint32_t>(ENUM_AXIS::X)] = 30;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.speed[static_cast<uint32_t>(ENUM_AXIS::Y)] = 40;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.zero[static_cast<uint32_t>(ENUM_AXIS::X)]   = 50;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].sway.zero[static_cast<uint32_t>(ENUM_AXIS::Y)]   = 60;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.tilt[static_cast<uint32_t>(ENUM_AXIS::X)] = 70;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)] = 80;
        sway_sensor_info->sway_info[static_cast<uint32_t>(ENUM_TARGET::TARGET_2)].target.size = 90;
//@@@��

        // �������
//@@@��
        if (sensor == static_cast<uint32_t>(ENUM_SWAY_SENSOR::SENSOR_1)) {
            sway_sensor_info->message[0] = 's';
            sway_sensor_info->message[1] = 'e';
            sway_sensor_info->message[2] = 'n';
            sway_sensor_info->message[3] = 's';
            sway_sensor_info->message[4] = 'o';
            sway_sensor_info->message[5] = 'r';
            sway_sensor_info->message[6] = '1';
            sway_sensor_info->message[7] = '\0';
        }
        else {
            sway_sensor_info->message[0] = 's';
            sway_sensor_info->message[1] = 'e';
            sway_sensor_info->message[2] = 'n';
            sway_sensor_info->message[3] = 's';
            sway_sensor_info->message[4] = 'o';
            sway_sensor_info->message[5] = 'r';
            sway_sensor_info->message[6] = '2';
            sway_sensor_info->message[7] = '\0';
        }
//@@@��
    }

    //--------------------------------------------------------------------------
    // �f�[�^��M����
    if (m_sockrecv != NULL) {//CSockUDP�̃C���X�^���Xinitialize()��new()
        // socket�̃I�[�v��
        if (m_sockrecv->get_sock_status() != SOCK_STATUS_OPEN) {
            std::string str = CHelper::conv_string(m_cnfgnet.local.ipaddress);
            m_sockrecv->open(SOCK_TYPE_RECV, SOCK_NONBLOCKING,
                             const_cast<char*>(str.c_str()), m_cnfgnet.local.port, NULL);
  
            std::wstring msg = std::format(L"<Information>Opened receive socket");
            outp_msg_to_listview(msg);
        }
        // �f�[�^��M
        if (m_sockrecv->get_sock_status() == SOCK_STATUS_OPEN) {
            size = m_sockrecv->recv_data(reinterpret_cast<char*>(&m_recv_data), sizeof(m_recv_data),
                                         NULL, &err);
            if (size > 0) {
                if (size == sizeof(m_recv_data)) {
                    // ��M�f�[�^���
                    str = CHelper::conv_string(m_cnfgsys.mcc_id);
                    if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(),
                                NW_SIZE_DEVICE_ID) == 0) {
                        // �R�}���h���
                        // �R�}���h�F�f�[�^���M�v��(�����)
                        if (m_recv_data.command_info.command & static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_CYCLE)) {
                            m_infoclt_data.cycle = m_recv_data.command_info.cycle;  // �ŏ���M����[msec]
                        }
                        // �R�}���h�F�f�[�^���M��~�v��
                        else if (m_recv_data.command_info.command & static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_STOP)) {
                            m_infoclt_data.cycle = 0;   // �ŏ���M����[msec]
                        }
                        // �R�}���h�F�f�[�^���M�v��(1�V���b�g)
                        else {
                            if (m_td_comm_send_hndl != NULL) {
                                ResumeThread(m_td_comm_send_hndl);
                            }
                        }
                        // �R�}���h�F0�_�ݒ�
                        uint32_t command = 0;
                        command = (static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_CAMERA_2)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_TILT_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::SET_ZERO_TILT_2));
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }
                        // �R�}���h�F���Z�b�g�v��
                        command = (static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_CAMERA_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_CAMERA_2)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_TILT_1)
                                |  static_cast<uint32_t>(ENUM_NW_COMMAND::RESET_TILT_2));
                        m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }
                        // �R�}���h�F�摜���M�v��
                        command = static_cast<uint32_t>(ENUM_NW_COMMAND::REQ_DATA_STOP);
                        if (m_recv_data.command_info.command & command) {
                            m_infoclt_data.command |= (m_recv_data.command_info.command & command);
                        }

                        // ���[�h�ݒ�
                        m_infoclt_data.mode = m_recv_data.command_info.mode;

                        // �J�����`�^�[�Q�b�g�ԋ���[mm]
                        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_SWAY_SENSOR::E_MAX); idx++) {
                            //@@@�J�������Ƃɕ�����
                            m_infoclt_data.tgt_len[idx] = static_cast<double>(m_recv_data.command_info.tgt_len[idx]);
                        }
                    }   // if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(), NW_SIZE_DEVICE_ID) == 0)
                    else {
                        ;   // �@��ID�ُ�
                    }   // if (strncmp(reinterpret_cast<char*>(m_recv_data.device_id), str.c_str(), NW_SIZE_DEVICE_ID) == 0) else
                }
                else {
                    ;   // ��M�f�[�^�T�C�Y�G���[
                }
            }   // if (size > 0)
            else {
                // �ُ폈��
                if (err != WSAEWOULDBLOCK) {
                    m_sockrecv->close();    // Socket�̃N���[�Y
                }
            }   // if (size > 0) else
        }   // if (m_sockrecv->get_sock_status() == SOCK_STATUS_OPEN)
    }   // if (m_sockrecv != NULL)

    //--------------------------------------------------------------------------
    // �f�[�^���M(�����)�v��
    LARGE_INTEGER frequency;    // �V�X�e���̎��g��
    LARGE_INTEGER start_count;  // ���݂̃J�E���g��
    LONGLONG      span_usec;    // ���Ԃ̊Ԋu[usec]

    QueryPerformanceFrequency(&frequency);  // �V�X�e���̎��g��
    QueryPerformanceCounter(&start_count);  // ���݂̃J�E���g��
    span_usec = ((start_count.QuadPart - m_send_cycle_counter.QuadPart) * 1000000L) / frequency.QuadPart;   // ���Ԃ̊Ԋu[usec]
    if (span_usec >= 0) {
        if (m_infoclt_data.cycle > 0) {
            if (static_cast<uint16_t>(span_usec / 1000) >= m_infoclt_data.cycle) {
                if (m_td_comm_send_hndl != NULL) {
                    ResumeThread(m_td_comm_send_hndl);
                }
                QueryPerformanceCounter(&m_send_cycle_counter); // ���݂̃J�E���g��
            }
        }
        else {
            QueryPerformanceCounter(&m_send_cycle_counter); // ���݂̃J�E���g��
        }
    }
    else {
        QueryPerformanceCounter(&m_send_cycle_counter); // ���݂̃J�E���g��
    }

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infoclt_data);    // �N���C�A���g���f�[�^

    return ;
}

/// @brief �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CAgent::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"�ƭ�1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), wstr.c_str()); wstr.clear();

    wstr = L"�ƭ�2";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), wstr.c_str()); wstr.clear();

    wstr = L"�ƭ�3";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), wstr.c_str()); wstr.clear();

    wstr = L"�ƭ�4";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), wstr.c_str()); wstr.clear();

    wstr = L"�ƭ�5";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), wstr.c_str()); wstr.clear();

    wstr = L"�ƭ�6";
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

/// @brief
/// @param
/// @return
/// @note
void CAgent::set_tab_tip_txt(void)
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

/// @brief �^�u�p�l���̐ݒ�
/// @param
/// @return 
/// @note
void CAgent::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
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
            // �@�\�I��ID�Z�b�g
            thread_info.panel_func_id = LOWORD(wp);
            set_tab_tip_txt();  // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
        case IDC_BUTTON_TASK_ITEM1:
        case IDC_BUTTON_TASK_ITEM2:
        case IDC_BUTTON_TASK_ITEM3:
        case IDC_BUTTON_TASK_ITEM4:
        case IDC_BUTTON_TASK_ITEM5:
        case IDC_BUTTON_TASK_ITEM6:
            // �@�\����ID�Z�b�g
            thread_info.panel_type_id = LOWORD(wp);
            // �eFUNCTION PB����
            switch (thread_info.panel_func_id) {
            case IDC_RADIO_TASK_FUNC1:
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
            break;
        default:
            break;
        }
        break;
    }

    return ;
}

/// @brief �f�[�^���M�X���b�h�֐�
/// @param [in] lpParam
/// @return 0
/// @note
unsigned WINAPI CAgent::td_comm_send(LPVOID lpParam)
{
    CAgent*      myclass = reinterpret_cast<CAgent*>(lpParam);
    std::wstring msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::td_comm_send]<Start>");

    while (myclass->m_td_comm_send_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_comm_send_hndl);    // Suspend thread

        //--------------------------------------------------------------------------
        // �f�[�^���M����
        if (myclass->m_socksend == NULL) {
            continue ;
        }
        if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN) {
            //--------------------------------------------------------------------------
            // Socket�̃I�[�v��
            char     addr[INET_ADDRSTRLEN];
            uint16_t port;

            if (myclass->m_cnfgnet.remote.ipaddress == NW_SEND_IP_AUTO) {
	            inet_ntop(AF_INET, &myclass->m_recv_data.sock.sin_addr, addr, sizeof(addr));
                port = ntohs(myclass->m_recv_data.sock.sin_port);
                myclass->m_socksend->open(SOCK_TYPE_SEND, SOCK_NONBLOCKING, addr, port, NULL);

                msg = std::format(L"<Information>Opened send socket");
                myclass->outp_msg_to_listview(msg);
            }
            else {
                std::string str;
                str = CHelper::conv_string(myclass->m_cnfgnet.remote.ipaddress);
                myclass->m_socksend->open(SOCK_TYPE_SEND, SOCK_NONBLOCKING,
                                          const_cast<char*>(str.c_str()), myclass->m_cnfgnet.remote.port, NULL);

                msg = std::format(L"<Information>Opened send socket");
                myclass->outp_msg_to_listview(msg);
            }
            if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN) {
                continue ;
            }
        }   // if (myclass->m_socksend->get_sock_status() != SOCK_STATUS_OPEN)

        //--------------------------------------------------------------------------
        // �f�[�^���M
        int size;
        int err;
        if (myclass->m_socksend->get_sock_status() == SOCK_STATUS_OPEN) {
            size = myclass->m_socksend->send_data(reinterpret_cast<char*>(&myclass->m_send_data),
                                                  sizeof(m_send_data),
                                                  &err);
            if (err != 0) {
                myclass->m_socksend->close();
                msg = std::format(L"<Error>Send socket error ({:d})", err);
                myclass->outp_msg_to_listview(msg);
            }
        }
    }   // while(myclass->m_td_comm_send_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CAgent::td_comm_send]<End>");
    msg = std::format(L"<Information>Exited td_comm_send() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_comm_send_stat = FALSE;
    _endthreadex(0);

    return 0;
}
