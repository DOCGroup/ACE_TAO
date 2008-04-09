// file      : CIDLC/FullTypeNameEmitter.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TYPENAME_EMITTER_HPP
#define TYPENAME_EMITTER_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "EmitterBase.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

// Base class that holds the ostream member
// common to every other class in this file,
// and collects the ancestors in one place.
//
// Derived classes encapsulate the CORBA rules for
// the C++ mapping of different type kinds.
class CorbaTypeNameEmitter : public EmitterBase,
                             public Traversal::Void,
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
                             public Traversal::BoundedString,
                             public Traversal::Wstring,
                             public Traversal::BoundedWideString,
                             public Traversal::Object,
                             public Traversal::ValueBase,
                             public Traversal::Any,
                             public Traversal::Array,
                             public Traversal::Enum,
                             public Traversal::Struct,
                             public Traversal::Union,
                             public Traversal::BoundedSequence,
                             public Traversal::UnboundedSequence,
                             public Traversal::Interface,
                             public Traversal::ValueType,
                             public Traversal::EventType,
                             public Traversal::Component,
                             public Traversal::Home
{
public:
  virtual void traverse (Void&);
  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);

protected:
  CorbaTypeNameEmitter (Context&);
};

// Generates the name of an operation's return type.
//
//
class ReturnTypeNameEmitter : public CorbaTypeNameEmitter
{
public:
  ReturnTypeNameEmitter (Context&);

  virtual void traverse (Void&);
  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

// Generates the typename of an IN argument.
//
//
class INArgTypeNameEmitter : public CorbaTypeNameEmitter
{
public:
  INArgTypeNameEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

// Generates the typename of an INOUT argument.
//
//
class INOUTArgTypeNameEmitter : public CorbaTypeNameEmitter
{
public:
  INOUTArgTypeNameEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

// Generates the typename of an OUT argument.
//
//
class OUTArgTypeNameEmitter : public CorbaTypeNameEmitter
{
public:
  OUTArgTypeNameEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

// Generates 'return' plus the appropriate NULL return value,
// including no output for Void.
//
//
class NullReturnEmitter : public CorbaTypeNameEmitter
{
public:
  NullReturnEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

/* Not currently used, but it was at one time and may be useful again.

// Generates the appropriate NULL value, including the assignment operator,
// if needed, for the rhs of an assignment statement.
//
//
class NullRHSEmitter : public CorbaTypeNameEmitter
{
public:
  NullRHSEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};
*/
// Declares and initializes a variable to use for any extraction.
//
//
class ExtractedTypeDeclEmitter : public CorbaTypeNameEmitter
{
public:
  ExtractedTypeDeclEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

// Declares and initializes a variable to use for any extraction.
//
//
class AssignFromExtractedEmitter : public CorbaTypeNameEmitter
{
public:
  AssignFromExtractedEmitter (Context&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);
};

class SetAttributeDelegationEmitter : public CorbaTypeNameEmitter,
                                      public Traversal::ReadWriteAttribute
{
public:
  SetAttributeDelegationEmitter (Context&,
                                 SemanticGraph::ReadWriteAttribute&);

  virtual void traverse (Boolean&);
  virtual void traverse (Octet&);
  virtual void traverse (Char&);
  virtual void traverse (Wchar&);
  virtual void traverse (Short&);
  virtual void traverse (UnsignedShort&);
  virtual void traverse (Long&);
  virtual void traverse (UnsignedLong&);
  virtual void traverse (LongLong&);
  virtual void traverse (UnsignedLongLong&);
  virtual void traverse (Float&);
  virtual void traverse (Double&);
  virtual void traverse (String&);
  virtual void traverse (SemanticGraph::BoundedString&);
  virtual void traverse (Wstring&);
  virtual void traverse (SemanticGraph::BoundedWideString&);
  virtual void traverse (Object&);
  virtual void traverse (ValueBase&);
  virtual void traverse (Any&);
  virtual void traverse (SemanticGraph::Array&);
  virtual void traverse (SemanticGraph::Enum&);
  virtual void traverse (SemanticGraph::Struct&);
  virtual void traverse (SemanticGraph::Union&);
  virtual void traverse (SemanticGraph::BoundedSequence&);
  virtual void traverse (SemanticGraph::UnboundedSequence&);
  virtual void traverse (SemanticGraph::Interface&);
  virtual void traverse (SemanticGraph::ValueType&);
  virtual void traverse (SemanticGraph::EventType&);
  virtual void traverse (SemanticGraph::Component&);
  virtual void traverse (SemanticGraph::Home&);

private:
  void emit_delegation (void);
  void emit_error (const char *);
  void open_if_block (void);
  void close_if_block (void);

private:
  SemanticGraph::ReadWriteAttribute& attr_;
  AssignFromExtractedEmitter assign_emitter_;
  ExtractedTypeDeclEmitter extract_emitter_;
  Traversal::Belongs assign_belongs_;
  Traversal::Belongs extract_belongs_;
};

#endif /* TYPENAME_EMITTER_HPP */
