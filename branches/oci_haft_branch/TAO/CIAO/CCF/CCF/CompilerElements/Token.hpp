#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "MSVC_Pragmas.hpp"

#include <vector>
#include <string>
//@@ it's a good idea to include ostream instead of iostream where I only
//   use ostreams
#include <ostream>

#include "CCF/CompilerElements/ReferenceCounting.hpp"

//
//
//
class Token : public ReferenceCounting::DefaultImpl <>
{
protected:
  Token (unsigned long line)
      : line_ (line)
  {
  }

public:
  virtual ~Token () throw () {}

public:
  unsigned long
  line () const
  {
    return line_;
  }

private:
  unsigned long line_;
};

typedef
ReferenceCounting::StrictPtr<Token>
TokenPtr;

typedef
std::vector<TokenPtr>
TokenStream;

//
//
//
class EndOfStream : public Token
{
public:
  EndOfStream ()
      : Token (0)
  {
  }

  virtual ~EndOfStream () throw () {}
};

typedef
ReferenceCounting::StrictPtr<EndOfStream>
EndOfStreamPtr;

//
//
//
class Keyword : public Token
{
public:
  Keyword ()
      : Token (0)
  {
  }

  virtual
  ~Keyword () throw () {}
};

typedef
ReferenceCounting::StrictPtr<Keyword>
KeywordPtr;



//
//
//
class Punctuation : public Token
{
public:
  Punctuation ()
      : Token (0)
  {
  }

  virtual
  ~Punctuation () throw () {}
};

typedef
ReferenceCounting::StrictPtr<Punctuation>
PunctuationPtr;


//
//
//
class Identifier : public Token
{
public:
  Identifier (std::string const& value, unsigned long line)
      : Token (line),
        value_ (value)
  {
  }

  virtual
  ~Identifier () throw () {}

  //@@ this mf should probably be called str () or something like this
  std::string
  value () const
  {
    return value_;
  }

protected:
  std::string value_;
};

typedef
ReferenceCounting::StrictPtr<Identifier>
IdentifierPtr;

inline std::ostream&
operator<< (std::ostream& o, IdentifierPtr const& id)
{
  if (id != 0) o << id->value ();
  return o;
}

//
//
//
class SimpleIdentifier : public Identifier
{
public:
  SimpleIdentifier (std::string const& value, unsigned long line)
      : Identifier (value, line)
  {
  }

  virtual
  ~SimpleIdentifier () throw () {}
};

typedef
ReferenceCounting::StrictPtr<SimpleIdentifier>
SimpleIdentifierPtr;

inline std::ostream&
operator<< (std::ostream& o, SimpleIdentifierPtr const& id)
{
  if (id != 0) o << id->value ();
  return o;
}

//
//
//
class ScopedIdentifier : public Identifier
{
public:
  ScopedIdentifier (std::string const& value, unsigned long line)
      : Identifier (value, line)
  {
  }

  virtual
  ~ScopedIdentifier () throw () {}
};

typedef
ReferenceCounting::StrictPtr<ScopedIdentifier>
ScopedIdentifierPtr;

inline std::ostream&
operator<< (std::ostream& o, ScopedIdentifierPtr const& id)
{
  if (id != 0) o << id->value ();
  return o;
}

//
//
//
class StringLiteral : public Token
{
public:
  StringLiteral (std::string const& value, unsigned long line)
      : Token (line),
        value_ (value)
  {
  }

  virtual
  ~StringLiteral () throw () {}

  std::string
  value () const
  {
    return value_;
  }

protected:
  std::string value_;
};

typedef
ReferenceCounting::StrictPtr<StringLiteral>
StringLiteralPtr;

inline std::ostream&
operator<< (std::ostream& o, StringLiteralPtr const& sl)
{
  if (sl != 0) o << sl->value ();
  return o;
}

//@@ this is experimental code
//
//
template <typename TypePtr>
bool identical (TypePtr const& a, TokenPtr const& b)
{
  typedef
  typename TypePtr::Type
  Type;

  Type* t = dynamic_cast<Type*>(b.in ());
  return (t != 0) && (t == a.in ());
};

template <typename TypePtr>
TypePtr typical (TokenPtr a)
{
  typedef
  typename TypePtr::Type
  Type;

  return TypePtr(add_ref(dynamic_cast<Type*>(a.in ())));
};

#endif // TOKEN_HPP
