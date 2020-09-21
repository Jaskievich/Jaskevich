#ifndef __CVQT_H__
#define __CVQT_H__


#ifndef WIN32
#include <ctime>   // For TStamp::set_Now() on posix like OS
#endif



//#ifdef WIN32
#include <windows.h>
#include "decldef.h"
typedef LONGLONG TTime;  // For TStamp::TTime  in WIN32 __int64, in posix-like OS longlong
#define _MSECOND ((TTime) 10000)
#define _SECOND ((TTime) 10000000)
#define _MINUTE ((TTime)60 * _SECOND)
#define _HOUR   ((TTime)60 * _MINUTE)
#define _DAY    ((TTime)24 * _HOUR) 
//#else
//typedef longlong TTime;
//#endif

#define MSEC	1000 //мсек в секунде
#pragma pack( push, 1)

///Real time data for tag - Value, Quality and Time.
//##ModelId=39D321A20240
typedef struct tagFieldsQuality
     {
         unsigned short LimitSt:2;
         unsigned short Substatus:4; /// Substatus bit field depends on the Quality Field, OPC DA2.03
         unsigned short Quality:2;  ///  Quality bit field: 0- Bad, 1- Uncertain, 2-N/A, 3-Good
         unsigned short Vendor:8;
     }FieldsQuality;

typedef double       TValue;
//    typedef ::TTime     TTime;
//    typedef ::TQuality  TQuality;

typedef struct TVQT
    {
            TValue          m_Value;
			union
			{
            FieldsQuality   m_Quality;
			WORD			m_wQuality;
			};
            TTime           m_Time;

    } VQT;

#ifdef __cplusplus

#include <iostream>
//#include "opcda.h"
using namespace std;


#ifdef WIN32
class TStamp
    {
    public:    
        TStamp():
          m_stamp(0)
              {}
        TStamp(const TStamp& right):
          m_stamp(right.m_stamp)
              {}
        TStamp(const TTime& right):
          m_stamp(right)
              {}
        TStamp(const FILETIME& from):
          m_stamp(0)
              {
              ULARGE_INTEGER  st;
              st.LowPart  = from.dwLowDateTime;
              st.HighPart = from.dwHighDateTime;
              m_stamp     = st.QuadPart;  
              }
        TStamp(const SYSTEMTIME& from):
          m_stamp(0)
              {
            FILETIME ft;
            SystemTimeToFileTime(&from, &ft);
              ULARGE_INTEGER  st;
              st.LowPart  = ft.dwLowDateTime;
              st.HighPart = ft.dwHighDateTime;
              m_stamp     = st.QuadPart;  
              }
        operator FILETIME() const
            {
              ULARGE_INTEGER  st;
              FILETIME ft;
              st.QuadPart = m_stamp;  
              ft.dwLowDateTime  = st.LowPart;
              ft.dwHighDateTime = st.HighPart;
              return ft;
            }
        operator SYSTEMTIME() const;
        TStamp&         operator=(const FILETIME& right)
            {
              ULARGE_INTEGER  st;
              st.LowPart  = right.dwLowDateTime;
              st.HighPart = right.dwHighDateTime;
              m_stamp     = st.QuadPart;  
            return *this;
            }

        TStamp&         operator=(const SYSTEMTIME& right)
            {
            FILETIME ft;
            SystemTimeToFileTime(&right, &ft);
              ULARGE_INTEGER  st;
              st.LowPart  = ft.dwLowDateTime;
              st.HighPart = ft.dwHighDateTime;
              m_stamp     = st.QuadPart;  
            return *this;
            }
		SYSTEMTIME getLocalTime() const;
        BOOL  setLocalTime(const TTime lt);
        BOOL  setLocalTime(const SYSTEMTIME lst);
        ~TStamp(){}

#endif

        operator TTime() const
            {return m_stamp;}
        const TTime getTime() const
			{return m_stamp;}
        BOOL  setTime(const TTime& st){m_stamp = st; return TRUE;}
        TStamp&         operator=(const TStamp& right)
            {
            if(&right == this) return *this;
            m_stamp = right.m_stamp;
            return *this;
            }
        TStamp&         operator=(const TTime& right)
            {
            if(right == m_stamp) return *this;
            setTime(right);
            return *this;
            }
		// Decrement time in sec.
        TStamp&         operator-=(const long& right)
            {
            m_stamp -= (TTime)right * _SECOND;
            return *this;
            }
		// Increment time in sec.
        TStamp&         operator+=(const long& right)
            {
            m_stamp += (TTime)right * _SECOND;
            return *this;
            }
    
        BOOL setNow();
		ostream& Print(ostream& os);
        friend ostream& operator<< (ostream& os, const TStamp& e);

    private:
        TTime m_stamp;
    };


#define OPC_VENDOR_REGLAMENT	0x0100
#define OPC_VENDOR_SIGNAL		0x0200
#define OPC_VENDOR_BLOCK		0x0400
#define OPC_VENDOR_ALARM_MASK	0x0700
#define OPC_VENDOR_FIELD		0x0800 //сигнализация с поля
#define OPC_VENDOR_NOTREFRESHED 0x8000
#define OPC_VENDOR_DISABLED		0x6000
#define OPC_VENDOR_DISABLED_ALARM	0x2000
// Остальные маски определены в файле opcaedef.h

struct TQuality
    {

   public:
    ///Quality 
    enum QT{ Bad = 0, Uncertain=1, Good=3};

    ///Substatus for Bad Quality
    enum S_BAD{
		NonSpec = 0,	// нет
		ConfigErr,		// ошибка конфигурации
		NotConnected,	// не подсоединен
		DeviceFail,		// ошибка контроллера
		SensorFail,		// ошибка датчика
		LastKnownValue, // последнее известное
		CommFail,		// ошибка связи
		OutOfService	// обновление отключено
	};

    ///Substatus for Uncertain Quality
    enum S_UN
	{ 
		NSpecific = 0,		// нет
		LastUsableValue=1,  // последнее использованное
		SensorCalibrate=4,	// Калибрация(поверка) датчика
//		SensorNAccurate=4,	// погрешность датчика
		EUExceeded=5,		// зашкал
		SubNormal=6			// около нормы
	};

    ///Substatus for Good Quality
    enum S_GOOD
	{
		Non = 0,		// нет
		LocalOverride=6	// перезаписано локально 
	};

    /// Limit Status
    enum LIMIT
	{ 
		NotLimited = 0, // нет
		Low=1,			// ниже
		High=2,			// выше
		Constant=3		// постоянное
	};


    enum VENDOR
	{
		Not		= 0,
		Reglament = 1,	// регламент
		Signal=2,		// сигнализация
		Block=4,		// блокировка
		Field=8,		//сигнализация с поля
		TehnoControl = 0x10,
		DisabledExportAlarm = 0x20,
		Disabled = 0x60,
		NotRefreshed = 0x80
	};

    public:
        TQuality(const unsigned short fq)
            {
            memcpy(&fQ, &fq, sizeof(FieldsQuality));

            }
        TQuality(const int& fq, const int& fsub=0, const int& flimit=0, const int& fvendor=0)
            {
            fQ.Quality = fq;
            fQ.Substatus = fsub;
            fQ.LimitSt = flimit;
            fQ.Vendor = fvendor;
            }
        TQuality(const FieldsQuality FQ)
            {
            fQ.Quality = FQ.Quality;
            fQ.Substatus = FQ.Substatus;
            fQ.LimitSt = FQ.LimitSt;
            fQ.Vendor = FQ.Vendor;
            }
        TQuality& operator=(FieldsQuality& FQ)
            {
            fQ.Quality = FQ.Quality;
            fQ.Substatus = FQ.Substatus;
            fQ.LimitSt = FQ.LimitSt;
            fQ.Vendor = FQ.Vendor;
            return *this;
            }
        TQuality& operator=(unsigned short& right)
            {
            memcpy(&fQ, &right, sizeof(FieldsQuality));
            return *this;
            }
        ~TQuality(){};
        operator FieldsQuality();
        operator unsigned short()
            {
            unsigned short ret = 0;
            memcpy(&ret, &fQ, sizeof(FieldsQuality));
            return ret;
            }
        operator unsigned short() const
            {
            unsigned short ret = 0;
			int sizeF = sizeof(FieldsQuality);
            memcpy(&ret, &fQ, sizeF);
            return ret;
            }
		//std::string& print(std::string &tostr);
        friend ostream& operator<< (ostream& os, const TQuality& e);
		bool IsGood();
		bool IsBad();
		bool IsUncertain();
		// for bad
		bool IsConfigErr();
		bool IsNotConnected();
		bool IsDeviceFail();
		bool IsSensorFail();
		bool IsLastKnownValue();
		bool IsCommFail();
		bool IsOutOfService();
		// for Uncertain
		bool IsLastUsableValue();
		bool IsSensorCalibrate();
		bool IsEUExceeded();
		bool IsSubNormal();
		// for good
		bool IsLocalOverride();
		bool IsLow();
		bool IsHigh();
		bool IsReglament();
		bool IsSignal();
		bool IsBlock();
		bool IsNotRefreshed();
		COLORREF GetColor();
	public:
    FieldsQuality   fQ;
        
    };

class CVQT
{
public:

// selectors
public:

    TValue        getValue() const;
    TQuality      getQuality() const;
    TTime         getTime() const;

        //##ModelId=393BB07300F2

/// modifiers

        //##ModelId=39D321A202A5
    void setQuality(const int& fQt, const int& fStatus=0, const int& fLimit=0, const int& fVendor=0)
        {
                m_Quality = TQuality(fQt, fStatus, fLimit, fVendor);
        }


        void setValue(TValue value);
		void setQuality(unsigned short value);
        void setQuality(TQuality value);
        void setQuality(FieldsQuality value);
        void setTime(TTime value);
    BOOL setNow() {return m_Time.setNow();}


        //##ModelId=39D321A20287
        CVQT();

        //##ModelId=39D321A20290
        CVQT(const CVQT& orig);

        CVQT(const VQT& value);
        //##ModelId=39D321A20286
        virtual ~CVQT();

        //##ModelId=39D321A2027C
        CVQT& operator=(const CVQT& rhs);
        CVQT& operator=(const VQT& rhs);
    operator VQT();
    friend ostream& operator<< (ostream& os, const CVQT& e);

private:
        TValue      m_Value;
        TQuality    m_Quality;
        TStamp      m_Time;
};


#pragma pack( pop )
#endif // 

#endif //__CVQT_H__
