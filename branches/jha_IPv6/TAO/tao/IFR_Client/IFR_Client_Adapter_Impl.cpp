// $Id$

#include "IFR_Client_Adapter_Impl.h"
#include "InterfaceC.h"
#include "tao/ORB_Core.h"

ACE_RCSID(IFR_Client, IFR_Client_Adapter_Impl, "$Id$")

TAO_IFR_Client_Adapter_Impl::~TAO_IFR_Client_Adapter_Impl (void)
{
}

CORBA::Boolean 
TAO_IFR_Client_Adapter_Impl::interfacedef_cdr_insert (
    TAO_OutputCDR &cdr,
    IR_InterfaceDef *object_type
  )
{
  return cdr << object_type;
}

void 
TAO_IFR_Client_Adapter_Impl::interfacedef_any_insert (
    CORBA_Any &any,
    IR_InterfaceDef *object_type
  )
{
  any <<= object_type;
}

IR_InterfaceDef *
TAO_IFR_Client_Adapter_Impl::get_interface (
    CORBA::ORB_ptr orb,
    const char *repo_id,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  CORBA::Object_var obj =
    orb->resolve_initial_references ("InterfaceRepository",
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR_InterfaceDef::_nil ());

  IR_Repository_var repo = IR_Repository::_narrow (obj.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR_InterfaceDef::_nil ());

  IR_Contained_var result = repo->lookup_id (repo_id,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (IR_InterfaceDef::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      return IR_InterfaceDef::_nil ();
    }
  else
    {
      return IR_InterfaceDef::_narrow (result.in (),
                                       ACE_TRY_ENV);
    }
}

CORBA_IRObject_ptr 
TAO_IFR_Client_Adapter_Impl::get_interface_def (
    CORBA::ORB_ptr orb,
    const char *repo_id,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  CORBA::Object_var obj =
    orb->resolve_initial_references ("InterfaceRepository",
                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_IRObject::_nil ());

  IR_Repository_var repo = IR_Repository::_narrow (obj.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_IRObject::_nil ());

  IR_Contained_var result = repo->lookup_id (repo_id,
                                             ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA_IRObject::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      return CORBA_IRObject::_nil ();
    }
  else
    {
      return CORBA_IRObject::_narrow (result.in (),
                                      ACE_TRY_ENV);
    }
}

// *********************************************************************

// Initialization of IFR typecodes that are in the CORBA namespace.

static const CORBA::Long _oc_CORBA_Identifier[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4964),
  ACE_NTOHL (0x656e7469),
  ACE_NTOHL (0x66696572),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0xfdfdfd),  // repository ID = IDL:CORBA/Identifier:1.0
  11,
  ACE_NTOHL (0x4964656e),
  ACE_NTOHL (0x74696669),
  ACE_NTOHL (0x657200fd),  // name = Identifier
  CORBA::tk_string,
  0, // string length
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Identifier (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_Identifier),
    (char *) &_oc_CORBA_Identifier,
    0,
    sizeof (CORBA::Identifier)
  );

static const CORBA::Long _oc_CORBA_ScopedName[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  25, 
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x434f5242), 
  ACE_NTOHL (0x412f5363), 
  ACE_NTOHL (0x6f706564), 
  ACE_NTOHL (0x4e616d65), 
  ACE_NTOHL (0x3a312e30), 
  ACE_NTOHL (0x0),  // repository ID = IDL:CORBA/ScopedName:1.0
  11, 
  ACE_NTOHL (0x53636f70), 
  ACE_NTOHL (0x65644e61), 
  ACE_NTOHL (0x6d650000),  // name = ScopedName
  CORBA::tk_string, 
  0U, // string length
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ScopedName (
    CORBA::tk_alias, 
    sizeof (_oc_CORBA_ScopedName), 
    (char *) &_oc_CORBA_ScopedName, 
    0, 
    sizeof (CORBA::ScopedName)
  );

static const CORBA::Long _oc_CORBA_RepositoryId[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  27,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5265),
  ACE_NTOHL (0x706f7369),
  ACE_NTOHL (0x746f7279),
  ACE_NTOHL (0x49643a31),
  ACE_NTOHL (0x2e3000fd),  // repository ID = IDL:CORBA/RepositoryId:1.0
  13,
  ACE_NTOHL (0x5265706f),
  ACE_NTOHL (0x7369746f),
  ACE_NTOHL (0x72794964),
  ACE_NTOHL (0xfdfdfd),  // name = RepositoryId
  CORBA::tk_string,
  0, // string length
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_RepositoryId (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_RepositoryId),
    (char *) &_oc_CORBA_RepositoryId,
    0,
    sizeof (CORBA::RepositoryId)
  );

static const CORBA::Long _oc_CORBA_IRObject[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31, 
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6f6d672e), 
  ACE_NTOHL (0x6f72672f), 
  ACE_NTOHL (0x434f5242), 
  ACE_NTOHL (0x412f4952), 
  ACE_NTOHL (0x4f626a65), 
  ACE_NTOHL (0x63743a31), 
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/IRObject:1.0
  9, 
  ACE_NTOHL (0x49524f62), 
  ACE_NTOHL (0x6a656374), 
  ACE_NTOHL (0x0),  // name = IRObject
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_IRObject (
    CORBA::tk_objref, 
    sizeof (_oc_CORBA_IRObject), 
    (char *) &_oc_CORBA_IRObject,
    0, 
    sizeof (CORBA_IRObject)
  );

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
  9, 
  ACE_NTOHL (0x62696c69), 
  ACE_NTOHL (0x74790000),  // name = Visibility
  CORBA::tk_short,
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Visibility (
    CORBA::tk_alias, 
    sizeof (_oc_CORBA_Visibility), 
    (char *) &_oc_CORBA_Visibility,
    0, 
    sizeof (CORBA::Visibility)
  );

static const CORBA::Long _oc_CORBA_ValueModifier[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  28, 
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x434f5242), 
  ACE_NTOHL (0x412f5661), 
  ACE_NTOHL (0x6c75654d), 
  ACE_NTOHL (0x6f646966), 
  ACE_NTOHL (0x6965723a), 
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:CORBA/ValueModifier:1.0
  14, 
  ACE_NTOHL (0x56616c75), 
  ACE_NTOHL (0x654d6f64), 
  ACE_NTOHL (0x69666965), 
  ACE_NTOHL (0x72000000),  // name = ValueModifier
  CORBA::tk_short,
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueModifier (
    CORBA::tk_alias, 
    sizeof (_oc_CORBA_ValueModifier), 
    (char *) &_oc_CORBA_ValueModifier, 
    0, 
    sizeof (CORBA::ValueModifier)
  );

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IFR_Client_Adapter_Impl::Initializer (void)
{
  CORBA::_tc_Identifier = &_tc_TAO_tc_CORBA_Identifier;
  CORBA::_tc_ScopedName = &_tc_TAO_tc_CORBA_ScopedName;
  CORBA::_tc_ScopedName = &_tc_TAO_tc_CORBA_RepositoryId;
  CORBA::_tc_IRObject = &_tc_TAO_tc_CORBA_IRObject;
  CORBA::_tc_Visibility = &_tc_TAO_tc_CORBA_Visibility;
  CORBA::_tc_ValueModifier = &_tc_TAO_tc_CORBA_ValueModifier;

  TAO_ORB_Core::ifr_client_adapter_name ("Concrete_IFR_Client_Adapter");

  ACE_Service_Config::static_svcs ()->insert (
      &ace_svc_desc_TAO_IFR_Client_Adapter_Impl
    );
  
  return 0;
}

ACE_STATIC_SVC_DEFINE (
    TAO_IFR_Client_Adapter_Impl,
    ACE_TEXT ("Concrete_IFR_Client_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IFR_Client_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IFR_Client, TAO_IFR_Client_Adapter_Impl)

