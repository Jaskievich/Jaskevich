#include "CVQT.h"
#include "opcaedef.h"
//#include <boost/format.hpp>

//using namespace boost;

namespace {
TTime t1970 = 0x019DB1DED53E8000; // Time in 100 nsec intervals on 1 jan,1979 year GMT.
    };


TQuality::operator FieldsQuality()
{
	return fQ;// 
}

//##ModelId=39535F7202F0
TValue CVQT::getValue() const
{
	return m_Value;
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
}

//##ModelId=39535F7202D2
TQuality CVQT::getQuality() const
{
	return m_Quality;
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
}

//##ModelId=39535F7202AA
TTime CVQT::getTime() const
{
	return (m_Time.getTime());
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
}




//##ModelId=393BB073016A
void CVQT::setValue(TValue value)
{
	m_Value = value;
	return;
}



//##ModelId=393BB07301D8
void CVQT::setQuality(TQuality value)
{
	m_Quality = value;
	return;
}

void CVQT::setQuality(FieldsQuality value)
{
   	m_Quality = TQuality(value);
    return;

}
void CVQT::setQuality(unsigned short value)
{
   	m_Quality = TQuality(value);
    return;
	
}

//##ModelId=393BB0730250
void CVQT::setTime(TTime value)
{
	m_Time = value;
	return;
}

//##ModelId=393BB07302AA
CVQT::CVQT() :
    m_Value(0),
    m_Quality(0),
    m_Time(0)
{
	// ToDo: Add your specialized code here and/or call the base class
}

//##ModelId=393BB07302C8
CVQT::CVQT(const CVQT& orig):
    m_Value(orig.m_Value),
    m_Quality(orig.m_Quality),
    m_Time(orig.m_Time)
{
	// ToDo: Add your specialized code here and/or call the base class
}

CVQT::CVQT(const VQT& value):
    m_Value(value.m_Value),
    m_Quality(value.m_wQuality),
    m_Time(value.m_Time)
{
	// ToDo: Add your specialized code here and/or call the base class
}

//##ModelId=393BB0730322
CVQT::~CVQT()
{
	// ToDo: Add your specialized code here and/or call the base class
}

//##ModelId=393BB0730340
CVQT& CVQT::operator=(const CVQT& rhs)
{
	// ToDo: Add your specialized code here and/or call the base class
if(this==&rhs) return *this;
m_Value = rhs.m_Value;
m_Quality = rhs.m_Quality;
m_Time = rhs.m_Time;
	return *this;
}

CVQT& CVQT::operator=(const VQT& rhs)
{
	// ToDo: Add your specialized code here and/or call the base class
m_Value = rhs.m_Value;
m_Quality = rhs.m_Quality;
m_Time = rhs.m_Time;
	return *this;
}


CVQT::operator VQT()
{
	// ToDo: Add your specialized code here and/or call the base class
VQT tmp;
tmp.m_Value   = getValue();
tmp.m_Quality = getQuality();
tmp.m_Time    = getTime();
	return tmp;
}

 TStamp::operator SYSTEMTIME() const
            {
              FILETIME ft = operator FILETIME();
			  SYSTEMTIME st;
			  FileTimeToSystemTime(&ft, &st);	
              return st;
            }

SYSTEMTIME TStamp::getLocalTime() const
		{
              FILETIME ft = operator FILETIME();
			  FILETIME lft;
			  FileTimeToLocalFileTime(&ft, &lft);
			  SYSTEMTIME st;
			  FileTimeToSystemTime(&lft, &st);	
              return st;
		}

#ifdef WIN32
BOOL TStamp::setLocalTime(const TTime lt)
{
	FILETIME ft;
	FILETIME lft = TStamp(lt);
	if(LocalFileTimeToFileTime(&lft, &ft) == 0)
		return FALSE;
	operator=(ft);
	return TRUE;
}

BOOL TStamp::setLocalTime(const SYSTEMTIME lst)
{
	FILETIME ft;
	FILETIME lft = TStamp(lst);
	if(LocalFileTimeToFileTime(&lft, &ft) == 0)
		return FALSE;
	operator=(ft);
	return TRUE;
}
#endif

BOOL TStamp::setNow()
            {
#ifdef WIN32
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            ULARGE_INTEGER tmp;
            tmp.LowPart = ft.dwLowDateTime;
            tmp.HighPart = ft.dwHighDateTime;
            m_stamp = tmp.QuadPart;
#else
            time_t ltime;
            time(&ltime);
            m_stamp = ((TTime)ltime) * (TTime)10000000 + t1970;
#endif
            return TRUE;
            };



ostream& operator<< (ostream& os, const FieldsQuality& e)
 {
        os << "Q: " << e.Quality<<" Sub: " << e.Substatus << " Lim: "
            << e.LimitSt <<" Ven: "<< e.Vendor;
   return os;
    }

//VINCALIB_API std::ostream& operator<< (std::ostream& os, const TQuality& e)
// {
//        os << "Q: " << e.fQ.Quality<<" Sub: " << e.fQ.Substatus << " Lim: "
//            << e.fQ.LimitSt <<" Ven: "<< e.fQ.Vendor;
//   return os;
//    }

ostream& operator<< (ostream& os, const TQuality& e)
{
	if((unsigned short)e == OPC_QUALITY_SENSOR_CAL)
	{
		os << "калибровка";
		return os;
	}
	switch( e.fQ.Quality )
    {
		
	case TQuality::Bad:
		os << "недостоверен";
		switch( e.fQ.Substatus )
		{
		case TQuality::ConfigErr:
			os << ": ошибка конфигурации";
			break;
			
		case TQuality::NotConnected:
			os << ": не подсоединён";
			break;
			
		case TQuality::DeviceFail:
			os << ": ошибка контроллера";
			break;
			
		case TQuality::SensorFail:
			os << ": ошибка датчика";
            break;
			
		case TQuality::LastKnownValue:
			os << ": последнее известное";
			break;
			
		case TQuality::CommFail:
			os << ": нет связи";
			break;
			
		case TQuality::OutOfService:
			os << ": нет обновления";
            break;
		default:
			
			break;
		}
		
		break;
		
		case TQuality::Uncertain:
			os << "сомнителен";
//			if((e.fQ.Substatus & TQuality::LastUsableValue) == TQuality::LastUsableValue)
//				os << ": последнее использованное";
//			if((e.fQ.Substatus & TQuality::SensorNAccurate) == TQuality::SensorNAccurate)
//				os << ": погрешность датчика";
//			if((e.fQ.Substatus & TQuality::EUExceeded) == TQuality::EUExceeded)
//				os << ": зашкал";
//			if((e.fQ.Substatus & TQuality::SubNormal) == TQuality::SubNormal)
//				os << ": около нормы";
			switch( e.fQ.Substatus )
			{
			case TQuality::LastUsableValue:
				os << ": последнее известное";
				break;
				
//			case TQuality::SensorNAccurate:
//				os << ": погрешность датчика";
//				break;
				
			case TQuality::EUExceeded:
					if((e.fQ.LimitSt & TQuality::Low) == TQuality::Low)
						os << ", ниже ";
					else if((e.fQ.LimitSt & TQuality::High) == TQuality::High)
						os << ", выше ";
					os << "шкалы";
//				os << ": зашкал";
				break;
				
			case TQuality::SubNormal:
				os << ": около нормы";
				break;
			default:
				break;
			}
			
			break;
			
			case TQuality::Good:
				os << "достоверен";
//				if((e.fQ.Substatus & TQuality::LocalOverride) == TQuality::LocalOverride)
//					os << ": перезаписано локально";
//				if((e.fQ.Substatus & TQuality::EUExceeded) == TQuality::EUExceeded)
//					os << ": дребезг зашкала";
				switch( e.fQ.Substatus )
				{
				case TQuality::LocalOverride:
					os << ": перезаписано локально";
					break;
				case TQuality::EUExceeded:
					if((e.fQ.LimitSt & TQuality::Low) == TQuality::Low)
						os << ", ниже ";
					else //if((e.fQ.LimitSt & TQuality::High) == TQuality::High)
						os << ", выше ";
					os << "шкалы";
				break;
				default:
					break;
				}
				
				break;
    }
	
	if(e.fQ.Vendor != 0)
	{
	if((e.fQ.Vendor & TQuality::NotRefreshed) == TQuality::NotRefreshed)
		os << ", нет обновления";
	else if((e.fQ.Vendor & TQuality::Disabled) == TQuality::Disabled)
		os << ", отключён";
	else if((e.fQ.Vendor & TQuality::DisabledExportAlarm) == TQuality::DisabledExportAlarm)
		os << ", отключены события";
	if((e.fQ.LimitSt & TQuality::Low) == TQuality::Low)
			os << ", ниже ";
		else if((e.fQ.LimitSt & TQuality::High) == TQuality::High)
			os << ", выше ";
		
	if((e.fQ.Vendor & TQuality::Block) == TQuality::Block)
		os << " блокировки";
	else if((e.fQ.Vendor & TQuality::Signal) == TQuality::Signal)
		os << " сигнализации";
	else if((e.fQ.Vendor & TQuality::Reglament) == TQuality::Reglament)
		os << " регламента";
	else if((e.fQ.Vendor & TQuality::TehnoControl) == TQuality::TehnoControl)
		os << " тех. уставки";
	}
	if((e.fQ.Vendor & TQuality::Field) == TQuality::Field)
		os << ", с поля";
	return os;
}

bool TQuality::IsGood()
{
	return (fQ.Quality == Good); 
}

bool TQuality::IsBad()
{
	return (fQ.Quality == Bad); 
}

bool TQuality::IsUncertain()
{
	return (fQ.Quality == Uncertain); 
}

bool TQuality::IsConfigErr()
{
	return (IsBad() && fQ.Substatus == ConfigErr); 
}

bool TQuality::IsNotConnected()
{
	return (IsBad() && fQ.Substatus == NotConnected); 
}

bool TQuality::IsDeviceFail()
{
	return (IsBad() && fQ.Substatus == DeviceFail); 
}

bool TQuality::IsSensorFail()
{
	return (IsBad() && fQ.Substatus == SensorFail); 
}

bool TQuality::IsLastKnownValue()
{
	return (IsBad() && fQ.Substatus == LastKnownValue); 
}

bool TQuality::IsCommFail()
{
	return (IsBad() && fQ.Substatus == CommFail); 
}

bool TQuality::IsOutOfService()
{
	return (IsBad() && fQ.Substatus == OutOfService); 
}

bool TQuality::IsLastUsableValue()
{
	return (IsUncertain() && fQ.Substatus == LastUsableValue); 
}

bool TQuality::IsSensorCalibrate()
{
	return (IsUncertain() && fQ.Substatus == SensorCalibrate); 
}
bool TQuality::IsEUExceeded()
{
	return (IsUncertain() && fQ.Substatus == EUExceeded); 
}

bool TQuality::IsSubNormal()
{
	return (IsUncertain() && fQ.Substatus == SubNormal); 
}

bool TQuality::IsLocalOverride()
{
	return (IsGood() && fQ.Substatus == LocalOverride); 
}

bool TQuality::IsLow()
{
	return (fQ.LimitSt == Low); 
}

bool TQuality::IsHigh()
{
	return (fQ.LimitSt == High); 
}

bool TQuality::IsReglament()
{
	return ((fQ.Vendor & Reglament)== Reglament); 
}

bool TQuality::IsSignal()
{
	return ((fQ.Vendor & Signal)== Signal); 
}

bool TQuality::IsBlock()
{
	return ((fQ.Vendor & Block)== Block); 
}

bool TQuality::IsNotRefreshed()
{
	return ((fQ.Vendor & NotRefreshed)== NotRefreshed); 
}

ostream& TStamp::Print(ostream& os)
{
        ULARGE_INTEGER tmp;
        tmp.QuadPart = getTime();
        FILETIME   ft;
        ft.dwLowDateTime = tmp.LowPart;
        ft.dwHighDateTime = tmp.HighPart;
        SYSTEMTIME st, lt;
        FileTimeToSystemTime(&ft, &st);                 //  Add error control !!!
        SystemTimeToTzSpecificLocalTime(NULL, &st, &lt);
// Вывод времени в формате с ведущими нулями для правильного поиска по времени
//		boost::format TimeFmt("%02d.%02d.%04d %02d:%02d:%02d,%03d");
//        os << format(TimeFmt) % lt.wDay % lt.wMonth % lt.wYear % lt.wHour %
//					lt.wMinute % lt.wSecond % lt.wMilliseconds;


        os << lt.wDay << "." << lt.wMonth << "." << lt.wYear;
        os << " " << lt.wHour <<":" << lt.wMinute <<":" << lt.wSecond <<"," << lt.wMilliseconds;
   return os;
}

std::ostream& operator<< (std::ostream& os, const TStamp& e)
    {
        ULARGE_INTEGER tmp;
        tmp.QuadPart = e.getTime();
        FILETIME   ft;
        ft.dwLowDateTime = tmp.LowPart;
        ft.dwHighDateTime = tmp.HighPart;
        SYSTEMTIME st, lt;
        FileTimeToSystemTime(&ft, &st);                 //  Add error control !!!
        SystemTimeToTzSpecificLocalTime(NULL, &st, &lt);
// Вывод времени в формате с ведущими нулями для правильного поиска по времени
//		boost::format TimeFmt("%02d.%02d.%04d %02d:%02d:%02d,%03d");
//        os << format(TimeFmt) % lt.wDay % lt.wMonth % lt.wYear % lt.wHour %
//					lt.wMinute % lt.wSecond % lt.wMilliseconds;


        os << lt.wDay << "." << lt.wMonth << "." << lt.wYear;
        os << " " << lt.wHour <<":" << lt.wMinute <<":" << lt.wSecond <<"," << lt.wMilliseconds;
   return os;
    }
std::ostream& operator<< (std::ostream& os, const CVQT& e)
   {
//    if(os.flags() & ios::binary)
//        {
//        TValue    val(e.getValue());
//        TStamp          t(e.getTime());
//        FieldsQuality   q(e.getQuality());
//        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
//
//        os.write(reinterpret_cast<const char*>(&q),sizeof(FieldsQuality));
//        os.write(reinterpret_cast<const char*>(&t), sizeof(TTime));
//        os << t;
//        }
//    else
//        {
//	os << e.operator VQT;
    os << e.getValue();
    os << "\t";
    os << e.getQuality();
	TStamp tm(e.getTime());
    os << "\t";
	tm.Print(os);
	os << endl;
//        }
   return os;
}

std::ostream& operator<< (std::ostream& os, const VQT& e) {
//    if(os.flags() & ios::binary)
//        {
//        os.write(reinterpret_cast<const char*>(&e.m_Value), sizeof(e.m_Value));
//        os.write(reinterpret_cast<const char*>(&e.m_Quality), sizeof(e.m_Quality));
//        os.write(reinterpret_cast<const char*>(&e.m_Time), sizeof(e.m_Time));
//        }
//    else
//        {
    os << e.m_Value;
    os << "\t";
    os << e.m_Quality;
	TStamp tm(e.m_Time);
    os << "\t";
	tm.Print(os);
	os << endl;
//        }
   return os;
}


std::istream& operator>> (std::istream& os, VQT& e) {
    if(os.flags() & ios::binary)
        {
        os.read(reinterpret_cast<char*>(&e.m_Value), sizeof(e.m_Value));
        os.read(reinterpret_cast<char*>(&e.m_Quality), sizeof(e.m_Quality));
        os.read(reinterpret_cast<char*>(&e.m_Time), sizeof(e.m_Time));
        }
    else
        {
        }
   return os;
}

std::istream& operator>> (std::istream& os, CVQT& e) {
    VQT v;
    if(os.flags() & ios::binary)
        {
        os >> v;
        e = v;
        }
    else
        {
        }
   return os;
}

