#ifndef LTAREADERLIB_H
#define LTAREADERLIB_H

#ifdef LTAREADERLIB_EXPORTS
#define LTAREADERLIB_API __declspec(dllexport)
#else
#define LTAREADERLIB_API __declspec(dllimport) 
#endif

#include <windows.h>
#include <vector>

using namespace std;


/*
 *  Заголовок шапки записи на экране формы
 */
struct T_LTAHeadRecDispl
{
    long		gid;
    char		TagName[32];
    char		EU[32];
    char		type_str[32];
    char		type_IO[16];
    /// Код группы/Блок установки
    short		block;
    /// Верхняя граница шкалы
    float		SC_HI;
    /// Нижняя граница шкалы
    float		SC_LO;
    /// Минимальное значение параметра GOOD за сутки
    double		MaxVal;
    /// Максимальное значение параметра GOOD за сутки
    double		MinVal;
    char		TagDesc[256];
};


struct T_ItemVal
{
    double                  val;
    unsigned long           ev;
    unsigned long long      time;
};

class LTAREADERLIB_API CLTAReaderLib
{
public:

	//static CLTAReaderLib * CreateReaderInst();
	CLTAReaderLib();

	virtual ~CLTAReaderLib();

  	// Открыть архив
    virtual bool Open(const char *name_file) = 0;

	// Получить время начало
    virtual LONGLONG GetFirstTime() = 0;

	// Получить период
    virtual DWORD GetPeriod() = 0;

	// Получить массив (шапку) заголовков
    virtual bool GetvRecHeadDisp(vector<T_LTAHeadRecDispl> &vRecHeadDispl) = 0;

	// Получить массив (шапку) заголовков c Unicod
	virtual bool GetvRecHeadDisp_utf8(vector<T_LTAHeadRecDispl> &vRecHeadDispl) = 0;

	// Получить данные по индексу
    virtual bool GetDataByIndex(const int index, vector< T_ItemVal > &array) = 0;

};


extern "C"
{

	LTAREADERLIB_API CLTAReaderLib * CreateReaderInst();

	LTAREADERLIB_API void GetStatusAsStr_utf8(unsigned short status, char text[1024]);

	LTAREADERLIB_API void GetStatusAsStr(unsigned short status, char text[1024]);
}

#endif // LTAREADERLIB_H
