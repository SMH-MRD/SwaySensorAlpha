#pragma once

#include "resource.h"

using namespace std;

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
// .ini �֘A�ݒ�
#define FOLDER_OF_INIFILE               L"Data\\"               // ini�t�@�C���t�H���_
#define EXT_OF_INIFILE                  L"ini"                  // ini�t�@�C���g���q
#define PATH_OF_INIFILE                 pszInifile              // ini�t�@�C���p�X

#define INI_SCTION_COMMON               L"COMMON"               // 
#define INI_KEY_TIMER_PERIOD_SYSTEM     L"TimerPeriodSystem"    // 

#define INI_SCTION_OBJECT               L"OBJECT"               // 
#define INI_KEY_ENVIRONMENT             L"Environment"          // 
#define INI_KEY_POLICY                  L"Policy"               // 
#define INI_KEY_AGENT                   L"Agent"                // 
#define INI_KEY_SCADA                   L"Scada"                // 
#define INI_KEY_HELPER                  L"Helper"               // 

#define INI_SCTION_MOUNTING             L"MOUNTING"             // 
#define INI_KEY_MOUNT_OFFSET_LX0        L"MountOffsetLX0"       // 
#define INI_KEY_MOUNT_OFFSET_LY0        L"MountOffsetLY0"       // 
#define INI_KEY_MOUNT_OFFSET_L0         L"MountOffsetL0"        // 
#define INI_KEY_MOUNT_OFFSET_LC         L"MountOffsetLC"        // 
#define INI_KEY_MOUNT_OFFSET_A0         L"MountOffsetA0"        // 
#define INI_KEY_MOUNT_OFFSET_AC         L"MountOffsetAC"        // 

#define INI_SCTION_CAMERA               L"CAMERA"               // 
#define INI_KEY_CAM_FRAME_SIZE          L"CamFrameSize"         // 
#define INI_KEY_CAM_VIEW_ANGLE          L"CamViewAngle"         // 
#define INI_KEY_CAM_FRAME_RATE          L"CamFrameRate"         // 
#define INI_KEY_CAM_EXPOSURE_TIME       L"CamExposureTime"      // 
#define INI_KEY_CAM_EXPOSURE_AUTO       L"CamExposureAuto"      // 

#define INI_SCTION_PROCESS              L"PROCESS"              // 
#define INI_KEY_IMG_ROI                 L"ImgROI"               // 
#define INI_KEY_MASK_IMG                L"MaskImg"              // 
#define INI_KEY_MASK_IMG1_LOW           L"MaskImg1Low"          // 
#define INI_KEY_MASK_IMG1_UPP           L"MaskImg1Upp"          // 
#define INI_KEY_MASK_IMG2_LOW           L"MaskImg2Low"          // 
#define INI_KEY_MASK_IMG2_UPP           L"MaskImg2Upp"          // 
#define INI_KEY_NOISE_FILTER1           L"NoiseFilter1"         // 
#define INI_KEY_NOISE_FILTER2           L"NoiseFilter2"         // 
#define INI_KEY_ALGORITHM               L"Algorithm"            // 
#define INI_KEY_FILTER                  L"Filter"               // 
#define INI_KEY_IMG_SAVE_FILENAME       L"ImgSaveFileName"      // 

#define INI_SCTION_RIO                  L"RIO"                  // 
#define INI_KEY_IP_ADDRS                L"IPAddrs"              // 
#define INI_KEY_TCP_PORT                L"TCPPort"              // 
#define INI_KEY_SLAVE_ADDRS             L"SlaveAddrs"           // 
#define INI_KEY_TIME_OUT                L"TimeOut"              // 
#define INI_KEY_PORT_NUM_X              L"PortNumX"             // 
#define INI_KEY_PORT_NUM_Y              L"PortNumY"             // 

#define INI_SCTION_ETHER                L"ETHER"                // 
#define INI_KEY_LOCAL_IP_ADDRS          L"LocalIPAddrs"         // 
#define INI_KEY_LOCAL_PORT              L"LocalPort"            // 
#define INI_KEY_REMOTE_IP_ADDRS         L"RemoteIPAddrs"        // 
#define INI_KEY_REMOTE_PORT             L"RemotePort"           // 

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
typedef struct tagKNL_MANAGE_SET {
    WORD          mmt_resolution;                               // �}���`���f�B�A�^�C�}�[�̕���\
    unsigned int  cycle_base;                                   // ���C���X���b�h����
    WORD          KnlTick_TimerID;                              // �}���`���f�B�A�^�C�}�[��ID
    unsigned int  num_of_task;                                  // �A�v���P�[�V�����ŗ��p����X���b�h��
    unsigned long sys_counter;                                  // �V�X�e���J�E���^
    SYSTEMTIME    Knl_Time;                                     // �A�v���P�[�V�����J�n����̌o�ߎ���
    unsigned int  stackSize;                                    // �^�X�N�̏����X�^�b�N�T�C�Y�

    tagKNL_MANAGE_SET() : mmt_resolution(TARGET_RESOLUTION),    // �}���`���f�B�A�^�C�}�[�̕���\
                          cycle_base(SYSTEM_TICK_MSEC),         // ���C���X���b�h����
                          KnlTick_TimerID(0),                   // �}���`���f�B�A�^�C�}�[��ID
                          num_of_task(0),                       // �A�v���P�[�V�����ŗ��p����X���b�h��
                          sys_counter(0),                       // �V�X�e���J�E���^
                          Knl_Time(),                           // �A�v���P�[�V�����J�n����̌o�ߎ���
                          stackSize(INITIAL_TASK_STACK_SIZE)    // �^�X�N�̏����X�^�b�N�T�C�Y
    {}
} ST_KNL_MANAGE_SET, *PST_MANAGE_SET;

//----------------------------------------------------------------------------
// 
typedef struct tagAPP_MANAGE {
    DWORD proccess_status[PROC_ITEM_ID_MAX];                    // �A�g��۾��̏��
    DWORD proccess_counter[PROC_ITEM_ID_MAX];                   // �A�g��۾����ټ������p
} ST_APP_MANAGE, *PST_APP_MANAGE;
