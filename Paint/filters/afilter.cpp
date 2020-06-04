#include "afilter.h"

AFilter::AFilter()
{

}
/**
 * @brief AFilter::AFilter
 * @param name - name of filter
 */
AFilter::AFilter(const QString& name):name(name){

}
/**
 * @brief AFilter::getParams
 * @return list of parameters for current filter
 */
QList<Parameter> AFilter::getParams() const{
    return params;
};
/**
 * @brief AFilter::getFilterName
 * @return name of filter
 */
const QString& AFilter::getFilterName()const{
    return name;
}
