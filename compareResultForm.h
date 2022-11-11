#ifndef COMPARERESULTFORM_H
#define COMPARERESULTFORM_H

#include <QWidget>
#include "ltareaderlib.h"

namespace Ui {
class CompareResultForm;
}

struct CompareResultIten
{
     long long time;

     T_ItemVal val1;

     T_ItemVal val2;

     CompareResultIten(long long _time, T_ItemVal &_val1, T_ItemVal &_val2)
         :time(_time), val1(_val1), val2(_val2)
     {
     }

     double DiffValue()
     {
         return val1.val - val2.val;
     }
};


struct CompareResultData
{
    QString name_field[2];                     // Имена сравниваемых тегов

    vector<CompareResultIten> vResultItem;     // Массив значений

    CompareResultData()
    {
        name_field[0] = name_field[1] = "";
    }

    CompareResultData(const CompareResultData &obj)
    {
        name_field[0] = obj.name_field[0];
        name_field[1] = obj.name_field[1];
        vResultItem = obj.vResultItem;
    }

    CompareResultData &operator = (const CompareResultData &obj)
    {
        if( this != &obj ) {
            name_field[0] = obj.name_field[0];
            name_field[1] = obj.name_field[1];
            vResultItem = obj.vResultItem;
        }
        return *this;
    }

    CompareResultData( CompareResultData && obj)
    {
        name_field[0] = std::move(obj.name_field[0]);
        name_field[1] = std::move(obj.name_field[1]);
        vResultItem = std::move(obj.vResultItem);
    }

    CompareResultData & operator = (CompareResultData && obj)
    {
        if( this != &obj ) {
            name_field[0] = std::move(obj.name_field[0]);
            name_field[1] = std::move(obj.name_field[1]);
            vResultItem = std::move(obj.vResultItem);
        }
        return *this;
    }

    void SeVectorResult( vector<T_ItemVal> &v1,   vector<T_ItemVal> &v2)
    {
        vResultItem.clear();
        for( unsigned i = 0; i < v1.size() && i < v2.size(); ++i ){
            vResultItem.emplace_back(v1[1].time, v1[i], v2[i]);
        }
    }

    void SetField(unsigned num, const char *nameTag, const char *nameArch)
    {
        if( num > 1) return;
        QString res = nameArch;
        res.append("\n\r");
        res.append(nameTag);
        name_field[num] = res;
    }

};

class CompareResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit CompareResultForm(CompareResultData  &_CompareResultData, QWidget *parent = nullptr);
    ~CompareResultForm() override;

private:

    Ui::CompareResultForm   *ui;

    CompareResultData       m_CompareResultData;
};

#endif // COMPARERESULTFORM_H
