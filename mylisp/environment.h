#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <boost/utility.hpp>


class Object;
typedef size_t SymbolID;


class Environment: private boost::noncopyable
{
public:
    explicit Environment(Environment* outer);

    Object* look_up_variable(SymbolID id);
    const Object* look_up_variable(SymbolID id) const;
    void define_variable(SymbolID id, Object* value);
    bool set_variable(SymbolID id, Object* value);

    Object* get(SymbolID id);
    const Object* get(SymbolID id) const;
    void set(SymbolID id, Object* value);

    const Environment* outer() const { return outer_; }
    Environment* outer() { return outer_; }
    void set_outer(Environment* outer) { outer_ = outer; }
private:
    typedef std::unordered_map<SymbolID, Object*> Bindings;
    Bindings bindings_;

    Environment* outer_;
};

#endif
