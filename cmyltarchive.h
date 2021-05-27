#ifndef CMYLTARCHIVE_H
#define CMYLTARCHIVE_H
#include <QString>
#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>
#include "LTArchive.h"
#include <QTextCodec>
#include <QTime>

static QTextCodec *codec = QTextCodec::codecForName("Windows-1251") ;
#define RUS(str) codec->toUnicode(str)

/*
 *  Заголовок шапки записи на экране формы
 */
struct T_LTAHeadRecDispl
{
    TGID		gid;
    QString		TagName;
    QString		EU;
    QString     type_str;
    QString     type_IO;
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
    QString		TagDesc;
};

struct CMyLTAHeadRec
{
    LTAHeadRec * ltaHeadRec;

    CMyLTAHeadRec():ltaHeadRec(nullptr){}

    CMyLTAHeadRec(LTAHeadRec * _ltaHeadRec):ltaHeadRec(_ltaHeadRec){}

    const char *GetNameType_IO()
    {
        switch(ltaHeadRec->type & (~C_Output))
        {
        case C_Analog:
           return ltaHeadRec->type & C_Output ? "AO":"AI";
        case C_Digital:
           return ltaHeadRec->type & C_Output ?  "DO":  "DI";
        case C_Const:
           return "CNS";
        case C_AnalogCtl:
            return "ACTL";
        case C_DigitalCtl:
            return "DCTL";
        }
    }

    const char *GetNameType()
    {
        switch(ltaHeadRec->type )
        {
        case C_Analog:
           return "Аналоговый";
        case C_Digital:
           return "Дискретный";
        case C_Const:
           return "Константный";
        case C_AnalogCtl:
            return "Аналоговый управление";
        case C_DigitalCtl:
            return "Дискретный управление";
        }
    }

    void ConvertTo(T_LTAHeadRecDispl *disp)
    {
        disp->gid = ltaHeadRec->gid;
        disp->TagName = RUS(ltaHeadRec->TagName.c_str());
        disp->TagDesc = RUS(ltaHeadRec->TagDesc.c_str());
        disp->EU = RUS(ltaHeadRec->EU.c_str());
        disp->block = ltaHeadRec->block;
        disp->SC_HI = ltaHeadRec->SC_HI;
        disp->SC_LO = ltaHeadRec->SC_LO;
        disp->MaxVal = ltaHeadRec->MaxVal;
        disp->MinVal = ltaHeadRec->MinVal;
        disp->type_str = GetNameType();
        disp->type_IO = GetNameType_IO();
    }
};

/*
 * CModelLTArchive класс для списка тегов для трендов
 */
class CModelLTArchive: public QAbstractTableModel
{
    Q_OBJECT
private:    

     CLTArchive m_LTArchive;
     QVector<T_LTAHeadRecDispl> vRecHeadDispl;

public:

    CModelLTArchive();
    ~CModelLTArchive();

    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex& index, int nRole) const;
    bool setData(const QModelIndex& index,  const QVariant& value,  int    nRole    ) ;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int nsection,    Qt::Orientation orientation,    int nRole = Qt::DisplayRole    ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRows (int nRow,    int nCount,    const QModelIndex& parent = QModelIndex()    ) ;
    bool removeRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()   ) ;

    T_LTAHeadRecDispl *getItem(int index);
    bool GetDataByIndex(const dword index, deque< VQT > &array);
    bool OpenFile(QString &fileName);
    QDateTime GetFirstTime();

};

#define min_step 10             // минимальный шаг в данных

struct THeaderParamLtaData
{
    QTime           t0;         // начальный отсчет
    int             period;     // интервал времени (рассматриваемый период) сек.
    int             step;       // шаг (расстояние между точками) сек.
    vector<QString> vDataStr;

    THeaderParamLtaData():period(0), step(0)
    {
    }

    void SetParam(const QTime &_t0, int period_min, int dist_s )
    {
        t0 = _t0 ;
        step =  dist_s ;
        period = period_min*60;
        SetVectorTime( "hh:mm:ss" );
    }

    void SetVectorTime(const QString &format_data )
    {
        QTime t(0,0,0) ;
        t = t0;
     //   t = t.addSecs( static_cast<int>( t0 ) );
        vDataStr.clear();
        for(int i = 0; i < period; i += step){
           vDataStr.push_back(t.toString(format_data));
           t = t.addSecs(step);
        }
    }

    const QString GetTimeByIndex(unsigned int index) const
    {
        if( index < vDataStr.size() ) return vDataStr[index];
        return "-";
    }

};


struct T_LTADataRecDispl
{
    TGID		gid;
    QString		TagName;
    QString		TagDesc;
    /// Верхняя граница шкалы
    float		SC_HI;
    /// Нижняя граница шкалы
    float		SC_LO;
    /// Вектор значений
    vector<TValue> vVal;

    void addData(deque<VQT> &arr, THeaderParamLtaData *par)
    {
        vVal.clear();
        unsigned step = static_cast<unsigned>(par->step / min_step );
        unsigned count = static_cast<unsigned>(par->period / min_step  );
        count = count < arr.size() ? count : arr.size();
        unsigned start = 0; // par->t0
        for(unsigned i = start ; i < count; i += step)
            vVal.push_back(arr.at(i).m_Value);
    }

    void addData(deque<VQT> &arr)
    {
        vVal.clear();
        deque<VQT>::iterator it;
        for(it = arr.begin(); it!= arr.end(); ++it) vVal.push_back(it->m_Value);
    }

    const QString GetGidStr() const
    {
        return QString::number(gid);
    }

    const QString GeSC_HIdStr() const
    {
        return QString::number(static_cast<double>(SC_HI));
    }

    const QString GeSC_LOdStr() const
    {
        return QString::number(static_cast<double>(SC_LO));
    }

    const QString GetValByIndex(unsigned int index) const
    {
        if( index >= vVal.size() ) return "-";
        return QString::number(vVal[index]);
    }

};


/*
 * CModelLTADatarchive класс для таблицы отчета
 */
class CModelLTADatarchive: public QAbstractTableModel
{
    Q_OBJECT
private:

    QVector<T_LTADataRecDispl>  vLTAdata;
    THeaderParamLtaData         headerParam;

public:
    CModelLTADatarchive();

    ~CModelLTADatarchive();

    THeaderParamLtaData *getHeaderParam();

    void addData(T_LTADataRecDispl &data);

    void clearData();

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex& index, int nRole) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant headerData(int nsection,    Qt::Orientation orientation,    int nRole = Qt::DisplayRole    ) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // CMYLTARCHIVE_H
