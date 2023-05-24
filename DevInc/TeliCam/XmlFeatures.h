///////////////////////////////////////////////////////////////////////////////
// file    : XmlFeatures.h
// brief   : List item name of XML file
// version : 1.0.2.1
// author  : (C)2014-2021 TOSHIBA TELI CORPORATION 
// date    : 2021/03/05


////////////////////////////////////////////////////////////////////////////////
//
// Sample code for TeliCamSDK
// 
// Header file for GenICam feature name (node name) declaration.  V1.0.0
// The following node names can be used in "Nd_GetNode()"function.
//
// CAM_API_STATUS uiSts = Nd_GetNode(hCam, Teli::XmlFeatures::XF_ID_DEV_VENDOR_NAME, &hNode);
//
// We recommend you not to insert "using namespace Teli::XmlFeatures;" in user application code
// for utilizing IntelliSence of Visual studio IDE.
// 
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _XMLFEATURES_H
#define _XMLFEATURES_H

namespace Teli {
    namespace XmlFeatures
    {
    
        // Device Information
        const char * const XF_ID_DEV_VENDOR_NAME        = "DeviceVendorName";
        const char * const XF_ID_DEV_MODEL_NAME         = "DeviceModelName";
        const char * const XF_ID_DEV_MAN_INFO           = "DeviceManufacturerInfo";
        const char * const XF_ID_DEV_VERSION            = "DeviceVersion";
        const char * const XF_ID_DEV_FWARE_VER          = "DeviceFirmwareVersion";
        const char * const XF_ID_DEV_ID                 = "DeviceID";
        const char * const XF_ID_DEV_USER_ID            = "DeviceUserID";
        const char * const XF_ID_DEV_SCANTYPE           = "DeviceScanType";
        const char * const XF_ID_DEV_MAX_THROUGHPUT     = "DeviceMaxThroughput";
        const char * const XF_ID_DEV_RESET              = "DeviceReset";
        const char * const XF_ID_DEV_REG_STRM_START     = "DeviceRegistersStreamingStart";
        const char * const XF_ID_DEV_REG_STRM_END       = "DeviceRegistersStreamingEnd";
        const char * const XF_ID_DEV_REG_CHECK          = "DeviceRegistersCheck";
        const char * const XF_ID_DEV_REG_VALID          = "DeviceRegistersValid";

        // Image Format Control
        const char * const XF_ID_IMGFMT_SELECTOR        = "ImageFormatSelector";
        const char * const XF_ID_IMGFMT_CONTROL         = "ImageFormatControl";
        const char * const XF_ID_SENSOR_WIDTH           = "SensorWidth";
        const char * const XF_ID_SENSOR_HEIGHT          = "SensorHeight";
        const char * const XF_ID_WIDTH_MAX              = "WidthMax";
        const char * const XF_ID_HEIGHT_MAX             = "HeightMax";
        const char * const XF_ID_WIDTH                  = "Width";
        const char * const XF_ID_HEIGHT                 = "Height";
        const char * const XF_ID_OFFSET_X               = "OffsetX";
        const char * const XF_ID_OFFSET_Y               = "OffsetY";
        const char * const XF_ID_LINE_PITCH             = "LinePitch";
        const char * const XF_ID_BINNING_HORIZONTAL     = "BinningHorizontal";
        const char * const XF_ID_BINNING_VERTICAL       = "BinningVertical";
        const char * const XF_ID_DECIMATION_HORIZONTAL  = "DecimationHorizontal";
        const char * const XF_ID_DECIMATION_VERTICAL    = "DecimationVertical";
        const char * const XF_ID_REVERSE_X              = "ReverseX";
        const char * const XF_ID_REVERSE_Y              = "ReverseY";
        const char * const XF_ID_PIXEL_CODING           = "PixelCoding";
        const char * const XF_ID_PIXEL_SIZE             = "PixelSize";
        const char * const XF_ID_PIXEL_COLORFILTER      = "PixelColorFilter";
        const char * const XF_ID_PIXEL_FORMAT           = "PixelFormat";
        const char * const XF_ID_PIXEL_DYN_RANGE_MIN    = "PixelDynamicRangeMin";
        const char * const XF_ID_PIXEL_DYN_RANGE_MAX    = "PixelDynamicRangMax";
        const char * const XF_ID_TEST_PATTERN           = "TestPattern";
        const char * const XF_ID_TESTIMAGE_SELECTOR     = "TestImageSelector";


        // Acquisition and Trigger Controls
        const char * const XF_ID_ACQ_CONTROL            = "AcquisitionControl";
        const char * const XF_ID_ACQ_MODE               = "AcquisitionMode";
        const char * const XF_ID_ACQ_START              = "AcquisitionStart";
        const char * const XF_ID_ACQ_STOP               = "AcquisitionStop";
        const char * const XF_ID_ACQ_ABORT              = "AcquisitionAbort";
        const char * const XF_ID_ACQ_ARM                = "AcquisitionArm";
        const char * const XF_ID_ACQ_FRAME_COUNT        = "AcquisitionFrameCount";
        const char * const XF_ID_ACQ_BURST_FRM_COUNT    = "AcquisitionBurstFrameCount";     // Appended V1.0.1.0.
        const char * const XF_ID_ACQ_FRAMERATE_CTRL     = "AcquisitionFrameRateControl";    //IIDC2
        const char * const XF_ID_ACQ_FRAMERATE          = "AcquisitionFrameRate";
        const char * const XF_ID_ACQ_FRAMERATE_ABS      = "AcquisitionFrameRateAbs";        //deprecated
        const char * const XF_ID_ACQ_FRAMERATE_RAW      = "AcquisitionFrameRateRaw";        //deprecated
        const char * const XF_ID_ACQ_FRAMEINTERVAL      = "AcquisitionFrameInterval";
        const char * const XF_ID_ACQ_STATUS_SELECTOR    = "AcquisitionStatusSelector";
        const char * const XF_ID_ACQ_STATUS             = "AcquisitionStatus";
        const char * const XF_ID_TRIGGER_SELECTOR       = "TriggerSelector";
        const char * const XF_ID_TRIGGER_MODE           = "TriggerMode";
        const char * const XF_ID_TRIGGER_SOFTWARE       = "TriggerSoftware";
        const char * const XF_ID_TRIGGER_SOURCE         = "TriggerSource";
        const char * const XF_ID_TRIGGER_ACTIVATION     = "TriggerActivation";
        //const char * const XF_ID_TRIGGER_OVERLAP      = "TriggerOverlap";                 // comment out V1.0.1.0
        const char * const XF_ID_TRIGGER_DELAY          = "TriggerDelay";
        const char * const XF_ID_TRIGGER_DELAY_ABS      = "TriggerDelayAbs";                //deprecated
        const char * const XF_ID_TRIGGER_DELAY_RAW      = "TriggerDelayRaw";                //deprecated
        const char * const XF_ID_TRIGGER_DIVIDER        = "TriggerDivider";
        const char * const XF_ID_TRIGGER_MULTIPLIER     = "TriggerMultiplier";
        const char * const XF_ID_TRIGGER_SEQUENCE       = "TriggerSequence";                //IIDC2
        const char * const XF_ID_TRIGGER_ADDITIONALPARAM= "TriggerAdditionalParameter";
        const char * const XF_ID_HIGH_FRAMERATE_MODE    = "HighFramerateMode";


        // LuminanceControl
        const char * const XF_ID_EXPOSURE_MODE          = "ExposureMode";
        const char * const XF_ID_EXPOSURE_TIME_CTRL     = "ExposureTimeControl";            //IDC2
        const char * const XF_ID_EXPOSURE_TIME          = "ExposureTime";
        const char * const XF_ID_EXPOSURE_TIME_ABS      = "ExposureTimeAbs";                //deprecated
        const char * const XF_ID_EXPOSURE_TIME_RAW      = "ExposureTimeRaw";                //deprecated
        const char * const XF_ID_EXPOSURE_AUTO          = "ExposureAuto";
        const char * const XF_ID_SHORT_EXPOSURE_MODE    = "ShortExposureMode";

        // Analog Controls
        const char * const XF_ID_ANALOG_CONTROL         = "AnalogControl";
        const char * const XF_ID_GAIN_SELECTOR          = "GainSelector";
        const char * const XF_ID_GAIN                   = "Gain";
        const char * const XF_ID_GAIN_ABS               = "GainAbs";                        //deprecated
        const char * const XF_ID_GAIN_RAW               = "GainRaw";                        //deprecated
        const char * const XF_ID_GAIN_AUTO              = "GainAuto";
        const char * const XF_ID_GAIN_AUTO_BALANCE      = "GainAutoBalance";
        const char * const XF_ID_BLACKLEVEL_SELECTOR    = "BlackLevelSelector";
        const char * const XF_ID_BLACKLEVEL             = "BlackLevel";
        const char * const XF_ID_BLACKLEVEL_ABS         = "BlackLevelAbs";                  //deprecated
        const char * const XF_ID_BLACKLEVEL_RAW         = "BlackLevelRaw";                  //deprecated
        const char * const XF_ID_BLACKLEVEL_AUTO        = "BlackLevelAuto";
        const char * const XF_ID_BLACKLEVEL_AUTO_BALANCE = "BlackLevelAutoBalance";
        const char * const XF_ID_WHITECLIP_SELECTOR =    "WhiteClipSelector";
        const char * const XF_ID_WHITECLIP              = "WhiteClip";
        const char * const XF_ID_WHITECLIP_ABS          = "WhiteClipAbs";                   //deprecated
        const char * const XF_ID_WHITECLIP_RAW          = "WhiteClipRaw";                   //deprecated
        const char * const XF_ID_BALANCERATIO_SELECTOR  = "BalanceRatioSelector";
        const char * const XF_ID_BALANCERATIO           = "BalanceRatio";
        const char * const XF_ID_BALANCERATIO_ABS       = "BalanceRatioAbs";                //deprecated
        const char * const XF_ID_BALANCERATIO_RAW       = "BalanceRatioRaw";                //deprecated
        const char * const XF_ID_BALANCEWHITE_AUTO      = "BalanceWhiteAuto";
        const char * const XF_ID_GAMMA                  = "Gamma";

        // ChromaControl

        // LUTControl
        const char * const XF_ID_LUT_SELECTOR           = "LUTSelector";
        const char * const XF_ID_LUT_ENABLE             = "LUTEnable";
        const char * const XF_ID_LUT_INDEX              = "LUTIndex";
        const char * const XF_ID_LUT_VALUE              = "LUTValue";
        const char * const XF_ID_LUT_VALUE_ALL          = "LUTValueAll";

        // DigitalIOControl
        const char * const XF_ID_DIO_CONTROL            = "DigitalIOControl";
        const char * const XF_ID_LINE_SELECTOR          = "LineSelector";
        const char * const XF_ID_LINE_MODE              = "LineModeAll";
        const char * const XF_ID_LINE_INVERTER_ALL      = "LineInverterAll";
        const char * const XF_ID_LINE_STATUS            = "LineStatus";
        const char * const XF_ID_LINE_STATUS_ALL        = "LineStatusAll";
        const char * const XF_ID_LINE_SOURCE            = "LineSource";
        //const char * const XF_ID_LINE_FORMAT          = "LineFormat";                     // comment out V1.0.1.0
        const char * const XF_ID_USER_OUTPUT_SELECTOR   = "UserOutputSelector";
        const char * const XF_ID_USER_OUTPUT_VALUE      = "UserOutputValue";
        const char * const XF_ID_USER_OUTPUT_VALUE_ALL  = "UserOutputValueAll";
        const char * const XF_ID_USER_OUTPUT_VALUE_ALL_MASK = "UserOutputValueAllMask";

        // Event Generation
        const char * const XF_ID_EVENT_SELECTOR         = "EventSelector";
        const char * const XF_ID_EVENT_NOTIFICATION     = "EventNotification";
        const char * const XF_ID_ACQUISITION_TRIGGER    = "AcquisitionTrigger";

        const char * const XF_ID_EVENTDT_FRM_TRIGGER    = "EventFrameTriggerData";
        const char * const XF_ID_EVENT_FRM_TRIGGER      = "EventFrameTrigger";
        const char * const XF_ID_EVENTTMS_FRM_TRIGGER   = "EventFrameTriggerTimestamp";
        const char * const XF_ID_EVENTINF_FRM_TRG_FRMID = "EventFrameTriggerFrameID";
        
        const char * const XF_ID_EVENTDT_EXPSRE_END     = "EventExposureEndData";
        const char * const XF_ID_EVENT_EXPSRE_END       = "EventExposureEnd";
        const char * const XF_ID_EVENTTMS_EXPSRE_END    = "EventExposureEndTimestamp";
        const char * const XF_ID_EVENTINF_EX_END_FRMID  = "EventExposureEndFrameID";

        const char * const XF_ID_EVENTDT_Error          = "EventErrorData";
        const char * const XF_ID_EVENT_Error            = "EventError";
        const char * const XF_ID_EVENTTMS_Error         = "EventErrorTimestamp";
        const char * const XF_ID_EVENTINF_Error_FRMID   = "EventErrorFrameID";
        const char * const XF_ID_EVENT_ERROR_CODE_      = "EventErrorCode";


        //GenICam
        const char * const XF_ID_DEVICE                 = "Device";

        // CounterAndTimerControl
        const char * const XF_ID_COUNTER_SELECTOR       = "CounterSelector";
        const char * const XF_ID_COUNTER_EVENT_SOURCE   = "CounterEventSource";
        const char * const XF_ID_COUNTER_RESET          = "CounterReset";
        const char * const XF_ID_COUNTER_VALUE          = "CounterValue";
        const char * const XF_ID_COUNTER_VALUE_AT_RESET = "CounterValueAtReset";
        const char * const XF_ID_COUNTER_DURATION       = "CounterDuration";
        const char * const XF_ID_COUNTER_STATUS         = "CounterStatus";
        const char * const XF_ID_COUNTER_TRIGGER_SOURCE = "CounterTriggerSource";
        const char * const XF_ID_COUNTER_TRIGGER_ACTIVATION = "CounterTriggerActivation";
        const char * const XF_ID_TIMER_SELECTOR         = "TimerSelector";
        const char * const XF_ID_TIMER_DURATION         = "TimerDuration";
        const char * const XF_ID_TIMER_DURATION_ABS     = "TimerDurationAbs";               //deprecated
        const char * const XF_ID_TIMER_DURATION_RAW     = "TimerDurationRaw";               //deprecated
        const char * const XF_ID_TIMER_DELAY            = "TimerDelay";
        const char * const XF_ID_TIMER_DELAY_ABS        = "TimerDelayAbs";                  //deprecated
        const char * const XF_ID_TIMER_DELAY_RAW        = "TimerDelayRaw";                  //deprecated
        const char * const XF_ID_TIMER_VALUE            = "Timervalue";
        const char * const XF_ID_TIMER_VALUE_ABS        = "TimervalueAbs";                  //deprecated
        const char * const XF_ID_TIMER_VAlUE_RAW        = "TimerValueRaw";                  //deprecated
        const char * const XF_ID_TIMER_STATUS           = "TimerStatus";
        const char * const XF_ID_TIMER_TRIGGER_SOURCE   = "TimerTriggerSource";
        const char * const XF_ID_TIMER_TRIGGER_ACTIVATION = "TimerTriggerActivation";

        // UserSetControl
        const char * const XF_ID_USERSET_SELECTOR       = "UserSetSelector";
        const char * const XF_ID_USERSET_SAVE           = "UserSetSave";
        const char * const XF_ID_USERSET_LOAD           = "UserSetLoad";

        // TLParamsLocked
        const char * const XF_ID_TLPARAMS_LOCKED        = "TLParamsLocked";

        // GigE Vision Transport Layer
        const char * const XF_ID_TRANSPORT_LAYER_CONTROL                        = "TransportLayerControl";
        const char * const XF_ID_PAYLOAD_SIZE                                   = "PayloadSize";
        const char * const XF_ID_GEV_VERSION_MAJOR                              = "GevVersionMajor";
        const char * const XF_ID_GEV_VERSION_MINOR                              = "GevVersionMinor";
        const char * const XF_ID_GEV_DEVICE_MODE_IS_BIGENDIAN                   = "GevDeviceModeIsBigEndian";
        const char * const XF_ID_GEV_DEVICE_MODE_CHARACTERSET                   = "GevDeviceModeCharacterSet";
        const char * const XF_ID_GEV_INTERFACE_SELECTOR                         = "GevInterfaceSelector";
        const char * const XF_ID_GEV_MAC_ADDRESS                                = "GevMACAddress";
        const char * const XF_ID_GEV_MAC_HIGH                                   = "GevMACAddressHigh";
        const char * const XF_ID_GEV_MACLOW                                     = "GevMACAddressLow";
        const char * const XF_ID_GEV_SUPPORTED_IP_CONFIGURATION_LLA             = "GevSupportedIPConfigurationLLA";
        const char * const XF_ID_GEV_SUPPORTED_IP_CONFIGURATION_DHCP            = "GevSupportedIPConfigurationDHCP";
        const char * const XF_ID_GEV_SUPPORTED_IP_CONFIGURATION_PERSISTENT_IP   = "GevSupportedIPConfigurationPersistentIP";
        const char * const XF_ID_GEV_CURRENT_IP_CONFIGURATION                   = "GevCurrentIPConfiguration";
        const char * const XF_ID_GEV_CURRENT_IP_ADDRESS                         = "GevCurrentIPAddress";
        const char * const XF_ID_GEV_CURRENT_SUBNET_MASK                        = "GevCurrentSubnetMask";
        const char * const XF_ID_GEV_CURRENT_DEFAULT_GATEWAY                    = "GevCurrentDefaultGateway";
        const char * const XF_ID_GEV_FIRST_URL                                  = "GevFirstURL";
        const char * const XF_ID_GEV_SECOND_URL                                 = "GevSecondURL";
        const char * const XF_ID_GEV_NUMBER_OF_INTERFACES                       = "GevNumberOfInterfaces";
        const char * const XF_ID_GEV_PERSISTENT_IP_ADDRESS                      = "GevPersistentIPAddress";
        const char * const XF_ID_GEV_PERSISTENT_SUBNET_MASK                     = "GevPersistentSubnetMask";
        const char * const XF_ID_GEV_PERSISTENT_DEFAULT_GATEWAY                 = "GevPersistentDefaultGateway";
        const char * const XF_ID_GEV_MESSAGE_CHANNEL_COUNT                      = "GevMessageChannelCount";
        const char * const XF_ID_GEV_STREAM_CHANNEL_COUNT                       = "GevStreamChannelCount";
        const char * const XF_ID_GEV_SUPPORTED_OPTIONAL_COMMANDS_EVENT_DATA     = "GevSupportedOptionalCommandsEVENTDATA";
        const char * const XF_ID_GEV_SUPPORTED_OPTIONAL_COMMANDS_EVENT          = "GevSupportedOptionalCommandsEVENT";
        const char * const XF_ID_GEV_SUPPORTED_OPTIONAL_COMMANDS_PACKET_RESEND  = "GevSupportedOptionalCommandsPACKETRESEND";
        const char * const XF_ID_GEV_SUPPORTED_OPTIONAL_COMMANDS_WRITEMEM       = "GevSupportedOptionalCommandsWRITEMEM";
        const char * const XF_ID_GEV_SUPPORTED_OPTIONAL_COMMANDS_CONCATENATION  = "GevSupportedOptionalCommandsConcatenation";
        const char * const XF_ID_GEV_HEARTBEAT_TIMEOUT                          = "GevHeartbeatTimeout";
        const char * const XF_ID_GEV_TIMESTAMP_TICK_FREQUENCY                   = "GevTimestampTickFrequency";
        const char * const XF_ID_GEV_TIMESTAMP_CONTROL_LATCH                    = "GevTimestampControlLatch";
        const char * const XF_ID_GEV_TIMESTAMP_CONTROL_RESET                    = "GevTimestampControlReset";
        const char * const XF_ID_GEV_TIMESTAMP_VALUE                            = "GevTimestampValue";
        const char * const XF_ID_GEV_CCP                                        = "GevCCP";
        const char * const XF_ID_GEV_MCP_HOST_PORT                              = "GevMCPHostPort";
        const char * const XF_ID_GEV_MCDA                                       = "GevMCDA";
        const char * const XF_ID_GEV_MCTT                                       = "GevMCTT";
        const char * const XF_ID_GEV_MCRC                                       = "GevMCRC";
        const char * const XF_ID_GEV_STREAM_CHANNEL_SELECTOR                    = "GevStreamChannelSelector";
        const char * const XF_ID_GEV_SCP_INTERFACE_INDEX                        = "GevSCPInterfaceIndex";
        const char * const XF_ID_GEV_SCP_HOST_PORT                              = "GevSCPHostPort";
        const char * const XF_ID_GEV_SCPS_FIRE_TEST_PACKET                      = "GevSCPSFireTestPacket";
        const char * const XF_ID_GEV_SCPS_DO_NOT_FRAGMENT                       = "GevSCPSDoNotFragment";
        const char * const XF_ID_GEV_SCPS_BIG_ENDIAN                            = "GevSCPSBigEndian";
        const char * const XF_ID_GEV_SCPS_PACKET_SIZE                           = "GevSCPSPacketSize";
        const char * const XF_ID_GEV_SCPD                                       = "GevSCPD";
        const char * const XF_ID_GEV_SCDA                                       = "GevSCDA";
        const char * const XF_ID_GEV_LINK_SPEED                                 = "GevLinkSpeed";

        // Chunk Data Streams
        const char * const XF_ID_CHUNK_MODE_ACTIVE          = "ChunkModeActive";
        const char * const XF_ID_CHUNK_SELECTOR             = "ChunkSelector";
        const char * const XF_ID_CHUNK_ENABLE               = "ChunkEnable";
        const char * const XF_ID_CHUNK_IMAGE                = "ChunkImage";
        const char * const XF_ID_CHUNK_OFFSET_X             = "ChunkOffsetX";
        const char * const XF_ID_CHUNK_OFFSET_Y             = "ChunkOffsetY";
        const char * const XF_ID_CHUNK_WIDTH                = "ChunkWidth";
        const char * const XF_ID_CHUNK_HEIGHT               = "ChunkHeight";
        const char * const XF_ID_CHUNK_PIXEL_FORMAT         = "ChunkPixelFormat";
        const char * const XF_ID_CHUNK_DYNAMICRANGE_MAX     = "ChunkDynamicRangeMax";
        const char * const XF_ID_CHUNK_DYNAMICRANGE_MIN     = "ChunkDynamicRangeMin";
        const char * const XF_ID_CHUNK_TIMESTAMP            = "ChunkTimestamp";
        const char * const XF_ID_CHUNK_LINESTATUS_ALL       = "ChunkLineStatusAll";
        const char * const XF_ID_CHUNK_COUNTER_SELECTOR     = "ChunkCounterSelector";
        const char * const XF_ID_CHUNK_COUNTER              = "ChunkCounter";
        const char * const XF_ID_CHUNK_TIMER_SELECTOR       = "ChunkTimerSelector";
        const char * const XF_ID_CHUNK_TIMER                = "ChunkTimer";

        // IIDC2
        const char * const XF_ID_IMAGEBUFFER_MODE           = "ImageBufferMode";
        const char * const XF_ID_IMAGEBUFFER_FRAMECOUNT_MIN = "ImageBufferFrameCountMin";
        const char * const XF_ID_IMAGEBUFFER_FRAMECOUNT_MAX = "ImageBufferFrameCountMax";
        const char * const XF_ID_IMAGEBUFFER_FRAMECOUNT     = "ImageBufferFrameCount";
        const char * const XF_ID_IMAGEBUFFER_READ           = "ImageBufferRead";

        //TeliUnique
        //TeliUnique IIDC2
        const char * const XF_ID_FRAME_SYNCHRONIZATION      = "FrameSynchronization";                           // Frame Synchronyzation            // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGRG                  = "MaskingRG";                                      // Masking parameter RG             // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGRB                  = "MaskingRB";                                      // Masking parameter RB             // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGGR                  = "MaskingGR";                                      // Masking parameter GR             // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGGB                  = "MaskingGB";                                      // Masking parameter GB             // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGBR                  = "MaskingBR";                                      // Masking parameter BR             // #Appended V1.0.1.0.
        const char * const XF_ID_MASKINGBG                  = "MaskingBG";                                      // Masking parameter BG             // #Appended V1.0.1.0.

        const char * const XF_ID_TRIG_SAMPLING_PERIOD       = "TriggerSamplingPeriod";                          // TriggerSamplingPeriod

        const char * const XF_ID_EVNT_FRAMETRIGGER_ERR      = "EventFrameTriggerError";                         // EventFrameTriggerError
        const char * const XF_ID_EVNTDT_FRAMETRIGGER_ERR    = "EventFrameTriggerErrorData";                     // EventFrameTriggerErrorData
        const char * const XF_ID_EVNTTMS_FRAMETRIGGER_ERR   = "EventFrameTriggerErrorTimestamp";                // EventFrameTriggerErrorTimestamp

        const char * const XF_ID_EVNT_FRAMETRIGGER_WAIT     = "EventFrameTriggerWait";                          // EventFrameTriggerWait
        const char * const XF_ID_EVNTDT_FRAMETRIGGER_WAIT   = "EventFrameTriggerWaitData";                      // EventFrameTriggerWaitData
        const char * const XF_ID_EVNTTMS_FRAMETRIGGER_WAIT  = "EventFrameTriggerWaitTimestamp";                 // EventFrameTriggerWaitTimestamp

        const char * const XF_ID_EVNT_ALC_LATEST_INFO       = "EventALCLatestInformation";                      // EventALCLatestInformation
        const char * const XF_ID_EVNTDT_ALC_LATEST_INFO     = "EventALCLatestInformationData";                  // EventALCLatestInformationData
        const char * const XF_ID_EVNTTMS_ALC_LATEST_INFO    = "EventALCLatestInformationTimestamp";             // EventALCLatestInformationTimestamp
        const char * const XF_ID_EVNTINF_ALC_LATEST_TTL_LUM = "EventALCLatestInformationTotalLuminance";        // EventALCLatestInformationTotalLuminance
        const char * const XF_ID_EVNTINF_ALC_LATEST_AVG_LUM = "EventALCLatestInformationAverageLuminance";      // EventALCLatestInformationAverageLuminance
        const char * const XF_ID_EVNTINF_ALC_LATEST_EXPSRE  = "EventALCLatestInformationExposureTime";          // EventALCLatestInformationExposureTime
        const char * const XF_ID_EVNTINF_ALC_LATEST_GAIN    = "EventALCLatestInformationGain";                  // EventALCLatestInformationGain

        const char * const XF_ID_EVNT_ALC_CONVERGED         = "EventALCConverged";                              // EventALCConverged
        const char * const XF_ID_EVNTDT_ALC_CONVERGED       = "EventALCConvergedData";                          // EventALCConvergedData
        const char * const XF_ID_EVNTTMS_ALC_CONVERGED      = "EventALCConvergedTimestamp";                     // EventALCConvergedTimestamp
        const char * const XF_ID_EVNTINF_ALC_CVG_TTL_LUM    = "EventALCConvergedLuminanceTotal";                // EventALCConvergedLuminanceTotal
        const char * const XF_ID_EVNTINF_ALC_CVG_AVG_LUM    = "EventALCConvergedLuminanceAverage";              // EventALCConvergedLuminanceAverage
        const char * const XF_ID_EVNTINF_ALC_CVG_EXPSRE     = "EventALCConvergedExposureTime";                  // EventALCConvergedExposureTime
        const char * const XF_ID_EVNTINF_ALC_CVG_GAIN       = "EventALCConvergedGain";                          // EventALCConvergedGain

        const char * const XF_ID_ALC_PHOTOMETRIC_AREA_SIZE  = "ALCPhotometricAreaSize";
        const char * const XF_ID_ALC_REFERNCE_LUM           = "ALCReferenceLuminance";
        const char * const XF_ID_ALC_EXPOSURE_VALUE         = "ALCExposureValue";
        const char * const XF_ID_ALC_TOLERANCE              = "ALCTolerance";
        const char * const XF_ID_ALC_THROTTLE               = "ALCThrottle";
        const char * const XF_ID_ALC_LOCK                   = "ALCLock";
        const char * const XF_ID_ALC_EXPOSURE_MIN           = "ALCExposureTimeMin";
        const char * const XF_ID_ALC_EXPOSURE_MAX           = "ALCExposureTimeMax";
        const char * const XF_ID_ALC_GAIN_MIN               = "ALCGainMin";
        const char * const XF_ID_ALC_GAIN_MAX               = "ALCGainMax";

        const char * const XF_ID_HUE                        = "Hue";

        const char * const XF_ID_SATURATION_SELECTOR        = "SaturationSelector";
        const char * const XF_ID_SATURATION                 = "Saturation";

        const char * const XF_ID_COLOR_CORRECT_MATRIX_I     = "ColorCorrectionMatrixSelectorI";
        const char * const XF_ID_COLOR_CORRECT_MATRIX_J     = "ColorCorrectionMatrixSelectorJ";
        const char * const XF_ID_COLOR_CORRECTION_MATRIX    = "ColorCorrectionMatrix";

        const char * const XF_ID_SHARPNESS                  = "Sharpness";

        const char * const XF_ID_BLOCK_START_DELAY          = "BlockStartDelay";

        const char * const XF_ID_EVNT_NTF_FRM_TRIGGER       = "EventNotificationOfFrameTrigger";                // EventNotificationOfFrameTrigger
        const char * const XF_ID_EVNT_NTF_FRM_TRIGGER_ERR   = "EventNotificationOfFrameTriggerError";           // EventNotificationOfFrameTriggerError
        const char * const XF_ID_EVNT_NTF_FRM_TRIGGER_WAIT  = "EventNotificationOfFrameTriggerWait";            // EventNotificationOfFrameTriggerWait
        const char * const XF_ID_EVNT_NTF_FRM_TRNSFR_START  = "EventNotificationOfFrameTransferStart";          // EventNotificationOfFrameTransferStart
        const char * const XF_ID_EVNT_NTF_FRM_TRNSFR_END    = "EventNotificationOfFrameTransferEnd";            // EventNotificationOfFrameTransferEnd
        const char * const XF_ID_EVNT_NTF_EXPOSURE_START    = "EventNotificationOfExposureStart";               // EventNotificationOfExposureStart
        const char * const XF_ID_EVNT_NTF_EXPOSURE_END      = "EventNotificationOfExposureEnd";                 // EventNotificationOfExposureEnd

        const char * const XF_ID_EVNTTMS_FRM_TRNSFR_START   = "EventFrameTransferStartTimestamp";               // EventFrameTransferStartTimestamp
        const char * const XF_ID_EVNTTMS_FRM_TRNSFR_END     = "EventFrameTransferEndTimestamp";                 // EventFrameTransferEndTimestamp
        const char * const XF_ID_EVNTTMS_EXPOSURE_START     = "EventFrameExposureStartTimestamp";               // EventFrameExposureStartTimestamp
        const char * const XF_ID_EVNTTMS_EXPOSURE_END       = "EventFrameExposureEndTimestamp";                 // EventFrameExposureEndTimestamp

        const char * const XF_ID_EVNTDT_DEV_OVERHEATED      = "EventDeviceOverheatedData";                      // EventDeviceOverheatedData
        const char * const XF_ID_EVNT_DEV_OVERHEATED        = "EventDeviceOverheated";                          // EventDeviceOverheated
        const char * const XF_ID_EVNTTMS_DEV_OVERHEATED     = "EventDeviceOverheatedTimestamp";                 // EventDeviceOverheatedTimestamp
        const char * const XF_ID_EVNTINF_DEV_OVRHEAT_COUNT  = "EventDeviceOverheatedCount";                     // EventDeviceOverheatedCount

        const char * const XF_ID_SEQ_SHUTTER_ENABLE         = "SequentialShutterEnable";
        const char * const XF_ID_SEQ_SHUTTER_TRMNT_AT_MIN   = "SequentialShutterTerminateAtMin";
        const char * const XF_ID_SEQ_SHUTTER_TRMNT_AT_MAX   = "SequentialShutterTerminateAtMax";
        const char * const XF_ID_SEQ_SHUTTER_TRMNT_AT       = "SequentialShutterTerminateAt";
        const char * const XF_ID_SEQ_SHUTTER_OFST_SEQ_TABLE = "SequentialShutterSequenceTableOffset";
        const char * const XF_ID_SEQ_SHUTTER_CB_NUM_SEQTBL  = "SequentialShutterSequenceTableCategoryBlockNumber";
        const char * const XF_ID_SEQ_SHUTTER_SEQTBL_EXP_MIN = "SequentialShutterSequenceTableExpandedMin";
        const char * const XF_ID_SEQ_SHUTTER_SEQTBL_EXP_MAX = "SequentialShutterSequenceTableExpandedMax";
        const char * const XF_ID_SEQ_SHUTTER_SHUTER_INDEX   = "SequentialShutterIndex";
        const char * const XF_ID_SEQ_SHUTTER_SHUTTER_ENTRY  = "SequentialShutterEntry";
        const char * const XF_ID_SEQ_SHUTTER_ADR_SHTR_ENTRY = "SequentialShutterEntryAddress";
        const char * const XF_ID_USERSET_DEFAULT_SELECTOR   = "UserSetDefaultSelector";             // Appended V1.0.1.0
        const char * const XF_ID_USERSET_DEFAULT            = "UserSetDefault";                     // Appended V1.0.1.0
        const char * const XF_ID_LED_INDICATOR_LUMINANCE    = "LEDIndicatorLuminance";              // Appended V1.0.1.0

        const char * const XF_ID_ANTI_GLITCH                = "AntiGlitch";                         // Appended V1.0.2.0
        const char * const XF_ID_ANTI_CHATTERING            = "AntiChattering";                     // Appended V1.0.2.0

        const char * const XF_ID_DPC_ENABLE                 = "DPCEnable";                          // Appended V1.0.2.0
        const char * const XF_ID_DPC_NUMBER                 = "DPCNumber";                          // Appended V1.0.2.0

        // GiantDragon items
        const char * const XF_ID_GD_EXPOSUREPRESET          = "ExposurePreset";
        const char * const XF_ID_GD_AEAREA                  = "AEArea";
        const char * const XF_ID_GD_AELOCK                  = "AELock";
        const char * const XF_ID_GD_EXPOSUREADJUST          = "ExposureAdjust";
        const char * const XF_ID_GD_GAMMATABLESELECTOR      = "GammaTableSelector";


    // Enumeration Entry
        // Control register
        const char * const ENUM_ENTRY_DEFAULT           = "Default";
        const char * const ENUM_ENTRY_MANUAL            = "Manual";
        const char * const ENUM_ENTRY_NO_SPECIFY        = "NoSpecify";

        // Pixel Format                                                 // #Appended V1.0.1.0.
        const char * const ENTRY_PIXFMT_MONO8           = "Mono8";
        const char * const ENTRY_PIXFMT_MONO10          = "Mono10";
        const char * const ENTRY_PIXFMT_MONO10P         = "Mono10p";
        const char * const ENTRY_PIXFMT_MONO12          = "Mono12";
        const char * const ENTRY_PIXFMT_MONO12P         = "Mono12p";
        const char * const ENTRY_PIXFMT_MONO14          = "Mono14";
        const char * const ENTRY_PIXFMT_MONO16          = "Mono16";
        const char * const ENTRY_PIXFMT_RGB8            = "RGB8";
        const char * const ENTRY_PIXFMT_RGB10           = "RGB10";
        const char * const ENTRY_PIXFMT_RGB12           = "RGB12";
        const char * const ENTRY_PIXFMT_RGB16           = "RGB16";
        const char * const ENTRY_PIXFMT_BGR8            = "BGR8";
        const char * const ENTRY_PIXFMT_BGR10           = "BGR10";
        const char * const ENTRY_PIXFMT_BGR12           = "BGR12";
        const char * const ENTRY_PIXFMT_BGR16           = "BGR16";
        const char * const ENTRY_PIXFMT_YUV411          = "YUV411_8";
        const char * const ENTRY_PIXFMT_YUV422          = "YUV422_8";
        const char * const ENTRY_PIXFMT_YUV444          = "YUV8";
        const char * const ENTRY_PIXFMT_YCBCR411        = "YCbCr411_8";
        const char * const ENTRY_PIXFMT_YCBCR422        = "YCbCr422_8";
        const char * const ENTRY_PIXFMT_YCBCR444        = "YCbCr8";
        const char * const ENTRY_PIXFMT_BAYER_BG8       = "BayerBG8";
        const char * const ENTRY_PIXFMT_BAYER_GB8       = "BayerGB8";
        const char * const ENTRY_PIXFMT_BAYER_GR8       = "BayerGR8";
        const char * const ENTRY_PIXFMT_BAYER_RG8       = "BayerRG8";
        const char * const ENTRY_PIXFMT_BAYER_BG10      = "BayerBG10";
        const char * const ENTRY_PIXFMT_BAYER_GB10      = "BayerGB10";
        const char * const ENTRY_PIXFMT_BAYER_GR10      = "BayerGR10";
        const char * const ENTRY_PIXFMT_BAYER_RG10      = "BayerRG10";
        const char * const ENTRY_PIXFMT_BAYER_BG12      = "BayerBG12";
        const char * const ENTRY_PIXFMT_BAYER_GB12      = "BayerGB12";
        const char * const ENTRY_PIXFMT_BAYER_GR12      = "BayerGR12";
        const char * const ENTRY_PIXFMT_BAYER_RG12      = "BayerRG12";
        const char * const ENTRY_PIXFMT_BAYER_BG16      = "BayerBG16";
        const char * const ENTRY_PIXFMT_BAYER_GB16      = "BayerGB16";
        const char * const ENTRY_PIXFMT_BAYER_GR16      = "BayerGR16";
        const char * const ENTRY_PIXFMT_BAYER_RG16      = "BayerRG16";

        //PixelCoding
        const char * const ENTRY_MONO                   = "Mono";
        const char * const ENTRY_MONOSIGNED             = "MonoSigned";
        const char * const ENTRY_MONOPACKED             = "MonoPacked";
        const char * const ENTRY_RGB                    = "RGB";
        const char * const ENTRY_RGBSIGNED              = "RGBSigned";
        const char * const ENTRY_RGBPACKED              = "RGBPacked";
        const char * const ENTRY_BGR                    = "BGR";
        const char * const ENTRY_BGRSIGNED              = "BGRSigned";
        const char * const ENTRY_BGRPACKED              = "BGRPacked";
        const char * const ENTRY_YUV411PACKED           = "YUV411Packed";
        const char * const ENTRY_YUV422PACKED           = "YUV422Packed";
        const char * const ENTRY_YUV444PACKED           = "YUV444Packed";
        const char * const ENTRY_BAYER_GR               = "BayerGR";
        const char * const ENTRY_BAYER_GR_PKD           = "BayerGRPacked";
        const char * const ENTRY_BAYER_RG               = "BayerRG";
        const char * const ENTRY_BAYER_RG_PKD           = "BayerRGPacked";
        const char * const ENTRY_BAYER_GB               = "BayerGB";
        const char * const ENTRY_BAYER_GB_PKD           = "BayerGBPacked";
        const char * const ENTRY_BAYER_BG               = "BayerBG";
        const char * const ENTRY_BAYER_BG_PKD           = "BayerBGPacked";  

        //PixelSize
        const char * const ENTRY_BPP8                   = "Bpp8";
        const char * const ENTRY_BPP10                  = "Bpp10";
        const char * const ENTRY_BPP12                  = "Bpp12";
        const char * const ENTRY_BPP16                  = "Bpp16";
        const char * const ENTRY_BPP24                  = "Bpp24";
        const char * const ENTRY_BPP30                  = "Bpp30";
        const char * const ENTRY_BPP36                  = "Bpp36";
        const char * const ENTRY_BPP48                  = "Bpp48";
        
        // Pixel Color Filter                                       // #Appended V1.0.1.0.
        const char * const ENTRY_PIXCLRFLT_NONE         = "None";
        const char * const ENTRY_PIXCLRFLT_BAYER_RG     = "BayerRG";
        const char * const ENTRY_PIXCLRFLT_BAYER_GB     = "BayerGB";
        const char * const ENTRY_PIXCLRFLT_BAYER_GR     = "BayerGR";
        const char * const ENTRY_PIXCLRFLT_BAYER_BG     = "BayerBG";
        
        // ImageFormatSelector                                      // #Appended V1.0.2.0.
        const char * const ENTRY_IMGFMT_FORMAT0         = "Format0";
        const char * const ENTRY_IMGFMT_FORMAT1         = "Format1";
        const char * const ENTRY_IMGFMT_FORMAT2         = "Format2";
        
        // Acquisition mode                                         // #Appended V1.0.1.0.
        const char * const ENTRY_ACQMODE_SINGLEFRAME    = "SingleFrame";
        const char * const ENTRY_ACQMODE_MULTIFRAME     = "MultiFrame";
        const char * const ENTRY_ACQMODE_CONTINUOUS     = "Continuous";
        
        
        // Trigger Selector                                         // #Appended V1.0.1.0.
        //const char * const ENTRY_TRGSEL_ACQUIS_START  = "AcquisitionStart";
        //const char * const ENTRY_TRGSEL_ACQUIS_END        = "AcquisitionEnd";
        //const char * const ENTRY_TRGSEL_ACQUIS_ACTIVE = "AcquisitionActive";
        const char * const ENTRY_TRGSEL_FRM_START       = "FrameStart";
        //const char * const ENTRY_TRGSEL_FRM_END           = "FrameEnd";
        //const char * const ENTRY_TRGSEL_FRM_ACTIVE        = "FrameActive";
        const char * const ENTRY_TRGSEL_FRMBURST_START  = "FrameBurstStart";
        //const char * const ENTRY_TRGSEL_FRMBURST_END  = "FrameBurstEnd";
        //const char * const ENTRY_TRGSEL_FRMBURST_ACTIVE   = "FrameBurstActive";
        //const char * const ENTRY_TRGSEL_EXPOSURE_START    = "ExposureStart";
        //const char * const ENTRY_TRGSEL_EXPOSURE_END  = "ExposureEnd";
        //const char * const ENTRY_TRGSEL_EXPOSURE_ACTIVE   = "ExposureActive";
        
        // Trigger Mode
        const char * const ENTRY_TRG_MODE_OFF           = "Off";
        const char * const ENTRY_TRG_MODE_ON            = "On";

        // Trigger source
        const char * const ENTRY_TRG_SRC_LINE0          = "Line0";
        const char * const ENTRY_TRG_SRC_LINE1          = "Line1";
        const char * const ENTRY_TRG_SRC_LINE2          = "Line2";
        const char * const ENTRY_TRG_SRC_SW             = "Software";
        const char * const ENTRY_TRG_SRC_SW_0           = "TriggerSoftware";

        // Trigger activation                                       // #Appended V1.0.1.0.
        const char * const ENTRY_TRG_ACTVT_RISINGEDGE   = "RisingEdge";
        const char * const ENTRY_TRG_ACTVT_FALLINGEDGE  = "FallingEdge";
        const char * const ENTRY_TRG_ACTVT_ANYEDGE      = "AnyEdge";
        const char * const ENTRY_TRG_ACTVT_LEVELHIGH    = "LevelHigh";
        const char * const ENTRY_TRG_ACTVT_LEVELLOW     = "LevelLow";
        
        // Trigger sequencee
        const char * const ENTRY_TRG_SEQ_0              = "TriggerSequence0";                   // External edge mode
        const char * const ENTRY_TRG_SEQ_1              = "TriggerSequence1";                   // External Level mode
        const char * const ENTRY_TRG_SEQ_6              = "TriggerSequence6";                   // Bulk Trigger mode

        //ExposureMode
        const char * const ENTRY_EXP_OFF                = "Off";
        const char * const ENTRY_EXP_TIMED              = "Timed";
        const char * const ENTRY_EXP_TRIGGER_WIDTH      = "TriggerWidth";
        const char * const ENTRY_EXP_TRIGGER_CONTROLLED = "TriggerControlled";

        //ExposureMode
        const char * const ENTRY_AUTO_OFF               = "Off";
        const char * const ENTRY_AUTO_ONCE              = "Once";
        const char * const ENTRY_AUTO_CONTINUOUS        = "Continuous";


        // Gain selector                                        // #Appended V1.0.1.0.
        //const char * const ENTRY_GAINSEL_ALL          = "All";
        //const char * const ENTRY_GAINSEL_RED          = "Red";
        //const char * const ENTRY_GAINSEL_GREEN            = "Green";
        //const char * const ENTRY_GAINSEL_BLUE         = "Blue";
        //const char * const ENTRY_GAINSEL_YUV          = "YUV";
        //const char * const ENTRY_GAINSEL_ANALOGALL        = "AnalogAll";
        //const char * const ENTRY_GAINSEL_ANALOGRED        = "AnalogRed";
        //const char * const ENTRY_GAINSEL_ANALOGREEN       = "AnalogGreen";
        //const char * const ENTRY_GAINSEL_ANALOGBLUE       = "AnalogBlue";
        const char * const ENTRY_GAINSEL_ANALOGY        = "AnalogY";
        //const char * const ENTRY_GAINSEL_ANALOGU      = "AnalogU";
        //const char * const ENTRY_GAINSEL_ANALOGV      = "AnalogV";
        //const char * const ENTRY_GAINSEL_DIGITALALL       = "DigitalAll";
        //const char * const ENTRY_GAINSEL_DIGITALRED       = "DigitalRed";
        //const char * const ENTRY_GAINSEL_DIGITALGREEN = "DigitalGreen";
        //const char * const ENTRY_GAINSEL_DIGITALBLUE  = "DigitalBlue";
        const char * const ENTRY_GAINSEL_DIGITALY       = "DigitalY";
        //const char * const ENTRY_GAINSEL_DIGITALU     = "DigitalU";
        //const char * const ENTRY_GAINSEL_DIGITALV     = "DigitalV";

        // Balance ratio selector                               // #Appended V1.0.1.0.
        const char * const ENTRY_BALANCESEL_R           = "Red";
        //const char * const ENTRY_BALANCESEL_G         = "Green";
        const char * const ENTRY_BALANCESEL_B           = "Blue";
        //const char * const ENTRY_BALANCESEL_Y         = "Y";
        //const char * const ENTRY_BALANCESEL_U         = "U";
        //const char * const ENTRY_BALANCESEL_V         = "V";


        // Line mode                                            // #Appended V1.0.1.0.
        const char * const ENTRY_LINEMODE_INPUT         = "Input";
        const char * const ENTRY_LINEMODE_OUTPUT        = "Output";

        // Line source                                          // #Appended V1.0.1.0.
        const char * const ENTRY_LINESRC_OFF            = "Off";
        //const char * const ENTRY_LINESRC_ACQ_TRG_WAIT = "AcquisitionTriggerWait";
        //const char * const ENTRY_LINESRC_ACQ_TRG_ACTIVE   = "AcquisitionActive";
        const char * const ENTRY_LINESRC_FRM_TRG_WAIT   = "FrameTriggerWait";
        const char * const ENTRY_LINESRC_FRM_ACTIVE     = "FrameActive";
        const char * const ENTRY_LINESRC_EXP_ACTIVE     = "ExposureActive";
        const char * const ENTRY_LINESRC_TRNSFR_ACTIVE  = "Stream0TransferActive";
        //const char * const ENTRY_LINESRC_TRNSFR_PAUSED    = "Stream0TransferPaused";
        //const char * const ENTRY_LINESRC_TRNS_STOPPING    = "Stream0TransferStopping";
        //const char * const ENTRY_LINESRC_TRNS_STOPPED = "Stream0TransferStopped";
        //const char * const ENTRY_LINESRC_TRNS_OVERFLOW    = "Stream0TransferOverflow";
        //const char * const ENTRY_LINESRC_CTR0_ACTIVE  = "Counter0Active";
        //const char * const ENTRY_LINESRC_TIMER0_ACTIVE    = "Timer0Active"
        const char * const ENTRY_LINESRC_FRM_TRNSFR     = "FrameTransfer";
        const char * const ENTRY_LINESRC_VD             = "VD";
        
        // Event selector                                       // #Appended V1.0.1.0.
        //const char * const ENTRY_EVENTSEL_ACQUIS_TRG  = "AcquisitionTrigger";
        //const char * const ENTRY_EVENTSEL_ACQUIS_START    = "AcquisitionStart";
        //const char * const ENTRY_EVENTSEL_ACQUIS_END  = "AcquisitionEnd";
        //const char * const ENTRY_EVENTSEL_ACQTRNS_START   = "AcquisitionTransferStart";
        //const char * const ENTRY_EVENTSEL_ACQTRNS_END = "AcquisitionTransferEnd";
        //const char * const ENTRY_EVENTSEL_ACQUIS_ERROR    = "AcquisitionError";
        const char * const ENTRY_EVENTSEL_FRAME_TRIGGER = "FrameTrigger";
        //const char * const ENTRY_EVENTSEL_FRAME_START = "FrameStart";
        //const char * const ENTRY_EVENTSEL_FRAME_END       = "FrameEnd";
        //const char * const ENTRY_EVENTSEL_FBURST_START    = "FrameBurstStart";
        //const char * const ENTRY_EVENTSEL_FBURST_END  = "FrameBurstEnd";
        //const char * const ENTRY_EVENTSEL_FRMTRNS_START   = "FrameTransferStart";
        //const char * const ENTRY_EVENTSEL_FRMTRNS_END = "FrameTransferEnd";
        //const char * const ENTRY_EVENTSEL_EXPO_START  = "ExposureStart";
        //const char * const ENTRY_EVENTSEL_EXPO_END        = "ExposureEnd";
        //const char * const ENTRY_EVENTSEL_SRMTRN_START    = "Stream0TransferStart";
        //const char * const ENTRY_EVENTSEL_SRMTRN_END  = "Stream0TransferEnd";
        //const char * const ENTRY_EVENTSEL_SRMTRN_PAUSE    = "Stream0TransferPause";
        //const char * const ENTRY_EVENTSEL_SRMTRN_RESUM    = "Stream0TransferResume";
        //const char * const ENTRY_EVENTSEL_SRMTRN_BLKSTR   = "Stream0TransferBlockStart";
        //const char * const ENTRY_EVENTSEL_SRMTRN_BLKEND   = "Stream0TransferBlockEnd";
        //const char * const ENTRY_EVENTSEL_SRMTRN_BLKTRG   = "Stream0TransferBlockTrigger";
        //const char * const ENTRY_EVENTSEL_SRMBRST_START   = "Stream0TransferBurstStart";
        //const char * const ENTRY_EVENTSEL_SRMBRST_END = "Stream0TransferBurstEnd";
        //const char * const ENTRY_EVENTSEL_SRMTRN_OVFL = "Stream0TransferOverflow;
        const char * const ENTRY_EVENTSEL_FRAME_TRG_ERR = "FrameTriggerError";
        const char * const ENTRY_EVENTSEL_FRAME_TRGWAIT = "FrameTriggerWait";
        const char * const ENTRY_EVENTSEL_ALC_LATESTINF = "ALCLatestInformation";
        const char * const ENTRY_EVENTSEL_ALC_CONVERGED = "ALCConverged";
        const char * const ENTRY_EVENTSEL_DEV_OVRHEATED = "DeviceOverheated";
            
        
        // Timer trigger source                                     // #Appended V1.0.1.0.
        const char * const ENTRY_TMRTRGSRC_OFF          = "Off";
        //const char * const ENTRY_TMRTRGSRC_ACQUIS_START   = "AcquisitionStart";
        //const char * const ENTRY_TMRTRGSRC_ACQUIS_END = "AcquisitionEnd";
        //const char * const ENTRY_TMRTRGSRC_FRM_START  = "FrameStart";
        //const char * const ENTRY_TMRTRGSRC_FRM_END        = "FrameEnd";
        //const char * const ENTRY_TMRTRGSRC_FBURST_START   = "FrameBurstStart";
        //const char * const ENTRY_TMRTRGSRC_FBURST_END = "FrameBurstEnd";
        const char * const ENTRY_TMRTRGSRC_EXPO_START   = "ExposureStart";
        //const char * const ENTRY_TMRTRGSRC_EXPO_END       = "ExposureEnd";
        
        // Frame Synchronyzation                                    // #Appended V1.0.1.0.
        const char * const ENTRY_FRMSYNCHRO_OFF         = "Off";
        const char * const ENTRY_FRMSYNCHRO_BUS         = "Bus";
        const char * const ENTRY_FRMSYNCHRO_IOLINE0     = "IOLine0";
        const char * const ENTRY_FRMSYNCHRO_IOLINE1     = "IOLine1";



    }   // end of namespace XmlFeatures
}       // end of namespace Teli

#endif  // _XMLFEATURES_H
