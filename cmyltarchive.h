#ifndef CMYLTARCHIVE_H
#define CMYLTARCHIVE_H
#include <QString>
#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>

#include "ltareaderlib.h"
#include <QTextCodec>
#include <QTime>
#include <QProgressDialog>
#include <QLibrary>

//static QTextCodec *codec = QTextCodec::codecForName("Windows-1251") ;
//#define RUS(str) codec->toUnicode(str)

//void Load_library_lta(const QString &str);


typedef CLTAReaderLib*( *T_CreateReaderInst )( const char * );
typedef void ( *T_GetStatus )( unsigned int status, char text[1024] );


class CLoaderLibrary
{
public:

    CLoaderLibrary();

    ~CLoaderLibrary();

    void Load_library_lta(const QString &str);

    CLTAReaderLib *CreateReaderInst(const char *param);

    void GetStatusAsStr_utf8(unsigned int status, char text[1024]);

 //   const QLibrary &GetLibrary() const;

private:

    T_CreateReaderInst  p_CreateReaderInst = nullptr;

    T_GetStatus         p_GetStatus = nullptr;

    QLibrary            libr;
};


struct TBeginParam  // начальные параметры
{
     QDateTime  t0;             // начальный отсчет

     int        min_step;       // минимальный шаг записи (сек)

     TBeginParam():min_step(10)
     {
     }
};

/*
 * CModelLTArchive класс для списка тегов для трендов
 */
class CModelLTArchive: public QAbstractTableModel
{
    Q_OBJECT

public:

    CModelLTArchive( vector<T_LTAHeadRecDispl> &_vRecHeadDispl);

    ~CModelLTArchive() override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex& index, int nRole) const override;

 //   bool setData(const QModelIndex& index,  const QVariant& value,  int    nRole    ) override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant headerData(int nsection,    Qt::Orientation orientation,    int nRole = Qt::DisplayRole    ) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

 //   bool insertRows (int nRow,    int nCount,    const QModelIndex& parent = QModelIndex()    ) override;

//    bool removeRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()   ) override;

 //   T_LTAHeadRecDispl *getItem(int index);

private:

    vector<T_LTAHeadRecDispl> &vRecHeadDispl;

};


struct T_ValParamLtaData
{
    unsigned step ;

    unsigned count ;

    unsigned start ;
};

struct THeaderParamLtaData
{
    QTime               t0;         // начальный отсчет

    int                 period;     // интервал времени (рассматриваемый период) сек.

    int                 step;       // шаг (расстояние между точками) сек.

    T_ValParamLtaData   valParam;   // расчетные параметры для значений

    vector<QString> vDataStr;


    THeaderParamLtaData():period(0), step(0)
    {
    }

    void SetParam(const QTime &_t0, int period_min, int dist_s,  const TBeginParam  &par0 )
    {
        t0 = _t0 ;
        step =  dist_s ;
        period = period_min*60;
        SetVectorTime( "hh:mm:ss" );
        valParam.step = static_cast<unsigned>(step / par0.min_step );
        valParam.count = static_cast<unsigned>(period / par0.min_step  );
        valParam.start = static_cast<unsigned>(par0.t0.time().msecsTo( t0 ) / (par0.min_step * 1000) );
    }

    void SetVectorTime(const QString &format_data )
    {
        QTime t(0,0,0) ;
        t = t0;
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

     QString GetStrCSV()
     {
         QString res;
         foreach (QString item, vDataStr) {
             res.append(QString(";%1").arg(item, 10));
         }
         return res;
     }

};

struct T_LTADataRecDispl
{
    T_LTAHeadRecDispl   *header;
    vector<T_ItemVal>   vVal;

    T_LTADataRecDispl(T_LTAHeadRecDispl *_header):header(_header)
    {
    }

    static QString GetHeadreStrCSV()
    {
        return QString("%1;%2;%3;%4;%5").arg("GID", 9).arg("Тех. параметр", 33).arg("Описание", 33).arg("Вверх шкалы", 12).arg("Низ шкалы", 12);
    }


    QString GetStrCSV(T_ValParamLtaData &param)
    {
        QString res = QString("%1;%2;%3;%4;%5").arg(header->gid, 9).arg(header->TagName, 33)
                .arg(header->TagDesc, 33).arg(static_cast<double>(header->SC_HI) , 12).arg(static_cast<double>(header->SC_LO), 12) ;
        unsigned int count = param.count < vVal.size() ? param.count : vVal.size();
        for (unsigned int i = param.start; i < count; i += param.step ) {
            res.append(QString(";%1").arg(vVal[i].val, 10));
        }
        return res;
    }

    const QString GetGidStr() const
    {
        return QString::number(header->gid);
    }

    const QString GeSC_HIdStr() const
    {
        return QString::number(static_cast<double>(header->SC_HI));
    }

    const QString GeSC_LOdStr() const
    {
        return QString::number(static_cast<double>(header->SC_LO));
    }

    const QString GetValByIndex(unsigned int _index, const T_ValParamLtaData &param) const
    {
        unsigned int index = param.start + _index * param.step;
        if( index < vVal.size() && index < param.count ) return QString::number(vVal[index].val);
        return "-";
    }

};


/*
 * CModelLTADatarchive класс для таблицы отчета
 */
class CModelLTADatarchive: public QAbstractTableModel
{
    Q_OBJECT
public:
    CModelLTADatarchive( QVector<T_LTADataRecDispl*>  &_vLTAdata);

    ~CModelLTADatarchive() override;

    THeaderParamLtaData *getHeaderParam();

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex& index, int nRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant headerData(int nsection,    Qt::Orientation orientation,    int nRole = Qt::DisplayRole    ) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool SaveToFile(const char * name_file, QProgressDialog *prg);

    void SetHeaderParamLtaData( THeaderParamLtaData &headerParam);

private:

    QVector<T_LTADataRecDispl*>  &vLTAdata;

    THeaderParamLtaData         headerParam;

};

#endif // CMYLTARCHIVE_H
