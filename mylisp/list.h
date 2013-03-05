#ifndef LIST_H
#define LIST_H

#include "object.h"


class ListObject: public Object
{
public:
    explicit ListObject(Object* value);

    virtual std::string to_string() const;
    virtual std::string type_name() const { return "list"; }

    virtual Object* eval(Environment* env) override;

    const Object* value() const { return value_; }
    Object* value() { return value_; }
    void set_value(Object* value) { value_ = value; }

    const ListObject* next() const { return next_; }
    ListObject* next() { return next_; }
    bool set_next(Object* next);

    size_t length() const;

    bool empty() const { return  next_ == nullptr; }
private:
    Object* value_;
    ListObject* next_;
};


ListObject* pair(Object* car, Object* cdr);


#endif
