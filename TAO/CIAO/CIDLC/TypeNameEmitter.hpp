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
class TypeNameEmitter : public Traversal::VoidDecl,
                        public Traversal::BooleanDecl,
                        public Traversal::OctetDecl,
                        public Traversal::CharDecl,
                        public Traversal::WcharDecl,
                        public Traversal::ShortDecl,
                        public Traversal::UnsignedShortDecl,
                        public Traversal::LongDecl,
                        public Traversal::UnsignedLongDecl,
                        public Traversal::LongLongDecl,
                        public Traversal::UnsignedLongLongDecl,
                        public Traversal::FloatDecl,
                        public Traversal::DoubleDecl,
                        public Traversal::StringDecl,
                        public Traversal::TypedefDecl<StringDecl>,
                        public Traversal::WstringDecl,
                        public Traversal::TypedefDecl<WstringDecl>,
                        public Traversal::ObjectDecl,
                        public Traversal::ValueBaseDecl,
                        public Traversal::AnyDecl,
                        public Traversal::StructDecl,
                        public Traversal::SequenceDecl,
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

  virtual void traverse (VoidDeclPtr const&);
  virtual void traverse (BooleanDeclPtr const&);
  virtual void traverse (OctetDeclPtr const&);
  virtual void traverse (CharDeclPtr const&);
  virtual void traverse (WcharDeclPtr const&);
  virtual void traverse (ShortDeclPtr const&);
  virtual void traverse (UnsignedShortDeclPtr const&);
  virtual void traverse (LongDeclPtr const&);
  virtual void traverse (UnsignedLongDeclPtr const&);
  virtual void traverse (LongLongDeclPtr const&);
  virtual void traverse (UnsignedLongLongDeclPtr const&);
  virtual void traverse (FloatDeclPtr const&);
  virtual void traverse (DoubleDeclPtr const&);
  virtual void traverse (StringDeclPtr const&);
  virtual void traverse (TypedefDeclPtr const&, StringDeclPtr const&);
  virtual void traverse (WstringDeclPtr const&);
  virtual void traverse (TypedefDeclPtr const&, WstringDeclPtr const&);
  virtual void traverse (ObjectDeclPtr const&);
  virtual void traverse (ValueBaseDeclPtr const&);
  virtual void traverse (AnyDeclPtr const&);
  virtual void traverse (StructDeclPtr const&);
  virtual void traverse (SequenceDeclPtr const&);
  virtual void traverse (LocalInterfaceDeclPtr const&);
};

// Generates the typename of an IN argument.
//
//
class INArgTypeNameEmitter : public TypeNameEmitter
{
public:
  INArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanDeclPtr const&);
  virtual void traverse (OctetDeclPtr const&);
  virtual void traverse (CharDeclPtr const&);
  virtual void traverse (WcharDeclPtr const&);
  virtual void traverse (ShortDeclPtr const&);
  virtual void traverse (UnsignedShortDeclPtr const&);
  virtual void traverse (LongDeclPtr const&);
  virtual void traverse (UnsignedLongDeclPtr const&);
  virtual void traverse (LongLongDeclPtr const&);
  virtual void traverse (UnsignedLongLongDeclPtr const&);
  virtual void traverse (FloatDeclPtr const&);
  virtual void traverse (DoubleDeclPtr const&);
  virtual void traverse (StringDeclPtr const&);
  virtual void traverse (TypedefDeclPtr const&, StringDeclPtr const&);
  virtual void traverse (WstringDeclPtr const&);
  virtual void traverse (ObjectDeclPtr const&);
  virtual void traverse (ValueBaseDeclPtr const&);
  virtual void traverse (AnyDeclPtr const&);
  virtual void traverse (StructDeclPtr const&);
  virtual void traverse (SequenceDeclPtr const&);
};

// Generates the typename of an INOUT argument.
//
//
class INOUTArgTypeNameEmitter : public TypeNameEmitter
{
public:
  INOUTArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanDeclPtr const&);
  virtual void traverse (OctetDeclPtr const&);
  virtual void traverse (CharDeclPtr const&);
  virtual void traverse (WcharDeclPtr const&);
  virtual void traverse (ShortDeclPtr const&);
  virtual void traverse (UnsignedShortDeclPtr const&);
  virtual void traverse (LongDeclPtr const&);
  virtual void traverse (UnsignedLongDeclPtr const&);
  virtual void traverse (LongLongDeclPtr const&);
  virtual void traverse (UnsignedLongLongDeclPtr const&);
  virtual void traverse (FloatDeclPtr const&);
  virtual void traverse (DoubleDeclPtr const&);
  virtual void traverse (StringDeclPtr const&);
  virtual void traverse (WstringDeclPtr const&);
  virtual void traverse (ObjectDeclPtr const&);
  virtual void traverse (ValueBaseDeclPtr const&);
  virtual void traverse (AnyDeclPtr const&);
  virtual void traverse (StructDeclPtr const&);
  virtual void traverse (SequenceDeclPtr const&);
};

// Generates the typename of an OUT argument.
//
//
class OUTArgTypeNameEmitter : public TypeNameEmitter
{
public:
  OUTArgTypeNameEmitter (std::ostream&);

  virtual void traverse (BooleanDeclPtr const&);
  virtual void traverse (OctetDeclPtr const&);
  virtual void traverse (CharDeclPtr const&);
  virtual void traverse (WcharDeclPtr const&);
  virtual void traverse (ShortDeclPtr const&);
  virtual void traverse (UnsignedShortDeclPtr const&);
  virtual void traverse (LongDeclPtr const&);
  virtual void traverse (UnsignedLongDeclPtr const&);
  virtual void traverse (LongLongDeclPtr const&);
  virtual void traverse (UnsignedLongLongDeclPtr const&);
  virtual void traverse (FloatDeclPtr const&);
  virtual void traverse (DoubleDeclPtr const&);
  virtual void traverse (StringDeclPtr const&);
  virtual void traverse (WstringDeclPtr const&);
  virtual void traverse (ObjectDeclPtr const&);
  virtual void traverse (ValueBaseDeclPtr const&);
  virtual void traverse (AnyDeclPtr const&);
  virtual void traverse (StructDeclPtr const&);
  virtual void traverse (SequenceDeclPtr const&);
};

#endif /* TYPENAME_EMITTER_HPP */
