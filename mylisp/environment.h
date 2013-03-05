#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>


class Object;


class Environment
{
public:
    explicit Environment(Environment* outer);

    Object* look_up_variable(const char* name);
    const Object* look_up_variable(const char* name) const;
    void define_variable(const char* name, Object* value);
    bool set_variable(const char* name, Object* value);

    Object* get(const char* name);
    const Object* get(const char* name) const;
    void set(const char* name, Object* value);

private:
    typedef std::unordered_map<const char*, Object*> Bindings;
    Bindings bindings_;

    Environment* outer_;
};

#endif
