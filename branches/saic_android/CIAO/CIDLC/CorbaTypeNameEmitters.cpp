// file      : CIDLC/CorbaTypeNameEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "CorbaTypeNameEmitters.hpp"
#include "Literals.hpp"

#include <ostream>

using namespace std;
using namespace StringLiterals;

CorbaTypeNameEmitter::CorbaTypeNameEmitter (Context& c)
  : EmitterBase (c)
{
}

void
CorbaTypeNameEmitter::traverse (Void&)
{
}

void
CorbaTypeNameEmitter::traverse (Boolean&)
{
}

void
CorbaTypeNameEmitter::traverse (Octet&)
{
}

void
CorbaTypeNameEmitter::traverse (Char&)
{
}

void
CorbaTypeNameEmitter::traverse (Wchar&)
{
}

void
CorbaTypeNameEmitter::traverse (Short&)
{
}

void
CorbaTypeNameEmitter::traverse (UnsignedShort&)
{
}

void
CorbaTypeNameEmitter::traverse (Long&)
{
}

void
CorbaTypeNameEmitter::traverse (UnsignedLong&)
{
}

void
CorbaTypeNameEmitter::traverse (LongLong&)
{
}

void
CorbaTypeNameEmitter::traverse (UnsignedLongLong&)
{
}

void
CorbaTypeNameEmitter::traverse (Float&)
{
}

void
CorbaTypeNameEmitter::traverse (Double&)
{
}

void
CorbaTypeNameEmitter::traverse (String&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::BoundedString&)
{
}

void
CorbaTypeNameEmitter::traverse (Wstring&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::BoundedWideString&)
{
}

void
CorbaTypeNameEmitter::traverse (Object&)
{
}

void
CorbaTypeNameEmitter::traverse (ValueBase&)
{
}

void
CorbaTypeNameEmitter::traverse (Any&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Array&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Enum&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Struct&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Union&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::BoundedSequence&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Interface&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::ValueType&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::EventType&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Component&)
{
}

void
CorbaTypeNameEmitter::traverse (SemanticGraph::Home&)
{
}

// ====================================================================

ReturnTypeNameEmitter::ReturnTypeNameEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
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
ReturnTypeNameEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "char *";
}

void
ReturnTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WChar *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::BoundedWideString&)
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
ReturnTypeNameEmitter::traverse (SemanticGraph::Array& a)
{
  os << a.scoped_name () << "_slice *";
}

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name ();
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
ReturnTypeNameEmitter::traverse (SemanticGraph::BoundedSequence& s)
{
  os << s.scoped_name () << " *";
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

void
ReturnTypeNameEmitter::traverse (SemanticGraph::Home& h)
{
  os << h.scoped_name () << "_ptr";
}

// ===============================================================

INArgTypeNameEmitter::INArgTypeNameEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
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
INArgTypeNameEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "const char *";
}

void
INArgTypeNameEmitter::traverse (Wstring&)
{
  os << "const ::CORBA::WChar *";
}

void
INArgTypeNameEmitter::traverse (SemanticGraph::BoundedWideString&)
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
INArgTypeNameEmitter::traverse (SemanticGraph::Array& a)
{
  os << "const " << a.scoped_name ();
}
void
INArgTypeNameEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name ();
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
INArgTypeNameEmitter::traverse (SemanticGraph::BoundedSequence& s)
{
  os << "const " << s.scoped_name () << " &";;
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

void
INArgTypeNameEmitter::traverse (SemanticGraph::Home& h)
{
  os << h.scoped_name () << "_ptr";
}

// ===============================================================

INOUTArgTypeNameEmitter::INOUTArgTypeNameEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
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
INOUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "char *&";
}

void
INOUTArgTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WChar *&";
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedWideString&)
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
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Array& a)
{
  os << a.scoped_name ();
}

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name () << " &";
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
INOUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedSequence& s)
{
  os << s.scoped_name () << " &";
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

void
INOUTArgTypeNameEmitter::traverse (SemanticGraph::Home& h)
{
  os << h.scoped_name () << "_ptr";
}

// =================================================================

OUTArgTypeNameEmitter::OUTArgTypeNameEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
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
OUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedString& s)
{
  os << s.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (Wstring&)
{
  os << "::CORBA::WString_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedWideString& s)
{
  os << s.scoped_name () << "_out";
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
OUTArgTypeNameEmitter::traverse (SemanticGraph::Array& a)
{
  os << a.scoped_name () << "_out";
}

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name () << "_out";
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
OUTArgTypeNameEmitter::traverse (SemanticGraph::BoundedSequence& s)
{
  os << s.scoped_name () << "_out";
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

void
OUTArgTypeNameEmitter::traverse (SemanticGraph::Home& h)
{
  os << h.scoped_name () << "_ptr";
}

// ====================================================================

NullReturnEmitter::NullReturnEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
}

void
NullReturnEmitter::traverse (Boolean&)
{
  os << "return false;";
}

void
NullReturnEmitter::traverse (Octet&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Char&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Wchar&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Short&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (UnsignedShort&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Long&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (UnsignedLong&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (LongLong&)
{
  os << "return ACE_CDR_LONGLONG_INITIALIZER;";
}

void
NullReturnEmitter::traverse (UnsignedLongLong&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Float&)
{
  os << "return 0.0f;";
}

void
NullReturnEmitter::traverse (Double&)
{
  os << "return 0.0;";
}

void
NullReturnEmitter::traverse (String&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Wstring&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::BoundedWideString&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Object&)
{
  os << "return ::CORBA::Object::_nil ();";
}

void
NullReturnEmitter::traverse (ValueBase&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (Any&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::Array&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::Enum& e)
{
  os << "return static_cast< " << e.scoped_name () << "> (0UL);";
}

void
NullReturnEmitter::traverse (SemanticGraph::Struct& s)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = s.context ().get<bool> (STRS[VAR_SIZE]);

  if (var_size)
  {
    os << "return 0;";
  }
  else
  {
    os << s.scoped_name () << " retval;" << endl
       << "return retval;";
  }
}

void
NullReturnEmitter::traverse (SemanticGraph::Union& u)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = u.context ().get<bool> (STRS[VAR_SIZE]);

  if (var_size)
  {
    os << "return 0;";
  }
  else
  {
    os << u.scoped_name () << " retval;" << endl
       << "return retval;";
  }
}

void
NullReturnEmitter::traverse (SemanticGraph::BoundedSequence&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::Interface& i)
{
  os << "return " << i.scoped_name () << "::_nil ();";
}

void
NullReturnEmitter::traverse (SemanticGraph::ValueType&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::EventType&)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::Component& c)
{
  os << "return " << c.scoped_name () << "::_nil ();";
}

void
NullReturnEmitter::traverse (SemanticGraph::Home& h)
{
  os << "return " << h.scoped_name () << "::_nil ();";
}

// ====================================================================
/* Not currently used, but it was at one time and may be useful again.

NullRHSEmitter::NullRHSEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
}

void
NullRHSEmitter::traverse (Boolean&)
{
  os << "= false;";
}

void
NullRHSEmitter::traverse (Octet&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Char&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Wchar&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Short&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (UnsignedShort&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Long&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (UnsignedLong&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (LongLong&)
{
  os << "= ACE_CDR_LONGLONG_INITIALIZER;";
}

void
NullRHSEmitter::traverse (UnsignedLongLong&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Float&)
{
  os << "= 0.0f;";
}

void
NullRHSEmitter::traverse (Double&)
{
  os << "= 0.0;";
}

void
NullRHSEmitter::traverse (String&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Wstring&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::BoundedWideString&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Object&)
{
  os << "= ::CORBA::Object::_nil ();";
}

void
NullRHSEmitter::traverse (ValueBase&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (Any&)
{
  // This will hopefully work both if the lhs is a pointer (for example
  // RetunrTypeNameEmitter) or not (for example INArgTypeNameEmitter).
  os << ";";
}

void
NullRHSEmitter::traverse (SemanticGraph::Array&)
{
  // This will hopefully work both if the lhs is a slice pointer (for example
  // RetunrTypeNameEmitter) or not (for example INArgTypeNameEmitter).
  os << ";";
}

void
NullRHSEmitter::traverse (SemanticGraph::Enum&)
{
  os << ";";
}

void
NullRHSEmitter::traverse (SemanticGraph::Struct& s)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = s.context ().get<bool> (STRS[VAR_SIZE]);

  if (var_size)
  {
    os << "= 0;";
  }
  else
  {
    os << ";";
  }
}

void
NullRHSEmitter::traverse (SemanticGraph::Union& u)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = u.context ().get<bool> (STRS[VAR_SIZE]);

  if (var_size)
  {
    os << "= 0;";
  }
  else
  {
    os << ";";
  }
}

void
NullRHSEmitter::traverse (SemanticGraph::BoundedSequence&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::Interface& i)
{
  os << "= " << i.scoped_name () << "::_nil ();";
}

void
NullRHSEmitter::traverse (SemanticGraph::ValueType&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::EventType&)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::Component& c)
{
  os << "= " << c.scoped_name () << "::_nil ();";
}

void
NullRHSEmitter::traverse (SemanticGraph::Home& h)
{
  os << "= " << h.scoped_name () << "::_nil ();";
}
*/
// ====================================================================

ExtractedTypeDeclEmitter::ExtractedTypeDeclEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
}

void
ExtractedTypeDeclEmitter::traverse (Boolean&)
{
  os << "::CORBA::Boolean " << STRS[EXTRACT_TMP] << " = false;"
     << "::CORBA::Any::to_boolean " << STRS[EXTRACT]
     << " (" << STRS[EXTRACT_TMP] << ");";
}

void
ExtractedTypeDeclEmitter::traverse (Octet&)
{
  os << "::CORBA::Octet " << STRS[EXTRACT_TMP] << " = 0;"
     << "::CORBA::Any::to_octet " << STRS[EXTRACT]
     << " (" << STRS[EXTRACT_TMP] << ");";
}

void
ExtractedTypeDeclEmitter::traverse (Char&)
{
  os << "::CORBA::Char " << STRS[EXTRACT_TMP] << " = 0;"
     << "::CORBA::Any::to_char " << STRS[EXTRACT]
     << " (" << STRS[EXTRACT_TMP] << ");";
}

void
ExtractedTypeDeclEmitter::traverse (Wchar&)
{
  os << "::CORBA::WChar " << STRS[EXTRACT_TMP] << " = 0;"
     << "::CORBA::Any::to_wchar " << STRS[EXTRACT]
     << " (" << STRS[EXTRACT_TMP] << ");";
}

void
ExtractedTypeDeclEmitter::traverse (Short&)
{
  os << "::CORBA::Short " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (UnsignedShort&)
{
  os << "::CORBA::UShort " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Long&)
{
  os << "::CORBA::Long " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (UnsignedLong&)
{
  os << "::CORBA::ULong " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (LongLong&)
{
  os << "::CORBA::LongLong " << STRS[EXTRACT]
     << " = ACE_CDR_LONGLONG_INITIALIZER;";
}

void
ExtractedTypeDeclEmitter::traverse (UnsignedLongLong&)
{
  os << "::CORBA::ULongLong " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Float&)
{
  os << "::CORBA::Float " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Double&)
{
  os << "::CORBA::Double " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (String&)
{
  os << "const char * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << "const char * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Wstring&)
{
  os << "::CORBA::WChar * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::BoundedWideString&)
{
  os << "::CORBA::WChar * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Object&)
{
  os << "::CORBA::Object_ptr " << STRS[EXTRACT]
     << " = ::CORBA::Object::_nil ();";
}

void
ExtractedTypeDeclEmitter::traverse (ValueBase&)
{
  os << "::CORBA::ValueBase * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (Any&)
{
  os << "::CORBA::Any * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Array& a)
{
  os << a.scoped_name () << "_forany & " << STRS[EXTRACT] << ";";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name () << " " << STRS[EXTRACT] << ";";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Struct& s)
{
  os << s.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Union& u)
{
  os << u.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::BoundedSequence& s)
{
  os << s.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << s.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Interface& i)
{
  os << i.scoped_name () << "_ptr " << STRS[EXTRACT] << " = "
     << i.scoped_name () << "::_nil ();";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << v.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::EventType& e)
{
  os << e.scoped_name () << " * " << STRS[EXTRACT] << " = 0;";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Component& c)
{
  os << c.scoped_name () << "_ptr " << STRS[EXTRACT] << " = "
     << c.scoped_name () << "::_nil ();";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Home& h)
{
  os << h.scoped_name () << "_ptr " << STRS[EXTRACT] << " = "
     << h.scoped_name () << "::_nil ();";
}

// ====================================================================

AssignFromExtractedEmitter::AssignFromExtractedEmitter (Context& c)
  : CorbaTypeNameEmitter (c)
{
}

void
AssignFromExtractedEmitter::traverse (Boolean&)
{
  os << STRS[EXTRACT] << ".ref_";
}

void
AssignFromExtractedEmitter::traverse (Octet&)
{
  os << STRS[EXTRACT] << ".ref_";
}

void
AssignFromExtractedEmitter::traverse (Char&)
{
  os << STRS[EXTRACT] << ".ref_";
}

void
AssignFromExtractedEmitter::traverse (Wchar&)
{
  os << STRS[EXTRACT] << ".ref_";
}

void
AssignFromExtractedEmitter::traverse (Short&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (UnsignedShort&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Long&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (UnsignedLong&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (LongLong&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (UnsignedLongLong&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Float&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Double&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (String&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::BoundedString&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Wstring&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::BoundedWideString&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Object&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (ValueBase&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (Any&)
{
  os << "*" << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Array&)
{
  os << STRS[EXTRACT] << ".in ()";
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Enum&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Struct& s)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = s.context ().get<bool> (STRS[VAR_SIZE]);

  if (!var_size)
  {
    os << "*";
  }

  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Union& u)
{
  // This should always be in the context, since the SizeTypeCalculator
  // is executed before the servant code generators.
  bool var_size = u.context ().get<bool> (STRS[VAR_SIZE]);

  if (!var_size)
  {
    os << "*";
  }

  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::BoundedSequence&)
{
  os << "*" << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
  os << "*" << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Interface&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::ValueType&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::EventType&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Component&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Home&)
{
  os << STRS[EXTRACT];
}

// ====================================================================

SetAttributeDelegationEmitter::SetAttributeDelegationEmitter (
    Context& c,
    SemanticGraph::ReadWriteAttribute& a
  )
  : CorbaTypeNameEmitter (c),
    attr_ (a)
    , assign_emitter_ (c)
    , extract_emitter_ (c)
{
  assign_belongs_.node_traverser (assign_emitter_);
  extract_belongs_.node_traverser (extract_emitter_);
}

void
SetAttributeDelegationEmitter::traverse (Boolean&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Octet&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Char&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Wchar&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Short&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (UnsignedShort&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Long&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (UnsignedLong&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (LongLong&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (UnsignedLongLong&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Float&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (Double&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (String&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::BoundedString&)
{
  emit_error ("bounded string");
}

void
SetAttributeDelegationEmitter::traverse (Wstring&)
{
  emit_error ("wstring");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::BoundedWideString&)
{
  emit_error ("bounded wstring");
}

void
SetAttributeDelegationEmitter::traverse (Object&)
{
  emit_error ("CORBA::Object");
}

void
SetAttributeDelegationEmitter::traverse (ValueBase&)
{
  emit_error ("CORBA::ValueBase");
}

void
SetAttributeDelegationEmitter::traverse (Any&)
{
  emit_error ("CORBA::Any");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Array&)
{
  emit_error ("array");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Enum&)
{
  emit_delegation ();
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Struct&)
{
  emit_error ("struct");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Union&)
{
  emit_error ("union");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::BoundedSequence&)
{
  emit_error ("bounded sequence");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
  emit_error ("unbounded sequence");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Interface&)
{
  emit_error ("interface");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::ValueType&)
{
  emit_error ("valuetype");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::EventType&)
{
  emit_error ("eventtype");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Component&)
{
  emit_error ("component");
}

void
SetAttributeDelegationEmitter::traverse (SemanticGraph::Home&)
{
  emit_error ("home");
}

void
SetAttributeDelegationEmitter::emit_delegation (void)
{
  open_if_block ();

  Traversal::ReadWriteAttribute::belongs (attr_, extract_belongs_);

  os << "descr_value >>= " << STRS[EXTRACT] << ";"
     << "this->" << attr_.name () << " (";

  Traversal::ReadWriteAttribute::belongs (attr_, assign_belongs_);

  os << ");";

  close_if_block ();
}

void
SetAttributeDelegationEmitter::emit_error (const char *corba_kind)
{
  open_if_block ();

  os << "ACE_ERROR ((" << endl
     << "    LM_ERROR," << endl
     << "    \"Component attributes of " << corba_kind << "\"" << endl
     << "    \"IDL type are not yet supported by CIAO\\n\"" << endl
     << "  ));";

  close_if_block ();
}

void
SetAttributeDelegationEmitter::open_if_block (void)
{
  os << "if (ACE_OS::strcmp (descr_name, \""
     << attr_.name ().unescaped_str () << "\") == 0)" << endl
     << "{";
}

void
SetAttributeDelegationEmitter::close_if_block (void)
{
  os << "continue;"
     << "}";
}
