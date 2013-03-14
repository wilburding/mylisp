#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include <boost/utility.hpp>

class Environment;


class Object: private boost::noncopyable
{
public:
    Object() = default;
    virtual ~Object();

    virtual std::string repr() const;
    virtual std::string to_string() const = 0;
    virtual std::string type_name() const = 0;

    virtual Object* eval(Environment* env) = 0;
};


class BoolObject: public Object
{
public:
    static bool is_bool(const Object* obj);
public:
    explicit BoolObject(bool value = false);

    virtual std::string type_name() const override { return {"bool"}; }

    virtual std::string to_string() const { return std::to_string(value_); }

    virtual Object* eval(Environment* ) override { return this; }

    bool value() const { return value_; }
private:
    bool value_;
};


bool is_true(const Object* obj);


class IntObject: public Object
{
public:
    explicit IntObject(int value = 0);

    virtual std::string type_name() const override { return {"int"}; }

    virtual std::string to_string() const { return std::to_string(value_); }

    virtual Object* eval(Environment* ) override { return this; }

    int value() const { return value_; }
private:
    int value_;
};


class StringObject: public Object
{
public:
    explicit StringObject(const std::string& value);
    StringObject(std::string&& value);

    virtual std::string type_name() const override { return {"string"}; }

    virtual std::string to_string() const { return value_; }

    virtual Object* eval(Environment* ) override { return this; }

    const std::string& value() const { return value_; }
    std::string& value() { return value_; }
private:
    std::string value_;
};


#endif  // OBJECT_H
