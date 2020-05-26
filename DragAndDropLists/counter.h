#ifndef COUNTER_H
#define COUNTER_H

#include <QPainter>
#include <QString>
#include <QStyledItemDelegate>
#include <QDebug>
class Counter {
private:
    QString _name;
    int _count;
public:
    inline Counter(const QString& n="aa"):_name(n),_count(0) {}
    inline Counter(const Counter& c):_name(c._name),_count(c._count){}
public:
    inline QString& name()  { return _name;}
    inline const QString& name() const  { return _name;}
    inline Counter& operator++() {++_count; return *this;}
    inline int& count() {return _count;}
    inline const int& count()const {return _count;}


    inline QString toString()  {return _name;}
    inline Counter& operator=(const Counter& c){
        _name = c._name;
        _count = c._count;
        return *this;
    }
};
Q_DECLARE_METATYPE( Counter )
inline QDataStream &operator<<(QDataStream &out, const Counter &myObj)
{
        out << myObj.name() << myObj.count();
        return out;
}

inline QDataStream &operator>>(QDataStream &in, Counter &myObj)
{
        in >> myObj.name() >> myObj.count();
        return in;
}

#endif // COUNTER_H
