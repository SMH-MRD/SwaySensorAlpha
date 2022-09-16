#pragma once

#include <windows.h>
#include <process.h>
#include <sstream>
#include <iostream>
#include "CHelper.h"

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
typedef struct TAG_TASK_INDEX {
    int environment;
    int scada;
    int agent;
    int policy;
    int device;
    int client;
    int manage;
    int dummy;
} TASK_INDEX, *PTASK_INDEX;

//----------------------------------------------------------------------------
// �^�X�N�I�u�W�F�N�g�̌ʊǗ����\����
typedef struct TAG_THREAD_INFO {
    // �I�u�W�F�N�g���ʏ��
    std::wstring name;  // �I�u�W�F�N�g��
    std::wstring sname; // �I�u�W�F�N�g����
    HBITMAP hBmp;       // �I�u�W�F�N�g���ʗp�r�b�g�}�b�v

    // �X���b�h�ݒ���e
    int          index; // �X���b�hIndex
    unsigned int ID;    // �X���b�hID
    HANDLE       hndl;  // �X���b�h�n���h��
    HANDLE       hevents[TASK_EVENT_MAX];   // �C�x���g�n���h��
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
    unsigned int   work_select;                                 // �X���b�h���s�̊֐��̎�ނ��w��

    TAG_THREAD_INFO()
        : name(L"")
        , sname(L"")
        , hBmp(NULL)
        , index(0)
        , ID(0)
        , hndl(NULL)
        , hevents{}
        , n_active_events(1)
        , event_triggered(0)
        , cycle_ms(DEFAUT_TASK_CYCLE)
        , cycle_count(0)
        , trigger_type(0)
        , priority(THREAD_PRIORITY_NORMAL)
        , thread_com(REPEAT_INFINIT)
        , start_time(0)
        , act_time(0)
        , period(0)
        , act_count(0)
        , total_act(0)
        , time_over_count(0)
        , hWnd_parent(NULL)
        , hWnd_msgStatics(NULL)
        , hWnd_opepane(NULL)
        , hWnd_msgList(NULL)
        , hWnd_work(NULL)
        , hInstance(NULL)
        , cnt_PNLlist_msg(0)
        , panel_func_id(0)
        , panel_type_id(0)
        , psys_counter(NULL)
        , work_select(THREAD_WORK_IDLE)
    {}
} THREAD_INFO, *PTHREAD_INFO;

//////////////////////////////////////////////////////////////////////////////
// CTaskObj

class CTaskObj
{
public:
    CTaskObj();
    virtual ~CTaskObj();

    THREAD_INFO ThreadInfo;

    virtual void init_task(void* pobj);
    unsigned __stdcall run(void* param);    // �X���b�h���s�Ώۊ֐�

    virtual LRESULT CALLBACK cb_panel_proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    unsigned int set_work(int work_id);
    virtual void set_window(void);

protected:
    CHelper             tool;
    std::ostringstream  s;
    std::wostringstream ws;
    std::wstring        wstr;
    std::string         str;

    virtual void routine_work(void* param);
    virtual void optional_work1(void* param);
    virtual void optional_work2(void* param);
    virtual void default_work(void* param);

private:
};
