#include "CEnvironment.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////
// CEnvironment

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief Constructor
/// @param
/// @return
/// @note
CEnvironment::CEnvironment()
{
    m_shared      = NULL;   // CShared�N���X
    m_telicam_lib = NULL;   // CTeliCamLib�N���X
    m_canopen_lib = NULL;   // CCANopenLib�N���X

    m_td_camera_start_hndl        = NULL;   // �X���b�h�n���h��
    m_td_camera_start_stat        = FALSE;  // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    m_td_camera_restart_hndl      = NULL;   // �X���b�h�n���h��
    m_td_camera_restart_stat      = FALSE;  // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    m_td_canopen_start_hndl       = NULL;   // �X���b�h�n���h��
    m_td_canopen_start_stat       = FALSE;  // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    m_td_canopen_tilt_preset_hndl = NULL;   // �X���b�h�n���h��
    m_td_canopen_tilt_preset_stat = FALSE;  // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    m_td_canopen_tilt_get_hndl    = NULL;   // �X���b�h�n���h��
    m_td_canopen_tilt_get_stat    = FALSE;  // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)

	m_img_src.data_bgr = NULL;  // �摜�f�[�^�o�b�t�@�̃|�C���^(BGR 24bit)
}

/// @brief Destructor
/// @param
/// @return
/// @note
CEnvironment::~CEnvironment()
{
    //----------------------------------------------------------------------------
    // �I������
    close();
}

/// @brief �摜�f�[�^�擾
/// @param
/// @return 
/// @note
IMAGE_DATA CEnvironment::get_opencv_image(void)
{
    //----------------------------------------------------------------------------
    // �摜�f�[�^�擾(�摜�t�@�C��)
    if (m_cnfgcmn.img_source_camera != static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {
        m_img_src.data_opencv = cv::imread(CHelper::conv_string(m_cnfgcmn.img_source_fname));
        if (m_img_src.data_opencv.data != NULL) {
	        m_img_src.status |= static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED);  // �摜�X�e�[�^�X:�摜�L��
	        m_img_src.width   = m_img_src.data_opencv.cols ;                        // �摜�T�C�Y(������f) [pixel]
	        m_img_src.height  = m_img_src.data_opencv.rows;                         // �摜�T�C�Y(������f) [pixel]
            m_img_src.fps     = m_cnfgcam.basis.framerate;                          // �摜�t���[�����[�g[fps]

            return m_img_src;
        }
	    m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // �摜�X�e�[�^�X:�摜�L��
	    m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // �摜�T�C�Y(������f) [pixel]
	    m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // �摜�T�C�Y(������f) [pixel]
        m_img_src.fps     = 0.0;                                                            // �摜�t���[�����[�g[fps]
    }
 
    //----------------------------------------------------------------------------
    // �摜�f�[�^�擾(�J����)
    if (m_img_src.data_bgr != NULL) {
        if ((m_telicam_lib != NULL) &&
            (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)))) { // �J�����Đڑ���
            // �摜�����擾
            if ((m_telicam_lib->get_image(m_img_src.data_bgr) >= 0) &&
                (m_telicam_lib->get_image_size(&m_img_src.width, &m_img_src.height) >= 0) &&
                (m_telicam_lib->get_image_fps(&m_img_src.fps) >= 0)) {
                m_img_src.status |= (static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));    // �摜�X�e�[�^�X:�摜�L��
            }
            else {
	            m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // �摜�X�e�[�^�X:�摜�L��
	            m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // �摜�T�C�Y(������f) [pixel]
	            m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // �摜�T�C�Y(������f) [pixel]
                m_img_src.fps     = 0.0;                                                            // �摜�t���[�����[�g [fps]
	            ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // �摜�f�[�^�o�b�t�@�̃|�C���^(BGR 24bit)
            }
        }
        else {
	        m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // �摜�X�e�[�^�X:�摜�L��
	        m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // �摜�T�C�Y(������f) [pixel]
	        m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // �摜�T�C�Y(������f) [pixel]
            m_img_src.fps     = 0.0;                                                            // �摜�t���[�����[�g [fps]
	        ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // �摜�f�[�^�o�b�t�@�̃|�C���^(BGR 24bit)
        }
        // OpenCV�摜�ւ̕ϊ�
        m_img_src.data_opencv = cv::Mat(m_img_src.height,
                                        m_img_src.width,
                                        CV_8UC3,
                                        m_img_src.data_bgr);    // �摜�f�[�^(OpenCV�ϊ��摜)    
    }   // if (m_img_src.data_bgr != NULL)
    else {
	    m_img_src.status &= (~static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED));           // �摜�X�e�[�^�X:�摜�L��
	    m_img_src.width   = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;  // �摜�T�C�Y(������f) [pixel]
	    m_img_src.height  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;  // �摜�T�C�Y(������f) [pixel]
        m_img_src.fps     = 0.0;                                                            // �摜�t���[�����[�g[fps]
    }   // else

	return m_img_src;
}

//////////////////////////////////////////////////////////////////////////////
// Private method

/// @brief
/// @param
/// @return
/// @note
void CEnvironment::init_task(void* param)
{
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(param); // ���N���X�C���X�^���X�|�C���^
    std::wstring  msg;

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
void CEnvironment::routine_work(void* param)
{
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(param); // ���N���X�C���X�^���X�|�C���^
    LARGE_INTEGER frequency;                // �V�X�e���̎��g��
    LARGE_INTEGER start_count, end_count;   // ���݂̃J�E���g��
    LONGLONG      span_usec;                // ���Ԃ̊Ԋu[usec]

    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::routine_work]<Error>Object pointer");
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
HRESULT CEnvironment::initialize(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    // CShared�N���X�I�u�W�F�N�g�̐���
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CShared");
            msg = std::format(L"<Error>Failed to create CShared");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CShared");
    outp_msg_to_listview(msg);

    // ���L�f�[�^�Ǎ���
    m_shared->get_app_config(&m_cnfgcmn);           // ���ʐݒ�
    m_shared->get_app_config(&m_cnfgcam);           // �J�����ݒ�
    m_shared->get_app_config(&m_cnfgtlt);           // �X�Όv�ݒ�
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^
    m_shared->get_app_info_data(&m_infosys_data);   // �V�X�e�����f�[�^

    // ���L�f�[�^������
    m_infotlt_data.status = static_cast<uint32_t>(ENUM_TILT_STATUS::DEFAULT);   // �X�Όv���:�f�t�H���g
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_AXIS::E_MAX); idx++) {
        m_infotlt_data.tilt[idx].deg = 0.0; // �X�Ίp[deg]
        m_infotlt_data.tilt[idx].rad = 0.0; // �X�Ίp[deg]
    }

    m_infosys_data.status = static_cast<uint32_t>(ENUM_SYSTEM_STATUS::DEFAULT); // �X�e�[�^�X

    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infotlt_data);    // �X�Όv���f�[�^
    m_shared->set_app_info_data(m_infosys_data);    // �V�X�e�����f�[�^

    //----------------------------------------------------------------------------
    // �����o�[�ϐ��̏�����
    // 
    QueryPerformanceCounter(&m_cycle_time_counter); // �p�t�H�[�}���X�J�E���^�[���ݒl
    m_cycle_time = thread_info.cycle_ms;            // ���s����[ms]

    //----------------------------------------------------------------------------
    // �J�����N������
	m_img_src.status = static_cast<uint32_t>(ENUM_IMAGE_STATUS::DEFAULT);               // �摜�X�e�[�^�X:�f�t�H���g
	m_img_src.width  = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size;   // �摜�T�C�Y(������f) [pixel]
	m_img_src.height = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size;   // �摜�T�C�Y(������f) [pixel]
	if (m_img_src.data_bgr == NULL) {
        m_img_src.data_bgr = new (uint8_t[IMAGE_SIZE * IMAGE_FORMAT_SIZE]); // �摜�f�[�^�o�b�t�@�̃|�C���^(BGR 24bit)
    }
	ZeroMemory(m_img_src.data_bgr, (sizeof(uint8_t) * IMAGE_SIZE * IMAGE_FORMAT_SIZE)); // The all clear the data area

    // CTeliCamLib�N���X�I�u�W�F�N�g�̐���
    if (m_cnfgcmn.img_source_camera == static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {   // �J�����摜�捞��
        TELICAM_LIB_INFO caminfo;   // �J�����̏��
        caminfo.details.cnfg.valid          = TRUE;                         // �J�����̗L���܂��͖���[0:���� 1:�L��]
        caminfo.details.cnfg.ipaddress      = m_cnfgcam.basis.ipaddress;    // �J������IP�A�h���X
        caminfo.details.cnfg.packetsize     = m_cnfgcam.basis.packetsize;   // �h���C�o���󂯎��p�P�b�g�̍ő�T�C�Y(�ʏ��0���w��)[byte]
        caminfo.details.cnfg.framerate_drop = m_cnfgcam.error.framedrop;    // �t���[�����[�g�ቺ�̔���l[fps]

        if (m_telicam_lib == NULL) {
            if ((m_telicam_lib = new CTeliCamLib(caminfo)) == NULL) {
                _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CTeliCamLib");
                msg = std::format(L"<Error>Failed to create CTeliCamLib");
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
        }
        msg = std::format(L"<Information>Created CTeliCamLib");
        outp_msg_to_listview(msg);

        // TeliCamAPI�̏�����
        if (m_telicam_lib->initialize() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::init_task]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::initialize() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        msg = std::format(L"<Information>Initialized CTeliCamLib");
        outp_msg_to_listview(msg);

        // �X���b�h�N��(�J��������N������)
        m_td_camera_start_stat = TRUE;
        if ((m_td_camera_start_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                              0,
                                                                              td_camera_start,
                                                                              reinterpret_cast<LPVOID>(this),
                                                                              0,
                                                                              NULL))) == NULL) {
	        m_td_camera_start_stat = FALSE;

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_camera_start) handle");
            msg = std::format(L"<Error>Failed to start td_camera_start() thread");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        SetThreadPriority(m_td_camera_start_hndl, THREAD_PRIORITY_LOWEST);
        Sleep(10);
        ResumeThread(m_td_camera_start_hndl);

        // �X���b�h�N��(�J�����ċN������)
        m_td_camera_restart_stat = TRUE;
        if ((m_td_camera_restart_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                0,
                                                                                td_camera_restart,
                                                                                reinterpret_cast<LPVOID>(this),
                                                                                0,
                                                                                NULL))) == NULL) {
	        m_td_camera_restart_stat = FALSE;

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_camera_restart) handle");
            msg = std::format(L"<Error>Failed to start td_camera_restart() thread");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        SetThreadPriority(m_td_camera_restart_hndl, THREAD_PRIORITY_LOWEST);
    }   // if (m_cnfgcmn.img_source_camera != static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA))

    //----------------------------------------------------------------------------
    // CANopen�N������
    // CCANopenLib�N���X�I�u�W�F�N�g�̐���
    if (m_canopen_lib == NULL) {
        if ((m_canopen_lib = new CCANopenLib()) == NULL) {
            m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB);  // �X�e�[�^�X���:CANopen Lib�G���[

            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Creating of CCANopenLib");
            msg = std::format(L"<Error>Failed to create CCANopenLib");
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    msg = std::format(L"<Information>Created CCANopenLib");
    outp_msg_to_listview(msg);

    // CANopen���W���[���̏�����
    int32_t stat;
    if ((stat = m_canopen_lib->initialize(m_cnfgtlt.canopen.port, m_cnfgtlt.canopen.node_id)) != 0) {
        m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB);  // �X�e�[�^�X���:CANopen Lib�G���[

        _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::init_task]<Error>Initialize of CCANopenLib", stat);
        msg = std::format(L"<Error>Failed in CCANopenLib::initialize() ({:d})", stat);
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    msg = std::format(L"<Information>Initialized CCANopenLib ({:s})", m_canopen_lib->get_libver());
    outp_msg_to_listview(msg);

    // �X���b�h�N��(CANopen�N������)
    m_td_canopen_start_stat = TRUE;
    if ((m_td_canopen_start_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                           0,
                                                                           td_canopen_start,
                                                                           reinterpret_cast<LPVOID>(this),
                                                                           0,
                                                                           NULL))) == NULL) {
	    m_td_canopen_start_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_start) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_start() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_start_hndl, THREAD_PRIORITY_LOWEST);
    Sleep(10);
    ResumeThread(m_td_canopen_start_hndl);

    // �X���b�h�N��(�X�Όv�v���Z�b�g����)
    m_td_canopen_tilt_preset_stat = TRUE;
    if ((m_td_canopen_tilt_preset_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                                 0,
                                                                                 td_canopen_tilt_preset,
                                                                                 reinterpret_cast<LPVOID>(this),
                                                                                 0,
                                                                                 NULL))) == NULL) {
	    m_td_canopen_tilt_preset_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_tilt_preset) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_tilt_preset() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_tilt_preset_hndl, THREAD_PRIORITY_LOWEST);

    // �X���b�h�N��(�X�Όv�f�[�^�擾����)
    m_td_canopen_tilt_get_stat = TRUE;
    if ((m_td_canopen_tilt_get_hndl = reinterpret_cast<HANDLE>(_beginthreadex(NULL,
                                                                              0,
                                                                              td_canopen_tilt_get,
                                                                              reinterpret_cast<LPVOID>(this),
                                                                              0,
                                                                              NULL))) == NULL) {
	    m_td_canopen_tilt_get_stat = FALSE;

        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::init_task]<Error>Thread(td_canopen_tilt_get) handle");
        msg = std::format(L"<Error>Failed to start td_canopen_tilt_get() thread");
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
    SetThreadPriority(m_td_canopen_tilt_get_hndl, THREAD_PRIORITY_LOWEST);

    return S_OK;
}

/// @brief �I������
/// @param
/// @return 
/// @note
void CEnvironment::close(void)
{
    //----------------------------------------------------------------------------
    // �X���b�h�̏I��
    //�X�Όv�f�[�^�擾����
    if (m_td_canopen_tilt_get_hndl != NULL) {
        if (m_td_canopen_tilt_get_stat) {
            m_td_canopen_tilt_get_stat = FALSE;
            ResumeThread(m_td_canopen_tilt_get_hndl);
            WaitForSingleObject(m_td_canopen_tilt_get_hndl, 500);   // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_canopen_tilt_get_hndl);
	    m_td_canopen_tilt_get_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_tilt_get_stat = FALSE;
    Sleep(100);

    //�X�Όv�v���Z�b�g����
    if (m_td_canopen_tilt_preset_hndl != NULL) {
        if (m_td_canopen_tilt_preset_stat) {
            m_td_canopen_tilt_preset_stat = FALSE;
            ResumeThread(m_td_canopen_tilt_preset_hndl);
            WaitForSingleObject(m_td_canopen_tilt_preset_hndl, 500);    // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_canopen_tilt_preset_hndl);
	    m_td_canopen_tilt_preset_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_tilt_preset_stat = FALSE;
    Sleep(100);

    // CANopen�N������
    if (m_td_canopen_start_hndl != NULL) {
        if (m_td_canopen_start_stat) {
            m_td_canopen_start_stat = FALSE;
            ResumeThread(m_td_canopen_start_hndl);
            WaitForSingleObject(m_td_canopen_start_hndl, 500);  // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_canopen_start_hndl);
	    m_td_canopen_start_hndl = NULL;																								// Handle is closed.
    }
    m_td_canopen_start_stat = FALSE;
    Sleep(100);

    // �J�����ċN������
    if (m_td_camera_restart_hndl != NULL) {
        if (m_td_camera_restart_stat) {
            m_td_camera_restart_stat = FALSE;
            ResumeThread(m_td_camera_restart_hndl);
            WaitForSingleObject(m_td_camera_restart_hndl, 500); // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_camera_restart_hndl);
	    m_td_camera_restart_hndl = NULL;																								// Handle is closed.
    }
    m_td_camera_restart_stat = FALSE;
    Sleep(100);

    // �J��������N������
    if (m_td_camera_start_hndl != NULL) {
        if (m_td_camera_start_stat) {
            m_td_camera_start_stat = FALSE;
            ResumeThread(m_td_camera_start_hndl);
            WaitForSingleObject(m_td_camera_start_hndl, 500);   // �X���b�h���I������܂őҋ@
        }
        CloseHandle(m_td_camera_start_hndl);
	    m_td_camera_start_hndl = NULL;																								// Handle is closed.
    }
    m_td_camera_start_stat = FALSE;
    Sleep(100);

    //----------------------------------------------------------------------------
    // CCANopenLib�N���X�I�u�W�F�N�g�̍폜
    if (m_canopen_lib != NULL) {
        // CANopen�l�b�g���[�N�̖�����
        m_canopen_lib->network_disable();

        Sleep(100);
        delete m_canopen_lib;
	    m_canopen_lib = NULL;
	    Sleep(100);
    }

    //----------------------------------------------------------------------------
    // CTeliCamLib�N���X�I�u�W�F�N�g�̍폜
    if (m_telicam_lib != NULL) {
        // Stop stream
        m_telicam_lib->stop_stream();

        // Close stream
        m_telicam_lib->close_stream();

        // Close camera
        m_telicam_lib->close_camera();

        Sleep(1000);
        delete m_telicam_lib;
	    m_telicam_lib = NULL;
	    Sleep(500);
    }

    //----------------------------------------------------------------------------
    // �摜�f�[�^�o�b�t�@�̍폜
    if (m_img_src.data_bgr != NULL) {
        delete[] m_img_src.data_bgr;
        m_img_src.data_bgr = NULL;
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
void CEnvironment::proc_main(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // ���s�����v��
    LARGE_INTEGER frequency;    // �V�X�e���̎��g��
    LARGE_INTEGER start_count;  // ���݂̃J�E���g��
    LONGLONG      span_usec;    // ���Ԃ̊Ԋu[usec]

    QueryPerformanceFrequency(&frequency);  // �V�X�e���̎��g��
    QueryPerformanceCounter(&start_count);  // ���݂̃J�E���g��
    span_usec = ((start_count.QuadPart - m_cycle_time_counter.QuadPart) * 1000000L) / frequency.QuadPart;   //
    if ((m_cycle_time = static_cast<uint32_t>(span_usec / 1000)) <= 0) { // ���s����[ms]
        m_cycle_time = thread_info.cycle_ms;
    }
    QueryPerformanceCounter(&m_cycle_time_counter); // �p�t�H�[�}���X�J�E���^�[���ݒl

    //----------------------------------------------------------------------------
    // ���L�f�[�^�Ǎ���
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^

    //----------------------------------------------------------------------------
    // �J�����ċN������
    if ((m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)) ||        // �J�����Đڑ��v��
        (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC))) {    // �J�����Đڑ��v��(MCC)
        if (m_td_camera_restart_hndl != NULL) {
            if (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)) &&       // �J�����Đڑ���
                !(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED))) { // �J�����Đڑ�����
                ResumeThread(m_td_camera_restart_hndl);
            }
        }
        else {
            m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED); // �J�����Đڑ�����
            msg = std::format(L"<Error>m_td_camera_restart_hndl NULL");
            outp_msg_to_listview(msg);
        }
        if (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED)) {    // �J�����Đڑ�����
            m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)     |    // �J�����Đڑ��v��
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC) |    // �J�����Đڑ��v��(MCC)
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)     |    // �J�����Đڑ���
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED));  // �J�����Đڑ�����
        }
    }

    //----------------------------------------------------------------------------
    // �J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
    if ((m_telicam_lib != NULL) &&
        (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)))) { // �J�����Đڑ���
        m_telicam_lib->set_expstime(static_cast<float64_t>(m_infoprc_data.exps_time));
    }

    //----------------------------------------------------------------------------
    // �X�Όv�v���Z�b�g����
    if ((m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)) ||     // �X�Όv�v���Z�b�g�v��
        (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC))) { // �X�Όv�v���Z�b�g�v��(MCC)
        if (m_td_canopen_tilt_preset_hndl != NULL) {
            if (!(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)) &&        // �X�Όv�v���Z�b�g���s��
                !(m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED))) {  // �X�Όv�v���Z�b�g����
                ResumeThread(m_td_canopen_tilt_preset_hndl);
            }
        }
        else {
            m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);  // �X�Όv�v���Z�b�g����
            msg = std::format(L"<Error>m_td_canopen_tilt_preset_hndl NULL");
            outp_msg_to_listview(msg);
        }
        if (m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED)) {     // �X�Όv�v���Z�b�g����
            m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)     |     // �X�Όv�v���Z�b�g�v��
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC) |     // �X�Όv�v���Z�b�g�v��(MCC)
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)     |     // �X�Όv�v���Z�b�g���s��
                                       static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED));   // �X�Όv�v���Z�b�g����
        }
    }

    //----------------------------------------------------------------------------
    // �X�Όv�f�[�^�̎擾
    if (m_td_canopen_tilt_get_hndl != NULL) {
        ResumeThread(m_td_canopen_tilt_get_hndl);
    }
    double               dt = m_cycle_time * 0.001; // ���s����[s]
    CANOPEN_TILTIX_SLOPE tiltix_slope = m_canopen_lib->get_tiltix_slope();
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PTILT_DATA tilt_data       = &m_infotlt_data.tilt[axis];    // �X�Όv�f�[�^
        double     last_tilt_angle = tilt_data->deg;                // �X�Ίp[deg]

        tilt_data->deg = static_cast<double>(tiltix_slope.data[axis].val)
                       * (static_cast<double>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001)
                       /  static_cast<double>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_1)); // �X�Ίp[deg]
        tilt_data->rad = tilt_data->deg * CONV_DEG_RAD; // �X�Ίp[rad]

        double tilt_speed = (tilt_data->deg - last_tilt_angle) / dt;    // �X�Ίp���x[deg/s]
        tilt_data->speed_deg = ((dt * tilt_speed) + (m_cnfgtlt.tiltspdfilter * tilt_data->speed_deg))
                             / (m_cnfgtlt.tiltspdfilter + dt);      // �X�Ίp���x[deg/s]
        tilt_data->speed_rad = tilt_data->speed_deg * CONV_DEG_RAD; // �X�Ίp���x[rad/s]
    }

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infotlt_data);    // �X�Όv���f�[�^
    m_shared->set_app_info_data(m_infosys_data);    // �V�X�e�����f�[�^

    return ;
}

/// @brief �J�����̊�{�ݒ�
/// @param
/// @return 
/// @note
HRESULT CEnvironment::set_telicam_property(void)
{
    std::wstring msg;

    // �J�����̊�{�ݒ�
#pragma region SET_CAMERA_BASIS
    //----------------------------------------------------------------------------
    // �J�����̃r�f�I�X�g���[���̃s�N�Z���`���̐ݒ�
    if (m_telicam_lib->set_pixelformat(Teli::_CAM_PIXEL_FORMAT::PXL_FMT_BayerBG8) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_pixelformat() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J������ROI(�̈�)�̐ݒ�
    if (m_telicam_lib->set_camroi(m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].offset,
                                  m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].offset,
	                              m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size,
                                  m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_camroi() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̃t���[�����[�g�̐ݒ�
//  if (m_telicam_lib->set_framerate(static_cast<float64_t>(m_cnfgcam.basis.framerate),
//                                   Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_NO_SPECIFY) != 0) {
    if (m_telicam_lib->set_framerate(static_cast<float64_t>(m_cnfgcam.basis.framerate),
                                     Teli::CAM_ACQ_FRAME_RATE_CTRL_TYPE::CAM_ACQ_FRAME_RATE_CTRL_MANUAL) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_framerate() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̃g���K�[���샂�[�h�̐ݒ�
    if (m_telicam_lib->set_triggermode(false) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_triggermode() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̍����x���̐ݒ�
    if (m_telicam_lib->set_blacklevel(static_cast<float64_t>(m_cnfgcam.basis.blacklevel)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_blacklevel]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_blacklevel() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̃K���}�␳�l�̐ݒ�
    if (m_telicam_lib->set_gamma(static_cast<float64_t>(m_cnfgcam.basis.gamma)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_gamma]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gamma() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̃z���C�g�o�����X�Q�C�������������[�h�̐ݒ�
    if ((static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_CONTINUOUS) ||
        (static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto) == Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_ONCE)) {
        if (m_telicam_lib->set_wbalance_auto(static_cast<Teli::CAM_BALANCE_WHITE_AUTO_TYPE>(m_cnfgcam.basis.wb.wb_auto)) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_auto() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
    }
    else {
        if (m_telicam_lib->set_wbalance_auto(Teli::CAM_BALANCE_WHITE_AUTO_TYPE::CAM_BALANCE_WHITE_AUTO_OFF) != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_auto() ({:s})", m_telicam_lib->get_error_message());
            outp_msg_to_listview(msg);

            return E_FAIL;
        }
        else {
            //----------------------------------------------------------------------------
            // �J�����̃z���C�g�o�����X�Q�C��(�{��)�̐ݒ�
            if (m_telicam_lib->set_wbalance_ratio(static_cast<float64_t>(m_cnfgcam.basis.wb.wb_ratio_red),
                                                  Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_RED) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_ratio() ({:s})", m_telicam_lib->get_error_message());
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
            else if (m_telicam_lib->set_wbalance_ratio(static_cast<float64_t>(m_cnfgcam.basis.wb.wb_ratio_blue),
                                                       Teli::CAM_BALANCE_RATIO_SELECTOR_TYPE::CAM_BALANCE_RATIO_SELECTOR_BLUE) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::set_wbalance_ratio() ({:s})", m_telicam_lib->get_error_message());
                outp_msg_to_listview(msg);

                return E_FAIL;
            }
            else {
                ;
            }
        }
    }
#pragma endregion SET_CAMERA_BASIS

    // �P�x�R���g���[���ݒ�(�I������)
#pragma region SET_CAMERA_EXPOSURE_CONTROL
    //----------------------------------------------------------------------------
    // �J�����̘I�����Ԃ̐��䃂�[�h�̐ݒ�
//  if (m_telicam_lib->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_NO_SPECIFY) != 0) {
    if (m_telicam_lib->set_expstime_control(Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE::CAM_EXPOSURE_TIME_CONTROL_MANUAL)     != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_expstime_control() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
    if (m_telicam_lib->set_expstime(static_cast<float64_t>(m_cnfgcam.expstime.val)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_expstime() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
#pragma endregion SET_CAMERA_EXPOSURE_CONTROL

    // �P�x�R���g���[���ݒ�(�Q�C��)
#pragma region SET_CAMERA_GAIN_CONTROL
    //----------------------------------------------------------------------------
    // �J������AGC(Automatic gain control)���샂�[�h�̐ݒ�
    if (m_telicam_lib->set_gain_auto(Teli::CAM_GAIN_AUTO_TYPE::CAM_GAIN_AUTO_OFF) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gain_auto() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }

    //----------------------------------------------------------------------------
    // �J�����̃Q�C���̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)
    if (m_telicam_lib->set_gain(static_cast<float64_t>(m_cnfgcam.gain.val)) != 0) {
        _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::set_telicam_property]", m_telicam_lib->get_error_message());
        msg = std::format(L"<Error>Failed in CTeliCamLib::set_gain_auto() ({:s})", m_telicam_lib->get_error_message());
        outp_msg_to_listview(msg);

        return E_FAIL;
    }
#pragma endregion SET_CAMERA_GAIN_CONTROL

    return S_OK;
}

/// @brief �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CEnvironment::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"�ƭ�1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), TRUE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), wstr.c_str());

    wstr = L"�ƭ�2";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC2), wstr.c_str());

    wstr = L"�ƭ�3";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC3), wstr.c_str());

    wstr = L"�ƭ�4";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC4), wstr.c_str());

    wstr = L"�ƭ�5";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC5), wstr.c_str());

    wstr = L"�ƭ�6";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), FALSE);
    SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC6), wstr.c_str());

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

/// @brief �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CEnvironment::set_tab_tip_txt(void)
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
        wstr = L"��׍Đڑ�";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), wstr.c_str());
        if (m_cnfgcmn.img_source_camera == static_cast<uint32_t>(ENUM_GRAB_IMAGE::GRAB_CAMERA)) {
            EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        }
        else {
            EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), FALSE);
        }
        wstr = L"�X�Όv��ؾ��";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), wstr.c_str());
        EnableWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM2), TRUE);
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
void CEnvironment::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
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
                    // �J�����Đڑ�
                    m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ);   // �V�X�e�����:�J�����Đڑ��v��
                    break;
                case IDC_BUTTON_TASK_ITEM2:
                    // �X�Όv�v���Z�b�g
                    m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ);    // �X�Όv�v���Z�b�g�v��
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

/// @brief �J��������N������
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_camera_start(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CTeliCamLib*  telicam = (myclass->m_telicam_lib);   // CTeliCamLib object
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<Start>");

    while (myclass->m_td_camera_start_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_camera_start_hndl); // Suspend thread

        if (telicam == NULL) {
            continue;
        }
 
        //----------------------------------------------------------------------------
        // �J�����N������
        //----------------------------------------------------------------------------
        // PC�ɐڑ�����Ă���J�����̒T��
        if (telicam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<Error>Update camera list of CTeliCamLib");
            msg = std::format(L"<Error>Failed in CTeliCamLib::update_camera_list() ({:s})", telicam->get_error_message());
            myclass->outp_msg_to_listview(msg);
        }
        else {
            //----------------------------------------------------------------------------
            // �J�����̃I�[�v�� -> �摜�X�g���[���̓]���J�n
            // �J�����̃I�[�v��
            if (telicam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_camera() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // �J�����̊�{�ݒ�
            else if (FAILED(myclass->set_telicam_property())) {
                ;
            }
            // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
            else if (telicam->open_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // �摜�X�g���[���̓]���J�n
            else if (telicam->start_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_start]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::start_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            else {
                msg = std::format(L"<Informasiton>Camera stream started");
                myclass->outp_msg_to_listview(msg);
            }
        }   // if (pTeliCam->update_camera_list() != 0) else
    }   // while (pParent->m_td_camera_start_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_start]<End>");
    msg = std::format(L"<Information>Exited td_camera_start() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_camera_start_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief �J�����ċN������
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_camera_restart(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CTeliCamLib*  telicam = (myclass->m_telicam_lib);   // CTeliCamLib object
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_restart]<Start>");
    while (myclass->m_td_camera_restart_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_camera_restart_hndl);   // Suspend thread

        if (!(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ)) &&      // �J�����Đڑ��v��
            !(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_REQ_MCC))) {  // �J�����Đڑ��v��(MCC)
            continue;
        }

        if (telicam == NULL) {
            myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED);    // �J�����Đڑ�����
            continue;
        }

        if ((myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN)) ||       // �J�����Đڑ���
            (myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED))) { // �J�����Đڑ�����
            continue;
        }

        //----------------------------------------------------------------------------
        // �J�����ċN������
        myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN);  // �J�����Đڑ���

        //----------------------------------------------------------------------------
        // �摜�X�g���[���̓]����~ -> �J�����̃N���[�Y
        // �摜�X�g���[���̓]����~
        telicam->stop_stream();

        // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃N���[�Y
        telicam->close_stream();

        // �J�����̃N���[�Y
        telicam->close_camera();

		//----------------------------------------------------------------------------
		// PC�ɐڑ�����Ă���J�����̒T��
        if (telicam->update_camera_list() != 0) {
            _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
            msg = std::format(L"<Error>Failed in CTeliCamLib::update_camera_list() ({:s})", telicam->get_error_message());
            myclass->outp_msg_to_listview(msg);
        }
        else {
            //----------------------------------------------------------------------------
            // �J�����̃I�[�v�� -> �摜�X�g���[���̓]���J�n
            // �J�����̃I�[�v��
            if (telicam->open_camera(Teli::CAM_ACCESS_MODE::CAM_ACCESS_MODE_CONTROL) != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_camera() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            // �J�����̊�{�ݒ�
            else if (FAILED(myclass->set_telicam_property())) {
                ;
            }
            // �摜�擾�p�̃X�g���[���C���^�[�t�F�[�X�̃I�[�v��
            else if (telicam->open_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::open_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
			// �摜�X�g���[���̓]���J�n
            else if (telicam->start_stream() != 0) {
                _RPTWN(_CRT_WARN, L"%s%s\n", L">>>[CEnvironment::td_camera_restart]", telicam->get_error_message());
                msg = std::format(L"<Error>Failed in CTeliCamLib::start_stream() ({:s})", telicam->get_error_message());
                myclass->outp_msg_to_listview(msg);
            }
            else {
                msg = std::format(L"<Informasiton>Camera stream started");
                myclass->outp_msg_to_listview(msg);
            }
		}
        myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_RUN));       // �J�����Đڑ���
        myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::CAMERA_RESET_COMPLETED);  // �J�����Đڑ�����
    }   // while (myclass->m_td_camera_restart_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_camera_restart]<End>");
    msg = std::format(L"<Information>Exited td_camera_restart() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_camera_restart_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief CANopen�N������
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_start(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    int32_t       stat;
    std::wstring  msg;

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_start]<Start>");
    while (myclass->m_td_canopen_start_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_start_hndl);    // Suspend thread

        if (canopen == NULL) {
            msg = std::format(L"<Error>CCANopenLib object NULL");
            myclass->outp_msg_to_listview(msg);
            continue;
        }

        //----------------------------------------------------------------------------
        // CANopen�N������
        wchar_t path[_MAX_PATH], szDrive[_MAX_DRIVE], szPath[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
        GetModuleFileName(NULL, path, sizeof(path) / sizeof(*path));    // exe fail��path���擾
        _wsplitpath_s(path,
                      szDrive, sizeof(szDrive) / sizeof(*szDrive),
                      szPath,  sizeof(szPath) / sizeof(*szPath),
                      szFName, sizeof(szFName) / sizeof(*szFName),
                      szExt,   sizeof(szExt) / sizeof(*szExt)); // �擾����path�𕪊�
        swprintf_s(path, sizeof(path) / sizeof(*path),
                   L"%s%s%s%s",
                   szDrive, szPath, FOLDER_OF_INIFILE, myclass->m_cnfgtlt.canopen.setup_fname.c_str()); // �t�H���_�̃p�X��ini�t�@�C���̃p�X�ɍ���

        //----------------------------------------------------------------------------
        // CANopen�l�b�g���[�N�̗L����
        if ((stat = myclass->m_canopen_lib->network_enable(path)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Enable of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::network_enable() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>Enable a CANOpen network");
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANOpen NMT�ʐM�I�u�W�F�N�g�̑��M (Operational)
        PCONFIG_CANOPEN cnfg_canopen = &myclass->m_cnfgtlt.canopen;
        if ((stat = myclass->m_canopen_lib->set_state(CANOPEN_NMT_COMMAND::OPERATIONAL)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Set state of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::set_state() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CANOpen NMT communication object ({:x})", static_cast<uint8_t>(CANOPEN_NMT_COMMAND::OPERATIONAL));
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANOpen RTR CAN SID�t���[���̑��M
        if((stat = myclass->m_canopen_lib->request_state()) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Request state of CCANopenLib", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::request_state() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a RTR CAN SID frame");
            myclass->outp_msg_to_listview(msg);
        }

        //----------------------------------------------------------------------------
        // CANopen�ݒ�
        int32_t stat;   //
        int32_t cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                       + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);
        // Digital recursive filter
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::DIGITAL_RECURSIVE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        myclass->m_cnfgtlt.dr_filter)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Digital recursive filter ({:d})", myclass->m_cnfgtlt.dr_filter);
            myclass->outp_msg_to_listview(msg);
        }

        // Moving Average Filter
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::MOVING_AGERAGE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        myclass->m_cnfgtlt.ma_filter)) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Moving Average Filter ({:d})", myclass->m_cnfgtlt.ma_filter);
            myclass->outp_msg_to_listview(msg);
        }

        // Resolution
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::MOVING_AGERAGE_FILTER),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE),
                                        static_cast<uint16_t>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
            continue;
        }
        else {
            msg = std::format(L"<Informasiton>To transmit a CAN SID frame - Resolution ({:d})", static_cast<uint16_t>(CANOPEN_TILTIX_RESOLUTION_VALUE::DEG_001));
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED);  // �X�e�[�^�X���:CANopen�N��
    }   // while (myclass->m_td_canopen_start_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_start]<End>");
    msg = std::format(L"<Information>Exited td_canopen_start() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_start_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief �X�Όv�v���Z�b�g����
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_tilt_preset(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    std::wstring  msg;
    int32_t       stat;
    int32_t       cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                         + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_preset]<Start>");
    while (myclass->m_td_canopen_tilt_preset_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_tilt_preset_hndl);  // Suspend thread

        if (!(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ)) &&       // �X�Όv�v���Z�b�g�v��
            !(myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_REQ_MCC))) {   // �X�Όv�v���Z�b�g�v��(MCC)
            continue;
        }

        if (canopen == NULL) {
            myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED); // �X�Όv�v���Z�b�g����
            continue;
        }

        if ((myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN)) ||        // �X�Όv�v���Z�b�g���s��
            (myclass->m_infosys_data.status & static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED))) {  // �X�Όv�v���Z�b�g����
            continue;
        }

        if (!(myclass->m_infotlt_data.status & static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED)) ||
            !(canopen->get_canopen_stat() & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {
            myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN));        // �X�Όv�v���Z�b�g���s��
            myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);   // �X�Όv�v���Z�b�g����
            continue;
        }

        //----------------------------------------------------------------------------
        // �X�Όv�v���Z�b�g����
        myclass->m_infosys_data.status |= static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN);   // �X�Όv�v���Z�b�g���s��

        // X��
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::PRESET_X_AXIS),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed to TILTIX preset X-Axis - CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }
        else {
            msg = std::format(L"<Informasiton>TILTIX preset X-Axis");
            myclass->outp_msg_to_listview(msg);
        }

        // Y��
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::DOWNLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::PRESET_Y_AXIS),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::td_canopen_start]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed to TILTIX preset Y-Axis - CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }
        else {
            msg = std::format(L"<Informasiton>TILTIX preset Y-Axis");
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infosys_data.status &= ~(static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_RUN));        // �X�Όv�v���Z�b�g���s��
        myclass->m_infosys_data.status |=   static_cast<uint32_t>(ENUM_SYSTEM_STATUS::TILT_PRESET_COMPLETED);   // �X�Όv�v���Z�b�g����
    }   // while (myclass->m_td_canopen_tilt_preset_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_preset]<End>");
    msg = std::format(L"<Information>Exited td_canopen_tilt_preset() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_tilt_preset_stat = FALSE;
    _endthreadex(0);

    return 0;
}

/// @brief �X�Όv�f�[�^�擾����
/// @param
/// @return 
/// @note
unsigned WINAPI CEnvironment::td_canopen_tilt_get(LPVOID lpParam)
{
    HRESULT       hr      = S_OK;
    CEnvironment* myclass = reinterpret_cast<CEnvironment*>(lpParam);
    CCANopenLib*  canopen = (myclass->m_canopen_lib);   // CCANopenLib object
    std::wstring  msg;
    int32_t       stat;
    int32_t       cob_id = static_cast<uint32_t>(CANOPEN_TILTIX_CAN_COB_ID::SDO_RX) 
                         + static_cast<uint32_t>(myclass->m_cnfgtlt.canopen.node_id);

    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_get]<Start>");
    while (myclass->m_td_canopen_tilt_get_stat) {
        //----------------------------------------------------------------------------
        // Standby of thread
        SuspendThread(myclass->m_td_canopen_tilt_get_hndl);  // Suspend thread

        if (canopen == NULL) {
            // �X�Όv�f�[�^���
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // �X�e�[�^�X���:�f�[�^�ǂݍ��݃G���[
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                PTILT_DATA tilt_data = &myclass->m_infotlt_data.tilt[axis]; // �X�Όv�f�[�^

                myclass->m_infotlt_data.tilt[axis].deg       = 0.0; // �X�Ίp[deg]
                myclass->m_infotlt_data.tilt[axis].rad       = 0.0; // �X�Ίp[rad]
                myclass->m_infotlt_data.tilt[axis].speed_deg = 0.0; // �X�Ίp���x[deg/s]
                myclass->m_infotlt_data.tilt[axis].speed_rad = 0.0; // �X�Ίp���x[rad/s]
            }
            continue;
        }

        if (!(myclass->m_infotlt_data.status & static_cast<uint32_t>(ENUM_TILT_STATUS::CANOPEN_STARTED)) ||
            !(canopen->get_canopen_stat() & static_cast<uint32_t>(ENUM_EMUC_CANOPEN_STAT::NW_EABLE))) {
            // �X�Όv�f�[�^���
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // �X�e�[�^�X���:�f�[�^�ǂݍ��݃G���[
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                PTILT_DATA tilt_data = &myclass->m_infotlt_data.tilt[axis]; // �X�Όv�f�[�^

                myclass->m_infotlt_data.tilt[axis].deg       = 0.0; // �X�Ίp[deg]
                myclass->m_infotlt_data.tilt[axis].rad       = 0.0; // �X�Ίp[rad]
                myclass->m_infotlt_data.tilt[axis].speed_deg = 0.0; // �X�Ίp���x[deg/s]
                myclass->m_infotlt_data.tilt[axis].speed_rad = 0.0; // �X�Ίp���x[rad/s]
            }
            continue;
        }

        // X��
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::UPLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LONG16),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // �X�e�[�^�X���:�f�[�^�ǂݍ��݃G���[

            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::control_canopen_proc]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }

        // Y��
        if ((stat = canopen->send_frame(cob_id,
                                        static_cast<uint8_t>(CANOPEN_TILTIX_CAN_COMMAND::UPLOAD_REQUEST),
                                        static_cast<int16_t>(CANOPEN_TILTIX_OD_INDEX::SLOPE_LATERAL16),
                                        static_cast<int8_t>(CANOPEN_TILTIX_OD_SUB_INDEX::VARIABLE))) != 0) {
            myclass->m_infotlt_data.status |= static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ);    // �X�e�[�^�X���:�f�[�^�ǂݍ��݃G���[

            _RPTWN(_CRT_WARN, L"%s(%d)\n", L">>>[CEnvironment::control_canopen_proc]<Error>Send frame of CANopen", stat);
            msg = std::format(L"<Error>Failed in CCANopenLib::send_frame() ({:d})", stat);
            myclass->outp_msg_to_listview(msg);
        }

        myclass->m_infotlt_data.status &= ~(static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ)); // �X�e�[�^�X���
    }   // while (myclass->m_td_canopen_tilt_get_stat)
    _RPTWN(_CRT_WARN, L"%s\n", L">>>[CEnvironment::td_canopen_tilt_get]<End>");
    msg = std::format(L"<Information>Exited td_canopen_tilt_get() thread");
    myclass->outp_msg_to_listview(msg);
    myclass->m_td_canopen_tilt_get_stat = FALSE;
    _endthreadex(0);

    return 0;
}
