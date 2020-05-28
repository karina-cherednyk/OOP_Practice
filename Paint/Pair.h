#ifndef PAIR_H
#define PAIR_H

#include <QPair>
/**
 *TODO: replace Pair with custom class for Layer(QString layerName, QImage image, bool isVisible)
 */
template <class T, class U>
class Pair{
    T _first;
    U _second;
public:
   inline  Pair(const T& f, const U& s):_first(f),_second(s){};
   inline  T& first(){ return  _first;};
   inline U& second(){ return  _second;};
   operator QPair<T,U>() { return QPair<T,U>(_first,_second);}
};

#endif // PAIR_H

