#ifndef AFILTER_H
#define AFILTER_H
#include <QString>
#include <QList>
class QImage;
/**
 * @brief The Types enum
 * type of parameter for filtering
 */
enum Types{
    INT,
    DOUBLE
};
/**
 * @brief The Parameter struct
 * information about parameter for filtering
 */
struct Parameter{
    QString name;
    Types type;
    double min;
    double max;
};

class AFilter
{
public:
    AFilter();
    virtual~AFilter() {} ;
    virtual QList<Parameter> getParams() const;
    virtual QImage doFiltration(const QImage&,const QHash<QString,double>* coeffs = nullptr)=0;
protected:
    QList<Parameter> params;//list of parameters for filtering

};

#endif // AFILTER_H
