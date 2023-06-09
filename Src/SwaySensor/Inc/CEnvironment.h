#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "CTeliCamLib.h"
#include "CCANopenLib.h"

//============================================================================
// define��`
#pragma region CONSTANT_DEFINITION
#pragma endregion CONSTANT_DEFINITION

//============================================================================
// enum��`
#pragma region ENUM_DEFINITION
// �摜�f�[�^���
enum class ENUM_IMAGE_STATUS : uint32_t {
    DEFAULT = 0x00000000,   // �f�t�H���g
    ENABLED = 0x00000001    // �摜�L��
} ;
#pragma endregion ENUM_DEFINITION

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#pragma region STRUCTURE_DEFINITION
#pragma pack(push, 1)
 // ���摜�f�[�^
typedef struct TAG_IMAGE_DATA {
	uint32_t status;        // �摜�X�e�[�^�X
	uint32_t width;         // �摜�T�C�Y(������f) [pixel]
	uint32_t height;        // �摜�T�C�Y(������f) [pixel]
    double   fps;           // �摜�t���[�����[�g [fps]
	uint8_t* data_bgr;      // �摜�f�[�^�o�b�t�@�̃|�C���^(BGR 24bit)
    cv::Mat  data_opencv;   // �摜�f�[�^(OpenCV�ϊ��摜)

    TAG_IMAGE_DATA()
        : status(static_cast<uint32_t>(ENUM_IMAGE_STATUS::DEFAULT))
	    , width(0)
	    , height(0)
        , fps(0.0)
	    , data_bgr(NULL)
    {}
} IMAGE_DATA, *PIMAGE_DATA;
#pragma pack(pop)
#pragma endregion STRUCTURE_DEFINITION

//////////////////////////////////////////////////////////////////////////////
// CEnvironment

/// <summary>
/// Environment�^�X�N�ł́A�ȉ��̏�����S�����܂�
/// <para>�J�����̍ċN������</para>
/// <para>�J�����̘I�����Ԃ̐ݒ�(API�ւ̐ݒ�̓X���b�h�Ŏ��s�����)</para>
/// <para>�X�Όv�v���Z�b�g����</para>
/// <para>�X�Όv�f�[�^�̎擾</para>
/// </summary>

class CEnvironment : public CTaskObj
{
public:
    CEnvironment();
    ~CEnvironment();

    // �����o�[�ϐ�

    // �����o�[�֐�
    IMAGE_DATA get_opencv_image(void);  // �摜�f�[�^�擾

protected:

private:
    // �����o�[�ϐ�
    CShared*     m_shared;      // CShared�N���X
    CTeliCamLib* m_telicam_lib; // CTeliCamLib�N���X
    CCANopenLib* m_canopen_lib; // CCANopenLib�N���X

    HANDLE m_td_camera_start_hndl;          // �X���b�h�n���h��
    BOOL   m_td_camera_start_stat;          // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    HANDLE m_td_camera_restart_hndl;        // �X���b�h�n���h��
    BOOL   m_td_camera_restart_stat;        // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_start_hndl;         // �X���b�h�n���h��
    BOOL   m_td_canopen_start_stat;         // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_tilt_preset_hndl;   // �X���b�h�n���h��
    BOOL   m_td_canopen_tilt_preset_stat;   // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)
    HANDLE m_td_canopen_tilt_get_hndl;      // �X���b�h�n���h��
    BOOL   m_td_canopen_tilt_get_stat;      // �X���b�h�X�e�[�^�X (FALSE:Exit TRUE:Run)

    CONFIG_COMMON    m_cnfgcmn;         // ���ʐݒ�
    CONFIG_CAMERA    m_cnfgcam;         // �J�����ݒ�
    CONFIG_TILTMETER m_cnfgtlt;         // �X�Όv�ݒ�
    INFO_TILTMT_DATA m_infotlt_data;    // �X�Όv���f�[�^
    INFO_IMGPRC_DATA m_infoprc_data;    // �摜�������f�[�^
    INFO_SYSTEM_DATA m_infosys_data;    // �V�X�e�����f�[�^

    IMAGE_DATA m_img_src;   // ���摜�f�[�^

    LARGE_INTEGER m_cycle_time_counter; // �p�t�H�[�}���X�J�E���^�[���ݒl
    int32_t       m_cycle_time;         // ���s����[ms]

    // �����o�[�֐�
    void init_task(void* param);    //
    void routine_work(void* param); //

    HRESULT initialize(void);   // ����������
    void close(void);           // �I������
    void proc_main(void);       // ���C������

    HRESULT set_telicam_property(void); // �J�����̊�{�ݒ�

    void set_tab_func_txt(void);    // �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
    void set_tab_tip_txt(void);     // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
    void set_tab(HWND hDlg,
                 uint32_t msg,
                 WPARAM wp,
                 LPARAM lp);        // �^�u�p�l���̐ݒ�

    static unsigned WINAPI td_camera_start(LPVOID lpParam);         // �J��������N������
    static unsigned WINAPI td_camera_restart(LPVOID lpParam);       // �J�����ċN������
    static unsigned WINAPI td_canopen_start(LPVOID lpParam);        // CANopen�N������
    static unsigned WINAPI td_canopen_tilt_preset(LPVOID lpParam);  // �X�Όv�v���Z�b�g����
    static unsigned WINAPI td_canopen_tilt_get(LPVOID lpParam);     // �X�Όv�f�[�^�擾����
};
