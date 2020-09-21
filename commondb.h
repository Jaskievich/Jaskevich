#ifndef __COMMONDB_H__
#define __COMMONDB_H__
#include "CVQT.h"

#pragma message("commondb.h ver 2.2")
#pragma warning( disable : 4786 ) // identifier was truncated to '255' characters in the debug information

extern"C"
{
typedef unsigned short word;
typedef unsigned long dword;
typedef long TGID;

enum {
	/// ������ ������ ��� ��������� �����
    short_name=33,
	/// ������ ������ ��� �������� �����	
    long_name=256 
    };

//----------------------------------------------------------------------------

enum    Flag
        {
	/// ��� �������
        F_None=0,  
	   /// ������� �� �����			
        F_HI,      
		/// ������� �� ����
        F_LO,         
		/// ������� �� ����� � �� ����
        F_HI_LO,      
    F_Last
        };

enum ModeNode
{
		///����������
		Autonom,
		///�������
		Master,
		///�������
		Slave,
		///������
		Client,
		///������������
		Switch
};

enum    DATA_COD
        {
		/// ��� �� ���������
        C_NoDataType=0, 
			/// ���������� ����
        C_Analog = 0x1,     
		/// ���������� ����
        C_Digital= 0x2,    
		/// ����������� ��������
        C_Const = 0x4,        
		/// ���������� ����������/�����
        C_AnalogCtl = 0x8,    
		/// ���������� ����������/�����
        C_DigitalCtl = 0x10, 
		C_Last,
		///��� ������� ������� � ����
		C_Output = 0x1000
        };

enum	DIG_Type
{
	DIG_Normal=0,
	DIG_Inverse=0x0001,
	DIG_Counter=0x0002,
	DIG_RG_HI=0x0004,
	DIG_RG_LO=0x0008,
	DIG_SG_HI=0x0010,
	DIG_SG_LO=0x0020,
	DIG_BL_HI=0x0040,
	DIG_BL_LO=0x0080
};
enum    NORM_EVAL
        {
        N_None = 0,
			/// ((VALUE-Instrument_LO)/(Instrument_HI - Instrument_LO)) * (SC_HI - SC_LO) + SC_LO ��������
        N_Linear,   
		/// SC_LO + ((SC_HI - SC_LO) / SQRT(Instrument_HI - Instrument_LO)) * SQRT(VALUE- Instrument_LO)	         mA ������������
        N_SQRT,     
		///SC_LO + ((SC_HI - SC_LO) / SQRT(Instrument_HI - Instrument_LO)) * SQRT(VALUE - Instrument_LO)
		/// ((VALUE-Instrument_LO)/(Instrument_HI - Instrument_LO)) * (SC_LO - SC_HI) + SC_HI �������� ��������
        N_RLinear,
		N_SQRTHyst,
    N_Last
        };


enum    DRV_Type
        {
			/// OPC DA ������
        D_OPC=0x01,
			/// ������ (����. ������ ��������� ��������)
        D_Native=0x02,
		D_OPC_AE=0x04,
		D_OPC_HDA=0x08,
		D_LTIME = 0x10 // ���������� � ���� ��������� ����� ������.
        };

#define NATIVE_DA 0x03
#define NATIVE_AE 0x05
#define NATIVE_HDA 0x09
#define NATIVE_DA_AE 0x07
#define NATIVE_DA_HDA 0x0B
#define NATIVE_DA_AE_HDA 0x0F
#define NATIVE_AE_HDA 0x0E

typedef enum { STATUS_RUNNING = 0,
STATUS_FAILED,
STATUS_NOCONFIG,
STATUS_SUSPENDED,
STATUS_TEST,
STATUS_ENG,
STATUS_ADM
} SERVERSTATE;

enum    STATE_COMMAND
{
			/// ����� ��� ����� �������
		S_Ready = 0,
		/// ���� ������� ��������
		S_Execute = 0x1,
		/// ������ ����������
		S_Error = 0x2,
		/// ���������� � ����
		S_Sending = 0x4,
		/// ���������
		S_Complete	= 0x8
};


#pragma pack(push,1)

/// ����������� ��������� ��� ������� ������� � ����
struct COutput
{
	/// ������ ���� � DBManager
	long	iTag;
	/// �������� , ������� ����� ��������, �������� � �������, ���� ������ ���� �� ������� � ���� (STATE_COMMAND State[0]==S_Sending)
	TValue	What[2];
	/// ��� ����� ��������
	long	Who;
	/// ��������� ���������� �������
		// ������������ ��������� - STATUS_MASK Values for Quality = BAD � opcaedef.h
	long	Result;
		/// ������ ���������� �������, ������ ������� � �������
	STATE_COMMAND State[2];

};

typedef struct S_Block
    {		
		/// ����� ����� � ������� ������ ���������
    short number;
		/// ����� ������������� �����, � ������� ������ ����� ����
    short parent;
		/// ��� �����
    char name[long_name];
		/// GID ������������ ����, ������������ ��������� ��������� ����� ��� ���������(��� �����, ��� number=1)
	long mode_index; // ��� �������� ���� ����������� � ������� ����
    }t_block;
	
	typedef struct  S_Drv
	{
		long	drv_id;
		/// ��� ��������
		DRV_Type type;
		/// ��� ����, �� ������� ��������� ������� (��� OPC Server \\��� �������)
		char    node[short_name];
		/// ��� �������� - (��� OPC Server - AppID, ��� ������� - ��� ������)
		char    name[long_name];
		/// Description	
		char	desc[long_name];
		/// ������ ������������
		char	cfg[long_name];
		///������ �������
		char	runstr[long_name]; 
		///����� �� ������ ��� ������ �������
		bool	is_startup;        
		/// ����� �� �������������� ���������� ��� ���������-��������
		bool	is_autorestart;    
		/// ���������� ������ ������������
		long	scanrate;          
		/// ������������ ������ ������������
		long	maxscanrate;
		/// ������� ������� ��� �������
		long	severity;
		/// "�������" ��� ��� ����������� "�������" ������
		TGID	prob_index;
		TGID	logon_index; // ����������� ��� Vinca.exe ��� �������� ����������� ������������
		bool	is_command_send;
		HANDLE	command_event;
    } t_drv;

typedef struct  S_Drv_Ver
        {
			 /// ��� �������� - (��� ������)
    char    name[long_name];
		/// Major version
    long	major;          
		/// Minor version
    long	minor;
    } t_drv_ver;

#define IS_PROCENT_HYST 0x08
#define NORMALIZE_MASK	0x07

///      ������� ������� 
typedef struct  S_Analog
        {	
		/// ������� ���������
        char    eu[short_name]; 
		/// ��������������� ��� �������
//        short   metrolog;               
		unsigned short mtype;
		unsigned short msubtype;
		/// ��� � �������� ������������
    NORM_EVAL   normalize;              
	/// ��������� ��������
        TValue   startval;
		/// ������� ������� ����� ��������
        TValue   Instrument_HI;          
		/// ������ ������� ����� ��������
        TValue   Instrument_LO;  
        /// ������� ������� �����
        TValue   SC_HI;          
		/// ������ ������� �����
        TValue  SC_LO;          
		        /// ���� ����. ����� 
        Flag    F_RG;           
		/// ������� ������� ����. ����� 
        TValue  RG_HI;          
		/// ������ ������� ����. ����� 
        TValue  RG_LO;          
		   /// ���� ������������
        Flag    F_SG;          
		 /// ������� ������� ������������
        TValue   SG_HI;         
		/// ������ ������� ������������
        TValue   SG_LO;          
		 /// ���� ����������
        Flag    F_BL;       
		    /// ������� ������� ����������
        TValue   BL_HI;     
		     /// ������ ������� ����������
        TValue   BL_LO;     
		/// ���������� ������������ � ����������
        TValue  Hyst;
		/// ���������� � ����� ������� (��� ���������� �������)
		TValue  HystInstr;
		/// �������������� ����� ������� ����������
        TGID    BL_CH_HI;               
		/// �������������� ����� ������ ����������
        TGID    BL_CH_LO;               
		/// ������ ������ �� ���������� � ���� ��������� ������, �������� - "%7.3f"
        char    format[12];
    } t_analog;

typedef struct  S_Digital
        {
				/// ��� ������
        DIG_Type   chan_type;              
				/// ������ ������� � m_properties, ����������� ���������.(index = number-1)
				/// Value[0] - �� ��������� (���������� ���������)
				/// ������ ����� - COLORREF ��� ������ ��������� ��������������� ������
        long    Value[2];
    } t_digital;

enum    Const_Type
        {
			/// ������ ����������
        C_Common=0,
			/// ��������� (����� ������...)
        C_System=1,
		/// �������������� (����� �����...)
		C_Block=2
        };

#define MAX_CONST 8
typedef struct  S_Const
        {
				/// �������� (������ � ������� Value)
        short   svalue;
				/// ������ ������� � m_properties, ����������� ���������.(index = number-1)
				/// ������ ����� - COLORREF ��� ������ ��������� ��������������� ������
        long    Value[MAX_CONST];
        long    Color[MAX_CONST];
		Const_Type Category;
    } t_const;


typedef struct  S_ACTL          /// ���� ����������� ����������
        {
        TGID    PV;
        TGID    SP;
        TGID    MV;
		TValue	kP;
		TValue	kI;
		TValue	kD;
    } t_actl;

	enum    DCTL_TYPE
	{
		/// ���� 0% - ���������� ����
		D_I1 = 0x1,
		/// ���� 100% - ���������� ����
		D_I2 = 0x2,
		/// ����� 0% - ���������� ����
		D_O1 = 0x4,
		/// ����� 100% - ���������� ����
		D_O2 = 0x8,
		///���� ��������� 1 - ����/�� ����� - ���������� ����
		D_DIST	= 0x10,
		/// ���� ��������� 2 - �����/������ - ���������� ����
		D_ALARM	= 0x20,
		/// ������, ����������������� � ���� ������� �������� ������������ ����� TIME 
		D_TIMER	= 0x40,
		D_COMMAND = 0x80,
		D_EKM = 0x100 // ���������� �� ���� �� ���
	};

	///S_DCTL.command
	enum    DCTLCommand
	{
		Open = 0,
		Close,
		Stop,
		Unblock,
		CountCommands
	};

	///S_DCTL.state
	enum    DCTLState
	{
		///�������
		Opened = 0,
		/// ������� 
		Closed,
		///�����������
		Opening,
		 ///����������� 
		Closing,
		///������
		Alarm,
		///���������� 
		Unknown_
	};
	///S_DCTL.state
	enum    DCTLStateEKM
	{
		///�����
		Normal = 0,
		/// ������
		LO_State,
		///�������
		HI_State,
		 ///������
		AlarmEKM
	};

	typedef struct  S_DCTL          /// ���� ��������� ����������
	{
		/// ��� ����� ��������� ����������
		/// ���������� �����, ��������� � DCTL_TYPE
        short   type;        
		/// ���� 0% - ������� �� ��������
        TGID    I_000; // ��� �������� ���� ����������� � ������� ����
		/// ���� 100% - ������� �� ��������
        TGID    I_100; // ��� �������� ���� ����������� � ������� ����
		/// ����� 0% - ����� �� ��������
        TGID    O_000;// ��� �������� ���� ����������� � ������� ����
		/// ����� 100% - ����� �� ��������
        TGID    O_100;// ��� �������� ���� ����������� � ������� ����
		/// ���� ��������� 1 - ����/�� �����
        TGID    is_local;           // ��� �������� ���� ����������� � ������� ����
		/// ���� ��������� 2 - �����/������
        TGID    is_alarm;           // ��� �������� ���� ����������� � ������� ����
		/// ������ �������� ������������ �������� ������ � ����.
        long    timer; 
		/// ��������� ��������� �������� ������ - ������ const tags
		// �������, �������, �����������, �����������, ����������, ������
        TGID    state; // ��� �������� ���� ����������� � ������� ����

		/// ��������� ��������� �������  - ������ const tags
		// �������, �������, ����, ��������������
        TGID    command; // ��� �������� ���� ����������� � ������� ����

		/// ��������� ��������� ������  - ������ const tags
		// �����, ������ �������, ������ ���������, ������ ���������
        TGID    alarm; // ��� �������� ���� ����������� � ������� ����
    } t_dctl;

typedef struct S_Property
	    {
	long prop_id;
			/// ��� ��������
    char    name[short_name];  
			/// �������� ��������
    char    value[short_name]; 
    } t_property;



enum    COD_ACCESS
{
			/// ������ ������ ��� ������
	U_Read = 0,
		/// ������ ��� ������
	U_Write = 0x1,
		/// ������ ��� ���������� (������������ source ����)
	U_Switch = 0x2,
		/// ������ ��� �����
	U_RWS	= 0x3
};
enum    DATA_VISIBLE
{
		/// �������� ����� ������ ��� �������
	V_Sys = 0,
		/// �������� ����� ������ ��������
	V_Local = 0x1,
		/// �������� ����� �����
	V_Export = 0x3,
		/// �������� ������ ������������� � ������� �������� ������������
	V_Watch = 0x7
//	V_Trace = 0xF
};
enum    DATA_SOURCE
        {
				/// �������� ������ ��������
		S_DataOff = 0,
			/// ������ �� ��������
        S_DataDrv,  
			/// ������� ������ (���������)- ���������� ��������
		S_Calc,	
			 /// ������ ����	- ���������� ��������
        S_Manual,  
    S_Last
        };

enum    USER_Command
{
			/// ��� ������� ��� ������� ���������
	C_None = 0,
			/// ����������� �������� ������ �� �������
	C_SetAuto,
			/// ����������� �������� ������ �� ������� ������
	C_SetCalc,
			/// ����������� �������� ������ �� ������
	C_SetMan,
			/// ��������� �������� ������
	C_Off
};

enum ALL_OR_ANY
{
	ANY_BIT = 0,
	ALL_BIT = 1
};

typedef	struct tagAccessFields
		{
	///�������
	unsigned 	command:4;	/// ������� (None|SetAuto|SetMan|SetCalc|Off)
	unsigned 	source:2;		/// �������� ������ (Auto|Man|Calc)
	unsigned 	admaccess:2;  /// ������ ��� ������/������ (Admin)
	unsigned 	engaccess:2;  /// ������ ��� ������/������ (Eng)
	unsigned 	usraccess:2;  /// ������ ��� ������/������ (User)
	unsigned	visible:4;    /// "������� ���������" ��������� (Sys|Local|Export|Watch)
	///�������
	unsigned	is_metrolog:1; /// ��� ������ ����������� �������� �����
	unsigned	is_calibrate:1; /// ���������� ������� (����� ��������� ����� �������� � m_computed[0])
	unsigned	reserved:14;
		} AccessFields;

enum WHO
{
	USR = 0,
	ENG = 1,
	ADM = 3
};

typedef	union tagAccess
	{
	AccessFields	fields;
			/// ��� ����� ��������/�������/�������
	unsigned long   all;     
	} Access;

#define SYS_VISIBLE		(unsigned long)0x0000l
#define LOCAL_VISIBLE	(unsigned long)0x1000l
#define EXPORT_VISIBLE	(unsigned long)0x3000l
#define WATCH_VISIBLE	(unsigned long)0x7000l
#define TRACE_VISIBLE	(unsigned long)0xF000l
#define ALL_RONLY		(unsigned long)0x0000l
#define USR_READ		(unsigned long)0x000l
#define USR_WRITE		(unsigned long)0x400l
#define USR_SWITCH		(unsigned long)0x800l
#define USR_RWS			(unsigned long)0xc00l
#define ENG_READ		(unsigned long)0x000l
#define ENG_WRITE		(unsigned long)0x100l
#define ENG_SWITCH		(unsigned long)0x200l
#define ENG_RWS			(unsigned long)0x300l
#define ADM_READ		(unsigned long)0x000l
#define ADM_WRITE		(unsigned long)0x40l
#define ADM_SWITCH		(unsigned long)0x80l
#define ADM_RWS			(unsigned long)0xc0l

#define DRV_SOURCE		(unsigned long)0x10l
#define CALC_SOURCE		(unsigned long)0x20l
#define MAN_SOURCE		(unsigned long)0x30l

#define AUTO_COMMAND	(unsigned long)0x1l
#define CALC_COMMAND	(unsigned long)0x2l
#define MAN_COMMAND		(unsigned long)0x3l
#define OFF_COMMAND		(unsigned long)0x4l

#define OFF_SCALE	    (unsigned long)0x10000l
#define SENSOR_CALIBRATE	(unsigned long)0x20000l

enum    TYPE_COUNTER
{
		/// ������� ������������ ����������� ��������� �� ��� ���������
	Events = 1,
		/// ������� ������� ���������� ��������� �� ��� ���������
	Time = 0x2,
		/// �������, ������������� ��������� ��������� source �� ���, ���, ���
		/// Sec
	I_Sec = 0x4,
		/// Min
	I_Min = 0x8,
		/// Hour
	I_Hour = 0xF
};

typedef struct S_Counter
{
	TYPE_COUNTER type; // �������, �����, ������������
	TGID	source; // ������ ���� ������� 

}t_counter;

enum    ACTIONS
{
		/// ������ � ������
	Log_Action = 1,
		/// ����� � ���� ���. ����������
	Window_Action = 0x2,
		/// ���������� �������������
	Require_Action = 0x4,
		/// �������� ������� �� ������� �������
	UpperLevel_Action = 0x8
};


typedef	struct ActionsFields
		{
	///�������
	unsigned 	action:4;	/// ������� �� �������
	unsigned 	severity:4;		/// ��������� �������- �� ����������� 0 �� 15
		} Act;

typedef union ActionSeverity
{
	Act actions; // ������� 4 ����-��������� �������, ������� 4 ����- ������� �� ������� 
	unsigned char	byte_actions; // 

}t_ActionSeverity;

typedef t_ActionSeverity TReact[8];

class EventActions
		{
public:
    EventActions();
    EventActions(const ActionSeverity events[8])
	{
		for(int i = 0; i < 8; i++)
		{
			Events[i].byte_actions = events[i].byte_actions;
		}
	}
    EventActions(const EventActions& right)
	{
		for(int i = 0; i < 8; i++)
		{
			Events[i].byte_actions = right.Events[i].byte_actions;
		}
	}
    EventActions(const unsigned __int64	PackEvents)
	{
		SetEvents(PackEvents);
	}
    ~EventActions(){}
    operator unsigned __int64() const
            {
				return GetPackEvents();
            }
    EventActions&  operator=(const unsigned __int64& right)
	{
		SetEvents(right);
	}
    EventActions&  operator=(const EventActions& right)
	{
		for(int i = 0; i < 8; i++)
		{
			Events[i].byte_actions = right.Events[i].byte_actions;
		}
	}

	bool SetEvents(const unsigned __int64	PackEvents)
	{
		unsigned __int64 pack = PackEvents;
		// ������� ���� � 0, ������� � 7 �������.
		for(int i = 7; i >= 0; i--)
		{
			(Events[i]).byte_actions = (unsigned char )pack;
			pack >>= 8;
		}
		return true;
	};
	unsigned __int64 GetPackEvents() const
	{
		unsigned __int64	PackEvents = 0;
		for(int i = 0; i < 8; i++)
		{
			PackEvents <<= 8;
			PackEvents += Events[i].byte_actions;
		}
		return PackEvents;
	};
	bool GetEvents(t_ActionSeverity* actsev) const
	{
		for(int i = 0; i < 8; i++)
		{
			actsev[i].byte_actions = (Events[i]).byte_actions;
		}
		return true;
	}
public:
	t_ActionSeverity	Events[8];
	
		};

//    EventActions&  operator=(const EventActions& right)

/// Events[0] ��� analog - RG_LO, ��� digital � const - ������� � ���������� (0-�) ���������
/// Events[1] ��� analog RG_HI, ��� digital � const - ������� � ������ (1-�) ���������
/// Events[2] ��� analog SG_LO, ��� const - ������� �� ������ (2-�) ���������
/// Events[3] ��� analog SG_HI, ��� const - ������� � ������ (3-�) ���������
/// Events[4] ��� analog BL_LO, ��� const - ������� � ��������� (4-�) ���������
/// Events[5] ��� analog BL_HI, ��� const - ������� � ����� (5-�) ���������
/// Events[6] ��� const - ������� � ������ (6-�) ���������
/// Events[7] ��� const - ������� � ������� (7-�) ���������

typedef struct  S_Data
        {
				/// ���������� ����������
		TGID	gid;            
				/// ���������� �������� ������� ����������
        long    driver_id;          
				/// ��� ������ �������� (������������� ���� � ��������� ��������)
        char    item[long_name];       
				/// ��� ������ � ������� Vinca
        char    name[short_name];       
				/// ������������ ���������
        char    desc[long_name];
				/// ��� ���������. ��. DATA_COD
unsigned short  type;           
				/// ��� ������/���� ���������
        short   block;          
				/// ����� ������ ������� ������� �� �������
		t_ActionSeverity react[8];
				/// ��� ����� ���������/�������/�������
		Access	access;
				/// ���������� ����� � ����� ��� ����������� � �������� ������
		long	number;
    union 
        {
				/// ���������� ������
        t_analog    analog;     
				/// �������� ������
        t_digital   digital;
				/// ���������
        t_const     constant;
				/// ���� ����������� ����������
        t_actl      actl;
				/// ���� ��������� ����������
        t_dctl      dctl;
        };
    } tag_info;

#pragma pack(pop)
}
#ifdef __cplusplus
#include <vector>
typedef std::vector<long> list_tags;
typedef list_tags list_index;
typedef list_tags list_gids;

typedef std::vector<t_drv> drivers;
typedef std::vector<t_property> properties;


struct Palette
{
	vector<long> colors;
	Palette()
	{//������������
		colors.push_back(RGB(0  , 255, 0  ));
		colors.push_back(RGB(144, 0  , 144));
		colors.push_back(RGB(255, 255, 0  ));
		colors.push_back(RGB(255, 0  , 0  ));
		colors.push_back(RGB(255, 0  , 255));
		colors.push_back(RGB(0  , 240, 240));
		colors.push_back(RGB(255, 127, 0  ));
		colors.push_back(RGB(160, 160, 160));

//��������������
		colors.push_back(RGB(127, 0  , 0  ));
		colors.push_back(RGB(0  , 127, 0  ));
		colors.push_back(RGB(128, 0  , 255));
		colors.push_back(RGB(127, 127, 0  ));
		colors.push_back(RGB(0  , 128, 255));
		colors.push_back(RGB(0  , 127, 127));


	}
};


struct PaletteSetPoint
{
	vector<long> colors;
	PaletteSetPoint()
	{
		colors.push_back(RGB(127, 0  , 0  ));
		colors.push_back(RGB(0  , 127, 0  ));
		colors.push_back(RGB(128, 0  , 255));
		colors.push_back(RGB(127, 127, 0  ));
		colors.push_back(RGB(0  , 128, 255));
		colors.push_back(RGB(0  , 127, 127));
	}
};


class CTag
{
public:
	CTag():m_pTag(NULL){}
	CTag(tag_info* ptag):m_pTag(ptag){}
	void Init(tag_info* ptag)
	{
		m_pTag = ptag;
	}
	virtual ~CTag(){}
public:
	tag_info* m_pTag;
};

class CAnalogTag : public CTag
{
public:
	CAnalogTag(){}
	CAnalogTag(tag_info* ptag):CTag(ptag){}
};
class CConstTag : public CTag
{
public:
	CConstTag(){}
	CConstTag(tag_info* ptag):CTag(ptag){}
};

class CDigitalTag : public CTag
{
public:
	CDigitalTag(){}
	CDigitalTag(tag_info* ptag):CTag(ptag){}
};
class CAnalogCtlTag : public CTag
{
public:
	CAnalogCtlTag(){}
	CAnalogCtlTag(tag_info* ptag):CTag(ptag){}
};

class CDigitalCtlTag : public CTag
{
public:
	CDigitalCtlTag(){}
	CDigitalCtlTag(tag_info* ptag):CTag(ptag){}
};

class CDCtlTagI1O1 : public CDigitalCtlTag
{
public:
	CDCtlTagI1O1(){}
	CDCtlTagI1O1(tag_info* ptag):CDigitalCtlTag(ptag){}
};
class CDCtlTagI2O1 : public CDigitalCtlTag
{
public:
	CDCtlTagI2O1(){}
	CDCtlTagI2O1(tag_info* ptag):CDigitalCtlTag(ptag){}
};
class CDCtlTagI1O2 : public CDigitalCtlTag
{
public:
	CDCtlTagI1O2(){}
	CDCtlTagI1O2(tag_info* ptag):CDigitalCtlTag(ptag){}
};
class CDCtlTagI2O2 : public CDigitalCtlTag
{
public:
	CDCtlTagI2O2(){}
	CDCtlTagI2O2(tag_info* ptag):CDigitalCtlTag(ptag){}
};

class CDCtlTagI1O1Timer : public CDigitalCtlTag
{
public:
	CDCtlTagI1O1Timer(){}
	CDCtlTagI1O1Timer(tag_info* ptag):CDigitalCtlTag(ptag){}
};
class CDCtlTagI2O1Timer : public CDigitalCtlTag
{
public:
	CDCtlTagI2O1Timer(){}
	CDCtlTagI2O1Timer(tag_info* ptag):CDigitalCtlTag(ptag){}
};
class CDCtlTagI1O2Timer : public CDigitalCtlTag
{
public:
	CDCtlTagI1O2Timer(){}
	CDCtlTagI1O2Timer(tag_info* ptag):CDigitalCtlTag(ptag){}
};

class CDCtlTagI2O2Timer : public CDigitalCtlTag
{
public:
	CDCtlTagI2O2Timer(){}
	CDCtlTagI2O2Timer(tag_info* ptag):CDigitalCtlTag(ptag){}
};


#endif //__cplusplus
#endif //__COMMONDB_H__
