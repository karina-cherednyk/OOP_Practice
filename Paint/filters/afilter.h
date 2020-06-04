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
    double def;
};

class AFilter
{
public:
    AFilter();
    AFilter(const QString&);
    virtual~AFilter() {} ;
    virtual QList<Parameter> getParams() const;
    virtual QImage doFiltration(const QImage&,const QHash<QString,double>* coeffs = nullptr)=0;
    virtual const QString& getFilterName()const;
protected:
    QList<Parameter> params;//list of parameters for filtering
    QString name;//name of filter

};

#endif // AFILTER_H
