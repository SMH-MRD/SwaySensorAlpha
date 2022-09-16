#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include "CTeliCamLib.h"

//============================================================================
// define��`

//============================================================================
// �\����/���p�̒�`

//////////////////////////////////////////////////////////////////////////////
// CEnvironment

class CEnvironment : public CTaskObj
{
public:
    CEnvironment();
    ~CEnvironment();

    void init_task(void* param);
    void routine_work(void* param);

protected:

private:
    CShared       m_shared;
    CTeliCamLib*  m_telicam;            // CTeliCamLib object
    HANDLE        m_td_camstart_hndl;   // Thread Handle
    BOOL          m_td_camstart_state;  // Thread status flag (FALSE:Exit TRUE:Run)
    CONFIG_CAMERA m_cnfgcam;            // �J�����ݒ�

    void set_window(void);
    static unsigned WINAPI td_camstart(LPVOID lpParam); //
	HRESULT set_telicam_property(void); // Set of TeliCam property
};
