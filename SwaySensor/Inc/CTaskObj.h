#pragma once

#include <windows.h>
#include <process.h>
#include <sstream>
#include <iostream>
#include "CHelper.h"

using namespace std;

//============================================================================
// define��`
// �X���b�h�Ŏ��s����֐��I��p��`
#define THREAD_WORK_IDLE                0   // �A�C�h������
#define THREAD_WORK_ROUTINE             1   // ��폈��
#define THREAD_WORK_OPTION1             2   // �I�v�V��������
#define THREAD_WORK_OPTION2             3   // �I�v�V��������

#define TASK_EVENT_MAX                  8   // �^�X�N�X���b�h�N���C�x���g�ő吔
#define DEFAUT_TASK_CYCLE               50  // �W���^�X�N����

#define REPEAT_INFINIT                  0   // �i�v���[�v
#define TERMINATE_THREAD                1   // ���f

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
// �^�X�N�I�u�W�F�N�g�C���f�b�N�X�\����    
typedef struct tagTASK_INDEX {
    int environment;
    int scada;
    int agent;
    int policy;
    int device;
    int client;
    int manage;
    int dummy;
} ST_TASK_INDEX, *PST_TASK_INDEX;

//----------------------------------------------------------------------------
// �^�X�N�I�u�W�F�N�g�̌ʊǗ����\����
typedef struct tagTHREAD_INFO {
    // �I�u�W�F�N�g���ʏ��
    WCHAR   name[24];   // �I�u�W�F�N�g��
    WCHAR   sname[8];   // �I�u�W�F�N�g����
    HBITMAP hBmp;       // �I�u�W�F�N�g���ʗp�r�b�g�}�b�v

    // �X���b�h�ݒ���e
    int          index; // �X���b�hIndex
    unsigned int ID;    // �X���b�hID
    HANDLE       hndl;  // �X���b�h�n���h��
    HANDLE       hevents[TASK_EVENT_MAX];   // �C�x���g�n���h��
//  int          n_active_events = 1;       // �L���ȃC�x���g��
    int          n_active_events;                               // �L���ȃC�x���g��
    int          event_triggered;                               // ���������C�x���g�ԍ�
    unsigned int cycle_ms;                                      // �X���b�h���s�ݒ����
    unsigned int cycle_count;                                   // �X���b�h���s�ݒ���� Tick count(ms/system tick)
    int          trigger_type;                                  // �X���b�h�N������ ����� or �C�x���g
    int          priority;                                      // �X���b�h�̃v���C�I���e�B
    int          thread_com;                                    // �X���b�h����t���O 0:�J��Ԃ� 1:���f ����ȊO:��������:

    // �X���b�h���j�^���
    DWORD        start_time;                                    // ���X�L�����̃X���b�h�J�n����
    DWORD        act_time;                                      // 1�X�L�����̃X���b�h����������
    DWORD        period;                                        // �X���b�h���[�v��������
    DWORD        act_count;                                     // �X���b�h���[�v�J�E���^
    DWORD        total_act;                                     // �N���ώZ�J�E���^
    unsigned int time_over_count;                               // �\��������I�[�o�[������

    // �֘A�E�B���h�E�n���h��
    HWND hWnd_parent;                                           // �e�E�B���h�E�̃n���h��
    HWND hWnd_msgStatics;                                       // �e�E�B���h�E���b�Z�[�W�\���p�E�B���h�E�ւ̃n���h��
    HWND hWnd_opepane;                                          // �����C���E�B���h�E�̃n���h��(���C���t���[����ɔz�u�j
    HWND hWnd_msgList;                                          // �����C���E�B���h�E�̃��b�Z�[�W�\���p���X�g�R���g���[���ւ̃n���h��
    HWND hWnd_work;                                             // ����p��Ɨp�E�C���h�E�̃n���h��

    HINSTANCE hInstance;

    // ����p�l���֘A
    int cnt_PNLlist_msg;                                        // �p�l�����b�Z�[�W���X�g�̃J�E���g��
    int panel_func_id;                                          // �p�l��function�{�^���̑I����e
    int panel_type_id;                                          // �p�l��type�{�^���̑I����e

    // �O���C���^�[�t�F�[�X
    unsigned long* psys_counter;                                // ���C���V�X�e���J�E���^�[�̎Q�Ɛ�|�C���^
    unsigned       work_select;                                 // �X���b�h���s�̊֐��̎�ނ��w��

    tagTHREAD_INFO() : n_active_events(1),                      // �L���ȃC�x���g��
                       cnt_PNLlist_msg(0),                      // �p�l�����b�Z�[�W���X�g�̃J�E���g��
                       panel_func_id(1),                        // �p�l��function�{�^���̑I����e
                       panel_type_id(1),                        // �p�l��type�{�^���̑I����e
                       work_select(0)                           // �X���b�h���s�̊֐��̎�ނ��w��
    {}
} ST_THREAD_INFO, *PST_THREAD_INFO;

class CTaskObj
{
public:
    CTaskObj();
    virtual ~CTaskObj();

    ST_THREAD_INFO ThreadInfo;

    virtual void InitTask(void* pobj);
    unsigned __stdcall Run(void* param);    // �X���b�h���s�Ώۊ֐�

    virtual LRESULT CALLBACK PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    unsigned int SetWork(int work_id);
    virtual void SetPanelPbTxt(void);   // �^�u�p�l����Function�{�^����Static�e�L�X�g��ݒ�

protected:
    CHelper        tool;
    ostringstream  s;
    wostringstream ws;
    wstring        wstr;
    string         str;

    virtual void RoutineWork(void* param);
    virtual void OptionalWork1(void* param);
    virtual void OptionalWork2(void* param);
    virtual void DefaultWork(void* param);

private:
};
