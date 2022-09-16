#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include <opencv2/opencv.hpp>           // OpenCV
#include <opencv2/core/core.hpp>        // OpenCV
#include <opencv2/highgui/highgui.hpp>  // OpenCV
#include <opencv2/video/video.hpp>      // OpenCV

//============================================================================
// define��`
#define PI              3.14159     // ��
#define CONV_DEG_RAD    (PI/180.0)  // deg��rad

//============================================================================
// enum��`

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
#define MOVE_AVERAGE_COUNT          1           // �ړ����ϐ�
#define MOVE_AVERAGE_BUFFER_SIZE    20          // �ړ����σo�b�t�@�T�C�Y
typedef struct TAG_MOVE_AVERAGE_BUFFER {        // �ړ����σo�b�t�@
    double data[MOVE_AVERAGE_BUFFER_SIZE];      // �f�[�^
    int    wptr;                                // �����݃|�C���^
    int    datacount;                           // 
    double totalval;                            // 
    double maxv;                                // Maxmimum brightness (after move average)
} MOVE_AVERAGE_BUFFER, *PMOVE_AVERAGE_BUFFER;

//////////////////////////////////////////////////////////////////////////////
// CPolicy

class CPolicy : public CTaskObj
{
public:
    CPolicy();
    ~CPolicy();

    void init_task(void* obj);
    void routine_work(void* param);

protected:

private:
    UINT8 m_bufidx_imgmask[IMGPROC_ID_MAX];
    UINT8 b_bufidx_imgproc;

    MOVE_AVERAGE_BUFFER m_maxv_avrg;    // �P�x�ړ�����
    
#if 0
    stCameraParamData   m_camparam;     // �J�����ݒ�f�[�^
    stCommonParamData   m_cmmnparam;    // �\���ݒ�f�[�^
    stImgProcParamData  m_imgprocparam; // �摜�����ݒ�f�[�^
    stProcInfoData      m_procinfo;     // �摜��������
#endif

    void proc_main(void);
    BOOL proc_center_gravity(void);
    void proc_sway(void);

    double get_maxv_avrg(void);
    void   set_expstime(void);

    void set_window(void);
};
