#include "cmyltarchive.h"
#include "LTArchive.h"

CModelLTArchive::CModelLTArchive(){

}


CModelLTArchive::~CModelLTArchive(){
   // if(instance)   delete instance;
}


//CModelLTArchive *CModelLTArchive::GetInst()
//{
//    if(!instance) instance = new CModelLTArchive();
//    return instance;
//}

int CModelLTArchive::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

//bool CModelLTArchive::OpenFile(QString &fileName)
//{
//    if( m_LTArchive.Open(fileName.toStdString().c_str()) ){
//        arrRecHead.clear();
//        return  m_LTArchive.GetRecHeads(arrRecHead);
//    }
//    return false;
//}

bool CModelLTArchive::OpenFile(QString &fileName)
{
    if( m_LTArchive.Open(fileName.toStdString().c_str()) ){
        CLTArchive::LTARecArrayT arrRecHead;
        CMyLTAHeadRec myLTAHeadRec;
        if(  m_LTArchive.GetRecHeads(arrRecHead) ){
            vRecHeadDispl.clear();
            vRecHeadDispl.reserve(arrRecHead.size());
            for(int i = 0; i < arrRecHead.size(); ++i){
                myLTAHeadRec.ltaHeadRec = &arrRecHead.at(i);
                T_LTAHeadRecDispl disp;
                myLTAHeadRec.ConvertTo(&disp);
                vRecHeadDispl.push_back(disp);
            }
            return !vRecHeadDispl.empty();
        }
    }
    return false;
}

//QVariant CModelLTArchive::data(const QModelIndex &index, int nRole) const
//{
//    if(!index.isValid())  return QVariant();
//    if(index.row()< 0 || index.row() >= arrRecHead.size())
//        return QVariant();
//    if(nRole == Qt::DisplayRole ){
//        if( index.column() == 0 )
//            return RUS(arrRecHead.at(index.row()).TagName.c_str());
//        if( index.column() == 1)
//            return RUS(arrRecHead.at(index.row()).TagDesc.c_str());
//        if( index.column() == 2)
//            return RUS(arrRecHead.at(index.row()).EU.c_str());
//    }
//    if(nRole == Qt::UserRole) return index.row();
//    return QVariant();
//}

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

T_LTAHeadRecDispl *CModelLTArchive::getItem(int index)
{
   // if( index < arrRecHead.size() ) return &arrRecHead[index];
    if( index < vRecHeadDispl.size() ) return &vRecHeadDispl[index];
    return nullptr;
}

bool CModelLTArchive::GetDataByIndex(const dword index, deque<VQT> &array)
{
    array.clear();
    return m_LTArchive.GetDataByIndex(index, array);
}

 //CModelLTArchive *CModelLTArchive::instance = nullptr;

//-------------------------------------------------------------------------

CModelLTADatarchive::CModelLTADatarchive(){

}


CModelLTADatarchive::~CModelLTADatarchive(){

}

void CModelLTADatarchive::addData(T_LTADataRecDispl &data, deque<VQT> *arr, TParamLtaData *par)
{
    beginResetModel();
    vLTAdata.push_back(data);
    if( arr ){
        vLTAdata.back().vaues.clear();
        if( par ){
            for(unsigned int i =  par->t0 ; i < arr->size() && i < par->count; i+= par->step)
                vLTAdata.back().vaues.push_back(arr->at(i).m_Value);
        }
        else{
            deque<VQT>::iterator it;
            for(it = arr->begin(); it!= arr->end(); ++it)
                vLTAdata.back().vaues.push_back(it->m_Value);
        }
    }
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
    if( vLTAdata.size() )  return (5 + static_cast<int>(vLTAdata[0].vaues.size()));
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
           return  vLTAdata.at(index.row()).GetValByIndex(static_cast<unsigned>( index.column() ));
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

QString headerTime(int nsection)
{
    return QString::number(nsection);
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
                    return headerTime(nsection);
            }
        }
        return QVariant();

}

Qt::ItemFlags CModelLTADatarchive::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
}




