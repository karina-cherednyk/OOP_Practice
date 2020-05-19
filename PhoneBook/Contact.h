#ifndef CONTACT_H
#define CONTACT_H



#include <QSet>

class Contact
{

public:

    QSet<QString> _nums;
    QString _name;

public:
    inline Contact(const QString& name, const QStringList& nums){
        this->_name = name;
        this->_nums = nums.toSet();
    }
    inline Contact(const Contact& c){
        this->_name =c._name;
        this->_nums =c._nums;

    };

    inline const QString& name() const{
        return _name;
    }
    inline void setName(const QString& name){
        this->_name = name;
    }

    inline void addNum(const QString& num){
        _nums << num;
    }
    inline void setNums(const QList<QString> nums){
        _nums = nums.toSet();
    }
    inline void removeNum(const QString& num){
         _nums.remove(num);
    }
    inline const QSet<QString>& nums() const {
        return _nums;
    }

};

#endif // CONTACT_H
