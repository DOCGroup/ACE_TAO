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
ReturnTypeNameEmitter::traverse (VoidPtr const&)
{
  os << "void";
}

void
ReturnTypeNameEmitter::traverse (BooleanPtr const&)
{
  os << "::CORBA::Boolean";
}

void
ReturnTypeNameEmitter::traverse (OctetPtr const&)
{
  os << "::CORBA::Octet";
}

void
ReturnTypeNameEmitter::traverse (CharPtr const&)
{
  os << "::CORBA::Char";
}

void
ReturnTypeNameEmitter::traverse (WcharPtr const&)
{
  os << "::CORBA::WChar";
}

void
ReturnTypeNameEmitter::traverse (ShortPtr const&)
{
  os << "::CORBA::Short";
}

void
ReturnTypeNameEmitter::traverse (UnsignedShortPtr const&)
{
  os << "::CORBA::UShort";
}

void
ReturnTypeNameEmitter::traverse (LongPtr const&)
{
  os << "::CORBA::Long";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLongPtr const&)
{
  os << "::CORBA::ULong";
}

void
ReturnTypeNameEmitter::traverse (LongLongPtr const&)
{
  os << "::CORBA::LongLong";
}

void
ReturnTypeNameEmitter::traverse (UnsignedLongLongPtr const&)
{
  os << "::CORBA::ULongLong";
}

void
ReturnTypeNameEmitter::traverse (FloatPtr const&)
{
  os << "::CORBA::Float";
}

void
ReturnTypeNameEmitter::traverse (DoublePtr const&)
{
  os << "::CORBA::Double";
}

void
ReturnTypeNameEmitter::traverse (StringPtr const&)
{
  os << "char *";
}

void
ReturnTypeNameEmitter::traverse (WstringPtr const&)
{
  os << "::CORBA::WChar *";
}

void
ReturnTypeNameEmitter::traverse (ObjectPtr const&)
{
  os << "::CORBA::Object_ptr";
}

void
ReturnTypeNameEmitter::traverse (ValueBasePtr const&)
{
  os << "::CORBA::ValueBase *";
}

void
ReturnTypeNameEmitter::traverse (AnyPtr const&)
{
  os << "::CORBA::Any *";
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
INArgTypeNameEmitter::traverse (BooleanPtr const&)
{
  os << "::CORBA::Boolean";
}

void
INArgTypeNameEmitter::traverse (OctetPtr const&)
{
  os << "::CORBA::Octet";
}

void
INArgTypeNameEmitter::traverse (CharPtr const&)
{
  os << "::CORBA::Char";
}

void
INArgTypeNameEmitter::traverse (WcharPtr const&)
{
  os << "::CORBA::WChar";
}

void
INArgTypeNameEmitter::traverse (ShortPtr const&)
{
  os << "::CORBA::Short";
}

void
INArgTypeNameEmitter::traverse (UnsignedShortPtr const&)
{
  os << "::CORBA::UShort";
}

void
INArgTypeNameEmitter::traverse (LongPtr const&)
{
  os << "::CORBA::Long";
}

void
INArgTypeNameEmitter::traverse (UnsignedLongPtr const&)
{
  os << "::CORBA::ULong";
}

void
INArgTypeNameEmitter::traverse (LongLongPtr const&)
{
  os << "::CORBA::LongLong";
}

void
INArgTypeNameEmitter::traverse (UnsignedLongLongPtr const&)
{
  os << "::CORBA::ULongLong";
}

void
INArgTypeNameEmitter::traverse (FloatPtr const&)
{
  os << "::CORBA::Float";
}

void
INArgTypeNameEmitter::traverse (DoublePtr const&)
{
  os << "::CORBA::Double";
}

void
INArgTypeNameEmitter::traverse (StringPtr const&)
{
  os << "const char *";
}

void
INArgTypeNameEmitter::traverse (WstringPtr const&)
{
  os << "const ::CORBA::WChar *";
}

void
INArgTypeNameEmitter::traverse (ObjectPtr const&)
{
  os << "::CORBA::Object_ptr";
}

void
INArgTypeNameEmitter::traverse (ValueBasePtr const&)
{
  os << "::CORBA::ValueBase *";
}

void
INArgTypeNameEmitter::traverse (AnyPtr const&)
{
  os << "const ::CORBA::Any &";
}

// ===============================================================

INOUTArgTypeNameEmitter::INOUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
INOUTArgTypeNameEmitter::traverse (BooleanPtr const&)
{
  os << "::CORBA::Boolean &";
}

void
INOUTArgTypeNameEmitter::traverse (OctetPtr const&)
{
  os << "::CORBA::Octet &";
}

void
INOUTArgTypeNameEmitter::traverse (CharPtr const&)
{
  os << "::CORBA::Char &";
}

void
INOUTArgTypeNameEmitter::traverse (WcharPtr const&)
{
  os << "::CORBA::WChar &";
}

void
INOUTArgTypeNameEmitter::traverse (ShortPtr const&)
{
  os << "::CORBA::Short &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedShortPtr const&)
{
  os << "::CORBA::UShort &";
}

void
INOUTArgTypeNameEmitter::traverse (LongPtr const&)
{
  os << "::CORBA::Long &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLongPtr const&)
{
  os << "::CORBA::ULong &";
}

void
INOUTArgTypeNameEmitter::traverse (LongLongPtr const&)
{
  os << "::CORBA::LongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (UnsignedLongLongPtr const&)
{
  os << "::CORBA::ULongLong &";
}

void
INOUTArgTypeNameEmitter::traverse (FloatPtr const&)
{
  os << "::CORBA::Float &";
}

void
INOUTArgTypeNameEmitter::traverse (DoublePtr const&)
{
  os << "::CORBA::Double &";
}

void
INOUTArgTypeNameEmitter::traverse (StringPtr const&)
{
  os << "char *&";
}

void
INOUTArgTypeNameEmitter::traverse (WstringPtr const&)
{
  os << "::CORBA::WChar *&";
}

void
INOUTArgTypeNameEmitter::traverse (ObjectPtr const&)
{
  os << "::CORBA::Object_ptr &";
}

void
INOUTArgTypeNameEmitter::traverse (ValueBasePtr const&)
{
  os << "::CORBA::ValueBase *&";
}

void
INOUTArgTypeNameEmitter::traverse (AnyPtr const&)
{
  os << "::CORBA::Any &";
}

// =================================================================

OUTArgTypeNameEmitter::OUTArgTypeNameEmitter (ostream& os_)
  : TypeNameEmitter (os_)
{
}

void
OUTArgTypeNameEmitter::traverse (BooleanPtr const&)
{
  os << "::CORBA::Boolean_out";
}

void
OUTArgTypeNameEmitter::traverse (OctetPtr const&)
{
  os << "::CORBA::Octet_out";
}

void
OUTArgTypeNameEmitter::traverse (CharPtr const&)
{
  os << "::CORBA::Char_out";
}

void
OUTArgTypeNameEmitter::traverse (WcharPtr const&)
{
  os << "::CORBA::WChar_out";
}

void
OUTArgTypeNameEmitter::traverse (ShortPtr const&)
{
  os << "::CORBA::Short_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedShortPtr const&)
{
  os << "::CORBA::UShort_out";
}

void
OUTArgTypeNameEmitter::traverse (LongPtr const&)
{
  os << "::CORBA::Long_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLongPtr const&)
{
  os << "::CORBA::ULong_out";
}

void
OUTArgTypeNameEmitter::traverse (LongLongPtr const&)
{
  os << "::CORBA::LongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (UnsignedLongLongPtr const&)
{
  os << "::CORBA::ULongLong_out";
}

void
OUTArgTypeNameEmitter::traverse (FloatPtr const&)
{
  os << "::CORBA::Float_out";
}

void
OUTArgTypeNameEmitter::traverse (DoublePtr const&)
{
  os << "::CORBA::Double_out";
}

void
OUTArgTypeNameEmitter::traverse (StringPtr const&)
{
  os << "::CORBA::String_out";
}

void
OUTArgTypeNameEmitter::traverse (WstringPtr const&)
{
  os << "::CORBA::WString_out";
}

void
OUTArgTypeNameEmitter::traverse (ObjectPtr const&)
{
  os << "::CORBA::Object_out";
}

void
OUTArgTypeNameEmitter::traverse (ValueBasePtr const&)
{
  os << "::CORBA::ValueBase_out";
}

void
OUTArgTypeNameEmitter::traverse (AnyPtr const&)
{
  os << "::CORBA::Any_out";
}


