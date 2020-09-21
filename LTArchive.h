// LTArchive.h: interface for the CLTArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LTARCHIVE_H__1674A048_7D90_401F_B1CF_BD14640AB507__INCLUDED_)
#define AFX_LTARCHIVE_H__1674A048_7D90_401F_B1CF_BD14640AB507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <deque>
#include <string>
#include <iostream>
//#include "../commondb.h"
#include "AbstractArchive.h"
//#include "../CVQT.H"
using namespace std;

#pragma pack(push,1)
#pragma warning( push, 1)
#pragma warning(disable : 4200 )
struct LTAHeaderFile
{
	dword   HeaderLen;
	char    Signatura[12];
	char	Plant[short_name];
    dword   RecPerTag;
	dword	LastRec;
    dword   Period;	// scantime in sec.
	TTime	FirstTime;
	TTime	LastTime;
    dword	CountTags;
	dword	CountBlocks;
	char	Blocks[][short_name];
};

struct LTARecHeadFile
{
	dword		LenRecHead;
	TGID		gid;
	dword	DataSize;
	dword	DataOffSet;
	char		TagName[short_name];
	char		EU[short_name];
unsigned short	type;
	/// Код группы/Блок установки
    short		block;          
	/// Верхняя граница шкалы
	float		SC_HI;
	/// Нижняя граница шкалы
	float		SC_LO;
	/// Минимальное значение параметра GOOD за сутки
	float		MaxVal;
	/// Максимальное значение параметра GOOD за сутки
	float		MinVal;
	short		TagDescLen;
	char		TagDesc[];
};
struct LTAHeadRec
{
	dword		LenRecHeadFile; // значение длины структуры в файле LTARecHeadFile
	dword		HeadOffSet;
	TGID		gid;
	dword	DataSize;
	dword	DataOffSet;
	string		TagName;
	string		EU;
unsigned short	type;
	/// Код группы/Блок установки
    short		block;          
	/// Верхняя граница шкалы
	float		SC_HI;
	/// Нижняя граница шкалы
	float		SC_LO;
	/// Минимальное значение параметра GOOD за сутки
	float		MaxVal;
	/// Максимальное значение параметра GOOD за сутки
	float		MinVal;
	string		TagDesc;
};

enum    LIMIT_CODE
{
	BAD_VAL = 0,
	HI_BLOCK=1,
	HI_SIG=2,
	HI_REG=3,
	NO_LIMIT = 4,
	LO_BLOCK=5,
	LO_SIG=6,
	LO_REG=7
};
enum    QUALITY_CODE
{
	UNCERTAIN = 0,
	GOOD = 1
};

//typedef union S_LTAItem
//	{
//struct 
//{
//	unsigned value:12;
//	unsigned limit:3;
//	unsigned quality:1;
//};
//unsigned short data;
//}LTAItem;

class LTAItem
{
public:
	union
	{
		struct 
		{
			unsigned value:12;
			unsigned limit:3;
			unsigned quality:1;
		};
		unsigned short data;
	};
};


#pragma pack(pop)

class CLTArchive
{
public:
	typedef enum tagState{NotInit, Init, Valid, Error} E_State;
	typedef std::vector<LTAHeadRec> LTARecArrayT;
static const short RangeVal;

	CLTArchive();
	CLTArchive(LPCTSTR Name);
	virtual ~CLTArchive();
	bool Open(LPCTSTR Name);
	bool Create(LPCTSTR Name,
				LPCTSTR Plant,
				const dword RecPerTag,
				const dword Period,
				const TTime FirstTime,
				const TTime LastTime,
				const dword CountTags,
				std::vector<t_block> &Blocks);
//		const dword RecPerTag, const dword Period, const dword *pIDs);
//	bool CreateFromHisFile(LPCTSTR Name); 
	long GetIndex(TGID gid);
//	bool GetDataByGid(const TGID gid, deque<VQT> &array);
	bool GetDataByIndex(const dword index, deque< VQT > &array);
	dword GetCountTag(){ return m_CountTags;}
	dword GetRecPerTag(){ return m_RecPerTag;}
	dword GetLastRec(){ return m_LastRec;}
	dword SetLastRec(dword lastrec){ m_LastRec = lastrec;
									return m_LastRec;}
	dword GetPeriod(){ return m_Period;}
	TTime GetFirstTime(){ return m_FirstTime;}
	TTime GetLastTime(){ return m_LastTime;}
	bool GetRecHeads(LTARecArrayT &RecHeads);
	bool GetBlocks(std::vector<std::string> &Blocks);
    bool IsValid(){ return m_State == Valid;}
	std::ostream& Print(std::ostream& os, const bool IsOnlyHeaders = true);

//	bool AddDataForTag(const vector< HDAItem > &data, const std::size_t tag_index);
//	bool AddLTAHeadRec(tag_info &tag);
//	bool FlashArrayLTAHeadRec(void);
//	bool FlashLTAHeadRec(LTARecArrayT::reference ref);

private:
std::ostream&	PrintHeader(std::ostream& os);
std::ostream&	PrintTagHead(std::ostream& os, const long tagindex);
std::ostream&	PrintTagValues(std::ostream& os, const long tagindex);

//	unsigned short QualityFromLTAItem( const LTAItem &item);
	bool ReadLTAHeadRec(LTAHeadRec *lta_head_rec, ifstream &file, const long offset_rec);
//	bool WriteLTAHeadRec(LTAHeadRec &lta_head_rec, fstream &file);
	bool HDA2LTA(const vector<HDAItem> &source,vector<LTAItem> &taget,std::size_t index);
	bool VQT2LTA(const vector<VQT> &source, vector<LTAItem> &taget, std::size_t index);
	bool LTA2VQT(const vector<LTAItem> &source, deque<VQT> &taget, std::size_t index);
	bool AnalogLTA2VQT(const vector<LTAItem> &source, deque<VQT> &taget, const TValue	&SC_HI, const TValue	&SC_LO);
	bool DigLTA2VQT(const vector<LTAItem> &source, deque<VQT> &taget);
	bool AnalogHDA2LTA(const vector<HDAItem> &source, vector<LTAItem> &taget, const TValue	&SC_HI, const TValue	&SC_LO);
	bool DigHDA2LTA(const vector<HDAItem> &source, vector<LTAItem> &taget);
	bool AnalogVQT2LTA(const vector<VQT> &source, vector<LTAItem> &taget, const TValue	&SC_HI, const TValue	&SC_LO);
	bool DigVQT2LTA(const vector<LTAItem> &source, vector<VQT> &taget);
private:
	E_State	m_State;
	dword   m_HeaderLen;
	char    m_Signatura[12];
	std::string	m_Plant;
    dword   m_RecPerTag;
	dword	m_LastRec;
    dword   m_Period;
	TTime	m_FirstTime;
	TTime	m_LastTime;
	dword	m_CountTags;
	std::vector<std::string> m_Blocks;
	LTARecArrayT m_RecHeads;
//	LTAHeadRec*	m_Tags;
	string  m_Name;
};

#pragma warning( pop )

#endif // !defined(AFX_LTARCHIVE_H__1674A048_7D90_401F_B1CF_BD14640AB507__INCLUDED_)
