// -*- C++ -*-
// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.dre.vanderbilt.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be\be_codegen.cpp:371


#include "tao/RTCORBA/RTCORBA.h"
#include "tao/CDR.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// TAO_IDL - Generated from
// be\be_visitor_arg_traits.cpp:72


TAO_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{
}

TAO_END_VERSIONED_NAMESPACE_DECL


TAO_BEGIN_VERSIONED_NAMESPACE_DECL



// TAO_IDL - Generated from
// be\be_visitor_interface/interface_cs.cpp:60

// Traits specializations for RTCORBA::ProtocolProperties.

RTCORBA::ProtocolProperties_ptr
TAO::Objref_Traits<RTCORBA::ProtocolProperties>::duplicate (
    RTCORBA::ProtocolProperties_ptr p)
{
  return RTCORBA::ProtocolProperties::_duplicate (p);
}

void
TAO::Objref_Traits<RTCORBA::ProtocolProperties>::release (
    RTCORBA::ProtocolProperties_ptr p)
{
  ::CORBA::release (p);
}

RTCORBA::ProtocolProperties_ptr
TAO::Objref_Traits<RTCORBA::ProtocolProperties>::nil ()
{
  return RTCORBA::ProtocolProperties::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits<RTCORBA::ProtocolProperties>::marshal (
    const RTCORBA::ProtocolProperties_ptr p,
    TAO_OutputCDR & cdr)
{
  return ::CORBA::Object::marshal (p, cdr);
}

RTCORBA::ProtocolProperties::ProtocolProperties ()
{}

RTCORBA::ProtocolProperties::~ProtocolProperties ()
{}

void
RTCORBA::ProtocolProperties::_tao_any_destructor (void *_tao_void_pointer)
{
  ProtocolProperties *_tao_tmp_pointer =
    static_cast<ProtocolProperties *> (_tao_void_pointer);
  ::CORBA::release (_tao_tmp_pointer);
}

RTCORBA::ProtocolProperties_ptr
RTCORBA::ProtocolProperties::_narrow (
    ::CORBA::Object_ptr _tao_objref
  )
{
  return ProtocolProperties::_duplicate (
      dynamic_cast<ProtocolProperties_ptr> (_tao_objref)
    );
}

RTCORBA::ProtocolProperties_ptr
RTCORBA::ProtocolProperties::_unchecked_narrow (
    ::CORBA::Object_ptr _tao_objref
  )
{
  return ProtocolProperties::_duplicate (
      dynamic_cast<ProtocolProperties_ptr> (_tao_objref)
    );
}

RTCORBA::ProtocolProperties_ptr
RTCORBA::ProtocolProperties::_duplicate (ProtocolProperties_ptr obj)
{
  if (! ::CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }

  return obj;
}

void
RTCORBA::ProtocolProperties::_tao_release (ProtocolProperties_ptr obj)
{
  ::CORBA::release (obj);
}

::CORBA::Boolean
RTCORBA::ProtocolProperties::_is_a (const char *value)
{
  if (
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/RTCORBA/ProtocolProperties:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* RTCORBA::ProtocolProperties::_interface_repository_id () const
{
  return "IDL:omg.org/RTCORBA/ProtocolProperties:1.0";
}

::CORBA::Boolean
RTCORBA::ProtocolProperties::marshal (TAO_OutputCDR &)
{
  return false;
}

//////////////////////////////////////////////////////
// CDR Encoding Support hand crafted

CORBA::Boolean
RTCORBA::ProtocolProperties::_tao_encode (TAO_OutputCDR &)
{
  return true;
}
// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:60

CORBA::Boolean
RTCORBA::ProtocolProperties::_tao_decode (TAO_InputCDR &)
{
  return true;
}
TAO_END_VERSIONED_NAMESPACE_DECL


