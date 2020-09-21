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
	/// Размер строки для короткого имени
    short_name=33,
	/// Размер строки для длинного имени	
    long_name=256 
    };

//----------------------------------------------------------------------------

enum    Flag
        {
	/// Нет уставки
        F_None=0,  
	   /// Уставка по верху			
        F_HI,      
		/// Уставка по низу
        F_LO,         
		/// Уставка по верху и по низу
        F_HI_LO,      
    F_Last
        };

enum ModeNode
{
		///Автономный
		Autonom,
		///Ведущий
		Master,
		///Ведомый
		Slave,
		///Клиент
		Client,
		///Переключение
		Switch
};

enum    DATA_COD
        {
		/// Тип не определен
        C_NoDataType=0, 
			/// Аналоговый вход
        C_Analog = 0x1,     
		/// Дискретный вход
        C_Digital= 0x2,    
		/// Константный параметр
        C_Const = 0x4,        
		/// Аналоговое управление/выход
        C_AnalogCtl = 0x8,    
		/// Дискретное управление/выход
        C_DigitalCtl = 0x10, 
		C_Last,
		///Для посылки команды в поле
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
			/// ((VALUE-Instrument_LO)/(Instrument_HI - Instrument_LO)) * (SC_HI - SC_LO) + SC_LO линейная
        N_Linear,   
		/// SC_LO + ((SC_HI - SC_LO) / SQRT(Instrument_HI - Instrument_LO)) * SQRT(VALUE- Instrument_LO)	         mA квадратичная
        N_SQRT,     
		///SC_LO + ((SC_HI - SC_LO) / SQRT(Instrument_HI - Instrument_LO)) * SQRT(VALUE - Instrument_LO)
		/// ((VALUE-Instrument_LO)/(Instrument_HI - Instrument_LO)) * (SC_LO - SC_HI) + SC_HI обратная линейная
        N_RLinear,
		N_SQRTHyst,
    N_Last
        };


enum    DRV_Type
        {
			/// OPC DA сервер
        D_OPC=0x01,
			/// Родной (напр. модуль расчетных значений)
        D_Native=0x02,
		D_OPC_AE=0x04,
		D_OPC_HDA=0x08,
		D_LTIME = 0x10 // Записывать в тэги локальное время модуля.
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
			/// Готов для приёма команды
		S_Ready = 0,
		/// Дана команда драйверу
		S_Execute = 0x1,
		/// Ошибка выполнения
		S_Error = 0x2,
		/// посылается в поле
		S_Sending = 0x4,
		/// завершена
		S_Complete	= 0x8
};


#pragma pack(push,1)

/// Управляющая структура для посылки команды в поле
struct COutput
{
	/// Индекс тэга в DBManager
	long	iTag;
	/// Значение , которое хотим записать, значение в очереди, если первое ушло на посылку в поле (STATE_COMMAND State[0]==S_Sending)
	TValue	What[2];
	/// Кто хочет записать
	long	Who;
	/// Результат выполнения команды
		// Определяется качеством - STATUS_MASK Values for Quality = BAD в opcaedef.h
	long	Result;
		/// Статус выполнения команды, статус команды в очереди
	STATE_COMMAND State[2];

};

typedef struct S_Block
    {		
		/// Номер блока в массиве блоков установки
    short number;
		/// Номер родительского блока, в который входит этоот блок
    short parent;
		/// Имя блока
    char name[long_name];
		/// GID константного тэга, описывающего возможные состояния блока или установки(для блока, где number=1)
	long mode_index; // При загрузке базы приводиться к индексу тэга
    }t_block;
	
	typedef struct  S_Drv
	{
		long	drv_id;
		/// Тип драйвера
		DRV_Type type;
		/// Имя узла, на котором находится драйвер (для OPC Server \\имя станции)
		char    node[short_name];
		/// Имя драйвера - (для OPC Server - AppID, для родного - имя модуля)
		char    name[long_name];
		/// Description	
		char	desc[long_name];
		/// Строка конфигурации
		char	cfg[long_name];
		///строка запуска
		char	runstr[long_name]; 
		///Нужен ли запуск при старте системы
		bool	is_startup;        
		/// Нужен ли автоматический перезапуск при зависании-выгрузке
		bool	is_autorestart;    
		/// Нормальный период сканирования
		long	scanrate;          
		/// Максимальный период сканирования
		long	maxscanrate;
		/// Уровень тревоги или события
		long	severity;
		/// "Пробный" тэг для определения "живости" модуля
		TGID	prob_index;
		TGID	logon_index; // заполняется для Vinca.exe для контроля регистрации пользователя
		bool	is_command_send;
		HANDLE	command_event;
    } t_drv;

typedef struct  S_Drv_Ver
        {
			 /// Имя драйвера - (имя модуля)
    char    name[long_name];
		/// Major version
    long	major;          
		/// Minor version
    long	minor;
    } t_drv_ver;

#define IS_PROCENT_HYST 0x08
#define NORMALIZE_MASK	0x07

///      Таблица каналов 
typedef struct  S_Analog
        {	
		/// Единицы измерения
        char    eu[short_name]; 
		/// Метрологический тип датчика
//        short   metrolog;               
		unsigned short mtype;
		unsigned short msubtype;
		/// Тип и алгоритм нормализации
    NORM_EVAL   normalize;              
	/// Начальное значение
        TValue   startval;
		/// Верхняя граница шкалы драйвера
        TValue   Instrument_HI;          
		/// Нижняя граница шкалы драйвера
        TValue   Instrument_LO;  
        /// Верхняя граница шкалы
        TValue   SC_HI;          
		/// Нижняя граница шкалы
        TValue  SC_LO;          
		        /// Флаг техн. нормы 
        Flag    F_RG;           
		/// Верхняя граница техн. нормы 
        TValue  RG_HI;          
		/// Нижняя граница техн. нормы 
        TValue  RG_LO;          
		   /// Флаг сигнализации
        Flag    F_SG;          
		 /// Верхняя граница сигнализации
        TValue   SG_HI;         
		/// Нижняя граница сигнализации
        TValue   SG_LO;          
		 /// Флаг блокировок
        Flag    F_BL;       
		    /// Верхняя граница блокировки
        TValue   BL_HI;     
		     /// Нижняя граница блокировки
        TValue   BL_LO;     
		/// Гистерезис сигнализации и блокировки
        TValue  Hyst;
		/// Гистерезис в шкале прибора (для вычисления зашкала)
		TValue  HystInstr;
		/// Исполнительный канал верхней блокировки
        TGID    BL_CH_HI;               
		/// Исполнительный канал нижней блокировки
        TGID    BL_CH_LO;               
		/// Формат вывода на индикаторы в виде форматной строки, например - "%7.3f"
        char    format[12];
    } t_analog;

typedef struct  S_Digital
        {
				/// Тип канала
        DIG_Type   chan_type;              
				/// Массив номеров в m_properties, описывающим состояния.(index = number-1)
				/// Value[0] - по умолчанию (нормальное состояние)
				/// вторая часть - COLORREF для показа состояния соответствующим цветом
        long    Value[2];
    } t_digital;

enum    Const_Type
        {
			/// общего назначения
        C_Common=0,
			/// системный (ярлык модуля...)
        C_System=1,
		/// технологическй (ярлык блока...)
		C_Block=2
        };

#define MAX_CONST 8
typedef struct  S_Const
        {
				/// Значение (индекс в массиве Value)
        short   svalue;
				/// Массив номеров в m_properties, описывающим состояния.(index = number-1)
				/// вторая часть - COLORREF для показа состояния соответствующим цветом
        long    Value[MAX_CONST];
        long    Color[MAX_CONST];
		Const_Type Category;
    } t_const;


typedef struct  S_ACTL          /// Блок аналогового управления
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
		/// Вход 0% - дискретный вход
		D_I1 = 0x1,
		/// Вход 100% - дискретный вход
		D_I2 = 0x2,
		/// Выход 0% - дискретный вход
		D_O1 = 0x4,
		/// Выход 100% - дискретный вход
		D_O2 = 0x8,
		///Вход состояния 1 - дист/по месту - дискретный вход
		D_DIST	= 0x10,
		/// Вход состояния 2 - норма/авария - дискретный вход
		D_ALARM	= 0x20,
		/// Таймер, продолжительность в мсек сигнала закрытия определяется полем TIME 
		D_TIMER	= 0x40,
		D_COMMAND = 0x80,
		D_EKM = 0x100 // Комбинация из двух по ИЛИ
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
		///Открыто
		Opened = 0,
		/// закрыто 
		Closed,
		///открывается
		Opening,
		 ///закрывается 
		Closing,
		///авария
		Alarm,
		///неизвестно 
		Unknown_
	};
	///S_DCTL.state
	enum    DCTLStateEKM
	{
		///Норма
		Normal = 0,
		/// Низкий
		LO_State,
		///Высокий
		HI_State,
		 ///Авария
		AlarmEKM
	};

	typedef struct  S_DCTL          /// Блок цифрового управления
	{
		/// Тип блока цифрового управления
		/// Комбинация битов, описанных в DCTL_TYPE
        short   type;        
		/// Вход 0% - контакт на закрытие
        TGID    I_000; // При загрузке базы приводиться к индексу тэга
		/// Вход 100% - контакт на открытие
        TGID    I_100; // При загрузке базы приводиться к индексу тэга
		/// Выход 0% - релле на закрытие
        TGID    O_000;// При загрузке базы приводиться к индексу тэга
		/// Выход 100% - релле на открытие
        TGID    O_100;// При загрузке базы приводиться к индексу тэга
		/// Вход состояния 1 - дист/по месту
        TGID    is_local;           // При загрузке базы приводиться к индексу тэга
		/// Вход состояния 2 - норма/авария
        TGID    is_alarm;           // При загрузке базы приводиться к индексу тэга
		/// Таймер задержки срабатывания рабочего органа в мсек.
        long    timer; 
		/// Индикатор состояния рабочего органа - индекс const tags
		// Открыто, закрыто, открывается, закрывается, неизвестно, авария
        TGID    state; // При загрузке базы приводиться к индексу тэга

		/// Индикатор состояния команды  - индекс const tags
		// Открыть, закрыть, стоп, разблокировать
        TGID    command; // При загрузке базы приводиться к индексу тэга

		/// Индикатор состояния аварии  - индекс const tags
		// Норма, авария питания, авария редуктора, авария двигателя
        TGID    alarm; // При загрузке базы приводиться к индексу тэга
    } t_dctl;

typedef struct S_Property
	    {
	long prop_id;
			/// Имя свойства
    char    name[short_name];  
			/// Значение свойства
    char    value[short_name]; 
    } t_property;



enum    COD_ACCESS
{
			/// Доступ только для чтения
	U_Read = 0,
		/// Доступ для записи
	U_Write = 0x1,
		/// Доступ для выполнения (переключения source типа)
	U_Switch = 0x2,
		/// Доступ для всего
	U_RWS	= 0x3
};
enum    DATA_VISIBLE
{
		/// Параметр виден только для системы
	V_Sys = 0,
		/// Параметр виден только локально
	V_Local = 0x1,
		/// Параметр виден везде
	V_Export = 0x3,
		/// Параметр должен отслеживаться в системе контроля безопасности
	V_Watch = 0x7
//	V_Trace = 0xF
};
enum    DATA_SOURCE
        {
				/// Источник данных отключен
		S_DataOff = 0,
			/// Данные из драйвера
        S_DataDrv,  
			/// Внешний модуль (системный)- сохранение значений
		S_Calc,	
			 /// Ручной ввод	- сохранение значений
        S_Manual,  
    S_Last
        };

enum    USER_Command
{
			/// Нет команды или команда выполнена
	C_None = 0,
			/// Переключить источник данных на драйвер
	C_SetAuto,
			/// Переключить источник данных на внешний модуль
	C_SetCalc,
			/// Переключить источник данных на ручной
	C_SetMan,
			/// Выключить источник данных
	C_Off
};

enum ALL_OR_ANY
{
	ANY_BIT = 0,
	ALL_BIT = 1
};

typedef	struct tagAccessFields
		{
	///Младший
	unsigned 	command:4;	/// Команды (None|SetAuto|SetMan|SetCalc|Off)
	unsigned 	source:2;		/// Источник данных (Auto|Man|Calc)
	unsigned 	admaccess:2;  /// Доступ для записи/чтения (Admin)
	unsigned 	engaccess:2;  /// Доступ для записи/чтения (Eng)
	unsigned 	usraccess:2;  /// Доступ для записи/чтения (User)
	unsigned	visible:4;    /// "Область видимости" параметра (Sys|Local|Export|Watch)
	///Старший
	unsigned	is_metrolog:1; /// при взводе отключается контроль шкалы
	unsigned	is_calibrate:1; /// калибровка датчика (нужно сохранять сырые значения в m_computed[0])
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
			/// Бит маска экспорта/доступа/команды
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
		/// счётчик переключений дискретного параметра во вкл состояние
	Events = 1,
		/// счётчик времени нахождения параметра во вкл состоянии
	Time = 0x2,
		/// счётчик, интегрирующий показания параметра source за сек, мин, час
		/// Sec
	I_Sec = 0x4,
		/// Min
	I_Min = 0x8,
		/// Hour
	I_Hour = 0xF
};

typedef struct S_Counter
{
	TYPE_COUNTER type; // события, время, интегральный
	TGID	source; // индекс тэга обсчёта 

}t_counter;

enum    ACTIONS
{
		/// запись в журнал
	Log_Action = 1,
		/// Вывод в окно тек. отклонений
	Window_Action = 0x2,
		/// требование подтверждения
	Require_Action = 0x4,
		/// Передача события на верхний уровень
	UpperLevel_Action = 0x8
};


typedef	struct ActionsFields
		{
	///Младший
	unsigned 	action:4;	/// реакция на событие
	unsigned 	severity:4;		/// приоритет события- от наименьшего 0 до 15
		} Act;

typedef union ActionSeverity
{
	Act actions; // старшие 4 бита-приоритет события, младшие 4 бита- реакция на событие 
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
		// старший байт в 0, младший в 7 позицию.
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

/// Events[0] для analog - RG_LO, для digital и const - переход в нормальное (0-е) состояние
/// Events[1] для analog RG_HI, для digital и const - переход в первое (1-е) состояние
/// Events[2] для analog SG_LO, для const - переход во второе (2-е) состояние
/// Events[3] для analog SG_HI, для const - переход в третье (3-е) состояние
/// Events[4] для analog BL_LO, для const - переход в четвертое (4-е) состояние
/// Events[5] для analog BL_HI, для const - переход в пятое (5-е) состояние
/// Events[6] для const - переход в шестое (6-е) состояние
/// Events[7] для const - переход в седьмое (7-е) состояние

typedef struct  S_Data
        {
				/// Глобальный дескриптор
		TGID	gid;            
				/// Дескриптор драйвера времени выполнения
        long    driver_id;          
				/// Тег канала драйвера (идентификатор тэга в контексте драйвера)
        char    item[long_name];       
				/// Тег канала в системе Vinca
        char    name[short_name];       
				/// Наименование параметра
        char    desc[long_name];
				/// Тип параметра. см. DATA_COD
unsigned short  type;           
				/// Код группы/Блок установки
        short   block;          
				/// Набор флагов реакции системы на события
		t_ActionSeverity react[8];
				/// Бит маска видимости/доступа/команды
		Access	access;
				/// Порядковый номер в блоке для отображения в таблицах блоков
		long	number;
    union 
        {
				/// аналоговый сигнал
        t_analog    analog;     
				/// цифровой сигнал
        t_digital   digital;
				/// константа
        t_const     constant;
				/// Блок аналогового управления
        t_actl      actl;
				/// Блок цифрового управления
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
	{//обязательные
		colors.push_back(RGB(0  , 255, 0  ));
		colors.push_back(RGB(144, 0  , 144));
		colors.push_back(RGB(255, 255, 0  ));
		colors.push_back(RGB(255, 0  , 0  ));
		colors.push_back(RGB(255, 0  , 255));
		colors.push_back(RGB(0  , 240, 240));
		colors.push_back(RGB(255, 127, 0  ));
		colors.push_back(RGB(160, 160, 160));

//дополнительные
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
