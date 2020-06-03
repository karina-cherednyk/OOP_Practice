#include "afilter.h"

AFilter::AFilter()
{

}
/**
 * @brief AFilter::getParams
 * @return list of parameters for current filter
 */
QList<Parameter> AFilter::getParams() const{
    return params;
};
