#include "cmyltarchive.h"
#include "LTArchive.h"
#include <QFile>
#include <QTextStream>

CModelLTArchive::CModelLTArchive(QVector<T_LTAHeadRecDispl> &_vRecHeadDispl) :vRecHeadDispl(_vRecHeadDispl)
{

}


CModelLTArchive::~CModelLTArchive(){

}


int CModelLTArchive::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant CModelLTArchive::data(const QModelIndex &index, int nRole) const
{
    if(!index.isValid())  return QVariant();
    if(index.row()< 0 || index.row() >= vRecHeadDispl.size())
        return QVariant();
    if(nRole == Qt::DisplayRole ){
        if( index.column() == 0 )
            return vRecHeadDispl.at(index.row()).TagName;
        if( index.column() == 1)
            return vRecHeadDispl.at(index.row()).TagDesc;
        if( index.column() == 2)
            return vRecHeadDispl.at(index.row()).type_str;
     //   if( index.column() == 3)
         return vRecHeadDispl.at(index.row()).EU;
    }
    if(nRole == Qt::UserRole) return index.row();
    return QVariant();
}

bool CModelLTArchive::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
     return false;
}

int CModelLTArchive::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return vRecHeadDispl.size();
}

QVariant CModelLTArchive::headerData(int nsection, Qt::Orientation orientation, int nRole) const
{

    if (nRole != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (nsection) {
                case 0:
                    return tr("Наименование");
                case 1:
                    return tr("Описание");
                case 2:
                    return tr("Тип");
                case 3:
                    return tr("Е.И.");
                default:
                    return QVariant();
            }
        }
        return QVariant();
}

Qt::ItemFlags CModelLTArchive::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
}

bool CModelLTArchive::insertRows(int nRow, int nCount, const QModelIndex &parent)
{
     return false;
}

bool CModelLTArchive::removeRows(int nRow, int nCount, const QModelIndex &parent)
{
    return false;
}

//T_LTAHeadRecDispl *CModelLTArchive::getItem(int index)
//{
//   // if( index < arrRecHead.size() ) return &arrRecHead[index];
//    if( index < vRecHeadDispl.size() ) return &vRecHeadDispl[index];
//    return nullptr;
//}


//-------------------------------------------------------------------------

CModelLTADatarchive::CModelLTADatarchive()
{

}


CModelLTADatarchive::~CModelLTADatarchive()
{

}

THeaderParamLtaData *CModelLTADatarchive::getHeaderParam()
{
    return &headerParam;
}

void CModelLTADatarchive::addData(T_LTADataRecDispl &data)
{
    beginResetModel();
    vLTAdata.push_back(data);
    endResetModel();
}

void CModelLTADatarchive::clearData()
{
    beginResetModel();
    vLTAdata.clear();
    endResetModel();
}


int CModelLTADatarchive::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if( vLTAdata.size() )  return (5 + static_cast<int>(vLTAdata[0].vVal.size()));
    return 5 ;
}

QVariant CModelLTADatarchive::data(const QModelIndex &index, int nRole) const
{
    if(!index.isValid())  return QVariant();
    if(index.row()< 0 || index.row() >= vLTAdata.size())
        return QVariant();
    if(nRole == Qt::DisplayRole ){
        switch(index.column()){
        case 0:
            return vLTAdata.at(index.row()).GetGidStr();
        case 1:
            return vLTAdata.at(index.row()).TagName;
        case 2:
           return vLTAdata.at(index.row()).TagDesc;
        case 3:
           return vLTAdata.at(index.row()).GeSC_HIdStr();
        case 4:
           return vLTAdata.at(index.row()).GeSC_LOdStr();
        default:
           return  vLTAdata.at(index.row()).GetValByIndex(static_cast<unsigned>( index.column() - 5 ));
        }
    }
    if(nRole == Qt::UserRole) return index.row();
    return QVariant();
}



int CModelLTADatarchive::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return vLTAdata.size();
}


QVariant CModelLTADatarchive::headerData(int nsection, Qt::Orientation orientation, int nRole) const
{
    if (nRole != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (nsection) {
                case 0:
                    return tr("Гид");
                case 1:
                    return tr("Наименование");
                case 2:
                    return tr("Описание");
                case 3:
                    return tr("Верхнее\r\nотклонение");
                case 4:
                    return tr("Нижнее\r\nотклонение");
                default:
                    return headerParam.GetTimeByIndex(nsection - 5);
            }
        }
        return QVariant();

}

Qt::ItemFlags CModelLTADatarchive::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
}

//bool CModelLTADatarchive::SaveToFile(const char *name_file)
//{
//    QFile file(name_file);
//    if( file.open(QIODevice::WriteOnly) ) {
//        QTextStream out(&file);
//        out.setCodec("windows-1251");
//        out << T_LTADataRecDispl::GetHeadreStrCSV();
//        out << headerParam.GetStrCSV() << "\n";
//        foreach (T_LTADataRecDispl item, vLTAdata){
//            out << item.GetStrCSV() << "\n";
//        }
//        file.close();
//        return true;
//    }
//    return false;
//}

bool CModelLTADatarchive::SaveToFile(const char *name_file, QProgressDialog *prg)
{

    QFile file(name_file);
    if( file.open(QIODevice::WriteOnly) ) {
        prg->setRange(0, vLTAdata.size());
        QTextStream out(&file);
        out.setCodec("windows-1251");
        out << T_LTADataRecDispl::GetHeadreStrCSV();
        out << headerParam.GetStrCSV() << "\n";
        for (int i = 0; i < vLTAdata.size(); ++i){
            out << vLTAdata[i].GetStrCSV() << "\n";
            prg->setValue(i);
            if( prg->wasCanceled() ) break;
        }
        file.close();
        prg->setValue(vLTAdata.size());
        return true;
    }
    return false;
}




