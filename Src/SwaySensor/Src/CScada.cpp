#include "CScada.h"
#include "Resource.h"
#include <Commdlg.h>
#include <commctrl.h>

//////////////////////////////////////////////////////////////////////////////
// CScada

CONFIG_COMMON    CScada::m_cnfgcmn;         // ���ʐݒ�
CONFIG_CAMERA    CScada::m_cnfgcam;         // �J�����ݒ�
CONFIG_IMGPROC   CScada::m_cnfgprc;         // �摜���������ݒ�
INFO_TILTMT_DATA CScada::m_infotlt_data;    // �X�Όv���f�[�^
INFO_CLIENT_DATA CScada::m_infoclt_data;    // �N���C�A���g���f�[�^
INFO_ADJUST_DATA CScada::m_infoajs_data;    // �������f�[�^
INFO_IMGPRC_DATA CScada::m_infoprc_data;    // �摜�������f�[�^
INFO_SYSTEM_DATA CScada::m_infosys_data;    // �V�X�e�����f�[�^

cv::Mat CScada::m_img_src;  // ���摜

HWND       CScada::m_cam_dlg_hndl;
uint32_t   CScada::m_sel_img;
BOOL       CScada::m_cursor;
POINT      CScada::m_cursor_pt;
BOOL       CScada::m_scale_img_src;
SCROLLINFO CScada::m_scrlinf_img_src_h;
SCROLLINFO CScada::m_scrlinf_img_src_v;
BOOL       CScada::m_sway_pos;
BOOL       CScada::m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
BOOL       CScada::m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
BOOL       CScada::m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CScada::CScada()
{
    m_shared       = NULL;  // CShared�N���X
    m_cam_dlg_hndl = NULL;
}

/// @brief
/// @param
/// @return
/// @note
CScada::~CScada()
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
void CScada::init_task(void* param)
{
    CScada* myclass = reinterpret_cast<CScada*>(param); // ���N���X�C���X�^���X�|�C���^

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
void CScada::routine_work(void* param)
{
    CScada*       myclass = reinterpret_cast<CScada*>(param);   // ���N���X�C���X�^���X�|�C���^
    LARGE_INTEGER frequency;                // �V�X�e���̎��g��
    LARGE_INTEGER start_count, end_count;   // ���݂̃J�E���g��
    LONGLONG      span_usec;                // ���Ԃ̊Ԋu[usec]
   
    if (myclass == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CScada::routine_work]<Error>Object pointer");
        return ;
    }

    if (myclass->m_shared == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CScada::routine_work]<Error>Object pointer");
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
HRESULT CScada::initialize(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    // CShared�N���X�I�u�W�F�N�g�̐���
    if (m_shared == NULL) {
        if ((m_shared = new CShared()) == NULL) {
            _RPTWN(_CRT_WARN, L"%s\n", L">>>[CScada::init_task]<Error>Creating of CShared");
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
    m_shared->get_app_config(&m_cnfgprc);           // �摜���������ݒ�
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoclt_data);   // �N���C�A���g���f�[�^
    m_shared->get_app_info_data(&m_infoajs_data);   // �������f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^
    m_shared->get_app_info_data(&m_infosys_data);   // �V�X�e�����f�[�^

    //----------------------------------------------------------------------------
    // �����o�[�ϐ��̏�����
    m_sel_img = 0;

    m_cursor      = FALSE;
    m_cursor_pt.x = 0;
    m_cursor_pt.y = 0;

    m_scale_img_src               = FALSE;
    m_scrlinf_img_src_h.cbSize    = sizeof(SCROLLINFO);
    m_scrlinf_img_src_h.fMask     = SIF_DISABLENOSCROLL | SIF_POS | SIF_RANGE | SIF_PAGE;
    m_scrlinf_img_src_h.nMin      = 0;
    m_scrlinf_img_src_h.nMax      = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::X)].size - m_cnfgcmn.img_screen_layout.width;
    m_scrlinf_img_src_h.nPage     = 1;
    m_scrlinf_img_src_h.nPos      = 0;
    m_scrlinf_img_src_h.nTrackPos = 0;
    m_scrlinf_img_src_v.cbSize    = sizeof(SCROLLINFO);
    m_scrlinf_img_src_v.fMask     = SIF_DISABLENOSCROLL | SIF_POS | SIF_RANGE | SIF_PAGE;
    m_scrlinf_img_src_v.nMin      = 0;
    m_scrlinf_img_src_v.nMax      = m_cnfgcam.basis.roi[static_cast<uint32_t>(ENUM_AXIS::Y)].size - m_cnfgcmn.img_screen_layout.height;
    m_scrlinf_img_src_v.nPage     = 1;
    m_scrlinf_img_src_v.nPos      = 0;
    m_scrlinf_img_src_v.nTrackPos = 0;

    m_sway_pos = TRUE;
    for (int32_t i = 0; i < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); i++) {
        m_target_roi[i]      = TRUE;
        m_target_pos[i]      = TRUE;
        m_target_contours[i] = TRUE;
    }

    return S_OK;
}

/// @brief �I������
/// @param
/// @return 
/// @note
void CScada::close(void)
{
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
void CScada::proc_main(void)
{
    HBITMAP      bmp;       // �摜(bitmap�t�@�C��)
    cv::Mat      img_proc;  // �����摜
    cv::Mat      img_mask;  // �}�X�N�摜
    cv::Mat      img_disp;  // �\���摜
    std::wstring str;

    if (m_cam_dlg_hndl == NULL) {
        _RPTWN(_CRT_WARN, L"%s\n", L">>>[CScada::proc_main]<Error>Object pointer");
        return ;
    }

    //----------------------------------------------------------------------------
    // ���L�f�[�^�Ǎ���
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoclt_data);   // �N���C�A���g���f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^
    m_shared->get_app_info_data(&m_infosys_data);   // �V�X�e�����f�[�^

    //----------------------------------------------------------------------------
    // �J�������
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) {
        // �J�������͏��
        str = std::format(L"{}", L"����");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_IMAGE_STAT), str.c_str());

        // �摜�捞����
        str = std::format(L"{:.1f}", m_infoprc_data.img_fps);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_IMG_GRAB_TIME), str.c_str());
    }
    else {
        // �J�������͏��
        str = std::format(L"{}", L"�ُ�");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_IMAGE_STAT), str.c_str());

        // �摜�捞����
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_IMG_GRAB_TIME), str.c_str());
    }

    //----------------------------------------------------------------------------
    // �X�Όv���
    if (!(m_infotlt_data.status & (static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_LIB) | 
                                   static_cast<uint32_t>(ENUM_TILT_STATUS::ERR_CANOPEN_READ)))) {
        // �X�e�[�^�X���
        str = std::format(L"{}", L"����");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_STAT), str.c_str());

        // �X�Όv�f�[�^
        str = std::format(L"{:.2f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::X)].deg);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_DEG_X), str.c_str());
        str = std::format(L"{:.2f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)].deg);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_DEG_Y), str.c_str());
        str = std::format(L"{:.4f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::X)].rad);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_RAD_X), str.c_str());
        str = std::format(L"{:.4f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)].rad);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_RAD_Y), str.c_str());

        str = std::format(L"{:.2f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::X)].speed_deg);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_DEG_X), str.c_str());
        str = std::format(L"{:.2f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)].speed_deg);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_DEG_Y), str.c_str());
        str = std::format(L"{:.4f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::X)].speed_rad);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_RAD_X), str.c_str());
        str = std::format(L"{:.4f}", m_infotlt_data.tilt[static_cast<uint32_t>(ENUM_AXIS::Y)].speed_rad);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_RAD_Y), str.c_str());
    }
    else {
        // �X�e�[�^�X���
        str = std::format(L"{}", L"�ُ�");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_STAT), str.c_str());

        // �X�Όv�f�[�^
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_DEG_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_DEG_Y), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_RAD_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_RAD_Y), str.c_str());

        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_DEG_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_DEG_Y), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_RAD_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TILT_SPD_RAD_Y), str.c_str());
    }

    //----------------------------------------------------------------------------
    // �������
    // �^�[�Q�b�g�ʒu
    if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
        str = std::format(L"{:.1f}", m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[static_cast<uint32_t>(ENUM_AXIS::X)]);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_X_1), str.c_str());
        str = std::format(L"{:.1f}", m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[static_cast<uint32_t>(ENUM_AXIS::Y)]);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_Y_1), str.c_str());
    }
    else {
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_X_1), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_Y_1), str.c_str());
    }
    if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
        str = std::format(L"{:.1f}", m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[static_cast<uint32_t>(ENUM_AXIS::X)]);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_X_2), str.c_str());
        str = std::format(L"{:.1f}", m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[static_cast<uint32_t>(ENUM_AXIS::Y)]);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_Y_2), str.c_str());
    }
    else {
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_X_2), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_TARGET_POS_Y_2), str.c_str());
    }

    // �U��
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)) {
        str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].sway_angle);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_X), str.c_str());
        str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].sway_angle);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_Y), str.c_str());
        str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].sway_speed);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_SPD_X), str.c_str());
        str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].sway_speed);
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_SPD_Y), str.c_str());
    }
    else {
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_Y), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_SPD_X), str.c_str());
        str = std::format(L"{}", L"-");
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_SPD_Y), str.c_str());
    }
    str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].sway_zero);
    SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_ZERO_X), str.c_str());
    str = std::format(L"{:.1f}", m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].sway_zero);
    SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_SWAY_ZERO_Y), str.c_str());  

    //----------------------------------------------------------------------------
    // �I������
    str = std::format(L"{:.1f}", m_infoprc_data.exps_time);
    SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_VAL_CAMERA_EXPOSURE), str.c_str());

    //----------------------------------------------------------------------------
    // �^�[�Q�b�g����
    if (m_infoajs_data.target_distance_fixed) {
        str = std::format(L"{:d}", static_cast<UINT>(m_infoajs_data.target_distance));
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_VAL_TARGET_LEN), str.c_str());
    }
    else {
        str = std::format(L"{:d}", static_cast<UINT>(m_infoclt_data.tgt_len[static_cast<uint32_t>(ENUM_SWAY_SENSOR::SENSOR_1)]));
        SetWindowText(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_VAL_TARGET_LEN), str.c_str());
    }

    //----------------------------------------------------------------------------
    // �����摜�Ǎ���
    if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::PROCESS), &m_img_src)) {
       return;
    }

    //----------------------------------------------------------------------------
    // �摜�\��
    {
        if (m_sel_img == 0) {   // ���摜
            // �摜�擾(���摜�͕ۑ��p�ɕێ����Ă���)
            m_img_src.copyTo(img_proc);

            int w  = LINE_TKN;
            int wx = LINE_TKN;
            int wy = LINE_TKN;
            int csx = CURSOR_LEN;
            int csy = CURSOR_LEN;
            if (m_scale_img_src) {
                w   *= static_cast<int>(m_img_src.rows / m_cnfgcmn.img_screen_layout.height);
                wx  *= static_cast<int>(m_img_src.rows / m_cnfgcmn.img_screen_layout.height);
                wy  *= static_cast<INT>(m_img_src.cols / m_cnfgcmn.img_screen_layout.width);
                csx *= static_cast<int>(m_img_src.rows / m_cnfgcmn.img_screen_layout.height);
                csy *= static_cast<INT>(m_img_src.cols / m_cnfgcmn.img_screen_layout.width);
            }

            cv::Scalar color;
            int x0, y0, x1, y1;

            // �J�[�\���\��
            if (m_cursor) {
                color = SCALAR_WHITE;
                cv::line(img_proc, cv::Point(0, m_cursor_pt.y), cv::Point((m_img_src.cols - 1), m_cursor_pt.y),        color, wx, cv::LINE_4);  // ����
                cv::line(img_proc, cv::Point(m_cursor_pt.x, 0), cv::Point(m_cursor_pt.x,        (m_img_src.rows - 1)), color, wy, cv::LINE_4);  // �c��
            }

            for (int32_t i = 0; i < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); i++) {
                color = (i == static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)) ? SCALAR_GREEN : SCALAR_MAGENTA;
                // ROI�\��
                if (m_target_roi[i]) {
                    cv::rectangle(img_proc, m_infoprc_data.target_data[i].roi, color, w, cv::LINE_4);
                }

                // �֊s�\��
                if (m_target_contours[i]) {
                    std::vector<cv::Vec4i> hierarchy;
                    x0 = m_infoprc_data.target_data[i].roi.x;
                    y0 = m_infoprc_data.target_data[i].roi.y;
                    cv::drawContours(img_proc,
                                     m_infoprc_data.contours[i],
                                     -1,
                                     color,
                                     1,
                                     cv::LINE_4,
                                     hierarchy,
                                     INT_MAX,
                                     cv::Point(x0, y0));
                }
                // �^�[�Q�b�g�ʒu�\��
                if (m_target_pos[i]) {
                    if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
                        x0 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::X)] - csx);
                        y0 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::Y)]);
                        x1 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::X)] + csx);
                        y1 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::Y)]);
                        cv::line(img_proc, cv::Point(x0, y0), cv::Point(x1, y1), color, wx, cv::LINE_4);    // ����
                        x0 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::X)]);
                        y0 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::Y)] - csy);
                        x1 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::X)]);
                        y1 = static_cast<INT>(m_infoprc_data.target_data[i].pos[static_cast<uint32_t>(ENUM_AXIS::Y)] + csy);
                        cv::line(img_proc, cv::Point(x0, y0), cv::Point(x1, y1), color, wy, cv::LINE_4);    // �c��
                    }
                }
            }

            // �U��ʒu�\��
            if (m_sway_pos) {
                color = SCALAR_RED;
                if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)) {
                    x0 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].target_pos - csx);
                    y0 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].target_pos);
                    x1 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].target_pos + csx);
                    y1 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].target_pos);
                    cv::line(img_proc, cv::Point(x0, y0), cv::Point(x1, y1), color, wx, cv::LINE_4);    // ����
                    x0 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].target_pos);
                    y0 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].target_pos - csy);
                    x1 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::X)].target_pos);
                    y1 = static_cast<INT>(m_infoprc_data.sway_data[static_cast<uint32_t>(ENUM_AXIS::Y)].target_pos + csy);
                    cv::line(img_proc, cv::Point(x0, y0), cv::Point(x1, y1), color, wy, cv::LINE_4);    // �c��
                }
            }

            // �\���摜�T�C�Y����
            if (!m_scale_img_src) {
                cv::Rect rct;
                rct.width  = m_cnfgcmn.img_screen_layout.width;
                rct.height = m_cnfgcmn.img_screen_layout.height;
                if ((rct.width <= img_proc.cols) && (rct.height <= img_proc.rows)) {
                    rct.x = ((m_scrlinf_img_src_h.nPos + rct.width)  <= img_proc.cols) ? m_scrlinf_img_src_h.nPos : (img_proc.cols - rct.width);
                    rct.y = ((m_scrlinf_img_src_v.nPos + rct.height) <= img_proc.rows) ? m_scrlinf_img_src_v.nPos : (img_proc.rows - rct.height);
                    img_disp = img_proc(rct);
                }
//              else {
//                  cv::resize(img_proc, img_disp, cv::Size(),
//                             static_cast<double>(rct.width)  / static_cast<double>(m_imgsrc.cols),
//                             static_cast<double>(rct.height) / static_cast<double>(m_imgsrc.rows));
//              }
                else {
                    double scale, scalex, scaley;
                    scalex = (rct.width  <= img_proc.cols) ? 1.0 : (static_cast<double>(rct.width)  / static_cast<double>(img_proc.cols));
                    scaley = (rct.height <= img_proc.rows) ? 1.0 : (static_cast<double>(rct.height) / static_cast<double>(img_proc.rows));
                    scale  = (scalex > scaley) ? scalex : scaley;
                    cv::resize(img_proc, img_disp, cv::Size(), static_cast<double>(scale), static_cast<double>(scale));

                    rct.x = ((m_scrlinf_img_src_h.nPos + rct.width)  <= img_disp.cols) ? m_scrlinf_img_src_h.nPos : (img_disp.cols - rct.width);
                    rct.y = ((m_scrlinf_img_src_v.nPos + rct.height) <= img_disp.rows) ? m_scrlinf_img_src_v.nPos : (img_disp.rows - rct.height);
                    img_disp = img_disp(rct);
                }
            }
            else {
                cv::resize(img_proc, img_disp, cv::Size(),
                            static_cast<double>(m_cnfgcmn.img_screen_layout.width)  / static_cast<double>(m_img_src.cols),
                            static_cast<double>(m_cnfgcmn.img_screen_layout.height) / static_cast<double>(m_img_src.rows));
            }

            // �e�L�X�g�\��(H,S,V)
            if (m_cursor) {
                cv::Mat img_hsv;
                cv::cvtColor(m_img_src, img_hsv, cv::COLOR_BGR2HSV);
                int h = img_hsv.data[m_cursor_pt.y * img_hsv.step + m_cursor_pt.x * img_hsv.elemSize() + static_cast<uint32_t>(ENUM_HSV_MODEL::H)];
                int s = img_hsv.data[m_cursor_pt.y * img_hsv.step + m_cursor_pt.x * img_hsv.elemSize() + static_cast<uint32_t>(ENUM_HSV_MODEL::S)];
                int v = img_hsv.data[m_cursor_pt.y * img_hsv.step + m_cursor_pt.x * img_hsv.elemSize() + static_cast<uint32_t>(ENUM_HSV_MODEL::V)];
                color = SCALAR_WHITE;                
                cv::putText(img_disp,
                            cv::format("(%d,%d)H:%03d S:%03d V:%03d", m_cursor_pt.x, m_cursor_pt.y, h, s, v),
                            cv::Point(6, static_cast<int>(m_cnfgcmn.img_screen_layout.height - 6)), cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
            }

            // �摜�\��
            int8_t* color_buf = static_cast<int8_t*>(calloc(static_cast<size_t>(img_disp.cols)
                                                     * static_cast<size_t>(img_disp.rows)
                                                     * 4, sizeof(RGBQUAD)));
            for (int y = 0; y < img_disp.rows; y++) {
                for (int x = 0; x < img_disp.cols; x++) {
                    color_buf[y * img_disp.cols * 4 + x * 4 + 0] = img_disp.data[y * img_disp.step + static_cast<int64>(x) * 3 + 0];    // Blue
                    color_buf[y * img_disp.cols * 4 + x * 4 + 1] = img_disp.data[y * img_disp.step + static_cast<int64>(x) * 3 + 1];    // Green
                    color_buf[y * img_disp.cols * 4 + x * 4 + 2] = img_disp.data[y * img_disp.step + static_cast<int64>(x) * 3 + 2];    // Red
                    color_buf[y * img_disp.cols * 4 + x * 4 + 3] = 0;                                                                   // Reserved
                }
            }
            bmp = CreateBitmap(img_disp.cols, img_disp.rows, 1, 32, color_buf);
            free(color_buf);

            SendMessage(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_BMP_IMAGE), STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bmp);
            DeleteObject(bmp);
        }   // if (m_sel_img == 0)
        else {
            // �摜�擾
            if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid &&
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
                if (m_sel_img == 1) {   // �摜1 
                    if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_1), &img_mask)) {
                        return ;    // �����ȊO�̂��߁A�I��
                    }
                }
                else {                  // �摜2
                    if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_2), &img_mask)) {
                        return ;    // �����ȊO�̂��߁A�I��
                    }
                }
            }
            else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {    // �摜1
                if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_1), &img_mask)) {
                    return ;    // �����ȊO�̂��߁A�I��
                }
            }
            else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {    // �摜2
                if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_2), &img_mask)) {
                    return ;    // �����ȊO�̂��߁A�I��
                }
            }
            else {
                if (!m_shared->get_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_1), &img_mask)) {
                    return ;    // �����ȊO�̂��߁A�I��
                }
            }

            // �\���摜�T�C�Y����
            if (!m_scale_img_src) {
                cv::Rect rct;
                rct.width  = m_cnfgcmn.img_screen_layout.width;
                rct.height = m_cnfgcmn.img_screen_layout.height;
                if ((rct.width <= img_mask.cols) && (rct.height <= img_mask.rows)) {
                    rct.x = ((m_scrlinf_img_src_h.nPos + rct.width)  <= img_mask.cols) ? m_scrlinf_img_src_h.nPos :
                                                                                         (img_mask.cols - rct.width);
                    rct.y = ((m_scrlinf_img_src_v.nPos + rct.height) <= img_mask.rows) ? m_scrlinf_img_src_v.nPos :
                                                                                         (img_mask.rows - rct.height);
                    img_disp = img_mask(rct);
                }
//              else {
//                  cv::resize(img_mask, img_disp, cv::Size(),
//                             static_cast<double>(rct.width)  / static_cast<double>(imgmask.cols),
//                             static_cast<double>(rct.height) / static_cast<double>(imgmask.rows));
//              }
                else {
                    double scale, scalex, scaley;
                    scalex = (rct.width  <= img_mask.cols) ? 1.0 : static_cast<double>(rct.width) / static_cast<double>(img_mask.cols);
                    scaley = (rct.height <= img_mask.rows) ? 1.0 : static_cast<double>(rct.height) / static_cast<double>(img_mask.rows);
                    scale = (scalex > scaley) ? scalex : scaley;
                    cv::resize(img_mask, img_disp, cv::Size(), static_cast<double>(scale), static_cast<double>(scale));

                    rct.x = ((m_scrlinf_img_src_h.nPos + rct.width)  <= img_disp.cols) ? m_scrlinf_img_src_h.nPos :
                                                                                         (img_disp.cols - rct.width);
                    rct.y = ((m_scrlinf_img_src_v.nPos + rct.height) <= img_disp.rows) ? m_scrlinf_img_src_v.nPos :
                                                                                         (img_disp.rows - rct.height);
                    img_disp = img_disp(rct);
                }
            }
            else
            {
                cv::resize(img_mask, img_disp, cv::Size(),
                            static_cast<double>(m_cnfgcmn.img_screen_layout.width)  / static_cast<double>(img_mask.cols),
                            static_cast<double>(m_cnfgcmn.img_screen_layout.height) / static_cast<double>(img_mask.rows));
            }

            // �摜�\��
            int8_t* color_buf = static_cast<int8_t*>(calloc(static_cast<size_t>(img_disp.cols)
                                                     * static_cast<size_t>(img_disp.rows)
                                                     * 4, sizeof(RGBQUAD)));
            for (int y = 0; y < img_disp.rows; y++) {
                for (int x = 0; x < img_disp.cols; x++) {
                    color_buf[y * img_disp.cols * 4 + x * 4 + 0] = img_disp.data[y * img_disp.step + x];    // Blue
                    color_buf[y * img_disp.cols * 4 + x * 4 + 1] = img_disp.data[y * img_disp.step + x];    // Green
                    color_buf[y * img_disp.cols * 4 + x * 4 + 2] = img_disp.data[y * img_disp.step + x];    // Red
                    color_buf[y * img_disp.cols * 4 + x * 4 + 3] = 0;                                       // Reserved
                }
            }
            bmp = CreateBitmap(img_disp.cols, img_disp.rows, 1, 32, color_buf);
            free(color_buf);

            SendMessage(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_BMP_IMAGE), STM_SETIMAGE, static_cast<WPARAM>(IMAGE_BITMAP), reinterpret_cast<LPARAM>(bmp));
            DeleteObject(bmp);
        }
    }

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    m_shared->set_app_config(m_cnfgcam);            // �J�����ݒ�
    m_shared->set_app_config(m_cnfgprc);            // �摜���������ݒ�
    m_shared->set_app_info_data(m_infoajs_data);    // �������f�[�^

    return ;
}

/// @brief �_�C�A���O�̃I�[�v��
/// @param
/// @return 
/// @note
HWND CScada::create_dlg_wnd()
{
    if (m_cam_dlg_hndl == NULL) {
        m_cam_dlg_hndl = CreateDialog(thread_info.hinstance,
                                      MAKEINTRESOURCE(IDD_DIALOG_IMAGE_PROC),
                                      nullptr,
                                      reinterpret_cast<DLGPROC>(cb_dlg_wnd));
        // �摜�r�b�g�}�b�v
        MoveWindow(GetDlgItem(m_cam_dlg_hndl, IDC_STATIC_BMP_IMAGE),
                   m_cnfgcmn.img_screen_layout.x0, m_cnfgcmn.img_screen_layout.y0,
                   m_cnfgcmn.img_screen_layout.width, m_cnfgcmn.img_screen_layout.height,
                   false);
        // �摜�X�N���[���o�[(����)
        MoveWindow(GetDlgItem(m_cam_dlg_hndl, IDC_SCROLLBAR_IMAGE_H),
                   m_cnfgcmn.img_screen_layout.x0, m_cnfgcmn.img_screen_layout.y0 + m_cnfgcmn.img_screen_layout.height,
                   m_cnfgcmn.img_screen_layout.width, DISP_IMG_SCROLL_SIZE,
                   false);
        // �摜�X�N���[���o�[(����)
        MoveWindow(GetDlgItem(m_cam_dlg_hndl, IDC_SCROLLBAR_IMAGE_V),
                   m_cnfgcmn.img_screen_layout.x0 + m_cnfgcmn.img_screen_layout.width, m_cnfgcmn.img_screen_layout.y0,
                   DISP_IMG_SCROLL_SIZE, m_cnfgcmn.img_screen_layout.height,
                   false);

        ShowWindow(m_cam_dlg_hndl, SW_SHOW);
    }

    return m_cam_dlg_hndl;
}

/// @brief �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CScada::set_tab_func_txt(void)
{
    std::wstring wstr;

    //----------------------------------------------------------------------------
    // 
    wstr = L"�ƭ�1";
    ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_RADIO_TASK_FUNC1), TRUE);
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

/// @brief �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CScada::set_tab_tip_txt(void)
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
        wstr = L"�\�����Open";
        ShowWindow(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), TRUE);
        SetWindowText(GetDlgItem(thread_info.hwnd_panel, IDC_BUTTON_TASK_ITEM1), wstr.c_str()); wstr.clear();
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

/// @brief �^�u�p�l���̐ݒ�
/// @param
/// @return 
/// @note
void CScada::set_tab(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
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
                    create_dlg_wnd();
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

/// @brief �_�C�A���O��ʗp�R�[���o�b�N�֐�
/// @param
/// @return 
/// @note
LRESULT CALLBACK CScada::cb_dlg_wnd(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    INT wmId = LOWORD(wp);
    
    switch (msg) {
    case WM_INITDIALOG:
        //----------------------------------------------------------------------------
        // �摜�\��
        {
            // �X�N���[���o�[
            SetScrollInfo(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_H), SB_CTL, &m_scrlinf_img_src_h, TRUE);
            SetScrollInfo(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_V), SB_CTL, &m_scrlinf_img_src_v, TRUE);

            // �J�[�\���\��
            (!m_cursor) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_CURSOR), BM_SETCHECK, BST_UNCHECKED, 0) :
                          SendMessage(GetDlgItem(hwnd, IDC_CHECK_CURSOR), BM_SETCHECK, BST_CHECKED, 0);

            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_L), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_R), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_U), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_D), m_cursor);

            // �k���\��
            (!m_scale_img_src) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_IMAGE_SCALE), BM_SETCHECK, BST_UNCHECKED, 0) :
                                 SendMessage(GetDlgItem(hwnd, IDC_CHECK_IMAGE_SCALE), BM_SETCHECK, BST_CHECKED, 0);

            // �U��ʒu�\��
            (!m_sway_pos) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_SWAY_POS), BM_SETCHECK, BST_UNCHECKED, 0) :
                            SendMessage(GetDlgItem(hwnd, IDC_CHECK_SWAY_POS), BM_SETCHECK, BST_CHECKED, 0);

            // ROI�\��
            (!m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_1), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                              SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_1), BM_SETCHECK, BST_CHECKED, 0);
            (!m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_2), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                              SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_2), BM_SETCHECK, BST_CHECKED, 0);

            // ���o�ʒu�\��
            (!m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_1), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                              SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_1), BM_SETCHECK, BST_CHECKED, 0);
            (!m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_2), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                              SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_2), BM_SETCHECK, BST_CHECKED, 0);

            // �֊s�\��
            (!m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_1), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                                   SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_1), BM_SETCHECK, BST_CHECKED, 0);
            (!m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)]) ? SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_2), BM_SETCHECK, BST_UNCHECKED, 0) :
                                                                                   SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_2), BM_SETCHECK, BST_CHECKED, 0);
        }

        //----------------------------------------------------------------------------
        // HSV
        // �}�X�N�摜1
        {
            HWND         wnd_hndl;
            std::wstring str;

            // �F��H(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_H1_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_H_MIN, IMAGE_HSV_H_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  20,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_H1_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �F��H(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_H1_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_H_MIN, IMAGE_HSV_H_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  20,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_H1_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �ʓxS(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_S1_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_S_MIN, IMAGE_HSV_S_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_S1_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �ʓxS(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_S1_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,   TRUE,  MAKELPARAM(IMAGE_HSV_S_MIN, IMAGE_HSV_S_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_S1_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);
            SetWindowText(wnd_hndl, str.c_str());

            // ���xV(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_V1_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_V_MIN, IMAGE_HSV_V_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_V1_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);
            SetWindowText(wnd_hndl, str.c_str());

            // ���xV(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_V1_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_V_MIN, IMAGE_HSV_V_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_V1_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);
            SetWindowText(wnd_hndl, str.c_str());
        }
        // �}�X�N�摜2
        {
            HWND         wnd_hndl;
            std::wstring str;

            // �F��H(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_H2_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_H_MIN, IMAGE_HSV_H_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  20,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_H2_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �F��H(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_H2_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_H_MIN, IMAGE_HSV_H_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  20,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_H2_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �ʓxS(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_S2_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_S_MIN, IMAGE_HSV_S_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_S2_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);
            SetWindowText(wnd_hndl, str.c_str());

            // �ʓxS(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_S2_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_S_MIN, IMAGE_HSV_S_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_S2_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)]);
            SetWindowText(wnd_hndl, str.c_str());

            // ���xV(Low)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_V2_LOW);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_V_MIN, IMAGE_HSV_V_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_V2_LOW);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);
            SetWindowText(wnd_hndl, str.c_str());

            // ���xV(Upp)
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_V2_UPP);
            SendMessage(wnd_hndl, TBM_SETRANGE,    TRUE, MAKELPARAM(IMAGE_HSV_V_MIN, IMAGE_HSV_V_MAX)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ,  25,   0);                                            // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS,      TRUE, m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);   // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0,    1);                                            // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_V2_UPP);
            str      = std::format(L"{:d}", m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)]);
            SetWindowText(wnd_hndl, str.c_str());
        }

        //----------------------------------------------------------------------------
        // �摜�I��
        {
            LPCTSTR str_img_row    = TEXT("���摜");
            LPCTSTR str_img_mask[] = {TEXT("Ͻ��摜1"), TEXT("Ͻ��摜2")};
            HWND    wnd_hndl       = GetDlgItem(hwnd, IDC_COMBO_IMAGE);

            SendMessage(wnd_hndl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str_img_row));
            for (uint32_t i = 0; i < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); i++) {
                if (m_cnfgprc.mask[i].valid) {
                    SendMessage(wnd_hndl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str_img_mask[i]));
                }
            }
            // �E�C���h�E�������ɂ͂��߂ɕ\������f�[�^���w��
            WPARAM index = SendMessage(wnd_hndl, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(str_img_row));
            SendMessage(wnd_hndl, CB_SETCURSEL, index, 0);
        }

        //----------------------------------------------------------------------------
        // �m�C�Y�t�B���^
        // �t�B���^�[1(�S�}��)
        {
            // ���
            uint32_t     page_size = 1;
            LPCTSTR      str_item[] = {TEXT("�Ȃ�"), TEXT("�����l�t�B���^"), TEXT("�I�[�v�j���O����")};
            HWND         wnd_hndl;
            std::wstring str;

            wnd_hndl = GetDlgItem(hwnd, IDC_COMBO_NOISEFILTER1);
            for (uint32_t i = 0; i < static_cast<uint32_t>(ENUM_NOISE_FILTER1::E_MAX); i++) {
                SendMessage(wnd_hndl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str_item[i]));
            }
            SendMessage(wnd_hndl, CB_SETCURSEL, m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type, 0);
            if ((m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type == static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN)) ||
                (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type == static_cast<uint32_t>(ENUM_NOISE_FILTER1::OPENNING))) {
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1),     SW_SHOW);
                ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), SW_SHOW);
                if (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type == static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN)) {
                    page_size = 2;
                }
            }
            else {
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1),     SW_HIDE);
                ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), SW_HIDE);
            }

            // �t�B���^�l
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1);
            SendMessage(wnd_hndl, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));   // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ, 5, 0);                    // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS, TRUE, m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);  // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0, page_size);           // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1);
            str      = std::format(L"{:d}", m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);
            SetWindowText(wnd_hndl, str.c_str());
        }
        // �t�B���^�[2(������)
        {
            // ���
            uint32_t     page_size = 1;
            LPCTSTR      str_item[] = {TEXT("�Ȃ�"), TEXT("�N���[�W���O����")};
            HWND         wnd_hndl;
            std::wstring str;

            wnd_hndl = GetDlgItem(hwnd, IDC_COMBO_NOISEFILTER2);
            for (uint32_t i = 0; i < static_cast<uint32_t>(ENUM_NOISE_FILTER2::E_MAX); i++) {
                SendMessage(wnd_hndl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str_item[i]));
            }
            SendMessage(wnd_hndl, CB_SETCURSEL, m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type, 0);
            if (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type == static_cast<uint32_t>(ENUM_NOISE_FILTER2::CLOSING)) {
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2),     SW_SHOW);
                ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2), SW_SHOW);
            }
            else {
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2),     SW_HIDE);
                ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2), SW_HIDE);
            }

            // �t�B���^�l
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2);
            SendMessage(wnd_hndl, TBM_SETRANGE, TRUE, MAKELPARAM(1, 30));   // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ, 5, 0);                    // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS, TRUE, m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);  // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0, page_size);           // �N���b�N���̈ړ���

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2);
            str      = std::format(L"{:d}", m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);
            SetWindowText(wnd_hndl, str.c_str());
        }

        //----------------------------------------------------------------------------
        // �J�����I������
        {
            HWND         wnd_hndl;
            std::wstring str;

            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE);
            SendMessage(wnd_hndl, TBM_SETRANGEMIN, TRUE, static_cast<int>(m_cnfgcam.expstime.val_min)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETRANGEMAX, TRUE, static_cast<int>(m_cnfgcam.expstime.val_max)); // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ, 1000, 0);                                             // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS, TRUE, static_cast<int>(m_infoprc_data.exps_time));        // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0, 1);                                               // �N���b�N���̈ړ���
            if (m_cnfgcam.expstime.auto_control) {
                SendMessage(GetDlgItem(hwnd, IDC_CHECK_CAMERA_EXPOSURE), BM_SETCHECK, BST_UNCHECKED, 0);
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE), SW_HIDE);
            }
            else {
                SendMessage(GetDlgItem(hwnd, IDC_CHECK_CAMERA_EXPOSURE), BM_SETCHECK, BST_CHECKED, 0);
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE), SW_SHOW);
            }

            wnd_hndl = GetDlgItem(hwnd, IDC_STATIC_VAL_CAMERA_EXPOSURE);
            str      = std::format(L"{:d}", static_cast<int>(m_infoprc_data.exps_time));
            SetWindowText(wnd_hndl, str.c_str());

            if (m_cnfgcmn.img_source_camera) {
                EnableWindow(GetDlgItem(hwnd, IDC_STATIC_TITLE_CAMERA_EXPOSURE), TRUE);
                EnableWindow(GetDlgItem(hwnd, IDC_CHECK_CAMERA_EXPOSURE),        TRUE);
                EnableWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE),       TRUE);
                EnableWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_CAMERA_EXPOSURE),   TRUE);
            }
            else {
                EnableWindow(GetDlgItem(hwnd, IDC_STATIC_TITLE_CAMERA_EXPOSURE), FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_CHECK_CAMERA_EXPOSURE),        FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE),       FALSE);
                EnableWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_CAMERA_EXPOSURE),   FALSE);
            }
        }

        //----------------------------------------------------------------------------
        // ROI�L��
        {
            HWND wnd_hndl;

            wnd_hndl = GetDlgItem(hwnd, IDC_CHECK_ROI);
            (m_cnfgprc.roi.valid) ? SendMessage(wnd_hndl, BM_SETCHECK, BST_CHECKED, 0) :
                                    SendMessage(wnd_hndl, BM_SETCHECK, BST_UNCHECKED, 0);
        }

        //----------------------------------------------------------------------------
        // �ݓ_�`�^�[�Q�b�g�ԋ���
        {
            HWND         wnd_hndl;
            std::wstring str;

            m_infoajs_data.target_distance = EXTN_TARGETDIST_MIN;
            wnd_hndl = GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN);
            SendMessage(wnd_hndl, TBM_SETRANGEMIN, TRUE, (UINT)EXTN_TARGETDIST_MIN);                    // �����W���w��
            SendMessage(wnd_hndl, TBM_SETRANGEMAX, TRUE, (UINT)EXTN_TARGETDIST_MAX);                    // �����W���w��
            SendMessage(wnd_hndl, TBM_SETTICFREQ, 1000, 0);                                             // �ڐ���̑���
            SendMessage(wnd_hndl, TBM_SETPOS, TRUE, static_cast<UINT>(m_infoajs_data.target_distance)); // �ʒu�̐ݒ�
            SendMessage(wnd_hndl, TBM_SETPAGESIZE, 0, 1);                                               // �N���b�N���̈ړ���
            if (m_infoajs_data.target_distance_fixed) {
                SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_LEN), BM_SETCHECK, BST_CHECKED, 0);
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN), SW_SHOW);
            }
            else
            {
                SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_LEN), BM_SETCHECK, BST_UNCHECKED, 0);
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN), SW_HIDE);
            }
            str = std::format(L"{:d}", static_cast<UINT>(m_infoajs_data.target_distance));
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_TARGET_LEN), str.c_str());
        }

        //----------------------------------------------------------------------------
        // �J�������
        {
            std::wstring str;

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_IMAGE_STAT), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_IMG_GRAB_TIME), str.c_str());
        }

        //----------------------------------------------------------------------------
        // �X�Όv���
        {
            std::wstring str;

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_STAT), str.c_str());

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_DEG_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_DEG_Y), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_RAD_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_RAD_Y), str.c_str());

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_SPD_DEG_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_SPD_DEG_Y), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_SPD_RAD_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TILT_SPD_RAD_Y), str.c_str());
        }

        //----------------------------------------------------------------------------
        // �������
        {
            std::wstring str;

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TARGET_POS_X_1), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TARGET_POS_Y_1), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TARGET_POS_X_2), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_TARGET_POS_Y_2), str.c_str());

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_Y), str.c_str());

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_SPD_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_SPD_Y), str.c_str());

            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_ZERO_X), str.c_str());
            str = std::format(L"{}", L"-");
            SetWindowText(GetDlgItem(hwnd, IDC_STATIC_SWAY_ZERO_Y), str.c_str());
        }
        break;
    case WM_HSCROLL:
        //----------------------------------------------------------------------------
        // �X�N���[���o�[
        // �摜�\��
        {
            HWND    wnd_hndl = GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_H);
	        int32_t pt       = 20;

            if (wnd_hndl == reinterpret_cast<HWND>(lp)) {
                switch(LOWORD(wp)) {
                case SB_PAGELEFT:
                    if ((m_scrlinf_img_src_h.nPos -= pt) < m_scrlinf_img_src_h.nMin) {
                        m_scrlinf_img_src_h.nPos = m_scrlinf_img_src_h.nMin;
                    }
                    break;
                case SB_PAGERIGHT:
                    if((m_scrlinf_img_src_h.nPos += pt) > m_scrlinf_img_src_h.nMax) {
                        m_scrlinf_img_src_h.nPos = m_scrlinf_img_src_h.nMax;
                    }
                    break;
                case SB_LINELEFT:
                    if((m_scrlinf_img_src_h.nPos -= 1) < m_scrlinf_img_src_h.nMin) {
                        m_scrlinf_img_src_h.nPos = m_scrlinf_img_src_h.nMin;
                    }
                    break;
                case SB_LINERIGHT:
                    if((m_scrlinf_img_src_h.nPos += 1) > m_scrlinf_img_src_h.nMax) {
                        m_scrlinf_img_src_h.nPos = m_scrlinf_img_src_h.nMax;
                    }
                    break;
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    m_scrlinf_img_src_h.nPos = HIWORD(wp);
                    break;
	            default:
                    break;
                }
                SetScrollInfo(wnd_hndl, SB_CTL, &m_scrlinf_img_src_h, TRUE);
            }
        }

        //----------------------------------------------------------------------------
        // HSV
        // �}�X�N�摜1
        {
            std::wstring str;
            int32_t      pos;

            // �F��H
            if (GetDlgItem(hwnd, IDC_SLIDER_H1_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_H1_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_H1_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_H1_UPP) == (HWND)lp)
            {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_H1_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_H1_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)] = pos;
            }
            else {
                ;
            }
            // �ʓxS
            if (GetDlgItem(hwnd, IDC_SLIDER_S1_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_S1_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_S1_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_S1_UPP) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_S1_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_S1_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)] = pos;
            }
            else {
                ;
            }
            // ���xV
            if (GetDlgItem(hwnd, IDC_SLIDER_V1_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_V1_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_V1_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_V1_UPP) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_V1_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_V1_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)] = pos;
            }
            else {
                ;
            }
        }
        // �}�X�N�摜2
        {
            std::wstring str;
            int32_t      pos;

            // �F��H
            if (GetDlgItem(hwnd, IDC_SLIDER_H2_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_H2_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_H2_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::H)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_H2_UPP) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_H2_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_H2_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::H)] = pos;
            }
            else {
                ;
            }
            // �ʓxS
            if (GetDlgItem(hwnd, IDC_SLIDER_S2_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_S2_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_S2_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::S)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_S2_UPP) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_S2_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_S2_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::S)] = pos;
            }
            else {
                ;
            }
            // ���xV
            if (GetDlgItem(hwnd, IDC_SLIDER_V2_LOW) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_V2_LOW), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_V2_LOW), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_l[static_cast<uint32_t>(ENUM_HSV_MODEL::V)] = pos;
            }
            else if (GetDlgItem(hwnd, IDC_SLIDER_V2_UPP) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_V2_UPP), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_V2_UPP), str.c_str());
                m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].hsv_u[static_cast<uint32_t>(ENUM_HSV_MODEL::V)] = pos;
            }
            else {
                ;
            }
        }

        //----------------------------------------------------------------------------
        // �m�C�Y�t�B���^
        // �S�}��
        {
            std::wstring str;
            int32_t      pos;

            if (GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_GETPOS, 0, 0));
                if (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type == static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN)) {
                    if ((pos % 2) == 0) {
                        pos = pos + 1;
                    }
                    SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_SETPOS, TRUE, pos);  // �ʒu�̐ݒ�
                }
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), str.c_str());
                m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val = pos;
            }
        }
        // ������
        {
            std::wstring str;
            int32_t      pos;

            if (GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2), str.c_str());
                m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val = pos;
            }
        }

        //----------------------------------------------------------------------------
        // �J�����I������
        {
            int32_t pos;

            if (GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE) == reinterpret_cast<HWND>(lp)) {
                pos                    = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE), TBM_GETPOS, 0, 0));
                m_cnfgcam.expstime.val = static_cast<double>(pos);
            }
        }

        //----------------------------------------------------------------------------
        // �^�[�Q�b�g�ԋ���
        {
            std::wstring str;
            int32_t      pos;

            if (GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN) == reinterpret_cast<HWND>(lp)) {
                pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN), TBM_GETPOS, 0, 0));
                str = std::format(L"{:d}", pos);
                SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_TARGET_LEN), str.c_str());
                m_infoajs_data.target_distance = static_cast<double>(pos);
            }
        }
        break;
    case WM_VSCROLL:
        //----------------------------------------------------------------------------
        // �X�N���[���o�[
        // �摜�\��
        {
            HWND    wnd_hndl = GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_V);
	        int32_t pt       = 20;

            if (wnd_hndl == reinterpret_cast<HWND>(lp)) {
                switch(LOWORD(wp)) {
                case SB_PAGEUP:
                    if((m_scrlinf_img_src_v.nPos -= pt) < m_scrlinf_img_src_v.nMin) {
                        m_scrlinf_img_src_v.nPos = m_scrlinf_img_src_v.nMin;
                    }
                    break;
                case SB_PAGEDOWN:
                    if((m_scrlinf_img_src_v.nPos += pt) > m_scrlinf_img_src_v.nMax) {
                        m_scrlinf_img_src_v.nPos = m_scrlinf_img_src_v.nMax;
                    }
                    break;
                case SB_LINEUP:
                    if((m_scrlinf_img_src_v.nPos -= 1) < m_scrlinf_img_src_v.nMin) {
                        m_scrlinf_img_src_v.nPos = m_scrlinf_img_src_v.nMin;
                    }
                    break;
                case SB_LINEDOWN:
                    if((m_scrlinf_img_src_v.nPos += 1) > m_scrlinf_img_src_v.nMax) {
                        m_scrlinf_img_src_v.nPos = m_scrlinf_img_src_v.nMax;
                    }
                    break;
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    m_scrlinf_img_src_v.nPos = HIWORD(wp);
                    break;
                default:
                    break;
                }
                SetScrollInfo(wnd_hndl, SB_CTL, &m_scrlinf_img_src_v, TRUE);
            }
        }
        break;
    case WM_COMMAND:
        // �I�����ꂽ���j���[�̉��:
        switch (wmId) {
        case IDC_BUTTON_IMAGE_SAVE:
            (m_img_src.empty() != TRUE) ? cv::imwrite(CHelper::conv_string(m_cnfgcmn.img_output_fname), m_img_src) :
                                          MessageBox(hwnd, L"�ۑ�����摜������܂���B", L"�G���[", MB_OK);
            break;
        case IDC_CHECK_CURSOR:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_CURSOR), BM_GETCHECK, 0, 0)) ? m_cursor = TRUE :
                                                                                                  m_cursor = FALSE;
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_L), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_R), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_U), m_cursor);
            EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_CURSOR_D), m_cursor);
            break;
        case IDC_BUTTON_CURSOR_L:
            {
                LONG pos = m_cursor_pt.x;
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    pos -= 50;
                }
                else if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    pos -= 10;
                }
                else {
                    pos -= 1;
                }
                if (pos < 0) {
                    pos = m_img_src.cols - 1;
                }
                m_cursor_pt.x = pos;
            }
            break;
        case IDC_BUTTON_CURSOR_R:
            {
                LONG pos = m_cursor_pt.x;
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    pos += 50;
                }
                else if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    pos += 10;
                }
                else {
                    pos += 1;
                }
                if (pos >= static_cast<LONG>(m_img_src.cols)) {
                    pos = 0;
                }
                m_cursor_pt.x = pos;
            }
            break;
        case IDC_BUTTON_CURSOR_U:
            {
                LONG pos = m_cursor_pt.y;
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    pos -= 50;
                }
                else if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    pos -= 10;
                }
                else {
                    pos -= 1;
                }
                if (pos < 0) {
                    pos = m_img_src.rows - 1;
                }
                m_cursor_pt.y = pos;
            }
            break;
        case IDC_BUTTON_CURSOR_D:
            {
                LONG pos = m_cursor_pt.y;
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    pos += 50;
                }
                else if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    pos += 10;
                }
                else {
                    pos += 1;
                }
                if (pos >= static_cast<LONG>(m_img_src.rows)) {
                    pos = 0;
                }
                m_cursor_pt.y = pos;
            }
            break;
        case IDC_CHECK_IMAGE_SCALE:
            if (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_IMAGE_SCALE), BM_GETCHECK, 0, 0)) {
                m_scale_img_src = TRUE;
                ShowWindow(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_H), SW_HIDE);
                ShowWindow(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_V), SW_HIDE);
            }
            else {
                m_scale_img_src = FALSE;
                ShowWindow(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_H), SW_SHOW);
                ShowWindow(GetDlgItem(hwnd, IDC_SCROLLBAR_IMAGE_V), SW_SHOW);
            }
            break;
        case IDC_COMBO_IMAGE:
            if (HIWORD(wp) == CBN_SELCHANGE) {
                m_sel_img = static_cast<UINT>(SendMessage(GetDlgItem(hwnd, IDC_COMBO_IMAGE), CB_GETCURSEL, 0, 0));
            }
            break;
        case IDC_CHECK_SWAY_POS:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_SWAY_POS), BM_GETCHECK, 0, 0)) ? m_sway_pos = TRUE :
                                                                                                    m_sway_pos = FALSE;
            break;
        case IDC_CHECK_TARGET_ROI_1:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_1), BM_GETCHECK, 0, 0)) ? m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = TRUE :
                                                                                                        m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = FALSE;
            break;
        case IDC_CHECK_TARGET_ROI_2:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_ROI_2), BM_GETCHECK, 0, 0)) ? m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = TRUE :
                                                                                                        m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = FALSE;
            break;
        case IDC_CHECK_TARGET_POS_1:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_1), BM_GETCHECK, 0, 0)) ? m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = TRUE :
                                                                                                        m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = FALSE;
            break;
        case IDC_CHECK_TARGET_POS_2:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_POS_2), BM_GETCHECK, 0, 0)) ? m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = TRUE :
                                                                                                        m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = FALSE;
            break;
        case IDC_CHECK_TARGET_CONTOURS_1:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_1), BM_GETCHECK, 0, 0)) ? m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = TRUE :
                                                                                                             m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] = FALSE;
            break;
        case IDC_CHECK_TARGET_CONTOURS_2:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_CONTOURS_2), BM_GETCHECK, 0, 0)) ? m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = TRUE :
                                                                                                             m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)] = FALSE;
            break;
        case IDC_COMBO_NOISEFILTER1:
            if (HIWORD(wp) == CBN_SELCHANGE) {
                uint32_t     sel;
                std::wstring str;
                int32_t      pos;

                sel = static_cast<uint32_t>(SendMessage(GetDlgItem(hwnd, IDC_COMBO_NOISEFILTER1), CB_GETCURSEL, 0, 0));
                if ((sel == static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN)) || (sel == static_cast<uint32_t>(ENUM_NOISE_FILTER1::OPENNING))) {
                    ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1),     SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), SW_SHOW);
                    if (sel == static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN)) {
                        SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_SETPAGESIZE, 0, 2);  // �N���b�N���̈ړ���
                        pos = static_cast<int32_t>(SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_GETPOS, 0, 0));
                        if ((pos % 2) == 0) {
                            pos = pos + 1;
                        }
                        SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_SETPOS, TRUE, pos);  // �ʒu�̐ݒ�
                        str = std::format(L"{:d}", pos);
                        SetWindowText(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), str.c_str());
                        m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val = pos;
                    }
                    else {
                        SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1), TBM_SETPAGESIZE, 0, 1);  // �N���b�N���̈ړ���
                    }
                }
                else {
                    ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER1),     SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER1), SW_HIDE);
                }
                m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type = sel;
            }
            break;
        case IDC_COMBO_NOISEFILTER2:
            if (HIWORD(wp) == CBN_SELCHANGE)
            {
                uint32_t sel;

                sel = static_cast<uint32_t>(SendMessage(GetDlgItem(hwnd, IDC_COMBO_NOISEFILTER2), CB_GETCURSEL, 0, 0));
                if (sel == static_cast<uint32_t>(ENUM_NOISE_FILTER2::CLOSING)) {
                    ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2),     SW_SHOW);
                    ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2), SW_SHOW);
                    SendMessage(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2), TBM_SETPAGESIZE, 0, 1);  // �N���b�N���̈ړ���
                }
                else {
                    ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_NOISEFILTER2),     SW_HIDE);
                    ShowWindow(GetDlgItem(hwnd, IDC_STATIC_VAL_NOISEFILTER2), SW_HIDE);
                }
                m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type = sel;
            }
            break;
        case IDC_CHECK_CAMERA_EXPOSURE:
            if (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_CAMERA_EXPOSURE), BM_GETCHECK, 0, 0)) {
                m_cnfgcam.expstime.auto_control = FALSE;
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE), SW_SHOW);
            }
            else {
                m_cnfgcam.expstime.auto_control = TRUE;
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_CAMERA_EXPOSURE), SW_HIDE);
            }
            break;
        case IDC_CHECK_ROI:
            (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_ROI), BM_GETCHECK, 0, 0)) ? m_cnfgprc.roi.valid = 1 :
                                                                                               m_cnfgprc.roi.valid = 0;
            break;
        case IDC_CHECK_TARGET_LEN:
            if (BST_CHECKED == SendMessage(GetDlgItem(hwnd, IDC_CHECK_TARGET_LEN), BM_GETCHECK, 0, 0)) {
                m_infoajs_data.target_distance_fixed = TRUE;
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN), SW_SHOW);
            }
            else {
                m_infoajs_data.target_distance_fixed = FALSE;
                ShowWindow(GetDlgItem(hwnd, IDC_SLIDER_TARGET_LEN), SW_HIDE);
            }
            break;
        case IDCANCEL:
//          PostQuitMessage(0);
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        break;
    case WM_PAINT:
        break;
    case WM_CLOSE:
        EndDialog(hwnd, LOWORD(wp));
        m_cam_dlg_hndl = NULL;
        return TRUE;
//      PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    return FALSE;
}
