// file      : CIDLC/TypeNameEmitter.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TYPENAME_EMITTER_HPP
#define TYPENAME_EMITTER_HPP

#include "CCF/CIDL/Traversal.hpp"

using namespace CCF;
using namespace CIDL;
using namespace SyntaxTree;

// Base class that holds the ostream member
// common to every other class in this file,
// and collects the ancestors in one place.
//
class TypeNameEmitter : public Traversal::Void,
                        public Traversal::Boolean,
                        public Traversal::Octet,
                        public Traversal::Char,
                        public Traversal::Wchar,
                        public Traversal::Short,
                        public Traversal::UnsignedShort,
                        public Traversal::Long,
                        public Traversal::UnsignedLong,
                        public Traversal::LongLong,
                        public Traversal::UnsignedLongLong,
                        public Traversal::Float,
                        public Traversal::Double,
                        public Traversal::String,
                        public Traversal::Wstring,
                        public Traversal::Object,
                        public Traversal::ValueBase,
                        public Traversal::Any,
                        public Traversal::LocalInterfaceDecl
{
protected:
  TypeNameEmitter (std::ostream&);

  std::ostream& os;
};

// Generates the name of an operation's return type.
//
//
class ReturnTypeNameEmitter : public TypeNameEmitter
{
public:
  ReturnTypeNameEmitter (std::ostream&);

  virtual void traverse (VoidPtr const&);
  virtual void traverse (BooleanPtr const&);
  virtual void traverse (OctetPtr const&);
  virtual void traverse (CharPtr const&);
  virtual void traverse (WcharPtr const&);
  virtual void traverse (ShortPtr const&);
  virtual void traverse (UnsignedShortPtr const&);
  virtual void traverse (LongPtr const&);
  virtual void traverse (UnsignedLongPtr const&);
  virtual void traverse (LongLongPtr const&);
  virtual void traverse (UnsignedLongLongPtr const&);
  virtual void traverse (FloatPtr const&);
  virtual void traverse (DoublePtr const&);
  virtual void traverse (StringPtr const&);
  virtual void traverse (WstringPtr const&);
  virtual void traverse (ObjectPtr const&);
  virtual void traverse (ValueBasePtr const&);
  virtual void traverse (AnyPtr const&);
  virtual void traverse (LocalInterfaceDeclPtr const&);
};
  
// Generates the typename of an IN argument.
//
//
class INArgTypeNameEmitter : public TypeNameEmitter
{
public:
  INArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanPtr const&);
  virtual void traverse (OctetPtr const&);
  virtual void traverse (CharPtr const&);
  virtual void traverse (WcharPtr const&);
  virtual void traverse (ShortPtr const&);
  virtual void traverse (UnsignedShortPtr const&);
  virtual void traverse (LongPtr const&);
  virtual void traverse (UnsignedLongPtr const&);
  virtual void traverse (LongLongPtr const&);
  virtual void traverse (UnsignedLongLongPtr const&);
  virtual void traverse (FloatPtr const&);
  virtual void traverse (DoublePtr const&);
  virtual void traverse (StringPtr const&);
  virtual void traverse (WstringPtr const&);
  virtual void traverse (ObjectPtr const&);
  virtual void traverse (ValueBasePtr const&);
  virtual void traverse (AnyPtr const&);
};
  
// Generates the typename of an INOUT argument.
//
//
class INOUTArgTypeNameEmitter : public TypeNameEmitter
{
public:
  INOUTArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanPtr const&);
  virtual void traverse (OctetPtr const&);
  virtual void traverse (CharPtr const&);
  virtual void traverse (WcharPtr const&);
  virtual void traverse (ShortPtr const&);
  virtual void traverse (UnsignedShortPtr const&);
  virtual void traverse (LongPtr const&);
  virtual void traverse (UnsignedLongPtr const&);
  virtual void traverse (LongLongPtr const&);
  virtual void traverse (UnsignedLongLongPtr const&);
  virtual void traverse (FloatPtr const&);
  virtual void traverse (DoublePtr const&);
  virtual void traverse (StringPtr const&);
  virtual void traverse (WstringPtr const&);
  virtual void traverse (ObjectPtr const&);
  virtual void traverse (ValueBasePtr const&);
  virtual void traverse (AnyPtr const&);
};
  
// Generates the typename of an OUT argument.
//
//
class OUTArgTypeNameEmitter : public TypeNameEmitter
{
public:
  OUTArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanPtr const&);
  virtual void traverse (OctetPtr const&);
  virtual void traverse (CharPtr const&);
  virtual void traverse (WcharPtr const&);
  virtual void traverse (ShortPtr const&);
  virtual void traverse (UnsignedShortPtr const&);
  virtual void traverse (LongPtr const&);
  virtual void traverse (UnsignedLongPtr const&);
  virtual void traverse (LongLongPtr const&);
  virtual void traverse (UnsignedLongLongPtr const&);
  virtual void traverse (FloatPtr const&);
  virtual void traverse (DoublePtr const&);
  virtual void traverse (StringPtr const&);
  virtual void traverse (WstringPtr const&);
  virtual void traverse (ObjectPtr const&);
  virtual void traverse (ValueBasePtr const&);
  virtual void traverse (AnyPtr const&);
};
  
#endif /* TYPENAME_EMITTER_HPP */

