// file      : CIDLC/TypeNameEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "TypeNameEmitter.hpp"
#include "Literals.hpp"

#include <ostream>

using namespace std;
using namespace StringLiterals;

TypeNameEmitter::TypeNameEmitter (ostream& os_)
  : os (os_)
{
}

// ====================================================================

ReturnTypeNameEmitter::ReturnTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
ReturnTypeNameEmitter::traverse (Void&)
{
  os << "void";
}

void
ReturnTypeNameEmitter::traverse (Boolean&)
{
  os << "::CORBA::Boolean";
}

void
ReturnTypeNameEmitter::traverse (Octet&)
{
  os << "::CORBA::Octet";
}

void
ReturnTypeNameEmitter::traverse (Char&)
{
  os << "::CORBA::Char";
}

void
ReturnTypeNameEmitter::traverse (Wchar&)
{
  os << "::CORBA::WChar";
}

void
ReturnTypeNameEmitter::traverse (Short&)
{
  os << "::CORBA::Short";
}

void
ReturnTypeNameEmitter::traverse (UnsignedShort&)
{
  os << "::CORBA::UShort";
}

void
ReturnTypeNameEmitter::traverse (Long&)
{
  os << "::CORBA::Long";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLong&)
{
  os << "::CORBA::ULong";
}

void
ReturnTypeNameEmitter::traverse (LongLong&)
{
  os << "::CORBA::LongLong";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLongLong&)
{
  os << "::CORBA::ULongLong";
}

void
ReturnTypeNameEmitter::traverse (Float&)
{
  os << "::CORBA::Float";
}

void
ReturnTypeNameEmitter::traverse (Double&)
{
  os << "::CORBA::Double";
}

void
ReturnTypeNameEmitter::traverse (String&)
{
  os << "char *";
}

void
ReturnTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WChar *";
}

void
ReturnTypeNameEmitter::traverse (Object&)
{
  os << "::CORBA::Object_ptr";
}

void
ReturnTypeNameEmitter::traverse (ValueBase&)
{
  os << "::CORBA::ValueBase *";
}

void
ReturnTypeNameEmitter::traverse (Any&)
{
  os << "::CORBA::Any *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Struct& s)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = s.context ().get<bool> (STRS[VAR_SIZE]);
  
  os << s.scoped_name ();
  
  if (var_size)
  {
    os << " *";
  }
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Union& u)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = u.context ().get<bool> (STRS[VAR_SIZE]);
  
  os << u.scoped_name ();
  
  if (var_size)
  {
    os << " *";
  }
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << s.scoped_name () << " *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Interface& i)
{
  os << i.scoped_name () << "_ptr";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << v.scoped_name () << " *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::EventType& e)
{
  os << e.scoped_name () << " *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Component& c)
{
  os << c.scoped_name () << "_ptr";
}

// ===============================================================

INArgTypeNameEmitter::INArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
INArgTypeNameEmitter::traverse (Void&)
{
  // No-op, but the base class method is pure virtual.
}

void
INArgTypeNameEmitter::traverse (Boolean&)
{
  os << "::CORBA::Boolean";
}

void
INArgTypeNameEmitter::traverse (Octet&)
{
  os << "::CORBA::Octet";
}

void
INArgTypeNameEmitter::traverse (Char&)
{
  os << "::CORBA::Char";
}

void
INArgTypeNameEmitter::traverse (Wchar&)
{
  os << "::CORBA::WChar";
}

void
INArgTypeNameEmitter::traverse (Short&)
{
  os << "::CORBA::Short";
}

void
INArgTypeNameEmitter::traverse (UnsignedShort&)
{
  os << "::CORBA::UShort";
}

void
INArgTypeNameEmitter::traverse (Long&)
{
  os << "::CORBA::Long";
}

void
INArgTypeNameEmitter::traverse (UnsignedLong&)
{
  os << "::CORBA::ULong";
}

void
INArgTypeNameEmitter::traverse (LongLong&)
{
  os << "::CORBA::LongLong";
}

void
INArgTypeNameEmitter::traverse (UnsignedLongLong&)
{
  os << "::CORBA::ULongLong";
}

void
INArgTypeNameEmitter::traverse (Float&)
{
  os << "::CORBA::Float";
}

void
INArgTypeNameEmitter::traverse (Double&)
{
  os << "::CORBA::Double";
}

void
INArgTypeNameEmitter::traverse (String&)
{
  os << "const char *";
}

void
INArgTypeNameEmitter::traverse (Wstring&)
{
  os << "const ::CORBA::WChar *";
}

void
INArgTypeNameEmitter::traverse (Object&)
{
  os << "::CORBA::Object_ptr";
}

void
INArgTypeNameEmitter::traverse (ValueBase&)
{
  os << "::CORBA::ValueBase *";
}

void
INArgTypeNameEmitter::traverse (Any&)
{
  os << "const ::CORBA::Any &";
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::Struct& s)
{
  os << "const " << s.scoped_name () << " &";;
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::Union& u)
{
  os << "const " << u.scoped_name () << " &";;
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << "const " << s.scoped_name () << " &";;
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::Interface& i)
{
  os << i.scoped_name () << "_ptr";
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << v.scoped_name () << " *";
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::EventType& e)
{
  os << e.scoped_name () << " *";
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::Component& c)
{
  os << c.scoped_name () << "_ptr";
}

// ===============================================================

INOUTArgTypeNameEmitter::INOUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
INOUTArgTypeNameEmitter::traverse (Void&)
{
  // No-op, but the base class method is pure virtual.
}

void
INOUTArgTypeNameEmitter::traverse (Boolean&)
{
  os << "::CORBA::Boolean &";
}

void
INOUTArgTypeNameEmitter::traverse (Octet&)
{
  os << "::CORBA::Octet &";
}

void
INOUTArgTypeNameEmitter::traverse (Char&)
{
  os << "::CORBA::Char &";
}

void
INOUTArgTypeNameEmitter::traverse (Wchar&)
{
  os << "::CORBA::WChar &";
}

void
INOUTArgTypeNameEmitter::traverse (Short&)
{
  os << "::CORBA::Short &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedShort&)
{
  os << "::CORBA::UShort &";
}

void
INOUTArgTypeNameEmitter::traverse (Long&)
{
  os << "::CORBA::Long &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLong&)
{
  os << "::CORBA::ULong &";
}

void
INOUTArgTypeNameEmitter::traverse (LongLong&)
{
  os << "::CORBA::LongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLongLong&)
{
  os << "::CORBA::ULongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (Float&)
{
  os << "::CORBA::Float &";
}

void
INOUTArgTypeNameEmitter::traverse (Double&)
{
  os << "::CORBA::Double &";
}

void
INOUTArgTypeNameEmitter::traverse (String&)
{
  os << "char *&";
}

void
INOUTArgTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WChar *&";
}

void
INOUTArgTypeNameEmitter::traverse (Object&)
{
  os << "::CORBA::Object_ptr &";
}

void
INOUTArgTypeNameEmitter::traverse (ValueBase&)
{
  os << "::CORBA::ValueBase *&";
}

void
INOUTArgTypeNameEmitter::traverse (Any&)
{
  os << "::CORBA::Any &";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Struct& s)
{
  os << s.scoped_name () << " &";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Union& u)
{
  os << u.scoped_name () << " &";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << s.scoped_name () << " &";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Interface& i)
{
  os << i.scoped_name () << "_ptr &";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << v.scoped_name () << " *&";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::EventType& e)
{
  os << e.scoped_name () << " *&";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Component& c)
{
  os << c.scoped_name () << "_ptr &";
}

// =================================================================

OUTArgTypeNameEmitter::OUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
OUTArgTypeNameEmitter::traverse (Void&)
{
  // No-op, but the base class method is pure virtual.
}

void
OUTArgTypeNameEmitter::traverse (Boolean&)
{
  os << "::CORBA::Boolean_out";
}

void
OUTArgTypeNameEmitter::traverse (Octet&)
{
  os << "::CORBA::Octet_out";
}

void
OUTArgTypeNameEmitter::traverse (Char&)
{
  os << "::CORBA::Char_out";
}

void
OUTArgTypeNameEmitter::traverse (Wchar&)
{
  os << "::CORBA::WChar_out";
}

void
OUTArgTypeNameEmitter::traverse (Short&)
{
  os << "::CORBA::Short_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedShort&)
{
  os << "::CORBA::UShort_out";
}

void
OUTArgTypeNameEmitter::traverse (Long&)
{
  os << "::CORBA::Long_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLong&)
{
  os << "::CORBA::ULong_out";
}

void
OUTArgTypeNameEmitter::traverse (LongLong&)
{
  os << "::CORBA::LongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLongLong&)
{
  os << "::CORBA::ULongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (Float&)
{
  os << "::CORBA::Float_out";
}

void
OUTArgTypeNameEmitter::traverse (Double&)
{
  os << "::CORBA::Double_out";
}

void
OUTArgTypeNameEmitter::traverse (String&)
{
  os << "::CORBA::String_out";
}

void
OUTArgTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WString_out";
}

void
OUTArgTypeNameEmitter::traverse (Object&)
{
  os << "::CORBA::Object_out";
}

void
OUTArgTypeNameEmitter::traverse (ValueBase&)
{
  os << "::CORBA::ValueBase_out";
}

void
OUTArgTypeNameEmitter::traverse (Any&)
{
  os << "::CORBA::Any_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Struct& s)
{
  os << s.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Union& u)
{
  os << u.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << s.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Interface& i)
{
  os << i.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << v.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::EventType& e)
{
  os << e.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Component& c)
{
  os << c.scoped_name () << "_out";
}


