#include "cmyltarchive.h"
#include <QMessageBox>
//-------------------------------------------------


CLoaderLibrary::CLoaderLibrary()
{
    p_CreateReaderInst = nullptr;
    p_GetStatus = nullptr;
}

CLoaderLibrary::~CLoaderLibrary()
{
    p_CreateReaderInst = nullptr;
    p_GetStatus = nullptr;
    libr.unload();
}

void CLoaderLibrary::Load_library_lta(const QString &str)
{
    const char *name_file_dll;
    if( str == ".lta" ){
        name_file_dll = "LTAReaderlibVinca.dll";
    }
    else if(  str == ".alta"  ){
        name_file_dll = "LTAReaderlibAlldan.dll";
    }
    else throw "Не верное расширение файла";
    libr.setFileName(name_file_dll);
    if( libr.load() ){
        p_CreateReaderInst = reinterpret_cast<T_CreateReaderInst>(libr.resolve( "CreateReaderInst"));
        p_GetStatus =  reinterpret_cast< T_GetStatus >(libr.resolve("GetStatusAsStr_utf8"));
    }
    if( !p_CreateReaderInst ) {
        QString msg = "Файл " + QString(name_file_dll) + " не открыт";
        throw msg;
    }
}

CLTAReaderLib * CLoaderLibrary::CreateReaderInst(const char *param)
{
    if( p_CreateReaderInst )  return p_CreateReaderInst(param);
    return nullptr;
}

void CLoaderLibrary::GetStatusAsStr_utf8(unsigned int status, char text[1024])
{
    text[0] = 0;
    if( p_GetStatus)  p_GetStatus(status, text);
}


CLTAReaderLib::CLTAReaderLib()
{

}

CLTAReaderLib::~CLTAReaderLib()
{
}


//---------------------------------------------


CModelLTArchive::CModelLTArchive(vector<T_LTAHeadRecDispl> &_vRecHeadDispl) :vRecHeadDispl(_vRecHeadDispl)
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
    unsigned int row = static_cast<unsigned int>(index.row());
    if(index.row() < 0 || row >= vRecHeadDispl.size())
        return QVariant();
    if(nRole == Qt::DisplayRole ){
        if( index.column() == 0 )
            return vRecHeadDispl.at(row).TagName;
        if( index.column() == 1)
            return vRecHeadDispl.at(row).TagDesc;
        if( index.column() == 2)
            return vRecHeadDispl.at(row).type_str;
     //   if( index.column() == 3)
         return vRecHeadDispl.at(row).EU;
    }
    if(nRole == Qt::UserRole) return row;
    return QVariant();
}

//bool CModelLTArchive::setData(const QModelIndex &index, const QVariant &value, int nRole)
//{
//     return false;
//}

int CModelLTArchive::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(vRecHeadDispl.size()) ;
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

//bool CModelLTArchive::insertRows(int nRow, int nCount, const QModelIndex &parent)
//{
//     return false;
//}

//bool CModelLTArchive::removeRows(int nRow, int nCount, const QModelIndex &parent)
//{
//    return false;
//}


//-------------------------------------------------------------------------

CModelLTADatarchive::CModelLTADatarchive(QVector<T_LTADataRecDispl*> &_vLTAdata):vLTAdata(_vLTAdata)
{

}


CModelLTADatarchive::~CModelLTADatarchive()
{

}

THeaderParamLtaData *CModelLTADatarchive::getHeaderParam()
{
    return &headerParam;
}


int CModelLTADatarchive::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
 //   if( vLTAdata.size() )  return (5 + static_cast<int>(vLTAdata[0]->vVal.size()));
    return (5 + static_cast<int>(headerParam.vDataStr.size())) ;
}

QVariant CModelLTADatarchive::data(const QModelIndex &index, int nRole) const
{
    if(!index.isValid())  return QVariant();
    if(index.row()< 0 || index.row() >= vLTAdata.size())
        return QVariant();
    if(nRole == Qt::DisplayRole ){
        switch(index.column()){
        case 0:
            return vLTAdata.at(index.row())->GetGidStr();
        case 1:
            return vLTAdata.at(index.row())->header->TagName;
        case 2:
           return vLTAdata.at(index.row())->header->TagDesc;
        case 3:
           return vLTAdata.at(index.row())->GeSC_HIdStr();
        case 4:
           return vLTAdata.at(index.row())->GeSC_LOdStr();
        default:
           return  vLTAdata.at(index.row())->GetValByIndex(static_cast<unsigned>( index.column() - 5 ), headerParam.valParam );
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
                    return headerParam.GetTimeByIndex(static_cast<unsigned int>(nsection) - 5);
            }
        }
        return QVariant();

}

Qt::ItemFlags CModelLTADatarchive::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
}

void CModelLTADatarchive::SetHeaderParamLtaData(THeaderParamLtaData &headerParam)
{
    this->headerParam = headerParam;
}




