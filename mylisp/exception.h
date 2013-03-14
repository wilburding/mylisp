#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "object.h"


class Exception: public Object
{
public:
    explicit Exception(const char* message = "");
    explicit Exception(const std::string& message);
    explicit Exception(std::string&& message);

    virtual std::string type_name() const override { return "exception"; }
    virtual std::string to_string() const override;

    virtual Object* eval(Environment* ) { return this; }

    const std::string& message() const { return message_; }
    void set_message(const std::string& message) { message_ = message; }

    const std::string& value() const { return message_; }
private:
    std::string message_;
};


void set_exception(Exception* exception);
Exception* get_exception();
inline bool exception_occured() { return get_exception(); }

#endif  // EXCEPTION_H
