#pragma once

#include "CTaskObj.h"
#include "CShared.h"
#include <opencv2/opencv.hpp>           // OpenCV
#include <opencv2/core/core.hpp>        // OpenCV
#include <opencv2/highgui/highgui.hpp>  // OpenCV
#include <opencv2/video/video.hpp>      // OpenCV

//============================================================================
// define定義
#define PI              3.14159     // π
#define CONV_DEG_RAD    (PI/180.0)  // deg→rad

//============================================================================
// enum定義

//============================================================================
// 構造体/共用体定義
//----------------------------------------------------------------------------
#define MOVE_AVERAGE_COUNT          1           // 移動平均数
#define MOVE_AVERAGE_BUFFER_SIZE    20          // 移動平均バッファサイズ
typedef struct TAG_MOVE_AVERAGE_BUFFER {        // 移動平均バッファ
    double data[MOVE_AVERAGE_BUFFER_SIZE];      // データ
    int    wptr;                                // 書込みポインタ
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

    MOVE_AVERAGE_BUFFER m_maxv_avrg;    // 輝度移動平均
    
#if 0
    stCameraParamData   m_camparam;     // カメラ設定データ
    stCommonParamData   m_cmmnparam;    // 構造設定データ
    stImgProcParamData  m_imgprocparam; // 画像処理設定データ
    stProcInfoData      m_procinfo;     // 画像処理結果
#endif

    void proc_main(void);
    BOOL proc_center_gravity(void);
    void proc_sway(void);

    double get_maxv_avrg(void);
    void   set_expstime(void);

    void set_window(void);
};
