#ifndef CMYLTARCHIVE_H
#define CMYLTARCHIVE_H
#include <QString>
#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>
#include "LTArchive.h"

#include <QTextCodec>
//#include <

static QTextCodec *codec = QTextCodec::codecForName("Windows-1251") ;
#define RUS(str) codec->toUnicode(str)


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

    T_LTAHeadRecDispl *getItem(uint index);
    bool GetDataByIndex(const dword index, deque< VQT > &array);
    bool OpenFile(QString &fileName);

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
    vector<TValue> vaues;

    const QString GetGidStr() const {
        return QString::number(gid);
    }

    const QString GeSC_HIdStr() const {
        return QString::number(static_cast<double>(SC_HI));
    }

    const QString GeSC_LOdStr() const {
        return QString::number(static_cast<double>(SC_LO));
    }

    const QString GetValByIndex(unsigned int index) const {
        if( index >= vaues.size() ) return "-";
        return QString::number(vaues[index]);
    }

};

struct TParamLtaData
{
    unsigned int t0;
    unsigned int count; // period
    unsigned int step;  // dist
    TParamLtaData():t0(0),count(60), step(1)
    {
    }

    void SetParam(int _t0, int period_min, int dist_s )
    {
       // t0 = _t0 - 3; // ????
        step = (unsigned int)dist_s/10;
        count = (unsigned int)period_min*60/dist_s;
    }
};

/*
 * CModelLTADatarchive класс для таблицы отчета
 */
class CModelLTADatarchive: public QAbstractTableModel
{
    Q_OBJECT
private:

    QVector<T_LTADataRecDispl> vLTAdata;

public:
    CModelLTADatarchive();
    ~CModelLTADatarchive();

    void addData(T_LTADataRecDispl &data, deque<VQT> *arr = nullptr, TParamLtaData *par = nullptr);
    void clearData();
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex& index, int nRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int nsection,    Qt::Orientation orientation,    int nRole = Qt::DisplayRole    ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // CMYLTARCHIVE_H