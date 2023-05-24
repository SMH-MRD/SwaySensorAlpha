///////////////////////////////////////////////////////////////////////////////
// file    : RegisterMap_BU.h
// brief   : Register map of BU series
// version : 1.0.3.1
// author  : (C)2017 TOSHIBA TELI CORPORATION 
// date    : 2017/11/8
// version : 1.0.2.1
// author  : (C)2015 TOSHIBA TELI CORPORATION 
// date    : 2015/04/7
// version : 1.0.2.0
// author  : (C)2015 TOSHIBA TELI CORPORATION 
// date    : 2015/02/16
// version : 1.0.1.0
// author  : (C)2014 TOSHIBA TELI CORPORATION 
// date    : 2014/10/31



/////////////////////////////////////////////////////////
//
// Register Map for BU series
//
// For
//  B/W:   BU030, BU031, BU080, BU130, BU205M, BU406M, BU238M
//  Color: BU030C, BU030CF, BU130C, BU130CF, BU406MC, BU406MC, BU238MCF
//
/////////////////////////////////////////////////////////

#ifndef _REGISTERMAP_BU_H
#define _REGISTERMAP_BU_H

namespace Teli {
    namespace RegMapBU {

        //================================================================
        // Register map of BU series
        //  
        // Register address symbol:  ADR_([GROUP]_)[register name]_[Data type]
        // [Datatype] definition
        //     B:                    byte data register                 (1 byte).
        //     S:                    Short data register                (2 bytes).
        //     I:                    Integer data register              (4 bytes).
        //     L:                    Long Integer data register         (8 bytes).
        //     F:                    Float data register                (4 bytes).
        //     E:                    Enumeration data register          (4 bytes).
        //     C:                    Command register                   (4 bytes). 
        //     C[N]:                 Character array register           (N bytes). 
        //     B[N]:                 byte data array register           (N bytes).
        //     ST[N]:                Register block(structure)          (N bytes). 
        //     INT32B;               IIDC2 Integer32 type featureCSR    (32 bytes)      
        //     RECT96B;              IIDC2 rectangle type featureCSR    (96 bytes)      
        //     ENUM32B;              IIDC2 enum type featureCSR         (32 bytes)      
        //     BULK20B;              IIDC2 bulk boolean type featureCSR (20 bytes)      
        //     AINT_STX;             IIDC2 Integer array type featureCSR
        //  
        // Address offset symbol:  OFS_([GROUP]_)[register name]_[Data type]
        // [Datatype] definition
        //     B:                    byte data register                 (1 byte).
        //     S:                    Short data register                (2 bytes).
        //     I:                    Integer data register              (4 bytes).
        //     L:                    Long Integer data register         (8 bytes).
        //     F:                    Float data register                (4 bytes).
        //     E:                    Enumeration data register          (4 bytes).
        //     C:                    Command register                   (4 bytes). 
        //     C[N]:                 Character array register           (N bytes). 
        //     B[N]:                 byte data array register           (N bytes).
        //     ST[N]:                Register block(structure)          (N bytes). 
        //     INT32B;               IIDC2 Integer32 type featureCSR    (32 bytes)      
        //     RECT96B;              IIDC2 rectangle type featureCSR    (96 bytes)      
        //     ENUM32B;              IIDC2 enum type featureCSR         (32 bytes)      
        //     BULK20N;              IIDC2 bulk boolean type featureCSR (20 bytes)      
        //     AINT_STX;             IIDC2 Integer array type featureCSR
        //  
        //------------------------------------------------------------------
        // Structure of USB 3 Vision Bootstrap Register map (Based on GenCP and USB3 Vision standard)  
        //
        // The following 5 register maps and a manifest table are defined in USB3 Vision standard. 
        //
        // ABRM is a register map common to all cameras suppoting GenCP standard,
        // The top address of ABRM is fixed to 0x00000000.
        //
        // The other register maps are defined i USB3 vision standard and IIDC2 register map standard.
        // The offset address of contents in each register maps are standardized except vender unique registers.
        // But, top address of register maps except ABRM are not standardized.
        // Camera vender can locate these register maps at any address.
        // 
        // Top address of bootstrap register maps can be obtained from ABRM register, SBRM register, or IIDC2 Entry.
        //
        // User application can access camera registers using these register maps.
        //
        //  ABRM            Provides registers common to all camera interface types(USB3, CameraLink, etc).
        //                  Top address is fixed to 0x00000000.
        //                  Address of SBRM and Manifest Entry are contained in this register map.
        //
        //  SBRM            Provides camera interface specific registers.
        //                  Address of SIRM, EIRM and IIDC2 Entry are contained in this register map.
        //
        //  SIRM            Provides stream interface (transport layer) registers.
        //
        //  EIRM            Provides event registers.
        //
        //  IIDC2 Entry     Provides offset to IIDC2 CategoryBlock registers.
        //                  This register map is oprtional. (BU series camera support IIDC" Entry)
        //
        //  Manifest Table  Provides Manifeset entries which provide file type, file size and location of camera description file (XML file).
        //  
        // Register layout inside register maps and location of ABRM is fixed, 
        // Location of register maps except ABRM can be acquired from ABRM or SBRM. 
        //
        //------------------------------------------------------------------
        // This header file provides address of ABRM registers (ADR_ABRM_...) and
        // offset for SBRM, SIRM, EIRM, Manifest Table, IIDC2 Entry, IIDC2 CategoryBlock registers (OFS_....)
        //
        //
        // #################################################################################################
        // # Address of each register should be calculated using top address of register map and           #
        // # offset address inside the register map, following procedure defined in USB3 vision standard.  #
        // ##################################################################################################
        //
        // This header file also provides address of current BU series SBRM, SIRM, EIRM,
        // Manifest Table, IIDC2 Entry, IIDC2 CategoryBlock registers (ADR_....). 
        // Location of register maps are common to current BU series.
        //
        // 
        // ###############################################################################################################
        // # Use standard address calculation if new USB3 Vision camera which has different registermao layout appears,  # 
        // ###############################################################################################################
        //
        //------------------------------------------------------------------
        // [ABRM]  Technology Agnostic Bootstrap Register Map (Top address = 0x00000000)   
        //                    ~        ~         ~        ~ 
        //   Provides registers common to all camera interface types.
        //     0x00000000 GenCP Version     
        //     .......... 
        //     0x000001D0 Manifest Table Address -> Get Manifest Table address.
        //     0x000001D8 SBRM Address           -> Get SBRM address.
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [Manifest Table]  Get address from ABRM register.    
        //   Provides Manifest Entries which has infromation of camera description file.
        //   BU series provides only one manifest entry.
        //     Base+0x0000 Entry count      
        //     Base+0x0008 Manifest Entry #0
        //    (Base+0x0048 Manifest Entry #1)
        //    (..........                   )
        //  
        //------------------------------------------------------------------
        // [Manifest Entry]  Exists in Manifest Table.    
        //   Provides information of camera description file.
        //     Base+0x0000 GenICam File Version   
        //     .......... 
        //     Base+0x0008 Camera description file register address 
        //     Base+0x0010 Camera description file size in byte
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [SBRM]  Technology Specific Bootstrap Register Map. Get address from ABRM register.   
        //                    ~        ~         ~        ~      
        //   Provides interface type specific registers.
        //     Base+0x0000 U3V Version     
        //     .......... 
        //     Base+0x0020 SIRM Address  -> Get SIRM address.
        //     Base+0x0028 SIRM Length
        //     Base+0x002C EIRM Address  -> Get EIRM address.
        //     Base+0x0034 EIRM Length 
        //     Base+0x0038 IIDC2 Address -> Get IIDC2 Entry table address.
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [SIRM]  Streaming Interface Register Map. Get address from SBRM register.   
        //         ~         ~         ~        ~      
        //   Provides streaming interface registers.
        //     Base+0x0000 Streaming Interface information     
        //     .......... 
        //     Base+0x0008 SI Required Payload Size
        //     .......... 
        //     Base+0x001C SI Payload Trnasfer Size
        //     Base+0x0020 SI Payload Trnasfer count
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [EIRM]  Event Interface Register Map. Get address from SBRM register.
        //         ~     ~         ~        ~      
        //   Provides Event interface registers.
        //     Base+0x0000 Control     
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [IIC2 Entry table]  Get address from SBRM register.   
        //   Provides Information of IIDC2 register map and offset of CategoryBlocks.
        //     Base+0x0000 keyword for IIDC2 Entry ("IIDC")     
        //     .......... 
        //     Base+0x0020 Offset for CategoryBlock  0 Device control
        //    (Base+0x0024 Offset for CategoryBlock  1 Transfer layer control) SIRM provides transfer layer registers in U3V camera. 
        //     Base+0x0028 Offset for CategoryBlock  2 Image format control
        //     Base+0x002C Offset for CategoryBlock  3 Acquisition control
        //     Base+0x0030 Offset for CategoryBlock  4 Luminance control
        //     Base+0x0034 Offset for CategoryBlock  5 Chroma control
        //     Base+0x0038 Offset for CategoryBlock  6 LUT control
        //     Base+0x003C Offset for CategoryBlock  7 Trigger control
        //     Base+0x003C Offset for CategoryBlock  8 UserSet control
        //     Base+0x003C Offset for CategoryBlock  9 Digital IO control
        //     Base+0x003C Offset for CategoryBlock 10 Counter and timer control
        //     .......... 
        //     Base+0x009C Offset for CategoryBlock 31 Vendorunique control
        //     .......... 
        //  
        //------------------------------------------------------------------
        // [CategoryBlock0]  Get Offset from IIDC2 Entry Table.   
        //   Provides FeatureCSR(Feature command and status register)s about camera device.
        //     Base+0x0000 Header     
        //     Base+0x0003 NumberCategoryBlock    
        //     Base+0x0020 Device reset FeatureCSR
        //     .......... 
        //  
        //  
        //------------------------------------------------------------------
        // [CategoryBlock2]  Get Offset from IIDC2 Entry Table.   
        //   Provides FeatureCSR(Feature command and status register)s about image format.
        //     Base+0x0000 Header     
        //     Base+0x0003 NumberCategoryBlock    
        //     Base+0x0020 Image format selector FeatureCSR
        //     .......... 
        //  
        // .......... 
        // .......... 
        //  
        //=================================================================



    // Top address of register maps for BU series released before June 2014.

        //********************************************************************************************************************
        // The following addresses are commonly used in BU seriess released before June 2014.
        // User can use these address without reading ABRM, SBRM, or IIDC2 Entry, if user uses BUseries mentioned before.
        //
        //********************************************************************************************************************

        #define MAN_ENTRY_BU    (0x10000000)
        #define SBRM_BU         (0x00010000)
        #define SIRM_BU         (0x00020000)
        #define EIRM_BU         (0x00030000)
        #define IIDC2E_BU       (0x00100000)

        #define ADR_CB00        (0x00200000)
        #define ADR_CB01        (0x00201000)
        #define ADR_CB02        (0x00202000)
        #define ADR_CB03        (0x00203000)
        #define ADR_CB04        (0x00204000)
        #define ADR_CB05        (0x00205000)
        #define ADR_CB06        (0x00206000)
        #define ADR_CB07        (0x00207000)
        #define ADR_CB08        (0x00208000)
        #define ADR_CB09        (0x00209000)
        #define ADR_CB10        (0x0020A000)
        #define ADR_CB31        (0x0021F000)
        #define ADR_CB32        (0x00220000)
        #define ADR_CB33        (0x002FFFBC)
        #define ADR_CB34        (0x003FFFBC)
        #define ADR_CB35        (0x00500000)



        enum _REG_ADR_BU
        {

            //=================================================================
            // ABRM     (Defined in USB3 Vision standard and GenICam CenCP standard)
            //=================================================================
            ADR_ABRM_GENCP_VERSION_I            = 0x00000000,   // RO   GenCP veersion.
            ADR_ABRM_MANUFACTURE_NAME_C64       = 0x00000004,   // RO   Manufacturer name.
            ADR_ABRM_MODEL_NAME_C64             = 0x00000044,   // RO   Model name.
            ADR_ABRM_FAMILY_NAME_C64            = 0x00000084,   // RO   Family name.
//          ADR_ABRM_DEVICE_VERSION_I           = 0x000000C4,   // RO   Device version.
            ADR_ABRM_DEVICE_VERSION_C64         = 0x000000C4,   // RO   Device version.             //Amended 2015/04/07
            ADR_ABRM_MANUFACTURER_INFO_C64      = 0x00000104,   // RO   Manufacturer Information.
            ADR_ABRM_SERIAL_NUMBER_C64          = 0x00000144,   // RO   Serial number.
            ADR_ABRM_USER_DEFIEND_NAME_C64      = 0x00000184,   // RW   User defined name.
            ADR_ABRM_DEVICE_CAPABILITY_L        = 0x000001C4,   // RO   Bit field describing the device's capability.
            ADR_ABRM_MAX_DEV_RESPONSE_TIME_I    = 0x000001CC,   // RO   Maximum divice response time in ms.
            ADR_ABRM_MANIFEST_TABLE_ADDRESS_L   = 0x000001D0,   // RO   Manifest Table address.
            ADR_ABRM_SBRM_ADDRESS_L             = 0x000001D8,   // RO   SBRM (technology Specific Bootstrap Register Map, address.
            ADR_ABRM_DEVICE_CONFIGURATION_L     = 0x000001E0,   // RW   Bit field describing device configuraton.
            ADR_ABRM_HEARTBEAT_TIMEOUT_I        = 0x000001E8,   // RW   Heartbeat timeout in ms. Not used in USB3 vision standard.
            ADR_ABRM_MESSAGE_CHANNEL_ID_I       = 0x000001EC,   // RW   Channel id used for the message channel.
            ADR_ABRM_TIMESTAMP_L                = 0x000001F0,   // RO   Current device time in ns.
            ADR_ABRM_TIMESTAMP_LATCH_C          = 0x000001F8,   // WO   Current device time is latched to timestamp register when lsb of this register is set.
            ADR_ABRM_TIMESTAMP_INCREMENT_L      = 0x000001FC,   // RO   Timestamp increment [ns /tick].
            ADR_ABRM_ACCESS_PRIVILEGE_I         = 0x00000204,   // RW   Access Privilege.  Not used in USB3 vision standard.
            ADR_ABRM_PROTOCOL_ENDIANESS_I       = 0x00000208,   // RO   Endianess of protocol fields and bootstrap register. [g_saVAL_Endian]. Only little-endian is supported by USB3 Vision standard.
            ADR_ABRM_IMPLEMENT_ENDIANESS_I      = 0x0000020C,   // RO   Endianess of device implementation registers. [g_saVAL_Endian].  Only little-endian is supported by USB3 Vision standard.


        //********************************************************************************************************************
        // IIDC2 Feature Control and Status register (FeatureCSR)has many information inside it.
        //
        // This header file basically provides offsets ( or address) of FeatureCSR top address  and main value register address, only, to avoid cmplication.
        //
        // Use the following offset data to get register address except main value register..
        //
        // For example minimum vale register address of exposure time FeatureCSR is as follows.
        //
        //   uiAdrExposureMin  = ADR_LMN_EXPOSURE_TIME_INT32B + ADR_LMN_EXPOSURE_TIME_INT32B;
        //                       (Top address of FeatureCSR)   (Offset of Minimum register from top of FeatureCSR)
        //
        //********************************************************************************************************************

            //=================================================================
            // Offset of each register in IIDC2 Feature Command and Status Register block
            // (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_FCSR_COM_CONTROL_INQ_S          = 0x00,         // Offset of 'ControlInq' member from top FeatureCSR block
            OFS_FCSR_COM_VALUE_TYPE_B           = 0x02,         // Offset of 'ValueType' member from top FeatureCSR block
            OFS_FCSR_COM_FEATURE_INQE_B         = 0x03,         // Offset of 'FeatureInq' member from top FeatureCSR block
            OFS_FCSR_COM_OFS_ADR_EXP_I          = 0x04,         // Offset of 'OffsetAddressExp' member from top of FeatureCSR block
            OFS_FCSR_COM_CONTROL_I              = 0x08,         // Offset of 'Control' member from top FeatureCSR block

            OFS_FCSR_INT32_MULT_I               = 0x0C,         // Offset of 'Mult' member from top of Integer32 block
            OFS_FCSR_INT32_DIV_I                = 0x10,         // Offset of 'Div' member from top of Integer32 block
            OFS_FCSR_INT32_MIN_I                = 0x14,         // Offset of 'Min' member from top of Integer32 block
            OFS_FCSR_INT32_MAX_I                = 0x18,         // Offset of 'Max' member from top of Integer32 block
            OFS_FCSR_INT32_RAW_VALUE_I          = 0x1C,         // Offset of 'Value' member from top of Integer32 block

            //OFS_FCSR_ENUM_LIST_OF_ELEMENT_B16 = 0x1C,         // Offset of 'List of Element' member from top of Enumeration block         #Amended V1.0.2.0
            OFS_FCSR_ENUM_LIST_OF_ELEMENT_B16   = 0x0C,         // Offset of 'List of Element' member from top of Enumeration block         #Amended V1.0.2.0
            OFS_FCSR_ENUM_VALUE_I               = 0x1C,         // Offset of 'Value' member from top of Enumeration block

            OFS_FCSR_BULK_BIT_WRITABLE_B4       = 0x0C,         // Offset of 'Bit_Writable' member from top of Bulk Boolean block
            OFS_FCSR_BULK_VALUE_B4              = 0x10,         // Offset of 'Value' member from top of Bulk Boolean block

            OFS_FCSR_RECT_MIN_OFFSET_X_I        = 0x0C,         // Offset of 'MinOffsetX' member from top of Rect32 block
            OFS_FCSR_RECT_INC_OFFSET_X_I        = 0x10,         // Offset of 'IncOffsetX' member from top of Rect32 block
            OFS_FCSR_RECT_MIN_WIDTH_I           = 0x14,         // Offset of 'MinWidth' member from top of Rect32 block
            OFS_FCSR_RECT_INC_WIDTH_I           = 0x18,         // Offset of 'IncWidth' member from top of Rect32 block
            OFS_FCSR_RECT_TOTAL_SIZE_X_I        = 0x1C,         // Offset of 'TotalSizeX' member from top of Rect32 block
            OFS_FCSR_RECT_MIN_OFFSET_Y_I        = 0x20,         // Offset of 'MinOffsetY' member from top of Rect32 block
            OFS_FCSR_RECT_INC_OFFSET_Y_I        = 0x24,         // Offset of 'IncOffsetY' member from top of Rect32 block
            OFS_FCSR_RECT_MIN_HEIGHT_I          = 0x28,         // Offset of 'MinHeight' member from top of Rect32 block
            OFS_FCSR_RECT_INC_HEIGHT_I          = 0x2C,         // Offset of 'IncHeight' member from top of Rect32 block
            OFS_FCSR_RECT_TOTAL_SIZE_Y_I        = 0x30,         // Offset of 'TotalSizeY' member from top of Rect32 block
            OFS_FCSR_RECT_OFFSET_X_I            = 0x34,         // Offset of 'OffsetX' member from top of Rect32 block
            OFS_FCSR_RECT_WIDTH_I               = 0x38,         // Offset of 'Width' member from top of Rect32 block
            OFS_FCSR_RECT_OFFSET_Y_I            = 0x3C,         // Offset of 'OffsetY' member from top of Rect32 block
            OFS_FCSR_RECT_HEIGHT_I              = 0x40,         // Offset of 'Height' member from top of Rect32 block

            //=================================================================
            // IIDC2 CategoryBlock common.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_CATCMN_HEADER_ST20              = 0x0000,       // RO   Header of category block. Size and index of CategoryBlock. 
            OFS_CATCMN_SIZE_CATEGORYBLOCK_B3    = 0x0000,       // RO   Size of CategoryBlock including header in quadlets. 
            OFS_CATCMN_CATEGORYBLOCK_NUMBER_B   = 0x0003,       // RO   Index of CategoryBlock.


    // Register address of BU series released before June 2014.

        //********************************************************************************************************************
        // The following addresses are commonly used in BU seriess released before June 2014.
        // User can use these address without reading ABRM, SBRM, or IIDC2 Entry, if user uses BUseries mentioned before.
        //
        //********************************************************************************************************************

            //=================================================================
            // Manifest table     (Defined in USB3 Vision standard and GenICam CenCP standard)
            //=================================================================
            ADR_MFSTBL_ENTRY_COUNT_L            = MAN_ENTRY_BU,             // 0x10000000 RO    Entry count         
            ADR_MFSTBL_ENTRY_0_ST64             = MAN_ENTRY_BU + 0x0008,    // 0x10000008 RO    Manifest Entry #0  
            ADR_MFSTBL_ENTRY_1_ST64             = MAN_ENTRY_BU + 0x0048,    // 0x10000048 RO    Manifest Entry #1  

            //=================================================================
            // Manifest Entry     (Defined in USB3 Vision standard and GenICam CenCP standard)
            //=================================================================
            ADR_MFSETRY_SUBMINOR_VER_S          = MAN_ENTRY_BU + 0x0008,    // 0x10000008 RO    GenICam file-subminor version.          
            ADR_MFSETRY_MINOR_VER_B             = MAN_ENTRY_BU + 0x000A,    // 0x1000000A RO    GenICam file-minor version. 
            ADR_MFSETRY_MAJOR_VER_B             = MAN_ENTRY_BU + 0x000B,    // 0x1000000B RO    GenICam file-major version.
            ADR_MFSETRY_FILE_TYPE_S             = MAN_ENTRY_BU + 0x000C,    // 0x1000000C RO    File type of camera description file.  [g_saVAL_GFType]
            ADR_MFSETRY_SCHEMA_MINOR_B          = MAN_ENTRY_BU + 0x000E,    // 0x1000000E RO    Minor version of camera description file schema.
            ADR_MFSETRY_SCHEMA_MAJOR_B          = MAN_ENTRY_BU + 0x000F,    // 0x1000000F RO    Major version of camera description file schema.
            ADR_MFSETRY_REGISTER_ADDR_L         = MAN_ENTRY_BU + 0x0010,    // 0x10000010 RO    Register address of camera description file.
            ADR_MFSETRY_FILE_SIZE_L             = MAN_ENTRY_BU + 0x0018,    // 0x10000018 RO    Size of camera description file in bytes.
            ADR_MFSETRY_SHA1_HASH_B20           = MAN_ENTRY_BU + 0x0020,    // 0x10000020 RO    SHA1 hash of camera description file.

            //=================================================================
            // SBRM     (Defined in USB3 Vision standard)
            //=================================================================
            ADR_SBRM_U3V_VERSION_I              = SBRM_BU + 0x0000,         // 0x010000 RO  Version of the USB3 Vision standard.
            ADR_SBRM_U3VCP_CAPABILITY_L         = SBRM_BU + 0x0004,         // 0x010004 RO  Bit field specifying the device's U3V features and capabilities.  [g_saVAL_U3VCPCapa]
            ADR_SBRM_U3VCP_CONFIG_L             = SBRM_BU + 0x000C,         // 0x01000C RW  Configuration register for additional features on the control channel. Current USB3 Vision standard has no additional features.
            ADR_SBRM_MAX_CMD_TRANSFER_LEN_I     = SBRM_BU + 0x0014,         // 0x010014 RO  Maximum command transfer length.
            ADR_SBRM_MAX_ACK_TRANSFER_LEN_I     = SBRM_BU + 0x0018,         // 0x010018 RO  Maximum acknowledge transfer length.
            ADR_SBRM_NUM_STREAM_CHANNELS_I      = SBRM_BU + 0x001C,         // 0x01001C RO  Number of stream channelsand the corresponding SIRM.
            ADR_SBRM_SIRM_ADDRESS_L             = SBRM_BU + 0x0020,         // 0x010020 RO  SIRM address.
            ADR_SBRM_SIRM_LENGTH_I              = SBRM_BU + 0x0028,         // 0x010028 RO  SIRM length.
            ADR_SBRM_EIRM_ADDRESS_L             = SBRM_BU + 0x002C,         // 0x01002C RO  EIRM address.
            ADR_SBRM_EIRM_LENGTH_I              = SBRM_BU + 0x0034,         // 0x010034 RO  EIRM length.
            ADR_SBRM_IIDC2_ADDRESS_L            = SBRM_BU + 0x0038,         // 0x010038 RO  IIDC2 Entry table address.
            ADR_SBRM_CURRENT_SPEED_I            = SBRM_BU + 0x0040,         // 0x010040 RO  Current speed of the USB link.  [g_saVAL_USB_Speed]

            //=================================================================
            // SIRM     (Defined in USB3 Vision standard)
            //=================================================================
            ADR_SIRM_SI_INFO_I                  = SIRM_BU + 0x0000,         // 0x020000 RO  Information about stream interface that device reports. Payload size alignment data is included in this member.  [g_saVAL_Align_Payload]
            ADR_SIRM_SI_CONTROL_C               = SIRM_BU + 0x0004,         // 0x020004 RW  Enable / disable streaming.  [g_saVAL_SIControl]
            ADR_SIRM_REQ_PAYLOAD_SIZE_L         = SIRM_BU + 0x0008,         // 0x020008 RO  Minimum required payload size that device reports.      #Amended V1.0.2.0
            //ADR_SIRM_REQ_LEADER_SIZE_I            = SIRM_BU + 0x0919,         // 0x020010 RO  Minimum required leader size that device reports.   #Amended V1.0.2.0
            ADR_SIRM_REQ_LEADER_SIZE_I          = SIRM_BU + 0x0010,         // 0x020010 RO  Minimum required leader size that device reports.
            ADR_SIRM_REQ_TRAILER_SIZE_I         = SIRM_BU + 0x0014,         // 0x020014 RO  Minimum required trailer size that device reports.
            ADR_SIRM_MAX_LEADER_SIZE_I          = SIRM_BU + 0x0018,         // 0x020018 RW  Maximum leader size.
            ADR_SIRM_PYLD_TRANSFER_SIZE_I       = SIRM_BU + 0x001C,         // 0x02001C RW  Expected size of a single payload trannsfer.
            ADR_SIRM_PYLD_TRANSFER_COUNT_I      = SIRM_BU + 0x0020,         // 0x020020 RW  Expected number of payload transfer.
            ADR_SIRM_PYLD_FINALTRANS1_SIZE_I    = SIRM_BU + 0x0024,         // 0x020024 RQ  Size of first final payload transfer.
            ADR_SIRM_PYLD_FINALTRANS2_SIZE_I    = SIRM_BU + 0x0028,         // 0x020028 RW  Size of second final payload transfer.
            ADR_SIRM_MAX_TRAILER_SIZE_I         = SIRM_BU + 0x002C,         // 0x02002C RW  Maximum trailer size.

            //=================================================================
            // EIRM     (Defined in USB3 Vision standard)
            //=================================================================
            ADR_EIRM_EI_CONTROL_C               = EIRM_BU + 0x0000,         // 0x030000 RW  Enable / disable event.  [g_saVAL_EIControl]
            ADR_EIRM_MAX_TRANSFER_LENGTH_I      = EIRM_BU + 0x0004,         // 0x030004 RO  Maximum event transfer length.
            ADR_EIRM_EVENT_TEST_CONTROL_C       = EIRM_BU + 0x0008,         // 0x030008 RW  Controls thre generation of test event. (lsb is TriggerEventTest bit)

            //=================================================================
            // IIDC2Entry table     (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_IIDC2_KEYWORD_C4                = IIDC2E_BU + 0x0000,       // 0x100000 RO  keyword for IIDC2 Entry (char[4] = "IIDC" = 0x49494443).
            ADR_IIDC2_VERSION_SUBMINOR_B        = IIDC2E_BU + 0x0004,       // 0x100004 RO  Sub-minor version of supported IIDC2 standard.
            ADR_IIDC2_VERSION_MINOR_B           = IIDC2E_BU + 0x0005,       // 0x100005 RO  Minor version of supported IIDC2 standard.
            ADR_IIDC2_VERSION_MAJOR_B           = IIDC2E_BU + 0x0006,       // 0x100006 RO  Major version of supported IIDC2 standard.
            ADR_IIDC2_NUM_CATEGORYBLOCK_S       = IIDC2E_BU + 0x0008,       // 0x100008 RO  Number of category blocks.
            ADR_IIDC2_OFS_MANIFEST_TABLE_I      = IIDC2E_BU + 0x0010,       // 0x100010 RO  Offset for xml manifest table.
            ADR_IIDC2_OFS_CATEGORYBLOCK_00_I    = IIDC2E_BU + 0x0020,       // 0x100020 RO  Offset for category block 0. Device Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_01_I    = IIDC2E_BU + 0x0024,       // 0x100024 RO  Offset for category block 1. Transfer Layer Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_02_I    = IIDC2E_BU + 0x0028,       // 0x100028 RO  Offset for category block 2. Image Format Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_03_I    = IIDC2E_BU + 0x002C,       // 0x10002C RO  Offset for category block 3. Acquisition Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_04_I    = IIDC2E_BU + 0x0030,       // 0x100030 RO  Offset for category block 4. Luminance Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_05_I    = IIDC2E_BU + 0x0034,       // 0x100034 RO  Offset for category block 5. Chroma Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_06_I    = IIDC2E_BU + 0x0038,       // 0x100038 RO  Offset for category block 6. LUT Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_07_I    = IIDC2E_BU + 0x003C,       // 0x10003C RO  Offset for category block 7. Trigger Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_08_I    = IIDC2E_BU + 0x0040,       // 0x100040 RO  Offset for category block 8. UserSet Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_09_I    = IIDC2E_BU + 0x0044,       // 0x100044 RO  Offset for category block 9. Digital IO Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_10_I    = IIDC2E_BU + 0x0048,       // 0x100048 RO  Offset for category block 10. Counter And Timer Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_31_I    = IIDC2E_BU + 0x009C,       // 0x10009C RO  Offset for category block 31. Vender Unique Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_32_I    = IIDC2E_BU + 0x00A0,       // 0x1000A0 RO  Offset for category block 32. Vender Unique Control.
            ADR_IIDC2_OFS_CATEGORYBLOCK_33_I    = IIDC2E_BU + 0x00A4,       // 0x1000A4 RO  Offset for category block 33. Vender Unique Control.

            //=================================================================
            // IIDC2 CategoryBlock 0. Device control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_DEVICE_RESET_ENUM32B            = ADR_CB00 + 0x0020,        // 0x200020 RW  Device Reset FeatureCSR.
            ADR_DEVICE_RESET_I                  = ADR_CB00 + 0x003C,        // 0x20003C RW  Device Reset Value.

            //=================================================================
            // IIDC2 CategoryBlock 2. Image Format control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_IMGFMT_IMGFMT_SEL_ENUM32B       = ADR_CB02 + 0x0020,        // 0x202020 RW  Image format selector FeatureCSR.
            ADR_IMGFMT_IMGFMT_SEL_I             = ADR_CB02 + 0x003C,        // 0x20203C RW  Image format selector value.  [g_saVAL_FormatSel]

            ADR_IMGFMT_APPLY_IMGFMT_ENUM32B     = ADR_CB02 + 0x0040,        // 0x202040 RW  Apply image format FeatureCSR.
            ADR_IMGFMT_APPLY_IMGFMT_I           = ADR_CB02 + 0x005C,        // 0x20205C RW  Apply image format value.  [g_saVAL_ApplyImgFmt]

            ADR_IMGFMT_IMAGE_SIZE_RECT96        = ADR_CB02 + 0x0060,        // 0x202060 RW  Image size FeatureCSR.
            ADR_IMGFMT_IMAGE_OFFSETX_MIN_I      = ADR_CB02 + 0x006C,        // 0x20206C RW  Image offset X minimum value.
            ADR_IMGFMT_IMAGE_OFFSETX_INC_I      = ADR_CB02 + 0x0070,        // 0x202070 RW  Image offset X increment value.
            ADR_IMGFMT_IMAGE_WIDTH_MIN_I        = ADR_CB02 + 0x0074,        // 0x202074 RW  Image width minimum value.
            ADR_IMGFMT_IMAGE_WIDTH_INC_I        = ADR_CB02 + 0x0078,        // 0x202078 RW  Image width increment value.
            ADR_IMGFMT_IMAGE_TOTAL_SIZE_X_I     = ADR_CB02 + 0x007C,        // 0x20207C RW  Image total size X value.
            ADR_IMGFMT_IMAGE_OFFSETY_MIN_I      = ADR_CB02 + 0x0080,        // 0x202080 RW  Image offset Y minimum value.
            ADR_IMGFMT_IMAGE_OFFSETY_INC_I      = ADR_CB02 + 0x0084,        // 0x202084 RW  Image offset Y increment value.
            ADR_IMGFMT_IMAGE_HEIGHT_MIN_I       = ADR_CB02 + 0x0088,        // 0x202088 RW  Image height minimum value.
            ADR_IMGFMT_IMAGE_HEIGHT_INC_I       = ADR_CB02 + 0x008C,        // 0x20208C RW  Image height increment value.
            ADR_IMGFMT_IMAGE_TOTAL_SIZE_Y_I     = ADR_CB02 + 0x0090,        // 0x202090 RW  Image tital size Y value.
            ADR_IMGFMT_IMAGE_OFFSETX_I          = ADR_CB02 + 0x0094,        // 0x202094 RW  Image offset X value.
            ADR_IMGFMT_IMAGE_WIDTH_I            = ADR_CB02 + 0x0098,        // 0x202098 RW  Image width value.
            ADR_IMGFMT_IMAGE_OFFSETY_I          = ADR_CB02 + 0x009C,        // 0x20209C RW  Image offset Y value.
            ADR_IMGFMT_IMAGE_HEIGHT_I           = ADR_CB02 + 0x00A0,        // 0x2020A0 RW  Image height value.

            ADR_IMGFMT_PIXEL_CODING_ENUM32B     = ADR_CB02 + 0x00C0,        // 0x2020C0 RW  Pixel coding FeatureCSR.
            ADR_IMGFMT_PIXEL_CODING_I           = ADR_CB02 + 0x00DC,        // 0x2020DC RW  Pixel coding value. [g_saVAL_PixelCoding]

            ADR_IMGFMT_PIXEL_SIZE_ENUM32B       = ADR_CB02 + 0x00E0,        // 0x2020E0 RW  Pixel size FeatureCSR.
            ADR_IMGFMT_PIXEL_SIZE_I             = ADR_CB02 + 0x00FC,        // 0x2020FC RW  Pixel size value. [g_saVAL_PixelSize]

            ADR_IMGFMT_PIXEL_ENDIAN_ENUM32B     = ADR_CB02 + 0x0100,        // 0x202100 RW  Pixel endian FeatureCSR.
            ADR_IMGFMT_PIXEL_ENDIAN_I           = ADR_CB02 + 0x011C,        // 0x20211C RW  Pixel endian value. [g_saVAL_PixelEndian]

            //=================================================================
            // IIDC2 CategoryBlock 3. Acquisition control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_ACQ_ACQUISITION_ENUM32B         = ADR_CB03 + 0x0020,        // 0x203020 RW  Acquisition command FeatureCSR. [g_saVAL_AcquisCmd]
            ADR_ACQ_ACQUISITION_I               = ADR_CB03 + 0x003C,        // 0x20303C RW  Acquisition command. [g_saVAL_AcquisCmd]

            ADR_ACQ_ACQUIS_FRM_COUNT_INT32B     = ADR_CB03 + 0x0040,        // 0x203040 RW  Acquisition frame count FeatureCSR. Mult and Div are not used.
            ADR_ACQ_ACQUIS_FRM_COUNT_I          = ADR_CB03 + 0x005C,        // 0x20305C RW  Acquisition frame count.

            ADR_ACQ_IMGBUF_MODE_ENUM32B         = ADR_CB03 + 0x0060,        // 0x203060 RW  Image buffer mode FeatureCSR. [g_saVAL_OnOff]    
            ADR_ACQ_IMGBUF_MODE_I               = ADR_CB03 + 0x007C,        // 0x20307C RW  Image buffer mode. [g_saVAL_OnOff] 

            ADR_ACQ_IMGBUF_FRM_COUNT_INT32B     = ADR_CB03 + 0x0080,        // 0x203080 RW  Image buffer frame count FeatureCSR. Mult and Div are not used. Min is always 0.
            ADR_ACQ_IMGBUF_FRM_COUNT_I          = ADR_CB03 + 0x009C,        // 0x20309C RW  Image buffer frame count.

            ADR_ACQ_FRAME_RATE_INT32B           = ADR_CB03 + 0x00A0,        // 0x2030A0 RW  Acquisition frame rate FeatureCSR.
            ADR_ACQ_FRAME_RATE_RAW_I            = ADR_CB03 + 0x00BC,        // 0x2030BC RW  Acquisition frame rate.
            ADR_ACQ_FRAME_RATE_CNTRL_I          = ADR_CB03 + 0x00A8,        // 0x2030A8 RW  Control register of Acquisition frame rate FeatureCSR.

            ADR_ACQ_FRAME_INTERVAL_INT32B       = ADR_CB03 + 0x00C0,        // 0x2030C0 RW  Acquisition frame interval FeatureCSR.
            ADR_ACQ_FRAME_INTERVAL_RAW_I        = ADR_CB03 + 0x00DC,        // 0x2030DC RW  Acquisition frame interval.

            //=================================================================
            // IIDC2 CategoryBlock 4. Luminance control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_LMN_EXPOSURE_TIME_INT32B        = ADR_CB04 + 0x0020,        // 0x204020 RW  Exposure time FeatureCSR in microseconds.
            ADR_LMN_EXPOSURE_TIME_RAW_I         = ADR_CB04 + 0x003C,        // 0x20403C RW  Exposure time in microseconds.
            ADR_LMN_EXPOSURE_TIME_CNTRL_I       = ADR_CB04 + 0x0028,        // 0x204028 RW  Control register of Exposure FeatureCSR.

            ADR_LMN_SHORT_EXPOSURE_MODE_INT32B  = ADR_CB04 + 0x0380,        // 0x204380 RW  Short exposure mode FeatureCSR.
            ADR_LMN_SHORT_EXPOSURE_MODE_RAW_I   = ADR_CB04 + 0x038C,        // 0x20438C RW  Short exposure mode.
            ADR_LMN_SHORT_EXPOSURE_MODE_CNTRL_I = ADR_CB04 + 0x039C,        // 0x20439C RW  Control register of Short exposure mode FeatureCSR.

            ADR_LMN_BLACK_LEVEL_INT32B          = ADR_CB04 + 0x0040,        // 0x204040 RW  Black level FeatureCSR.
            ADR_LMN_BLACK_LEVEL_RAW_I           = ADR_CB04 + 0x005C,        // 0x20405C RW  Black level.

            ADR_LMN_GAIN_INT32B                 = ADR_CB04 + 0x0060,        // 0x204060 RW  Gain FeatureCSR.
            ADR_LMN_GAIN_RAW_I                  = ADR_CB04 + 0x007C,        // 0x20407C RW  Gain.

            ADR_LMN_GAMMA_INT32B                = ADR_CB04 + 0x0080,        // 0x204080 RW  Gamma FeatureCSR.
            ADR_LMN_GAMMA_RAW_I                 = ADR_CB04 + 0x009C,        // 0x20409C RW  Gamma.

            ADR_LMN_SHARPNESS_INT32B            = ADR_CB04 + 0x00A0,        // 0x2040A0 RW  Sharpness FeatureCSR.
            ADR_LMN_SHARPNESS_RAW_I             = ADR_CB04 + 0x00BC,        // 0x2040BC RW  Sharpness.

            ADR_LMN_ALC_LEVEL_INT32B            = ADR_CB04 + 0x00C0,        // 0x2040C0 RW  ALC level FeatureCSR.
            ADR_LMN_ALC_LEVEL_RAW_I             = ADR_CB04 + 0x00DC,        // 0x2040DC RW  ALC level.

            //=================================================================
            // IIDC2 CategoryBlock 5.Chroma Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_CHROMA_HUE_INT32B               = ADR_CB05 + 0x0020,        // 0x205020 RW  Hue FeatureCSR.
            ADR_CHROMA_HUE_I                    = ADR_CB05 + 0x003C,        // 0x20503C RW  Hue.

            ADR_CHROMA_SATURATION_INT32B        = ADR_CB05 + 0x0040,        // 0x205040 RW  Saturation FeatureCSR.
            ADR_CHROMA_SATURATION_I             = ADR_CB05 + 0x005C,        // 0x20505C RW  Saturation.

            ADR_CHROMA_WH_BALANCE_R_INT32B      = ADR_CB05 + 0x0060,        // 0x205060 RW  WhiteBalanceR FeatureCSR.
            ADR_CHROMA_WH_BALANCE_R_I           = ADR_CB05 + 0x007C,        // 0x20507C RW  WhiteBalanceR.

            ADR_CHROMA_WH_BALANCE_B_INT32B      = ADR_CB05 + 0x0080,        // 0x205080 RW  WhiteBalanceB FeatureCSR.
            ADR_CHROMA_WH_BALANCE_B_I           = ADR_CB05 + 0x009C,        // 0x20509C RW  WhiteBalanceB.

            ADR_CHROMA_WH_BALANCE_U_INT32B      = ADR_CB05 + 0x00A0,        // 0x2050A0 RW  WhiteBalanceU FeatureCSR.
            ADR_CHROMA_WH_BALANCE_U_I           = ADR_CB05 + 0x00BC,        // 0x2050BC RW  WhiteBalanceU.

            ADR_CHROMA_WH_BALANCE_V_INT32B      = ADR_CB05 + 0x00C0,        // 0x2050C0 RW  WhiteBalanceV FeatureCSR.
            ADR_CHROMA_WH_BALANCE_V_I           = ADR_CB05 + 0x00DC,        // 0x2050DC RW  WhiteBalanceV.

            ADR_CHROMA_COLOR_TEMP_INT32B        = ADR_CB05 + 0x00E0,        // 0x2050E0 RW  ColorTemperture FeatureCSR.
            ADR_CHROMA_COLOR_TEMP_I             = ADR_CB05 + 0x00FC,        // 0x2050FC RW  ColorTemperture.

            //=================================================================
            // IIDC2 CategoryBlock 6.LUTControl    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_LUT_LUT_ENABLE_ENUM32B          = ADR_CB06 + 0x0020,        // 0x206020 RW  LUTEnable FeatureCSR. 
            ADR_LUT_LUT_ENABLE_I                = ADR_CB06 + 0x003C,        // 0x20603C RW  LUTEnable.  [g_saVAL_OnOff]

            ADR_LUT_BANK_SELECTOR_ENUM32B       = ADR_CB06 + 0x0040,        // 0x206040 RW  LUTBankSelector FeatureCSR. [g_saVAL_LUTBankSel]
            ADR_LUT_BANK_SELECTOR_I             = ADR_CB06 + 0x005C,        // 0x20605C RW  LUTBankSelector.  [g_saVAL_LUTBankSel]
    
            ADR_LUT_VALUE_ALL_ENUM32B           = ADR_CB06 + 0x0060,        // 0x206060 RW  LUTValueAll FeatureCSR. [g_saVAL_LUTValAll]
            ADR_LUT_VALUE_ALL_I                 = ADR_CB06 + 0x007C,        // 0x20607C RW  LUTValueAll. [g_saVAL_LUTValAll]

            //=================================================================
            // IIDC2 CategoryBlock 7.Trigger Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_TRG_TRG_MODE_ENUM32B            = ADR_CB07 + 0x0020,        // 0x207020 RW  Trigger mode FeatyreCSR. [g_saVAL_OnOff]
            ADR_TRG_TRG_MODE_I                  = ADR_CB07 + 0x003C,        // 0x20703C RW  Trigger mode. [g_saVAL_OnOff]

            ADR_TRG_TRG_SEQUENCE_ENUM32B        = ADR_CB07 + 0x0040,        // 0x207040 RW  Trigger sequence FeatureCSR. [g_saVAL_TrigSeq]
            ADR_TRG_TRG_SEQUENCE_I              = ADR_CB07 + 0x005C,        // 0x20705C RW  Trigger sequence. [g_saVAL_TrigSeq]

            ADR_TRG_TRG_SOURCE_ENUM32B          = ADR_CB07 + 0x0060,        // 0x207060 RW  Trigger source FeatureCSR. [g_saVal_TrigSrc]
            ADR_TRG_TRG_SOURCE_I                = ADR_CB07 + 0x007C,        // 0x20707C RW  Trigger source. [g_saVal_TrigSrc]

            ADR_TRG_ADDITIONAL_PARAM_INT32B     = ADR_CB07 + 0x0080,        // 0x207080 RW  Trigger Additional Parameter FeatureCSR.
            ADR_TRG_ADDITIONAL_PARAM_RAW_I      = ADR_CB07 + 0x009C,        // 0x20709C RW  Trigger Additional Parameter.

            ADR_TRG_TRIGGER_DELAY_INT32B        = ADR_CB07 + 0x00A0,        // 0x2070A0 RW  Trigger Delay FeatureCSR.
            ADR_TRG_TRIGGER_DELAY_RAW_I         = ADR_CB07 + 0x00BC,        // 0x2070BC RW  Trigger Delay.

            ADR_TRG_SOFT_TRG_ENUM32B            = ADR_CB07 + 0x00C0,        // 0x2070C0 RW  SoftwareTrigger FeatureCSR. [g_saVAL_SoftTrig]
            ADR_TRG_SOFT_TRG_I                  = ADR_CB07 + 0x00DC,        // 0x2070DC RW  SoftwareTrigger. [g_saVAL_SoftTrig]

            //=================================================================
            // IIDC2 CategoryBlock 8.Userset Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_USET_STD_FORMAT_ENUM32B         = ADR_CB08 + 0x0020,        // 0x208020 RW  Standard format FeatureCSR. [g_saVAL_StdFmt]
            ADR_USET_STD_FORMAT_I               = ADR_CB08 + 0x003C,        // 0x20803C RW  Standard format. [g_saVAL_StdFmt]

            ADR_USET_STD_FRAME_RATE_ENUM32B     = ADR_CB08 + 0x0040,        // 0x208040 RW  Standard frame rate FeatureCSR. [g_saVAL_StdFrmRate]
            ADR_USET_STD_FRAME_RATE_I           = ADR_CB08 + 0x005C,        // 0x20805C RW  Standard frame rate. [g_saVAL_StdFrmRate]

            ADR_USET_USERSET_SEL_ENUM32B        = ADR_CB08 + 0x0060,        // 0x208060 RW  User set selector FeatureCSR. [g_saVAL_UsersetSel]
            ADR_USET_USERSET_SEL_I              = ADR_CB08 + 0x007C,        // 0x20807C RW  User set selector. [g_saVAL_UsersetSel]

            ADR_USET_USERSET_CMD_ENUM32B        = ADR_CB08 + 0x0080,        // 0x208080 RW  User set command FeatureCSR. [g_saVAL_UsersetCmd]
            ADR_USET_USERSET_CMD_I              = ADR_CB08 + 0x009C,        // 0x20809C RW  User set command. [g_saVAL_UsersetCmd]

            //=================================================================
            // IIDC2 CategoryBlock 9.Digital IO Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_DIO_IOLINE_MODE_ALL_BULK20B     = ADR_CB09 + 0x0020,        // 0x209020 RW  IO line mode all FeatureCSR.
            ADR_DIO_IOLINE_MODE_ALL_I           = ADR_CB09 + 0x0030,        // 0x209030 RW  IO line mode all.

            ADR_DIO_IO_INVERTER_ALL_BULK20B     = ADR_CB09 + 0x0040,        // 0x209040 RW  IO line inverter all FeytureCSR.
            ADR_DIO_IO_INVERTER_ALL_I           = ADR_CB09 + 0x0050,        // 0x209050 RW  IO line inverter all.

            ADR_DIO_IO_STATUS_ALL_BULK20B       = ADR_CB09 + 0x0060,        // 0x209060 RW  IO line status all FeytureCSR.
            ADR_DIO_IO_STATUS_ALL_I             = ADR_CB09 + 0x0070,        // 0x209070 RW  IO line status all.

            ADR_DIO_IO_OUT_VAL_ALL_BULK20B      = ADR_CB09 + 0x0080,        // 0x209080 RW  IO line output value all FeytureCSR.
            ADR_DIO_IO_OUT_VAL_ALL_I            = ADR_CB09 + 0x0090,        // 0x209090 RW  IO line output value all.

            ADR_DIO_IO_LINE_SEL_ENUM32B         = ADR_CB09 + 0x00A0,        // 0x2090A0 RW  IO line selector FeytureCSR. [g_saVAL_IOLineSel]
            ADR_DIO_IO_LINE_SEL_I               = ADR_CB09 + 0x00BC,        // 0x2090BC RW  IO line selector. [g_saVAL_IOLineSel]

            ADR_DIO_IO_LINE_SOURCEL_ENUM32B     = ADR_CB09 + 0x00C0,        // 0x2090C0 RW  IO line source FeytureCSR. [g_saVAL_IOLineSource]
            ADR_DIO_IO_LINE_SOURCE_I            = ADR_CB09 + 0x00DC,        // 0x2090DC RW  IO line source. [g_saVAL_IOLineSource]

            //=================================================================
            // IIDC2 CategoryBlock 10.Counter and timer Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            ADR_CNTTM_TIMER_SEL_ENUM32B         = ADR_CB10 + 0x0020,        // 0x20A020 RW  Timer selector FeytureCSR. [g_saVAL_TimerSel]
            ADR_CNTTM_TIMER_SEL_I               = ADR_CB10 + 0x003C,        // 0x20A03C RW  Timer selector. [g_saVAL_TimerSel]

            ADR_CNTTM_TIMER_DELAY_INT32B        = ADR_CB10 + 0x0040,        // 0x20A040 RW  Timer delay FeatureCSR.
            ADR_CNTTM_TIMER_DELAY_RAW_I         = ADR_CB10 + 0x005C,        // 0x20A05C RW  Timer delay.

            ADR_CNTTM_TIMER_DURATION_INT32B     = ADR_CB10 + 0x0060,        // 0x20A060 RW  Timer duration FeatureCSR.
            ADR_CNTTM_TIMER_DURATION_RAW_I      = ADR_CB10 + 0x007C,        // 0x20A07C RW  Timer duration.

            //=================================================================
            // IIDC2 CategoryBlock 31.Vendor unique Control    (Defined by Teli)
            //=================================================================
            ADR_VENDOR_FRAME_SYNC_ENUM32B       = ADR_CB31 + 0x0020,        // 0x21F020 RW  Frame synchronization FeatureCSR, [g_saVAL_FrmSync]
            ADR_VENDOR_FRAME_SYNC_I             = ADR_CB31 + 0x003C,        // 0x21F03C RW  Frame synchronization, [g_saVAL_FrmSync]

            ADR_VENDOR_MASKING_RG_INT32B        = ADR_CB31 + 0x0040,        // 0x21F040 RW  Masking RG FeatureCSR.
            ADR_VENDOR_MASKING_RG_RAW_I         = ADR_CB31 + 0x005C,        // 0x21F05C RW  Masking RG.

            ADR_VENDOR_MASKING_RB_INT32B        = ADR_CB31 + 0x0060,        // 0x21F060 RW  Masking RB FeatureCSR.
            ADR_VENDOR_MASKING_RB_RAW_I         = ADR_CB31 + 0x007C,        // 0x21F07C RW  Masking RB.

            ADR_VENDOR_MASKING_GR_INT32B        = ADR_CB31 + 0x0080,        // 0x21F080 RW  Masking GR FeatureCSR.
            ADR_VENDOR_MASKING_GR_RAW_I         = ADR_CB31 + 0x009C,        // 0x21F09C RW  Masking GR.

            ADR_VENDOR_MASKING_GB_INT32B        = ADR_CB31 + 0x00A0,        // 0x21F0A0 RW  Masking GB FeatureCSR.
            ADR_VENDOR_MASKING_GB_RAW_I         = ADR_CB31 + 0x00BC,        // 0x21F0BC RW  Masking GB.

            ADR_VENDOR_MASKING_BR_INT32B        = ADR_CB31 + 0x00C0,        // 0x21F0C0 RW  Masking BR FeatureCSR.
            ADR_VENDOR_MASKING_BR_RAW_I         = ADR_CB31 + 0x00DC,        // 0x21F0DC RW  Masking BR.

            ADR_VENDOR_MASKING_BG_INT32B        = ADR_CB31 + 0x00E0,        // 0x21F0E0 RW  Masking BG FeatureCSR.
            ADR_VENDOR_MASKING_BG_RAW_I         = ADR_CB31 + 0x00FC,        // 0x21F0FC RW  Masking BG.

            ADR_VENDOR_LED_LUMINANCE_INT32B     = ADR_CB31 + 0x0100,        // 0x21F100 RW  LED Indicator Luminance FeatureCSR.
            ADR_VENDOR_LED_LUMINANCE_RAW_I      = ADR_CB31 + 0x011C,        // 0x21F11C RW  LED Indicator Luminance.

            ADR_VENDOR_TEST_PATTERN_ENUM32B     = ADR_CB31 + 0x0120,        // 0x21F120 RW  Test pattern FeatureCSR. [g_saVAL_TestPtn]
            ADR_VENDOR_TEST_PATTERN_I           = ADR_CB31 + 0x013C,        // 0x21F13C RW  Test pattern. [g_saVAL_TestPtn]

            ADR_VENDOR_BINNING_H_INT32B         = ADR_CB31 + 0x0140,        // 0x21F140 RW  Binning Horizontal FeatureCSR.
            ADR_VENDOR_BINNING_H_RAW_I          = ADR_CB31 + 0x015C,        // 0x21F15C RW  Binning Horizontal.

            ADR_VENDOR_BINNING_V_INT32B         = ADR_CB31 + 0x0160,        // 0x21F160 RW  Binning Vertical FeatureCSR.
            ADR_VENDOR_BINNING_V_RAW_I          = ADR_CB31 + 0x017C,        // 0x21F17C RW  Binning Vertical.

            ADR_VENDOR_DECIMATION_H_INT32B      = ADR_CB31 + 0x0180,        // 0x21F180 RW  Desimation Horizontal FeatureCSR.
            ADR_VENDOR_DECIMATION_H_RAW_I       = ADR_CB31 + 0x019C,        // 0x21F19C RW  Desimation Horizontal.

            ADR_VENDOR_DECIMATION_V_INT32B      = ADR_CB31 + 0x01A0,        // 0x21F1A0 RW  Decimation Vertical FeatureCSR.
            ADR_VENDOR_DECIMATION_V_RAW_I       = ADR_CB31 + 0x01BC,        // 0x21F1BC RW  Decimation Vertical.

            ADR_VENDOR_REVERSE_X_BULK20B        = ADR_CB31 + 0x01C0,        // 0x21F1C0 RW  Reverse X FeatureCSR. [g_saVAL_Reverse]
            ADR_VENDOR_REVERSE_X_I              = ADR_CB31 + 0x01D0,        // 0x21F1D0 RW  Reverse X. [g_saVAL_Reverse]

            ADR_VENDOR_REVERSE_Y_BULK20B        = ADR_CB31 + 0x01E0,        // 0x21F1E0 RW  Reverse Y FeatureCSR. [g_saVAL_Reverse]
            ADR_VENDOR_REVERSE_Y_I              = ADR_CB31 + 0x01F0,        // 0x21F1F0 RW  Reverse Y. [g_saVAL_Reverse]

            ADR_VENDOR_EVENT_ACQUIS_BULK20B     = ADR_CB31 + 0x0200,        // 0x21F200 RW  Event Notification Of Acquisition FeatureCSR. [g_saVAL_EventAcquis]
            ADR_VENDOR_EVENT_ACQUIS_I           = ADR_CB31 + 0x0210,        // 0x21F210 RW  Event Notification Of Acquisition. [g_saVAL_EventAcquis]

            ADR_VENDOR_EVENT_FRAME_BULK20B      = ADR_CB31 + 0x0220,        // 0x21F220 RW  Event Notification Of Frame FeatureCSR. [g_saVAL_EventFrame]
            ADR_VENDOR_EVENT_FRAME_I            = ADR_CB31 + 0x0230,        // 0x21F230 RW  Event Notification Of Frame. [g_saVAL_EventFrame]

            ADR_VENDOR_EVENT_EXPO_BULK20B       = ADR_CB31 + 0x0240,        // 0x21F240 RW  Event Notification Of Exposure FeatureCSR. [g_saVAL_EventExposure]
            ADR_VENDOR_EVENT_EXPO_I             = ADR_CB31 + 0x0250,        // 0x21F250 RW  Event Notification Of Exposure. [g_saVAL_EventExposure]

            ADR_VENDOR_TMER_TRG_SRC_ENUM32B     = ADR_CB31 + 0x0260,        // 0x21F260 RW  Timer Trigger Source FeatureCSR. [g_saVAL_TmTrgSrc]
            ADR_VENDOR_TMER_TRG_SRC_I           = ADR_CB31 + 0x027C,        // 0x21F27C RW  Timer Trigger Source. [g_saVAL_TmTrgSrc]

            ADR_VENDOR_DPC_ENABLE_ENUM32B       = ADR_CB31 + 0x0280,        // 0x21F280 RW  Defect pixel compensation enable FeatureCSR. [g_saVAL_OnOff]
            ADR_VENDOR_DPC_ENABLE_I             = ADR_CB31 + 0x029C,        // 0x21F29C RW  Defect pixel compensation enable. [g_saVAL_OnOff]

            ADR_VENDOR_DPC_NUMBER_INT32B        = ADR_CB31 + 0x02A0,        // 0x21F2A0 RW  Defect pixel compensation number FeatureCSR. 
            ADR_VENDOR_DPC_NUMBER_RAW_I         = ADR_CB31 + 0x02BC,        // 0x21F2BC RW  Defect pixel compensation number. 

            ADR_VENDOR_DPC_COORD_ENUM32B        = ADR_CB31 + 0x02C0,        // 0x21F2C0 RW  Defect pixel compensation coordinates for introducing Category block 33 FeatureCSR.         #Appended V1.0.2.0
            ADR_VENDOR_DPCCOORD_EXPCATBLK_I     = ADR_CB31 + 0x02C4,        // 0x21F2C4 RO  Defect pixel compensation coordinates category block number[31..24], OffsetAddress[23..0].  #Appended V1.0.2.0

            ADR_VENDOR_USERSET_DEFAULT_ENUM32B  = ADR_CB31 + 0x02E0,        // 0x21F2E0 RW  UserSet Default FeatureCSR. [g_saVAL_UsersetSel]                                        #Appended V1.0.2.0
            ADR_VENDOR_USERSET_DEFAULT_I        = ADR_CB31 + 0x02FC,        // 0x21F2FC RW  UserSet Default. [g_saVAL_UsersetSel]                                                   #Appended V1.0.2.0

            ADR_VENDOR_SEQ_SHT_ENABLE_ENUM32B   = ADR_CB31 + 0x0300,        // 0x21F300 RW  Sequential Shutter Enable FeatureCSR. [g_saVAL_OnOff]                                   #Appended V1.0.2.0
            ADR_VENDOR_SEQ_SHT_ENABLE_I         = ADR_CB31 + 0x031C,        // 0x21F31C RW  Sequential Shutter Enable. [g_saVAL_OnOff]                                              #Appended V1.0.2.0

            ADR_VENDOR_SEQ_SHT_TRMNAT_AT_INT32B = ADR_CB31 + 0x0320,        // 0x21F320 RW  Sequential Shutter Terminate at FeatureCSR.                                             #Appended V1.0.2.0
            ADR_VENDOR_SEQ_SHT_TRMNAT_AT_I      = ADR_CB31 + 0x033C,        // 0x21F33C RW  Sequential Shutter Terminate at.                                                        #Appended V1.0.2.0

            ADR_VENDOR_SEQ_SHT_SEQ_TBL_ENUM32B  = ADR_CB31 + 0x0340,        // 0x21F340 RW  Sequential Shutter Sequence Table FeatureCSR.                                           #Appended V1.0.2.0
            ADR_VENDOR_SEQ_SHT_SEQ_TBL_EXP_I    = ADR_CB31 + 0x0344,        // 0x21F344 RW  Sequential Shutter Sequence Table Offset for expanded CSR. (CB35 Offset 0x0020)         #Appended V1.0.2.0

            ADR_VENDOR_ALC_AREA_INT32B          = ADR_CB31 + 0x0360,        // 0x21F360 RW  ALC Area FeatureCSR.                                                                    #Appended V1.0.2.0
            ADR_VENDOR_ALC_AREA_I               = ADR_CB31 + 0x037C,        // 0x21F37C RW  ALC ARea.                                                                               #Appended V1.0.2.0

            ADR_VENDOR_EVENT_TMR_START_BULK20B  = ADR_CB31 + 0x0380,        // 0x21F380 RW  Event Notification Of Timer Start FeatureCSR.  [g_saVAL_TimerSel]                       #Appended V1.0.2.0
            ADR_VENDOR_EVENT_TMR_START_I        = ADR_CB31 + 0x0390,        // 0x21F390 RW  Event Notification Of Timer Start. [g_saVAL_TimerSel]                                   #Appended V1.0.2.0

            ADR_VENDOR_EVENT_TMR_END_BULK20B    = ADR_CB31 + 0x03A0,        // 0x21F3A0 RW  Event Notification Of Timer End FeatureCSR. [g_saVAL_TimerSel]                          #Appended V1.0.2.0
            ADR_VENDOR_EVENT_TMR_END_I          = ADR_CB31 + 0x03B0,        // 0x21F3B0 RW  Event Notification Of Timer End. [g_saVAL_TimerSel]                                     #Appended V1.0.2.0

            ADR_VENDOR_ANTI_GLITCH_INT32B       = ADR_CB31 + 0x03C0,        // 0x21F3C0 RW  AntiGlitch FeatureCSR.                                                                  #Appended V1.0.3.1
            ADR_VENDOR_ANTI_GLITCH_RAW_I        = ADR_CB31 + 0x03DC,        // 0x21F3DC RW  AntiGlitch.                                                                             #Appended V1.0.3.1

            ADR_VENDOR_ANTI_CHATTERING_INT32B   = ADR_CB31 + 0x03E0,        // 0x21F3E0 RW  AntiGlitch FeatureCSR.                                                                  #Appended V1.0.3.1
            ADR_VENDOR_ANTI_CHATTERING_RAW_I    = ADR_CB31 + 0x03FC,        // 0x21F3FC RW  AntiGlitch.                                                                             #Appended V1.0.3.1

            ADR_BAYER_PROCESSING_MODE_ENUM32B   = ADR_CB31 + 0x0420,        // 0x21F420 RW  BayerProcessingMode FeatureCSR.
            ADR_BAYER_PROCESSING_MODE_I         = ADR_CB31 + 0x043C,        // 0x21F43C RW  BayerProcessingMode.

            ADR_VENDOR_HI_FRAMERATE_MODE_INT32B     = ADR_CB31 + 0x0660,    // 0x21F660 RW  High framerate mode FeatureCSR.
            ADR_VENDOR_HI_FRAMERATE_MODE_RAW_I      = ADR_CB31 + 0x066C,    // 0x21F66C RW  High framerate mode.
            ADR_VENDOR_HI_FRAMERATE_MODE_CNTRL_I    = ADR_CB31 + 0x067C,    // 0x21F67C RW  Control register of High framerate mode FeatureCSR.

            //=================================================================
            // IIDC2 CategoryBlock 32.Expanded for Digital IO Control  (Defined by Teli)
            //=================================================================
            ADR_VENDOR_IOLINESOURCE_1_ENUM32B   = ADR_CB32 + 0x0040,        // 0x222040 RW  IO Line Source 1 ExpandedCSR. [g_saVAL_IOLineSrc]                                       #Appended V1.0.2.0
            ADR_VENDOR_IOLINESOURCE_1__I        = ADR_CB32 + 0x005C,        // 0x22205C RW  IO Line Source 1. [g_saVAL_IOLineSrc]                                                   #Appended V1.0.2.0

            ADR_VENDOR_IOLINESOURCE_2_ENUM32B   = ADR_CB32 + 0x0060,        // 0x222060 RW  IO Line Source 2 ExpandedCSR. [g_saVAL_IOLineSrc]                                       #Appended V1.0.2.0
            ADR_VENDOR_IOLINESOURCE_2__I        = ADR_CB32 + 0x007C,        // 0x22207C RW  IO Line Source 2. [g_saVAL_IOLineSrc]                                                   #Appended V1.0.2.0

            //=================================================================
            // IIDC2 CategoryBlock 33.Expanded for LUT   (Defined by Teli)
            //=================================================================
            ADR_VENDOR_LUTVALUE_AINT_STX        = ADR_CB33 + 0x0020,        // 0x2FFFDC RW  LUT ExpandedCSR.                                                                        #Appended V1.0.2.0
            ADR_VENDOR_LUTVALUE_NUM_CHANNEL_I   = ADR_CB33 + 0x002C,        // 0x2FFFE8 RW  LUT Number of channel.                                                                  #Appended V1.0.2.0
            ADR_VENDOR_LUTVALUE_NUM_ELEMENT_I   = ADR_CB33 + 0x0030,        // 0x2FFFEC RW  LUT Number of Element.                                                                  #Appended V1.0.2.0
            ADR_VENDOR_LUTVALUE_VALUE_TOP_I     = ADR_CB33 + 0x0044,        // 0x300000 RW  LUT Top of values.                                                                      #Appended V1.0.2.0

            
            //=================================================================
            // IIDC2 CategoryBlock 34.Expanded for DPC   (Defined by Teli)
            //=================================================================
            ADR_VENDOR_DPC_COORD_AINT_STX       = ADR_CB34 + 0x0020,        // 0x3FFFDC RW  Defect pixel compensation coordinates ExpandedCSR.                                      #Appended V1.0.2.0
            ADR_VENDOR_DPC_COORD_NUM_ELEMENT_I  = ADR_CB34 + 0x002C,        // 0x3FFFE8 RW  DPC Number of Element.                                                                  #Appended V1.0.2.0
            ADR_VENDOR_DPC_COORD_NUM_POSITION_I = ADR_CB34 + 0x0030,        // 0x3FFFEC RW  DPC Number of Position.                                                                 #Appended V1.0.2.0
            ADR_VENDOR_DPCVALUE_VALUE_TOP_I     = ADR_CB34 + 0x0044,        // 0x400000 RW  DPC Top of values.                                                                      #Appended V1.0.2.0

            //=================================================================
            // IIDC2 CategoryBlock 35.Expanded for Sequential Shutter   (Defined by Teli)
            //=================================================================
            ADR_VENDOR_SEQSHT_SEQTABLE_AINT_STX = ADR_CB35 + 0x0020,        // 0x500020 RW  Sequential Shutter Seqquence table ExpandedCSR.                                         #Appended V1.0.2.0
            ADR_VENDOR_SEQTABLE_NUM_ELEMENT_I   = ADR_CB35 + 0x002C,        // 0x50002C RW  Seqquence table Number of Element.                                                      #Appended V1.0.2.0
            ADR_VENDOR_SEQTABLE_VALUE_TOP_I     = ADR_CB35 + 0x0040,        // 0x500040 RW  Seqquence table Top of values.                                                          #Appended V1.0.2.0

    // Offsets of registers (Commonly usable except vender unique registers (IIDC2 Category block 31, 32, 33).)


            //=================================================================
            // Manifest table     (Defined in USB3 Vision standard and GenICam CenCP standard)
            //=================================================================
            OFS_MFSTBL_ENTRY_COUNT_L            = 0x0000,       // RO   Entry count
            OFS_MFSTBL_ENTRY_0_ST64             = 0x0008,       // RO   Manifest Entry #0
            OFS_MFSTBL_ENTRY_1_ST64             = 0x0048,       // RO   Manifest Entry #1

            //=================================================================
            // Manifest Entry     (Defined in USB3 Vision standard and GenICam CenCP standard)
            //=================================================================
            LEN_MFSETRY_ENTRY                   = 0x0040,       // Length of Manifest Entry.
            OFS_MFSETRY_SUBMINOR_VER_S          = 0x0000,       // RO   GenICam file-subminor version.
            OFS_MFSETRY_MINOR_VER_B             = 0x0002,       // RO   GenICam file-minor version.
            OFS_MFSETRY_MAJOR_VER_B             = 0x0003,       // RO   GenICam file-major version.
            OFS_MFSETRY_FILE_TYPE_S             = 0x0004,       // RO   File type of camera description file.  [g_saVAL_GFType]
            OFS_MFSETRY_SCHEMA_MINOR_B          = 0x0006,       // RO   Minor version of camera description file schema.
            OFS_MFSETRY_SCHEMA_MAJOR_B          = 0x0007,       // RO   Major version of camera description file schema.
            OFS_MFSETRY_REGISTER_ADDR_L         = 0x0008,       // RO   Register address of camera description file.
            OFS_MFSETRY_FILE_SIZE_L             = 0x0010,       // RO   Size of camera description file in bytes.
            OFS_MFSETRY_SHA1_HASH_B20           = 0x0018,       // RO   SHA1 hash of camera description file.

            //=================================================================
            // SBRM     (Defined in USB3 Vision standard)
            //=================================================================
            OFS_SBRM_U3V_VERSION_I              = 0x0000,       // RO   Version of the USB3 Vision standard.
            OFS_SBRM_U3VCP_CAPABILITY_L         = 0x0004,       // RO   Bit field specifying the device's U3V features and capabilities. [g_saVAL_U3VCPCapa]
            OFS_SBRM_U3VCP_CONFIG_L             = 0x000C,       // RW   Configuration register for additional features on the control channel. Current USB3 Vision standard has no additional features.
            OFS_SBRM_MAX_CMD_TRANSFER_LEN_I     = 0x0014,       // RO   Maximum command transfer length.
            OFS_SBRM_MAX_ACK_TRANSFER_LEN_I     = 0x0018,       // RO   Maximum acknowledge transfer length.
            OFS_SBRM_NUM_STREAM_CHANNELS_I      = 0x001C,       // RO   Number of stream channelsand the corresponding SIRM.
            OFS_SBRM_SIRM_ADDRESS_L             = 0x0020,       // RO   SIRM address.
            OFS_SBRM_SIRM_LENGTH_I              = 0x0028,       // RO   SIRM length.
            OFS_SBRM_EIRM_ADDRESS_L             = 0x002C,       // RO   EIRM address.
            OFS_SBRM_EIRM_LENGTH_I              = 0x0034,       // RO   EIRM length.
            OFS_SBRM_IIDC2_ADDRESS_L            = 0x0038,       // RO   IIDC2 Entry table address.
            OFS_SBRM_CURRENT_SPEED_I            = 0x0040,       // RO   Current speed of the USB link. [g_saVAL_USB_Speed]

            //=================================================================
            // SIRM     (Defined in USB3 Vision standard)
            //=================================================================
            OFS_SIRM_SI_INFO_I                  = 0x0000,       // RO   Information about stream interface that device reports. Payload size alignment data is included in this member. [g_saVAL_Align_Payload]
            OFS_SIRM_SI_CONTROL_C               = 0x0004,       // RW   Enable / disable streaming. [g_saVAL_SIControl]
            OFS_SIRM_REQ_PAYLOAD_SIZE_L         = 0x0008,       // RO   Minimum required payload size that device reports.
            OFS_SIRM_REQ_LEADER_SIZE_I          = 0x0010,       // RO   Minimum required leader size that device reports.
            OFS_SIRM_REQ_TRAILER_SIZE_I         = 0x0014,       // RO   Minimum required trailer size that device reports.
            OFS_SIRM_MAX_LEADER_SIZE_I          = 0x0018,       // RW   Maximum leader size.
            OFS_SIRM_PYLD_TRANSFER_SIZE_I       = 0x001C,       // RW   Expected size of a single payload trannsfer.
            OFS_SIRM_PYLD_TRANSFER_COUNT_I      = 0x0020,       // RW   Expected number of payload transfer.
            OFS_SIRM_PYLD_FINALTRANS1_SIZE_I    = 0x0024,       // RQ   Size of first final payload transfer.
            OFS_SIRM_PYLD_FINALTRANS2_SIZE_I    = 0x0028,       // RW   Size of second final payload transfer.
            OFS_SIRM_MAX_TRAILER_SIZE_I         = 0x002C,       // RW   Maximum trailer size.

            //=================================================================
            // EIRM     (Defined in USB3 Vision standard)
            //=================================================================
            OFS_EIRM_EI_CONTROL_C               = 0x0000,       // RW   Enable / disable event. [g_saVAL_EIControl]
            OFS_EIRM_MAX_TRANSFER_LENGTH_I      = 0x0004,       // RO   Maximum event transfer length.
            OFS_EIRM_EVENT_TEST_CONTROL_C       = 0x0008,       // RW   Controls thre generation of test event. (lsb is TriggerEventTest bit)

            //=================================================================
            // IIDC2Entry table     (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_IIDC2_KEYWORD_C4                = 0x0000,       // RO   keyword for IIDC2 Entry (char[4] = "IIDC" = 0x49494443).
            OFS_IIDC2_VERSION_SUBMINOR_B        = 0x0004,       // RO   Sub-minor version of supported IIDC2 standard.
            OFS_IIDC2_VERSION_MINOR_B           = 0x0005,       // RO   Minor version of supported IIDC2 standard.
            OFS_IIDC2_VERSION_MAJOR_B           = 0x0006,       // RO   Major version of supported IIDC2 standard.
            OFS_IIDC2_NUM_CATEGORYBLOCK_S       = 0x0008,       // RO   Number of category blocks.
            OFS_IIDC2_OFS_MANIFEST_TABLE_I      = 0x0010,       // RO   Offset for xml manifest table.
            OFS_IIDC2_OFS_CATEGORYBLOCK_00_I    = 0x0020,       // RO   Offset for category block 0. Device Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_01_I    = 0x0024,       // RO   Offset for category block 1. Transfer Layer Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_02_I    = 0x0028,       // RO   Offset for category block 2. Image Format Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_03_I    = 0x002C,       // RO   Offset for category block 3. Acquisition Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_04_I    = 0x0030,       // RO   Offset for category block 4. Luminance Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_05_I    = 0x0034,       // RO   Offset for category block 5. Chroma Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_06_I    = 0x0038,       // RO   Offset for category block 6. LUT Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_07_I    = 0x003C,       // RO   Offset for category block 7. Trigger Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_08_I    = 0x0040,       // RO   Offset for category block 8. UserSet Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_09_I    = 0x0044,       // RO   Offset for category block 9. Digital IO Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_10_I    = 0x0048,       // RO   Offset for category block 10. Counter And Timer Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_31_I    = 0x009C,       // RO   Offset for category block 31. Vender Unique Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_32_I    = 0x00A0,       // RO   Offset for category block 32. Vender Unique Control.
            OFS_IIDC2_OFS_CATEGORYBLOCK_33_I    = 0x00A4,       // RO   Offset for category block 33. Vender Unique Control.


            //=================================================================
            // IIDC2 CategoryBlock 0. Device control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_DEVICE_RESET_ENUM32B            = 0x0020,       // RW   Device Reset FeatureCSR.
            OFS_DEVICE_RESET_I                  = 0x003C,       // RW   Device Reset Value.

            //=================================================================
            // IIDC2 CategoryBlock 1. Transfer layer control.     (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            // CategoryBlock1 is not used in USB3 vision because SIRM provides transport layer registers. 

            //=================================================================
            // IIDC2 CategoryBlock 2. Image Format control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_IMGFMT_IMGFMT_SEL_ENUM32B       = 0x0020,       // RW   Image format selector FeatureCSR. [g_saVAL_FormatSel]
            OFS_IMGFMT_IMGFMT_SEL_I             = 0x003C,       // RW   Image format selector value. [g_saVAL_FormatSel]

            OFS_IMGFMT_APPLY_IMGFMT_ENUM32B     = 0x0040,       // RW   Apply image format FeatureCSR. [g_saVAL_ApplyImgFmt]
            OFS_IMGFMT_APPLY_IMGFMT_I           = 0x005C,       // RW   Apply image format value. [g_saVAL_ApplyImgFmt]

            OFS_IMGFMT_IMAGE_SIZE_RECT96        = 0x0060,       // RW   Image size FeatureCSR.
            OFS_IMGFMT_IMAGE_OFFSETX_MIN_I      = 0x006C,       // RW   Image offset X minimum value.
            OFS_IMGFMT_IMAGE_OFFSETX_INC_I      = 0x0070,       // RW   Image offset X increment value.
            OFS_IMGFMT_IMAGE_WIDTH_MIN_I        = 0x0074,       // RW   Image width minimum value.
            OFS_IMGFMT_IMAGE_WIDTH_INC_I        = 0x0078,       // RW   Image width increment value.
            OFS_IMGFMT_IMAGE_TOTAL_SIZE_X_I     = 0x007C,       // RW   Image total size X value.
            OFS_IMGFMT_IMAGE_OFFSETY_MIN_I      = 0x0080,       // RW   Image offset Y minimum value.
            OFS_IMGFMT_IMAGE_OFFSETY_INC_I      = 0x0084,       // RW   Image offset Y increment value.
            OFS_IMGFMT_IMAGE_HEIGHT_MIN_I       = 0x0088,       // RW   Image height minimum value.
            OFS_IMGFMT_IMAGE_HEIGHT_INC_I       = 0x008C,       // RW   Image height increment value.
            OFS_IMGFMT_IMAGE_TOTAL_SIZE_Y_I     = 0x0090,       // RW   Image tital size Y value.
            OFS_IMGFMT_IMAGE_OFFSETX_I          = 0x0094,       // RW   Image offset X value.
            OFS_IMGFMT_IMAGE_WIDTH_I            = 0x0098,       // RW   Image width value.
            OFS_IMGFMT_IMAGE_OFFSETY_I          = 0x009C,       // RW   Image offset Y value.
            OFS_IMGFMT_IMAGE_HEIGHT_I           = 0x00A0,       // RW   Image height value.

            OFS_IMGFMT_PIXEL_CODING_ENUM32B     = 0x00C0,       // RW   Pixel coding FeatureCSR. [g_saVAL_PixelCoding]
            OFS_IMGFMT_PIXEL_CODING_I           = 0x00DC,       // RW   Pixel coding value. [g_saVAL_PixelCoding]

            OFS_IMGFMT_PIXEL_SIZE_ENUM32B       = 0x00E0,       // RW   Pixel size FeatureCSR. [g_saVAL_PixelSize]
            OFS_IMGFMT_PIXEL_SIZE_I             = 0x00FC,       // RW   Pixel size value. [g_saVAL_PixelSize]

            OFS_IMGFMT_PIXEL_ENDIAN_ENUM32B     = 0x0100,       // RW   Pixel endian FeatureCSR. [g_saVAL_PixelEndian]
            OFS_IMGFMT_PIXEL_ENDIAN_I           = 0x011C,       // RW   Pixel endian value. [g_saVAL_PixelEndian]

            //=================================================================
            // IIDC2 CategoryBlock 3. Acquisition control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_ACQ_ACQUISITION_ENUM32B         = 0x0020,       // RW   Acquisition command FeatureCSR. [g_saVAL_AcquisCmd]
            OFS_ACQ_ACQUISITION_I               = 0x003C,       // RW   Acquisition command. [g_saVAL_AcquisCmd]

            OFS_ACQ_ACQUIS_FRM_COUNT_INT32B     = 0x0040,       // RW   Acquisition frame count  FeatureCSR. Mult and Div are not used.
            OFS_ACQ_ACQUIS_FRM_COUNT_I          = 0x005C,       // RW   Acquisition frame count.

            OFS_ACQ_IMGBUF_MODE_ENUM32B         = 0x0060,       // RW   Image buffer mode FeatureCSR. [g_saVAL_OnOff]
            OFS_ACQ_IMGBUF_MODE_I               = 0x007C,       // RW   Image buffer mode. [g_saVAL_OnOff]

            OFS_ACQ_IMGBUF_FRM_COUNT_INT32B     = 0x0080,       // RW   Image buffer frame count FeatureCSR. Mult and Div are not used. Min is always 0.
            OFS_ACQ_IMGBUF_FRM_COUNT_I          = 0x009C,       // RW   Image buffer frame count.

            OFS_ACQ_FRAME_RATE_INT32B           = 0x00A0,       // RW   Acquisition frame rate FeatureCSR.
            OFS_ACQ_FRAME_RATE_RAW_I            = 0x00BC,       // RW   Acquisition frame rate.
            OFS_ACQ_FRAME_RATE_CONTROL_I        = 0x00A8,       // RW   Acquisition frame rate control.

            OFS_ACQ_FRAME_INTERVAL_INT32B       = 0x00C0,       // RW   Acquisition frame interval FeatureCSR.
            OFS_ACQ_FRAME_INTERVAL_RAW_I        = 0x00DC,       // RW   Acquisition frame interval.

            //=================================================================
            // IIDC2 CategoryBlock 4. Luminance control.    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_LMN_EXPOSURE_TIME_INT32B        = 0x0020,       // RW   Exposure time  FeatureCSR in microseconds.
            OFS_LMN_EXPOSURE_TIME_RAW_I         = 0x003C,       // RW   Exposure time in microseconds.
            OFS_LMN_EXPOSURE_TIME_CONTROL_I     = 0x0028,       // RW   Exposure time control.

            OFS_LMN_BLACK_LEVEL_INT32B          = 0x0040,       // RW   Black level FeatureCSR.
            OFS_LMN_BLACK_LEVEL_RAW_I           = 0x005C,       // RW   Black level.

            OFS_LMN_GAIN_INT32B                 = 0x0060,       // RW   Gain FeatureCSR.
            OFS_LMN_GAIN_RAW_I                  = 0x007C,       // RW   Gain.

            OFS_LMN_GAMMA_INT32B                = 0x0080,       // RW   Gamma FeatureCSR.
            OFS_LMN_GAMMA_RAW_I                 = 0x009C,       // RW   Gamma.

            OFS_LMN_SHARPNESS_INT32B            = 0x00A0,       // RW   Sharpness FeatureCSR.
            OFS_LMN_SHARPNESS_RAW_I             = 0x00BC,       // RW   Sharpness.

            OFS_LMN_ALC_LEVEL_INT32B            = 0x00C0,       // RW   ALC level FeatureCSR.
            OFS_LMN_ALC_LEVEL_RAW_I             = 0x00DC,       // RW   ALC level.

            //=================================================================
            // IIDC2 CategoryBlock 5.Chroma Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_CHROMA_HUE_INT32B               = 0x0020,       // RW   Hue FeatureCSR.
            OFS_CHROMA_HUE_I                    = 0x003C,       // RW   Hue.

            OFS_CHROMA_SATURATION_INT32B        = 0x0040,       // RW   Saturation FeatureCSR.
            OFS_CHROMA_SATURATION_I             = 0x005C,       // RW   Saturation.

            OFS_CHROMA_WH_BALANCE_R_INT32B      = 0x0060,       // RW   WhiteBalanceR FeatureCSR.
            OFS_CHROMA_WH_BALANCE_R_I           = 0x007C,       // RW   WhiteBalanceR.

            OFS_CHROMA_WH_BALANCE_B_INT32B      = 0x0080,       // RW   WhiteBalanceB FeatureCSR.
            OFS_CHROMA_WH_BALANCE_B_I           = 0x009C,       // RW   WhiteBalanceB.

            OFS_CHROMA_WH_BALANCE_U_INT32B      = 0x00A0,       // RW   WhiteBalanceU FeatureCSR.
            OFS_CHROMA_WH_BALANCE_U_I           = 0x00BC,       // RW   WhiteBalanceU.

            OFS_CHROMA_WH_BALANCE_V_INT32B      = 0x00C0,       // RW   WhiteBalanceV FeatureCSR.
            OFS_CHROMA_WH_BALANCE_V_I           = 0x00DC,       // RW   WhiteBalanceV.

            OFS_CHROMA_COLOR_TEMP_INT32B        = 0x00E0,       // RW   ColorTemperture FeatureCSR.
            OFS_CHROMA_COLOR_TEMP_I             = 0x00FC,       // RW   ColorTemperture.

            //=================================================================
            // IIDC2 CategoryBlock 6.LUTControl    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_LUT_LUT_ENABLE_ENUM32B          = 0x0020,       // RW   LUTEnable FeatureCSR. [g_saVAL_OnOff]
            OFS_LUT_LUT_ENABLE_I                = 0x003C,       // RW   LUTEnable. [g_saVAL_OnOff]

            OFS_LUT_BANK_SELECTOR_ENUM32B       = 0x0040,       // RW   LUTBankSelector FeatureCSR. [g_saVAL_LUTBankSel]
            OFS_LUT_BANK_SELECTOR_I             = 0x005C,       // RW   LUTBankSelector. [g_saVAL_LUTBankSel]
    
            OFS_LUT_VALUE_ALL_ENUM32B           = 0x0060,       // RW   LUTValueAll FeatureCSR. [g_saVAL_LUTValAll]
            OFS_LUT_VALUE_ALL_I                 = 0x007C,       // RW   LUTValueAll. [g_saVAL_LUTValAll]

            //=================================================================
            // IIDC2 CategoryBlock 7.Trigger Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_TRG_TRG_MODE_ENUM32B            = 0x0020,       // RW   Trigger mode FeatyreCSR. [g_saVAL_OnOff]
            OFS_TRG_TRG_MODE_I                  = 0x003C,       // RW   Trigger mode. [g_saVAL_OnOff]

            OFS_TRG_TRG_SEQUENCE_ENUM32B        = 0x0040,       // RW   Trigger sequence FeatureCSR. [g_saVAL_TrigSeq]
            OFS_TRG_TRG_SEQUENCE_I              = 0x005C,       // RW   Trigger sequence. [g_saVAL_TrigSeq]

            OFS_TRG_TRG_SOURCE_ENUM32B          = 0x0060,       // RW   Trigger source FeatureCSR. [g_saVal_TrigSrc]
            OFS_TRG_TRG_SOURCE_I                = 0x007C,       // RW   Trigger source. [g_saVal_TrigSrc]

            OFS_TRG_ADDITIONAL_PARAM_INT32B     = 0x0080,       // RW   Trigger Additional Parameter FeatureCSR.
            OFS_TRG_ADDITIONAL_PARAM_RAW_I      = 0x009C,       // RW   Trigger Additional Parameter.

            OFS_TRG_TRG_DELAY_INT32B            = 0x00A0,       // RW   Trigger Delay FeatureCSR.
            OFS_TRG_TRG_DELAY_RAW_I             = 0x00BC,       // RW   Trigger Delay.

            OFS_TRG_SOFT_TRG_ENUM32B            = 0x00C0,       // RW   SoftwareTrigger FeatureCSR. [g_saVAL_SoftTrig]
            OFS_TRG_SOFT_TRG_I                  = 0x00DC,       // RW   SoftwareTrigger. [g_saVAL_SoftTrig]

            //=================================================================
            // IIDC2 CategoryBlock 8.Userset Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_USET_STD_FORMAT_ENUM32B         = 0x0020,       // RW   Standard format FeatureCSR. [g_saVAL_StdFmt]
            OFS_USET_STD_FORMAT_I               = 0x003C,       // RW   Standard format. [g_saVAL_StdFmt] 

            OFS_USET_STD_FRAME_RATE_ENUM32B     = 0x0040,       // RW   Standard frame rate FeatureCSR. [g_saVAL_StdFrmRate]
            OFS_USET_STD_FRAME_RATE_I           = 0x005C,       // RW   Standard frame rate. [g_saVAL_StdFrmRate]

            OFS_USET_USERSET_SEL_ENUM32B        = 0x0060,       // RW   User set selector FeatureCSR. [g_saVAL_UsersetSel]
            OFS_USET_USERSET_SEL_I              = 0x007C,       // RW   User set selector. [g_saVAL_UsersetSel]

            OFS_USET_USERSET_CMD_ENUM32B        = 0x0080,       // RW   User set command FeatureCSR. [g_saVAL_UsersetCmd]
            OFS_USET_USERSET_CMD_I              = 0x009C,       // RW   User set command. [g_saVAL_UsersetCmd]

            //=================================================================
            // IIDC2 CategoryBlock 9.Digital IO Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_DIO_IOLINE_MODE_ALL_BULK20B     = 0x0020,       // RW   IO line mode all FeatureCSR.
            OFS_DIO_IOLINE_MODE_ALL_I           = 0x0030,       // RW   IO line mode all.

            OFS_DIO_IO_INVERTER_ALL_BULK20B     = 0x0040,       // RW   IO line inverter all FeytureCSR.
            OFS_DIO_IO_INVERTER_ALL_I           = 0x0050,       // RW   IO line inverter all.

            OFS_DIO_IO_STATUS_ALL_BULK20B       = 0x0060,       // RW   IO line status all FeytureCSR.
            OFS_DIO_IO_STATUS_ALL_I             = 0x0070,       // RW   IO line status all.

            OFS_DIO_IO_OUT_VAL_ALL_BULK20B      = 0x0080,       // RW   IO line output value all FeytureCSR.
            OFS_DIO_IO_OUT_VAL_ALL_I            = 0x0090,       // RW   IO line output value all.

            OFS_DIO_IO_LINE_SEL_ENUM32B         = 0x00A0,       // RW   IO line selector FeytureCSR. [g_saVAL_IOLineSel]
            OFS_DIO_IO_LINE_SEL_I               = 0x00BC,       // RW   IO line selector. [g_saVAL_IOLineSel]

            OFS_DIO_IO_LINE_SOURCE_ENUM32B      = 0x00C0,       // RW   IO line source FeytureCSR. [g_saVAL_IOLineSource]
            OFS_DIO_IO_LINE_SOURCE_I            = 0x00DC,       // RW   IO line source. [g_saVAL_IOLineSource]

            //=================================================================
            // IIDC2 CategoryBlock 10.Counter and timer Control    (Defined in IIDC2 Digital Camera Control Specification)
            //=================================================================
            OFS_CNTTM_TIMER_SEL_ENUM32B         = 0x0020,       // RW   Timer selector FeytureCSR. [g_saVAL_TimerSel]
            OFS_CNTTM_TIMER_SEL_I               = 0x003C,       // RW   Timer selector. [g_saVAL_TimerSel]

            OFS_CNTTM_TIMER_DELAY_INT32B        = 0x0040,       // RW   Timer delay FeatureCSR.
            OFS_CNTTM_TIMER_DELAY_RAW_I         = 0x005C,       // RW   Timer delay.

            OFS_CNTTM_TIMER_DURATION_INT32B     = 0x0060,       // RW   Timer duration FeatureCSR.
            OFS_CNTTM_TIMER_DURATION_RAW_I      = 0x007C,       // RW   Timer duration.

            //=================================================================
            // IIDC2 CategoryBlock 31.Vendor unique Control    (Defined by Teli)
            //=================================================================
            OFS_VENDOR_FRAME_SYNC_ENUM32B       = 0x0020,       // RW   Frame synchronization FeatureCSR, [g_saVAL_FrmSync]
            OFS_VENDOR_FRAME_SYNC_I             = 0x003C,       // RW   Frame synchronization, [g_saVAL_FrmSync]

            OFS_VENDOR_MASKING_RG_INT32B        = 0x0040,       // RW   Masking RG FeatureCSR.
            OFS_VENDOR_MASKING_RG_RAW_I         = 0x005C,       // RW   Masking RG.

            OFS_VENDOR_MASKING_RB_INT32B        = 0x0060,       // RW   Masking RB FeatureCSR.
            OFS_VENDOR_MASKING_RB_RAW_I         = 0x007C,       // RW   Masking RB.

            OFS_VENDOR_MASKING_GR_INT32B        = 0x0080,       // RW   Masking GR FeatureCSR.
            OFS_VENDOR_MASKING_GR_RAW_I         = 0x009C,       // RW   Masking GR.

            OFS_VENDOR_MASKING_GB_INT32B        = 0x00A0,       // RW   Masking GB FeatureCSR.
            OFS_VENDOR_MASKING_GB_RAW_I         = 0x00BC,       // RW   Masking GB.

            OFS_VENDOR_MASKING_BR_INT32B        = 0x00C0,       // RW   Masking BR FeatureCSR.
            OFS_VENDOR_MASKING_BR_RAW_I         = 0x00DC,       // RW   Masking BR.

            OFS_VENDOR_MASKING_BG_INT32B        = 0x00E0,       // RW   Masking BG FeatureCSR.
            OFS_VENDOR_MASKING_BG_RAW_I         = 0x00FC,       // RW   Masking BG.

            OFS_VENDOR_LED_LUMINANCE_INT32B     = 0x0100,       // RW   LED Indicator Luminance FeatureCSR.
            OFS_VENDOR_LED_LUMINANCE_RAW_I      = 0x011C,       // RW   LED Indicator Luminance.

            OFS_VENDOR_TEST_PATTERN_ENUM32B     = 0x0120,       // RW   Test pattern FeatureCSR. [g_saVAL_TestPtn]
            OFS_VENDOR_TEST_PATTERN_I           = 0x013C,       // RW   Test pattern. [g_saVAL_TestPtn]

            OFS_VENDOR_BINNING_H_INT32B         = 0x0140,       // RW   Binning Horizontal FeatureCSR.
            OFS_VENDOR_BINNING_H_RAW_I          = 0x015C,       // RW   Binning Horizontal.

            OFS_VENDOR_BINNING_V_INT32B         = 0x0160,       // RW   Binning Vertical FeatureCSR.
            OFS_VENDOR_BINNING_V_RAW_I          = 0x017C,       // RW   Binning Vertical.

            OFS_VENDOR_DECIMATION_H_INT32B      = 0x0180,       // RW   Desimation Horizontal FeatureCSR.
            OFS_VENDOR_DECIMATION_H_RAW_I       = 0x019C,       // RW   Desimation Horizontal.

            OFS_VENDOR_DECIMATION_V_INT32B      = 0x01A0,       // RW   Decimation Vertical FeatureCSR.
            OFS_VENDOR_DECIMATION_V_RAW_I       = 0x01BC,       // RW   Decimation Vertical.

            OFS_VENDOR_REVERSE_X_BULK20B        = 0x01C0,       // RW   Reverse X FeatureCSR. [g_saVAL_Reverse]
            OFS_VENDOR_REVERSE_X_I              = 0x01D0,       // RW   Reverse X. [g_saVAL_Reverse]

            OFS_VENDOR_REVERSE_Y_BULK20B        = 0x01E0,       // RW   Reverse Y FeatureCSR. [g_saVAL_Reverse]
            OFS_VENDOR_REVERSE_Y_I              = 0x01F0,       // RW   Reverse Y. [g_saVAL_Reverse]

            OFS_VENDOR_EVENT_ACQUIS_BULK20B     = 0x0200,       // RW   Event Notification Of Acquisition FeatureCSR. [g_saVAL_EventAcquis]
            OFS_VENDOR_EVENT_ACQUIS_I           = 0x0210,       // RW   Event Notification Of Acquisition. [g_saVAL_EventAcquis]

            OFS_VENDOR_EVENT_FRAME_BULK20B      = 0x0220,       // RW   Event Notification Of Frame FeatureCSR. [g_saVAL_EventFrame]
            OFS_VENDOR_EVENT_FRAME_I            = 0x0230,       // RW   Event Notification Of Frame. [g_saVAL_EventFrame]

            OFS_VENDOR_EVENT_EXPO_BULK20B       = 0x0240,       // RW   Event Notification Of Exposure FeatureCSR. [g_saVAL_EventExposure]
            OFS_VENDOR_EVENT_EXPO_I             = 0x0250,       // RW   Event Notification Of Exposure. [g_saVAL_EventExposure]

            OFS_VENDOR_TMER_TRG_SRC_ENUM32B     = 0x0260,       // RW   Timer Trigger Source FeatureCSR. [g_saVAL_TmTrgSrc]
            OFS_VENDOR_TMER_TRG_SRC_I           = 0x027C,       // RW   Timer Trigger Source. [g_saVAL_TmTrgSrc]

            OFS_VENDOR_DPC_ENABLE_ENUM32B       = 0x0280,       // RW   Defect pixel compensation enable FeatureCSR. [g_saVAL_OnOff]
            OFS_VENDOR_DPC_ENABLE_I             = 0x029C,       // RW   Defect pixel compensation enable. [g_saVAL_OnOff]

            OFS_VENDOR_DPC_NUMBER_INT32B        = 0x02A0,       // RW   Defect pixel compensation number FeatureCSR. 
            OFS_VENDOR_DPC_NUMBER_RAW_I         = 0x02BC,       // RW   Defect pixel compensation number. 

            OFS_VENDOR_DPC_COORD_ENUM32B        = 0x02C0,       // RW   Defect pixel compensation coordinates for introducing Category block 33 FeatureCSR.         #Appended V1.0.2.0
            OFS_VENDOR_DPCCOORD_EXPCATBLK_I     = 0x02C4,       // RO   Defect pixel compensation coordinates category block number[31..24], OffsetAddress[23..0].  #Appended V1.0.2.0

            OFS_VENDOR_USERSET_DEFAULT_Enum32B  = 0x02E0,       // RW   UserSet Default FeatureCSR. [g_saVAL_UsersetSel]                                        #Appended V1.0.2.0
            OFS_VENDOR_USERSET_DEFAULT_I        = 0x02FC,       // RW   UserSet Default.  [g_saVAL_UsersetSel]                                                  #Appended V1.0.2.0

            OFS_VENDOR_SEQ_SHT_ENABLE_ENUM32B   = 0x0300,       // RW   Sequential Shutter Enable FeatureCSR. [g_saVAL_OnOff]                                   #Appended V1.0.2.0
            OFS_VENDOR_SEQ_SHT_ENABLE_I         = 0x031C,       // RW   Sequential Shutter Enable. [g_saVAL_OnOff]                                              #Appended V1.0.2.0

            OFS_VENDOR_SEQ_SHT_TRMNAT_AT_INT32B = 0x0320,       // RW   Sequential Shutter Terminate at FeatureCSR.                                             #Appended V1.0.2.0
            OFS_VENDOR_SEQ_SHT_TRMNAT_AT_I      = 0x033C,       // RW   Sequential Shutter Terminate at.                                                        #Appended V1.0.2.0

            OFS_VENDOR_SEQ_SHT_SEQ_TBL_ENUM32B  = 0x0340,       // RW   Sequential Shutter Sequence Table FeatureCSR.                                           #Appended V1.0.2.0
            OFS_VENDOR_SEQ_SHT_SEQ_TBL_EXP_I    = 0x0344,       // RW   Sequential Shutter Sequence Table Offset for expanded CSR. (CB35 Offset 0x0020)         #Appended V1.0.2.0

            OFS_VENDOR_ALC_AREA_INT32B          = 0x0360,       // RW   ALC Area FeatureCSR.                                                                    #Appended V1.0.2.0
            OFS_VENDOR_ALC_AREA_I               = 0x037C,       // RW   ALC ARea.                                                                               #Appended V1.0.2.0

            OFS_VENDOR_EVENT_TMR_START_BULK20B  = 0x0380,       // RW   Event Notification Of Timer Start FeatureCSR. [g_saVAL_TimerSel]                        #Appended V1.0.2.0
            OFS_VENDOR_EVENT_TMR_START_I        = 0x0390,       // RW   Event Notification Of Timer Start. [g_saVAL_TimerSel]                                   #Appended V1.0.2.0

            OFS_VENDOR_EVENT_TMR_END_BULK20B    = 0x03A0,       // RW   Event Notification Of Timer End FeatureCSR. [g_saVAL_TimerSel.                          #Appended V1.0.2.0
            OFS_VENDOR_EVENT_TMR_END_I          = 0x03B0,       // RW   Event Notification Of Timer End. [g_saVAL_TimerSel.                                     #Appended V1.0.2.0

            //=================================================================
            // IIDC2 CategoryBlock 32.Expanded for Digital IO Control  (Defined by Teli)
            //=================================================================
            OFS_VENDOR_IOLINESOURCE_1_ENUM32B   = 0x0040,       // RW   IO Line Source 1 ExpandedCSR. [g_saVAL_IOLineSrc]                                       #Appended V1.0.2.0
            OFS_VENDOR_IOLINESOURCE_1_I         = 0x005C,       // RW   IO Line Source 1. [g_saVAL_IOLineSrc]                                                   #Appended V1.0.2.0

            OFS_VENDOR_IOLINESOURCE_2_ENUM32B   = 0x0060,       // RW   IO Line Source 2 ExpandedCSR. [g_saVAL_IOLineSrc]                                       #Appended V1.0.2.0
            OFS_VENDOR_IOLINESOURCE_2_I         = 0x007C,       // RW   IO Line Source 2. [g_saVAL_IOLineSrc]                                                   #Appended V1.0.2.0

            //=================================================================
            // IIDC2 CategoryBlock 33.Expanded for LUT   (Defined by Teli)
            //=================================================================
            OFS_VENDOR_LUTVALUE_AINT_STX        = 0x0020,       // RW   LUT ExpandedCSR.                                                                        #Appended V1.0.2.0
            OFS_VENDOR_LUTVALUE_NUM_CHANNEL_I   = 0x002C,       // RW   LUT Number of channel.                                                                  #Appended V1.0.2.0
            OFS_VENDOR_LUTVALUE_NUM_ELEMENT_I   = 0x0030,       // RW   LUT Number of Element.                                                                  #Appended V1.0.2.0
            OFS_VENDOR_LUTVALUE_VALUE_TOP_I     = 0x0044,       // RW   LUT Top of values.                                                                      #Appended V1.0.2.0

            
            //=================================================================
            // IIDC2 CategoryBlock 34.Expanded for DPC   (Defined by Teli)
            //=================================================================
            OFS_VENDOR_DPC_COORD_AINT_STX       = 0x0020,       // RW   Defect pixel compensation coordinates ExpandedCSR.                                      #Appended V1.0.2.0
            OFS_VENDOR_DPC_COORD_NUM_ELEMENT_I  = 0x002C,       // RW   DPC Number of Element.                                                                  #Appended V1.0.2.0
            OFS_VENDOR_DPC_COORD_NUM_POSITION_I = 0x0030,       // RW   DPC Number of Position.                                                                 #Appended V1.0.2.0
            OFS_VENDOR_DPCVALUE_VALUE_TOP_I     = 0x0044,       // RW   DPC Top of values.                                                                      #Appended V1.0.2.0

            //=================================================================
            // IIDC2 CategoryBlock 35.Expanded for Sequential Shutter   (Defined by Teli)
            //=================================================================
            OFS_VENDOR_SEQSHT_SEQTABLE_AINT_STX = 0x0020,       // RW   Sequential Shutter Seqquence table ExpandedCSR.                                         #Appended V1.0.2.0
            OFS_VENDOR_SEQTABLE_NUM_ELEMENT_I   = 0x002C,       // RW   Seqquence table Number of Element.                                                      #Appended V1.0.2.0
            OFS_VENDOR_SEQTABLE_VALUE_TOP_I     = 0x0040,       // RW   Seqquence table Top of values.                                                          #Appended V1.0.2.0
        };


        //================================================================
        // Enumeration type register data  of BU series
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

        // ================================================================
        // Structure for defing register Data
        // ================================================================
        typedef struct _REGDATA
        {
            uint32_t    ValInt;             // Value in integer
            char        *Name;              // Value name

            // construuctor
            _REGDATA()
            {
                ValInt      = 0;
                Name        = (char*)"";
            };

            // construuctor
            _REGDATA(uint32_t value, char* name)
            {
                ValInt  = value;
                Name    = name;
            };
        } REGDATA;




        //=================================================================
        // Endianess in ABRM 
        //=================================================================
        enum REGVAL_ENDIANESS
        {
            RV_ENDIAN_BIG_ENDIAN    = 0,                    // Big Endian
            RV_ENDIAN_LITTLE_ENDIAN = 0xFFFFFFFF            // Little Endian
        };
        const REGDATA g_sREGV_Endian_Big    = REGDATA( RV_ENDIAN_BIG_ENDIAN,    (char*)"BigEndian");
        const REGDATA g_sREGV_Endian_Little = REGDATA( RV_ENDIAN_LITTLE_ENDIAN, (char*)"LittleEndian");

        const REGDATA g_saVAL_Endian[] =
        {   
            g_sREGV_Endian_Big,
            g_sREGV_Endian_Little,
        };

        //=================================================================
        // File type used in FileType member of manifest table. 
        //=================================================================
        enum REGVAL_GFTYPE
        {
            RV_GFTYPE_XML = 0,
            RV_GFTYPE_ZIP = 1024,
        };

        const REGDATA g_sREGV_GFType_XML    = REGDATA( RV_GFTYPE_XML,   (char*)"XML");
        const REGDATA g_sREGV_GFType_ZIP    = REGDATA( RV_GFTYPE_ZIP,   (char*)"ZIP");

        const REGDATA g_saVAL_GFType[] =
        {   
            g_sREGV_GFType_XML,
            g_sREGV_GFType_ZIP,
        };


        //=================================================================
        //Bit value of device's U3V features and capabilities.
        //=================================================================
        enum REGVAL_U3VCP_CAPA
        {
            RV_U3VCPCAPA_SIRM_AVAILABLE     = 1,                // If corresponding bit is set, the device supports at least one device streaming interface.
            RV_U3VCPCAPA_EIRM_AVAILABLE     = 2,                // If corresponding bit is set, the device supports at least one device event interface.
            RV_U3VCPCAPA_IIDC2_AVAILABLE    = 4,                // If corresponding bit is set, the device supports IIDC2 register map.
        };

        const REGDATA g_sREGV_U3VCPCAPA_SIRM    = REGDATA( RV_U3VCPCAPA_SIRM_AVAILABLE,     (char*)"SIRMAvailable");
        const REGDATA g_sREGV_U3VCPCAPA_EIRM    = REGDATA( RV_U3VCPCAPA_EIRM_AVAILABLE,     (char*)"EIRMAvailable");
        const REGDATA g_sREGV_U3VCPCAPA_IIDC2   = REGDATA( RV_U3VCPCAPA_IIDC2_AVAILABLE,    (char*)"IIDC2Available");

        const REGDATA g_saVAL_U3VCPCAPA[] =
        {   
            g_sREGV_U3VCPCAPA_SIRM,
            g_sREGV_U3VCPCAPA_EIRM,
            g_sREGV_U3VCPCAPA_IIDC2,
        };


        //=================================================================
        //Bit value of CurrentSpeed flag (member of SBRM)
        //=================================================================
        enum REGVAL_USB_SPEED
        {
            RV_USBSPEED_LOW     = 1,                // If corresponding bit is set, the device supports low speed (1.5Mbps).
            RV_USBSPEED_FULL    = 2,                // If corresponding bit is set, the device supports full speed (12Mbps).
            RV_USBSPEED_HIGH    = 4,                // If corresponding bit is set, the device supports high speed (480Mbps).
            RV_USBSPEED_SUPER   = 8,                // If corresponding bit is set, the device supports super speed (4.8Gbps).
        };

        const REGDATA g_sREGV_USBSpeed_Low      = REGDATA( RV_USBSPEED_LOW,     (char*)"LowSpeed");
        const REGDATA g_sREGV_USBSpeed_Full     = REGDATA( RV_USBSPEED_FULL,    (char*)"FullSpeed");
        const REGDATA g_sREGV_USBSpeed_High     = REGDATA( RV_USBSPEED_HIGH,    (char*)"HighwSpeed");
        const REGDATA g_sREGV_USBSpeed_Super    = REGDATA( RV_USBSPEED_SUPER,   (char*)"SuperSpeed");

        const REGDATA g_saVAL_USBSpeed[] =
        {   
            g_sREGV_USBSpeed_Low,
            g_sREGV_USBSpeed_Full,
            g_sREGV_USBSpeed_High,
            g_sREGV_USBSpeed_Super,
        };


        //=================================================================
        // Stream interface information (Payload alignment size)
        //=================================================================
        enum REGVAL_ALIGN_PAYLOAD
        {
            RV_ALIGN_PAYLOAD_00001      = 0x00000000,
            RV_ALIGN_PAYLOAD_00002      = 0x01000000,
            RV_ALIGN_PAYLOAD_00004      = 0x02000000,
            RV_ALIGN_PAYLOAD_00008      = 0x03000000,
            RV_ALIGN_PAYLOAD_00016      = 0x04000000,
            RV_ALIGN_PAYLOAD_00032      = 0x05000000,
            RV_ALIGN_PAYLOAD_00064      = 0x06000000,
            RV_ALIGN_PAYLOAD_00128      = 0x07000000,
            RV_ALIGN_PAYLOAD_00256      = 0x08000000,
            RV_ALIGN_PAYLOAD_00512      = 0x09000000,
            RV_ALIGN_PAYLOAD_01024      = 0x0A000000,
            RV_ALIGN_PAYLOAD_02048      = 0x0B000000,
            RV_ALIGN_PAYLOAD_04096      = 0x0C000000,
            RV_ALIGN_PAYLOAD_08192      = 0x0D000000,
            RV_ALIGN_PAYLOAD_16384      = 0x0E000000,
            RV_ALIGN_PAYLOAD_32768      = 0x0F000000,
        };

        const REGDATA g_sREGV_Align_Payload_00001 = REGDATA( RV_ALIGN_PAYLOAD_00001, (char*)"Align00001");
        const REGDATA g_sREGV_Align_Payload_00002 = REGDATA( RV_ALIGN_PAYLOAD_00002, (char*)"Align00002");
        const REGDATA g_sREGV_Align_Payload_00004 = REGDATA( RV_ALIGN_PAYLOAD_00004, (char*)"Align00004");
        const REGDATA g_sREGV_Align_Payload_00008 = REGDATA( RV_ALIGN_PAYLOAD_00008, (char*)"Align00008");
        const REGDATA g_sREGV_Align_Payload_00016 = REGDATA( RV_ALIGN_PAYLOAD_00016, (char*)"Align00016");
        const REGDATA g_sREGV_Align_Payload_00032 = REGDATA( RV_ALIGN_PAYLOAD_00032, (char*)"Align00032");
        const REGDATA g_sREGV_Align_Payload_00064 = REGDATA( RV_ALIGN_PAYLOAD_00064, (char*)"Align00064");
        const REGDATA g_sREGV_Align_Payload_00128 = REGDATA( RV_ALIGN_PAYLOAD_00128, (char*)"Align00128");
        const REGDATA g_sREGV_Align_Payload_00256 = REGDATA( RV_ALIGN_PAYLOAD_00256, (char*)"Align00256");
        const REGDATA g_sREGV_Align_Payload_00512 = REGDATA( RV_ALIGN_PAYLOAD_00512, (char*)"Align00512");
        const REGDATA g_sREGV_Align_Payload_01024 = REGDATA( RV_ALIGN_PAYLOAD_01024, (char*)"Align01024");
        //const REGDATA g_sREGV_Align_Payload_02048 = REGDATA( RV_ALIGN_PAYLOAD_02048, (char*)"Align03048");        //#Amended V1.0.2.0.
        const REGDATA g_sREGV_Align_Payload_02048 = REGDATA( RV_ALIGN_PAYLOAD_02048, (char*)"Align02048");          //#Amended V1.0.2.0.
        const REGDATA g_sREGV_Align_Payload_04096 = REGDATA( RV_ALIGN_PAYLOAD_04096, (char*)"Align04096");
        const REGDATA g_sREGV_Align_Payload_08192 = REGDATA( RV_ALIGN_PAYLOAD_08192, (char*)"Align08192");
        const REGDATA g_sREGV_Align_Payload_16384 = REGDATA( RV_ALIGN_PAYLOAD_16384, (char*)"Align16384");
        const REGDATA g_sREGV_Align_Payload_32768 = REGDATA( RV_ALIGN_PAYLOAD_32768, (char*)"Align32768");

        const REGDATA g_saVAL_Align_Payload[] =
        {   
            g_sREGV_Align_Payload_00001,
            g_sREGV_Align_Payload_00002,
            g_sREGV_Align_Payload_00004,
            g_sREGV_Align_Payload_00008,
            g_sREGV_Align_Payload_00016,
            g_sREGV_Align_Payload_00032,
            g_sREGV_Align_Payload_00064,
            g_sREGV_Align_Payload_00128,
            g_sREGV_Align_Payload_00256,
            g_sREGV_Align_Payload_00512,
            g_sREGV_Align_Payload_01024,
            g_sREGV_Align_Payload_02048,
            g_sREGV_Align_Payload_04096,
            g_sREGV_Align_Payload_08192,
            g_sREGV_Align_Payload_16384,
            g_sREGV_Align_Payload_32768,
        };


        //=================================================================
        // Enable /disable streaming
        //=================================================================
        enum REGVAL_SI_CONTROL
        {
            RV_SI_CONTROL_DISABLE   = 0,            // Streaming is disabled. Switching to "0" while a Stream is active will abort the stream.
            RV_SI_CONTROL_ENABLE    = 1,            // Streaming is enabled.
        };

        const REGDATA g_sREGV_SIContol_Disable  = REGDATA(RV_SI_CONTROL_DISABLE, (char*)"DisableStream");
        const REGDATA g_sREGV_SIContol_Enable   = REGDATA(RV_SI_CONTROL_DISABLE, (char*)"EnableStream");

        const REGDATA g_saVAL_SIControl[] =
        {   
            g_sREGV_SIContol_Disable,
            g_sREGV_SIContol_Enable,
        };


        //=================================================================
        // Enable /disable event
        //=================================================================
        enum REGVAL_EI_CONTROL
        {
            RV_EI_CONTROL_DISABLE   = 0,            // Event is disabled.
            RV_EI_CONTROL_ENABLE    = 1,            // Event is enabled.
        };

        const REGDATA g_sREGV_EIContol_Disable  = REGDATA(RV_EI_CONTROL_DISABLE, (char*)"DisableEvent");
        const REGDATA g_sREGV_EIContol_Enable   = REGDATA(RV_EI_CONTROL_DISABLE, (char*)"EnableEvent");

        const REGDATA g_saVAL_EIControl[] =
        {   
            g_sREGV_EIContol_Disable,
            g_sREGV_EIContol_Enable,
        };


        //================================================================
        // Type of FeatureCSR Value. Not prepare string information because registers using these value are read only. 
        //================================================================
        enum _FCSRTYPE_IIDC
        {
            FCSRTYPE_INTEGER32              = 0x30,             // Integer32  (0x30)
            FCSRTYPE_INTEGER64              = 0x40,             // Integer64  (0x40)

            FCSRTYPE_PLAIN_INTEGER8         = 0x11,             // PlainInteger8  (0x11)
            FCSRTYPE_PLAIN_INTEGER32        = 0x31,             // PlainInteger32  (0x31)
            FCSRTYPE_PLAIN_INTEGER64        = 0x41,             // PlainInteger64  (0X41)
        
            FCSRTYPE_FLOAT32                = 0x32,             // Float32  (0x32)
            FCSRTYPE_FLOAT64                = 0x42,             // Float64  (0x42)
        
            FCSRTYPE_ENUMERATION            = 0x03,             // Enumeration  (0x03)
        
            FCSRTYPE_BULK_BOOLEAN32         = 0x34,             // BulkBoolean32  (0x34)
            FCSRTYPE_BULK_BOOLEAN64         = 0x44,             // BulkBoolean64  (0x44)
        
            FCSRTYPE_RECTANGLE32            = 0x35,             // Rectangle32  (0x35)
        
            FCSRTYPE_INTEGER32ARRAY         = 0x30 + 0x80,      // Array of Integer32  (0xB0)
            FCSRTYPE_INTEGER64ARRAY         = 0x40 + 0x80,      // Array of Integer64  (0xC0)
        
//          FCSRTYPE_PLAIN_INT8ARRAY        = 0x10 + 0x80,      // Array of PlainInteger8  (0x91))
            FCSRTYPE_PLAIN_INT8ARRAY        = 0x11 + 0x80,      // Array of PlainInteger8  (0x91))  // Amended 2015/04/07
            FCSRTYPE_PLAIN_INT32ARRAY       = 0x31 + 0x80,      // Array of PlainInteger32  (0xB1)
            FCSRTYPE_PLAIN_INT64ARRAY       = 0x41 + 0x80,      // Array of PlainInteger64  (0xC1)
        
            FCSRTYPE_FLOAT32ARRAY           = 0x32 + 0x80,      // Array of Float32  (0xB2)
            FCSRTYPE_FLOAT64ARRAY           = 0x42 + 0x80,      // Array of Float64  (0xC2)
        
            FCSRTYPE_ENUMARRAY              = 0x03 + 0x80,      // Array of Enumeration  (0X83)
        
            FCSRTYPE_BULK_BOOL32ARRAY       = 0x34 + 0x80,      // Array of BulkBoolean32  (0XB4)
            FCSRTYPE_BULK_BOOL64ARRAY       = 0x44 + 0x80,      // Array of BulkBoolean64  (0XC4)
        
            FCSRTYPE_RECTANGLE32ARRAY       = 0x35 + 0x80,      // Array of Rectangle32  (0XB5)
        
            FCSRTYPE_ZERO                   = 0x0,              // Prepared for clearing cached data. This value does not exist in IIDC2 Stabdard.
        };
        typedef byte FCSRTYPE_IIDC;


        //================================================================
        // Inquiry for Control.  Not prepare string information because registers using these value are read only. 
        //================================================================
        enum _CNTRL_INQ_IIDC
        {
            CNTRL_INQ_BIT_NO_SPECIFY        = 0x01,             // If corresponding bit is set, the device supports "NoSpecify" mode (sets feature value by its own control).  (0x01)
            CNTRL_INQ_BIT_MANUAL            = 0x02,             // If corresponding bit is set, the device supports "Manual" mode (host can control this feature value).  (0x02)
            CNTRL_INQ_BIT_AUTO              = 0x04,             // If corresponding bit is set, the device supports "Auto" mode (the device adjusts this feature value automatically).  (0x04)
            CNTRL_INQ_BIT_AUTO_ONCE         = 0x08,             // If corresponding bit is set, the device supports "AutoOnce" mode (the device adjusts this feature value automatically once).  (0x08)
            CNTRL_INQ_BIT_DEFAULT           = 0x10,             // If corresponding bit is set, the device supports "Default" mode (the device set Value and Control registers to Initial state).  (0x10)
            CNTRL_INQ_ZERO                  = 0x00,             // All flag is 0. Prepared for clearing cached data, This value does not exist in IIDC2 Standard.
        };
        typedef uint16_t CNTRL_INQ_IIDC;

        //================================================================
        // Inquiry for Feature.  Not prepare string information because registers using these value are read only. 
        //================================================================
        enum _FEATINQ_IIDC
        {
            FEATINQ_BIT_READABLE            = 0x01,             // If set, Value register is readable.  (0x01)
            FEATINQ_BIT_WRITABLE            = 0x02,             // If set, Value register is writable.  (0x02)
            FEATINQ_BIT_USERSET_LOADABLE    = 0x04,             // If set, this feature is included in userset data.  (0x04)
            FEATINQ_BIT_ACTIVE              = 0x40,             // If set, this feature is activated. (0x40)
            FEATINQ_BIT_IMPLEMENTED         = 0x80,             // If set, this feature is implemented. (0x80)
            FEATINQ_ZERO                    = 0x00,             // All flag is 0. Prepared for clearing cached data. This value does not exist in IIDC2 Standard.
        };
        typedef byte FEATINQ_IIDC;


        //================================================================
        // Control register
        //================================================================
        enum _FEAT_CNTRL_IIDC
        {
            FEATCNT_NOSPECIFY               = 0,                // The device sets feature value by its own control. (0)
            FEATCNT_MANUAL                  = 1,                // Host can control feature value. (1)
            FEATCNT_AUTO                    = 2,                // The device adjusts feature value automatically. (2)
            FEATCNT_AUTO_ONCE               = 3,                // The device adjusts feature value automatically once. (3)
            FEATCNT_DEFAULT                 = 4,                // The device set Value and Control registers to Initial state. (4)
        };
        typedef uint32_t FEAT_CNTRL_IIDC;
    
    
        const REGDATA g_sREGV_FeatCntrl_NoSpecify   = REGDATA(FEATCNT_NOSPECIFY,    (char*)"NoSpecify");
        const REGDATA g_sREGV_FeatCntrl_Manual      = REGDATA(FEATCNT_MANUAL,       (char*)"Manual");
        const REGDATA g_sREGV_FeatCntrl_Auto        = REGDATA(FEATCNT_AUTO,         (char*)"Auto");
        const REGDATA g_sREGV_FeatCntrl_Auto_Once   = REGDATA(FEATCNT_AUTO_ONCE,    (char*)"AutoOnce");
        const REGDATA g_sREGV_FeatCntrl_Default     = REGDATA(FEATCNT_DEFAULT,      (char*)"Default");

        const REGDATA g_saVALFeatCntrl[] =
        { 
            g_sREGV_FeatCntrl_NoSpecify,
            g_sREGV_FeatCntrl_Manual,
            g_sREGV_FeatCntrl_Auto,
            g_sREGV_FeatCntrl_Auto_Once,
            g_sREGV_FeatCntrl_Default,
        };

    
    
    
        //=================================================================
        // On/Off (category common)
        //=================================================================
        enum  REGVAL_CATC_ONOFF
        {
            RV_CATC_ONOFF_OFF   = 0,                            // Off  (0)
            RV_CATC_ONOFF_ON    = 1,                            // On   (1)
        };

        const REGDATA g_sREGV_CATC_OnOff_Off    = REGDATA(RV_CATC_ONOFF_OFF,    (char*)"Off");
        const REGDATA g_sREGV_CATC_OnOff_On     = REGDATA(RV_CATC_ONOFF_ON,     (char*)"On");

        const REGDATA g_saVAL_OnOff[] =
        {
            g_sREGV_CATC_OnOff_Off,
            g_sREGV_CATC_OnOff_On,
        };



        //=================================================================
        // Device reset
        //=================================================================
        enum REGVAL_CAT0_DEVRESET
        {
            RV_CAT0_DEVRESET_OFF    = 0,            // Off.
            RV_CAT0_DEVRESET_RESET  = 1,            // Reset.
        };

        const REGDATA g_sREGV_CAT0_DEVReset_Off     = REGDATA(RV_CAT0_DEVRESET_OFF,     (char*)"Off");
        const REGDATA g_sREGV_CAT0_DevReset_Reset   = REGDATA(RV_CAT0_DEVRESET_RESET,   (char*)"Reset");

        const REGDATA g_saVAL_DevReset[] =
        {   
            g_sREGV_CAT0_DEVReset_Off,
            g_sREGV_CAT0_DevReset_Reset,
        };

        //=================================================================
        // Image format selector
        //=================================================================
        enum  REGVAL_IMGFMT_SEL
        {
            RV_IMGFMT_FORMAT_SEL_0      = 0,        // Format0 Scalable mode. (0)
            RV_IMGFMT_FORMAT_SEL_1      = 1,        // Format1 Binning mode. (1)
            RV_IMGFMT_FORMAT_SEL_2      = 2,        // Format2 Draft mode. (2)
        };

        const REGDATA g_sREGV_ImgFmt_Format_Sel_0   = REGDATA(RV_IMGFMT_FORMAT_SEL_0,   (char*)"Format0");
        const REGDATA g_sREGV_ImgFmt_Format_Sel_1   = REGDATA(RV_IMGFMT_FORMAT_SEL_1,   (char*)"Format1");
        const REGDATA g_sREGV_ImgFmt_Format_Sel_2   = REGDATA(RV_IMGFMT_FORMAT_SEL_2,   (char*)"Format2");

        const REGDATA g_saVAL_FormatSel[] =
        {   
            g_sREGV_ImgFmt_Format_Sel_0,
            g_sREGV_ImgFmt_Format_Sel_1,
            g_sREGV_ImgFmt_Format_Sel_2,
        };

        //=================================================================
        // Value of ApplyImageFormat register
        //=================================================================
        enum  REGVAL_APPLY_IMAGE_FORMAT
        {
            RV_IMGFMT_APPLY_IMGFMT_DONE     = 0,        // Normal state  (0)
            RV_IMGFMT_APPLY_IMGFMT_APPLY    = 1,        // Command to apply image format (1)
            RV_IMGFMT_APPLY_IMGFMT_CHANGED  = 8,        // Image format applied (8)
            RV_IMGFMT_APPLY_IMGFMT_ERROR    = 16        // Error (16)
        };

        const REGDATA g_sREGV_ImgFmt_Apply_ImgFmt_Done      = REGDATA(RV_IMGFMT_APPLY_IMGFMT_DONE,      (char*)"Done");
        const REGDATA g_sREGV_ImgFmt_Apply_ImgFmt_Apply     = REGDATA(RV_IMGFMT_APPLY_IMGFMT_APPLY,     (char*)"Apply");
        const REGDATA g_sREGV_ImgFmt_Apply_ImgFmt_Changed   = REGDATA(RV_IMGFMT_APPLY_IMGFMT_CHANGED,   (char*)"Changed");
        const REGDATA g_sREGV_ImgFmt_Apply_ImgFmt_Error     = REGDATA(RV_IMGFMT_APPLY_IMGFMT_ERROR,     (char*)"Error");

        const REGDATA g_saVAL_ApplyImgFmt[] =
        {   
            g_sREGV_ImgFmt_Apply_ImgFmt_Done,
            g_sREGV_ImgFmt_Apply_ImgFmt_Apply,
            g_sREGV_ImgFmt_Apply_ImgFmt_Changed,
            g_sREGV_ImgFmt_Apply_ImgFmt_Error,
        };


        //=================================================================
        // PixelCoding value
        // Host must change PixelCoding value before PixelSize value, onChanging Pixel Format.
        //=================================================================
        enum  REGVAL_PIXEL_CODING
        {
            RV_IMGFMT_MONO          =  0,               // Mono (value:0). Bpp8->Mono8 pixelformat,  Bpp16->ono16 pixel format.
            RV_IMGFMT_MONOSIGNED    =  1,               // MonoSigned (value:1). Bpp16->MonoSigned16 pixel format. BU Series does not support this value.
            RV_IMGFMT_MONOPACKED    =  2,               // MonoPacked (value:2). Bpp12->Mono12Packed pixel format. BU Series does not support this value.
            RV_IMGFMT_RGB           = 32,               // RGB16 (32). Bpp48->RGB16 pixelFormat. BU Series does not support this value.
            RV_IMGFMT_RGBSIGNED     = 33,               // RGBSigned (33). Bpp48->RGBSigned16 pixelFormat. BU Series does not support this value.
            RV_IMGFMT_RGBPACKED     = 34,               // RGBPacked (34). Bpp24->RGB8Packed pixel format.
            RV_IMGFMT_DEPLICATED64  = 64,               // This coding is deplicated.
            RV_IMGFMT_YUV411PACKED  = 66,               // YUV411Packed (66). Bpp12->YUV411Packed pixel format [U, Y0, Y1, V, Y2, Y3],
            RV_IMGFMT_DEPLICATED72  = 72,               // This coding is deplicated.
            RV_IMGFMT_YUV422PACKED  = 74,               // YUV422Packed (74). Bpp16->YUV422Packed pixel format [U, Y0, V, Y1].
            RV_IMGFMT_YUV444PACKED  = 82,               // YUV444Packed (82). Bpp24->YUV444Packed pixel format [U, Y, V].
            RV_IMGFMT_BAYER_GR      = 96,               // BayerGR (96). Bpp8->BayerGR8 pixel format, Bpp16->BayerGR16 pixel format.
            RV_IMGFMT_BAYER_GR_PKD  = 98,               // BayerGRPacked (98). Bpp12->BayerGR12Packed. BU Series does not support this value.
            RV_IMGFMT_BAYER_RG      = 99,               // BayerRG (99). Bpp8->BayerRG8 pixel format, Bpp16->BayerRG16 pixel format.
            RV_IMGFMT_BAYER_RG_PKD  = 101,              // BayerRGPacked (101). Bpp12->BayerRG12Packed. BU Series does not support this value.
            RV_IMGFMT_BAYER_GB      = 102,              // BayerGB (102). Bpp8->BayerGB8 pixel format, Bpp16->BayerGB16 pixel format.
            RV_IMGFMT_BAYER_GB_PKD  = 104,              // BayerGBPacked (104). Bpp12->BayerGB12Packed. BU Series does not support this value.
            RV_IMGFMT_BAYER_BG      = 105,              // BayerBG (105). Bpp8->BayerBG8 pixel format, Bpp16->BayerBG16 pixel format.
            RV_IMGFMT_BAYER_BG_PKD  = 107,              // BayerBGPacked (107). Bpp12->BayerBG12Packed. BU Series does not support this value.
        };

        const REGDATA g_sREGV_ImgFmt_Mono               = REGDATA(RV_IMGFMT_MONO,           (char*)"Mono");
        const REGDATA g_sREGV_ImgFmt_MonoSigned         = REGDATA(RV_IMGFMT_MONOSIGNED,     (char*)"MonoSigned");
        const REGDATA g_sREGV_ImgFmt_MonoPacked         = REGDATA(RV_IMGFMT_MONOPACKED,     (char*)"MonoPacked");
        const REGDATA g_sREGV_ImgFmt_RGB                = REGDATA(RV_IMGFMT_RGB,            (char*)"RGB");
        const REGDATA g_sREGV_ImgFmt_RGBSigned          = REGDATA(RV_IMGFMT_RGBSIGNED,      (char*)"RGBSigned");
        const REGDATA g_sREGV_ImgFmt_RGBPacked          = REGDATA(RV_IMGFMT_RGBPACKED,      (char*)"RGBPacked");
        const REGDATA g_sREGV_ImgFmt_Deplicated64       = REGDATA(RV_IMGFMT_DEPLICATED64,   (char*)"Deplicated64");
        const REGDATA g_sREGV_ImgFmt_YUV411Packed       = REGDATA(RV_IMGFMT_YUV411PACKED,   (char*)"YUV411Packed");
        const REGDATA g_sREGV_ImgFmt_Deplicated72       = REGDATA(RV_IMGFMT_DEPLICATED64,   (char*)"Deplicated72");
        const REGDATA g_sREGV_ImgFmt_YUV422Packed       = REGDATA(RV_IMGFMT_YUV422PACKED,   (char*)"YUV422Packed");
        const REGDATA g_sREGV_ImgFmt_YUV444Packed       = REGDATA(RV_IMGFMT_YUV444PACKED,   (char*)"YUV444Packed");
        const REGDATA g_sREGV_ImgFmt_Bayer_GR           = REGDATA(RV_IMGFMT_BAYER_GR,       (char*)"BayerGR");
        const REGDATA g_sREGV_ImgFmt_Bayer_GR_Packed    = REGDATA(RV_IMGFMT_BAYER_GR_PKD,   (char*)"BayerGRPacked");
        const REGDATA g_sREGV_ImgFmt_Bayer_RG           = REGDATA(RV_IMGFMT_BAYER_RG,       (char*)"BayerRG");
        const REGDATA g_sREGV_ImgFmt_Bayer_RG_Packed    = REGDATA(RV_IMGFMT_BAYER_RG_PKD,   (char*)"BayerRGPacked");
        const REGDATA g_sREGV_ImgFmt_Bayer_GB           = REGDATA(RV_IMGFMT_BAYER_GB,       (char*)"BayerGB");
        const REGDATA g_sREGV_ImgFmt_Bayer_GB_Packed    = REGDATA(RV_IMGFMT_BAYER_GB_PKD,   (char*)"BayerGBPacked");
        const REGDATA g_sREGV_ImgFmt_Bayer_BG           = REGDATA(RV_IMGFMT_BAYER_BG,       (char*)"BayerBG");
        const REGDATA g_sREGV_ImgFmt_Bayer_BG_Packed    = REGDATA(RV_IMGFMT_BAYER_BG_PKD,   (char*)"BayerBGPacked");

        const REGDATA g_saVAL_PixelCoding[] =
        {   
            g_sREGV_ImgFmt_Mono,
            g_sREGV_ImgFmt_MonoSigned,
            g_sREGV_ImgFmt_MonoPacked,
            g_sREGV_ImgFmt_RGB,
            g_sREGV_ImgFmt_RGBSigned,
            g_sREGV_ImgFmt_RGBPacked,
            g_sREGV_ImgFmt_Deplicated64,
            g_sREGV_ImgFmt_YUV411Packed,
            g_sREGV_ImgFmt_Deplicated72,
            g_sREGV_ImgFmt_YUV422Packed,
            g_sREGV_ImgFmt_YUV444Packed,
            g_sREGV_ImgFmt_Bayer_GR,
            g_sREGV_ImgFmt_Bayer_GR_Packed,
            g_sREGV_ImgFmt_Bayer_RG,
            g_sREGV_ImgFmt_Bayer_RG_Packed,
            g_sREGV_ImgFmt_Bayer_GB,
            g_sREGV_ImgFmt_Bayer_GB_Packed,
            g_sREGV_ImgFmt_Bayer_BG,
            g_sREGV_ImgFmt_Bayer_BG_Packed,
        };


        //=================================================================
        /// PixelSize Value
        /// Host must change PixelCoding value before PixelSize value, onChanging Pixel Format.
        //=================================================================
        enum  REGVAL_PIXEL_SIZE
        {
            RV_IMGFMT_BPP8   = 8,                   //  8bit.
            RV_IMGFMT_BPP10  = 10,                  // 10bit.
            RV_IMGFMT_BPP12  = 12,                  // 12bit.
            RV_IMGFMT_BPP16  = 16,                  // 16bit.
            RV_IMGFMT_BPP24  = 24,                  // 24bit.(8bit x 3 components)
            RV_IMGFMT_BPP30  = 30,                  // 30bit.(10bit x 3 components) 
            RV_IMGFMT_BPP36  = 36,                  // 36bit.(12bit x 3 components)
            RV_IMGFMT_BPP48  = 48,                  // 48bit. (16bit x 3 components)
        };

        const REGDATA g_sREGV_ImgFmt_Bpp8   = REGDATA(RV_IMGFMT_BPP8,   (char*)"Bpp8");
        const REGDATA g_sREGV_ImgFmt_Bpp10  = REGDATA(RV_IMGFMT_BPP10,  (char*)"Bpp10");
        const REGDATA g_sREGV_ImgFmt_Bpp12  = REGDATA(RV_IMGFMT_BPP12,  (char*)"Bpp12");
        const REGDATA g_sREGV_ImgFmt_Bpp16  = REGDATA(RV_IMGFMT_BPP16,  (char*)"Bpp16");
        const REGDATA g_sREGV_ImgFmt_Bpp24  = REGDATA(RV_IMGFMT_BPP24,  (char*)"Bpp24");
        const REGDATA g_sREGV_ImgFmt_Bpp30  = REGDATA(RV_IMGFMT_BPP30,  (char*)"Bpp30");
        const REGDATA g_sREGV_ImgFmt_Bpp36  = REGDATA(RV_IMGFMT_BPP36,  (char*)"Bpp36");
        const REGDATA g_sREGV_ImgFmt_Bpp48  = REGDATA(RV_IMGFMT_BPP48,  (char*)"Bpp48");

        const REGDATA g_saVAL_PixelSize[] =
        {
            g_sREGV_ImgFmt_Bpp8,
            g_sREGV_ImgFmt_Bpp10,
            g_sREGV_ImgFmt_Bpp12,
            g_sREGV_ImgFmt_Bpp16,
            g_sREGV_ImgFmt_Bpp24,
            g_sREGV_ImgFmt_Bpp30,
            g_sREGV_ImgFmt_Bpp36,
            g_sREGV_ImgFmt_Bpp48,
        };

        //=================================================================
        // PixelEndian 
        //=================================================================
        enum  REGVAL_PIXEL_ENDIAN
        {
            RV_IMGFMT_BIG_ENDIAN    = 0,            //  Big endianess. (0)
            RV_IMGFMT_LITTLE_ENDIAN = 1,            //  Little endianess. (1)
        };

        const REGDATA g_sREGV_ImgFmt_BigEndian      = REGDATA(RV_IMGFMT_BIG_ENDIAN,     (char*)"BigEndian");
        const REGDATA g_sREGV_ImgFmt_LittleEndian   = REGDATA(RV_IMGFMT_LITTLE_ENDIAN,  (char*)"LittleEndian");

        const REGDATA g_saVAL_PixelEndian[] =
        {
            g_sREGV_ImgFmt_BigEndian,
            g_sREGV_ImgFmt_LittleEndian,
        };


        //=================================================================
        // AcquisitionCommand
        //=================================================================
        enum  REGVAL_ACQUIS_COMMAND
        {
            RV_ACQ_ACQUIS_CMD_ABORT         = 0,        // Abort acquisition (0)
            RV_ACQ_ACQUIS_CMD_STOP          = 1,        // Stop acquisition (1)
            RV_ACQ_ACQUIS_CMD_CONTINUOUS    = 8,        // Start continuoue acquisition (8)
            RV_ACQ_ACQUIS_CMD_MULTI_FRAME   = 9,        // Start Multi-frame acquisition (9)
            RV_ACQ_ACQUIS_CMD_IMGBUF_READ   = 10,       // Read image in image buffer (10)
            RV_ACQ_ACQUIS_CMD_RETRANSMIT    = 18,       // Transmit current image again (16)
        };

        const REGDATA g_sREGV_Acq_AcqCmd_Abort      = REGDATA(RV_ACQ_ACQUIS_CMD_ABORT,          (char*)"Abort");
        const REGDATA g_sREGV_Acq_AcqCmd_Stop       = REGDATA(RV_ACQ_ACQUIS_CMD_STOP,           (char*)"Stop");
        const REGDATA g_sREGV_Acq_AcqCmd_Continuous = REGDATA(RV_ACQ_ACQUIS_CMD_CONTINUOUS,     (char*)"Continuous");
        const REGDATA g_sREGV_Acq_AcqCmd_MultiFrame = REGDATA(RV_ACQ_ACQUIS_CMD_MULTI_FRAME,    (char*)"MultiFrame");
        const REGDATA g_sREGV_Acq_AcqCmd_ImgBufRead = REGDATA(RV_ACQ_ACQUIS_CMD_IMGBUF_READ,    (char*)"ImageBufferRead");
        const REGDATA g_sREGV_Acq_AcqCmd_ReTransmit = REGDATA(RV_ACQ_ACQUIS_CMD_RETRANSMIT,     (char*)"ReTransmit");

        const REGDATA g_saVAL_AcquisCmd[] =
        {
            g_sREGV_Acq_AcqCmd_Abort,
            g_sREGV_Acq_AcqCmd_Stop,
            g_sREGV_Acq_AcqCmd_Continuous,
            g_sREGV_Acq_AcqCmd_MultiFrame,
            g_sREGV_Acq_AcqCmd_ImgBufRead,
            g_sREGV_Acq_AcqCmd_ReTransmit,
        };

        //=================================================================
        // LUT Bank Selector
        //=================================================================
        enum  REGVAL_LUT_BANK_SEL
        {
            RV_LUT_BANK_00  = 0,                        // Select Bank0
            RV_LUT_BANK_01  = 1,                        // Select Bank1
            RV_LUT_BANK_02  = 2,                        // Select Bank2
            RV_LUT_BANK_03  = 3,                        // Select Bank3
            RV_LUT_BANK_04  = 4,                        // Select Bank4
            RV_LUT_BANK_05  = 5,                        // Select Bank5
            RV_LUT_BANK_06  = 6,                        // Select Bank6
            RV_LUT_BANK_07  = 7,                        // Select Bank7
            RV_LUT_BANK_08  = 8,                        // Select Bank8
            RV_LUT_BANK_09  = 9,                        // Select Bank9
            RV_LUT_BANK_10  = 10,                       // Select Bank10
            RV_LUT_BANK_11  = 11,                       // Select Bank11
            RV_LUT_BANK_12  = 12,                       // Select Bank12
            RV_LUT_BANK_13  = 13,                       // Select Bank13
            RV_LUT_BANK_14  = 14,                       // Select Bank14
            RV_LUT_BANK_15  = 15,                       // Select Bank15
            RV_LUT_BANK_16  = 16,                       // Select Bank16
            RV_LUT_BANK_17  = 17,                       // Select Bank17
            RV_LUT_BANK_18  = 18,                       // Select Bank18
            RV_LUT_BANK_19  = 19,                       // Select Bank19
            RV_LUT_BANK_20  = 20,                       // Select Bank20
            RV_LUT_BANK_21  = 21,                       // Select Bank21
            RV_LUT_BANK_22  = 22,                       // Select Bank22
            RV_LUT_BANK_23  = 23,                       // Select Bank23
            RV_LUT_BANK_24  = 24,                       // Select Bank24
            RV_LUT_BANK_25  = 25,                       // Select Bank25
            RV_LUT_BANK_26  = 26,                       // Select Bank26
            RV_LUT_BANK_27  = 27,                       // Select Bank27
            RV_LUT_BANK_28  = 28,                       // Select Bank28
            RV_LUT_BANK_29  = 29,                       // Select Bank29
            RV_LUT_BANK_30  = 30,                       // Select Bank30
            RV_LUT_BANK_31  = 31,                       // Select Bank31
        };

        const REGDATA g_sREGV_LUT_Bank00    = REGDATA(RV_LUT_BANK_00,   (char*)"Bank0");
        const REGDATA g_sREGV_LUT_Bank01    = REGDATA(RV_LUT_BANK_01,   (char*)"Bank1");
        const REGDATA g_sREGV_LUT_Bank02    = REGDATA(RV_LUT_BANK_02,   (char*)"Bank2");
        const REGDATA g_sREGV_LUT_Bank03    = REGDATA(RV_LUT_BANK_03,   (char*)"Bank3");
        const REGDATA g_sREGV_LUT_Bank04    = REGDATA(RV_LUT_BANK_04,   (char*)"Bank4");
        const REGDATA g_sREGV_LUT_Bank05    = REGDATA(RV_LUT_BANK_05,   (char*)"Bank5");
        const REGDATA g_sREGV_LUT_Bank06    = REGDATA(RV_LUT_BANK_06,   (char*)"Bank6");
        const REGDATA g_sREGV_LUT_Bank07    = REGDATA(RV_LUT_BANK_07,   (char*)"Bank7");
        const REGDATA g_sREGV_LUT_Bank08    = REGDATA(RV_LUT_BANK_08,   (char*)"Bank8");
        const REGDATA g_sREGV_LUT_Bank09    = REGDATA(RV_LUT_BANK_09,   (char*)"Bank9");
        const REGDATA g_sREGV_LUT_Bank10    = REGDATA(RV_LUT_BANK_10,   (char*)"Bank10");
        const REGDATA g_sREGV_LUT_Bank11    = REGDATA(RV_LUT_BANK_11,   (char*)"Bank11");
        const REGDATA g_sREGV_LUT_Bank12    = REGDATA(RV_LUT_BANK_12,   (char*)"Bank12");
        const REGDATA g_sREGV_LUT_Bank13    = REGDATA(RV_LUT_BANK_13,   (char*)"Bank13");
        const REGDATA g_sREGV_LUT_Bank14    = REGDATA(RV_LUT_BANK_14,   (char*)"Bank14");
        const REGDATA g_sREGV_LUT_Bank15    = REGDATA(RV_LUT_BANK_15,   (char*)"Bank15");
        const REGDATA g_sREGV_LUT_Bank16    = REGDATA(RV_LUT_BANK_16,   (char*)"Bank16");
        const REGDATA g_sREGV_LUT_Bank17    = REGDATA(RV_LUT_BANK_17,   (char*)"Bank17");
        const REGDATA g_sREGV_LUT_Bank18    = REGDATA(RV_LUT_BANK_18,   (char*)"Bank18");
        const REGDATA g_sREGV_LUT_Bank19    = REGDATA(RV_LUT_BANK_19,   (char*)"Bank19");
        const REGDATA g_sREGV_LUT_Bank20    = REGDATA(RV_LUT_BANK_20,   (char*)"Bank20");
        const REGDATA g_sREGV_LUT_Bank21    = REGDATA(RV_LUT_BANK_21,   (char*)"Bank21");
        const REGDATA g_sREGV_LUT_Bank22    = REGDATA(RV_LUT_BANK_22,   (char*)"Bank22");
        const REGDATA g_sREGV_LUT_Bank23    = REGDATA(RV_LUT_BANK_23,   (char*)"Bank23");
        const REGDATA g_sREGV_LUT_Bank24    = REGDATA(RV_LUT_BANK_24,   (char*)"Bank24");
        const REGDATA g_sREGV_LUT_Bank25    = REGDATA(RV_LUT_BANK_25,   (char*)"Bank25");
        const REGDATA g_sREGV_LUT_Bank26    = REGDATA(RV_LUT_BANK_26,   (char*)"Bank26");
        const REGDATA g_sREGV_LUT_Bank27    = REGDATA(RV_LUT_BANK_27,   (char*)"Bank27");
        const REGDATA g_sREGV_LUT_Bank28    = REGDATA(RV_LUT_BANK_28,   (char*)"Bank28");
        const REGDATA g_sREGV_LUT_Bank29    = REGDATA(RV_LUT_BANK_29,   (char*)"Bank29");
        const REGDATA g_sREGV_LUT_Bank30    = REGDATA(RV_LUT_BANK_30,   (char*)"Bank30");
        const REGDATA g_sREGV_LUT_Bank31    = REGDATA(RV_LUT_BANK_31,   (char*)"Bank31");

        const REGDATA g_saVAL_LUTBankSel[] =
        {
            g_sREGV_LUT_Bank00,
            g_sREGV_LUT_Bank01,
            g_sREGV_LUT_Bank02,
            g_sREGV_LUT_Bank03,
            g_sREGV_LUT_Bank04,
            g_sREGV_LUT_Bank05,
            g_sREGV_LUT_Bank06,
            g_sREGV_LUT_Bank07,
            g_sREGV_LUT_Bank08,
            g_sREGV_LUT_Bank09,
            g_sREGV_LUT_Bank20,
            g_sREGV_LUT_Bank11,
            g_sREGV_LUT_Bank12,
            g_sREGV_LUT_Bank13,
            g_sREGV_LUT_Bank14,
            g_sREGV_LUT_Bank15,
            g_sREGV_LUT_Bank16,
            g_sREGV_LUT_Bank17,
            g_sREGV_LUT_Bank18,
            g_sREGV_LUT_Bank19,
            g_sREGV_LUT_Bank20,
            g_sREGV_LUT_Bank21,
            g_sREGV_LUT_Bank22,
            g_sREGV_LUT_Bank23,
            g_sREGV_LUT_Bank24,
            g_sREGV_LUT_Bank25,
            g_sREGV_LUT_Bank26,
            g_sREGV_LUT_Bank27,
            g_sREGV_LUT_Bank28,
            g_sREGV_LUT_Bank29,
            g_sREGV_LUT_Bank30,
            g_sREGV_LUT_Bank31,
        };

        //=================================================================
        //  LUT Value All
        //=================================================================
        enum  REGVAL_LUT_VALUE_ALL
        {
            RV_LUT_PRESET_00 = 0,                       // Preset lookup table 0
            RV_LUT_PRESET_01 = 1,                       // Preset lookup table 1
            RV_LUT_PRESET_02 = 2,                       // Preset lookup table 2
            RV_LUT_PRESET_03 = 3,                       // Preset lookup table 3
            RV_LUT_PRESET_04 = 4,                       // Preset lookup table 4
            RV_LUT_PRESET_05 = 5,                       // Preset lookup table 5
            RV_LUT_PRESET_06 = 6,                       // Preset lookup table 6
            RV_LUT_PRESET_07 = 7,                       // Preset lookup table 7
            RV_LUT_PRESET_08 = 8,                       // Preset lookup table 8
            RV_LUT_PRESET_09 = 9,                       // Preset lookup table 9
            RV_LUT_PRESET_10 = 10,                      // Preset lookup table 10
            RV_LUT_PRESET_11 = 11,                      // Preset lookup table 11
            RV_LUT_PRESET_12 = 12,                      // Preset lookup table 12
            RV_LUT_PRESET_13 = 13,                      // Preset lookup table 13
            RV_LUT_PRESET_14 = 14,                      // Preset lookup table 14
            RV_LUT_PRESET_15 = 15,                      // Preset lookup table 15
            RV_LUT_PRESET_16 = 16,                      // Preset lookup table 16
            RV_LUT_PRESET_17 = 17,                      // Preset lookup table 17
            RV_LUT_PRESET_18 = 18,                      // Preset lookup table 18
            RV_LUT_PRESET_19 = 19,                      // Preset lookup table 19
            RV_LUT_PRESET_20 = 20,                      // Preset lookup table 20
            RV_LUT_PRESET_21 = 21,                      // Preset lookup table 21
            RV_LUT_PRESET_22 = 22,                      // Preset lookup table 22
            RV_LUT_PRESET_23 = 23,                      // Preset lookup table 23
            RV_LUT_PRESET_24 = 24,                      // Preset lookup table 24
            RV_LUT_PRESET_25 = 25,                      // Preset lookup table 25
            RV_LUT_PRESET_26 = 26,                      // Preset lookup table 26
            RV_LUT_PRESET_27 = 27,                      // Preset lookup table 27
            RV_LUT_PRESET_28 = 28,                      // Preset lookup table 28
            RV_LUT_PRESET_29 = 29,                      // Preset lookup table 29
            RV_LUT_PRESET_30 = 30,                      // Preset lookup table 30
            RV_LUT_PRESET_31 = 31,                      // Preset lookup table 31
        };

        const REGDATA g_sREGV_LUT_Preset00  = REGDATA(RV_LUT_PRESET_00, (char*)"Preset0");
        const REGDATA g_sREGV_LUT_Preset01  = REGDATA(RV_LUT_PRESET_01, (char*)"Preset1");
        const REGDATA g_sREGV_LUT_Preset02  = REGDATA(RV_LUT_PRESET_02, (char*)"Preset2");
        const REGDATA g_sREGV_LUT_Preset03  = REGDATA(RV_LUT_PRESET_03, (char*)"Preset3");
        const REGDATA g_sREGV_LUT_Preset04  = REGDATA(RV_LUT_PRESET_04, (char*)"Preset4");
        const REGDATA g_sREGV_LUT_Preset05  = REGDATA(RV_LUT_PRESET_05, (char*)"Preset5");
        const REGDATA g_sREGV_LUT_Preset06  = REGDATA(RV_LUT_PRESET_06, (char*)"Preset6");
        const REGDATA g_sREGV_LUT_Preset07  = REGDATA(RV_LUT_PRESET_07, (char*)"Preset7");
        const REGDATA g_sREGV_LUT_Preset08  = REGDATA(RV_LUT_PRESET_08, (char*)"Preset8");
        const REGDATA g_sREGV_LUT_Preset09  = REGDATA(RV_LUT_PRESET_09, (char*)"Preset9");
        const REGDATA g_sREGV_LUT_Preset10  = REGDATA(RV_LUT_PRESET_10, (char*)"Preset10");
        const REGDATA g_sREGV_LUT_Preset11  = REGDATA(RV_LUT_PRESET_11, (char*)"Preset11");
        const REGDATA g_sREGV_LUT_Preset12  = REGDATA(RV_LUT_PRESET_12, (char*)"Preset12");
        const REGDATA g_sREGV_LUT_Preset13  = REGDATA(RV_LUT_PRESET_13, (char*)"Preset13");
        const REGDATA g_sREGV_LUT_Preset14  = REGDATA(RV_LUT_PRESET_14, (char*)"Preset14");
        const REGDATA g_sREGV_LUT_Preset15  = REGDATA(RV_LUT_PRESET_15, (char*)"Preset15");
        const REGDATA g_sREGV_LUT_Preset16  = REGDATA(RV_LUT_PRESET_16, (char*)"Preset16");
        const REGDATA g_sREGV_LUT_Preset17  = REGDATA(RV_LUT_PRESET_17, (char*)"Preset17");
        const REGDATA g_sREGV_LUT_Preset18  = REGDATA(RV_LUT_PRESET_18, (char*)"Preset18");
        const REGDATA g_sREGV_LUT_Preset19  = REGDATA(RV_LUT_PRESET_19, (char*)"Preset19");
        const REGDATA g_sREGV_LUT_Preset20  = REGDATA(RV_LUT_PRESET_20, (char*)"Preset20");
        const REGDATA g_sREGV_LUT_Preset21  = REGDATA(RV_LUT_PRESET_21, (char*)"Preset21");
        const REGDATA g_sREGV_LUT_Preset22  = REGDATA(RV_LUT_PRESET_22, (char*)"Preset22");
        const REGDATA g_sREGV_LUT_Preset23  = REGDATA(RV_LUT_PRESET_23, (char*)"Preset23");
        const REGDATA g_sREGV_LUT_Preset24  = REGDATA(RV_LUT_PRESET_24, (char*)"Preset24");
        const REGDATA g_sREGV_LUT_Preset25  = REGDATA(RV_LUT_PRESET_25, (char*)"Preset25");
        const REGDATA g_sREGV_LUT_Preset26  = REGDATA(RV_LUT_PRESET_26, (char*)"Preset26");
        const REGDATA g_sREGV_LUT_Preset27  = REGDATA(RV_LUT_PRESET_27, (char*)"Preset27");
        const REGDATA g_sREGV_LUT_Preset28  = REGDATA(RV_LUT_PRESET_28, (char*)"Preset28");
        const REGDATA g_sREGV_LUT_Preset29  = REGDATA(RV_LUT_PRESET_29, (char*)"Preset29");
        const REGDATA g_sREGV_LUT_Preset30  = REGDATA(RV_LUT_PRESET_30, (char*)"Preset30");
        const REGDATA g_sREGV_LUT_Preset31  = REGDATA(RV_LUT_PRESET_31, (char*)"Preset31");

        const REGDATA g_saVAL_LUTValAll[] =
        {
            g_sREGV_LUT_Preset00,
            g_sREGV_LUT_Preset01,
            g_sREGV_LUT_Preset02,
            g_sREGV_LUT_Preset03,
            g_sREGV_LUT_Preset04,
            g_sREGV_LUT_Preset05,
            g_sREGV_LUT_Preset06,
            g_sREGV_LUT_Preset07,
            g_sREGV_LUT_Preset08,
            g_sREGV_LUT_Preset09,
            g_sREGV_LUT_Preset20,
            g_sREGV_LUT_Preset11,
            g_sREGV_LUT_Preset12,
            g_sREGV_LUT_Preset13,
            g_sREGV_LUT_Preset14,
            g_sREGV_LUT_Preset15,
            g_sREGV_LUT_Preset16,
            g_sREGV_LUT_Preset17,
            g_sREGV_LUT_Preset18,
            g_sREGV_LUT_Preset19,
            g_sREGV_LUT_Preset20,
            g_sREGV_LUT_Preset21,
            g_sREGV_LUT_Preset22,
            g_sREGV_LUT_Preset23,
            g_sREGV_LUT_Preset24,
            g_sREGV_LUT_Preset25,
            g_sREGV_LUT_Preset26,
            g_sREGV_LUT_Preset27,
            g_sREGV_LUT_Preset28,
            g_sREGV_LUT_Preset29,
            g_sREGV_LUT_Preset30,
            g_sREGV_LUT_Preset31,
        };
    

        //=================================================================
        // Trigger Sequence
        //=================================================================
        enum  REGVAL_TRIGGER_SEQUENCE
        {
            RV_TRIGGER_SEQUENCE_0    = 0,                       // External edge mode
            RV_TRIGGER_SEQUENCE_1    = 1,                       // External level mode
            RV_TRIGGER_SEQUENCE_2    = 2,                       // External event mode
            RV_TRIGGER_SEQUENCE_3    = 3,                       // Frame interval mode
            RV_TRIGGER_SEQUENCE_4    = 4,                       // Multiple shutter preset mode
            RV_TRIGGER_SEQUENCE_5    = 5,                       // Multiple shutter pulse width mode
            RV_TRIGGER_SEQUENCE_6    = 6,                       // Bulk trigger mode (Not defined in IIDC2 standard)
        };

        const REGDATA g_sREGV_Trg_Sequence_0    = REGDATA(RV_TRIGGER_SEQUENCE_0,    (char*)"TriggerSequence0");
        const REGDATA g_sREGV_Trg_Sequence_1    = REGDATA(RV_TRIGGER_SEQUENCE_1,    (char*)"TriggerSequence1");
        const REGDATA g_sREGV_Trg_Sequence_2    = REGDATA(RV_TRIGGER_SEQUENCE_2,    (char*)"TriggerSequence2");
        const REGDATA g_sREGV_Trg_Sequence_3    = REGDATA(RV_TRIGGER_SEQUENCE_3,    (char*)"TriggerSequence3");
        const REGDATA g_sREGV_Trg_Sequence_4    = REGDATA(RV_TRIGGER_SEQUENCE_4,    (char*)"TriggerSequence4");
        const REGDATA g_sREGV_Trg_Sequence_5    = REGDATA(RV_TRIGGER_SEQUENCE_5,    (char*)"TriggerSequence5");
        const REGDATA g_sREGV_Trg_Sequence_6    = REGDATA(RV_TRIGGER_SEQUENCE_6,    (char*)"TriggerSequence6");

        const REGDATA g_saVAL_TrigSeq[] =
        {
            g_sREGV_Trg_Sequence_0,
            g_sREGV_Trg_Sequence_1,
            //g_sREGV_Trg_Sequence_2,
            //g_sREGV_Trg_Sequence_3,
            //g_sREGV_Trg_Sequence_4,
            //g_sREGV_Trg_Sequence_5,
            g_sREGV_Trg_Sequence_6,
        };

        //=================================================================
        // Trigger Source
        //=================================================================
        enum  REGVAL_TRIGGER_SOURCE
        {
            RV_TRG_SRC_IOLINE_00    = 0,                        // IO line 0  (0)
            RV_TRG_SRC_IOLINE_01    = 1,                        // IO line 1  (1)
            RV_TRG_SRC_IOLINE_02    = 2,                        // IO line 2  (2)
            RV_TRG_SRC_IOLINE_03    = 3,                        // IO line 3  (3)
            RV_TRG_SRC_IOLINE_04    = 4,                        // IO line 4  (4)
            RV_TRG_SRC_IOLINE_05    = 5,                        // IO line 5  (5)
            RV_TRG_SRC_IOLINE_06    = 6,                        // IO line 6  (6)
            RV_TRG_SRC_IOLINE_07    = 7,                        // IO line 7  (7)
            RV_TRG_SRC_IOLINE_08    = 8,                        // IO line 8  (8)
            RV_TRG_SRC_IOLINE_09    = 9,                        // IO line 9  (9)
            RV_TRG_SRC_IOLINE_10    = 10,                       // IO line 10 (10)
            RV_TRG_SRC_IOLINE_11    = 11,                       // IO line 11 (11)
            RV_TRG_SRC_IOLINE_12    = 12,                       // IO line 12 (12)
            RV_TRG_SRC_IOLINE_13    = 13,                       // IO line 13 (13)
            RV_TRG_SRC_IOLINE_14    = 14,                       // IO line 14 (14)
            RV_TRG_SRC_IOLINE_15    = 15,                       // IO line 15 (15)
            RV_TRG_SRC_IOLINE_16    = 16,                       // IO line 16 (16)
            RV_TRG_SRC_IOLINE_17    = 17,                       // IO line 17 (17)
            RV_TRG_SRC_IOLINE_18    = 18,                       // IO line 18 (18)
            RV_TRG_SRC_IOLINE_19    = 19,                       // IO line 19 (19)
            RV_TRG_SRC_IOLINE_20    = 20,                       // IO line 20 (20)
            RV_TRG_SRC_IOLINE_21    = 21,                       // IO line 21 (21)
            RV_TRG_SRC_IOLINE_22    = 22,                       // IO line 22 (22)
            RV_TRG_SRC_IOLINE_23    = 23,                       // IO line 23 (23)
            RV_TRG_SRC_IOLINE_24    = 24,                       // IO line 24 (24)
            RV_TRG_SRC_IOLINE_25    = 25,                       // IO line 25 (25)
            RV_TRG_SRC_IOLINE_26    = 26,                       // IO line 26 (26)
            RV_TRG_SRC_IOLINE_27    = 27,                       // IO line 27 (27)
            RV_TRG_SRC_IOLINE_28    = 28,                       // IO line 28 (28)
            RV_TRG_SRC_IOLINE_29    = 29,                       // IO line 29 (29)
            RV_TRG_SRC_IOLINE_30    = 30,                       // IO line 30 (30)
            RV_TRG_SRC_IOLINE_31    = 31,                       // IO line 31 (31)
            RV_TRG_SRC_SOFTWARE     = 64,                       // Software trigger mode (64)
        };

        const REGDATA g_sREGV_Trg_Src_Line_00   = REGDATA(RV_TRG_SRC_IOLINE_00, (char*)"Line0");
        const REGDATA g_sREGV_Trg_Src_Line_01   = REGDATA(RV_TRG_SRC_IOLINE_01, (char*)"Line1");
        const REGDATA g_sREGV_Trg_Src_Line_02   = REGDATA(RV_TRG_SRC_IOLINE_02, (char*)"Line2");
        const REGDATA g_sREGV_Trg_Src_Line_03   = REGDATA(RV_TRG_SRC_IOLINE_03, (char*)"Line3");
        const REGDATA g_sREGV_Trg_Src_Line_04   = REGDATA(RV_TRG_SRC_IOLINE_04, (char*)"Line4");
        const REGDATA g_sREGV_Trg_Src_Line_05   = REGDATA(RV_TRG_SRC_IOLINE_05, (char*)"Line5");
        const REGDATA g_sREGV_Trg_Src_Line_06   = REGDATA(RV_TRG_SRC_IOLINE_06, (char*)"Line6");
        const REGDATA g_sREGV_Trg_Src_Line_07   = REGDATA(RV_TRG_SRC_IOLINE_07, (char*)"Line7");
        const REGDATA g_sREGV_Trg_Src_Line_08   = REGDATA(RV_TRG_SRC_IOLINE_08, (char*)"Line8");
        const REGDATA g_sREGV_Trg_Src_Line_09   = REGDATA(RV_TRG_SRC_IOLINE_09, (char*)"Line9");
        const REGDATA g_sREGV_Trg_Src_Line_10   = REGDATA(RV_TRG_SRC_IOLINE_10, (char*)"Line10");
        const REGDATA g_sREGV_Trg_Src_Line_11   = REGDATA(RV_TRG_SRC_IOLINE_11, (char*)"Line11");
        const REGDATA g_sREGV_Trg_Src_Line_12   = REGDATA(RV_TRG_SRC_IOLINE_12, (char*)"Line12");
        const REGDATA g_sREGV_Trg_Src_Line_13   = REGDATA(RV_TRG_SRC_IOLINE_13, (char*)"Line13");
        const REGDATA g_sREGV_Trg_Src_Line_14   = REGDATA(RV_TRG_SRC_IOLINE_14, (char*)"Line14");
        const REGDATA g_sREGV_Trg_Src_Line_15   = REGDATA(RV_TRG_SRC_IOLINE_15, (char*)"Line15");
        const REGDATA g_sREGV_Trg_Src_Line_16   = REGDATA(RV_TRG_SRC_IOLINE_16, (char*)"Line16");
        const REGDATA g_sREGV_Trg_Src_Line_17   = REGDATA(RV_TRG_SRC_IOLINE_17, (char*)"Line17");
        const REGDATA g_sREGV_Trg_Src_Line_18   = REGDATA(RV_TRG_SRC_IOLINE_18, (char*)"Line18");
        const REGDATA g_sREGV_Trg_Src_Line_19   = REGDATA(RV_TRG_SRC_IOLINE_19, (char*)"Line19");
        const REGDATA g_sREGV_Trg_Src_Line_20   = REGDATA(RV_TRG_SRC_IOLINE_20, (char*)"Line20");
        const REGDATA g_sREGV_Trg_Src_Line_21   = REGDATA(RV_TRG_SRC_IOLINE_21, (char*)"Line21");
        const REGDATA g_sREGV_Trg_Src_Line_22   = REGDATA(RV_TRG_SRC_IOLINE_22, (char*)"Line22");
        const REGDATA g_sREGV_Trg_Src_Line_23   = REGDATA(RV_TRG_SRC_IOLINE_23, (char*)"Line23");
        const REGDATA g_sREGV_Trg_Src_Line_24   = REGDATA(RV_TRG_SRC_IOLINE_24, (char*)"Line24");
        const REGDATA g_sREGV_Trg_Src_Line_25   = REGDATA(RV_TRG_SRC_IOLINE_25, (char*)"Line25");
        const REGDATA g_sREGV_Trg_Src_Line_26   = REGDATA(RV_TRG_SRC_IOLINE_26, (char*)"Line26");
        const REGDATA g_sREGV_Trg_Src_Line_27   = REGDATA(RV_TRG_SRC_IOLINE_27, (char*)"Line27");
        const REGDATA g_sREGV_Trg_Src_Line_28   = REGDATA(RV_TRG_SRC_IOLINE_28, (char*)"Line28");
        const REGDATA g_sREGV_Trg_Src_Line_29   = REGDATA(RV_TRG_SRC_IOLINE_29, (char*)"Line29");
        const REGDATA g_sREGV_Trg_Src_Line_30   = REGDATA(RV_TRG_SRC_IOLINE_30, (char*)"Line30");
        const REGDATA g_sREGV_Trg_Src_Line_31   = REGDATA(RV_TRG_SRC_IOLINE_31, (char*)"Line31");
        const REGDATA g_sREGV_Trg_Src_Software  = REGDATA(RV_TRG_SRC_SOFTWARE,  (char*)"Software");

        const REGDATA g_saVAL_TrigSrc[] =
        {
            g_sREGV_Trg_Src_Line_00,
            g_sREGV_Trg_Src_Line_01,
            g_sREGV_Trg_Src_Line_02,
            g_sREGV_Trg_Src_Line_03,
            g_sREGV_Trg_Src_Line_04,
            g_sREGV_Trg_Src_Line_05,
            g_sREGV_Trg_Src_Line_06,
            g_sREGV_Trg_Src_Line_07,
            g_sREGV_Trg_Src_Line_08,
            g_sREGV_Trg_Src_Line_09,
            g_sREGV_Trg_Src_Line_10,
            g_sREGV_Trg_Src_Line_11,
            g_sREGV_Trg_Src_Line_12,
            g_sREGV_Trg_Src_Line_13,
            g_sREGV_Trg_Src_Line_14,
            g_sREGV_Trg_Src_Line_15,
            g_sREGV_Trg_Src_Line_16,
            g_sREGV_Trg_Src_Line_17,
            g_sREGV_Trg_Src_Line_18,
            g_sREGV_Trg_Src_Line_19,
            g_sREGV_Trg_Src_Line_20,
            g_sREGV_Trg_Src_Line_21,
            g_sREGV_Trg_Src_Line_22,
            g_sREGV_Trg_Src_Line_23,
            g_sREGV_Trg_Src_Line_24,
            g_sREGV_Trg_Src_Line_25,
            g_sREGV_Trg_Src_Line_26,
            g_sREGV_Trg_Src_Line_27,
            g_sREGV_Trg_Src_Line_28,
            g_sREGV_Trg_Src_Line_29,
            g_sREGV_Trg_Src_Line_30,
            g_sREGV_Trg_Src_Line_31,
            g_sREGV_Trg_Src_Software,
        };

        //=================================================================
        // Software Trigger
        //=================================================================
        enum  REGVAL_SOFTWARE_TRIGGER
        {
            RV_TRG_SOFTWARE_INACTIVE    = 0,            // Inactive  (0)
            RV_TRG_SOFTWARE_ACTIVE      = 1,            // Active  (1)
            RV_TRG_SOFTWARE_IMPULSE = 8,                // Impulse mode  (8)
        };

        const REGDATA g_sREGV_TrgSof_Inactive   = REGDATA(RV_TRG_SOFTWARE_INACTIVE, (char*)"Inactive");
        const REGDATA g_sREGV_TrgSof_Active     = REGDATA(RV_TRG_SOFTWARE_ACTIVE,   (char*)"Active");
        const REGDATA g_sREGV_TrgSof_Impulse    = REGDATA(RV_TRG_SOFTWARE_IMPULSE,  (char*)"Impulse");

        const REGDATA g_saVAL_SoftTrig[] =
        {
            g_sREGV_TrgSof_Inactive,
            g_sREGV_TrgSof_Active,
            g_sREGV_TrgSof_Impulse,
        };


        //=================================================================
        // Standard format
        //=================================================================
       enum  REGVAL_STANDARD_FORMAT
        {
            RV_USET_STDFMT_VGA_MONO8            = 0,        // 640 x 480 8bit Monochrome (0)
            RV_USET_STDFMT_VGA_RGB8PACKED       = 1,        // 640 x 480 8bit RGB (1)
            RV_USET_STDFMT_VGA_YUV422PACKED     = 2,        // 640 x 480 8bit YUV {U, Y0, V, Y1} (2)
            RV_USET_STDFMT_XGA_MONO8            = 8,        // 1024 x 768 8 bit Monochrome  (8)
            RV_USET_STDFMT_XGA_RGB8PACKED       = 9,        // 1024 x 768 8bit RGB (9)
            RV_USET_STDFMT_XGA_YUV422PACKED     = 10,       // 1024 x 768 8bit YUV {U, Y0, V, Y1} (10)
            RV_USET_STDFMT_HD720P_MONO8         = 16,       // 1280 x 720 8 bit Monochrome (16)
            RV_USET_STDFMT_HD720P_RGB8PACKED    = 17,       // 1280 x 720 8bit RGB  (17)
            RV_USET_STDFMT_HD720P_YUV422PACKED  = 18,       // 1280 x 720 YUV {U, Y0, V, Y1}  (18)
            RV_USET_STDFMT_SXGA_MONO8           = 24,       // 1280 x 960 8 bit Monochrome  (24)
            RV_USET_STDFMT_SXGA_RGB8PACKED      = 25,       // 1280 x 960 8bit RGB (25)
            RV_USET_STDFMT_SXGA_YUV422PACKED    = 26,       // 1280 x 960 YUV {U, Y0, V, Y1} (26)
            RV_USET_STDFMT_UXGA_MONO8           = 32,       // 1600 x 1200 8 bit Monochrome (32)
            RV_USET_STDFMT_UXGA_RGB8PACKED      = 33,       // 1600 x 1200 8bit RGB (33)
            RV_USET_STDFMT_UXGA_YUV422PACKED    = 34,       // 1600 x 1200 YUV {U, Y0, V, Y1} (34)
            RV_USET_STDFMT_HD1080P_MONO8        = 40,       // 1920 x 1080 8 bit Monochrome  (40)
            RV_USET_STDFMT_HD1080P_RGB8PACKED   = 41,       // 1920 x 1080 8bit RGB  (41)
            RV_USET_STDFMT_HD1080P_YUV422PACKED = 42        // 1920 x 1080 YUV  {U, Y0, V, Y1}  (42)
        };

        const REGDATA g_sREGV_StdFmt_VGA_Mono8          = REGDATA(RV_USET_STDFMT_VGA_MONO8,             (char*)"VGA_Mono8");
        const REGDATA g_sREGV_StdFmt_VGA_RGB8P          = REGDATA(RV_USET_STDFMT_VGA_RGB8PACKED,        (char*)"VGA_RGB8Packed");
        const REGDATA g_sREGV_StdFmt_VGA_Yuv422P        = REGDATA(RV_USET_STDFMT_VGA_YUV422PACKED,      (char*)"VGA_Yuv422Packed");
        const REGDATA g_sREGV_StdFmt_XGA_Mono8          = REGDATA(RV_USET_STDFMT_XGA_MONO8,             (char*)"XGA_Mono8");
        const REGDATA g_sREGV_StdFmt_XGA_RGB8P          = REGDATA(RV_USET_STDFMT_XGA_RGB8PACKED,        (char*)"XGA_RGB8Packed");
        const REGDATA g_sREGV_StdFmt_XGA_Yuv422P        = REGDATA(RV_USET_STDFMT_XGA_YUV422PACKED,      (char*)"XGA_Yuv422Packed");
        const REGDATA g_sREGV_StdFmt_HD72op_Mono8       = REGDATA(RV_USET_STDFMT_HD720P_MONO8,          (char*)"HD720p_Mono8");
        const REGDATA g_sREGV_StdFmt_HD720p_RGB8P       = REGDATA(RV_USET_STDFMT_HD720P_RGB8PACKED,     (char*)"HD720p_RGB8Packed");
        const REGDATA g_sREGV_StdFmt_HD720p_Yuv422P     = REGDATA(RV_USET_STDFMT_HD720P_YUV422PACKED,   (char*)"HD720p_Yuv422Packed");
        const REGDATA g_sREGV_StdFmt_SXGA_Mono8         = REGDATA(RV_USET_STDFMT_SXGA_MONO8,            (char*)"SXGA_Mono8");
        const REGDATA g_sREGV_StdFmt_SXGA_RGB8P         = REGDATA(RV_USET_STDFMT_SXGA_RGB8PACKED,       (char*)"SXGA_RGB8Packed");
        const REGDATA g_sREGV_StdFmt_SXGA_Yuv422P       = REGDATA(RV_USET_STDFMT_SXGA_YUV422PACKED,     (char*)"SXGA_Yuv422Packed");
        const REGDATA g_sREGV_StdFmt_HD1080p_Mono8      = REGDATA(RV_USET_STDFMT_HD1080P_MONO8,         (char*)"HD1080p_Mono8");
        const REGDATA g_sREGV_StdFmt_HD1080p_RGB8P      = REGDATA(RV_USET_STDFMT_HD1080P_RGB8PACKED,    (char*)"HD1080p_RGB8Packed");
        const REGDATA g_sREGV_StdFmt_HD1080p_Yuv422P    = REGDATA(RV_USET_STDFMT_HD1080P_YUV422PACKED,  (char*)"HD1080p_Yuv422Packed");

        const REGDATA g_saVAL_STdFormat[] =
        {
            g_sREGV_StdFmt_VGA_Mono8,
            g_sREGV_StdFmt_VGA_RGB8P,
            g_sREGV_StdFmt_VGA_Yuv422P,
            g_sREGV_StdFmt_XGA_Mono8,
            g_sREGV_StdFmt_XGA_RGB8P,
            g_sREGV_StdFmt_XGA_Yuv422P,
            g_sREGV_StdFmt_HD72op_Mono8,
            g_sREGV_StdFmt_HD720p_RGB8P,
            g_sREGV_StdFmt_HD720p_Yuv422P,
            g_sREGV_StdFmt_SXGA_Mono8,
            g_sREGV_StdFmt_SXGA_RGB8P,
            g_sREGV_StdFmt_SXGA_Yuv422P,
            g_sREGV_StdFmt_HD1080p_Mono8,
            g_sREGV_StdFmt_HD1080p_RGB8P,
            g_sREGV_StdFmt_HD1080p_Yuv422P,
        };

        //=================================================================
        // Standard frame rate
        //=================================================================
       enum  REGVAL_STANDARD_FRAME_RATE
        {
            RV_USET_STD_FRAME_RATE_1_875FPS = 0,        // 1.875fps  (0)
            RV_USET_STD_FRAME_RATE_3_75FPS  = 1,        // 3.75fps  (1)
            RV_USET_STD_FRAME_RATE_7_5FPS   = 2,        // 7.5fps  (2)
            RV_USET_STD_FRAME_RATE_15FPS    = 3,        // 15fps  (3)
            RV_USET_STD_FRAME_RATE_30FPS    = 4,        // 30fps  (4)
            RV_USET_STD_FRAME_RATE_60FPS    = 5,        // 60fps  (5)
            RV_USET_STD_FRAME_RATE_120FPS   = 6,        // 120fps  (6)
            RV_USET_STD_FRAME_RATE_240FPS   = 7,        // 240fps  (7)
        };

        const REGDATA g_sREGV_StdFrmRate_1_875fps   = REGDATA(RV_USET_STD_FRAME_RATE_1_875FPS,  (char*)"1.875fps");
        const REGDATA g_sREGV_StdFrmRate_3_75fps    = REGDATA(RV_USET_STD_FRAME_RATE_3_75FPS,   (char*)"3.75fps");
        const REGDATA g_sREGV_StdFrmRate_7_5fps     = REGDATA(RV_USET_STD_FRAME_RATE_7_5FPS,    (char*)"7.5fps");
        const REGDATA g_sREGV_StdFrmRate_15fps      = REGDATA(RV_USET_STD_FRAME_RATE_15FPS,     (char*)"15fps");
        const REGDATA g_sREGV_StdFrmRate_30fps      = REGDATA(RV_USET_STD_FRAME_RATE_30FPS,     (char*)"30fps");
        const REGDATA g_sREGV_StdFrmRate_60fps      = REGDATA(RV_USET_STD_FRAME_RATE_60FPS,     (char*)"60fps");
        const REGDATA g_sREGV_StdFrmRate_120fps     = REGDATA(RV_USET_STD_FRAME_RATE_120FPS,    (char*)"120fps");
        const REGDATA g_sREGV_StdFrmRate_240fps     = REGDATA(RV_USET_STD_FRAME_RATE_240FPS,    (char*)"240fps");

        const REGDATA g_saVAL_StdFrmRate[] =
        {
            g_sREGV_StdFrmRate_1_875fps,
            g_sREGV_StdFrmRate_3_75fps,
            g_sREGV_StdFrmRate_7_5fps,
            g_sREGV_StdFrmRate_15fps,
            g_sREGV_StdFrmRate_30fps,
            g_sREGV_StdFrmRate_60fps,
            g_sREGV_StdFrmRate_120fps,
            g_sREGV_StdFrmRate_240fps,
        };



        //=================================================================
        // Userset Selector
        //=================================================================
       enum  REGVAL_USERSET
        {
            RV_USET_USERSET_DEFAULT = 0,                    // Userset default
            RV_USET_USERSET_00  = 0,                        // Userset 0
            RV_USET_USERSET_01  = 1,                        // Userset 1
            RV_USET_USERSET_02  = 2,                        // Userset 2
            RV_USET_USERSET_03  = 3,                        // Userset 3
            RV_USET_USERSET_04  = 4,                        // Userset 4
            RV_USET_USERSET_05  = 5,                        // Userset 5
            RV_USET_USERSET_06  = 6,                        // Userset 6
            RV_USET_USERSET_07  = 7,                        // Userset 7
            RV_USET_USERSET_08  = 8,                        // Userset 8
            RV_USET_USERSET_09  = 9,                        // Userset 9
            RV_USET_USERSET_10  = 10,                       // Userset 10
            RV_USET_USERSET_11  = 11,                       // Userset 11
            RV_USET_USERSET_12  = 12,                       // Userset 12
            RV_USET_USERSET_13  = 13,                       // Userset 13
            RV_USET_USERSET_14  = 14,                       // Userset 14
            RV_USET_USERSET_15  = 15,                       // Userset 15
            RV_USET_USERSET_16  = 16,                       // Userset 16
            RV_USET_USERSET_17  = 17,                       // Userset 17
            RV_USET_USERSET_18  = 18,                       // Userset 18
            RV_USET_USERSET_19  = 19,                       // Userset 19
            RV_USET_USERSET_20  = 20,                       // Userset 20
            RV_USET_USERSET_21  = 21,                       // Userset 21
            RV_USET_USERSET_22  = 22,                       // Userset 22
            RV_USET_USERSET_23  = 23,                       // Userset 23
            RV_USET_USERSET_24  = 24,                       // Userset 24
            RV_USET_USERSET_25  = 25,                       // Userset 25
            RV_USET_USERSET_26  = 26,                       // Userset 26
            RV_USET_USERSET_27  = 27,                       // Userset 27
            RV_USET_USERSET_28  = 28,                       // Userset 28
            RV_USET_USERSET_29  = 29,                       // Userset 29
            RV_USET_USERSET_30  = 30,                       // Userset 30
            RV_USET_USERSET_31  = 31,                       // Userset 31
        };

        const REGDATA g_sREGV_USET_UsersetDflt  = REGDATA(RV_USET_USERSET_DEFAULT,  (char*)"UsersetDefault");
        const REGDATA g_sREGV_USET_Userset00    = REGDATA(RV_USET_USERSET_00,   (char*)"Userset0");
        const REGDATA g_sREGV_USET_Userset01    = REGDATA(RV_USET_USERSET_01,   (char*)"Userset1");
        const REGDATA g_sREGV_USET_Userset02    = REGDATA(RV_USET_USERSET_02,   (char*)"Userset2");
        const REGDATA g_sREGV_USET_Userset03    = REGDATA(RV_USET_USERSET_03,   (char*)"Userset3");
        const REGDATA g_sREGV_USET_Userset04    = REGDATA(RV_USET_USERSET_04,   (char*)"Userset4");
        const REGDATA g_sREGV_USET_Userset05    = REGDATA(RV_USET_USERSET_05,   (char*)"Userset5");
        const REGDATA g_sREGV_USET_Userset06    = REGDATA(RV_USET_USERSET_06,   (char*)"Userset6");
        const REGDATA g_sREGV_USET_Userset07    = REGDATA(RV_USET_USERSET_07,   (char*)"Userset7");
        const REGDATA g_sREGV_USET_Userset08    = REGDATA(RV_USET_USERSET_08,   (char*)"Userset8");
        const REGDATA g_sREGV_USET_Userset09    = REGDATA(RV_USET_USERSET_09,   (char*)"Userset9");
        const REGDATA g_sREGV_USET_Userset10    = REGDATA(RV_USET_USERSET_10,   (char*)"Userset10");
        const REGDATA g_sREGV_USET_Userset11    = REGDATA(RV_USET_USERSET_11,   (char*)"Userset11");
        const REGDATA g_sREGV_USET_Userset12    = REGDATA(RV_USET_USERSET_12,   (char*)"Userset12");
        const REGDATA g_sREGV_USET_Userset13    = REGDATA(RV_USET_USERSET_13,   (char*)"Userset13");
        const REGDATA g_sREGV_USET_Userset14    = REGDATA(RV_USET_USERSET_14,   (char*)"Userset14");
        const REGDATA g_sREGV_USET_Userset15    = REGDATA(RV_USET_USERSET_15,   (char*)"Userset15");
        const REGDATA g_sREGV_USET_Userset16    = REGDATA(RV_USET_USERSET_16,   (char*)"Userset16");
        const REGDATA g_sREGV_USET_Userset17    = REGDATA(RV_USET_USERSET_17,   (char*)"Userset17");
        const REGDATA g_sREGV_USET_Userset18    = REGDATA(RV_USET_USERSET_18,   (char*)"Userset18");
        const REGDATA g_sREGV_USET_Userset19    = REGDATA(RV_USET_USERSET_19,   (char*)"Userset19");
        const REGDATA g_sREGV_USET_Userset20    = REGDATA(RV_USET_USERSET_20,   (char*)"Userset20");
        const REGDATA g_sREGV_USET_Userset21    = REGDATA(RV_USET_USERSET_21,   (char*)"Userset21");
        const REGDATA g_sREGV_USET_Userset22    = REGDATA(RV_USET_USERSET_22,   (char*)"Userset22");
        const REGDATA g_sREGV_USET_Userset23    = REGDATA(RV_USET_USERSET_23,   (char*)"Userset23");
        const REGDATA g_sREGV_USET_Userset24    = REGDATA(RV_USET_USERSET_24,   (char*)"Userset24");
        const REGDATA g_sREGV_USET_Userset25    = REGDATA(RV_USET_USERSET_25,   (char*)"Userset25");
        const REGDATA g_sREGV_USET_Userset26    = REGDATA(RV_USET_USERSET_26,   (char*)"Userset26");
        const REGDATA g_sREGV_USET_Userset27    = REGDATA(RV_USET_USERSET_27,   (char*)"Userset27");
        const REGDATA g_sREGV_USET_Userset28    = REGDATA(RV_USET_USERSET_28,   (char*)"Userset28");
        const REGDATA g_sREGV_USET_Userset29    = REGDATA(RV_USET_USERSET_29,   (char*)"Userset29");
        const REGDATA g_sREGV_USET_Userset30    = REGDATA(RV_USET_USERSET_30,   (char*)"Userset30");
        const REGDATA g_sREGV_USET_Userset31    = REGDATA(RV_USET_USERSET_31,   (char*)"Userset31");

        const REGDATA g_saVAL_UsersetSel[] =
        {
            g_sREGV_USET_UsersetDflt,
            //g_sREGV_USET_Userset00,
            g_sREGV_USET_Userset01,
            g_sREGV_USET_Userset02,
            g_sREGV_USET_Userset03,
            g_sREGV_USET_Userset04,
            g_sREGV_USET_Userset05,
            g_sREGV_USET_Userset06,
            g_sREGV_USET_Userset07,
            g_sREGV_USET_Userset08,
            g_sREGV_USET_Userset09,
            g_sREGV_USET_Userset20,
            g_sREGV_USET_Userset11,
            g_sREGV_USET_Userset12,
            g_sREGV_USET_Userset13,
            g_sREGV_USET_Userset14,
            g_sREGV_USET_Userset15,
            g_sREGV_USET_Userset16,
            g_sREGV_USET_Userset17,
            g_sREGV_USET_Userset18,
            g_sREGV_USET_Userset19,
            g_sREGV_USET_Userset20,
            g_sREGV_USET_Userset21,
            g_sREGV_USET_Userset22,
            g_sREGV_USET_Userset23,
            g_sREGV_USET_Userset24,
            g_sREGV_USET_Userset25,
            g_sREGV_USET_Userset26,
            g_sREGV_USET_Userset27,
            g_sREGV_USET_Userset28,
            g_sREGV_USET_Userset29,
            g_sREGV_USET_Userset30,
            g_sREGV_USET_Userset31,
        };

        //=================================================================
        // Usersert Select command
        //=================================================================
       enum  REGVAL_USERSET_COMMAND
        {
            RV_USET_USERSET_CMD_DONE    = 0,        // Normal state  (0)
            RV_USET_USERSET_CMD_LOAD    = 8,        // Load  (8)
            RV_USET_USERSET_CMD_SAVE    = 9,        // Save  (9)
        };

        const REGDATA g_sREGV_UsersetCmd_Done   = REGDATA(RV_USET_USERSET_CMD_DONE, (char*)"Done");
        const REGDATA g_sREGV_UsersetCmd_Load   = REGDATA(RV_USET_USERSET_CMD_LOAD, (char*)"Load");
        const REGDATA g_sREGV_UsersetCmd_Save   = REGDATA(RV_USET_USERSET_CMD_SAVE, (char*)"Save");

        const REGDATA g_saVAL_UsersetCmd[] =
        {
            g_sREGV_UsersetCmd_Done,
            g_sREGV_UsersetCmd_Load,
            g_sREGV_UsersetCmd_Save,
        };


        //=================================================================
        // IO line selector
        //=================================================================
        enum  REGVAL_IOLINE_SEL
        {
            RV_DIO_IOLINE_00    = 0,                    // IO LINE0
            RV_DIO_IOLINE_01    = 1,                    // IO LINE1
            RV_DIO_IOLINE_02    = 2,                    // IO LINE2
            RV_DIO_IOLINE_03    = 3,                    // IO LINE3
            RV_DIO_IOLINE_04    = 4,                    // IO LINE4
            RV_DIO_IOLINE_05    = 5,                    // IO LINE5
            RV_DIO_IOLINE_06    = 6,                    // IO LINE6
            RV_DIO_IOLINE_07    = 7,                    // IO LINE7
            RV_DIO_IOLINE_08    = 8,                    // IO LINE8
            RV_DIO_IOLINE_09    = 9,                    // IO LINE9
            RV_DIO_IOLINE_10    = 10,                   // IO LINE10
            RV_DIO_IOLINE_11    = 11,                   // IO LINE11
            RV_DIO_IOLINE_12    = 12,                   // IO LINE12
            RV_DIO_IOLINE_13    = 13,                   // IO LINE13
            RV_DIO_IOLINE_14    = 14,                   // IO LINE14
            RV_DIO_IOLINE_15    = 15,                   // IO LINE15
            RV_DIO_IOLINE_16    = 16,                   // IO LINE16
            RV_DIO_IOLINE_17    = 17,                   // IO LINE17
            RV_DIO_IOLINE_18    = 18,                   // IO LINE18
            RV_DIO_IOLINE_19    = 19,                   // IO LINE19
            RV_DIO_IOLINE_20    = 20,                   // IO LINE20
            RV_DIO_IOLINE_21    = 21,                   // IO LINE21
            RV_DIO_IOLINE_22    = 22,                   // IO LINE22
            RV_DIO_IOLINE_23    = 23,                   // IO LINE23
            RV_DIO_IOLINE_24    = 24,                   // IO LINE24
            RV_DIO_IOLINE_25    = 25,                   // IO LINE25
            RV_DIO_IOLINE_26    = 26,                   // IO LINE26
            RV_DIO_IOLINE_27    = 27,                   // IO LINE27
            RV_DIO_IOLINE_28    = 28,                   // IO LINE28
            RV_DIO_IOLINE_29    = 29,                   // IO LINE29
            RV_DIO_IOLINE_30    = 30,                   // IO LINE30
            RV_DIO_IOLINE_31    = 31,                   // IO LINE31
        };

        const REGDATA g_sREGV_DIO_IOLine00  = REGDATA(RV_DIO_IOLINE_00, (char*)"IOLine0");
        const REGDATA g_sREGV_DIO_IOLine01  = REGDATA(RV_DIO_IOLINE_01, (char*)"IOLine1");
        const REGDATA g_sREGV_DIO_IOLine02  = REGDATA(RV_DIO_IOLINE_02, (char*)"IOLine2");
        const REGDATA g_sREGV_DIO_IOLine03  = REGDATA(RV_DIO_IOLINE_03, (char*)"IOLine3");
        const REGDATA g_sREGV_DIO_IOLine04  = REGDATA(RV_DIO_IOLINE_04, (char*)"IOLine4");
        const REGDATA g_sREGV_DIO_IOLine05  = REGDATA(RV_DIO_IOLINE_05, (char*)"IOLine5");
        const REGDATA g_sREGV_DIO_IOLine06  = REGDATA(RV_DIO_IOLINE_06, (char*)"IOLine6");
        const REGDATA g_sREGV_DIO_IOLine07  = REGDATA(RV_DIO_IOLINE_07, (char*)"IOLine7");
        const REGDATA g_sREGV_DIO_IOLine08  = REGDATA(RV_DIO_IOLINE_08, (char*)"IOLine8");
        const REGDATA g_sREGV_DIO_IOLine09  = REGDATA(RV_DIO_IOLINE_09, (char*)"IOLine9");
        const REGDATA g_sREGV_DIO_IOLine10  = REGDATA(RV_DIO_IOLINE_10, (char*)"IOLine10");
        const REGDATA g_sREGV_DIO_IOLine11  = REGDATA(RV_DIO_IOLINE_11, (char*)"IOLine11");
        const REGDATA g_sREGV_DIO_IOLine12  = REGDATA(RV_DIO_IOLINE_12, (char*)"IOLine12");
        const REGDATA g_sREGV_DIO_IOLine13  = REGDATA(RV_DIO_IOLINE_13, (char*)"IOLine13");
        const REGDATA g_sREGV_DIO_IOLine14  = REGDATA(RV_DIO_IOLINE_14, (char*)"IOLine14");
        const REGDATA g_sREGV_DIO_IOLine15  = REGDATA(RV_DIO_IOLINE_15, (char*)"IOLine15");
        const REGDATA g_sREGV_DIO_IOLine16  = REGDATA(RV_DIO_IOLINE_16, (char*)"IOLine16");
        const REGDATA g_sREGV_DIO_IOLine17  = REGDATA(RV_DIO_IOLINE_17, (char*)"IOLine17");
        const REGDATA g_sREGV_DIO_IOLine18  = REGDATA(RV_DIO_IOLINE_18, (char*)"IOLine18");
        const REGDATA g_sREGV_DIO_IOLine19  = REGDATA(RV_DIO_IOLINE_19, (char*)"IOLine19");
        const REGDATA g_sREGV_DIO_IOLine20  = REGDATA(RV_DIO_IOLINE_20, (char*)"IOLine20");
        const REGDATA g_sREGV_DIO_IOLine21  = REGDATA(RV_DIO_IOLINE_21, (char*)"IOLine21");
        const REGDATA g_sREGV_DIO_IOLine22  = REGDATA(RV_DIO_IOLINE_22, (char*)"IOLine22");
        const REGDATA g_sREGV_DIO_IOLine23  = REGDATA(RV_DIO_IOLINE_23, (char*)"IOLine23");
        const REGDATA g_sREGV_DIO_IOLine24  = REGDATA(RV_DIO_IOLINE_24, (char*)"IOLine24");
        const REGDATA g_sREGV_DIO_IOLine25  = REGDATA(RV_DIO_IOLINE_25, (char*)"IOLine25");
        const REGDATA g_sREGV_DIO_IOLine26  = REGDATA(RV_DIO_IOLINE_26, (char*)"IOLine26");
        const REGDATA g_sREGV_DIO_IOLine27  = REGDATA(RV_DIO_IOLINE_27, (char*)"IOLine27");
        const REGDATA g_sREGV_DIO_IOLine28  = REGDATA(RV_DIO_IOLINE_28, (char*)"IOLine28");
        const REGDATA g_sREGV_DIO_IOLine29  = REGDATA(RV_DIO_IOLINE_29, (char*)"IOLine29");
        const REGDATA g_sREGV_DIO_IOLine30  = REGDATA(RV_DIO_IOLINE_30, (char*)"IOLine30");
        const REGDATA g_sREGV_DIO_IOLine31  = REGDATA(RV_DIO_IOLINE_31, (char*)"IOLine31");

        const REGDATA g_saVAL_IOLineSel[] =
        {
            g_sREGV_DIO_IOLine00,
            g_sREGV_DIO_IOLine01,
            g_sREGV_DIO_IOLine02,
            g_sREGV_DIO_IOLine03,
            g_sREGV_DIO_IOLine04,
            g_sREGV_DIO_IOLine05,
            g_sREGV_DIO_IOLine06,
            g_sREGV_DIO_IOLine07,
            g_sREGV_DIO_IOLine08,
            g_sREGV_DIO_IOLine09,
            g_sREGV_DIO_IOLine20,
            g_sREGV_DIO_IOLine11,
            g_sREGV_DIO_IOLine12,
            g_sREGV_DIO_IOLine13,
            g_sREGV_DIO_IOLine14,
            g_sREGV_DIO_IOLine15,
            g_sREGV_DIO_IOLine16,
            g_sREGV_DIO_IOLine17,
            g_sREGV_DIO_IOLine18,
            g_sREGV_DIO_IOLine19,
            g_sREGV_DIO_IOLine20,
            g_sREGV_DIO_IOLine21,
            g_sREGV_DIO_IOLine22,
            g_sREGV_DIO_IOLine23,
            g_sREGV_DIO_IOLine24,
            g_sREGV_DIO_IOLine25,
            g_sREGV_DIO_IOLine26,
            g_sREGV_DIO_IOLine27,
            g_sREGV_DIO_IOLine28,
            g_sREGV_DIO_IOLine29,
            g_sREGV_DIO_IOLine30,
            g_sREGV_DIO_IOLine31,
        };

        //=================================================================
        // IO line source  
        //=================================================================
       enum  REGVAL_IOLINE_SOURCE
        {
            RV_DIO_SRC_OFF          = 0,        // Off  (0)
            RV_DIO_SRC_HD           = 16,       // Horizontal driving signal  (16)
            RV_DIO_SRC_VD           = 17,       // Vertical driving signal  (17)
            RV_DIO_SRC_USEROUTPUT   = 32,       // User output (32)
            RV_DIO_SRC_TMR00_ACTIVE = 64,       // Timer00Active  (64)
            RV_DIO_SRC_TMR01_ACTIVE = 65,       // Timer01Active  (65)
            RV_DIO_SRC_TMR02_ACTIVE = 66,       // Timer02Active  (66)
            RV_DIO_SRC_TMR03_ACTIVE = 67,       // Timer03Active  (67)
            RV_DIO_SRC_TMR04_ACTIVE = 68,       // Timer04Active  (68)
            RV_DIO_SRC_TMR05_ACTIVE = 69,       // Timer05Active  (69)
            RV_DIO_SRC_TMR06_ACTIVE = 70,       // Timer06Active  (70)
            RV_DIO_SRC_TMR07_ACTIVE = 71,       // Timer07Active  (71)
            RV_DIO_SRC_TMR08_ACTIVE = 72,       // Timer08Active  (72)
            RV_DIO_SRC_TMR09_ACTIVE = 73,       // Timer09Active  (73)
            RV_DIO_SRC_TMR10_ACTIVE = 74,       // Timer10Active  (74)
            RV_DIO_SRC_TMR11_ACTIVE = 75,       // Timer11Active  (75)
            RV_DIO_SRC_TMR12_ACTIVE = 76,       // Timer12Active  (76)
            RV_DIO_SRC_TMR13_ACTIVE = 77,       // Timer13Active  (77)
            RV_DIO_SRC_TMR14_ACTIVE = 78,       // Timer14Active  (78)
            RV_DIO_SRC_TMR15_ACTIVE = 79,       // Timer15Active  (79)
            RV_DIO_SRC_TMR16_ACTIVE = 80,       // Timer16Active  (80)
            RV_DIO_SRC_TMR17_ACTIVE = 81,       // Timer17Active  (81)
            RV_DIO_SRC_TMR18_ACTIVE = 82,       // Timer18Active  (82)
            RV_DIO_SRC_TMR19_ACTIVE = 83,       // Timer19Active  (83)
            RV_DIO_SRC_TMR20_ACTIVE = 84,       // Timer20Active  (84)
            RV_DIO_SRC_TMR21_ACTIVE = 85,       // Timer21Active  (85)
            RV_DIO_SRC_TMR22_ACTIVE = 86,       // Timer22Active  (86)
            RV_DIO_SRC_TMR23_ACTIVE = 87,       // Timer23Active  (87)
            RV_DIO_SRC_TMR24_ACTIVE = 88,       // Timer24Active  (88)
            RV_DIO_SRC_TMR25_ACTIVE = 89,       // Timer25Active  (89)
            RV_DIO_SRC_TMR26_ACTIVE = 90,       // Timer26Active  (90)
            RV_DIO_SRC_TMR27_ACTIVE = 91,       // Timer27Active  (91)
            RV_DIO_SRC_TMR28_ACTIVE = 92,       // Timer28Active  (92)
            RV_DIO_SRC_TMR29_ACTIVE = 93,       // Timer29Active  (83)
            RV_DIO_SRC_TMR30_ACTIVE = 94,       // Timer31Active  (94)
            RV_DIO_SRC_TMR31_ACTIVE = 95,       // Timer32Active  (95)
            RV_DIO_SRC_ACQ_TRG_WAIT = 98,       // AcquisitionTriggerWait  (98)
            RV_DIO_SRC_ACQ_ACTIVE   = 99,       // AcquisitionActive  (99)
            RV_DIO_SRC_FRM_TRG_WAIT = 106,      // FrameTriggerWait  (106)
            RV_DIO_SRC_FRM_ACTIVE   = 107,      // FrameActive  (107)
            RV_DIO_SRC_FRM_TRNS_ACT = 115,      // FrameTransferActive (115)
            RV_DIO_SRC_EXP_ACTIVE   = 123,      // ExposureActive  (107)
        };

        const REGDATA g_sREGV_IOSrc_Off             = REGDATA(RV_DIO_SRC_OFF,           (char*)"Off");
        const REGDATA g_sREGV_IOSrc_HD              = REGDATA(RV_DIO_SRC_HD,            (char*)"HD");
        const REGDATA g_sREGV_IOSrc_VD              = REGDATA(RV_DIO_SRC_VD,            (char*)"VD");
        const REGDATA g_sREGV_IOSrc_UserOutput      = REGDATA(RV_DIO_SRC_USEROUTPUT,    (char*)"UserOutput");
        const REGDATA g_sREGV_IOSrc_Tmr00Active     = REGDATA(RV_DIO_SRC_TMR00_ACTIVE,  (char*)"Timer0Active");
        const REGDATA g_sREGV_IOSrc_Tmr01Active     = REGDATA(RV_DIO_SRC_TMR01_ACTIVE,  (char*)"Timer1Active");
        const REGDATA g_sREGV_IOSrc_Tmr02Active     = REGDATA(RV_DIO_SRC_TMR02_ACTIVE,  (char*)"Timer2Active");
        const REGDATA g_sREGV_IOSrc_Tmr03Active     = REGDATA(RV_DIO_SRC_TMR03_ACTIVE,  (char*)"Timer3Active");
        const REGDATA g_sREGV_IOSrc_Tmr04Active     = REGDATA(RV_DIO_SRC_TMR04_ACTIVE,  (char*)"Timer4Active");
        const REGDATA g_sREGV_IOSrc_Tmr05Active     = REGDATA(RV_DIO_SRC_TMR05_ACTIVE,  (char*)"Timer5Active");
        const REGDATA g_sREGV_IOSrc_Tmr06Active     = REGDATA(RV_DIO_SRC_TMR06_ACTIVE,  (char*)"Timer6Active");
        const REGDATA g_sREGV_IOSrc_Tmr07Active     = REGDATA(RV_DIO_SRC_TMR07_ACTIVE,  (char*)"Timer7Active");
        const REGDATA g_sREGV_IOSrc_Tmr08Active     = REGDATA(RV_DIO_SRC_TMR08_ACTIVE,  (char*)"Timer8Active");
        const REGDATA g_sREGV_IOSrc_Tmr09Active     = REGDATA(RV_DIO_SRC_TMR09_ACTIVE,  (char*)"Timer9Active");
        const REGDATA g_sREGV_IOSrc_Tmr10Active     = REGDATA(RV_DIO_SRC_TMR10_ACTIVE,  (char*)"Timer10Active");
        const REGDATA g_sREGV_IOSrc_Tmr11Active     = REGDATA(RV_DIO_SRC_TMR11_ACTIVE,  (char*)"Timer11Active");
        const REGDATA g_sREGV_IOSrc_Tmr12Active     = REGDATA(RV_DIO_SRC_TMR12_ACTIVE,  (char*)"Timer12Active");
        const REGDATA g_sREGV_IOSrc_Tmr13Active     = REGDATA(RV_DIO_SRC_TMR13_ACTIVE,  (char*)"Timer13Active");
        const REGDATA g_sREGV_IOSrc_Tmr14Active     = REGDATA(RV_DIO_SRC_TMR14_ACTIVE,  (char*)"Timer14Active");
        const REGDATA g_sREGV_IOSrc_Tmr15Active     = REGDATA(RV_DIO_SRC_TMR15_ACTIVE,  (char*)"Timer15Active");
        const REGDATA g_sREGV_IOSrc_Tmr16Active     = REGDATA(RV_DIO_SRC_TMR16_ACTIVE,  (char*)"Timer16Active");
        const REGDATA g_sREGV_IOSrc_Tmr17Active     = REGDATA(RV_DIO_SRC_TMR17_ACTIVE,  (char*)"Timer17Active");
        const REGDATA g_sREGV_IOSrc_Tmr18Active     = REGDATA(RV_DIO_SRC_TMR18_ACTIVE,  (char*)"Timer18Active");
        const REGDATA g_sREGV_IOSrc_Tmr19Active     = REGDATA(RV_DIO_SRC_TMR19_ACTIVE,  (char*)"Timer19Active");
        const REGDATA g_sREGV_IOSrc_Tmr20Active     = REGDATA(RV_DIO_SRC_TMR20_ACTIVE,  (char*)"Timer20Active");
        const REGDATA g_sREGV_IOSrc_Tmr21Active     = REGDATA(RV_DIO_SRC_TMR21_ACTIVE,  (char*)"Timer21Active");
        const REGDATA g_sREGV_IOSrc_Tmr22Active     = REGDATA(RV_DIO_SRC_TMR22_ACTIVE,  (char*)"Timer22Active");
        const REGDATA g_sREGV_IOSrc_Tmr23Active     = REGDATA(RV_DIO_SRC_TMR23_ACTIVE,  (char*)"Timer23Active");
        const REGDATA g_sREGV_IOSrc_Tmr24Active     = REGDATA(RV_DIO_SRC_TMR24_ACTIVE,  (char*)"Timer24Active");
        const REGDATA g_sREGV_IOSrc_Tmr25Active     = REGDATA(RV_DIO_SRC_TMR25_ACTIVE,  (char*)"Timer25Active");
        const REGDATA g_sREGV_IOSrc_Tmr26Active     = REGDATA(RV_DIO_SRC_TMR26_ACTIVE,  (char*)"Timer26Active");
        const REGDATA g_sREGV_IOSrc_Tmr27Active     = REGDATA(RV_DIO_SRC_TMR27_ACTIVE,  (char*)"Timer27Active");
        const REGDATA g_sREGV_IOSrc_Tmr28Active     = REGDATA(RV_DIO_SRC_TMR28_ACTIVE,  (char*)"Timer28Active");
        const REGDATA g_sREGV_IOSrc_Tmr29Active     = REGDATA(RV_DIO_SRC_TMR29_ACTIVE,  (char*)"Timer29Active");
        const REGDATA g_sREGV_IOSrc_Tmr30Active     = REGDATA(RV_DIO_SRC_TMR30_ACTIVE,  (char*)"Timer30Active");
        const REGDATA g_sREGV_IOSrc_Tmr31Active     = REGDATA(RV_DIO_SRC_TMR31_ACTIVE,  (char*)"Timer31Active");
        const REGDATA g_sREGV_IOSrc_Acq_Trg_Wait    = REGDATA(RV_DIO_SRC_ACQ_TRG_WAIT,  (char*)"AcquisitionTriggerWai");
        const REGDATA g_sREGV_IOSrc_Acq_Active      = REGDATA(RV_DIO_SRC_ACQ_ACTIVE,    (char*)"AcquisitionActive");
        const REGDATA g_sREGV_IOSrc_Frame_Trg_Wart  = REGDATA(RV_DIO_SRC_FRM_TRG_WAIT,  (char*)"FrameTriggerWait");
        const REGDATA g_sREGV_IOSrc_Frame_Active    = REGDATA(RV_DIO_SRC_FRM_ACTIVE,    (char*)"FrameActive");
        const REGDATA g_sREGV_IOSrc_Frm_Trns_Active = REGDATA(RV_DIO_SRC_FRM_TRNS_ACT,  (char*)"FrameTransferActive");
        const REGDATA g_sREGV_IOSrc_Expo_Active     = REGDATA(RV_DIO_SRC_EXP_ACTIVE,    (char*)"ExposureActive");

        const REGDATA g_saVAL_IOLineSrc[] =
        {
            g_sREGV_IOSrc_Off,
            g_sREGV_IOSrc_HD,
            g_sREGV_IOSrc_VD,
            g_sREGV_IOSrc_UserOutput,
            g_sREGV_IOSrc_Tmr00Active,
            g_sREGV_IOSrc_Tmr01Active,
            g_sREGV_IOSrc_Tmr02Active,
            g_sREGV_IOSrc_Tmr03Active,
            g_sREGV_IOSrc_Tmr04Active,
            g_sREGV_IOSrc_Tmr05Active,
            g_sREGV_IOSrc_Tmr06Active,
            g_sREGV_IOSrc_Tmr07Active,
            g_sREGV_IOSrc_Tmr08Active,
            g_sREGV_IOSrc_Tmr09Active,
            g_sREGV_IOSrc_Tmr10Active,
            g_sREGV_IOSrc_Tmr11Active,
            g_sREGV_IOSrc_Tmr12Active,
            g_sREGV_IOSrc_Tmr13Active,
            g_sREGV_IOSrc_Tmr14Active,
            g_sREGV_IOSrc_Tmr15Active,
            g_sREGV_IOSrc_Tmr16Active,
            g_sREGV_IOSrc_Tmr17Active,
            g_sREGV_IOSrc_Tmr18Active,
            g_sREGV_IOSrc_Tmr19Active,
            g_sREGV_IOSrc_Tmr20Active,
            g_sREGV_IOSrc_Tmr21Active,
            g_sREGV_IOSrc_Tmr22Active,
            g_sREGV_IOSrc_Tmr23Active,
            g_sREGV_IOSrc_Tmr24Active,
            g_sREGV_IOSrc_Tmr25Active,
            g_sREGV_IOSrc_Tmr26Active,
            g_sREGV_IOSrc_Tmr27Active,
            g_sREGV_IOSrc_Tmr28Active,
            g_sREGV_IOSrc_Tmr29Active,
            g_sREGV_IOSrc_Tmr30Active,
            g_sREGV_IOSrc_Tmr31Active,
            g_sREGV_IOSrc_Acq_Trg_Wait,
            g_sREGV_IOSrc_Acq_Active,
            g_sREGV_IOSrc_Frame_Trg_Wart,
            g_sREGV_IOSrc_Frame_Active,
            g_sREGV_IOSrc_Frm_Trns_Active,
            g_sREGV_IOSrc_Expo_Active,
        };


        //=================================================================
        // IO Timer selector 
        //=================================================================
        enum  REGVAL_TIMER_SELECTOR
        {
            RV_CNTTM_TIMER_00   = 0,                    // TIMER0
            RV_CNTTM_TIMER_01   = 1,                    // TIMER1
            RV_CNTTM_TIMER_02   = 2,                    // TIMER2
            RV_CNTTM_TIMER_03   = 3,                    // TIMER3
            RV_CNTTM_TIMER_04   = 4,                    // TIMER4
            RV_CNTTM_TIMER_05   = 5,                    // TIMER5
            RV_CNTTM_TIMER_06   = 6,                    // TIMER6
            RV_CNTTM_TIMER_07   = 7,                    // TIMER7
            RV_CNTTM_TIMER_08   = 8,                    // TIMER8
            RV_CNTTM_TIMER_09   = 9,                    // TIMER9
            RV_CNTTM_TIMER_10   = 10,                   // TIMER10
            RV_CNTTM_TIMER_11   = 11,                   // TIMER11
            RV_CNTTM_TIMER_12   = 12,                   // TIMER12
            RV_CNTTM_TIMER_13   = 13,                   // TIMER13
            RV_CNTTM_TIMER_14   = 14,                   // TIMER14
            RV_CNTTM_TIMER_15   = 15,                   // TIMER15
            RV_CNTTM_TIMER_16   = 16,                   // TIMER16
            RV_CNTTM_TIMER_17   = 17,                   // TIMER17
            RV_CNTTM_TIMER_18   = 18,                   // TIMER18
            RV_CNTTM_TIMER_19   = 19,                   // TIMER19
            RV_CNTTM_TIMER_20   = 20,                   // TIMER20
            RV_CNTTM_TIMER_21   = 21,                   // TIMER21
            RV_CNTTM_TIMER_22   = 22,                   // TIMER22
            RV_CNTTM_TIMER_23   = 23,                   // TIMER23
            RV_CNTTM_TIMER_24   = 24,                   // TIMER24
            RV_CNTTM_TIMER_25   = 25,                   // TIMER25
            RV_CNTTM_TIMER_26   = 26,                   // TIMER26
            RV_CNTTM_TIMER_27   = 27,                   // TIMER27
            RV_CNTTM_TIMER_28   = 28,                   // TIMER28
            RV_CNTTM_TIMER_29   = 29,                   // TIMER29
            RV_CNTTM_TIMER_30   = 30,                   // TIMER30
            RV_CNTTM_TIMER_31   = 31,                   // TIMER31
        };

        const REGDATA g_sREGV_CntTm_Timer00 = REGDATA(RV_CNTTM_TIMER_00,    (char*)"Timer0");
        const REGDATA g_sREGV_CntTm_Timer01 = REGDATA(RV_CNTTM_TIMER_01,    (char*)"Timer1");
        const REGDATA g_sREGV_CntTm_Timer02 = REGDATA(RV_CNTTM_TIMER_02,    (char*)"Timer2");
        const REGDATA g_sREGV_CntTm_Timer03 = REGDATA(RV_CNTTM_TIMER_03,    (char*)"Timer3");
        const REGDATA g_sREGV_CntTm_Timer04 = REGDATA(RV_CNTTM_TIMER_04,    (char*)"Timer4");
        const REGDATA g_sREGV_CntTm_Timer05 = REGDATA(RV_CNTTM_TIMER_05,    (char*)"Timer5");
        const REGDATA g_sREGV_CntTm_Timer06 = REGDATA(RV_CNTTM_TIMER_06,    (char*)"Timer6");
        const REGDATA g_sREGV_CntTm_Timer07 = REGDATA(RV_CNTTM_TIMER_07,    (char*)"Timer7");
        const REGDATA g_sREGV_CntTm_Timer08 = REGDATA(RV_CNTTM_TIMER_08,    (char*)"Timer8");
        const REGDATA g_sREGV_CntTm_Timer09 = REGDATA(RV_CNTTM_TIMER_09,    (char*)"Timer9");
        const REGDATA g_sREGV_CntTm_Timer10 = REGDATA(RV_CNTTM_TIMER_10,    (char*)"Timer10");
        const REGDATA g_sREGV_CntTm_Timer11 = REGDATA(RV_CNTTM_TIMER_11,    (char*)"Timer11");
        const REGDATA g_sREGV_CntTm_Timer12 = REGDATA(RV_CNTTM_TIMER_12,    (char*)"Timer12");
        const REGDATA g_sREGV_CntTm_Timer13 = REGDATA(RV_CNTTM_TIMER_13,    (char*)"Timer13");
        const REGDATA g_sREGV_CntTm_Timer14 = REGDATA(RV_CNTTM_TIMER_14,    (char*)"Timer14");
        const REGDATA g_sREGV_CntTm_Timer15 = REGDATA(RV_CNTTM_TIMER_15,    (char*)"Timer15");
        const REGDATA g_sREGV_CntTm_Timer16 = REGDATA(RV_CNTTM_TIMER_16,    (char*)"Timer16");
        const REGDATA g_sREGV_CntTm_Timer17 = REGDATA(RV_CNTTM_TIMER_17,    (char*)"Timer17");
        const REGDATA g_sREGV_CntTm_Timer18 = REGDATA(RV_CNTTM_TIMER_18,    (char*)"Timer18");
        const REGDATA g_sREGV_CntTm_Timer19 = REGDATA(RV_CNTTM_TIMER_19,    (char*)"Timer19");
        const REGDATA g_sREGV_CntTm_Timer20 = REGDATA(RV_CNTTM_TIMER_20,    (char*)"Timer20");
        const REGDATA g_sREGV_CntTm_Timer21 = REGDATA(RV_CNTTM_TIMER_21,    (char*)"Timer21");
        const REGDATA g_sREGV_CntTm_Timer22 = REGDATA(RV_CNTTM_TIMER_22,    (char*)"Timer22");
        const REGDATA g_sREGV_CntTm_Timer23 = REGDATA(RV_CNTTM_TIMER_23,    (char*)"Timer23");
        const REGDATA g_sREGV_CntTm_Timer24 = REGDATA(RV_CNTTM_TIMER_24,    (char*)"Timer24");
        const REGDATA g_sREGV_CntTm_Timer25 = REGDATA(RV_CNTTM_TIMER_25,    (char*)"Timer25");
        const REGDATA g_sREGV_CntTm_Timer26 = REGDATA(RV_CNTTM_TIMER_26,    (char*)"Timer26");
        const REGDATA g_sREGV_CntTm_Timer27 = REGDATA(RV_CNTTM_TIMER_27,    (char*)"Timer27");
        const REGDATA g_sREGV_CntTm_Timer28 = REGDATA(RV_CNTTM_TIMER_28,    (char*)"Timer28");
        const REGDATA g_sREGV_CntTm_Timer29 = REGDATA(RV_CNTTM_TIMER_29,    (char*)"Timer29");
        const REGDATA g_sREGV_CntTm_Timer30 = REGDATA(RV_CNTTM_TIMER_30,    (char*)"Timer30");
        const REGDATA g_sREGV_CntTm_Timer31 = REGDATA(RV_CNTTM_TIMER_31,    (char*)"Timer31");

        const REGDATA g_saVAL_TimerSel[] =
        {
            g_sREGV_CntTm_Timer00,
            g_sREGV_CntTm_Timer01,
            g_sREGV_CntTm_Timer02,
            g_sREGV_CntTm_Timer03,
            g_sREGV_CntTm_Timer04,
            g_sREGV_CntTm_Timer05,
            g_sREGV_CntTm_Timer06,
            g_sREGV_CntTm_Timer07,
            g_sREGV_CntTm_Timer08,
            g_sREGV_CntTm_Timer09,
            g_sREGV_CntTm_Timer20,
            g_sREGV_CntTm_Timer11,
            g_sREGV_CntTm_Timer12,
            g_sREGV_CntTm_Timer13,
            g_sREGV_CntTm_Timer14,
            g_sREGV_CntTm_Timer15,
            g_sREGV_CntTm_Timer16,
            g_sREGV_CntTm_Timer17,
            g_sREGV_CntTm_Timer18,
            g_sREGV_CntTm_Timer19,
            g_sREGV_CntTm_Timer20,
            g_sREGV_CntTm_Timer21,
            g_sREGV_CntTm_Timer22,
            g_sREGV_CntTm_Timer23,
            g_sREGV_CntTm_Timer24,
            g_sREGV_CntTm_Timer25,
            g_sREGV_CntTm_Timer26,
            g_sREGV_CntTm_Timer27,
            g_sREGV_CntTm_Timer28,
            g_sREGV_CntTm_Timer29,
            g_sREGV_CntTm_Timer30,
            g_sREGV_CntTm_Timer31,
        };



        //=================================================================
        // Frame synchronization 
        //=================================================================
        enum  REGVAL_FRAME_SYNCHRO
        {
            RV_VNDR_FRMSYNC_OFF     = 0,        // Off  (0)
            RV_VNDR_FRMSYNC_BUS     = 1,        // Bus  (1)
            RV_VNDR_FRMSYNC_LINE_00 = 32,       // IO Line 00  (32)
            RV_VNDR_FRMSYNC_LINE_01 = 33,       // IO Line 01  (33)
            RV_VNDR_FRMSYNC_LINE_02 = 34,       // IO Line 02  (34)
            RV_VNDR_FRMSYNC_LINE_03 = 35,       // IO Line 03  (35)
            RV_VNDR_FRMSYNC_LINE_04 = 36,       // IO Line 04  (36)
            RV_VNDR_FRMSYNC_LINE_05 = 37,       // IO Line 05  (37)
            RV_VNDR_FRMSYNC_LINE_06 = 38,       // IO Line 06  (38)
            RV_VNDR_FRMSYNC_LINE_07 = 39,       // IO Line 07  (39)
            RV_VNDR_FRMSYNC_LINE_08 = 40,       // IO Line 08  (40)
            RV_VNDR_FRMSYNC_LINE_09 = 41,       // IO Line 09  (41)
            RV_VNDR_FRMSYNC_LINE_10 = 42,       // IO Line 10  (42)
            RV_VNDR_FRMSYNC_LINE_11 = 43,       // IO Line 11  (43)
            RV_VNDR_FRMSYNC_LINE_12 = 44,       // IO Line 12  (44)
            RV_VNDR_FRMSYNC_LINE_13 = 45,       // IO Line 13  (45)
            RV_VNDR_FRMSYNC_LINE_14 = 46,       // IO Line 14  (46)
            RV_VNDR_FRMSYNC_LINE_15 = 47,       // IO Line 15  (47)
            RV_VNDR_FRMSYNC_LINE_16 = 48,       // IO Line 16  (48)
            RV_VNDR_FRMSYNC_LINE_17 = 49,       // IO Line 17  (49)
            RV_VNDR_FRMSYNC_LINE_18 = 50,       // IO Line 18  (50)
            RV_VNDR_FRMSYNC_LINE_19 = 51,       // IO Line 19  (51)
            RV_VNDR_FRMSYNC_LINE_20 = 52,       // IO Line 20  (52)
            RV_VNDR_FRMSYNC_LINE_21 = 53,       // IO Line 21  (53)
            RV_VNDR_FRMSYNC_LINE_22 = 54,       // IO Line 22  (54)
            RV_VNDR_FRMSYNC_LINE_23 = 55,       // IO Line 23  (55)
            RV_VNDR_FRMSYNC_LINE_24 = 56,       // IO Line 24  (56)
            RV_VNDR_FRMSYNC_LINE_25 = 57,       // IO Line 25  (57)
            RV_VNDR_FRMSYNC_LINE_26 = 58,       // IO Line 26  (58)
            RV_VNDR_FRMSYNC_LINE_27 = 59,       // IO Line 27  (59)
            RV_VNDR_FRMSYNC_LINE_28 = 60,       // IO Line 28  (60)
            RV_VNDR_FRMSYNC_LINE_29 = 61,       // IO Line 29  (61)
            RV_VNDR_FRMSYNC_LINE_30 = 62,       // IO Line 30  (62)
            RV_VNDR_FRMSYNC_LINE_31 = 63,       // IO Line 31  (63)
        };

        const REGDATA g_sREGV_Vndr_FrmSync_Off      = REGDATA(RV_VNDR_FRMSYNC_OFF,      (char*)"Off");
        const REGDATA g_sREGV_Vndr_FrmSync_Bus      = REGDATA(RV_VNDR_FRMSYNC_BUS,      (char*)"Bus");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_00  = REGDATA(RV_VNDR_FRMSYNC_LINE_00,  (char*)"Line0");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_01  = REGDATA(RV_VNDR_FRMSYNC_LINE_01,  (char*)"Line1");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_02  = REGDATA(RV_VNDR_FRMSYNC_LINE_02,  (char*)"Line2");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_03  = REGDATA(RV_VNDR_FRMSYNC_LINE_03,  (char*)"Line3");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_04  = REGDATA(RV_VNDR_FRMSYNC_LINE_04,  (char*)"Line4");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_05  = REGDATA(RV_VNDR_FRMSYNC_LINE_05,  (char*)"Line5");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_06  = REGDATA(RV_VNDR_FRMSYNC_LINE_06,  (char*)"Line6");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_07  = REGDATA(RV_VNDR_FRMSYNC_LINE_07,  (char*)"Line7");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_08  = REGDATA(RV_VNDR_FRMSYNC_LINE_08,  (char*)"Line8");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_09  = REGDATA(RV_VNDR_FRMSYNC_LINE_09,  (char*)"Line9");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_10  = REGDATA(RV_VNDR_FRMSYNC_LINE_10,  (char*)"Line10");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_11  = REGDATA(RV_VNDR_FRMSYNC_LINE_11,  (char*)"Line11");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_12  = REGDATA(RV_VNDR_FRMSYNC_LINE_12,  (char*)"Line12");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_13  = REGDATA(RV_VNDR_FRMSYNC_LINE_13,  (char*)"Line13");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_14  = REGDATA(RV_VNDR_FRMSYNC_LINE_14,  (char*)"Line14");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_15  = REGDATA(RV_VNDR_FRMSYNC_LINE_15,  (char*)"Line15");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_16  = REGDATA(RV_VNDR_FRMSYNC_LINE_16,  (char*)"Line16");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_17  = REGDATA(RV_VNDR_FRMSYNC_LINE_17,  (char*)"Line17");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_18  = REGDATA(RV_VNDR_FRMSYNC_LINE_18,  (char*)"Line18");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_19  = REGDATA(RV_VNDR_FRMSYNC_LINE_19,  (char*)"Line19");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_20  = REGDATA(RV_VNDR_FRMSYNC_LINE_20,  (char*)"Line20");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_21  = REGDATA(RV_VNDR_FRMSYNC_LINE_21,  (char*)"Line21");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_22  = REGDATA(RV_VNDR_FRMSYNC_LINE_22,  (char*)"Line22");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_23  = REGDATA(RV_VNDR_FRMSYNC_LINE_23,  (char*)"Line23");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_24  = REGDATA(RV_VNDR_FRMSYNC_LINE_24,  (char*)"Line24");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_25  = REGDATA(RV_VNDR_FRMSYNC_LINE_25,  (char*)"Line25");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_26  = REGDATA(RV_VNDR_FRMSYNC_LINE_26,  (char*)"Line26");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_27  = REGDATA(RV_VNDR_FRMSYNC_LINE_27,  (char*)"Line27");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_28  = REGDATA(RV_VNDR_FRMSYNC_LINE_28,  (char*)"Line28");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_29  = REGDATA(RV_VNDR_FRMSYNC_LINE_29,  (char*)"Line29");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_30  = REGDATA(RV_VNDR_FRMSYNC_LINE_30,  (char*)"Line30");
        const REGDATA g_sREGV_Vndr_FrmSync_Line_31  = REGDATA(RV_VNDR_FRMSYNC_LINE_31,  (char*)"Line31");
    
        const REGDATA g_saVAL_FrmSync[] =
        {
            g_sREGV_Vndr_FrmSync_Off,
            g_sREGV_Vndr_FrmSync_Bus,
            g_sREGV_Vndr_FrmSync_Line_00,
            g_sREGV_Vndr_FrmSync_Line_01,
            g_sREGV_Vndr_FrmSync_Line_02,
            g_sREGV_Vndr_FrmSync_Line_03,
            g_sREGV_Vndr_FrmSync_Line_04,
            g_sREGV_Vndr_FrmSync_Line_05,
            g_sREGV_Vndr_FrmSync_Line_06,
            g_sREGV_Vndr_FrmSync_Line_07,
            g_sREGV_Vndr_FrmSync_Line_08,
            g_sREGV_Vndr_FrmSync_Line_09,
            g_sREGV_Vndr_FrmSync_Line_10,
            g_sREGV_Vndr_FrmSync_Line_11,
            g_sREGV_Vndr_FrmSync_Line_12,
            g_sREGV_Vndr_FrmSync_Line_13,
            g_sREGV_Vndr_FrmSync_Line_14,
            g_sREGV_Vndr_FrmSync_Line_15,
            g_sREGV_Vndr_FrmSync_Line_16,
            g_sREGV_Vndr_FrmSync_Line_17,
            g_sREGV_Vndr_FrmSync_Line_18,
            g_sREGV_Vndr_FrmSync_Line_19,
            g_sREGV_Vndr_FrmSync_Line_20,
            g_sREGV_Vndr_FrmSync_Line_21,
            g_sREGV_Vndr_FrmSync_Line_22,
            g_sREGV_Vndr_FrmSync_Line_23,
            g_sREGV_Vndr_FrmSync_Line_24,
            g_sREGV_Vndr_FrmSync_Line_25,
            g_sREGV_Vndr_FrmSync_Line_26,
            g_sREGV_Vndr_FrmSync_Line_27,
            g_sREGV_Vndr_FrmSync_Line_28,
            g_sREGV_Vndr_FrmSync_Line_29,
            g_sREGV_Vndr_FrmSync_Line_30,
            g_sREGV_Vndr_FrmSync_Line_31,
        };

        //=================================================================
        // Test pattern
        //=================================================================
        enum  REGVAL_TEST_PATTERN
        {
            RV_VNDR_TEST_PTN_OFF        = 0,        // Off  (0)
            RV_VNDR_TEST_PTN_BLACK      = 1,        // Black solid pattern  (1)
            RV_VNDR_TEST_PTN_WHITE      = 2,        // white solid pattern  (2)
            RV_VNDR_TEST_PTN_GRAY_A     = 3,        // Gray A solid pattern  (3)
            RV_VNDR_TEST_PTN_GRAY_B     = 4,        // Gray B solid pattern  (4)
            RV_VNDR_TEST_PTN_RAMP       = 5,        // Ramp image  (5)
            RV_VNDR_TEST_PTN_GRAYSCALE  = 6,        // Grayscale pattern  (6)
            RV_VNDR_TEST_PTN_COLOR_BAR  = 7,        // Color bar pattern  (7)
        };

        const REGDATA g_sREGV_Vndr_TestPtn_Off          = REGDATA(RV_VNDR_TEST_PTN_OFF,         (char*)"Off");
        const REGDATA g_sREGV_Vndr_TestPtn_Black        = REGDATA(RV_VNDR_TEST_PTN_BLACK,       (char*)"Black");
        const REGDATA g_sREGV_Vndr_TestPtn_White        = REGDATA(RV_VNDR_TEST_PTN_WHITE,       (char*)"White");
        const REGDATA g_sREGV_Vndr_TestPtn_Gray_A       = REGDATA(RV_VNDR_TEST_PTN_GRAY_A,      (char*)"Gray_A");
        const REGDATA g_sREGV_Vndr_TestPtn_Gray_B       = REGDATA(RV_VNDR_TEST_PTN_GRAY_B,      (char*)"Gray_B");
        const REGDATA g_sREGV_Vndr_TestPtn_Ramp         = REGDATA(RV_VNDR_TEST_PTN_RAMP,        (char*)"Ramp");
        const REGDATA g_sREGV_Vndr_TestPtn_GrayScale    = REGDATA(RV_VNDR_TEST_PTN_GRAYSCALE,   (char*)"GrayScale");
        const REGDATA g_sREGV_Vndr_TestPtn_ColorBar     = REGDATA(RV_VNDR_TEST_PTN_COLOR_BAR,   (char*)"ColorBar");

        const REGDATA g_saVAL_TestPtn[] =
        {
            g_sREGV_Vndr_TestPtn_Off,
            g_sREGV_Vndr_TestPtn_Black,
            g_sREGV_Vndr_TestPtn_White,
            g_sREGV_Vndr_TestPtn_Gray_A,
            g_sREGV_Vndr_TestPtn_Gray_B,
            g_sREGV_Vndr_TestPtn_Ramp,
            g_sREGV_Vndr_TestPtn_GrayScale,
            g_sREGV_Vndr_TestPtn_ColorBar,
        };

        //=================================================================
        // ReverseX, Reverse Y 
        //=================================================================
        enum  REGVAL_REVERSE
        {
            RV_VNDR_REVERSE_NORMAL      = 0,            // Normal  (0)
            RV_VNDR_REVERSE_REVERSE     = 1,            // Reverse  (1)
        };

        const REGDATA g_sREGV_Vndr_Reverse_Normal   = REGDATA(RV_VNDR_REVERSE_NORMAL,   (char*)"Normal");
        const REGDATA g_sREGV_Vndr_Reverse_Reverse  = REGDATA(RV_VNDR_REVERSE_REVERSE,  (char*)"Reverse");

        const REGDATA g_saVAL_Reverse[] =
        {
            g_sREGV_Vndr_Reverse_Normal,
            g_sREGV_Vndr_Reverse_Reverse,
        };
    
    
        //=================================================================
        // Event Notification Of Acquisition
        //=================================================================
        enum  REGVAL_EVENT_ACQUISITION
        {
            RV_VNDR_EVNT_ACQUIS_NOTHING     = 0x00000000,   // Nothing  (0)
            RV_VNDR_EVNT_ACQUIS_TRIGGER     = 0x00000001,   // AcquisitionTrigger  (1)
            RV_VNDR_EVNT_ACQUIS_START       = 0x00000002,   // AcquisituinStart  (2)
            RV_VNDR_EVNT_ACQUIS_END         = 0x00000004,   // AcquisituinEnd  (4)
            RV_VNDR_EVNT_ACQUIS_TRNS_START  = 0x00000100,   // AcquisitionTransferStart (100)
            RV_VNDR_EVNT_ACQUIS_TRNS_END    = 0x00000200,   // AcquisitionTransferEnd (200)
            RV_VNDR_EVNT_ACQUIS_ERROR       = 0x00000400,   // AcquisitionError (400)
        };

        const REGDATA g_sREGV_Vndr_EventAcquis_Nothing      = REGDATA(RV_VNDR_EVNT_ACQUIS_NOTHING,      (char*)"Nothing");
        const REGDATA g_sREGV_Vndr_EventAcquis_Trigger      = REGDATA(RV_VNDR_EVNT_ACQUIS_TRIGGER,      (char*)"Trigger");
        const REGDATA g_sREGV_Vndr_EventAcquis_Start        = REGDATA(RV_VNDR_EVNT_ACQUIS_START,        (char*)"Start");
        const REGDATA g_sREGV_Vndr_EventAcquis_End          = REGDATA(RV_VNDR_EVNT_ACQUIS_END,          (char*)"End");
        const REGDATA g_sREGV_Vndr_EventAcquis_Trans_Start  = REGDATA(RV_VNDR_EVNT_ACQUIS_TRNS_START,   (char*)"TransferStart");
        const REGDATA g_sREGV_Vndr_EventAcquis_Trans_End    = REGDATA(RV_VNDR_EVNT_ACQUIS_TRNS_END,     (char*)"TransferEnd");
        const REGDATA g_sREGV_Vndr_EventAcquis_Error        = REGDATA(RV_VNDR_EVNT_ACQUIS_ERROR,        (char*)"Error");

        const REGDATA g_saVAL_EventAcquis[] =
        {
            g_sREGV_Vndr_EventAcquis_Nothing,
            g_sREGV_Vndr_EventAcquis_Trigger,
            g_sREGV_Vndr_EventAcquis_Start,
            g_sREGV_Vndr_EventAcquis_End,
            g_sREGV_Vndr_EventAcquis_Trans_Start,
            g_sREGV_Vndr_EventAcquis_Trans_End,
            g_sREGV_Vndr_EventAcquis_Error,
        };
    

        //=================================================================
        // Event Notification Of Frame
        //=================================================================
        enum  REGVAL_EVENT_FRAME
        {
            RV_VNDR_EVNT_FRM_NOTHING        = 0x00000000,   // Nothing  (0x00)
            RV_VNDR_EVNT_FRM_FRAME_TRIGGER  = 0x00000001,   // FrameTrigger  (0x01)
            RV_VNDR_EVNT_FRM_TRIGGER_ERROR  = 0x00000002,   // FrameTriggerError  (0x02)
            RV_VNDR_EVNT_FRM_TRIGGER_WAIT   = 0x00000004,   // FrameTriggerWait  (0x04)
            RV_VNDR_EVNT_FRM_FRAME_START    = 0x00000008,   // FrameStart  (0x08)
            RV_VNDR_EVNT_FRM_FRAME_END      = 0x00000010,   // FrameEnd  (0x10)
            RV_VNDR_EVNT_FRM_TRANSFER_START = 0x00010000,   // FrameTransferStart  (0x10000)
            RV_VNDR_EVNT_FRM_TRANSFER_END   = 0x00020000,   // FrameTransferEnd  (0x20000)
        };

        const REGDATA g_sREGV_Vndr_EventFrame_Nothing       = REGDATA(RV_VNDR_EVNT_FRM_NOTHING,         (char*)"Nothing");
        const REGDATA g_sREGV_Vndr_EventFrame_Frm_Trigger   = REGDATA(RV_VNDR_EVNT_FRM_FRAME_TRIGGER,   (char*)"FrameTrigger");
        const REGDATA g_sREGV_Vndr_EventFrame_Trg_Error     = REGDATA(RV_VNDR_EVNT_FRM_TRIGGER_ERROR,   (char*)"TriggerStart");
        const REGDATA g_sREGV_Vndr_EventFrame_Trg_Wait      = REGDATA(RV_VNDR_EVNT_FRM_TRIGGER_WAIT,    (char*)"TriggerWait");
        const REGDATA g_sREGV_Vndr_EventFrame_Start         = REGDATA(RV_VNDR_EVNT_FRM_FRAME_START,     (char*)"FrameStart");
        const REGDATA g_sREGV_Vndr_EventFrame_End           = REGDATA(RV_VNDR_EVNT_FRM_FRAME_END,       (char*)"FrameEnd");
        const REGDATA g_sREGV_Vndr_EventFrame_Trans_Start   = REGDATA(RV_VNDR_EVNT_FRM_TRANSFER_START,  (char*)"TransferStart");
        const REGDATA g_sREGV_Vndr_EventFrame_Trans_End     = REGDATA(RV_VNDR_EVNT_FRM_TRANSFER_END,    (char*)"TransferEnd");

        const REGDATA g_saVAL_EventFrame[] =
        {
            g_sREGV_Vndr_EventFrame_Nothing,
            g_sREGV_Vndr_EventFrame_Frm_Trigger,
            g_sREGV_Vndr_EventFrame_Trg_Error,
            g_sREGV_Vndr_EventFrame_Trg_Wait,
            g_sREGV_Vndr_EventFrame_Start,
            g_sREGV_Vndr_EventFrame_End,
            g_sREGV_Vndr_EventFrame_Trans_Start,
            g_sREGV_Vndr_EventFrame_Trans_End,
        };

        //=================================================================
        // Event Notification Of Exposure
        //=================================================================
        enum  REGVAL_EVENT_EXPOSURE
        {
            RV_VNDR_EVNT_EXPOSURE_NOTHING       = 0x00000000,       // Nothing  (0)
            RV_VNDR_EVNT_EXPOSURE_START         = 0x00000001,       // Start  (1)
            RV_VNDR_EVNT_EXPOSURE_END           = 0x00000002,       // End  (2)
        };

        const REGDATA g_sREGV_Vndr_EventExposure_Nothing    = REGDATA(RV_VNDR_EVNT_EXPOSURE_NOTHING,    (char*)"Nothing");
        const REGDATA g_sREGV_Vndr_EventExposure_Start      = REGDATA(RV_VNDR_EVNT_EXPOSURE_START,      (char*)"ExposureStart");
        const REGDATA g_sREGV_Vndr_EventExposure_End        = REGDATA(RV_VNDR_EVNT_EXPOSURE_END,        (char*)"ExposureEnd");


        const REGDATA g_saVAL_EventExposure[] =
        {
            g_sREGV_Vndr_EventExposure_Nothing, 
            g_sREGV_Vndr_EventExposure_Start,   
            g_sREGV_Vndr_EventExposure_End, 
        };

        //=================================================================
        // Timer trigger source
        //=================================================================
        enum  REGVAL_TIMER_TRIGGER_SOURCE
        {
            RV_VNDR_TIMER_TRG_SRC_OFF           = 0,    // Off  (0)
            RV_VNDR_TIMER_TRG_SRC_LINE_00       = 32,   // IO Line 00  (32)
            RV_VNDR_TIMER_TRG_SRC_LINE_01       = 33,   // IO Line 01  (33)
            RV_VNDR_TIMER_TRG_SRC_LINE_02       = 34,   // IO Line 02  (34)
            RV_VNDR_TIMER_TRG_SRC_LINE_03       = 35,   // IO Line 03  (35)
            RV_VNDR_TIMER_TRG_SRC_LINE_04       = 36,   // IO Line 04  (36)
            RV_VNDR_TIMER_TRG_SRC_LINE_05       = 37,   // IO Line 05  (37)
            RV_VNDR_TIMER_TRG_SRC_LINE_06       = 38,   // IO Line 06  (38)
            RV_VNDR_TIMER_TRG_SRC_LINE_07       = 39,   // IO Line 07  (39)
            RV_VNDR_TIMER_TRG_SRC_LINE_08       = 40,   // IO Line 08  (40)
            RV_VNDR_TIMER_TRG_SRC_LINE_09       = 41,   // IO Line 09  (41)
            RV_VNDR_TIMER_TRG_SRC_LINE_10       = 42,   // IO Line 10  (42)
            RV_VNDR_TIMER_TRG_SRC_LINE_11       = 43,   // IO Line 11  (43)
            RV_VNDR_TIMER_TRG_SRC_LINE_12       = 44,   // IO Line 12  (44)
            RV_VNDR_TIMER_TRG_SRC_LINE_13       = 45,   // IO Line 13  (45)
            RV_VNDR_TIMER_TRG_SRC_LINE_14       = 46,   // IO Line 14  (46)
            RV_VNDR_TIMER_TRG_SRC_LINE_15       = 47,   // IO Line 15  (47)
            RV_VNDR_TIMER_TRG_SRC_LINE_16       = 48,   // IO Line 16  (48)
            RV_VNDR_TIMER_TRG_SRC_LINE_17       = 49,   // IO Line 17  (49)
            RV_VNDR_TIMER_TRG_SRC_LINE_18       = 50,   // IO Line 18  (50)
            RV_VNDR_TIMER_TRG_SRC_LINE_19       = 51,   // IO Line 19  (51)
            RV_VNDR_TIMER_TRG_SRC_LINE_20       = 52,   // IO Line 20  (52)
            RV_VNDR_TIMER_TRG_SRC_LINE_21       = 53,   // IO Line 21  (53)
            RV_VNDR_TIMER_TRG_SRC_LINE_22       = 54,   // IO Line 22  (54)
            RV_VNDR_TIMER_TRG_SRC_LINE_23       = 55,   // IO Line 23  (55)
            RV_VNDR_TIMER_TRG_SRC_LINE_24       = 56,   // IO Line 24  (56)
            RV_VNDR_TIMER_TRG_SRC_LINE_25       = 57,   // IO Line 25  (57)
            RV_VNDR_TIMER_TRG_SRC_LINE_26       = 58,   // IO Line 26  (58)
            RV_VNDR_TIMER_TRG_SRC_LINE_27       = 59,   // IO Line 27  (59)
            RV_VNDR_TIMER_TRG_SRC_LINE_28       = 60,   // IO Line 28  (60)
            RV_VNDR_TIMER_TRG_SRC_LINE_29       = 61,   // IO Line 29  (61)
            RV_VNDR_TIMER_TRG_SRC_LINE_30       = 62,   // IO Line 30  (62)
            RV_VNDR_TIMER_TRG_SRC_LINE_31       = 63,   // IO Line 31  (63) 
            RV_VNDR_TIMER_TRG_SRC_ACQ_TRIGGER   = 96,   // AcquisitionTrigger  (96)
            RV_VNDR_TIMER_TRG_SRC_ACQ_START     = 100,  // AcquisitionStart (100)
            RV_VNDR_TIMER_TRG_SRC_ACQ_END       = 101,  // AcquisitionEnd (101)             // #Appended V1.0.2.0
            RV_VNDR_TIMER_TRG_SRC_FRM_TRIGGER   = 104,  // FrameTrigger  (104)
            RV_VNDR_TIMER_TRG_SRC_FRM_START     = 108,  // FrameStart  (108)
            RV_VNDR_TIMER_TRG_SRC_FRM_END       = 109,  // FrameEnd  (109)
            RV_VNDR_TIMER_TRG_SRC_EXP_START     = 124,  // ExcposureStart  (124)
            RV_VNDR_TIMER_TRG_SRC_EXP_END       = 125,  // ExposureEnd  (125)
        };
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Off         = REGDATA(RV_VNDR_TIMER_TRG_SRC_OFF,            (char*)"Off");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_00     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_00,        (char*)"Line0");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_01     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_01,        (char*)"Line1");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_02     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_02,        (char*)"Line2");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_03     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_03,        (char*)"Line3");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_04     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_04,        (char*)"Line4");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_05     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_05,        (char*)"Line5");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_06     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_06,        (char*)"Line6");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_07     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_07,        (char*)"Line7");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_08     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_08,        (char*)"Line8");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_09     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_09,        (char*)"Line9");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_10     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_10,        (char*)"Line10");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_11     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_11,        (char*)"Line11");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_12     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_12,        (char*)"Line12");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_13     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_13,        (char*)"Line13");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_14     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_14,        (char*)"Line14");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_15     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_15,        (char*)"Line15");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_16     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_16,        (char*)"Line16");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_17     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_17,        (char*)"Line17");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_18     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_18,        (char*)"Line18");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_19     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_19,        (char*)"Line19");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_20     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_20,        (char*)"Line20");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_21     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_21,        (char*)"Line21");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_22     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_22,        (char*)"Line22");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_23     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_23,        (char*)"Line23");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_24     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_24,        (char*)"Line24");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_25     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_25,        (char*)"Line25");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_26     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_26,        (char*)"Line26");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_27     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_27,        (char*)"Line27");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_28     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_28,        (char*)"Line28");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_29     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_29,        (char*)"Line29");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_30     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_30,        (char*)"Line30");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Line_31     = REGDATA(RV_VNDR_TIMER_TRG_SRC_LINE_31,        (char*)"Line31");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Acq_Trigger = REGDATA(RV_VNDR_TIMER_TRG_SRC_ACQ_TRIGGER,    (char*)"AcquisitionTrigger");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Acq_Start   = REGDATA(RV_VNDR_TIMER_TRG_SRC_ACQ_START,      (char*)"AcquisitionEnd");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Frm_Trigger = REGDATA(RV_VNDR_TIMER_TRG_SRC_FRM_TRIGGER,    (char*)"FrameTrigger");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Frm_Start   = REGDATA(RV_VNDR_TIMER_TRG_SRC_FRM_START,      (char*)"FrameStart");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Frm_End     = REGDATA(RV_VNDR_TIMER_TRG_SRC_FRM_END,        (char*)"FrameEnd");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Exp_Start   = REGDATA(RV_VNDR_TIMER_TRG_SRC_EXP_START,      (char*)"ExcposureStart");
        const REGDATA g_sREGV_Vndr_Timer_TrgSrc_Exp_End     = REGDATA(RV_VNDR_TIMER_TRG_SRC_EXP_END,        (char*)"ExposureEnd");
    
        const REGDATA g_saVAL_TmTrgSrc[] =
        {
            g_sREGV_Vndr_Timer_TrgSrc_Off,
            g_sREGV_Vndr_Timer_TrgSrc_Line_00,
            g_sREGV_Vndr_Timer_TrgSrc_Line_01,
            g_sREGV_Vndr_Timer_TrgSrc_Line_02,
            g_sREGV_Vndr_Timer_TrgSrc_Line_03,
            g_sREGV_Vndr_Timer_TrgSrc_Line_04,
            g_sREGV_Vndr_Timer_TrgSrc_Line_05,
            g_sREGV_Vndr_Timer_TrgSrc_Line_06,
            g_sREGV_Vndr_Timer_TrgSrc_Line_07,
            g_sREGV_Vndr_Timer_TrgSrc_Line_08,
            g_sREGV_Vndr_Timer_TrgSrc_Line_09,
            g_sREGV_Vndr_Timer_TrgSrc_Line_10,
            g_sREGV_Vndr_Timer_TrgSrc_Line_11,
            g_sREGV_Vndr_Timer_TrgSrc_Line_12,
            g_sREGV_Vndr_Timer_TrgSrc_Line_13,
            g_sREGV_Vndr_Timer_TrgSrc_Line_14,
            g_sREGV_Vndr_Timer_TrgSrc_Line_15,
            g_sREGV_Vndr_Timer_TrgSrc_Line_16,
            g_sREGV_Vndr_Timer_TrgSrc_Line_17,
            g_sREGV_Vndr_Timer_TrgSrc_Line_18,
            g_sREGV_Vndr_Timer_TrgSrc_Line_19,
            g_sREGV_Vndr_Timer_TrgSrc_Line_20,
            g_sREGV_Vndr_Timer_TrgSrc_Line_21,
            g_sREGV_Vndr_Timer_TrgSrc_Line_22,
            g_sREGV_Vndr_Timer_TrgSrc_Line_23,
            g_sREGV_Vndr_Timer_TrgSrc_Line_24,
            g_sREGV_Vndr_Timer_TrgSrc_Line_25,
            g_sREGV_Vndr_Timer_TrgSrc_Line_26,
            g_sREGV_Vndr_Timer_TrgSrc_Line_27,
            g_sREGV_Vndr_Timer_TrgSrc_Line_28,
            g_sREGV_Vndr_Timer_TrgSrc_Line_29,
            g_sREGV_Vndr_Timer_TrgSrc_Line_30,
            g_sREGV_Vndr_Timer_TrgSrc_Line_31,
            g_sREGV_Vndr_Timer_TrgSrc_Acq_Trigger,
            g_sREGV_Vndr_Timer_TrgSrc_Acq_Start,
            g_sREGV_Vndr_Timer_TrgSrc_Frm_Trigger,
            g_sREGV_Vndr_Timer_TrgSrc_Frm_Start,
            g_sREGV_Vndr_Timer_TrgSrc_Frm_End,
            g_sREGV_Vndr_Timer_TrgSrc_Exp_Start,
            g_sREGV_Vndr_Timer_TrgSrc_Exp_End,
        };
        
        
        
    }   // end of namespace RegMapBU
}       // end of namespace Teli

#endif  //  _REGISTERMAP_BU_H
