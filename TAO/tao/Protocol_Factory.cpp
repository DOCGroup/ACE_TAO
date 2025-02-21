#include "tao/Protocol_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Protocol_Factory::TAO_Protocol_Factory (CORBA::ULong tag)
  : tag_ (tag)
{
}

TAO_Protocol_Factory::~TAO_Protocol_Factory ()
{
}

CORBA::ULong
TAO_Protocol_Factory::tag () const
{
  return this->tag_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
