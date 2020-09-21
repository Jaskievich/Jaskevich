// LTArchive.cpp: implementation of the CLTArchive class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cmath> 

#include "LTArchive.h"
#include "zlib.h"


const short CLTArchive::RangeVal = 4000;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace
{
	const char* LTASignatura = "Vinca2.1LTA";
//	bool QualityToLTAItem(TQuality quality, LTAItem &item);
//	unsigned short QualityFromLTAItem( const LTAItem &item);

//----------------------------------------------------------------------------------
bool QualityToLTAItem(TQuality quality, LTAItem &item)
{
	item.limit = NO_LIMIT;
	if(quality.IsHigh())
	{
		item.limit = 0;
	}
	if(quality.IsBlock())	item.limit += 1;
	else if(quality.IsSignal())	item.limit += 2;
	else if(quality.IsReglament())	item.limit += 3;
	
	if(quality.IsGood())
		item.quality = GOOD;
	if(quality.IsBad())	
	{
		item.quality = UNCERTAIN;
		item.limit = BAD_VAL;
	}
	if(quality.IsUncertain())	item.quality = UNCERTAIN;
	return true;
}
//----------------------------------------------------------------------------------
	unsigned short QualityFromLTAItem(const LTAItem &item)
	{

		FieldsQuality   fQ;
		fQ.Substatus =	TQuality::Non;
		fQ.Vendor = TQuality::Not;
		if(item.quality == GOOD)
		{
			fQ.Quality = TQuality::Good;
		}
		else if(item.quality == UNCERTAIN)
		{
			fQ.Quality = TQuality::Uncertain;
		}
		if(item.limit < NO_LIMIT)
			fQ.LimitSt = TQuality::High;
		else if(item.limit > NO_LIMIT)
			fQ.LimitSt = TQuality::Low;

		switch(item.limit)
		{
		case HI_BLOCK:
		case LO_BLOCK:
			fQ.Vendor = TQuality::Block;
			break;
		case HI_SIG:
		case LO_SIG:
			fQ.Vendor = TQuality::Signal;
			break;
		case HI_REG:
		case LO_REG:
			fQ.Vendor = TQuality::Reglament;
			break;
		case NO_LIMIT:
			fQ.LimitSt = TQuality::NotLimited;
			fQ.Vendor =		TQuality::Not;
			break;
		case BAD_VAL:
		default:
			fQ.Vendor =		TQuality::Not;
			fQ.Quality =	TQuality::Bad;
			fQ.LimitSt =	TQuality::NotLimited;
			break;
//			fQ.Vendor = TQuality::Not;
		}
      unsigned short ret = 0;
      memcpy(&ret, &fQ, sizeof(FieldsQuality));
      return ret;

	}
}

//-------------------------------------------------------------------------
CLTArchive::CLTArchive(): m_State(NotInit),
						  m_HeaderLen(0),
						  m_CountTags(0),	
						  m_RecPerTag(0),	
						  m_LastRec(0),	
						  m_Period(0),	
						  m_FirstTime(0),	
						  m_LastTime(0)
{
m_Signatura[0] = 0;
}
//-------------------------------------------------------------------------
CLTArchive::~CLTArchive()
{
}

//-------------------------------------------------------------------------
std::ostream& CLTArchive::PrintHeader(std::ostream& os)
{
	os << "Signatura-\t" << m_Signatura
		<< "\nPlant-\t" << m_Plant
		<< "\nRecPerTag-\t" << m_RecPerTag
		<< "\nLastRec-\t" << m_LastRec
		<< "\nPeriod-\t" << m_Period << " sec." 
		<< "\nFirstTime-\t" << TStamp(m_FirstTime)
		<< "\nLastTime-\t" << TStamp(m_LastTime)
		<< "\nCountTags-\t" << m_CountTags
		<< "\nCountBlocks-\t" << m_Blocks.size() << "\n";
 return os; 
}

//-------------------------------------------------------------------------
std::ostream& CLTArchive::PrintTagHead(std::ostream& os, const long tagindex)
{
	LTARecArrayT::reference rec = m_RecHeads.at(tagindex);
	os << "\n\nGID=\t" << rec.gid
		<< "\n\tTagName-\t" << rec.TagName
		<< "\n\tEU-\t" << rec.EU
		<< "\n\tTagDesc-\t" << rec.TagDesc
		<< "\n\tType-\t";
		short type = rec.type;
		type &= ~C_Output;
		switch(type)
	{
		case C_Analog:
			os << "Analog ";
			break;
		case C_Digital:
			os << "Digital ";
			break;
		case C_Const:
			os << "Const ";
			break;
		case C_AnalogCtl:
			os << "AnalogCtl ";
			break;
		case C_DigitalCtl:
			os << "DigitalCtl ";
			break;
		default:
			os << "None ";
			break;
	}
		if((rec.type & C_Output) == C_Output)
			os << "Output";
		else
			os << "Input";
//		if(rec.block > 0)
			os  << "\n\tBlock-\t" << m_Blocks.at(rec.block-1);
//		else
//			os  << "\nBlock-\t" << m_Blocks.at(rec.block);
		os 	<< "\n\tSC_HI=\t" << rec.SC_HI
		 	<< "\n\tSC_LO=\t" << rec.SC_LO
		 	<< "\n\tMaxVal=\t" << rec.MaxVal
		 	<< "\n\tMinVal=\t" << rec.MinVal << "\n";
 return os; 
}

//-------------------------------------------------------------------------
std::ostream& CLTArchive::PrintTagValues(std::ostream& os, const long tagindex)
{
	deque< VQT > array;
	if(GetDataByIndex(tagindex, array))
	{
		for(deque< VQT >::iterator it = array.begin();
			it != array.end(); ++it)
			{
                os << "\n" << &(*it);
			}
	}
 return os; 
}

//-------------------------------------------------------------------------
std::ostream& CLTArchive::Print(std::ostream& os, const bool IsOnlyHeaders)
{
	dword tagindex = 0;
	try
	{
	if(IsValid())
	{
		PrintHeader(os);
    //	boost::progress_display show_progress(m_CountTags);

		for(;
			tagindex < m_CountTags;
			tagindex++)
		{
			PrintTagHead(os, tagindex);
			if(!IsOnlyHeaders)
				PrintTagValues(os, tagindex);
//			PrintTagHead(os, 10);
//			PrintTagValues(os, 10);
//			PrintTagHead(os, 11);
//			PrintTagValues(os, 11);
    //		++show_progress;
		}
	}
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		cerr << "ERROR_NOT_ENOUGH_MEMORY";
	}
	catch(std::exception &ex)
	{
		cerr << ex.what();
	}
	return os;
}

//-------------------------------------------------------------------------
bool CLTArchive::Open(LPCTSTR Name)
{
	try
	{
    ifstream file(Name, ios_base::binary);
	if(!file.is_open())
	{
		SetLastError(ERROR_FILE_EXISTS);
		return false;
	}
	m_Name = Name;
//	filebuf *fbuf = file.rdbuf();
//	if(!fbuf)
//		return false;
	file.read(reinterpret_cast<char*>(&m_HeaderLen), sizeof(m_HeaderLen));
	file.read(reinterpret_cast<char*>(m_Signatura), sizeof(m_Signatura));
	if(strcmp(m_Signatura, LTASignatura) != 0)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return false;
	}
	char	plant[short_name] = "";
	file.read(reinterpret_cast<char*>(plant), sizeof(plant));
	m_Plant = plant;
	file.read(reinterpret_cast<char*>(&m_RecPerTag), sizeof(m_RecPerTag));
	file.read(reinterpret_cast<char*>(&m_LastRec), sizeof(m_LastRec));
	file.read(reinterpret_cast<char*>(&m_Period), sizeof(m_Period));
	file.read(reinterpret_cast<char*>(&m_FirstTime), sizeof(m_FirstTime));
	file.read(reinterpret_cast<char*>(&m_LastTime), sizeof(m_LastTime));
	file.read(reinterpret_cast<char*>(&m_CountTags), sizeof(m_CountTags));
	dword	CountBlocks = 0;
	file.read(reinterpret_cast<char*>(&CountBlocks), sizeof(CountBlocks));
	m_Blocks.resize(CountBlocks);
	for(dword i = 0; i < CountBlocks; i++)
	{
		char block[short_name];
		block[0] = '\0';
		file.read(reinterpret_cast<char*>(block), sizeof(block));
		block[short_name-1] = '\0';
		m_Blocks[i] = block;
	}
	m_RecHeads.resize(m_CountTags);
	file.seekg(m_HeaderLen);//, ios::beg);
	long offset_rec = m_HeaderLen;
	for(dword j = 0; j < m_CountTags; j++)
	{
		LTAHeadRec lta_head_rec;
		if(!ReadLTAHeadRec(&lta_head_rec, file, offset_rec))
		{
			SetLastError(ERROR_BAD_FORMAT);
			return false;
		}
		m_RecHeads[j] = lta_head_rec;
		offset_rec = lta_head_rec.LenRecHeadFile + lta_head_rec.HeadOffSet;
		//file.seekg(lta_head_rec.LenRecHeadFile + lta_head_rec.HeadOffSet);
	}
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
m_State = Valid;
return true;	// 
	// 
}
bool CLTArchive::ReadLTAHeadRec(LTAHeadRec *lta_head_rec, ifstream &file, const long offset_rec)
{
//	basic_filebuf< char > *fbuf = file.rdbuf();
	if(!file.is_open())
	{
		SetLastError(ERROR_FILE_EXISTS);
		return false;
	}
//	filebuf *fbuf = file.rdbuf();
//	if(!fbuf)
//		return false;
	file.clear();
	file.seekg(offset_rec);
#ifdef _DEBUG 
	std::streampos offset = file.tellg();
#endif

	file.read(reinterpret_cast<char*>(&lta_head_rec->LenRecHeadFile), sizeof(lta_head_rec->LenRecHeadFile));
	file.read(reinterpret_cast<char*>(&lta_head_rec->gid), sizeof(lta_head_rec->gid));
	file.read(reinterpret_cast<char*>(&lta_head_rec->DataSize), sizeof(lta_head_rec->DataSize));
	file.read(reinterpret_cast<char*>(&lta_head_rec->DataOffSet), sizeof(lta_head_rec->DataOffSet));
	char TagName[short_name];
	TagName[0] = '\0'; 
	file.read(reinterpret_cast<char*>(TagName), sizeof(TagName));
	lta_head_rec->TagName =  TagName;
	char EU[short_name];
	EU[0] = '\0'; 
	file.read(reinterpret_cast<char*>(EU), sizeof(EU));
	lta_head_rec->EU =  EU;
	file.read(reinterpret_cast<char*>(&lta_head_rec->type), sizeof(lta_head_rec->type));
	file.read(reinterpret_cast<char*>(&lta_head_rec->block), sizeof(lta_head_rec->block));
	file.read(reinterpret_cast<char*>(&lta_head_rec->SC_HI), sizeof(lta_head_rec->SC_HI));
	file.read(reinterpret_cast<char*>(&lta_head_rec->SC_LO), sizeof(lta_head_rec->SC_LO));
	file.read(reinterpret_cast<char*>(&lta_head_rec->MaxVal), sizeof(lta_head_rec->MaxVal));
	file.read(reinterpret_cast<char*>(&lta_head_rec->MinVal), sizeof(lta_head_rec->MinVal));
	short		TagDescLen = 0;
	file.read(reinterpret_cast<char*>(&TagDescLen), sizeof(TagDescLen));
	if(TagDescLen >= long_name || TagDescLen < 0)
		TagDescLen = long_name - 1;
	char		TagDesc[long_name];
//	TagDesc[0]= '\0';
	file.read(reinterpret_cast<char*>(TagDesc), TagDescLen);
	TagDesc[TagDescLen]= '\0';
	lta_head_rec->TagDesc = TagDesc;
	lta_head_rec->HeadOffSet = offset_rec;
	return true;
	// 
}

//-------------------------------------------------------------------------
bool CLTArchive::Create(LPCTSTR Name,LPCTSTR Plant,const dword RecPerTag,const dword Period,
						const TTime FirstTime,const TTime LastTime,const dword CountTags,
						std::vector<t_block> &Blocks)
{
	try
	{
		ofstream file(Name, ios::binary | ios::trunc);
	if(!file.is_open())
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
    m_Name = Name;
	m_Plant = Plant;
	m_Period = Period;
	m_FirstTime = FirstTime;
	m_LastTime = LastTime;
	m_CountTags = CountTags; // С версии 2.1 добавим виртуальный тэг для размещения архива тревог.
	m_RecPerTag = RecPerTag;
//	std::basic_filebuf< char > *fbuf = file.rdbuf();
//	if(!fbuf)
//		return false;
	m_HeaderLen =	sizeof(dword) + //LTAHeaderFile::HeaderLen) +
					sizeof(char[12]) + //LTAHeaderFile::Signatura) +
					sizeof(char[short_name]) + //LTAHeaderFile::Plant) +
					sizeof(dword) + //LTAHeaderFile::RecPerTag) +
					sizeof(dword) + //LTAHeaderFile::LastRec) +
					sizeof(dword) + //LTAHeaderFile::Period) +
					sizeof(TTime) + //LTAHeaderFile::FirstTime) +
					sizeof(TTime) + //LTAHeaderFile::LastTime) +
					sizeof(dword) + //LTAHeaderFile::CountTags) +
					sizeof(dword) + //LTAHeaderFile::CountBlocks) +
					short_name * Blocks.size();

	file.write(reinterpret_cast<char*>(&m_HeaderLen), sizeof(m_HeaderLen));
	file.write(LTASignatura, sizeof(m_Signatura));
	strncpy(m_Signatura, LTASignatura, sizeof(m_Signatura));
	m_Plant = Plant;
	file.write(m_Plant.c_str(), m_Plant.size());
	char dummy = '\0';
	file.write(&dummy, short_name - m_Plant.size());
	file.write(reinterpret_cast<char*>(&m_RecPerTag), sizeof(m_RecPerTag));
	file.write(reinterpret_cast<char*>(&m_LastRec), sizeof(m_LastRec));
	file.write(reinterpret_cast<char*>(&m_Period), sizeof(m_Period));
	file.write(reinterpret_cast<char*>(&m_FirstTime), sizeof(m_FirstTime));
	file.write(reinterpret_cast<char*>(&m_LastTime), sizeof(m_LastTime));
	file.write(reinterpret_cast<char*>(&m_CountTags), sizeof(m_CountTags));
	dword	CountBlocks = Blocks.size();
	file.write(reinterpret_cast<char*>(&CountBlocks), sizeof(CountBlocks));

//	for(dword i = 0; i < CountBlocks; ++i)
//	{
//		char block[short_name] = "";
//		file.write(reinterpret_cast<char*>(block), sizeof(block));
//		m_Blocks.push_back(std::string(block));
//	}
	// Записываем имена блоков в файл и заносим в вектор
	m_Blocks.clear();
	for(std::vector<t_block>::const_iterator it = Blocks.begin();
		it != Blocks.end(); ++it)
	{
		//char block[short_name] = "";
		file.write(it->name, short_name);
		m_Blocks.push_back(it->name);
	}
	file.seekp(m_HeaderLen);//, ios::beg);
	strcpy(m_Signatura, LTASignatura);
	m_State = Init;
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
	m_RecHeads.clear();
return true;	// 
}

namespace
{
class OffSetsOfHeadRec {
      private:
        dword result;    
      public:
        // constructor initializes the value to add
        OffSetsOfHeadRec (const dword& v) : result(v)
		{
        }
		dword GetOffSet(void)
		{
			return result;
		}
        // the function call for the element adds the value
        void operator() (const LTAHeadRec& elem)
		{
            result += elem.LenRecHeadFile;
        }
    };
}

//----------------------------------------------------------------------------------
/*
bool CLTArchive::AddLTAHeadRec(tag_info &tag)
{

	LTAHeadRec head;
	head.gid = tag.gid;
	head.TagName = tag.name;
	
	head.TagDesc	= tag.desc;
	head.block		= tag.block;
	if(head.block == 0)
		head.block = 1;
	head.type		= tag.type;
	if((head.type & C_Analog) == C_Analog)
	{
		head.SC_HI		= (float)tag.analog.SC_HI;
		head.SC_LO		= (float)tag.analog.SC_LO;
		head.MaxVal		= (float)tag.analog.SC_HI;
		head.MinVal		= (float)tag.analog.SC_LO;
		head.EU = tag.analog.eu;
	}
	else if((head.type & C_Digital) == C_Digital)
	{
		head.SC_HI		= 1;
		head.SC_LO		= 0;
		head.MaxVal		= (float)head.SC_HI;//tag.analog.SC_HI;
		head.MinVal		= (float)head.SC_LO;//tag.analog.SC_LO;
	}
	else if((head.type & C_Const) == C_Const)
	{
		head.SC_HI		= MAX_CONST-1;
		head.SC_LO		= 0;
		head.MaxVal		= (float)head.SC_HI;//tag.analog.SC_HI;
		head.MinVal		= (float)head.SC_LO;//tag.analog.SC_LO;
	}
	else if((head.type & C_DigitalCtl) == C_DigitalCtl)
	{
		
		head.gid		= pDB->at(tag.dctl.state).gid;
		head.SC_HI		= MAX_CONST-1;
		head.SC_LO		= 0;
		head.MaxVal		= (float)head.SC_HI;//tag.analog.SC_HI;
		head.MinVal		= (float)head.SC_LO;//tag.analog.SC_LO;
	}
	else
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}
	head.DataSize = 0;
	head.DataOffSet = 0;
	head.LenRecHeadFile = sizeof(head.LenRecHeadFile) +
						sizeof(head.gid) +
						sizeof(head.DataOffSet) +
						sizeof(head.DataSize) +
						sizeof(char) * short_name +//TagName[short_name]
						sizeof(char) * short_name +//EU[short_name]
						sizeof(head.type) +
						sizeof(head.block) +
						sizeof(head.SC_HI) +
						sizeof(head.SC_LO) +
						sizeof(head.MaxVal) +
						sizeof(head.MinVal) +
						sizeof(short) +//	TagDescLen 
						sizeof(char) * head.TagDesc.size();

	//подсчитываем смещение для этой записи (складываем все длины записей)
//	OffSetsOfHeadRec OffSet(m_HeaderLen); // начинаем отсчет  после заголовка файла
//	OffSet = for_each (m_RecHeads.begin(), m_RecHeads.end(),         // range
//                  OffSet);
//	head.HeadOffSet = OffSet.GetOffSet();
	if(m_RecHeads.empty())
	{
		head.HeadOffSet = m_HeaderLen;
	}
	else
	{
		LTARecArrayT::reverse_iterator lastRec = m_RecHeads.rbegin();
		head.HeadOffSet = lastRec->LenRecHeadFile + lastRec->HeadOffSet;
	}
	m_RecHeads.push_back(head);
#ifdef _DEBUG
	LTARecArrayT::reverse_iterator last_rec = m_RecHeads.rbegin();
	LTARecArrayT::reference last = *last_rec;
#endif
return true;	// 
}

//----------------------------------------------------------------------------------
bool CLTArchive::WriteLTAHeadRec(LTAHeadRec &lta_head_rec, fstream &file)
{
	if(!file.is_open())
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
	file.clear();
	file.seekp(lta_head_rec.HeadOffSet);
#ifdef _DEBUG 
	std::streampos offset = file.tellp();
#endif
	file.write(reinterpret_cast<char*>(&lta_head_rec.LenRecHeadFile), sizeof(lta_head_rec.LenRecHeadFile));
	file.write(reinterpret_cast<char*>(&lta_head_rec.gid), sizeof(lta_head_rec.gid));
	file.write(reinterpret_cast<char*>(&lta_head_rec.DataSize), sizeof(lta_head_rec.DataSize));
	file.write(reinterpret_cast<char*>(&lta_head_rec.DataOffSet), sizeof(lta_head_rec.DataOffSet));
	file.write(lta_head_rec.TagName.c_str(), lta_head_rec.TagName.size());
	char dummy = '\0';
	file.write(&dummy, short_name - lta_head_rec.TagName.size());
	file.write(lta_head_rec.EU.c_str(), lta_head_rec.EU.size());
	file.write(&dummy, short_name - lta_head_rec.EU.size());
	file.write(reinterpret_cast<char*>(&lta_head_rec.type), sizeof(lta_head_rec.type));
	file.write(reinterpret_cast<char*>(&lta_head_rec.block), sizeof(lta_head_rec.block));
	file.write(reinterpret_cast<char*>(&lta_head_rec.SC_HI), sizeof(lta_head_rec.SC_HI));
	file.write(reinterpret_cast<char*>(&lta_head_rec.SC_LO), sizeof(lta_head_rec.SC_LO));
	file.write(reinterpret_cast<char*>(&lta_head_rec.MaxVal), sizeof(lta_head_rec.MaxVal));
	file.write(reinterpret_cast<char*>(&lta_head_rec.MinVal), sizeof(lta_head_rec.MinVal));
	short		TagDescLen = (short)lta_head_rec.TagDesc.size();
	file.write(reinterpret_cast<char*>(&TagDescLen), sizeof(TagDescLen));
//	char		TagDesc[long_name];
//	lta_head_rec.TagDesc.copy(TagDesc, TagDescLen);
//	TagDesc[TagDescLen]= '\0';
//	file.write(TagDesc, TagDescLen);
	file.write(lta_head_rec.TagDesc.c_str(), TagDescLen);
//if(file.fail())
//	return false;
//	file.clear();

return true;	// 
}
*/
//----------------------------------------------------------------------------------
/*
bool CLTArchive::FlashLTAHeadRec(LTARecArrayT::reference ref)
{
	try
	{
		fstream file(m_Name.c_str(), ios::binary | ios::in | ios::out);
	if(!file.is_open() || ref.HeadOffSet == 0)
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
//	file.clear();
//	file.seekp(ref.HeadOffSet);
	return WriteLTAHeadRec(ref, file);
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}

	// 
return true;	// 
}



//----------------------------------------------------------------------------------
bool CLTArchive::FlashArrayLTAHeadRec(void)
{
	try
	{
		fstream file(m_Name.c_str(), ios::binary | ios::in | ios::out);
	if(!file.is_open())
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
	file.clear();
	file.seekp(m_HeaderLen);
	for(LTARecArrayT::iterator it = m_RecHeads.begin();
		it != m_RecHeads.end();
		++it)
		{
			if(!WriteLTAHeadRec(*it, file))
				return false;
		}
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
return true;	// 
}

*/

//bool VQTLess (VQT &elem1, VQT &elem2)
//    {
//	if(elem1.m_Quality.Quality == TQuality::Bad ||
//		elem2.m_Quality.Quality == TQuality::Bad)
//			return false;
//        return elem1.m_Value < elem2.m_Value;
//    }

// function object to process the max VQT value with ONLY GOOD quality
    class MaxMinVQTValue {
      private:
        TValue m_Max;     // number of elements
        TValue m_Min;     // number of elements
		long	m_NumGood;
      public:
        // constructor
        MaxMinVQTValue() : m_Max(0), m_Min(0), m_NumGood(0)
		{    }

        // function call
        // - process one more element of the sequence
        void operator() (VQT elem)
		{
            if(elem.m_Quality.Quality == TQuality::Good)
			{
				m_NumGood++;
			}
			if(elem.m_Value > m_Max)
				m_Max = elem.m_Value;   // set max value
			if(elem.m_Value < m_Min)
				m_Min = elem.m_Value;   // set min value
        }

        // return max value
        TValue GetMax()
		{
            return m_Max;
        }
        // return min value
        TValue GetMin()
		{
            return m_Min;
        }
        // return number good values
        long GetNumGood()
		{
            return m_NumGood;
        }

    };
//----------------------------------------------------------------------------------
bool CLTArchive::VQT2LTA(const vector<VQT> &source, vector<LTAItem> &taget, std::size_t index)
{
taget.clear();
if(index > m_RecHeads.size())
{
	SetLastError(ERROR_INVALID_PARAMETER);
	return false;
}
try
{
LTARecArrayT::reference head = m_RecHeads.at(index);
MaxMinVQTValue statVQT;
statVQT = for_each (source.begin(), source.end(),         // range
                  statVQT);    // operation
if(fabs((double)statVQT.GetMax()- (double)statVQT.GetMin()) >
	fabs((double)head.SC_HI - (double)head.SC_LO)/RangeVal)
{
	head.MaxVal = (float)statVQT.GetMax();
	head.MinVal = (float)statVQT.GetMin();
}
/// Если мин или максимум вылез за шкалу, то значит шкала была изменена, заменяем её
if((head.SC_HI > head.SC_LO && // Шкала нормальная
   head.MaxVal > head.SC_HI) ||
   (head.SC_HI < head.SC_LO && // Шкала обратная
   head.MaxVal < head.SC_HI))
{
	head.SC_HI = head.MaxVal;
}
if((head.SC_HI > head.SC_LO &&  // Шкала нормальная
   head.MinVal < head.SC_LO) ||
   (head.SC_HI < head.SC_LO &&	// Шкала обратная
   head.MinVal > head.SC_LO))
{
	head.SC_LO = head.MinVal;
}

for(vector<VQT>::const_iterator it = source.begin();
	it != source.end();
	++it)
	{
		LTAItem item;
		if((head.type & C_Analog) == C_Analog)
		{
			double div = fabs((double)it->m_Value - (double)head.MinVal)/fabs((double)head.MaxVal - (double)head.MinVal);
			item.value = (unsigned short)(div * double(RangeVal) + 0.5);
		}
		else if((head.type & C_Digital) == C_Digital ||
			(head.type & C_Const) == C_Const)
		{
			item.value = (unsigned short)it->m_Value;
		}
		else
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}

		TQuality quality(it->m_Quality);
		QualityToLTAItem(quality, item);
		taget.push_back(item);
	}
}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
return true;	// 
}

// function object to process the max HDAItem value with ONLY GOOD quality
    class MaxMinHDAValue {
      private:
        TValue m_Max;     
        TValue m_Min;     
		long	m_NumGood;// number of elements
		bool	m_IsInit;
      public:
        // constructor
        MaxMinHDAValue() : m_Max(0), m_Min(0), m_NumGood(0), m_IsInit(false)
		{    }

        // function call
        // - process one more element of the sequence
        void operator() (HDAItem elem)
		{
            if(elem.State.Quality == TQuality::Good)
			{
				m_NumGood++;
			}
			if(!m_IsInit)
			{
				m_Max = elem.Value;   // set max value
				m_Min = elem.Value;   // set min value
				m_IsInit = true;
			}
			else
			{
				if(elem.Value > m_Max)
					m_Max = elem.Value;   // set max value
				if(elem.Value < m_Min)
					m_Min = elem.Value;   // set min value
			}
        }

        // return max value
        TValue GetMax()
		{
            return m_Max;
        }
        // return min value
        TValue GetMin()
		{
            return m_Min;
        }
        // return number good values
        long GetNumGood()
		{
            return m_NumGood;
        }

    };

//----------------------------------------------------------------------------------
bool CLTArchive::HDA2LTA(const vector<HDAItem> &source, vector<LTAItem> &taget, std::size_t index)
{
	taget.clear();
	if(index > m_RecHeads.size())
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}
	try
	{
		LTARecArrayT::reference head = m_RecHeads.at(index);
		if((head.type & C_Analog) == C_Analog)
		{
			MaxMinHDAValue stat = 	for_each (source.begin(), source.end(),  // range
									MaxMinHDAValue());    // operation
			//Если разница больше максимального кванта, то пишем Min и Max
			if(fabs((double)stat.GetMax() - (double)stat.GetMin()) >
				fabs((double)head.SC_HI - (double)head.SC_LO)/RangeVal)
			{
				head.MaxVal = (float)stat.GetMax();
				head.MinVal = (float)stat.GetMin();
			}
			/// Если мин или максимум вылез за шкалу, то значит шкала была изменена, заменяем её
			if(head.SC_HI > head.SC_LO && head.MaxVal > head.SC_HI)
				head.SC_HI = head.MaxVal;
			else if(head.SC_HI < head.SC_LO && head.MaxVal < head.SC_LO)
				head.SC_LO = head.MaxVal;
				
			if(head.SC_HI > head.SC_LO && head.MinVal < head.SC_LO)
				head.SC_LO = head.MinVal;
			else if(head.SC_HI < head.SC_LO && head.MinVal > head.SC_HI)
				head.SC_LO = head.MinVal;
			//FlashLTAHeadRec(head);
			AnalogHDA2LTA(source, taget, head.MaxVal, head.MinVal);
		}
		else if((head.type & C_Digital) == C_Digital ||
				(head.type & C_Const) == C_Const)
		{
			DigHDA2LTA(source, taget);
		}
		else
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}
		
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
	return true;	// 
}

//----------------------------------------------------------------------------------
bool CLTArchive::LTA2VQT(const vector<LTAItem> &source, deque<VQT> &taget, std::size_t index)
{
	taget.clear();
	if(index > m_RecHeads.size())
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}
	try
	{

		LTARecArrayT::const_reference head = m_RecHeads.at(index);
		if((head.type & C_Analog) == C_Analog)
		{
			AnalogLTA2VQT(source, taget, head.MaxVal, head.MinVal);
		}
		else if((head.type & C_Digital) == C_Digital ||
			(head.type & C_Const) == C_Const)
		{
			DigLTA2VQT(source, taget);
		}
		else
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return false;
		}
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
	return true;	// 
}
bool CLTArchive::AnalogLTA2VQT(const vector<LTAItem> &source,deque<VQT> &taget,const TValue	&SC_HI,const TValue	&SC_LO)
{
	long i = 0;
//	if(m_FirstTime != 0)
	TStamp BeginDay(m_FirstTime);
	SYSTEMTIME st = BeginDay;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;
	BeginDay = st;
	taget.resize(source.size());
	deque<VQT>::iterator itTaget = taget.begin();
for(vector<LTAItem>::const_iterator it = source.begin();
	it != source.end();
	++itTaget, ++it, i++)
	{
//		TVQT item;
		itTaget->m_Value = (TValue)(((double)(it->value) * (((double)SC_HI - (double)SC_LO)/(double)RangeVal)) +  (double)SC_LO);
		itTaget->m_wQuality = QualityFromLTAItem(*it);
		itTaget->m_Time = BeginDay + ((TTime)(m_Period * i) * _SECOND);
//		taget.push_back(item);
	}
	return true;
}

bool CLTArchive::DigLTA2VQT(const vector<LTAItem> &source, deque<VQT> &taget)
{
	long i = 0;
	TStamp BeginDay(m_FirstTime);
	SYSTEMTIME st = BeginDay;
	st.wHour = 0;
	st.wMinute = 0;
	st.wSecond = 0;
	st.wMilliseconds = 0;
	BeginDay = st;
	taget.resize(source.size());
	deque<VQT>::iterator itTaget = taget.begin();
for(vector<LTAItem>::const_iterator it = source.begin();
	it != source.end();
	++itTaget, ++it, i++)
	{
//		TVQT item;
		itTaget->m_Value = (TValue)it->value;
		itTaget->m_wQuality = QualityFromLTAItem(*it);
		itTaget->m_Time = BeginDay + ((TTime)(m_Period * i) * _SECOND);
//		taget.push_back(item);
	}
	return true;
}


//----------------------------------------------------------------------------------
bool CLTArchive::AnalogHDA2LTA(const vector<HDAItem> &source,vector<LTAItem> &taget, const TValue	&SC_HI,const TValue	&SC_LO)
{
//	LTAItem item;
//	TValue val_to_archiv = 0.0;
	taget.resize(source.size());
	vector<LTAItem>::iterator itTaget = taget.begin();
for(vector<HDAItem>::const_iterator it = source.begin();
	it != source.end();
	++itTaget,	++it)
	{
		TValue val_to_archiv = it->Value;
		
		double div = fabs((double)val_to_archiv - (double)SC_LO)/fabs((double)SC_HI - (double)SC_LO);
		itTaget->value = (unsigned short)(div * double(RangeVal) + 0.5);
		TQuality quality(it->State);
		QualityToLTAItem(quality, (*itTaget));
	}
	
return true;	// 
}

//----------------------------------------------------------------------------------
bool CLTArchive::DigHDA2LTA(const vector<HDAItem> &source,vector<LTAItem> &taget)
{
	taget.resize(source.size());
	vector<LTAItem>::iterator itTaget = taget.begin();
for(vector<HDAItem>::const_iterator it = source.begin();
	it != source.end();
	++itTaget, ++it)
	{
		itTaget->value = (unsigned short)it->Value;
		TQuality quality(it->State);
		QualityToLTAItem(quality, (*itTaget));
	}
	
return true;	// 
}

//----------------------------------------------------------------------------------
/*
bool CLTArchive::AddDataForTag(const vector< HDAItem > &data, const std::size_t tag_index)
{
	try
	{
		fstream file(m_Name.c_str(), ios::binary | ios::in | ios::out | ios::ate);
	if(!file.is_open())
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
	LTARecArrayT::reference head = m_RecHeads.at(tag_index);
	vector<LTAItem> item_data;
	if(!HDA2LTA(data, item_data, tag_index))
		return false;
	vector<unsigned char>	dest;
	dest.resize((long)(item_data.size() * sizeof(LTAItem) * 1.002f) + 12);
	uLongf len = dest.size();
	int ret = compress OF((&dest[0],   &len, (const Bytef *)(&item_data[0]), item_data.size()* sizeof(LTAItem)));

	if(ret != Z_OK )
		return false;
	head.DataOffSet = (dword)file.tellp();
	head.DataSize	= len;
	file.clear();
	file.write((char*)(&dest[0]), len);
	file.close();
	file.clear();
	FlashLTAHeadRec(head);
	if(tag_index == m_CountTags -1)
		m_State = Valid;
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
return true;	// 

}
*/
bool CLTArchive::GetDataByIndex(const dword index, deque< VQT > &array)
{
	try
	{
		ifstream file(m_Name.c_str(), ios::binary);
	if(!file.is_open() || m_State != Valid)
	{
		SetLastError(ERROR_CANNOT_MAKE);
		return false;
	}
	LTARecArrayT::reference head = m_RecHeads.at(index);
	vector<LTAItem> item_data;
	vector<unsigned char>	buf;
	//dword num_items = (dword)(_DAY/(m_Period * _SECOND));
	item_data.resize(m_RecPerTag);
	buf.resize(head.DataSize); // resizing to size of compressed data
	file.seekg(head.DataOffSet);
	file.read((char*)&buf[0], head.DataSize);
	uLongf destLen = item_data.size() * sizeof(LTAItem);
	int ret = uncompress OF(((Bytef *)&item_data[0],   &destLen, (const Bytef *)&buf[0], head.DataSize));
	if(ret != Z_OK)
		return false;
	file.close();
	file.clear();
	if(!LTA2VQT(item_data, array, index))
		return false;
	}
	catch(bad_alloc)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return false;
	}
	catch(...)
	{
		SetLastError(E_FAIL);
		return false;
	}
return true;	// 
}

struct IsGidInHead: public unary_function<LTAHeadRec&, bool>
	{
		IsGidInHead(const TGID gid): m_gid(gid)
			{}

        bool operator()(LTAHeadRec& RecHead) const
		{
			return m_gid == RecHead.gid;
		}
		TGID m_gid;
	};

long CLTArchive::GetIndex(TGID gid)
{
	LTARecArrayT::iterator result = find_if(m_RecHeads.begin(), m_RecHeads.end(), IsGidInHead(gid));// 
	if(result != m_RecHeads.end())
		return result - m_RecHeads.begin();
	return -1;
}

bool CLTArchive::GetRecHeads(LTARecArrayT &RecHeads)
{
	RecHeads.clear();
	copy(m_RecHeads.begin(), m_RecHeads.end(), back_inserter(RecHeads));
return true;	// 
}
