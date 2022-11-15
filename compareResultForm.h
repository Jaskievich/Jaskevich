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

// НОД
inline unsigned long _GCD(unsigned long a, unsigned b)
{
    while(a != b){
        if( a > b) a -= b ; else b -= a;
    }
    return a;
}


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

    void SetVectorResult( vector<T_ItemVal> &v1, unsigned long step1,   vector<T_ItemVal> &v2, unsigned long step2)
    {
        unsigned step_i = 1, step_j = 1;
        if( step1 != step2) {
           unsigned long gcd = _GCD(step1, step2); // вычислить НОД (наибольший общий делитель)
           step_j = step1/gcd;
           step_i = step2/gcd;
        }
        vResultItem.clear();
        for( unsigned i = 0, j = 0; i < v1.size() && j < v2.size(); i += step_i, j += step_j ){
            vResultItem.emplace_back(v1[i].time, v1[i], v2[j]);
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
