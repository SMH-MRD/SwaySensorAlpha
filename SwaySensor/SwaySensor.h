#pragma once

#include "resource.h"

//============================================================================
// define��`
//----------------------------------------------------------------------------
// �^�X�N����
#define MAX_APP_TASK                    8                       // �^�X�N�X���b�h�ő吔
#define TASK_NUM                        4                       // ���̃A�v���P�[�V�����̃^�X�N��
#define TARGET_RESOLUTION               1                       // �}���`���f�B�A�^�C�}�[�C�x���g����\
#define SYSTEM_TICK_MSEC                25                      // ���C���X���b�h����
#define INITIAL_TASK_STACK_SIZE         16384                   // �e�X���b�h�p�X�^�b�N�T�C�Y
#define ID_TIMER_EVENT                  0                       // �^�C�}�[�p�C�x���g�z��̈ʒu

//----------------------------------------------------------------------------
// ID��` Main�X���b�h�p�@WM_USER + 1000 + ��
#define ID_STATUS                       (WM_USER + 1001)
#define ID_STATIC_MAIN                  (WM_USER + 1002)
#define ID_TASK_SET_TAB                 (WM_USER + 1003)

//----------------------------------------------------------------------------
// 
#define MSG_LIST_MAX                    14

//----------------------------------------------------------------------------
// �x�[�X�ݒ�
// Main Window�̏����T�C�Y, �ʒu�ݒ�
#define MAIN_WND_INIT_SIZE_W            620
#define MAIN_WND_INIT_SIZE_H            420
#define MAIN_WND_INIT_POS_X             20
#define MAIN_WND_INIT_POS_Y             20

// Tweet Message�\���pStatic Window�̃T�C�Y�ݒ�l
#define MSG_WND_H                       20                      // Height of window
#define MSG_WND_W                       590                     // Width of window
#define MSG_WND_ORG_X                   35                      // Origin X of window arrangement 
#define MSG_WND_ORG_Y                   10                      // Origin Y of window arrangement 
#define MSG_WND_Y_SPACE                 12                      // Space between windows Y direction 
#define TWEET_IMG_ORG_X                 5                       // Origin X of bitmap icon arrangement

// �e�^�X�N�A�C�R���p�C���[�W���X�g�ݒ�l
#define ICON_IMG_W                      32                      // �C���[�W��
#define ICON_IMG_H                      32                      // �C���[�W����

// �e�^�X�N�ݒ�p�^�u�֘A
#define TAB_DIALOG_W                    620
#define TAB_DIALOG_H                    300
#define TAB_POS_X                       10
#define TAB_POS_Y                       (MSG_WND_ORG_Y + (MSG_WND_H + MSG_WND_Y_SPACE) * TASK_NUM + 10)
#define TAB_SIZE_H                      35

//----------------------------------------------------------------------------
// �A�v���P�[�V�����Ǘ�
#define PROC_ITEM_ID_MAX                16

//----------------------------------------------------------------------------
#ifndef MIN
#define MIN(a,b)                        ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)                        ((a) < (b) ? (b) : (a))
#endif

//----------------------------------------------------------------------------
// ����
#define L_ON                            0x01                    // ON
#define L_OFF                           0x00                    // OFF

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
// �J�[�l���Ǘ��\����
typedef struct TAG_KNL_MANAGE_SET {
    WORD          mmt_resolution;                               // �}���`���f�B�A�^�C�}�[�̕���\
    unsigned int  cycle_base;                                   // ���C���X���b�h����
    WORD          KnlTick_TimerID;                              // �}���`���f�B�A�^�C�}�[��ID
    unsigned int  num_of_task;                                  // �A�v���P�[�V�����ŗ��p����X���b�h��
    unsigned long sys_counter;                                  // �V�X�e���J�E���^
    SYSTEMTIME    Knl_Time;                                     // �A�v���P�[�V�����J�n����̌o�ߎ���
    unsigned int  stackSize;                                    // �^�X�N�̏����X�^�b�N�T�C�Y�

    TAG_KNL_MANAGE_SET()
        : mmt_resolution(TARGET_RESOLUTION)
        , cycle_base(SYSTEM_TICK_MSEC)
        , KnlTick_TimerID(0)
        , num_of_task(0)
        , sys_counter(0)
        , Knl_Time()
        , stackSize(INITIAL_TASK_STACK_SIZE)
    {}
} KNL_MANAGE_SET, *PKNL_MANAGE_SET;

//----------------------------------------------------------------------------
// 
typedef struct TAG_APP_MANAGE {
    DWORD proccess_status[PROC_ITEM_ID_MAX];                    // �A�g��۾��̏��
    DWORD proccess_counter[PROC_ITEM_ID_MAX];                   // �A�g��۾����ټ������p

    TAG_APP_MANAGE()
        : proccess_status{}
        , proccess_counter{}
    {}
} APP_MANAGE, *PAPP_MANAGE;
