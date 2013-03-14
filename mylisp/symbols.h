#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "object.h"
#include "util.h"

#include <unordered_map>
#include <vector>
#include <boost/noncopyable.hpp>


typedef size_t SymbolID;
static const SymbolID INVALID_SYMBOL_ID = static_cast<SymbolID>(-1);


class Symbols;


class Symbol: public Object
{
public:
    static bool is_symbol(Object* obj);
public:
    Symbol(const char* name, Symbols* symbols);
    Symbol(SymbolID id, Symbols* symbols);

    virtual std::string to_string() const;
    virtual std::string type_name() const { return "symbol"; }

    virtual Object* eval(Environment* env) override;

    const char* name() const;

    SymbolID id() const { return id_; }

    bool equal(const Symbol& rhs) const { return id_ == rhs.id_; }
private:
    SymbolID id_;
    Symbols* owner_;
};


class Symbols: private boost::noncopyable
{
public:
    Symbols() = default;
    ~Symbols();

    SymbolID add(const char* symbol);

    SymbolID find(const char* symbol) const;

    const char* get(SymbolID id) const;
private:
    typedef std::vector<char*> SymbolStrs;
    typedef std::unordered_map<const char*, SymbolID, CStrHash, CStrEqualTo> SymbolMap;

    SymbolStrs symbol_strs_;
    SymbolMap symbol_map_;
};

#endif  // OBJECT_H
