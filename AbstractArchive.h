// AbstractArchive.h: interface for the CAbstractArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABSTRACTARCHIVE_H__C0681683_3D5F_11D6_BDB4_0050DACA892E__INCLUDED_)
#define AFX_ABSTRACTARCHIVE_H__C0681683_3D5F_11D6_BDB4_0050DACA892E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>

#include "commondb.h"
#include "CVQT.H"
using namespace std;


#pragma pack( push, 1)
#pragma warning( push, 4)
#pragma warning(disable : 4200 )

#define SMALL_TAGS 100
#define MEDIUM_TAGS 300

typedef struct S_HDAItem
{
	TValue				Value;
	union
	{
		FieldsQuality   State;
		WORD			wState;
	};
} HDAItem;

typedef struct S_IDvqt
{
	long				Index;
	TGID				Gid;
	TVQT				Vqt;
} IDVQT;

const char signatura[] = "Vinca2.1";
typedef enum E_Kind {Custom = 0, Hour, Day, Week, Month} HDAKind;

typedef struct T_TATItem
{
	TGID	TagID;
	long	Index; // индекс в таблице tag allocation table
}TATItem;
//#pragma warning( disable : 4200 )  // Disable warning zero-sized array
typedef struct T_HeaderArchive
{
	dword   HeaderLen;
	char    Signatura[10];
	word	Kind;
    dword	CountTag;
    dword   CountRec;
	dword	LastRec;
    dword   Period;
	TTime	FirstTime;
	TTime	LastTime;
    dword	Capacity;
	TATItem TAT[1];
} FileHeader;
//----------------------------------------------------------------------------------------------------
// returns true if item.TagID is an  number
struct IsGID : public unary_function< TATItem &, bool >
{
    IsGID(const TGID gid) :m_gid(gid)
	{
	}
	bool operator ==(TATItem &item)
	{
		return (item.TagID == m_gid && item.Index != -1);
	}

	TGID m_gid;
};

#ifdef VINCAHIS_API
class VINCAHIS_API CAbstractArchive  
#else
class CAbstractArchive  
#endif // VINCAHIS_API
{
public:
        CAbstractArchive()
        {}
        virtual ~CAbstractArchive()
        {}

        virtual string GetName(void)=0;
//      virtual bool SetPeriod(dword time_period)=0;
        virtual bool GetArrayID(vector<TGID> &pArray)=0;
        virtual FileHeader* GetHeader(void)=0;

        virtual bool Reset()=0;

        virtual bool Open(LPCTSTR name, bool ROnly = true)=0;

        virtual bool Create(LPCTSTR name, const dword count_tags,
                const dword count_rec, const dword period,
				const word Kind,
                const TGID*  ArrayIDs)=0;


};


#pragma pack( pop )
#pragma warning( pop )
#pragma warning(default : 4200 )

#endif // !defined(AFX_ABSTRACTARCHIVE_H__C0681683_3D5F_11D6_BDB4_0050DACA892E__INCLUDED_)
