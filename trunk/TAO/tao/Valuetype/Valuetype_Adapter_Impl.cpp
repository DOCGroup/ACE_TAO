// $Id$

#include "Valuetype_Adapter_Impl.h"
#include "AbstractBase.h"
#include "ValueBase.h"
#include "ValueFactory_Map.h"
#include "tao/ORB_Core.h"

ACE_RCSID (Valuetype,
           Valuetype_Adapter_Impl,
           "$Id$")

TAO_Valuetype_Adapter_Impl::TAO_Valuetype_Adapter_Impl (void)
{
  static const CORBA::Long _oc_CORBA_ValueBase[] =
  {
      TAO_ENCAP_BYTE_ORDER, // byte order
    32,
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f5661), 
    ACE_NTOHL (0x6c756542), 
    ACE_NTOHL (0x6173653a), 
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ValueBase:1.0
      10,
    ACE_NTOHL (0x56616c75), 
    ACE_NTOHL (0x65426173), 
    ACE_NTOHL (0x65000000),  // name = ValueBase
      0, // value modifier
    CORBA::tk_null, // no stateful base valuetype

      0, // member count
  };

  ACE_NEW (CORBA::_tc_ValueBase,
           CORBA::TypeCode (CORBA::tk_value,
                            sizeof (_oc_CORBA_ValueBase),
                            (char *) &_oc_CORBA_ValueBase,
                            0,
                            sizeof (CORBA::ValueBase)));

  static const CORBA::Long _oc_CORBA_Visibility[] =
  {
      TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f5669), 
    ACE_NTOHL (0x73696269), 
    ACE_NTOHL (0x6c697479), 
    ACE_NTOHL (0x3a312e30), 
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Visibility:1.0
      11,
    ACE_NTOHL (0x56697369), 
    ACE_NTOHL (0x62696c69), 
    ACE_NTOHL (0x74790000),  // name = Visibility
      CORBA::tk_short,

  };

  ACE_NEW (CORBA::_tc_Visibility,
           CORBA::TypeCode (CORBA::tk_alias,
                            sizeof (_oc_CORBA_Visibility),
                            (char *) &_oc_CORBA_Visibility,
                            0,
                            sizeof (CORBA::Visibility)));

  static const CORBA::Long _oc_CORBA_ValueModifier[] =
  {
      TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6f6d672e), 
    ACE_NTOHL (0x6f72672f), 
    ACE_NTOHL (0x434f5242), 
    ACE_NTOHL (0x412f5661), 
    ACE_NTOHL (0x6c75654d), 
    ACE_NTOHL (0x6f646966), 
    ACE_NTOHL (0x6965723a), 
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ValueModifier:1.0
      14,
    ACE_NTOHL (0x56616c75), 
    ACE_NTOHL (0x654d6f64), 
    ACE_NTOHL (0x69666965), 
    ACE_NTOHL (0x72000000),  // name = ValueModifier
      CORBA::tk_short,

  };

  ACE_NEW (CORBA::_tc_ValueModifier,
           CORBA::TypeCode (CORBA::tk_alias,
                            sizeof (_oc_CORBA_ValueModifier),
                            (char *) &_oc_CORBA_ValueModifier,
                            0,
                            sizeof (CORBA::ValueModifier)));
}

TAO_Valuetype_Adapter_Impl::~TAO_Valuetype_Adapter_Impl (void)
{
  CORBA::release (CORBA::_tc_ValueBase);
  CORBA::release (CORBA::_tc_Visibility);
  CORBA::release (CORBA::_tc_ValueModifier);
}

CORBA::Object_ptr
TAO_Valuetype_Adapter_Impl::abstractbase_to_object (
    CORBA::AbstractBase_ptr p
  )
{
  return p->_to_object ();
}

CORBA::Boolean 
TAO_Valuetype_Adapter_Impl::stream_to_value (TAO_InputCDR &cdr,
                                             CORBA::ValueBase *& val)
{
  return cdr >> val;
}

CORBA::Boolean 
TAO_Valuetype_Adapter_Impl::stream_to_abstract_base (
    TAO_InputCDR &cdr,
    CORBA::AbstractBase_ptr & obj
  )
{
  return cdr >> obj;
}

CORBA::ULong
TAO_Valuetype_Adapter_Impl::type_info_single (void) const
{
  return TAO_OBV_GIOP_Flags::Type_info_single;
}

int
TAO_Valuetype_Adapter_Impl::vf_map_rebind (const char *repo_id,
                                           CORBA::ValueFactory &factory)
{
  return TAO_VALUEFACTORY_MAP::instance ()->rebind (repo_id,
                                                    factory);
}

int
TAO_Valuetype_Adapter_Impl::vf_map_unbind (const char *repo_id)

{
  CORBA::ValueFactory fac;
  return TAO_VALUEFACTORY_MAP::instance ()->unbind (repo_id,
                                                    fac);
}

CORBA::ValueFactory
TAO_Valuetype_Adapter_Impl::vf_map_find (const char *repo_id)
{
  CORBA::ValueFactory factory = 0;
  (void) TAO_VALUEFACTORY_MAP::instance ()->find (repo_id,
                                                  factory);

  return factory;
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_Valuetype_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::valuetype_adapter_name (
      "Concrete_Valuetype_Adapter"
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_Valuetype_Adapter_Impl
      );
}

ACE_STATIC_SVC_DEFINE (
    TAO_Valuetype_Adapter_Impl,
    ACE_TEXT ("Concrete_Valuetype_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Valuetype_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_Valuetype, TAO_Valuetype_Adapter_Impl)
