///////////////////////////////////////////////////////////////////////////////
// file    : RegisterMap_BG_Type2.h
// brief   : Register map of BG series Type 2
// version : 1.0.0.0
// author  : (C)2015 TOSHIBA TELI CORPORATION 
// date    : 2015/02/16

/////////////////////////////////////////////////////////
//
// Register Map for BG series Type 2 
//
// For
//  B/W:   BG205M,
//  Color: BG205MC
//
/////////////////////////////////////////////////////////

#ifndef _REGISTERMAP_BG_TYPE2_H
#define _REGISTERMAP_BG_TYPE2_H


namespace Teli {
    namespace RegMapBGType2 {
    
        //================================================================
        // Register map of BG series Type 2
        //  
        // Register address symbol:  ADR_[register name]_[Data type]
        // [Datatype] definition
        //     I:                    Integer data register     (4 bytes).
        //     F:                    Float data register       (4 bytes).
        //     E:                    Enumeration data register (4 bytes).
        //     C:                    Command register          (4 bytes). 
        //     C[N]:                 Character array register  (N bytes). 
        //  
        //=================================================================
        enum _REG_ADR_BG
        {
            //==========================================================================
            // Bootstrap register map defined by GigE Vision specification
            // Adress data whose integer value is "0x0" is address for registers tha current BG seriees does not support.  
            //==========================================================================
            ADR_GEV_VERSION_MAJOR_I         = 0x00000000,           // RO   GigE Vision majopr version.     bit0  to bit15 in Big Endian.
            ADR_GEV_VERSION_MINOR_I         = 0x00000000,           // RO   GigE Vision minor version.      bit16 to bit31 in Big Endian.
            ADR_GEV_DEV_MODE_IS_BIGENDIAN_I = 0x00000004,           // RO   Endian of register data. Always BigEndiann (bit0 = 1).
            ADR_DEV_MAC_ADDR_0_H_I          = 0x00000008,           // RO   Higher part MAC address of network adapter #0.          
            ADR_DEV_MAC_ADDR_0_L_I          = 0x0000000C,           // RO   Lower part MAC address of network adapter #0.
            ADR_SUPPORTED_IP_CONFIG_0_I     = 0x00000010,           // RO   Supported IP configuration method of network adapter #0.
            ADR_CURRENT_IP_CONFIG_0_I       = 0x00000014,           // RW   Current IP configuration method of network adapter #0.
            ADR_CURRENT_IP_ADDRESS_0_I      = 0x00000024,           // RO   Current IP Address of network adapter #0.
            ADR_CURRENT_SUBNET_MASK_0_I     = 0x00000034,           // RO   Current subnet mask of network adapter #0.
            ADR_CURRENT_DEFAULT_GATEWAY_0_I = 0x00000044,           // RO   Current default gateway of network adapter #0.
            ADR_DEV_VENDOR_NAME_C32         = 0x00000048,           // RO   Vendor name of the camera.
            ADR_DEV_MODEL_NAME_C32          = 0x00000068,           // RO   Model name of the camera.
            ADR_DEV_VERSION_C32             = 0x00000088,           // RO   Version of the camera.
            ADR_DEV_FIRMWARE_VERSION_C32    = 0x0000A000,           // RO   Firmware version of the camera.
            ADR_DEV_MANUFACTURER_INFO_C48   = 0x000000A8,           // RO   Manufacture information of the camera.
            ADR_DEV_SERIAL_NUMBER_C16       = 0x000000D8,           // RO   Serial number of the camera. (DeviceID)
            ADR_DEV_USER_DEFINED_NAME_C16   = 0x000000E8,           // RW   User defined name of the camera. (Device User ID)
            ADR_DEV_SFNCVERSION_MAJOR_I     = 0x0000A200,           // RO   Major version of Standard Feature Name Convention.
            ADR_DEV_SFNCVERSION_MINOR_I     = 0x0000A204,           // RO   Minor version of Standard Feature Name Convention.
            ADR_DEV_SFNCVERSION_SUB_I       = 0x0000A208,           // RO   Sub Minor version of Standard Feature Name Convention.
        
            ADR_FIRST_URL_XML_C512          = 0x00000200,           // RO   The first URL of camera description file (XML file)
            ADR_SECOND_URL_XML_C512         = 0x00000400,           // RO   The Second URL of camera description file (XML file)

            ADR_NUM_NETWORK_INTERFACE_I     = 0x00000600,           // RO   Number of network interface exists in the camera.
            ADR_PERSISTENT_IP_ADDRESS_0_I   = 0x0000064C,           // RW   IP addres setting of network interface #0 for Persistent IP mode.
            ADR_PERSISTENT_SUBNET_MASK_0_I  = 0x0000065C,           // RW   Subnet mask setting of network interface #0 for Persistent IP mode.
            ADR_PERSISTENT_DFLT_DATEWAY_0_I = 0x0000066C,           // RW   Default gateway setting of network interface #0 for Persistent IP mode.
            ADR_LINK_SPEED_0_I              = 0x00000670,           // R0   The speed of transmission negotiated byf network interface #0.

            ADR_DEV_MAC_ADDR_1_H_I          = 0x00000680,           // RO   Higher part MAC address of network interface #1.  Network interface #1 does not exist in current BG series. For future use. 
            ADR_DEV_MAC_ADDR_1_L_I          = 0x00000684,           // RO   Lower part MAC address of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_SUPPORTED_IP_CONFIG_1_I     = 0x00000688,           // RO   Supported IP configuration method of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_CURRENT_IP_CONFIG_1_I       = 0x0000068C,           // RW   Current IP configuration method of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_CURRENT_IP_ADDRESS_1_I      = 0x0000069C,           // RO   Current IP Address of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_CURRENT_SUBNET_MASK_1_I     = 0x000006AC,           // RO   Current subnet mask of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_CURRENT_DEFAULT_GATEWAY_1_I = 0x000006BC,           // RO   Current default gateway of network interface #1.  Network interface #1 does not exist in current BG series. For future use.
            ADR_PERSISTENT_IP_ADDRESS_1_I   = 0x000006CC,           // RW   IP addres setting of network interface #1 for Persistent IP mode.  Network interface #1 does not exist in current BG series. For future use.
            ADR_PERSISTENT_SUBNET_MASK_1_I  = 0x000006DC,           // RW   Subnet mask setting of network interface #1 for Persistent IP mode.  Network interface #1 does not exist in current BG series. For future use.
            ADR_PERSISTENT_DFLT_DATEWAY_1_I = 0x000006EC,           // RW   Default gateway setting of network interface #1 for Persistent IP mode.  Network interface #1 does not exist in current BG series. For future use.
            ADR_LINK_SPEED_1_I              = 0x000006F0,           // R0   The speed of transmission negotiated byf network interface #1.  Network interface #1 does not exist in current BG series. For future use.

            ADR_NUMBER_OF_MESSAGE_CHANNEL_I = 0x00000900,           // RO   Number of message channel exists in the camera.
            ADR_NUMBER_OF_STREAM_CHANNEL_I  = 0x00000904,           // RO   Number of stream channel exists in the camera.

            ADR_MESSAGE_CH_CAPABILITY_I     = 0x00000930,           // RO   Information of supported non-mandatory message channel features.
            ADR_GVCP_CAPABILITY_I           = 0x00000934,           // RO   Information of supported non-mandatory GigE Vision Control Protocol features.
            ADR_HEARTBEAT_TIMEOUT_I         = 0x00000938,           // RW   Time out value of heartbeat in milliseconds.
            ADR_TIMESTAMP_TICK_FREQUENCYH_I = 0x0000093C,           // R0   Higher 32bit value of timestamp clock frequenc.
            ADR_TIMESTAMP_TICK_FREQUENCYL_I = 0x00000940,           // R0   Lower 32bit value of timestamp clock frequenc.
            ADR_TIMESTAMP_CONTROL_C         = 0x00000944,           // W0   Current timestamp clock value is latched to Timestamp value register when "1" is written to thos register.
            ADR_TIMESTAMP_VALUE_H_I         = 0x00000948,           // RO   Higher 32bit of latched timestamp value.
            ADR_TIMESTAMP_VALUE_L_I         = 0x0000094C,           // RO   Lower 32bit of latched timestamp value.
            ADR_GVCP_CONFIGURATION_I        = 0x00000954,           // RW   Configuration of GigE Vision Control Protocol features including disable/enable  heartbeat. Bit31 is Heartbeat_disable bit. 

            ADR_CCP_I                       = 0x00000A00,           // RW   Control Channel Privilege.  [g_saVAL_CCP]
            ADR_MCP_I                       = 0x00000B00,           // RW   Message Channel Port. Bit 12-15: network interface index, Bit 16-31: host port.
            ADR_MCDA_I                      = 0x00000B10,           // RW   Message Channel Destination Address.
            ADR_MCTT_I                      = 0x00000B14,           // RW   Message Channel Transmission Timeout in milliseconds.
            ADR_MCRC_I                      = 0x00000B18,           // RW   Message Channel Retry Count.
            ADR_MCSP_I                      = 0x00000B1C,           // RO   Message Channel Source Port.
            ADR_SCP_0_I                     = 0x00000D00,           // RW   Stream Channel #0 Host Port.    ( bit16 to 31: Host Port, bit12 to 15 : networlinterface index bit0: direction in Big Endian)   Restricted by TLParamsLocked.
            ADR_SCPS_0_I                    = 0x00000D04,           // RW   Stream Channel #0 Packet Size.  (bit16 to 31 in BigEndian)  72 to 16364.                                                        Restricted by TLParamsLocked.
            ADR_SCPD_0_I                    = 0x00000D08,           // RW   Stream Channel #0 Packet Delay in timestamp counter unit.
            ADR_SCDA_0_I                    = 0x00000D18,           // RW   Stream Channel #0 Destination Address.                                                                                          Restricted by TLParamsLocked.   
            ADR_SCSP_0_I                    = 0x00000D1C,           // RO   Stream Channel #0 Source Port.
            ADR_SCC_0_I                     = 0x00000D20,           // RO   Stream Channel #0 Capabilty.
            ADR_SCCFG_0_I                   = 0x00000D24,           // RW   Stream Channel #0 Configuration.

            ADR_MANIFEST_TABLE_C512         = 0x00009000,           // RO   Manifest table (Optional)


            //====================================
            //Manufacturer specific registers
            //====================================
            ADR_DEV_RESET_C                 = 0x0000A038,           // WO   Device reset command register.
            ADR_DEV_REG_STRM_START_C        = 0x0000A03C,           // WO   Prepare the device for registers streaming without checking for consistency.
            ADR_DEV_REG_STRM_END_C          = 0x0000A040,           // WO   Announce the end of registers streaming. This will do a register set validation for consistency and activate it. This will also update the DeviceRegistersValid flag.
            ADR_DEV_REG_CHECK_C             = 0x0000A044,           // WO   Execute consistency chack. The result canbe read from Reg_VAlid register. 
            ADR_DEV_REG_VALID_I             = 0x0000A048,           // RO   Result of consistency check. 0: NG, 1: OK.
        
            // Image Format Control
            ADR_SENSOR_WIDTH_I              = 0x0000A800,           // RO   Sensor width.   
            ADR_SENSOR_HEIGHT_I             = 0x0000A818,           // RO   Sensor height.
            ADR_WIDTH_I                     = 0x0000A804,           // RW,(RO)  Width of output image in pixel.                 Restricted by TLParamsLocked.
            ADR_HEIGHT_I                    = 0x0000A81C,           // RW,(RO)  Height of output image in pixel.                Restricted by TLParamsLocked.
            ADR_OFFSET_X_I                  = 0x0000A838,           // RW,(RO)  X direction offset of output image in pixel.    Restricted by TLParamsLocked.
            ADR_OFFSET_Y_I                  = 0x0000A84C,           // RW,(RO)  Y direction offset of output image in pixel.    Restricted by TLParamsLocked.
            ADR_BINNING_H_I                 = 0x0000A874,           // RW,(RO)  Horizontal binning.                             Restricted by TLParamsLocked.
            ADR_BINNING_V_I                 = 0x0000A888,           // RW,(RO)  Vertical binning.                               Restricted by TLParamsLocked.
            ADR_DECIMATION_H_I              = 0x0000A89C,           // RW,(RO)  Horizontal decimation.
            ADR_DECIMATION_V_I              = 0x0000A8B0,           // RW,(RO)  Vertical decimation.
            ADR_REVERSE_X_I                 = 0x0000A8C4,           // RW,(RO)  Horizontal reverse (mirror).                    Not implemented in current BG Series.
            ADR_REVERSE_Y_I                 = 0x0000A8CC,           // RW,(RO)  Vertical reverse (mirror).                      Not implemented in current BG Series.
            ADR_PIXEL_FORMAT_E              = 0x0000A8D4,           // RW,(RO)  Pixel format. [g_saVAL_PixFmt]
            ADR_PIXEL_SIZE_E                = 0x0000A8E4,           // RW,(RO)  Pixel size. [g_saVAL_PixSizeBG]
            ADR_PIXEL_COLOR_FILTER_E        = 0x0000A8E8,           // RW,(RO)  Pixel color filter. [g_saVAL_PixClrFlt]
            ADR_TESTIMAGE_SELECTOR_E        = 0x0000A904,           // RW       Selects Test pattern. [g_saVAL_TstImg]


            // Acquisition and Trigger Controls
            ADR_ACQ_MODE_E                  = 0x0000B000,           // RW,(RO)  Acquisition mode. [g_saVAL_AcqMd] BG series supports "Continuous" only.     Restricted by TLParamsLocked
            ADR_ACQ_START_C                 = 0x0000B008,           // WO       Start Acquisition command.
            ADR_ACQ_STOP_C                  = 0x0000B00C,           // WO       Stop  Acquisition command.
            ADR_ACQ_ABORT_C                 = 0x0000B010,           // WO       Abort Acquisition command.
            //ADR_ACQ_FRAME_COUNT_I         = 0x0,                  //          Frame count for "MultiFrame" acqusition mode.   Not implemented in current BG Series.
            //ADR_ACQ_FRAME_COUNT_MIN_I     = 0x0,                  //          Minimum value of frame count register.          Not implemented in current BG Series.
            //ADR_ACQ_FRAME_COUNT_MAX_I     = 0x0,                  //          Maximum value of frame count register.          Not ot implemented in current BG Series.
            //ADR_ACQ_FRAME_COUNT_INC_I     = 0x0,                  //          Increment　value of frame count register.       Not ot implemented in current BG Series.
            ADR_ACQ_BURST_FRAME_COUNT_I     = 0x0000B01C,           // RW,(R)   Frame count for "FrameBurstStart" trigger mode.
            ADR_ACQ_FRAMERATE_F             = 0x0000B030,           // RW,(RO)  Frame rate.                                     Restricted by TLParamsLocked
            ADR_ACQ_FRAMERATE_MIN_F         = 0x0000B038,           // RO       Minimum value of frame rate register.
            ADR_ACQ_FRAMERATE_MAX_F         = 0x0000B03C,           // RO       Maximum value of frame rate register.

            ADR_TRIGGER_SOFTWARE_C          = 0x0000B0A0,           // WO       Trigger command for software trigger mode.      1: Execution.

            ADR_TRIGGER_MODE_E              = 0x0000B220,           // RW,(RO)  Trigger mode [Frame Start mode]. See g_saVAL_OnOff.
            ADR_TRIGGER_SOURCE_E            = 0x0000B228,           // RW,(RO)  Trigger source [Frame Start mode].  See g_saVAL_TrgSrc.
            ADR_TRIGGER_ACTIVATION_E        = 0x0000B22C,           // RW,(RO)  Trigger mode [Frame Start mode]. See g_saVAL_OnOff.
            ADR_TRIGGER_DELAY_F             = 0x0000B234,           // RW,(RO)  Trigger delay in microseconds [Frame Start mode]. 
            ADR_TRIGGER_SAMPLING_F          = 0x0000B238,           // RW,(RO)  Trigger sampling period in microseconds [Frame Start mode]. 

            ADR_TRIGGER_MODE_BURST_E        = 0x0000B240,           // RW,(RO)  Trigger mode [Frame Burst Start mode]. See g_saVAL_OnOff.
            ADR_TRIGGER_SOURCE_BURST_E      = 0x0000B248,           // RW,(RO)  Trigger source [Frame Burst Start mode].    See g_saVAL_TrgSrc.
            ADR_TRIGGER_ACTIVATION_BURST_E  = 0x0000B24C,           // RW,(RO)  Trigger mode [Frame Burst Start mode]. See g_saVAL_OnOff.
            ADR_TRIGGER_DELAY_BURST_F       = 0x0000B254,           // RW,(RO)  Trigger delay in microseconds [Frame Burst Start mode]. 
            ADR_TRIGGER_SAMPLING_BURST_F    = 0x0000B258,           // RW,(RO)  Trigger sampling period in microseconds [FrameBurst  Start mode]. 

            ADR_EXPOSURE_MODE_E             = 0x0000B110,           // RW,(WO)  Exposure mode. [g_saVAL_ExpMd]              Restricted by TLParamsLocked
            ADR_EXPOSURE_TIME_F             = 0x0000B118,           // RW       Exposuretime in microseconds.
            ADR_EXPOSURE_TIME_MIN_F         = 0x0000B120,           // RO       Minimum value of Exposure time in microseconds.
            ADR_EXPOSURE_TIME_MAX_F         = 0x0000B124,           // RO       Maximum value of Exposure time in microseconds.
            ADR_EXPOSURE_AUTO_E             = 0x0000B140,           // RW       Automatic exposure mode. [g_saVAL_AutoCont] Restricted by TLParamsLocked

            ADR_GAIN_F                      = 0x0000D000,           // RW       Gain in db. 0.0 to 18.0 db.
            ADR_GAIN_AUTO_E                 = 0x0000D010,           // RW       Autonmatic gain mode. [g_saVAL_AutoCont]

            ADR_BLACKLEVEL_F                = 0x0000D040,           // RW       Black lebvel. -5.0 to 25.0.
            
            ADR_BALANCERATIO_RED_F          = 0x0000D080,           // RW       Red color balance ratio(red / green).   0.5 to 8.0.  Color model only.
            ADR_BALANCERATIO_BLUE_F         = 0x0000D088,           // RW       Blue color balance ratio(blue / green). 0.5 to 8.0.  Color model only.
            ADR_BALANCEWHITE_AUTO_E         = 0x0000D08C,           // RW       Automatic color balance. [g_saVAL_AutoOnce] Color model only.

            ADR_GAMMA_F                     = 0x0000D0C0,           // RW       Gamma.  0.45 to 1.0.  

            ADR_LUT_ENABLE_I                = 0x0000F000,           // RW       Look up table enable control. [g_saVAL_OnOff]
            ADR_LUT_VALUE_TOP_I             = 0x000010000,          //          Top address of look up table. Maximum 1024 members.

            ADR_LINE0_INVERTER_I            = 0x0000B810,           // RW       Polarity setting of IO Line #0. [g_saVAL_IOInv]
            ADR_LINE0_SOURCE_E              = 0x0000B818,           // RW       Signal source of Line #0. [g_saVAL_LineSrc]     Not available because Line 0 is input in current BG series. 
            ADR_LINE1_INVERTER_I            = 0x0000B820,           // RW       Polarity setting of IO Line #1. [g_saVAL_IOInv]
            ADR_LINE1_SOURCE_E              = 0x0000B828,           // RW       Signal source of Line #1 [g_saVAL_LineSrc]  
            ADR_LINE2_INVERTER_I            = 0x0000B830,           // RW       Polarity setting of IO Line #2. [g_saVAL_IOInv]
            ADR_LINE2_SOURCE_E              = 0x0000B838,           // RW       Signal source of Line #2. [g_saVAL_LineSrc] 

            ADR_EVENT_SELECTOR_E            = 0x0000C800,           // RW       Event selector. [g_saVAL_EventSel]  

            ADR_EVNT_NTFY_FRM_TRG_E         = 0x0000C810,           // RW       On/Off of Frame trigger event. [g_saVAL_OnOff]      
            ADR_EVNT_NTFY_ALC_LATEST_INF_E  = 0x0000c824,           // RW       On/Off of ALC Latest Information event. [g_saVAL_OnOff]
            ADR_EVNT_NTFY_ALC_CONVERGED_E   = 0x0000C828,           // RW       On/Off of ALC Converged event. [g_saVAL_OnOff]
            ADR_EVNT_NTFY_FRM_TRG_ERROR_E   = 0x0000C830,           // RW       On/Off of Frame trigger error event. [g_saVAL_OnOff]
            ADR_EVNT_NTFY_FRM_TRG_WAIT_E    = 0x0000C834,           // RW       On/Off of Frame trigger wait event. [g_saVAL_OnOff]

            ADR_TIMER_DURATION_F            = 0x0000C000,           // RW       Duration of timer in microseconds.  0.0 to 4095 microseconds.   Restricted by TLParamsLocked
            ADR_TIMER_DELAY_F               = 0x0000C004,           // RW       Timer delay in microseconds. 0.0 to 4095 microseconds.      Restricted by TLParamsLocked
            ADR_TIMER_TRIGGER_SOURCE_E      = 0x0000C014,           // RW       Trigger source of timer. [g_saVAL_TmrTrgSrc]        Restricted by TLParamsLocked
            ADR_TIMER_TRG_ACTIVATION_E      = 0x0000C018,           // RW       Trigger activation of timer. [g_saVAL_TrgAct]       Restricted by TLParamsLocked

            // UserSetControl
            ADR_USERSET_SELECTOR_E          = 0x0000E800,           // RW       Userset selector. [g_saVAL_UsrsetSel]
            ADR_USERSET_LOAD_C              = 0x0000E804,           // WO       Loading userset data register.  Restricted by TLParamsLocked.
            ADR_USERSET_SAVE_C              = 0x0000E808,           // WO       Saving userset data register.
            ADR_USERSET_DEFAULT_E           = 0x0000E80C,           // RW       Select default userset which is loaded on device reset. [g_saVAL_UsrsetSel]

            ADR_PAYLOAD_SIZE_I              = 0x0000E000,           // RO       Maximum (image) data size in byte sent for one ID in the data payload packets on the stream channel.

            //==============================================
            //TeliUnique
            //==============================================
            ADR_HUE_F                       = 0x0000D0D0,           // RW       Hue data.   -40 to 35. Only for color model.
            ADR_SATURATION_U_F              = 0x0000D0D4,           // RW       Saturation of U component in YUV data.  0 to 2.5.   Only for color model.
            ADR_SATURATION_V_F              = 0x0000D0D8,           // RW       Saturation of V component in YUV data.  0 to 2.5.   Only for color model.
            ADR_COLOR_CORRECTION_MATRIX_F   = 0x0000D0F0,           // RW       Top address of Color correction matrix data registers.  -1.0 to 1.0.  Address = ADR_COLOR_CORRECTION_MATRIX_F + I * LEN_I + J * LEN_J
            ADR_SHARPNESS_I                 = 0x0000D0E0,           // RW       Sharpness setting.  0 to 7.     Only for color model.

            ADR_ALC_PM_AREA_SIZE_I          = 0x0000D824,           // RW       Automatic Luminance Control area size. [g_saVAL_ALC_Area]   Only for color model.
            ADR_ALC_REF_LUMINANCE_I         = 0x0000D800,           // RW       Reference luminance value of Automatic Luminance Control,   0 to 255.   Only for color model.           
            ADR_ALC_EXPOSURE_VALUE_F        = 0x0000D820,           // RW       Reference exposurevalue in eV.  -2.0 to 2.0[eV].    Only for color model.
            ADR_ALC_TOLERANCE_F             = 0x0000D804,           // RW       Tolerance of ALC in %.  0.0 to 50.0 %.      Only for color model.
            ADR_ALC_THROTTLE_F              = 0x0000D808,           // RW       Adjusting speed of ALC in %.    1.0 to 100 %.       Only for color model.
            ADR_ALC_LOCK_I                  = 0x0000D80C,           // RW       Lock state of ALC. See  g_saVAL_OnOff.      Only for color model.
            ADR_ALC_EXPOSURE_MIN_F          = 0x0000D810,           // RW       Minimum exposure value in AEC.      Only for Color model.
            ADR_ALC_EXPOSURE_MAX_F          = 0x0000D814,           // RW       Maximum exposure value in AEC.  up to 16000000 microseconds.    Only for Color model.
            ADR_ALC_GAIN_MIN_F              = 0x0000D818,           // RW       Minimum gain value in AGC.  minimum is 0.       Only for color model, 
            ADR_ALC_GAIN_MAX_F              = 0x0000D81C,           // RW       Maximum gain value in AGC.  minimum is 0. Maximum is 18.0   Only for color model.

            ADR_DEV_OVERHEATED_COUNT_I      = 0x00018000,           // RO       Count of Device overheated event.

            ADR_BLOCK_START_DELAY_I         = 0x0000e204,           // RW       Block Start delay.
            ADR_BLOCK_START_DELAY_MAX_I     = 0x0000e210,           // RW       Block Start delay maximum value.

        };
        typedef uint32_t REG_ADR_BG;


        //================================================================
        // Data length
        //================================================================
    #define LEN_STREAM_CHANNEL_REG          (0x00000040)            // Length of registers for a stream channel. 
    #define LEN_LINE_REG                    (0x00000010)            // Length of IO line registers(LineInverter and Line source registers) for a line.
    #define LEN_COLOR_CORRECTION_MATRIX_I   (0x10)                  // Length of ColorCorrectionMatrix register set for a color component. (mask_Xr, mask_Xg, and mask_Xb registers).
    #define LEN_COLOR_CORRECTION_MATRIX_J   (0x04)                  // Length of a ColorCorrectionMatrix register.



        //================================================================
        // Enumeration type register data  of BG series
        // 
        //  enum REGVAL_xxx         : Defines available register values as enum.
        //
        //  REGDATA g_sREGV_xxx     : Structure for defining name of REGVAL_xxx
        //
        //  REGDATA g_saVAL_xxx     : Defines Avaliable values and their name as Array of g_sREGV_xxx.
        //
        //
        //  g_saVAL_xxx is available for setting combo-Box items, and parsing selected value of combo-Box.
        //
        //
        //================================================================




        //================================================================
        // Structure for defining register Data
        //================================================================
        typedef struct _REGDATA
        {
            uint32_t    ValInt;             // Value in integer
            char        *Name;              // Value name

            // construuctor
            _REGDATA()
            {
                ValInt      = 0;
                Name        = "";
            };

            // construuctor
            _REGDATA(uint32_t value, char* name)
            {
                ValInt  = value;
                Name    = name;
            };
        } REGDATA;


        //================================================================
        // CCP register value
        //================================================================
        enum REGVAL_CCP
        {
            RV_CCP_OPEN_ACCESS  = 0x00000000,
            RV_CCP_EXCLUSIVE    = 0x80000000,
            RV_CCP_CONTROL      = 0x40000000,
            RV_CCP_CONTROL_SW   = 0x20000000,
        };

        const REGDATA g_sREGV_CCP_None      = REGDATA( RV_CCP_OPEN_ACCESS,  "OpenAccess");
        const REGDATA g_sREGV_CCP_Exclusive = REGDATA( RV_CCP_EXCLUSIVE,    "ExclusiveAccess");
        const REGDATA g_sREGV_CCP_Control   = REGDATA( RV_CCP_CONTROL,      "ControlAccess");
        const REGDATA g_sREGV_CCP_SwOver    = REGDATA( RV_CCP_CONTROL + RV_CCP_CONTROL_SW,  "ControlAccessSwitchoverActive");

        const REGDATA g_saVAL_CCP[] =
        {   
            g_sREGV_CCP_None,
            g_sREGV_CCP_Exclusive,
            g_sREGV_CCP_Control
        };

        const int c_iVAL_CCP_N = 3;



        //================================================================
        // PixelFormat register value
        //================================================================
        // Pixel format is delared in TeliCamPixFmt.h"
    
        const REGDATA g_sREGV_PixFmt_Mono8      = REGDATA(PXL_FMT_Mono8,        "Mono8");
        const REGDATA g_sREGV_PixFmt_Mono10     = REGDATA(PXL_FMT_Mono10,       "Mono10");
        const REGDATA g_sREGV_PixFmt_BayerGR8   = REGDATA(PXL_FMT_BayerGR8,     "BayerGR8");
        const REGDATA g_sREGV_PixFmt_BayerGR10  = REGDATA(PXL_FMT_BayerGR10,    "BayerGR10");
        const REGDATA g_sREGV_PixFmt_BayerGR12  = REGDATA(PXL_FMT_BayerGR12,    "BayerGR12");
        const REGDATA g_sREGV_PixFmt_BayerRG8   = REGDATA(PXL_FMT_BayerRG8,     "BayerRG8");
        const REGDATA g_sREGV_PixFmt_BayerRG10  = REGDATA(PXL_FMT_BayerRG10,    "BayerRG10");
        const REGDATA g_sREGV_PixFmt_BayerRG12  = REGDATA(PXL_FMT_BayerRG12,    "BayerRG12");
        const REGDATA g_sREGV_PixFmt_BayerGB8   = REGDATA(PXL_FMT_BayerGB8,     "BayerGB8");
        const REGDATA g_sREGV_PixFmt_BayerGB10  = REGDATA(PXL_FMT_BayerGB10,    "BayerGB10");
        const REGDATA g_sREGV_PixFmt_BayerGB12  = REGDATA(PXL_FMT_BayerGB12,    "BayerGB12");
        const REGDATA g_sREGV_PixFmt_BayerBG8   = REGDATA(PXL_FMT_BayerBG8,     "BayerBG8");
        const REGDATA g_sREGV_PixFmt_BayerBG10  = REGDATA(PXL_FMT_BayerBG10,    "BayerBG10");
        const REGDATA g_sREGV_PixFmt_BayerBG12  = REGDATA(PXL_FMT_BayerBG12,    "BayerBG12");
        const REGDATA g_sREGV_PixFmt_RGB8       = REGDATA(PXL_FMT_RGB8,         "RGB8");
    //  const REGDATA g_sREGV_PixFmt_RGB8       = REGDATA(PXL_FMT_RGB8,         "RGB8Packed");      // GD
        const REGDATA g_sREGV_PixFmt_BGR8       = REGDATA(PXL_FMT_BGR8,         "BGR8");
    //  const REGDATA g_sREGV_PixFmt_BGR8       = REGDATA(PXL_FMT_BGR8,         "BGR8Packed");      // GD
        const REGDATA g_sREGV_PixFmt_YUV411P    = REGDATA(PXL_FMT_YUV411_8,     "YUV411Packed");
        const REGDATA g_sREGV_PixFmt_YUV422P    = REGDATA(PXL_FMT_YUV422_8,     "YUV422Packed");

        const REGDATA g_saVAL_PixFmt[] =
        {   
            g_sREGV_PixFmt_Mono8,
            g_sREGV_PixFmt_Mono10,
            g_sREGV_PixFmt_BayerGR8,
            g_sREGV_PixFmt_BayerGR10,
            g_sREGV_PixFmt_BayerGR12,
            g_sREGV_PixFmt_BayerRG8,
            g_sREGV_PixFmt_BayerRG10,
            g_sREGV_PixFmt_BayerRG12,
            g_sREGV_PixFmt_BayerGB8,
            g_sREGV_PixFmt_BayerGB10,
            g_sREGV_PixFmt_BayerGB12,
            g_sREGV_PixFmt_BayerBG8,
            g_sREGV_PixFmt_BayerBG10,
            g_sREGV_PixFmt_BayerBG12,
            g_sREGV_PixFmt_RGB8,
    //      g_sREGV_PixFmt_RGB8,
            g_sREGV_PixFmt_BGR8,
    //      g_sREGV_PixFmt_BGR8,
            g_sREGV_PixFmt_YUV411P,
            g_sREGV_PixFmt_YUV422P,
        };

        const int c_iVAL_PixFmt_N = 18;


        //================================================================
        // Pixel size register for BG series.
        // Use this enum with Register direct access API or GenAPI Node access functions.
        // Never use this enum with "CameraControl" class method of TeliCamApi.
        //================================================================
        enum REGVAL_PIXSIZE_BG
        {
            RV_PIXSIZE_BG_BPP8  =  0,
            RV_PIXSIZE_BG_BPP10 =  1,
            RV_PIXSIZE_BG_BPP12 =  2,
            RV_PIXSIZE_BG_BPP16 =  3,
            RV_PIXSIZE_BG_BPP24 =  4,
        };

        const REGDATA g_sREGV_PixSizeBG_BPP8    = REGDATA(RV_PIXSIZE_BG_BPP8,   "Bpp8");
        const REGDATA g_sREGV_PixSizeBG_BPP10   = REGDATA(RV_PIXSIZE_BG_BPP10,  "Bpp10");
        const REGDATA g_sREGV_PixSizeBG_BPP12   = REGDATA(RV_PIXSIZE_BG_BPP12,  "Bpp12");
        const REGDATA g_sREGV_PixSizeBG_BPP16   = REGDATA(RV_PIXSIZE_BG_BPP16,  "Bpp16");
        const REGDATA g_sREGV_PixSizeBG_BPP24   = REGDATA(RV_PIXSIZE_BG_BPP24,  "Bpp24");

        const REGDATA g_saVAL_PixSizeBG[] =
        {   
            g_sREGV_PixSizeBG_BPP8,
            g_sREGV_PixSizeBG_BPP10,
            g_sREGV_PixSizeBG_BPP12,
            g_sREGV_PixSizeBG_BPP16,
            g_sREGV_PixSizeBG_BPP24,
        };

        const int c_iVAL_PixSizeBG_N = 5;
        
        
        //================================================================
        // Pixel Color Filter.
        //================================================================
        enum REGVAL_PIXCLRFLT
        {
            RV_PIXCLRFLT_NONE       =  0,
            RV_PIXCLRFLT_BAYER_RG   =  1,
            RV_PIXCLRFLT_BAYER_GB   =  2,
            RV_PIXCLRFLT_BAYER_GR   =  3,
            RV_PIXCLRFLT_BAYER_BG   =  4,
        };

        const REGDATA g_sREGV_PixClrFilter_None     = REGDATA(RV_PIXCLRFLT_NONE,     "None");
        const REGDATA g_sREGV_PixClrFilter_BBayerRG = REGDATA(RV_PIXCLRFLT_BAYER_RG, "BayerRG");
        const REGDATA g_sREGV_PixClrFilter_BBayerGB = REGDATA(RV_PIXCLRFLT_BAYER_GB, "BayerGB");
        const REGDATA g_sREGV_PixClrFilter_BBayerGR = REGDATA(RV_PIXCLRFLT_BAYER_GR, "BayerGR");
        const REGDATA g_sREGV_PixClrFilter_BBayerBG = REGDATA(RV_PIXCLRFLT_BAYER_BG, "BayerBG");

        const REGDATA g_saVAL_PixClrFlt[] =
        {   
            //g_sREGV_PixClrFilter_None,
            g_sREGV_PixClrFilter_BBayerRG,
            g_sREGV_PixClrFilter_BBayerGB,
            g_sREGV_PixClrFilter_BBayerGR,
            g_sREGV_PixClrFilter_BBayerBG,
        };

        const int c_iVAL_PixClrFlt_N = 4;
        
        
        //================================================================
        // Test Image register value BG
        // Use this enum with Register direct access API or GenAPI Node access functions.
        // Never use this enum with "CameraControl" class method of TeliCamApi.
        //================================================================
        enum REGVAL_TST_IMG
        {
            RV_TSTIMG_OFF           = 0x00000000,
            RV_TSTIMG_BLACK         = 0x00000001,
            RV_TSTIMG_WHITE         = 0x00000002,
            RV_TSTIMG_GREY_H_RAMP   = 0x00000003,
            RV_TSTIMG_BRIGHT_GREY   = 0x00000004,
            RV_TSTIMG_DARK_GREY     = 0x00000005,
        };

        const REGDATA g_sREGV_TstImg_Off        = REGDATA( RV_TSTIMG_OFF,           "Off");
        const REGDATA g_sREGV_TstImg_Black      = REGDATA( RV_TSTIMG_BLACK,         "Black");
        const REGDATA g_sREGV_TstImg_White      = REGDATA( RV_TSTIMG_WHITE,         "White");
        const REGDATA g_sREGV_TstImg_RAMP       = REGDATA( RV_TSTIMG_GREY_H_RAMP,   "GreyHorizontalRamp");
        const REGDATA g_sREGV_TstImg_Grey_B     = REGDATA( RV_TSTIMG_BRIGHT_GREY,   "BrightGrey");
        const REGDATA g_sREGV_TstImg_Grey_D     = REGDATA( RV_TSTIMG_DARK_GREY,     "DarkGrey");

        const REGDATA g_saVAL_TstImg[] =
        {   
            g_sREGV_TstImg_Off,
            g_sREGV_TstImg_Black,
            g_sREGV_TstImg_White,
            g_sREGV_TstImg_RAMP,
            g_sREGV_TstImg_Grey_B,
            g_sREGV_TstImg_Grey_D,
        };

        const int c_iVAL_TstImg = 6;



        //================================================================
        // Acquisition mode register value
        // Use this enum with Register direct access API or GenAPI Node access functions.
        // Never use this enum with "CameraControl" class method of TeliCamApi.
        //================================================================
        enum REGVAL_ACQUSITION_MODE
        {
            RV_ACQMD_SINGLE     = 0,
            RV_ACQMD_MULTI      = 1,
            RV_ACQMD_CONTINUOUS = 2,
        };
    
        const REGDATA g_sREGV_AcqMd_Single  = REGDATA( RV_ACQMD_SINGLE,     "SingleFrame");
        const REGDATA g_sREGV_AcqMd_Multi   = REGDATA( RV_ACQMD_MULTI,      "MultiFrame");
        const REGDATA g_sREGV_AcqMd_Cont    = REGDATA( RV_ACQMD_CONTINUOUS, "Continuous");

        const REGDATA g_saVAL_AcqMd[] =
        {   
            g_sREGV_AcqMd_Single,
            g_sREGV_AcqMd_Multi,
            g_sREGV_AcqMd_Cont,
        };

        const int c_iVAL_AcqMd_N = 3;



        //================================================================
        // On /Off register value
        //================================================================
        enum REGVAL_ONOFF
        {
            RV_ONOFF_OFF        = 0,
            RV_ONOFF_ON     = 1,
        };

        const REGDATA g_sREGV_Off   = REGDATA( RV_ONOFF_OFF,    "Off");
        const REGDATA g_sREGV_On    = REGDATA(RV_ONOFF_ON,      "On");

        const REGDATA g_saVAL_OnOff[] =
        {   
            g_sREGV_Off,
            g_sREGV_On,
        };

        const int VAL_OnOff_N = 2;


        //================================================================
        // Trigger source register value
        // Use this enum with Register direct access API or GenAPI Node access functions.
        // Never use this enum with "CameraControl" class method of TeliCamApi.
        //================================================================
        enum REGVAL_TRIGGER_SOURCE
        {
            RV_TRGSRC_SOFT  = 0,
            RV_TRGSRC_LINE0 = 1,
        };

        const REGDATA g_sREGV_TrgSrc_Soft   = REGDATA( RV_TRGSRC_SOFT,  "Software");
        const REGDATA g_sREGV_TrgSrc_Line0  = REGDATA( RV_TRGSRC_LINE0, "Line0");

        const REGDATA g_saVAL_TrgSrc[] =
        {   
            g_sREGV_TrgSrc_Soft,
            g_sREGV_TrgSrc_Line0
        };

        const int VAL_TrgSrc_N = 2;



        //================================================================
        // TriggerActivation register value
        //================================================================
        enum REGVAL_TRIGGER_ACTIVATION
        {
            RV_TRGACT_RISING_EDGE   = 0,
            RV_TRGACT_FALLING_EDGE  = 1,
            RV_TRGACT_ANY_EDGE      = 2,
            RV_TRGACT_LEVEL_HIGH    = 3,
            RV_TRGACT_LEVEL_LOW     = 4,
        };

        const REGDATA g_sREGV_TrgAct_RiseE  = REGDATA( RV_TRGACT_RISING_EDGE,   "RisingEdge");
        const REGDATA g_sREGV_TrgAct_FallE  = REGDATA( RV_TRGACT_FALLING_EDGE,  "FallingEdge");
        const REGDATA g_sREGV_TrgAct_AnyE   = REGDATA( RV_TRGACT_ANY_EDGE,      "AnyEdge");
        const REGDATA g_sREGV_TrgAct_LvlH   = REGDATA( RV_TRGACT_LEVEL_HIGH,    "LevelHigh");
        const REGDATA g_sREGV_TrgAct_LvlL   = REGDATA( RV_TRGACT_LEVEL_LOW,     "LevelLow");

        const REGDATA g_saVAL_TrgAct[] =
        {   
            g_sREGV_TrgAct_RiseE,
            g_sREGV_TrgAct_FallE,
            //g_sREGV_TrgAct_AnyE,
            //g_sREGV_TrgAct_LvlH,
            //g_sREGV_TrgAct_LvlL,
        };

        const int c_iVAL_TrgAct_N = 2;


        ////================================================================
        //// TriggerOverlap register value
        ////================================================================
        //enum REGVAL_TRIGGER_OVERLAP
        //{
        //  RV_TRGOVL_OFF           = 0,
        //  RV_TRGOVL_READOUT       = 1,
        //  RV_TRGOVL_PREV_FRAME    = 2,
        //};

        //const REGDATA g_sREGV_TrgOvl_Off  = REGDATA( RV_TRGOVL_OFF,           "Off");
        //const REGDATA g_sREGV_TrgOvl_ROut = REGDATA( RV_TRGOVL_READOUT,       "ReadOut");
        //const REGDATA g_sREGV_TrgOvl_PrevF    = REGDATA( RV_TRGOVL_PREV_FRAME,    "PreviousFrame");

        //const REGDATA g_saVAL_TrgOvl[] =
        //{ 
        //  g_sREGV_TrgOvl_Off,
        //  g_sREGV_TrgOvl_ROut,
        //  g_sREGV_TrgOvl_PrevF,
        //};

        //const int c_iVAL_TrgOvl_N = 3;


        //================================================================
        // Exposure mode register value
        //================================================================
        enum REGVAL_EXPOSURE_MODE
        {
            RV_EXPMD_OFF            = 0,
            RV_EXPMD_TIMED          = 1,
            RV_EXPMD_TRIGGERWIDTH   = 2,
            RV_EXPMD_TRIGGERCONTROL = 3,
        };

        const REGDATA g_sREGV_ExpMd_Off             = REGDATA( RV_EXPMD_OFF,            "Off");
        const REGDATA g_sREGV_ExpMd_Timed           = REGDATA( RV_EXPMD_TIMED,          "Timed");
        const REGDATA g_sREGV_ExpMd_TriggerWidth    = REGDATA( RV_EXPMD_TRIGGERWIDTH,   "TriggerWidth");
        const REGDATA g_sREGV_ExpMd_TriggerControl  = REGDATA( RV_EXPMD_TRIGGERCONTROL, "TriggerControl");

        const REGDATA g_saVAL_ExpMd[] =
        {   
        //  g_sREGV_ExpMd_Off,
            g_sREGV_ExpMd_Timed,
            g_sREGV_ExpMd_TriggerWidth,
        //  g_sREGV_ExpMd_TriggerControl
        };

        const int c_iVAL_ExpMd_N = 2;


        //================================================================
        // Automatic adjustment register value
        //================================================================
        enum REGVAL_AUTO
        {
            RV_AUTO_OFF         = 0,
            RV_AUTO_ONCE        = 1,
            RV_AUTO_CONTINUOUS  = 2,
        };

        const REGDATA g_sREGV_Auto_Off  = REGDATA( RV_AUTO_OFF,         "Off");
        const REGDATA g_sREGV_Auto_Once = REGDATA( RV_AUTO_ONCE,        "Once");
        const REGDATA g_sREGV_Auto_Cont = REGDATA( RV_AUTO_CONTINUOUS,  "Continuous");

        const REGDATA VAL_Auto[] =
        {   
            g_sREGV_Auto_Off,
            g_sREGV_Auto_Once,
            g_sREGV_Auto_Cont,
        };

        const int c_iVAL_Auto_N = 3;

        const REGDATA g_saVAL_AutoOnce[] =
        {   
            g_sREGV_Auto_Off,
            g_sREGV_Auto_Once,
        //  g_sREGV_Auto_Cont,
        };

        const int c_iVAL_AutoOnce_N = 2;


        const REGDATA g_saVAL_AutoCont[] =
        {   
            g_sREGV_Auto_Off,
        //  g_sREGV_Auto_Once,
            g_sREGV_Auto_Cont,
        };

        const int c_iVAL_AutoCont_N = 2;



        //================================================================
        // Line Inverter register value
        //================================================================
        enum REGVAL_LINE_INVERTER
        {
            RV_IOINV_ACTIVE_L       = 0,
            RV_IOINV_ACTIVE_H       = 1,
        };
        const REGDATA g_sREGV_IOInv_ActL    = REGDATA( RV_IOINV_ACTIVE_L,       "ActiveLow");
        const REGDATA g_sREGV_IOInv_ActH    = REGDATA( RV_IOINV_ACTIVE_H,       "ActiveHigh");

        const REGDATA g_saVAL_IOInv[] =
        {   
            g_sREGV_IOInv_ActL,
            g_sREGV_IOInv_ActH,
        };

        const int c_iVAL_IOInv_N = 2;


        //================================================================
        // Line sourcet register value
        //================================================================
        const REGDATA g_sREGV_LineSrc_Off                       = REGDATA(  0, "Off");
        const REGDATA g_sREGV_LineSrc_AcquisitionTriggerWait    = REGDATA(  1, "AcquisitionTriggerWait");
        const REGDATA g_sREGV_LineSrc_AcquisitionActive         = REGDATA(  2, "AcquisitionActive");
        const REGDATA g_sREGV_LineSrc_FrameTriggerWait          = REGDATA(  3, "FrameTriggerWait");
        const REGDATA g_sREGV_LineSrc_FrameActive               = REGDATA(  4, "FrameActive");
        const REGDATA g_sREGV_LineSrc_ExposureActive            = REGDATA(  5, "ExposureActive");
        const REGDATA g_sREGV_LineSrc_Stream0TransferActive     = REGDATA(  6, "Stream0TransferActive");
        const REGDATA g_sREGV_LineSrc_Stream1TransferActive     = REGDATA(  7, "Stream1TransferActive");
        const REGDATA g_sREGV_LineSrc_Stream0TransferPaused     = REGDATA(  9, "Stream0TransferPaused");
        const REGDATA g_sREGV_LineSrc_Stream1TransferPaused     = REGDATA( 10, "Stream1TransferPaused");
        const REGDATA g_sREGV_LineSrc_Stream0TransferStopping   = REGDATA( 12, "Stream0TransferStopping");
        const REGDATA g_sREGV_LineSrc_Stream1TransferStopping   = REGDATA( 13, "Stream1TransferStopping");
        const REGDATA g_sREGV_LineSrc_Stream0TransferStopped    = REGDATA( 15, "Stream0TransferStopped");
        const REGDATA g_sREGV_LineSrc_Stream1TransferStopped    = REGDATA( 16, "Stream1TransferStopped");
        const REGDATA g_sREGV_LineSrc_Stream0TransferOverflow   = REGDATA( 18, "Stream0TransferOverflow");
        const REGDATA g_sREGV_LineSrc_Stream1TransferOverflow   = REGDATA( 19, "Stream1TransferOverflow");
        const REGDATA g_sREGV_LineSrc_Counter0Active            = REGDATA( 21, "Counter0Active");
        const REGDATA g_sREGV_LineSrc_Counter1Active            = REGDATA( 22, "Counter1Active");
        const REGDATA g_sREGV_LineSrc_Counter2Active            = REGDATA( 23, "Counter2Active");
        const REGDATA g_sREGV_LineSrc_Timer0Active              = REGDATA( 25, "Timer0Active");
        const REGDATA g_sREGV_LineSrc_Timer1Active              = REGDATA( 26, "Timer1Active");
        const REGDATA g_sREGV_LineSrc_Timer2Active              = REGDATA( 27, "Timer2Active");
        const REGDATA g_sREGV_LineSrc_TUserOutput0              = REGDATA( 29, "UserOutput0");
        const REGDATA g_sREGV_LineSrc_TUserOutput1              = REGDATA( 30, "UserOutput1");
        const REGDATA g_sREGV_LineSrc_TUserOutput2              = REGDATA( 31, "UserOutput2");

        const REGDATA g_sREGV_LineSrc_VD                        = REGDATA( 32, "VD");
        const REGDATA g_sREGV_LineSrc_FrameTransfer             = REGDATA( 33, "FrameTransfer");

        const REGDATA g_saVAL_LineSrc[] =
        {   
            g_sREGV_LineSrc_Off,
            //g_sREGV_LineSrc_AcquisitionTriggerWait,
            //g_sREGV_LineSrc_AcquisitionActive,
            g_sREGV_LineSrc_FrameTriggerWait,
            g_sREGV_LineSrc_FrameActive,
            g_sREGV_LineSrc_ExposureActive,
            //g_sREGV_LineSrc_Stream0TransferActive,
            //g_sREGV_LineSrc_Stream1TransferActive,
            //g_sREGV_LineSrc_Stream0TransferPaused,
            //g_sREGV_LineSrc_Stream1TransferPaused,
            //g_sREGV_LineSrc_Stream0TransferStopping,
            //g_sREGV_LineSrc_Stream1TransferStopping,
            //g_sREGV_LineSrc_Stream0TransferStopped,
            //g_sREGV_LineSrc_Stream1TransferStopped,
            //g_sREGV_LineSrc_Stream0TransferOverflow,
            //g_sREGV_LineSrc_Stream1TransferOverflow,
            //g_sREGV_LineSrc_Counter0Active,
            //g_sREGV_LineSrc_Counter1Active,
            //g_sREGV_LineSrc_Counter2Active,
            g_sREGV_LineSrc_Timer0Active,
            //g_sREGV_LineSrc_Timer1Active,
            //g_sREGV_LineSrc_Timer2Active,
            //g_sREGV_LineSrc_TUserOutput0,
            //g_sREGV_LineSrc_TUserOutput1,
            //g_sREGV_LineSrc_TUserOutput2,
            g_sREGV_LineSrc_VD,
            g_sREGV_LineSrc_FrameTransfer,
        };

        const int c_iVAL_LineSrc_N = 7;



        //================================================================
        // Event selector register value
        //================================================================
        enum REGVAL_EVENT_SEL
        {
            RV_EVNT_FRM_TRG         =  7,
            RV_EVNT_ALC_LATEST      = 16,
            RV_EVNT_ALC_CONVERGED   = 17,
            RV_EVNT_DEV_OVERHEAT    = 18,
        };
        const REGDATA g_sREGV_Evnt_Frm_Trg          = REGDATA(RV_EVNT_FRM_TRG,              "FrameTrigger");
        const REGDATA g_sREGV_Evnt_ALC_Latest       = REGDATA(RV_EVNT_ALC_LATEST,           "ALCLatestInformation");
        const REGDATA g_sREGV_Evnt_ALC_Converged    = REGDATA(RV_EVNT_ALC_CONVERGED,        "ALCConverged");
        const REGDATA g_sREGV_Evnt_Dev_OverHeat     = REGDATA(RV_EVNT_DEV_OVERHEAT, "DeviceOverHeated");

        const REGDATA g_saVAL_EventSel[] =
        {   
            g_sREGV_Evnt_Frm_Trg,
            g_sREGV_Evnt_ALC_Latest,
            g_sREGV_Evnt_ALC_Converged,
            g_sREGV_Evnt_Dev_OverHeat,
        };

        const int c_iVAL_EventSel_N = 4;



        //================================================================
        // Timer trigger source register value
        //================================================================
        enum REGVAL_TMR_TRG_SRC
        {
            RV_TMR_TRGSRC_OFF       =  0,
            RV_TMR_TRGSRC_ACQ_TRG   =  1,
            RV_TMR_TRGSRC_ACQ_STR   =  2,
            RV_TMR_TRGSRC_ACQ_END   =  3,
            RV_TMR_TRGSRC_FRM_TRG   =  4,
            RV_TMR_TRGSRC_FRM_STR   =  5,
            RV_TMR_TRGSRC_FRM_END   =  6,
            RV_TMR_TRGSRC_LIN_STR   =  7,
            RV_TMR_TRGSRC_LIN_END   =  8,
            RV_TMR_TRGSRC_EXP_STR   =  9,
        };

        const REGDATA g_sREGV_TmrTrgSrc_Off         = REGDATA(RV_TMR_TRGSRC_OFF,        "Off");
        const REGDATA g_sREGV_TmrTrgSrc_Acq_Trg     = REGDATA(RV_TMR_TRGSRC_ACQ_TRG,    "AcquisitionTrigger");
        const REGDATA g_sREGV_TmrTrgSrc_Acq_Str     = REGDATA(RV_TMR_TRGSRC_ACQ_STR,    "AcquisitionStart");
        const REGDATA g_sREGV_TmrTrgSrc_Acq_End     = REGDATA(RV_TMR_TRGSRC_ACQ_END,    "AcquisitionEnd");
        const REGDATA g_sREGV_TmrTrgSrc_Frm_Trg     = REGDATA(RV_TMR_TRGSRC_FRM_TRG,    "FrameTrigger");
        const REGDATA g_sREGV_TmrTrgSrc_Frm_Str     = REGDATA(RV_TMR_TRGSRC_FRM_STR,    "FrameStart");
        const REGDATA g_sREGV_TmrTrgSrc_Frm_End     = REGDATA(RV_TMR_TRGSRC_FRM_END,    "FrameEnd");
        const REGDATA g_sREGV_TmrTrgSrc_Lin_Str     = REGDATA(RV_TMR_TRGSRC_LIN_STR,    "LineStart");
        const REGDATA g_sREGV_TmrTrgSrc_Lin_End     = REGDATA(RV_TMR_TRGSRC_LIN_END,    "LineEnd");
        const REGDATA g_sREGV_TmrTrgSrc_Exp_Str     = REGDATA(RV_TMR_TRGSRC_EXP_STR,    "ExposureStart");

        const REGDATA g_saVAL_TmrTrgSrc[] =
        {   
            g_sREGV_TmrTrgSrc_Off,
            //g_sREGV_TmrTrgSrc_Acq_Trg,
            //g_sREGV_TmrTrgSrc_Acq_Str,
            //g_sREGV_TmrTrgSrc_Acq_End,
            //g_sREGV_TmrTrgSrc_Frm_Trg,
            //g_sREGV_TmrTrgSrc_Frm_Str,
            //g_sREGV_TmrTrgSrc_Frm_End,
            //g_sREGV_TmrTrgSrc_Lin_Str,
            //g_sREGV_TmrTrgSrc_Lin_End,
            g_sREGV_TmrTrgSrc_Exp_Str,
        };

        const int c_iVAL_TmrTrgSrc_N = 2;




        //================================================================
        // Userset selector register value
        //================================================================
        enum REGVAL_USRSET_SELECT
        {
            RV_USRSET_SEL_DEFAULT   =  0,
            RV_USRSET_SEL_1         =  1,
            RV_USRSET_SEL_2         =  2,
            RV_USRSET_SEL_3         =  3,
            RV_USRSET_SEL_SAFE_MODE =  4,
        };

        const REGDATA g_sREGV_Usrset_Default    = REGDATA(RV_USRSET_SEL_DEFAULT,    "Default");
        const REGDATA g_sREGV_Usrset_1          = REGDATA(RV_USRSET_SEL_1,          "Userset1");
        const REGDATA g_sREGV_Usrset_2          = REGDATA(RV_USRSET_SEL_2,          "Userset2");
        const REGDATA g_sREGV_Usrset_3          = REGDATA(RV_USRSET_SEL_3,          "Userset3");
        const REGDATA g_sREGV_Usrset_SafeMode   = REGDATA(RV_USRSET_SEL_SAFE_MODE,  "SafeMode");

        const REGDATA g_saVAL_UsrsetSel[] =
        {   
            g_sREGV_Usrset_Default,
            g_sREGV_Usrset_1,
            g_sREGV_Usrset_2,
            g_sREGV_Usrset_3,
            g_sREGV_Usrset_SafeMode,
        };

        const int c_iVAL_UsrsetSel_N = 5;




        //================================================================
        // Automatic Luminance Control area size
        //================================================================
        enum REGVAL_ALC_AREA
        {
            RV_ALC_AREA_SIZE_FULL   =  0,
            RV_ALC_AREA_SIZE_MEDIUM =  1,
            RV_ALC_AREA_SIZE_SMALL  =  2,
        };

        const REGDATA g_sREGV_ALC_AreaFull      = REGDATA(RV_ALC_AREA_SIZE_FULL,    "Full");
        const REGDATA g_sREGV_ALC_AreaMedium    = REGDATA(RV_ALC_AREA_SIZE_MEDIUM,  "Medium");
        const REGDATA g_sREGV_ALC_AreaSmall     = REGDATA(RV_ALC_AREA_SIZE_SMALL,   "Small");

        const REGDATA g_saVAL_ALC_Area[] =
        {   
            g_sREGV_ALC_AreaFull,
            g_sREGV_ALC_AreaMedium,
            g_sREGV_ALC_AreaSmall,
        };

        const int c_iVAL_ALC_Area_N = 3;



        //================================================================
        // The other
        //================================================================
        enum REG_ENUM_VAL
        {

            RV_EXECUTE      = 1,

            RV_RESULT_NG    = 0,
            RV_RESULT_OK    = 1,
        };

    }   // end of namespace RegMapBGType1
}       // end of namespace Teli

#endif  // _REGISTERMAP_BG_TYPE1_H
