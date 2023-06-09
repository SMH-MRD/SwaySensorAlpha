#pragma once

#include "CTaskObj.h"
#include "CShared.h"

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
#define DISP_IMG_SCROLL_SIZE    20      // �\���摜�X�N���[���o�[�T�C�Y

#define SCALAR_WHITE            cv::Scalar(255, 255, 255)
#define SCALAR_BLUE             cv::Scalar(255, 0, 0)
#define SCALAR_GREEN            cv::Scalar(0, 255, 0)
#define SCALAR_RED              cv::Scalar(0, 0, 255)
#define SCALAR_YELLOW           cv::Scalar(0, 255, 255)
#define SCALAR_MAGENTA          cv::Scalar(255, 0, 255)
#define SCALAR_LT_BLUE          cv::Scalar(255, 255, 0)

#define LINE_TKN                1   // ���C���\������
#define CURSOR_LEN              10  // �J�[�\���\������
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CScada

/// <summary>
/// Scada�^�X�N�ł́A�ȉ��̏�����S�����܂�
/// <para>���[�U�C���^�t�F�C�X�����i���j�^�����O�E�B���h�E�����F�\��,�p�����[�^�ύX�j</para>
/// </summary>

class CScada : public CTaskObj
{
public:
    CScada();
    ~CScada();

    // �����o�[�ϐ�

    // �����o�[�֐�

protected:

private:
    // �����o�[�ϐ�
    CShared* m_shared;  // CShared�N���X

    static CONFIG_COMMON    m_cnfgcmn;      // ���ʐݒ�
    static CONFIG_CAMERA    m_cnfgcam;      // �J�����ݒ�
    static CONFIG_IMGPROC   m_cnfgprc;      // �摜���������ݒ�
    static INFO_TILTMT_DATA m_infotlt_data; // �X�Όv���f�[�^
    static INFO_CLIENT_DATA m_infoclt_data; // �N���C�A���g���f�[�^
    static INFO_ADJUST_DATA m_infoajs_data; // �������f�[�^
    static INFO_IMGPRC_DATA m_infoprc_data; // �摜�������f�[�^
    static INFO_SYSTEM_DATA m_infosys_data; // �V�X�e�����f�[�^

    static cv::Mat m_img_src;   // ���摜

    static HWND       m_cam_dlg_hndl;
    static uint32_t   m_sel_img;
    static BOOL       m_cursor;
    static POINT      m_cursor_pt;
    static BOOL       m_scale_img_src;
    static SCROLLINFO m_scrlinf_img_src_h;
    static SCROLLINFO m_scrlinf_img_src_v;
    static BOOL       m_sway_pos;
    static BOOL       m_target_roi[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
    static BOOL       m_target_pos[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];
    static BOOL       m_target_contours[static_cast<uint32_t>(ENUM_IMAGE_MASK::E_MAX)];

    // �����o�[�֐�
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // ����������
    void close(void);           // �I������
    void proc_main(void);       // ���C������

    HWND create_dlg_wnd();  // �_�C�A���O�̃I�[�v��

    void set_tab_func_txt(void);    // �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
    void set_tab_tip_txt(void);     // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // �^�u�p�l���̐ݒ�

    static LRESULT CALLBACK cb_dlg_wnd(HWND hwnd,
                                       uint32_t msg,
                                       WPARAM wp,
                                       LPARAM lp);  // �_�C�A���O��ʗp�R�[���o�b�N�֐�
};
