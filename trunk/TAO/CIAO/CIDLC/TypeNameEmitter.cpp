// file      : CIDLC/TypeNameEmitter.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "TypeNameEmitter.hpp"

#include <ostream>

using namespace std;

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
ReturnTypeNameEmitter::traverse (VoidDeclPtr const&)
{
  os << "void";
}

void
ReturnTypeNameEmitter::traverse (BooleanDeclPtr const&)
{
  os << "::CORBA::Boolean";
}

void
ReturnTypeNameEmitter::traverse (OctetDeclPtr const&)
{
  os << "::CORBA::Octet";
}

void
ReturnTypeNameEmitter::traverse (CharDeclPtr const&)
{
  os << "::CORBA::Char";
}

void
ReturnTypeNameEmitter::traverse (WcharDeclPtr const&)
{
  os << "::CORBA::WChar";
}

void
ReturnTypeNameEmitter::traverse (ShortDeclPtr const&)
{
  os << "::CORBA::Short";
}

void
ReturnTypeNameEmitter::traverse (UnsignedShortDeclPtr const&)
{
  os << "::CORBA::UShort";
}

void
ReturnTypeNameEmitter::traverse (LongDeclPtr const&)
{
  os << "::CORBA::Long";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLongDeclPtr const&)
{
  os << "::CORBA::ULong";
}

void
ReturnTypeNameEmitter::traverse (LongLongDeclPtr const&)
{
  os << "::CORBA::LongLong";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLongLongDeclPtr const&)
{
  os << "::CORBA::ULongLong";
}

void
ReturnTypeNameEmitter::traverse (FloatDeclPtr const&)
{
  os << "::CORBA::Float";
}

void
ReturnTypeNameEmitter::traverse (DoubleDeclPtr const&)
{
  os << "::CORBA::Double";
}

void
ReturnTypeNameEmitter::traverse (StringDeclPtr const&)
{
  os << "char *";
}

void
ReturnTypeNameEmitter::traverse (TypedefDeclPtr const &, StringDeclPtr const&)
{
  os << "char *";
}

void
ReturnTypeNameEmitter::traverse (WstringDeclPtr const&)
{
  os << "::CORBA::WChar *";
}

void
ReturnTypeNameEmitter::traverse (TypedefDeclPtr const &, WstringDeclPtr const&)
{
  os << "::CORBA::WChar *";
}

void
ReturnTypeNameEmitter::traverse (ObjectDeclPtr const&)
{
  os << "::CORBA::Object_ptr";
}

void
ReturnTypeNameEmitter::traverse (ValueBaseDeclPtr const&)
{
  os << "::CORBA::ValueBase *";
}

void
ReturnTypeNameEmitter::traverse (AnyDeclPtr const&)
{
  os << "::CORBA::Any *";
}

void
ReturnTypeNameEmitter::traverse (StructDeclPtr const&)
{
  //os << "::CORBA::Any *";
}

void
ReturnTypeNameEmitter::traverse (SequenceDeclPtr const& s)
{
  os << s->name () << " *";
}

void
ReturnTypeNameEmitter::traverse (LocalInterfaceDeclPtr const& i)
{
  os << i->name () << "_ptr";
}

// ===============================================================

INArgTypeNameEmitter::INArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
INArgTypeNameEmitter::traverse (BooleanDeclPtr const&)
{
  os << "::CORBA::Boolean";
}

void
INArgTypeNameEmitter::traverse (OctetDeclPtr const&)
{
  os << "::CORBA::Octet";
}

void
INArgTypeNameEmitter::traverse (CharDeclPtr const&)
{
  os << "::CORBA::Char";
}

void
INArgTypeNameEmitter::traverse (WcharDeclPtr const&)
{
  os << "::CORBA::WChar";
}

void
INArgTypeNameEmitter::traverse (ShortDeclPtr const&)
{
  os << "::CORBA::Short";
}

void
INArgTypeNameEmitter::traverse (UnsignedShortDeclPtr const&)
{
  os << "::CORBA::UShort";
}

void
INArgTypeNameEmitter::traverse (LongDeclPtr const&)
{
  os << "::CORBA::Long";
}

void
INArgTypeNameEmitter::traverse (UnsignedLongDeclPtr const&)
{
  os << "::CORBA::ULong";
}

void
INArgTypeNameEmitter::traverse (LongLongDeclPtr const&)
{
  os << "::CORBA::LongLong";
}

void
INArgTypeNameEmitter::traverse (UnsignedLongLongDeclPtr const&)
{
  os << "::CORBA::ULongLong";
}

void
INArgTypeNameEmitter::traverse (FloatDeclPtr const&)
{
  os << "::CORBA::Float";
}

void
INArgTypeNameEmitter::traverse (DoubleDeclPtr const&)
{
  os << "::CORBA::Double";
}

void
INArgTypeNameEmitter::traverse (StringDeclPtr const&)
{
  os << "const char *";
}

void
INArgTypeNameEmitter::traverse (WstringDeclPtr const&)
{
  os << "const ::CORBA::WChar *";
}

void
INArgTypeNameEmitter::traverse (ObjectDeclPtr const&)
{
  os << "::CORBA::Object_ptr";
}

void
INArgTypeNameEmitter::traverse (ValueBaseDeclPtr const&)
{
  os << "::CORBA::ValueBase *";
}

void
INArgTypeNameEmitter::traverse (AnyDeclPtr const&)
{
  os << "const ::CORBA::Any &";
}

void
INArgTypeNameEmitter::traverse (StructDeclPtr const&)
{
  //os << "::CORBA::Any *";
}

void
INArgTypeNameEmitter::traverse (SequenceDeclPtr const& s)
{
  os << "const " << s->name () << " &";;
}

// ===============================================================

INOUTArgTypeNameEmitter::INOUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
INOUTArgTypeNameEmitter::traverse (BooleanDeclPtr const&)
{
  os << "::CORBA::Boolean &";
}

void
INOUTArgTypeNameEmitter::traverse (OctetDeclPtr const&)
{
  os << "::CORBA::Octet &";
}

void
INOUTArgTypeNameEmitter::traverse (CharDeclPtr const&)
{
  os << "::CORBA::Char &";
}

void
INOUTArgTypeNameEmitter::traverse (WcharDeclPtr const&)
{
  os << "::CORBA::WChar &";
}

void
INOUTArgTypeNameEmitter::traverse (ShortDeclPtr const&)
{
  os << "::CORBA::Short &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedShortDeclPtr const&)
{
  os << "::CORBA::UShort &";
}

void
INOUTArgTypeNameEmitter::traverse (LongDeclPtr const&)
{
  os << "::CORBA::Long &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLongDeclPtr const&)
{
  os << "::CORBA::ULong &";
}

void
INOUTArgTypeNameEmitter::traverse (LongLongDeclPtr const&)
{
  os << "::CORBA::LongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLongLongDeclPtr const&)
{
  os << "::CORBA::ULongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (FloatDeclPtr const&)
{
  os << "::CORBA::Float &";
}

void
INOUTArgTypeNameEmitter::traverse (DoubleDeclPtr const&)
{
  os << "::CORBA::Double &";
}

void
INOUTArgTypeNameEmitter::traverse (StringDeclPtr const&)
{
  os << "char *&";
}

void
INOUTArgTypeNameEmitter::traverse (WstringDeclPtr const&)
{
  os << "::CORBA::WChar *&";
}

void
INOUTArgTypeNameEmitter::traverse (ObjectDeclPtr const&)
{
  os << "::CORBA::Object_ptr &";
}

void
INOUTArgTypeNameEmitter::traverse (ValueBaseDeclPtr const&)
{
  os << "::CORBA::ValueBase *&";
}

void
INOUTArgTypeNameEmitter::traverse (AnyDeclPtr const&)
{
  os << "::CORBA::Any &";
}

void
INOUTArgTypeNameEmitter::traverse (StructDeclPtr const&)
{
  //os << "::CORBA::Any *";
}

void
INOUTArgTypeNameEmitter::traverse (SequenceDeclPtr const& s)
{
  os << s->name () << " &";
}

// =================================================================

OUTArgTypeNameEmitter::OUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
OUTArgTypeNameEmitter::traverse (BooleanDeclPtr const&)
{
  os << "::CORBA::Boolean_out";
}

void
OUTArgTypeNameEmitter::traverse (OctetDeclPtr const&)
{
  os << "::CORBA::Octet_out";
}

void
OUTArgTypeNameEmitter::traverse (CharDeclPtr const&)
{
  os << "::CORBA::Char_out";
}

void
OUTArgTypeNameEmitter::traverse (WcharDeclPtr const&)
{
  os << "::CORBA::WChar_out";
}

void
OUTArgTypeNameEmitter::traverse (ShortDeclPtr const&)
{
  os << "::CORBA::Short_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedShortDeclPtr const&)
{
  os << "::CORBA::UShort_out";
}

void
OUTArgTypeNameEmitter::traverse (LongDeclPtr const&)
{
  os << "::CORBA::Long_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLongDeclPtr const&)
{
  os << "::CORBA::ULong_out";
}

void
OUTArgTypeNameEmitter::traverse (LongLongDeclPtr const&)
{
  os << "::CORBA::LongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLongLongDeclPtr const&)
{
  os << "::CORBA::ULongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (FloatDeclPtr const&)
{
  os << "::CORBA::Float_out";
}

void
OUTArgTypeNameEmitter::traverse (DoubleDeclPtr const&)
{
  os << "::CORBA::Double_out";
}

void
OUTArgTypeNameEmitter::traverse (StringDeclPtr const&)
{
  os << "::CORBA::String_out";
}

void
OUTArgTypeNameEmitter::traverse (WstringDeclPtr const&)
{
  os << "::CORBA::WString_out";
}

void
OUTArgTypeNameEmitter::traverse (ObjectDeclPtr const&)
{
  os << "::CORBA::Object_out";
}

void
OUTArgTypeNameEmitter::traverse (ValueBaseDeclPtr const&)
{
  os << "::CORBA::ValueBase_out";
}

void
OUTArgTypeNameEmitter::traverse (AnyDeclPtr const&)
{
  os << "::CORBA::Any_out";
}

void
OUTArgTypeNameEmitter::traverse (StructDeclPtr const&)
{
  //os << "::CORBA::Any *";
}

void
OUTArgTypeNameEmitter::traverse (SequenceDeclPtr const& s)
{
  os << s->name () << "_out";
}

