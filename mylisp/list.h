#ifndef LIST_H
#define LIST_H

#include "object.h"


/*
 * null list is a ListObject with both cdr and car are nullptr 
 * otherwise neither should be nullptr
 */


class ListObject: public Object
{
public:
    static bool is_pair(const Object*);
    static bool is_list(const Object*);
    static bool is_null_list(const Object*);
public:
    explicit ListObject(Object* car, Object* cdr = nullptr);

    virtual std::string to_string() const;
    virtual std::string type_name() const { return "list"; }

    virtual Object* eval(Environment* env) override;

#define IMPLEMENT_ACCESSORS(name) \
    const Object* name() const { return name ## _; } \
    Object* name() { return name ## _; } \
    void set_ ## name(Object* name) { name ## _ = name; } \

    IMPLEMENT_ACCESSORS(car)
    IMPLEMENT_ACCESSORS(cdr)

#undef IMPLEMENT_ACCESSORS
    // use only when you know its good!
    ListObject* cdr_as_list() { return static_cast<ListObject*>(cdr_); }
    const ListObject* cdr_as_list() const { return static_cast<ListObject*>(cdr_); }

    bool proper() const;  // ends with nil
    bool improper() const { return !proper(); }
    int length() const;
    bool empty() const { return car_ == nullptr; }
private:
    Object* car_;
    Object* cdr_;
};


ListObject* pair(Object* car, Object* cdr);
ListObject* null_list();


inline ListObject* list(Object* car)
{
    return new ListObject(car, null_list());
}

template<typename... Targs>
ListObject* list(Object* car, Targs...fargs)
{
    return new ListObject(car, list(fargs...));
}


// try to treat cdr as a list head
ListObject* try_list_cdr(ListObject* l);


#endif  // LIST_H
