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

void
TypeNameEmitter::traverse (Void&)
{
}

void
TypeNameEmitter::traverse (Boolean&)
{
}

void
TypeNameEmitter::traverse (Octet&)
{
}

void
TypeNameEmitter::traverse (Char&)
{
}

void
TypeNameEmitter::traverse (Wchar&)
{
}

void
TypeNameEmitter::traverse (Short&)
{
}

void
TypeNameEmitter::traverse (UnsignedShort&)
{
}

void
TypeNameEmitter::traverse (Long&)
{
}

void
TypeNameEmitter::traverse (UnsignedLong&)
{
}

void
TypeNameEmitter::traverse (LongLong&)
{
}

void
TypeNameEmitter::traverse (UnsignedLongLong&)
{
}

void
TypeNameEmitter::traverse (Float&)
{
}

void
TypeNameEmitter::traverse (Double&)
{
}

void
TypeNameEmitter::traverse (String&)
{
}

void
TypeNameEmitter::traverse (Wstring&)
{
}

void
TypeNameEmitter::traverse (Object&)
{
}

void
TypeNameEmitter::traverse (ValueBase&)
{
}

void
TypeNameEmitter::traverse (Any&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Enum&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Struct&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Union&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Interface&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::ValueType&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::EventType&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Component&)
{
}

void
TypeNameEmitter::traverse (SemanticGraph::Home&)
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

INArgTypeNameEmitter::INArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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

INOUTArgTypeNameEmitter::INOUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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

OUTArgTypeNameEmitter::OUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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

NullReturnEmitter::NullReturnEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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
NullReturnEmitter::traverse (Wstring&)
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
NullReturnEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name () << " retval;" << endl
     << "return retval;";
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
NullReturnEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::Interface& i)
{
  os << "return " << i.scoped_name () << "::_nil ();";
}

void
NullReturnEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << "return 0;";
}

void
NullReturnEmitter::traverse (SemanticGraph::EventType& e)
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

NullRHSEmitter::NullRHSEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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
NullRHSEmitter::traverse (Wstring&)
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
NullRHSEmitter::traverse (SemanticGraph::Enum& e)
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
NullRHSEmitter::traverse (SemanticGraph::UnboundedSequence& s)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::Interface& i)
{
  os << "= " << i.scoped_name () << "::_nil ();";
}

void
NullRHSEmitter::traverse (SemanticGraph::ValueType& v)
{
  os << "= 0;";
}

void
NullRHSEmitter::traverse (SemanticGraph::EventType& e)
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

// ====================================================================

ExtractedTypeDeclEmitter::ExtractedTypeDeclEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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
ExtractedTypeDeclEmitter::traverse (Wstring&)
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
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Enum& e)
{
  os << e.scoped_name () << " " << STRS[EXTRACT] << ";";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Struct& s)
{
  os << s.scoped_name () << " " << STRS[EXTRACT] << ";";
}

void
ExtractedTypeDeclEmitter::traverse (SemanticGraph::Union& u)
{
  os << u.scoped_name () << " " << STRS[EXTRACT] << ";";
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

AssignFromExtractedEmitter::AssignFromExtractedEmitter (ostream& os_)
  : TypeNameEmitter (os_)
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
AssignFromExtractedEmitter::traverse (Wstring&)
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
AssignFromExtractedEmitter::traverse (SemanticGraph::Enum&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Struct&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::Union&)
{
  os << STRS[EXTRACT];
}

void
AssignFromExtractedEmitter::traverse (SemanticGraph::UnboundedSequence&)
{
  os << STRS[EXTRACT];
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

