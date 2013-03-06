#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "object.h"
#include <unordered_map>
#include <vector>

typedef size_t SymbolID;
static const SymbolID INVALID_SYMBOL_ID = static_cast<SymbolID>(-1);


class Symbol: public Object
{
public:
    static bool is_symbol(Object* obj);
public:
    explicit Symbol(const char* value);

    virtual std::string to_string() const;
    virtual std::string type_name() const { return "symbol"; }

    virtual Object* eval(Environment* env) override;

    const char* name() const;

    SymbolID id() const { return id_; }

    bool equal(const Symbol& rhs) const { return id_ == rhs.id_; }
private:
    SymbolID id_;
};


class Symbols
{
public:
    static Symbols& instance();
public:
    SymbolID add_symbol(const char* symbol);

    SymbolID find_symbol(const char* symbol);

    const char* get(SymbolID id) const;
private:
    Symbols() = default;
    ~Symbols();
    Symbols& operator=(const Symbols&) = delete;
    Symbols(const Symbols&) = delete;

    typedef std::vector<char*> SymbolStrs;
    typedef std::unordered_map<const char*, size_t> SymbolMap;

    SymbolStrs symbol_strs_;
    SymbolMap symbol_map_;
};

#endif
