#pragma once

#include "framework.h"
#include <windows.h>
#include <string>
#include <format>
#include <process.h>
#include <sstream>
#include <iostream>
#include "CHelper.h"

//============================================================================
// define��`
// �X���b�h�Ŏ��s����֐��I��p��`
#define THREAD_WORK_IDLE                0       // �A�C�h������
#define THREAD_WORK_ROUTINE             1       // ��폈��

#define TASK_EVENT_MAX                  8       // �^�X�N�X���b�h�N���C�x���g�ő吔
#define DEFAUT_TASK_CYCLE               50      // �W���^�X�N����

#define REPEAT_INFINIT                  0       // �i�v���[�v
#define TERMINATE_THREAD                1       // ���f

#define LISTVIEW_ROW_MAX                1000    // ���X�g�r���[�s��
#define LISTVIEW_COLUMN_MAX             2       // ���X�g�r���[��
#define LISTVIEW_COLUMN_WIDTH_TIME      90      // ���X�g�r���[��(Time)
#define LISTVIEW_COLUMN_WIDTH_MESSAGE   450     // ���X�g�r���[��(Message)

//============================================================================
// �\����/���p�̒�`
//----------------------------------------------------------------------------
// �^�X�N�I�u�W�F�N�g�C���f�b�N�X�\����    
typedef struct TAG_TASK_INDEX {
    int32_t environment;
    int32_t scada;
    int32_t agent;
    int32_t policy;
    int32_t device;
    int32_t client;
    int32_t manage;
    int32_t dummy;
} TASK_INDEX, *PTASK_INDEX;

//----------------------------------------------------------------------------
// �^�X�N�I�u�W�F�N�g�̌ʊǗ����\����
typedef struct TAG_THREAD_INFO {
    // �I�u�W�F�N�g���ʏ��
    std::wstring name;  // �I�u�W�F�N�g��
    std::wstring sname; // �I�u�W�F�N�g����
    HBITMAP      hbmp;  // �I�u�W�F�N�g���ʗp�r�b�g�}�b�v

    // �X���b�h�ݒ���e
    int32_t  index;                     // �X���b�hIndex
    uint32_t id;                        // �X���b�hID
    HANDLE   hndl;                      // �X���b�h�n���h��
    HANDLE   hevents[TASK_EVENT_MAX];   // �C�x���g�n���h��
    int32_t  n_active_events;           // �L���ȃC�x���g��
    int32_t  event_triggered;           // ���������C�x���g�ԍ�
    uint32_t cycle_ms;                  // �X���b�h���s�ݒ����
    uint32_t cycle_count;               // �X���b�h���s�ݒ���� Tick count(ms/system tick)
    int32_t  trigger_type;              // �X���b�h�N������ ����� or �C�x���g
    int32_t  priority;                  // �X���b�h�̃v���C�I���e�B
    int32_t  thread_com;                // �X���b�h����t���O 0:�J��Ԃ� 1:���f ����ȊO:��������:

    // �X���b�h���j�^���
    DWORD    start_time;        // ���X�L�����̃X���b�h�J�n����
    DWORD    act_time;          // 1�X�L�����̃X���b�h����������
    DWORD    period;            // �X���b�h���[�v��������
    DWORD    act_count;         // �X���b�h���[�v�J�E���^
    DWORD    total_act;         // �N���ώZ�J�E���^
    uint32_t time_over_count;   // �\��������I�[�o�[������

    // �֘A�E�B���h�E�n���h��
    HWND hwnd_parent;       // �e�E�B���h�E�̃n���h��
    HWND hwnd_msgstatics;   // �e�E�B���h�E���b�Z�[�W�\���p�E�B���h�E�ւ̃n���h��
    HWND hwnd_panel;        // �����C���E�B���h�E�̃n���h��(���C���t���[����ɔz�u�j
    HWND hwnd_msglist;      // �����C���E�B���h�E�̃��b�Z�[�W�\���p���X�g�R���g���[���ւ̃n���h��
    HWND hwnd_work;         // ����p��Ɨp�E�C���h�E�̃n���h��

    HINSTANCE hinstance;    // �A�v���P�[�V�����̃C���X�^���X

    // ����p�l���֘A
    int32_t panel_msglist_count;    // �p�l�����b�Z�[�W���X�g�̃J�E���g��
    int32_t panel_func_id;          // �p�l��function�{�^���̑I����e
    int32_t panel_type_id;          // �p�l��type�{�^���̑I����e

    // �O���C���^�[�t�F�[�X
    unsigned long* psys_counter;    // ���C���V�X�e���J�E���^�[�̎Q�Ɛ�|�C���^
    uint32_t       work_select;     // �X���b�h���s�̊֐��̎�ނ��w��

    TAG_THREAD_INFO()
        : name(L"")
        , sname(L"")
        , hbmp(NULL)
        , index(0)
        , id(0)
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
        , hwnd_parent(NULL)
        , hwnd_msgstatics(NULL)
        , hwnd_panel(NULL)
        , hwnd_msglist(NULL)
        , hwnd_work(NULL)
        , hinstance(NULL)
        , panel_msglist_count(0)
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

    // �����o�[�ϐ�
    THREAD_INFO thread_info;

    // �����o�[�֐�
    virtual void init_task(void* pobj);
    unsigned __stdcall run(void* param);    // �X���b�h���s�Ώۊ֐�

    virtual void set_tab_func_txt(void);    // �^�u�p�l����Radio Button��Static�e�L�X�g��ݒ�
    virtual void set_tab_tip_txt(void);     // �^�u�p�l���̐����pStatic�e�L�X�g��ݒ�
    virtual void set_tab(HWND hDlg,
                         UINT msg,
                         WPARAM wp,
                         LPARAM lp);        // �^�u�p�l���̐ݒ�

protected:
    // �����o�[�ϐ�

    // �����o�[�֐�
    void outp_msg_to_mainwnd(const std::string& src);   // ���C���E�B���h�E�ւ̃V���[�g���b�Z�[�W�\��(string)
    void outp_msg_to_mainwnd(const std::wstring& src);  // ���C���E�B���h�E�ւ̃V���[�g���b�Z�[�W�\��(wstring)
    void outp_msg_to_listview(const std::string str);   // ���X�g�R���g���[���ւ̃V���[�g���b�Z�[�W�\��(string)
    void outp_msg_to_listview(const std::wstring str);  // ���X�g�R���g���[���ւ̃V���[�g���b�Z�[�W�\��(wstring)

private:
    // �����o�[�ϐ�
    CRITICAL_SECTION cs_listview;

    CHelper             tool;
    std::ostringstream  s;
    std::wostringstream ws;
    std::wstring        wstr;
    std::string         str;

    // �����o�[�֐�
    virtual void routine_work(void* param);
    virtual void default_work(void* param);
};
