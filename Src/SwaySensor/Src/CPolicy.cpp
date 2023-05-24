#include "CPolicy.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////
// CPolicy

CONFIG_COMMON    CPolicy::m_cnfgcmn;        // ���ʐݒ�
CONFIG_CAMERA    CPolicy::m_cnfgcam;        // �J�����ݒ�
CONFIG_MOUNTING  CPolicy::m_cnfgmnt;        // ��t���@�ݒ�
CONFIG_IMGPROC   CPolicy::m_cnfgprc;        // �摜���������ݒ�
INFO_TILTMT_DATA CPolicy::m_infotlt_data;   // �X�Όv���f�[�^
INFO_CLIENT_DATA CPolicy::m_infoclt_data;   // �N���C�A���g���f�[�^
INFO_ADJUST_DATA CPolicy::m_infoajs_data;   // �������f�[�^
INFO_IMGPRC_DATA CPolicy::m_infoprc_data;   // �摜�������f�[�^

//////////////////////////////////////////////////////////////////////////////
// Public method

/// @brief
/// @param
/// @return
/// @note
CPolicy::CPolicy()
{
    m_shared      = NULL;   // CShared�N���X
    m_environment = NULL;   // CEnvironment�N���X
}

/// @brief
/// @param
/// @return
/// @note
CPolicy::~CPolicy()
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
void CPolicy::init_task(void* param)
{
    CPolicy* myclass = reinterpret_cast<CPolicy*>(param);   // ���N���X�C���X�^���X�|�C���^

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
void CPolicy::routine_work(void* param)
{
    CPolicy*      myclass = reinterpret_cast<CPolicy*>(param);  // ���N���X�C���X�^���X�|�C���^
    LARGE_INTEGER frequency;                // �V�X�e���̎��g��
    LARGE_INTEGER start_count, end_count;   // ���݂̃J�E���g��
    LONGLONG      span_usec;                // ���Ԃ̊Ԋu[usec]

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
    // �������Ԍv��(�J�n���Ԏ擾)
    QueryPerformanceFrequency(&frequency);  // �V�X�e���̎��g��
    QueryPerformanceCounter(&start_count);  // ���݂̃J�E���g��

    //----------------------------------------------------------------------------
    // �摜����
    myclass->proc_main();

    //----------------------------------------------------------------------------
    // �������Ԍv��(�I�����Ԏ擾)
    QueryPerformanceCounter(&end_count);    // ���݂̃J�E���g��
    span_usec = ((end_count.QuadPart - start_count.QuadPart) * 1000000L) / frequency.QuadPart; // ���Ԃ̊Ԋu[usec]

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
HRESULT CPolicy::initialize(void)
{
    std::wstring msg;

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    // CShared�N���X�I�u�W�F�N�g�̐���
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

    // ���L�f�[�^�Ǎ���
    m_shared->get_app_config(&m_cnfgcmn);           // ���ʐݒ�
    m_shared->get_app_config(&m_cnfgcam);           // �J�����ݒ�
    m_shared->get_app_config(&m_cnfgmnt);           // ��t���@�ݒ�
    m_shared->get_app_config(&m_cnfgprc);           // �摜���������ݒ�
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoclt_data);   // �N���C�A���g���f�[�^
    m_shared->get_app_info_data(&m_infoajs_data);   // �������f�[�^
    m_shared->get_app_info_data(&m_infoprc_data);   // �摜�������f�[�^

    // ���L�f�[�^������
    for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
        m_infoprc_data.target_data[idx].valid   = FALSE;    // ���o���
        m_infoprc_data.target_data[idx].max_val = 0.0;      // �ő�P�x
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            m_infoprc_data.target_data[idx].pos[axis] = 0.0;  // ���o�ʒu[pixel]
        }
        m_infoprc_data.target_data[idx].size       = 0; // ���o�T�C�Y
        m_infoprc_data.target_data[idx].roi.x      = 0; // ROI:x coordinate of the top-left corner
        m_infoprc_data.target_data[idx].roi.y      = 0; // ROI:y coordinate of the top-left corner
        m_infoprc_data.target_data[idx].roi.width  = 0; // ROI:width of the rectangle
        m_infoprc_data.target_data[idx].roi.height = 0; // ROI:height of the rectangle
    }
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // �^�[�Q�b�g�ʒu[pixel]
        m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
        m_infoprc_data.sway_data[axis].sway_angle  = 0.0;   // �U��p[pixel]
        m_infoprc_data.sway_data[axis].sway_speed  = 0.0;   // �U�ꑬ�x[pixel/s]
        m_infoprc_data.sway_data[axis].sway_zero   = 0.0;   // �U�ꒆ�S[pixel]
    }
    m_infoprc_data.target_size = 0.0;                                                   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
    m_infoprc_data.status      = static_cast<uint32_t>(ENUM_PROCCESS_STATUS::DEFAULT);  // ���o���
    m_infoprc_data.img_fps     = 0.0;                                                   // �t���[�����[�g
    m_infoprc_data.img_val     = 0.0 ;                                                  // ���x
    m_infoprc_data.exps_mode   = EXPOSURE_CONTROL_HOLD;                                 // �����I���R�}���h(0:��~ 1:Up -1:Down)
    m_infoprc_data.exps_time   = m_cnfgcam.expstime.val;                                // �I������[us]
    //@@@�t�@�C������ǂނ悤�ɂ���
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_infoprc_data.sway_data[axis].sway_zero = (m_cnfgcam.basis.roi[axis].offset
                                                 +  m_cnfgcam.basis.roi[axis].size) * 0.5;  //�U�ꒆ�S[pixel]
    }

    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infoprc_data);    // �摜�������f�[�^

    //----------------------------------------------------------------------------
    // �����o�[�ϐ��̏�����
    // �P�x�ړ����σf�[�^������
    PMOVE_AVERAGE_DATA move_avrg_data = &m_move_avrg_data;
    ZeroMemory(move_avrg_data->data, sizeof(move_avrg_data->data)); // �f�[�^
    move_avrg_data->wptr       = 0;     // �����݃|�C���^
    move_avrg_data->data_count = 0;     // �f�[�^��
    move_avrg_data->total_val  = 0;     // �P�x�ώZ
    move_avrg_data->max_val    = 0.0;   // �ő�P�x(�ړ����ό�)

    // �U�ꒆ�S�v���f�[�^
    QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        m_sway_zero_data.sway_min[axis]  = static_cast<double>((m_cnfgcam.basis.roi[axis].offset
                                         + m_cnfgcam.basis.roi[axis].size));            // �U��p�ŏ��l
        m_sway_zero_data.sway_max[axis]  = 0.0;                                         // �U��p�ő�l
        m_sway_zero_data.sway_zero[axis] = m_infoprc_data.sway_data[axis].sway_zero;    // �U��[���_
    }

    // 
    QueryPerformanceCounter(&m_cycle_time_counter); // �p�t�H�[�}���X�J�E���^�[���ݒl
    m_cycle_time = thread_info.cycle_ms;            // ���s����[ms]

    //----------------------------------------------------------------------------
    // CEnvironment�N���X�I�u�W�F�N�g�̐���
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

/// @brief �I������
/// @param
/// @return 
/// @note
void CPolicy::close(void)
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
void CPolicy::proc_main(void)
{
    IMAGE_DATA img_src;
    cv::Mat    img_roi; // �ؔ����摜
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
    m_shared->get_app_config(&m_cnfgcam);           // �J�����ݒ�
    m_shared->get_app_config(&m_cnfgprc);           // �摜���������ݒ�
    m_shared->get_app_info_data(&m_infotlt_data);   // �X�Όv���f�[�^
    m_shared->get_app_info_data(&m_infoclt_data);   // �N���C�A���g���f�[�^
    m_shared->get_app_info_data(&m_infoajs_data);   // �������f�[�^

    //----------------------------------------------------------------------------
    // �摜�捞��
#pragma region GET_IMAGE
    img_src = m_environment->get_opencv_image();
    (img_src.status & static_cast<uint32_t>(ENUM_IMAGE_STATUS::ENABLED)) ? (m_infoprc_data.status |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) :   // �摜�������:�摜�f�[�^�L��
                                                                           (m_infoprc_data.status &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))); // �摜�������:�摜�f�[�^����
    m_infoprc_data.img_fps = img_src.fps;   // �t���[�����[�g[fps]
#pragma endregion GET_IMAGE

    //----------------------------------------------------------------------------
    // ���o����
#pragma region PROCESS_TAGET
    if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) {
        //----------------------------------------------------------------------------
        // �摜�F��BGR��HSV�ɕϊ�
#pragma region CONV_BGR_TO_HSV
        if (!m_cnfgprc.roi.valid) {
            cv::cvtColor(img_src.data_opencv, img_hsv, cv::COLOR_BGR2HSV);
        }
#pragma endregion CONV_BGR_TO_HSV

        //----------------------------------------------------------------------------
        // �e�`�����l�����Ƃ�2�l��(LUT�ϊ�)���A3�`�����l���S�Ă�AND�����A�}�X�N�摜���쐬����
#pragma region CREATE_MASK_IMAGE
        lut = cv::Mat(256, 1, CV_8UC3); // LUT
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // �^�[�Q�b�g���o�f�[�^

            if (!m_cnfgprc.mask[idx].valid) {
                target_data->max_val = 0.0; // �ő�P�x
                continue;
            }

            // ROI�͈̔�(�����`)��ݒ肷��
            if (m_cnfgprc.roi.valid) {
                // * (x, y, width, height)�Ŏw��
                if (target_data->valid) {
                    // ��������
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

                    // ��������
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

                // �����摜�𐶐�
                // * �����摜�Ƃ��̌��摜�͋��ʂ̉摜�f�[�^���Q�Ƃ��邽�߁A
                //   �����摜�ɕύX��������ƁA���摜���ύX�����B
                img_roi = img_src.data_opencv(target_data->roi);
                // �摜�F��BGR��HSV�ɕϊ�
                cv::cvtColor(img_roi, img_hsv, cv::COLOR_BGR2HSV);
            }   // if (m_cnfgprc.roi.valid > 0)
            else {
                target_data->roi.x      = 0;
                target_data->roi.y      = 0;
                target_data->roi.width  = img_src.width;
                target_data->roi.height = img_src.height;
            }   // if (m_cnfgprc.roi.valid > 0) else

            // 3�`�����l����LUT�쐬
            for (uint32_t i = 0; i < static_cast<uint32_t>(ENUM_HSV_MODEL::E_MAX); i++) {
                mask_low[i] = m_cnfgprc.mask[idx].hsv_l[i]; // HSV�}�X�N����l(����)
                mask_upp[i] = m_cnfgprc.mask[idx].hsv_u[i]; // HSV�}�X�N����l(���)
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

            // �`�����l�����Ƃ�LUT�ϊ�(�e�`�����l�����Ƃ�2�l������)
            cv::LUT(img_hsv, lut, img_hsv_bin);

            // �}�X�N�摜�̍쐬
            cv::split(img_hsv_bin, planes); // �`�����l�����Ƃ�2�l�����ꂽ�摜�����ꂼ��̃`�����l���ɕ�������
            cv::bitwise_and(planes[static_cast<uint32_t>(ENUM_HSV_MODEL::H)], planes[static_cast<uint32_t>(ENUM_HSV_MODEL::S)], img_mask[idx]);
            cv::bitwise_and(img_mask[idx], planes[static_cast<uint32_t>(ENUM_HSV_MODEL::V)], img_mask[idx]);

            // �ő�P�x���o
            cv::split(img_hsv, planes);
            cv::minMaxLoc(planes[static_cast<uint32_t>(ENUM_HSV_MODEL::V)], NULL, &target_data->max_val);
        }   // for (UINT idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++)
#pragma endregion CREATE_MASK_IMAGE

        //----------------------------------------------------------------------------
        // �}�X�N���ꂽ�摜�̍���
#pragma region MASK_IMAGE_COMBINED
// �e�}�X�N�摜�͌ʂɏ���
#if 0
        cv::Mat imgMask;
//      imgMask = imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)] + imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)];
        cv::add(imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)], imgMask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)], imgMask);

//      // �o��
//      cv::Mat imgMasked;
//      imgSrc.copyTo(imgMasked, imgMask);
#endif
#pragma endregion MASK_IMAGE_COMBINED

        //----------------------------------------------------------------------------
        // �m�C�Y����
        // �S�}��
#pragma region NOISE_CUT_1
        switch (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].type) {
        case static_cast<uint32_t>(ENUM_NOISE_FILTER1::MEDIAN):     // �����l�t�B���^�[
            for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
                if (m_cnfgprc.mask[idx].valid) {
                    cv::medianBlur(img_mask[idx],
                                   img_mask[idx],
                                   m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);
                }
            }
            break;
        case static_cast<uint32_t>(ENUM_NOISE_FILTER1::OPENNING):   // �I�[�v�j���O����(�k�����g��)
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
                              m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);    // ���k
                    cv::dilate(img_mask[idx],
                               img_mask[idx],
                               cv::Mat(),
                               cv::Point(-1, -1),
                               m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_1)].val);   // �c��
                }
            }
            break;
        default:
            break;
        }
#pragma endregion NOISE_CUT_1

        // ������
#pragma region NOISE_CUT_2
        switch (m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].type) {
        case static_cast<uint32_t>(ENUM_NOISE_FILTER2::CLOSING):    // �N���[�W���O����(�g�偨�k��)
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
                               m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);   // �c��
                    cv::erode(img_mask[idx],
                              img_mask[idx],
                              cv::Mat(),
                              cv::Point(-1, -1),
                              m_cnfgprc.filter[static_cast<uint32_t>(ENUM_NOISE_FILTER::FILTER_2)].val);    // ���k
                }
            }
            break;
        default:
            break;
        }
#pragma endregion NOISE_CUT_2

        //----------------------------------------------------------------------------
        // �摜����
#pragma region IMAGE_PROC
        double  pos_x, pos_y;
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // �^�[�Q�b�g���o�f�[�^
            if (m_cnfgprc.mask[idx].valid) {
                // �֊s���o(��ԊO���̔��̗֊s�݂̂��擾)
                cv::findContours(img_mask[idx], m_infoprc_data.contours[idx], cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

                // �d�S���o
                pos_x = 0.0;
                pos_y = 0.0;
                target_data->valid = proc_center_gravity(m_infoprc_data.contours[idx],
                                                         &pos_x,
                                                         &pos_y,
                                                         &target_data->size,
                                                         m_cnfgprc.imgprc); // ���o���
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = pos_x + target_data->roi.x; // ���o�ʒuX[pixel]
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = pos_y + target_data->roi.y; // ���o�ʒuY[pixel]
            }
            else {
                target_data->valid                                    = FALSE;  // ���o���
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = 0.0;    // ���o�ʒuX[pixel]
                target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = 0.0;    // ���o�ʒuY[pixel]
                target_data->size                                     = 0;      // ���o�T�C�Y
            }
        }   // for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++)
#pragma endregion IMAGE_PROC
    }   // if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE))
    else {
        //----------------------------------------------------------------------------
        // �}�X�N�摜���쐬����
#pragma region CREATE_MASK_IMAGE
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // �^�[�Q�b�g���o�f�[�^
            // ROI�͈̔�(�����`)��ݒ肷��
            if (m_cnfgprc.roi.valid) {
                // * (x, y, width, height)�Ŏw��
                target_data->roi.x      = 0;
                target_data->roi.y      = 0;
                target_data->roi.width  = img_src.width;
                target_data->roi.height = img_src.height;

                // �����摜�𐶐�
                // * �����摜�Ƃ��̌��摜�͋��ʂ̉摜�f�[�^���Q�Ƃ��邽�߁A
                //   �����摜�ɕύX��������ƁA���摜���ύX�����B
                img_roi = img_src.data_opencv(target_data->roi);
                // �摜�F��BGR��HSV�ɕϊ�
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
        // �摜����
#pragma region IMAGE_PROC
        for (uint32_t idx = 0; idx < static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX); idx++) {
            PTARGET_DATA target_data = &m_infoprc_data.target_data[idx];    // �^�[�Q�b�g���o�f�[�^
            target_data->valid                                    = FALSE;  // ���o���
            target_data->max_val                                  = 0.0;    // �ő�P�x
            target_data->pos[static_cast<uint32_t>(ENUM_AXIS::X)] = 0.0;    // ���o�ʒuX[pixel]
            target_data->pos[static_cast<uint32_t>(ENUM_AXIS::Y)] = 0.0;    // ���o�ʒuY[pixel]
            target_data->size                                     = 0;      // ���o�T�C�Y
            target_data->roi.x                                    = 0;      // ROI:x coordinate of the top-left corner
            target_data->roi.y                                    = 0;      // ROI:y coordinate of the top-left corner
            target_data->roi.width                                = 0;      // ROI:width of the rectangle
            target_data->roi.height                               = 0;      // ROI:height of the rectangle
        }
#pragma endregion IMAGE_PROC
    }   // if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::IMAGE_ENABLE)) else
#pragma endregion PROCESS_TAGET

    //----------------------------------------------------------------------------
    // �摜�ۑ�
#pragma region PUT_IMAGE
    // �}�X�N�摜1
    if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
        m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_1),
                                    img_mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)]);
    }
    // �}�X�N�摜2
    if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
        m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::MASK_2),
                                    img_mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)]);
    }
    // �����摜
    m_shared->set_app_info_data(static_cast<uint32_t>(ENUM_IMAGE::PROCESS), img_src.data_opencv);
#pragma endregion PUT_IMAGE

    //----------------------------------------------------------------------------
    // �U�ꌟ�o����
#pragma region SWAY_PROC
    proc_sway();
#pragma endregion SWAY_PROC

    //----------------------------------------------------------------------------
    // �V���b�^�R���g���[��
#pragma region EXPOSURE_CONTROL
    set_expstime();
#pragma endregion EXPOSURE_CONTROL

    //----------------------------------------------------------------------------
    // ���L�f�[�^������
    m_shared->set_app_info_data(m_infoprc_data);    // �摜�������f�[�^

    return ;
}

/// @brief �d�S���o
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
    // �d�S�ʒu�Z�o�A���S���Y��(�ő�֊s�ʐ�)
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
    // �d�S�ʒu�Z�o�A���S���Y��(�ő�֊s��)
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

/// @brief �U�ꌟ�o����
/// @param
/// @return
/// @note
void CPolicy::proc_sway(void)
{
    //----------------------------------------------------------------------------
    // �^�[�Q�b�g���o(�^�[�Q�b�g���o�f�[�^�̒��S)
    if ((m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) &&
        (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid)) {
        if ((m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) &&
            (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid)) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis]
                                                           +  m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis]) * 0.5;  // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis]
                                                           - m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis];          // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size
                                       +  m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].size) * 0.5;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // ���
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = 0.0;                               // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // ���
        }
    }
    else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
        if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].valid) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].pos[axis];  // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // ���
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = 0.0;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // ���
        }
    }
    else if (m_cnfgprc.mask[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
        if (m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].valid) {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_2)].pos[axis];  // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = m_infoprc_data.target_data[static_cast<uint32_t>(ENUM_IMAGE_MASK::MASK_1)].size;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE);    // ���
        }
        else {
            for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
                m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // �^�[�Q�b�g�ʒu[pixel]
                m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
            }
            m_infoprc_data.target_size = 0.0;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
            m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // ���
        }
    }
    else {
        for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
            m_infoprc_data.sway_data[axis].target_pos  = 0.0;   // �^�[�Q�b�g�ʒu[pixel]
            m_infoprc_data.sway_data[axis].target_tilt = 0.0;   // �^�[�Q�b�g�X��[pixel]
        }
        m_infoprc_data.target_size = 0.0;   // �^�[�Q�b�g�T�C�Y(�^�[�Q�b�g���o�f�[�^�̕���)
        m_infoprc_data.status     &= (~static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)); // ���
    }

    //----------------------------------------------------------------------------
    // �U�ꌟ�o
    double dt = m_cycle_time * CONV_MSEC_SEC;   // ���s����[s]
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PSWAY_DATA sway_data       = &m_infoprc_data.sway_data[axis];   // �U�ꌟ�o�f�[�^(�^�[�Q�b�g���o�f�[�^�̒��S)
        double     last_sway_angle = sway_data->sway_angle;             // �U��p(�O��l)[pixel]

        if (m_infoprc_data.status & static_cast<uint32_t>(ENUM_PROCCESS_STATUS::TARGET_ENABLE)) {
            sway_data->sway_angle = sway_data->sway_zero - sway_data->target_pos;   // �U��p[pixel]

            double sway_speed = (sway_data->sway_angle - last_sway_angle) / dt; // �U�ꑬ�x[pixel/s]
            sway_data->sway_speed = ((dt * sway_speed) + (m_cnfgprc.swayspdfilter * sway_data->sway_speed))
                                  / (m_cnfgprc.swayspdfilter + dt); // �U�ꑬ�x[pixel/s]
        }
        else {
            sway_data->sway_angle = 0.0;    // �U��p[pixel]
            sway_data->sway_speed = 0.0;    // �U�ꑬ�x[pixel/s]
        }
    }

    //----------------------------------------------------------------------------
    // �U��[���_�ݒ菈��
    for (uint32_t axis = 0; axis < static_cast<uint32_t>(ENUM_AXIS::E_MAX); axis++) {
        PSWAY_DATA sway_data = &m_infoprc_data.sway_data[axis];   // �U�ꌟ�o�f�[�^(�^�[�Q�b�g���o�f�[�^�̒��S)
        sway_data->sway_zero = get_sway_zero(axis); // �U�ꒆ�S[pixel]
    }

    return ;
}

/// @brief �U��[���_�ݒ菈��
/// @param
/// @return
/// @note
double CPolicy::get_sway_zero(uint32_t idx)
{
    PSWAY_DATA    sway_data = &m_infoprc_data.sway_data[idx];   // �U�ꌟ�o�f�[�^(�^�[�Q�b�g���o�f�[�^�̒��S)
    LARGE_INTEGER frequency;                                    // �V�X�e���̎��g��
    LARGE_INTEGER cur_count;                                    // ���݂̃J�E���g��
    LONGLONG      span_usec;                                    // ���Ԃ̊Ԋu[usec]
    uint32_t      span_msec;                                    // ���Ԃ̊Ԋu[msec]
    std::wstring  msg;
 
    QueryPerformanceFrequency(&frequency);  // �V�X�e���̎��g��
    QueryPerformanceCounter(&cur_count);    // ���݂̃J�E���g��
    span_usec = ((cur_count.QuadPart - m_sway_zero_data.time_counter.QuadPart) * 1000000L) / frequency.QuadPart;    // ���Ԃ̊Ԋu[usec]
    span_msec = static_cast<uint32_t>(span_usec / 1000);    // ���Ԃ̊Ԋu[msec]

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
                    QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
                    msg = std::format(L"<Information>Completed sway zero setting");
                    outp_msg_to_listview(msg);
                }
            }
            else {
                QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
            }
        }
        else {
            m_infoprc_data.status |= static_cast<uint32_t>(ENUM_PROCCESS_STATUS::ZERO_SET_COMPLETED);   // 
            QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
            msg = std::format(L"<Error>Failed to sway zero setting");
            outp_msg_to_listview(msg);
        }
    }
    else {
        QueryPerformanceCounter(&m_sway_zero_data.time_counter);    // �v�����ԗp�p�t�H�[�}���X�J�E���^�[
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

/// @brief �V���b�^�R���g���[��
/// @param
/// @return
/// @note
void CPolicy::set_expstime(void)
{
    if (!m_cnfgcmn.img_source_camera) {
        return ;
    }

    //----------------------------------------------------------------------------
    // �V���b�^�[�R���g���[���֎~����
    // ���l�F�摜���ُ͈�܂��̓V���b�^�[�R���g���[���Œ�ŋ֎~
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
        // �P�x�ړ�����
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
        case EXPOSURE_CONTROL_LIGHT:    // ���邭
            if (m_infoprc_data.img_val > m_cnfgcam.expstime.auto_stop_l) {
                m_infoprc_data.exps_mode = EXPOSURE_CONTROL_HOLD;
            }
            break;

        case EXPOSURE_CONTROL_DARKEN:   // �Â�
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
        case EXPOSURE_CONTROL_LIGHT:    // ���邭
            if ((m_infoprc_data.exps_time += m_cnfgcam.expstime.auto_rate) > m_cnfgcam.expstime.val_max) {
                m_infoprc_data.exps_time = m_cnfgcam.expstime.val_max;
            }
            break;

        case EXPOSURE_CONTROL_DARKEN:   // �Â�
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

/// @brief �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
/// @param
/// @return
/// @note
void CPolicy::set_tab_func_txt(void)
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
        wstr = L"���0�_�ݒ�";
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

/// @brief �^�u�p�l���̐ݒ�
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
                    // �U��[���_�ݒ�
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
