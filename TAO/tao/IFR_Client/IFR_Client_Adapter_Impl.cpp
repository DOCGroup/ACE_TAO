// $Id$

#include "IFR_Client_Adapter_Impl.h"
#include "IFR_ExtendedC.h"
#include "tao/ORB_Core.h"
#include "tao/Invocation.h"

ACE_RCSID(IFR_Client, IFR_Client_Adapter_Impl, "$Id$")

TAO_IFR_Client_Adapter_Impl::~TAO_IFR_Client_Adapter_Impl (void)
{
}

CORBA::Boolean
TAO_IFR_Client_Adapter_Impl::interfacedef_cdr_insert (
    TAO_OutputCDR &cdr,
    CORBA_InterfaceDef_ptr object_type
  )
{
  return cdr << object_type;
}

void
TAO_IFR_Client_Adapter_Impl::interfacedef_any_insert (
    CORBA_Any &any,
    CORBA_InterfaceDef_ptr object_type
  )
{
  any <<= object_type;
}

void
TAO_IFR_Client_Adapter_Impl::dispose (
    CORBA_InterfaceDef_ptr orphan
  )
{
  CORBA::release (orphan);
}

CORBA_InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface (
    CORBA::ORB_ptr orb,
    const char *repo_id
    ACE_ENV_ARG_DECL
  )
{
  CORBA::Object_var obj =
    orb->resolve_initial_references ("InterfaceRepository"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_InterfaceDef::_nil ());

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        CORBA_InterfaceDef::_nil ());
    }

  CORBA_Repository_var repo =
    CORBA_Repository::_narrow (obj.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_InterfaceDef::_nil ());

  if (CORBA::is_nil (repo.in ()))
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        CORBA_InterfaceDef::_nil ());
    }

  CORBA_Contained_var result = repo->lookup_id (repo_id
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA_InterfaceDef::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      return CORBA_InterfaceDef::_nil ();
    }
  else
    {
      return CORBA_InterfaceDef::_narrow (result.in ()
                                          ACE_ENV_ARG_PARAMETER);
    }
}

CORBA_InterfaceDef_ptr
TAO_IFR_Client_Adapter_Impl::get_interface_remote (
    const CORBA::Object_ptr target
    ACE_ENV_ARG_DECL
  )
{
  CORBA_InterfaceDef_ptr _tao_retval = CORBA_InterfaceDef::_nil ();
  CORBA_InterfaceDef_var _tao_safe_retval (_tao_retval);

  ACE_TRY
    {
      // Must catch exceptions, if the server raises a
      // CORBA::OBJECT_NOT_EXIST then we must return 1, instead of
      // propagating the exception.
      TAO_Stub *istub = target->_stubobj ();
      if (istub == 0)
        ACE_THROW_RETURN (CORBA::INTERNAL (
                            CORBA_SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              EINVAL),
                            CORBA::COMPLETED_NO),
                          _tao_retval);

      TAO_GIOP_Twoway_Invocation _tao_call (istub,
                                            "_interface",
                                            10,
                                            1,
                                            istub->orb_core ());

      for (;;)
        {
          _tao_call.start (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

          _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag)
                                    ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          int _invoke_status =
            _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (_invoke_status == TAO_INVOKE_RESTART)
            {
              _tao_call.restart_flag (1);
              continue;
            }

          ACE_ASSERT (_invoke_status != TAO_INVOKE_EXCEPTION);
          if (_invoke_status != TAO_INVOKE_OK)
            {
              ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_YES),
                                CORBA_InterfaceDef::_nil ());
            }
          break;
        }
      TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
      if (!(
            (_tao_in >> _tao_safe_retval.inout ())
            ))
        ACE_THROW_RETURN (CORBA::MARSHAL (), CORBA_InterfaceDef::_nil ());
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      return CORBA_InterfaceDef::_nil ();
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return _tao_safe_retval._retn ();
}

// *********************************************************************

// Initialization of IFR typecodes that are in the CORBA namespace.

static const CORBA::Long _oc_CORBA_Identifier[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4964),
  ACE_NTOHL (0x656e7469),
  ACE_NTOHL (0x66696572),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
  11,
  ACE_NTOHL (0x4964656e),
  ACE_NTOHL (0x74696669),
  ACE_NTOHL (0x65720000),  // name = Identifier
  CORBA::tk_string,
  0U, // string length
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
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5363),
  ACE_NTOHL (0x6f706564),
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/ScopedName:1.0
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
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5265),
  ACE_NTOHL (0x706f7369),
  ACE_NTOHL (0x746f7279),
  ACE_NTOHL (0x49643a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
  13,
  ACE_NTOHL (0x5265706f),
  ACE_NTOHL (0x7369746f),
  ACE_NTOHL (0x72794964),
  ACE_NTOHL (0x0),  // name = RepositoryId
  CORBA::tk_string,
  0U, // string length
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_RepositoryId (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_RepositoryId),
    (char *) &_oc_CORBA_RepositoryId,
    0,
    sizeof (CORBA::RepositoryId)
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

static const CORBA::Long _oc_CORBA_DefinitionKind[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4465),
  ACE_NTOHL (0x66696e69),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x4b696e64),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/DefinitionKind:1.0
  15,
  ACE_NTOHL (0x44656669),
  ACE_NTOHL (0x6e697469),
  ACE_NTOHL (0x6f6e4b69),
  ACE_NTOHL (0x6e640000),  // name = DefinitionKind
  26, // member count
  8,
  ACE_NTOHL (0x646b5f6e),
  ACE_NTOHL (0x6f6e6500),  // name = dk_none
  7,
  ACE_NTOHL (0x646b5f61),
  ACE_NTOHL (0x6c6c0000),  // name = dk_all
  13,
  ACE_NTOHL (0x646b5f41),
  ACE_NTOHL (0x74747269),
  ACE_NTOHL (0x62757465),
  ACE_NTOHL (0x0),  // name = dk_Attribute
  12,
  ACE_NTOHL (0x646b5f43),
  ACE_NTOHL (0x6f6e7374),
  ACE_NTOHL (0x616e7400),  // name = dk_Constant
  13,
  ACE_NTOHL (0x646b5f45),
  ACE_NTOHL (0x78636570),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = dk_Exception
  13,
  ACE_NTOHL (0x646b5f49),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x66616365),
  ACE_NTOHL (0x0),  // name = dk_Interface
  10,
  ACE_NTOHL (0x646b5f4d),
  ACE_NTOHL (0x6f64756c),
  ACE_NTOHL (0x65000000),  // name = dk_Module
  13,
  ACE_NTOHL (0x646b5f4f),
  ACE_NTOHL (0x70657261),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = dk_Operation
  11,
  ACE_NTOHL (0x646b5f54),
  ACE_NTOHL (0x79706564),
  ACE_NTOHL (0x65660000),  // name = dk_Typedef
  9,
  ACE_NTOHL (0x646b5f41),
  ACE_NTOHL (0x6c696173),
  ACE_NTOHL (0x0),  // name = dk_Alias
  10,
  ACE_NTOHL (0x646b5f53),
  ACE_NTOHL (0x74727563),
  ACE_NTOHL (0x74000000),  // name = dk_Struct
  9,
  ACE_NTOHL (0x646b5f55),
  ACE_NTOHL (0x6e696f6e),
  ACE_NTOHL (0x0),  // name = dk_Union
  8,
  ACE_NTOHL (0x646b5f45),
  ACE_NTOHL (0x6e756d00),  // name = dk_Enum
  13,
  ACE_NTOHL (0x646b5f50),
  ACE_NTOHL (0x72696d69),
  ACE_NTOHL (0x74697665),
  ACE_NTOHL (0x0),  // name = dk_Primitive
  10,
  ACE_NTOHL (0x646b5f53),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x67000000),  // name = dk_String
  12,
  ACE_NTOHL (0x646b5f53),
  ACE_NTOHL (0x65717565),
  ACE_NTOHL (0x6e636500),  // name = dk_Sequence
  9,
  ACE_NTOHL (0x646b5f41),
  ACE_NTOHL (0x72726179),
  ACE_NTOHL (0x0),  // name = dk_Array
  14,
  ACE_NTOHL (0x646b5f52),
  ACE_NTOHL (0x65706f73),
  ACE_NTOHL (0x69746f72),
  ACE_NTOHL (0x79000000),  // name = dk_Repository
  11,
  ACE_NTOHL (0x646b5f57),
  ACE_NTOHL (0x73747269),
  ACE_NTOHL (0x6e670000),  // name = dk_Wstring
  9,
  ACE_NTOHL (0x646b5f46),
  ACE_NTOHL (0x69786564),
  ACE_NTOHL (0x0),  // name = dk_Fixed
  9,
  ACE_NTOHL (0x646b5f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x0),  // name = dk_Value
  12,
  ACE_NTOHL (0x646b5f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x426f7800),  // name = dk_ValueBox
  15,
  ACE_NTOHL (0x646b5f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x65720000),  // name = dk_ValueMember
  10,
  ACE_NTOHL (0x646b5f4e),
  ACE_NTOHL (0x61746976),
  ACE_NTOHL (0x65000000),  // name = dk_Native
  21,
  ACE_NTOHL (0x646b5f41),
  ACE_NTOHL (0x62737472),
  ACE_NTOHL (0x61637449),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x66616365),
  ACE_NTOHL (0x0),  // name = dk_AbstractInterface
  18,
  ACE_NTOHL (0x646b5f4c),
  ACE_NTOHL (0x6f63616c),
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65000000),  // name = dk_LocalInterface
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_DefinitionKind (
    CORBA::tk_enum,
    sizeof (_oc_CORBA_DefinitionKind),
    (char *) &_oc_CORBA_DefinitionKind,
    0,
    sizeof (CORBA::DefinitionKind)
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

static const CORBA::Long _oc_CORBA_VersionSpec[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5665),
  ACE_NTOHL (0x7273696f),
  ACE_NTOHL (0x6e537065),
  ACE_NTOHL (0x633a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
  12,
  ACE_NTOHL (0x56657273),
  ACE_NTOHL (0x696f6e53),
  ACE_NTOHL (0x70656300),  // name = VersionSpec
  CORBA::tk_string,
  0U, // string length
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_VersionSpec (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_VersionSpec),
    (char *) &_oc_CORBA_VersionSpec,
    0,
    sizeof (CORBA::VersionSpec)
  );

static const CORBA::Long _oc_CORBA_Contained[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e746169),
  ACE_NTOHL (0x6e65643a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/Contained:1.0
  10,
  ACE_NTOHL (0x436f6e74),
  ACE_NTOHL (0x61696e65),
  ACE_NTOHL (0x64000000),  // name = Contained
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Contained (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_Contained),
    (char *) &_oc_CORBA_Contained,
    0,
    sizeof (CORBA::Contained)
  );

static const CORBA::Long _oc_CORBA_ContainedSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e746169),
  ACE_NTOHL (0x6e656453),
  ACE_NTOHL (0x65713a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ContainedSeq:1.0
  13,
  ACE_NTOHL (0x436f6e74),
  ACE_NTOHL (0x61696e65),
  ACE_NTOHL (0x64536571),
  ACE_NTOHL (0x0),  // name = ContainedSeq
  CORBA::tk_sequence, // typecode kind
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      32,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f436f),
      ACE_NTOHL (0x6e746169),
      ACE_NTOHL (0x6e65643a),
      ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/Contained:1.0
      10,
      ACE_NTOHL (0x436f6e74),
      ACE_NTOHL (0x61696e65),
      ACE_NTOHL (0x64000000),  // name = Contained

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ContainedSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ContainedSeq),
    (char *) &_oc_CORBA_ContainedSeq,
    0,
    sizeof (CORBA::ContainedSeq)
  );

static const CORBA::Long _oc_CORBA_InterfaceDefSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f496e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636544),
  ACE_NTOHL (0x65665365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/InterfaceDefSeq:1.0
  16,
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x53657100),  // name = InterfaceDefSeq
  CORBA::tk_sequence, // typecode kind
  80, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    64, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      35,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f496e),
      ACE_NTOHL (0x74657266),
      ACE_NTOHL (0x61636544),
      ACE_NTOHL (0x65663a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/InterfaceDef:1.0
      13,
      ACE_NTOHL (0x496e7465),
      ACE_NTOHL (0x72666163),
      ACE_NTOHL (0x65446566),
      ACE_NTOHL (0x0),  // name = InterfaceDef

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_InterfaceDefSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_InterfaceDefSeq),
    (char *) &_oc_CORBA_InterfaceDefSeq,
    0,
    sizeof (CORBA::InterfaceDefSeq)
  );

static const CORBA::Long _oc_CORBA_AbstractInterfaceDefSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  46,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4162),
  ACE_NTOHL (0x73747261),
  ACE_NTOHL (0x6374496e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636544),
  ACE_NTOHL (0x65665365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/AbstractInterfaceDefSeq:1.0
  24,
  ACE_NTOHL (0x41627374),
  ACE_NTOHL (0x72616374),
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x53657100),  // name = AbstractInterfaceDefSeq
  CORBA::tk_sequence, // typecode kind
  96, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    80, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      43,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4162),
      ACE_NTOHL (0x73747261),
      ACE_NTOHL (0x6374496e),
      ACE_NTOHL (0x74657266),
      ACE_NTOHL (0x61636544),
      ACE_NTOHL (0x65663a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AbstractInterfaceDef:1.0
      21,
      ACE_NTOHL (0x41627374),
      ACE_NTOHL (0x72616374),
      ACE_NTOHL (0x496e7465),
      ACE_NTOHL (0x72666163),
      ACE_NTOHL (0x65446566),
      ACE_NTOHL (0x0),  // name = AbstractInterfaceDef

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AbstractInterfaceDefSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_AbstractInterfaceDefSeq),
    (char *) &_oc_CORBA_AbstractInterfaceDefSeq,
    0,
    sizeof (CORBA::AbstractInterfaceDefSeq)
  );

static const CORBA::Long _oc_CORBA_LocalInterfaceDefSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4c6f),
  ACE_NTOHL (0x63616c49),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x66616365),
  ACE_NTOHL (0x44656653),
  ACE_NTOHL (0x65713a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/LocalInterfaceDefSeq:1.0
  21,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x6c496e74),
  ACE_NTOHL (0x65726661),
  ACE_NTOHL (0x63654465),
  ACE_NTOHL (0x66536571),
  ACE_NTOHL (0x0),  // name = LocalInterfaceDefSeq
  CORBA::tk_sequence, // typecode kind
  88, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    72, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      40,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4c6f),
      ACE_NTOHL (0x63616c49),
      ACE_NTOHL (0x6e746572),
      ACE_NTOHL (0x66616365),
      ACE_NTOHL (0x4465663a),
      ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/LocalInterfaceDef:1.0
      18,
      ACE_NTOHL (0x4c6f6361),
      ACE_NTOHL (0x6c496e74),
      ACE_NTOHL (0x65726661),
      ACE_NTOHL (0x63654465),
      ACE_NTOHL (0x66000000),  // name = LocalInterfaceDef

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_LocalInterfaceDefSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_LocalInterfaceDefSeq),
    (char *) &_oc_CORBA_LocalInterfaceDefSeq,
    0,
    sizeof (CORBA::LocalInterfaceDefSeq)
  );

static const CORBA::Long _oc_CORBA_StructMember[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5374),
  ACE_NTOHL (0x72756374),
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x65723a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/StructMember:1.0
  13,
  ACE_NTOHL (0x53747275),
  ACE_NTOHL (0x63744d65),
  ACE_NTOHL (0x6d626572),
  ACE_NTOHL (0x0),  // name = StructMember
  3, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  9,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x5f646566),
  ACE_NTOHL (0x0),  // name = type_def
  CORBA::tk_objref, // typecode kind
  52, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    30,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4944),
    ACE_NTOHL (0x4c547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
    8,
    ACE_NTOHL (0x49444c54),
    ACE_NTOHL (0x79706500),  // name = IDLType

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_StructMember (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_StructMember),
    (char *) &_oc_CORBA_StructMember,
    0,
    sizeof (CORBA::StructMember)
  );

static const CORBA::Long _oc_CORBA_StructMemberSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5374),
  ACE_NTOHL (0x72756374),
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x65725365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/StructMemberSeq:1.0
  16,
  ACE_NTOHL (0x53747275),
  ACE_NTOHL (0x63744d65),
  ACE_NTOHL (0x6d626572),
  ACE_NTOHL (0x53657100),  // name = StructMemberSeq
  CORBA::tk_sequence, // typecode kind
  264, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    248, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      35,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5374),
      ACE_NTOHL (0x72756374),
      ACE_NTOHL (0x4d656d62),
      ACE_NTOHL (0x65723a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/StructMember:1.0
      13,
      ACE_NTOHL (0x53747275),
      ACE_NTOHL (0x63744d65),
      ACE_NTOHL (0x6d626572),
      ACE_NTOHL (0x0),  // name = StructMember
      3, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,

      9,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x5f646566),
      ACE_NTOHL (0x0),  // name = type_def
      CORBA::tk_objref, // typecode kind
      52, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        30,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4944),
        ACE_NTOHL (0x4c547970),
        ACE_NTOHL (0x653a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
        8,
        ACE_NTOHL (0x49444c54),
        ACE_NTOHL (0x79706500),  // name = IDLType


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_StructMemberSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_StructMemberSeq),
    (char *) &_oc_CORBA_StructMemberSeq,
    0,
    sizeof (CORBA::StructMemberSeq)
  );

static const CORBA::Long _oc_CORBA_Initializer[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f496e),
  ACE_NTOHL (0x69746961),
  ACE_NTOHL (0x6c697a65),
  ACE_NTOHL (0x723a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/Initializer:1.0
  12,
  ACE_NTOHL (0x496e6974),
  ACE_NTOHL (0x69616c69),
  ACE_NTOHL (0x7a657200),  // name = Initializer
  2, // member count
  8,
  ACE_NTOHL (0x6d656d62),
  ACE_NTOHL (0x65727300),  // name = members
  CORBA::tk_alias, // typecode kind for typedefs
  340, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5374),
    ACE_NTOHL (0x72756374),
    ACE_NTOHL (0x4d656d62),
    ACE_NTOHL (0x65725365),
    ACE_NTOHL (0x713a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/StructMemberSeq:1.0
    16,
    ACE_NTOHL (0x53747275),
    ACE_NTOHL (0x63744d65),
    ACE_NTOHL (0x6d626572),
    ACE_NTOHL (0x53657100),  // name = StructMemberSeq
    CORBA::tk_sequence, // typecode kind
    264, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_struct, // typecode kind
      248, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5374),
        ACE_NTOHL (0x72756374),
        ACE_NTOHL (0x4d656d62),
        ACE_NTOHL (0x65723a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/StructMember:1.0
        13,
        ACE_NTOHL (0x53747275),
        ACE_NTOHL (0x63744d65),
        ACE_NTOHL (0x6d626572),
        ACE_NTOHL (0x0),  // name = StructMember
        3, // member count
        5,
        ACE_NTOHL (0x6e616d65),
        ACE_NTOHL (0x0),  // name = name
        CORBA::tk_alias, // typecode kind for typedefs
        68, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          33,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4964),
          ACE_NTOHL (0x656e7469),
          ACE_NTOHL (0x66696572),
          ACE_NTOHL (0x3a312e30),
          ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
          11,
          ACE_NTOHL (0x4964656e),
          ACE_NTOHL (0x74696669),
          ACE_NTOHL (0x65720000),  // name = Identifier
          CORBA::tk_string,
          0U, // string length

        5,
        ACE_NTOHL (0x74797065),
        ACE_NTOHL (0x0),  // name = type
        CORBA::tk_TypeCode,

        9,
        ACE_NTOHL (0x74797065),
        ACE_NTOHL (0x5f646566),
        ACE_NTOHL (0x0),  // name = type_def
        CORBA::tk_objref, // typecode kind
        52, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          30,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4944),
          ACE_NTOHL (0x4c547970),
          ACE_NTOHL (0x653a312e),
          ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
          8,
          ACE_NTOHL (0x49444c54),
          ACE_NTOHL (0x79706500),  // name = IDLType


      0U,


  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Initializer (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_Initializer),
    (char *) &_oc_CORBA_Initializer,
    0,
    sizeof (CORBA::Initializer)
  );

static const CORBA::Long _oc_CORBA_InitializerSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f496e),
  ACE_NTOHL (0x69746961),
  ACE_NTOHL (0x6c697a65),
  ACE_NTOHL (0x72536571),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/InitializerSeq:1.0
  15,
  ACE_NTOHL (0x496e6974),
  ACE_NTOHL (0x69616c69),
  ACE_NTOHL (0x7a657253),
  ACE_NTOHL (0x65710000),  // name = InitializerSeq
  CORBA::tk_sequence, // typecode kind
  528, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    512, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      34,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f496e),
      ACE_NTOHL (0x69746961),
      ACE_NTOHL (0x6c697a65),
      ACE_NTOHL (0x723a312e),
      ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/Initializer:1.0
      12,
      ACE_NTOHL (0x496e6974),
      ACE_NTOHL (0x69616c69),
      ACE_NTOHL (0x7a657200),  // name = Initializer
      2, // member count
      8,
      ACE_NTOHL (0x6d656d62),
      ACE_NTOHL (0x65727300),  // name = members
      CORBA::tk_alias, // typecode kind for typedefs
      340, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        38,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5374),
        ACE_NTOHL (0x72756374),
        ACE_NTOHL (0x4d656d62),
        ACE_NTOHL (0x65725365),
        ACE_NTOHL (0x713a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/StructMemberSeq:1.0
        16,
        ACE_NTOHL (0x53747275),
        ACE_NTOHL (0x63744d65),
        ACE_NTOHL (0x6d626572),
        ACE_NTOHL (0x53657100),  // name = StructMemberSeq
        CORBA::tk_sequence, // typecode kind
        264, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_struct, // typecode kind
          248, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            35,
            ACE_NTOHL (0x49444c3a),
            ACE_NTOHL (0x6f6d672e),
            ACE_NTOHL (0x6f72672f),
            ACE_NTOHL (0x434f5242),
            ACE_NTOHL (0x412f5374),
            ACE_NTOHL (0x72756374),
            ACE_NTOHL (0x4d656d62),
            ACE_NTOHL (0x65723a31),
            ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/StructMember:1.0
            13,
            ACE_NTOHL (0x53747275),
            ACE_NTOHL (0x63744d65),
            ACE_NTOHL (0x6d626572),
            ACE_NTOHL (0x0),  // name = StructMember
            3, // member count
            5,
            ACE_NTOHL (0x6e616d65),
            ACE_NTOHL (0x0),  // name = name
            CORBA::tk_alias, // typecode kind for typedefs
            68, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              33,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4964),
              ACE_NTOHL (0x656e7469),
              ACE_NTOHL (0x66696572),
              ACE_NTOHL (0x3a312e30),
              ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
              11,
              ACE_NTOHL (0x4964656e),
              ACE_NTOHL (0x74696669),
              ACE_NTOHL (0x65720000),  // name = Identifier
              CORBA::tk_string,
              0U, // string length

            5,
            ACE_NTOHL (0x74797065),
            ACE_NTOHL (0x0),  // name = type
            CORBA::tk_TypeCode,

            9,
            ACE_NTOHL (0x74797065),
            ACE_NTOHL (0x5f646566),
            ACE_NTOHL (0x0),  // name = type_def
            CORBA::tk_objref, // typecode kind
            52, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              30,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4944),
              ACE_NTOHL (0x4c547970),
              ACE_NTOHL (0x653a312e),
              ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
              8,
              ACE_NTOHL (0x49444c54),
              ACE_NTOHL (0x79706500),  // name = IDLType


          0U,


      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_InitializerSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_InitializerSeq),
    (char *) &_oc_CORBA_InitializerSeq,
    0,
    sizeof (CORBA::InitializerSeq)
  );

static const CORBA::Long _oc_CORBA_UnionMember[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f556e),
  ACE_NTOHL (0x696f6e4d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x723a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/UnionMember:1.0
  12,
  ACE_NTOHL (0x556e696f),
  ACE_NTOHL (0x6e4d656d),
  ACE_NTOHL (0x62657200),  // name = UnionMember
  4, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  6,
  ACE_NTOHL (0x6c616265),
  ACE_NTOHL (0x6c000000),  // name = label
  CORBA::tk_any,

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  9,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x5f646566),
  ACE_NTOHL (0x0),  // name = type_def
  CORBA::tk_objref, // typecode kind
  52, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    30,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4944),
    ACE_NTOHL (0x4c547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
    8,
    ACE_NTOHL (0x49444c54),
    ACE_NTOHL (0x79706500),  // name = IDLType

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_UnionMember (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_UnionMember),
    (char *) &_oc_CORBA_UnionMember,
    0,
    sizeof (CORBA::UnionMember)
  );

static const CORBA::Long _oc_CORBA_UnionMemberSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f556e),
  ACE_NTOHL (0x696f6e4d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x72536571),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/UnionMemberSeq:1.0
  15,
  ACE_NTOHL (0x556e696f),
  ACE_NTOHL (0x6e4d656d),
  ACE_NTOHL (0x62657253),
  ACE_NTOHL (0x65710000),  // name = UnionMemberSeq
  CORBA::tk_sequence, // typecode kind
  276, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    260, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      34,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f556e),
      ACE_NTOHL (0x696f6e4d),
      ACE_NTOHL (0x656d6265),
      ACE_NTOHL (0x723a312e),
      ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/UnionMember:1.0
      12,
      ACE_NTOHL (0x556e696f),
      ACE_NTOHL (0x6e4d656d),
      ACE_NTOHL (0x62657200),  // name = UnionMember
      4, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      6,
      ACE_NTOHL (0x6c616265),
      ACE_NTOHL (0x6c000000),  // name = label
      CORBA::tk_any,

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,

      9,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x5f646566),
      ACE_NTOHL (0x0),  // name = type_def
      CORBA::tk_objref, // typecode kind
      52, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        30,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4944),
        ACE_NTOHL (0x4c547970),
        ACE_NTOHL (0x653a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
        8,
        ACE_NTOHL (0x49444c54),
        ACE_NTOHL (0x79706500),  // name = IDLType


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_UnionMemberSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_UnionMemberSeq),
    (char *) &_oc_CORBA_UnionMemberSeq,
    0,
    sizeof (CORBA::UnionMemberSeq)
  );

static const CORBA::Long _oc_CORBA_EnumMemberSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f456e),
  ACE_NTOHL (0x756d4d65),
  ACE_NTOHL (0x6d626572),
  ACE_NTOHL (0x5365713a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/EnumMemberSeq:1.0
  14,
  ACE_NTOHL (0x456e756d),
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x65725365),
  ACE_NTOHL (0x71000000),  // name = EnumMemberSeq
  CORBA::tk_sequence, // typecode kind
  84, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_alias, // typecode kind for typedefs
    68, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      33,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4964),
      ACE_NTOHL (0x656e7469),
      ACE_NTOHL (0x66696572),
      ACE_NTOHL (0x3a312e30),
      ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
      11,
      ACE_NTOHL (0x4964656e),
      ACE_NTOHL (0x74696669),
      ACE_NTOHL (0x65720000),  // name = Identifier
      CORBA::tk_string,
      0U, // string length

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_EnumMemberSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_EnumMemberSeq),
    (char *) &_oc_CORBA_EnumMemberSeq,
    0,
    sizeof (CORBA::EnumMemberSeq)
  );

static const CORBA::Long _oc_CORBA_Container[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e746169),
  ACE_NTOHL (0x6e65723a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/Container:1.0
  10,
  ACE_NTOHL (0x436f6e74),
  ACE_NTOHL (0x61696e65),
  ACE_NTOHL (0x72000000),  // name = Container
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Container (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_Container),
    (char *) &_oc_CORBA_Container,
    0,
    sizeof (CORBA::Container)
  );

static const CORBA::Long _oc_CORBA_IDLType[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4944),
  ACE_NTOHL (0x4c547970),
  ACE_NTOHL (0x653a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
  8,
  ACE_NTOHL (0x49444c54),
  ACE_NTOHL (0x79706500),  // name = IDLType
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_IDLType (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_IDLType),
    (char *) &_oc_CORBA_IDLType,
    0,
    sizeof (CORBA::IDLType)
  );

static const CORBA::Long _oc_CORBA_TypedefDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5479),
  ACE_NTOHL (0x70656465),
  ACE_NTOHL (0x66446566),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/TypedefDef:1.0
  11,
  ACE_NTOHL (0x54797065),
  ACE_NTOHL (0x64656644),
  ACE_NTOHL (0x65660000),  // name = TypedefDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_TypedefDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_TypedefDef),
    (char *) &_oc_CORBA_TypedefDef,
    0,
    sizeof (CORBA::TypedefDef)
  );

static const CORBA::Long _oc_CORBA_TypeDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5479),
  ACE_NTOHL (0x70654465),
  ACE_NTOHL (0x73637269),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e3a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/TypeDescription:1.0
  16,
  ACE_NTOHL (0x54797065),
  ACE_NTOHL (0x44657363),
  ACE_NTOHL (0x72697074),
  ACE_NTOHL (0x696f6e00),  // name = TypeDescription
  5, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_TypeDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_TypeDescription),
    (char *) &_oc_CORBA_TypeDescription,
    0,
    sizeof (CORBA::TypeDescription)
  );

static const CORBA::Long _oc_CORBA_PrimitiveKind[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5072),
  ACE_NTOHL (0x696d6974),
  ACE_NTOHL (0x6976654b),
  ACE_NTOHL (0x696e643a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/PrimitiveKind:1.0
  14,
  ACE_NTOHL (0x5072696d),
  ACE_NTOHL (0x69746976),
  ACE_NTOHL (0x654b696e),
  ACE_NTOHL (0x64000000),  // name = PrimitiveKind
  22, // member count
  8,
  ACE_NTOHL (0x706b5f6e),
  ACE_NTOHL (0x756c6c00),  // name = pk_null
  8,
  ACE_NTOHL (0x706b5f76),
  ACE_NTOHL (0x6f696400),  // name = pk_void
  9,
  ACE_NTOHL (0x706b5f73),
  ACE_NTOHL (0x686f7274),
  ACE_NTOHL (0x0),  // name = pk_short
  8,
  ACE_NTOHL (0x706b5f6c),
  ACE_NTOHL (0x6f6e6700),  // name = pk_long
  10,
  ACE_NTOHL (0x706b5f75),
  ACE_NTOHL (0x73686f72),
  ACE_NTOHL (0x74000000),  // name = pk_ushort
  9,
  ACE_NTOHL (0x706b5f75),
  ACE_NTOHL (0x6c6f6e67),
  ACE_NTOHL (0x0),  // name = pk_ulong
  9,
  ACE_NTOHL (0x706b5f66),
  ACE_NTOHL (0x6c6f6174),
  ACE_NTOHL (0x0),  // name = pk_float
  10,
  ACE_NTOHL (0x706b5f64),
  ACE_NTOHL (0x6f75626c),
  ACE_NTOHL (0x65000000),  // name = pk_double
  11,
  ACE_NTOHL (0x706b5f62),
  ACE_NTOHL (0x6f6f6c65),
  ACE_NTOHL (0x616e0000),  // name = pk_boolean
  8,
  ACE_NTOHL (0x706b5f63),
  ACE_NTOHL (0x68617200),  // name = pk_char
  9,
  ACE_NTOHL (0x706b5f6f),
  ACE_NTOHL (0x63746574),
  ACE_NTOHL (0x0),  // name = pk_octet
  7,
  ACE_NTOHL (0x706b5f61),
  ACE_NTOHL (0x6e790000),  // name = pk_any
  12,
  ACE_NTOHL (0x706b5f54),
  ACE_NTOHL (0x79706543),
  ACE_NTOHL (0x6f646500),  // name = pk_TypeCode
  13,
  ACE_NTOHL (0x706b5f50),
  ACE_NTOHL (0x72696e63),
  ACE_NTOHL (0x6970616c),
  ACE_NTOHL (0x0),  // name = pk_Principal
  10,
  ACE_NTOHL (0x706b5f73),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x67000000),  // name = pk_string
  10,
  ACE_NTOHL (0x706b5f6f),
  ACE_NTOHL (0x626a7265),
  ACE_NTOHL (0x66000000),  // name = pk_objref
  12,
  ACE_NTOHL (0x706b5f6c),
  ACE_NTOHL (0x6f6e676c),
  ACE_NTOHL (0x6f6e6700),  // name = pk_longlong
  13,
  ACE_NTOHL (0x706b5f75),
  ACE_NTOHL (0x6c6f6e67),
  ACE_NTOHL (0x6c6f6e67),
  ACE_NTOHL (0x0),  // name = pk_ulonglong
  14,
  ACE_NTOHL (0x706b5f6c),
  ACE_NTOHL (0x6f6e6764),
  ACE_NTOHL (0x6f75626c),
  ACE_NTOHL (0x65000000),  // name = pk_longdouble
  9,
  ACE_NTOHL (0x706b5f77),
  ACE_NTOHL (0x63686172),
  ACE_NTOHL (0x0),  // name = pk_wchar
  11,
  ACE_NTOHL (0x706b5f77),
  ACE_NTOHL (0x73747269),
  ACE_NTOHL (0x6e670000),  // name = pk_wstring
  14,
  ACE_NTOHL (0x706b5f76),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x5f626173),
  ACE_NTOHL (0x65000000),  // name = pk_value_base
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_PrimitiveKind (
    CORBA::tk_enum,
    sizeof (_oc_CORBA_PrimitiveKind),
    (char *) &_oc_CORBA_PrimitiveKind,
    0,
    sizeof (CORBA::PrimitiveKind)
  );

static const CORBA::Long _oc_CORBA_Repository[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5265),
  ACE_NTOHL (0x706f7369),
  ACE_NTOHL (0x746f7279),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Repository:1.0
  11,
  ACE_NTOHL (0x5265706f),
  ACE_NTOHL (0x7369746f),
  ACE_NTOHL (0x72790000),  // name = Repository
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_Repository (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_Repository),
    (char *) &_oc_CORBA_Repository,
    0,
    sizeof (CORBA::Repository)
  );

static const CORBA::Long _oc_CORBA_ModuleDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4d6f),
  ACE_NTOHL (0x64756c65),
  ACE_NTOHL (0x4465663a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ModuleDef:1.0
  10,
  ACE_NTOHL (0x4d6f6475),
  ACE_NTOHL (0x6c654465),
  ACE_NTOHL (0x66000000),  // name = ModuleDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ModuleDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ModuleDef),
    (char *) &_oc_CORBA_ModuleDef,
    0,
    sizeof (CORBA::ModuleDef)
  );

static const CORBA::Long _oc_CORBA_ModuleDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4d6f),
  ACE_NTOHL (0x64756c65),
  ACE_NTOHL (0x44657363),
  ACE_NTOHL (0x72697074),
  ACE_NTOHL (0x696f6e3a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ModuleDescription:1.0
  18,
  ACE_NTOHL (0x4d6f6475),
  ACE_NTOHL (0x6c654465),
  ACE_NTOHL (0x73637269),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e000000),  // name = ModuleDescription
  4, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ModuleDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ModuleDescription),
    (char *) &_oc_CORBA_ModuleDescription,
    0,
    sizeof (CORBA::ModuleDescription)
  );

static const CORBA::Long _oc_CORBA_ConstantDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e737461),
  ACE_NTOHL (0x6e744465),
  ACE_NTOHL (0x663a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ConstantDef:1.0
  12,
  ACE_NTOHL (0x436f6e73),
  ACE_NTOHL (0x74616e74),
  ACE_NTOHL (0x44656600),  // name = ConstantDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ConstantDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ConstantDef),
    (char *) &_oc_CORBA_ConstantDef,
    0,
    sizeof (CORBA::ConstantDef)
  );

static const CORBA::Long _oc_CORBA_ConstantDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  42,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e737461),
  ACE_NTOHL (0x6e744465),
  ACE_NTOHL (0x73637269),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e3a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ConstantDescription:1.0
  20,
  ACE_NTOHL (0x436f6e73),
  ACE_NTOHL (0x74616e74),
  ACE_NTOHL (0x44657363),
  ACE_NTOHL (0x72697074),
  ACE_NTOHL (0x696f6e00),  // name = ConstantDescription
  6, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  6,
  ACE_NTOHL (0x76616c75),
  ACE_NTOHL (0x65000000),  // name = value
  CORBA::tk_any,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ConstantDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ConstantDescription),
    (char *) &_oc_CORBA_ConstantDescription,
    0,
    sizeof (CORBA::ConstantDescription)
  );

static const CORBA::Long _oc_CORBA_StructDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5374),
  ACE_NTOHL (0x72756374),
  ACE_NTOHL (0x4465663a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/StructDef:1.0
  10,
  ACE_NTOHL (0x53747275),
  ACE_NTOHL (0x63744465),
  ACE_NTOHL (0x66000000),  // name = StructDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_StructDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_StructDef),
    (char *) &_oc_CORBA_StructDef,
    0,
    sizeof (CORBA::StructDef)
  );

static const CORBA::Long _oc_CORBA_UnionDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f556e),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/UnionDef:1.0
  9,
  ACE_NTOHL (0x556e696f),
  ACE_NTOHL (0x6e446566),
  ACE_NTOHL (0x0),  // name = UnionDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_UnionDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_UnionDef),
    (char *) &_oc_CORBA_UnionDef,
    0,
    sizeof (CORBA::UnionDef)
  );

static const CORBA::Long _oc_CORBA_EnumDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f456e),
  ACE_NTOHL (0x756d4465),
  ACE_NTOHL (0x663a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/EnumDef:1.0
  8,
  ACE_NTOHL (0x456e756d),
  ACE_NTOHL (0x44656600),  // name = EnumDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_EnumDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_EnumDef),
    (char *) &_oc_CORBA_EnumDef,
    0,
    sizeof (CORBA::EnumDef)
  );

static const CORBA::Long _oc_CORBA_AliasDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f416c),
  ACE_NTOHL (0x69617344),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AliasDef:1.0
  9,
  ACE_NTOHL (0x416c6961),
  ACE_NTOHL (0x73446566),
  ACE_NTOHL (0x0),  // name = AliasDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AliasDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_AliasDef),
    (char *) &_oc_CORBA_AliasDef,
    0,
    sizeof (CORBA::AliasDef)
  );

static const CORBA::Long _oc_CORBA_NativeDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4e61),
  ACE_NTOHL (0x74697665),
  ACE_NTOHL (0x4465663a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/NativeDef:1.0
  10,
  ACE_NTOHL (0x4e617469),
  ACE_NTOHL (0x76654465),
  ACE_NTOHL (0x66000000),  // name = NativeDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_NativeDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_NativeDef),
    (char *) &_oc_CORBA_NativeDef,
    0,
    sizeof (CORBA::NativeDef)
  );

static const CORBA::Long _oc_CORBA_PrimitiveDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5072),
  ACE_NTOHL (0x696d6974),
  ACE_NTOHL (0x69766544),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/PrimitiveDef:1.0
  13,
  ACE_NTOHL (0x5072696d),
  ACE_NTOHL (0x69746976),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x0),  // name = PrimitiveDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_PrimitiveDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_PrimitiveDef),
    (char *) &_oc_CORBA_PrimitiveDef,
    0,
    sizeof (CORBA::PrimitiveDef)
  );

static const CORBA::Long _oc_CORBA_StringDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5374),
  ACE_NTOHL (0x72696e67),
  ACE_NTOHL (0x4465663a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/StringDef:1.0
  10,
  ACE_NTOHL (0x53747269),
  ACE_NTOHL (0x6e674465),
  ACE_NTOHL (0x66000000),  // name = StringDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_StringDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_StringDef),
    (char *) &_oc_CORBA_StringDef,
    0,
    sizeof (CORBA::StringDef)
  );

static const CORBA::Long _oc_CORBA_WstringDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5773),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x67446566),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/WstringDef:1.0
  11,
  ACE_NTOHL (0x57737472),
  ACE_NTOHL (0x696e6744),
  ACE_NTOHL (0x65660000),  // name = WstringDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_WstringDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_WstringDef),
    (char *) &_oc_CORBA_WstringDef,
    0,
    sizeof (CORBA::WstringDef)
  );

static const CORBA::Long _oc_CORBA_SequenceDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5365),
  ACE_NTOHL (0x7175656e),
  ACE_NTOHL (0x63654465),
  ACE_NTOHL (0x663a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/SequenceDef:1.0
  12,
  ACE_NTOHL (0x53657175),
  ACE_NTOHL (0x656e6365),
  ACE_NTOHL (0x44656600),  // name = SequenceDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_SequenceDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_SequenceDef),
    (char *) &_oc_CORBA_SequenceDef,
    0,
    sizeof (CORBA::SequenceDef)
  );

static const CORBA::Long _oc_CORBA_ArrayDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4172),
  ACE_NTOHL (0x72617944),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ArrayDef:1.0
  9,
  ACE_NTOHL (0x41727261),
  ACE_NTOHL (0x79446566),
  ACE_NTOHL (0x0),  // name = ArrayDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ArrayDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ArrayDef),
    (char *) &_oc_CORBA_ArrayDef,
    0,
    sizeof (CORBA::ArrayDef)
  );

static const CORBA::Long _oc_CORBA_ExceptionDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4578),
  ACE_NTOHL (0x63657074),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDef:1.0
  13,
  ACE_NTOHL (0x45786365),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e446566),
  ACE_NTOHL (0x0),  // name = ExceptionDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ExceptionDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ExceptionDef),
    (char *) &_oc_CORBA_ExceptionDef,
    0,
    sizeof (CORBA::ExceptionDef)
  );

static const CORBA::Long _oc_CORBA_ExceptionDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4578),
  ACE_NTOHL (0x63657074),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDescription:1.0
  21,
  ACE_NTOHL (0x45786365),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = ExceptionDescription
  5, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ExceptionDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ExceptionDescription),
    (char *) &_oc_CORBA_ExceptionDescription,
    0,
    sizeof (CORBA::ExceptionDescription)
  );

static const CORBA::Long _oc_CORBA_ExceptionDefSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4578),
  ACE_NTOHL (0x63657074),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65665365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ExceptionDefSeq:1.0
  16,
  ACE_NTOHL (0x45786365),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e446566),
  ACE_NTOHL (0x53657100),  // name = ExceptionDefSeq
  CORBA::tk_sequence, // typecode kind
  80, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    64, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      35,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4578),
      ACE_NTOHL (0x63657074),
      ACE_NTOHL (0x696f6e44),
      ACE_NTOHL (0x65663a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDef:1.0
      13,
      ACE_NTOHL (0x45786365),
      ACE_NTOHL (0x7074696f),
      ACE_NTOHL (0x6e446566),
      ACE_NTOHL (0x0),  // name = ExceptionDef

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ExceptionDefSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ExceptionDefSeq),
    (char *) &_oc_CORBA_ExceptionDefSeq,
    0,
    sizeof (CORBA::ExceptionDefSeq)
  );

static const CORBA::Long _oc_CORBA_ExcDescriptionSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4578),
  ACE_NTOHL (0x63446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x5365713a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ExcDescriptionSeq:1.0
  18,
  ACE_NTOHL (0x45786344),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e5365),
  ACE_NTOHL (0x71000000),  // name = ExcDescriptionSeq
  CORBA::tk_sequence, // typecode kind
  476, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    460, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      43,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4578),
      ACE_NTOHL (0x63657074),
      ACE_NTOHL (0x696f6e44),
      ACE_NTOHL (0x65736372),
      ACE_NTOHL (0x69707469),
      ACE_NTOHL (0x6f6e3a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDescription:1.0
      21,
      ACE_NTOHL (0x45786365),
      ACE_NTOHL (0x7074696f),
      ACE_NTOHL (0x6e446573),
      ACE_NTOHL (0x63726970),
      ACE_NTOHL (0x74696f6e),
      ACE_NTOHL (0x0),  // name = ExceptionDescription
      5, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      3,
      ACE_NTOHL (0x69640000),  // name = id
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      11,
      ACE_NTOHL (0x64656669),
      ACE_NTOHL (0x6e65645f),
      ACE_NTOHL (0x696e0000),  // name = defined_in
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      8,
      ACE_NTOHL (0x76657273),
      ACE_NTOHL (0x696f6e00),  // name = version
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        34,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5665),
        ACE_NTOHL (0x7273696f),
        ACE_NTOHL (0x6e537065),
        ACE_NTOHL (0x633a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
        12,
        ACE_NTOHL (0x56657273),
        ACE_NTOHL (0x696f6e53),
        ACE_NTOHL (0x70656300),  // name = VersionSpec
        CORBA::tk_string,
        0U, // string length

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ExcDescriptionSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ExcDescriptionSeq),
    (char *) &_oc_CORBA_ExcDescriptionSeq,
    0,
    sizeof (CORBA::ExcDescriptionSeq)
  );

static const CORBA::Long _oc_CORBA_AttributeMode[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4174),
  ACE_NTOHL (0x74726962),
  ACE_NTOHL (0x7574654d),
  ACE_NTOHL (0x6f64653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/AttributeMode:1.0
  14,
  ACE_NTOHL (0x41747472),
  ACE_NTOHL (0x69627574),
  ACE_NTOHL (0x654d6f64),
  ACE_NTOHL (0x65000000),  // name = AttributeMode
  2, // member count
  12,
  ACE_NTOHL (0x41545452),
  ACE_NTOHL (0x5f4e4f52),
  ACE_NTOHL (0x4d414c00),  // name = ATTR_NORMAL
  14,
  ACE_NTOHL (0x41545452),
  ACE_NTOHL (0x5f524541),
  ACE_NTOHL (0x444f4e4c),
  ACE_NTOHL (0x59000000),  // name = ATTR_READONLY
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AttributeMode (
    CORBA::tk_enum,
    sizeof (_oc_CORBA_AttributeMode),
    (char *) &_oc_CORBA_AttributeMode,
    0,
    sizeof (CORBA::AttributeMode)
  );

static const CORBA::Long _oc_CORBA_AttributeDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4174),
  ACE_NTOHL (0x74726962),
  ACE_NTOHL (0x75746544),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AttributeDef:1.0
  13,
  ACE_NTOHL (0x41747472),
  ACE_NTOHL (0x69627574),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x0),  // name = AttributeDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AttributeDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_AttributeDef),
    (char *) &_oc_CORBA_AttributeDef,
    0,
    sizeof (CORBA::AttributeDef)
  );

static const CORBA::Long _oc_CORBA_AttributeDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4174),
  ACE_NTOHL (0x74726962),
  ACE_NTOHL (0x75746544),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AttributeDescription:1.0
  21,
  ACE_NTOHL (0x41747472),
  ACE_NTOHL (0x69627574),
  ACE_NTOHL (0x65446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = AttributeDescription
  6, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  5,
  ACE_NTOHL (0x6d6f6465),
  ACE_NTOHL (0x0),  // name = mode
  CORBA::tk_enum, // typecode kind
  104, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4174),
    ACE_NTOHL (0x74726962),
    ACE_NTOHL (0x7574654d),
    ACE_NTOHL (0x6f64653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/AttributeMode:1.0
    14,
    ACE_NTOHL (0x41747472),
    ACE_NTOHL (0x69627574),
    ACE_NTOHL (0x654d6f64),
    ACE_NTOHL (0x65000000),  // name = AttributeMode
    2, // member count
    12,
    ACE_NTOHL (0x41545452),
    ACE_NTOHL (0x5f4e4f52),
    ACE_NTOHL (0x4d414c00),  // name = ATTR_NORMAL
    14,
    ACE_NTOHL (0x41545452),
    ACE_NTOHL (0x5f524541),
    ACE_NTOHL (0x444f4e4c),
    ACE_NTOHL (0x59000000),  // name = ATTR_READONLY

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AttributeDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_AttributeDescription),
    (char *) &_oc_CORBA_AttributeDescription,
    0,
    sizeof (CORBA::AttributeDescription)
  );

static const CORBA::Long _oc_CORBA_OperationMode[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4f70),
  ACE_NTOHL (0x65726174),
  ACE_NTOHL (0x696f6e4d),
  ACE_NTOHL (0x6f64653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/OperationMode:1.0
  14,
  ACE_NTOHL (0x4f706572),
  ACE_NTOHL (0x6174696f),
  ACE_NTOHL (0x6e4d6f64),
  ACE_NTOHL (0x65000000),  // name = OperationMode
  2, // member count
  10,
  ACE_NTOHL (0x4f505f4e),
  ACE_NTOHL (0x4f524d41),
  ACE_NTOHL (0x4c000000),  // name = OP_NORMAL
  10,
  ACE_NTOHL (0x4f505f4f),
  ACE_NTOHL (0x4e455741),
  ACE_NTOHL (0x59000000),  // name = OP_ONEWAY
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_OperationMode (
    CORBA::tk_enum,
    sizeof (_oc_CORBA_OperationMode),
    (char *) &_oc_CORBA_OperationMode,
    0,
    sizeof (CORBA::OperationMode)
  );

static const CORBA::Long _oc_CORBA_ParameterMode[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5061),
  ACE_NTOHL (0x72616d65),
  ACE_NTOHL (0x7465724d),
  ACE_NTOHL (0x6f64653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParameterMode:1.0
  14,
  ACE_NTOHL (0x50617261),
  ACE_NTOHL (0x6d657465),
  ACE_NTOHL (0x724d6f64),
  ACE_NTOHL (0x65000000),  // name = ParameterMode
  3, // member count
  9,
  ACE_NTOHL (0x50415241),
  ACE_NTOHL (0x4d5f494e),
  ACE_NTOHL (0x0),  // name = PARAM_IN
  10,
  ACE_NTOHL (0x50415241),
  ACE_NTOHL (0x4d5f4f55),
  ACE_NTOHL (0x54000000),  // name = PARAM_OUT
  12,
  ACE_NTOHL (0x50415241),
  ACE_NTOHL (0x4d5f494e),
  ACE_NTOHL (0x4f555400),  // name = PARAM_INOUT
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ParameterMode (
    CORBA::tk_enum,
    sizeof (_oc_CORBA_ParameterMode),
    (char *) &_oc_CORBA_ParameterMode,
    0,
    sizeof (CORBA::ParameterMode)
  );

static const CORBA::Long _oc_CORBA_ParameterDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5061),
  ACE_NTOHL (0x72616d65),
  ACE_NTOHL (0x74657244),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ParameterDescription:1.0
  21,
  ACE_NTOHL (0x50617261),
  ACE_NTOHL (0x6d657465),
  ACE_NTOHL (0x72446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = ParameterDescription
  4, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  9,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x5f646566),
  ACE_NTOHL (0x0),  // name = type_def
  CORBA::tk_objref, // typecode kind
  52, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    30,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4944),
    ACE_NTOHL (0x4c547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
    8,
    ACE_NTOHL (0x49444c54),
    ACE_NTOHL (0x79706500),  // name = IDLType

  5,
  ACE_NTOHL (0x6d6f6465),
  ACE_NTOHL (0x0),  // name = mode
  CORBA::tk_enum, // typecode kind
  116, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5061),
    ACE_NTOHL (0x72616d65),
    ACE_NTOHL (0x7465724d),
    ACE_NTOHL (0x6f64653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParameterMode:1.0
    14,
    ACE_NTOHL (0x50617261),
    ACE_NTOHL (0x6d657465),
    ACE_NTOHL (0x724d6f64),
    ACE_NTOHL (0x65000000),  // name = ParameterMode
    3, // member count
    9,
    ACE_NTOHL (0x50415241),
    ACE_NTOHL (0x4d5f494e),
    ACE_NTOHL (0x0),  // name = PARAM_IN
    10,
    ACE_NTOHL (0x50415241),
    ACE_NTOHL (0x4d5f4f55),
    ACE_NTOHL (0x54000000),  // name = PARAM_OUT
    12,
    ACE_NTOHL (0x50415241),
    ACE_NTOHL (0x4d5f494e),
    ACE_NTOHL (0x4f555400),  // name = PARAM_INOUT

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ParameterDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ParameterDescription),
    (char *) &_oc_CORBA_ParameterDescription,
    0,
    sizeof (CORBA::ParameterDescription)
  );

static const CORBA::Long _oc_CORBA_ParDescriptionSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5061),
  ACE_NTOHL (0x72446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x5365713a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParDescriptionSeq:1.0
  18,
  ACE_NTOHL (0x50617244),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e5365),
  ACE_NTOHL (0x71000000),  // name = ParDescriptionSeq
  CORBA::tk_sequence, // typecode kind
  416, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    400, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      43,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5061),
      ACE_NTOHL (0x72616d65),
      ACE_NTOHL (0x74657244),
      ACE_NTOHL (0x65736372),
      ACE_NTOHL (0x69707469),
      ACE_NTOHL (0x6f6e3a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ParameterDescription:1.0
      21,
      ACE_NTOHL (0x50617261),
      ACE_NTOHL (0x6d657465),
      ACE_NTOHL (0x72446573),
      ACE_NTOHL (0x63726970),
      ACE_NTOHL (0x74696f6e),
      ACE_NTOHL (0x0),  // name = ParameterDescription
      4, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,

      9,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x5f646566),
      ACE_NTOHL (0x0),  // name = type_def
      CORBA::tk_objref, // typecode kind
      52, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        30,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4944),
        ACE_NTOHL (0x4c547970),
        ACE_NTOHL (0x653a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
        8,
        ACE_NTOHL (0x49444c54),
        ACE_NTOHL (0x79706500),  // name = IDLType

      5,
      ACE_NTOHL (0x6d6f6465),
      ACE_NTOHL (0x0),  // name = mode
      CORBA::tk_enum, // typecode kind
      116, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        36,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5061),
        ACE_NTOHL (0x72616d65),
        ACE_NTOHL (0x7465724d),
        ACE_NTOHL (0x6f64653a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParameterMode:1.0
        14,
        ACE_NTOHL (0x50617261),
        ACE_NTOHL (0x6d657465),
        ACE_NTOHL (0x724d6f64),
        ACE_NTOHL (0x65000000),  // name = ParameterMode
        3, // member count
        9,
        ACE_NTOHL (0x50415241),
        ACE_NTOHL (0x4d5f494e),
        ACE_NTOHL (0x0),  // name = PARAM_IN
        10,
        ACE_NTOHL (0x50415241),
        ACE_NTOHL (0x4d5f4f55),
        ACE_NTOHL (0x54000000),  // name = PARAM_OUT
        12,
        ACE_NTOHL (0x50415241),
        ACE_NTOHL (0x4d5f494e),
        ACE_NTOHL (0x4f555400),  // name = PARAM_INOUT


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ParDescriptionSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ParDescriptionSeq),
    (char *) &_oc_CORBA_ParDescriptionSeq,
    0,
    sizeof (CORBA::ParDescriptionSeq)
  );

static const CORBA::Long _oc_CORBA_ContextIdentifier[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e746578),
  ACE_NTOHL (0x74496465),
  ACE_NTOHL (0x6e746966),
  ACE_NTOHL (0x6965723a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ContextIdentifier:1.0
  18,
  ACE_NTOHL (0x436f6e74),
  ACE_NTOHL (0x65787449),
  ACE_NTOHL (0x64656e74),
  ACE_NTOHL (0x69666965),
  ACE_NTOHL (0x72000000),  // name = ContextIdentifier
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ContextIdentifier (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ContextIdentifier),
    (char *) &_oc_CORBA_ContextIdentifier,
    0,
    sizeof (CORBA::ContextIdentifier)
  );

static const CORBA::Long _oc_CORBA_ContextIdSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f436f),
  ACE_NTOHL (0x6e746578),
  ACE_NTOHL (0x74496453),
  ACE_NTOHL (0x65713a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ContextIdSeq:1.0
  13,
  ACE_NTOHL (0x436f6e74),
  ACE_NTOHL (0x65787449),
  ACE_NTOHL (0x64536571),
  ACE_NTOHL (0x0),  // name = ContextIdSeq
  CORBA::tk_sequence, // typecode kind
  164, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_alias, // typecode kind for typedefs
    148, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      40,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f436f),
      ACE_NTOHL (0x6e746578),
      ACE_NTOHL (0x74496465),
      ACE_NTOHL (0x6e746966),
      ACE_NTOHL (0x6965723a),
      ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ContextIdentifier:1.0
      18,
      ACE_NTOHL (0x436f6e74),
      ACE_NTOHL (0x65787449),
      ACE_NTOHL (0x64656e74),
      ACE_NTOHL (0x69666965),
      ACE_NTOHL (0x72000000),  // name = ContextIdentifier
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ContextIdSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ContextIdSeq),
    (char *) &_oc_CORBA_ContextIdSeq,
    0,
    sizeof (CORBA::ContextIdSeq)
  );

static const CORBA::Long _oc_CORBA_OperationDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4f70),
  ACE_NTOHL (0x65726174),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/OperationDef:1.0
  13,
  ACE_NTOHL (0x4f706572),
  ACE_NTOHL (0x6174696f),
  ACE_NTOHL (0x6e446566),
  ACE_NTOHL (0x0),  // name = OperationDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_OperationDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_OperationDef),
    (char *) &_oc_CORBA_OperationDef,
    0,
    sizeof (CORBA::OperationDef)
  );

static const CORBA::Long _oc_CORBA_OperationDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4f70),
  ACE_NTOHL (0x65726174),
  ACE_NTOHL (0x696f6e44),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/OperationDescription:1.0
  21,
  ACE_NTOHL (0x4f706572),
  ACE_NTOHL (0x6174696f),
  ACE_NTOHL (0x6e446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = OperationDescription
  9, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  7,
  ACE_NTOHL (0x72657375),
  ACE_NTOHL (0x6c740000),  // name = result
  CORBA::tk_TypeCode,

  5,
  ACE_NTOHL (0x6d6f6465),
  ACE_NTOHL (0x0),  // name = mode
  CORBA::tk_enum, // typecode kind
  100, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    36,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4f70),
    ACE_NTOHL (0x65726174),
    ACE_NTOHL (0x696f6e4d),
    ACE_NTOHL (0x6f64653a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/OperationMode:1.0
    14,
    ACE_NTOHL (0x4f706572),
    ACE_NTOHL (0x6174696f),
    ACE_NTOHL (0x6e4d6f64),
    ACE_NTOHL (0x65000000),  // name = OperationMode
    2, // member count
    10,
    ACE_NTOHL (0x4f505f4e),
    ACE_NTOHL (0x4f524d41),
    ACE_NTOHL (0x4c000000),  // name = OP_NORMAL
    10,
    ACE_NTOHL (0x4f505f4f),
    ACE_NTOHL (0x4e455741),
    ACE_NTOHL (0x59000000),  // name = OP_ONEWAY

  9,
  ACE_NTOHL (0x636f6e74),
  ACE_NTOHL (0x65787473),
  ACE_NTOHL (0x0),  // name = contexts
  CORBA::tk_alias, // typecode kind for typedefs
  236, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f436f),
    ACE_NTOHL (0x6e746578),
    ACE_NTOHL (0x74496453),
    ACE_NTOHL (0x65713a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ContextIdSeq:1.0
    13,
    ACE_NTOHL (0x436f6e74),
    ACE_NTOHL (0x65787449),
    ACE_NTOHL (0x64536571),
    ACE_NTOHL (0x0),  // name = ContextIdSeq
    CORBA::tk_sequence, // typecode kind
    164, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_alias, // typecode kind for typedefs
      148, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        40,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f436f),
        ACE_NTOHL (0x6e746578),
        ACE_NTOHL (0x74496465),
        ACE_NTOHL (0x6e746966),
        ACE_NTOHL (0x6965723a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ContextIdentifier:1.0
        18,
        ACE_NTOHL (0x436f6e74),
        ACE_NTOHL (0x65787449),
        ACE_NTOHL (0x64656e74),
        ACE_NTOHL (0x69666965),
        ACE_NTOHL (0x72000000),  // name = ContextIdentifier
        CORBA::tk_alias, // typecode kind for typedefs
        68, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          33,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4964),
          ACE_NTOHL (0x656e7469),
          ACE_NTOHL (0x66696572),
          ACE_NTOHL (0x3a312e30),
          ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
          11,
          ACE_NTOHL (0x4964656e),
          ACE_NTOHL (0x74696669),
          ACE_NTOHL (0x65720000),  // name = Identifier
          CORBA::tk_string,
          0U, // string length


      0U,


  11,
  ACE_NTOHL (0x70617261),
  ACE_NTOHL (0x6d657465),
  ACE_NTOHL (0x72730000),  // name = parameters
  CORBA::tk_alias, // typecode kind for typedefs
  496, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    40,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5061),
    ACE_NTOHL (0x72446573),
    ACE_NTOHL (0x63726970),
    ACE_NTOHL (0x74696f6e),
    ACE_NTOHL (0x5365713a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParDescriptionSeq:1.0
    18,
    ACE_NTOHL (0x50617244),
    ACE_NTOHL (0x65736372),
    ACE_NTOHL (0x69707469),
    ACE_NTOHL (0x6f6e5365),
    ACE_NTOHL (0x71000000),  // name = ParDescriptionSeq
    CORBA::tk_sequence, // typecode kind
    416, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_struct, // typecode kind
      400, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        43,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5061),
        ACE_NTOHL (0x72616d65),
        ACE_NTOHL (0x74657244),
        ACE_NTOHL (0x65736372),
        ACE_NTOHL (0x69707469),
        ACE_NTOHL (0x6f6e3a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ParameterDescription:1.0
        21,
        ACE_NTOHL (0x50617261),
        ACE_NTOHL (0x6d657465),
        ACE_NTOHL (0x72446573),
        ACE_NTOHL (0x63726970),
        ACE_NTOHL (0x74696f6e),
        ACE_NTOHL (0x0),  // name = ParameterDescription
        4, // member count
        5,
        ACE_NTOHL (0x6e616d65),
        ACE_NTOHL (0x0),  // name = name
        CORBA::tk_alias, // typecode kind for typedefs
        68, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          33,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4964),
          ACE_NTOHL (0x656e7469),
          ACE_NTOHL (0x66696572),
          ACE_NTOHL (0x3a312e30),
          ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
          11,
          ACE_NTOHL (0x4964656e),
          ACE_NTOHL (0x74696669),
          ACE_NTOHL (0x65720000),  // name = Identifier
          CORBA::tk_string,
          0U, // string length

        5,
        ACE_NTOHL (0x74797065),
        ACE_NTOHL (0x0),  // name = type
        CORBA::tk_TypeCode,

        9,
        ACE_NTOHL (0x74797065),
        ACE_NTOHL (0x5f646566),
        ACE_NTOHL (0x0),  // name = type_def
        CORBA::tk_objref, // typecode kind
        52, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          30,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4944),
          ACE_NTOHL (0x4c547970),
          ACE_NTOHL (0x653a312e),
          ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
          8,
          ACE_NTOHL (0x49444c54),
          ACE_NTOHL (0x79706500),  // name = IDLType

        5,
        ACE_NTOHL (0x6d6f6465),
        ACE_NTOHL (0x0),  // name = mode
        CORBA::tk_enum, // typecode kind
        116, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          36,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f5061),
          ACE_NTOHL (0x72616d65),
          ACE_NTOHL (0x7465724d),
          ACE_NTOHL (0x6f64653a),
          ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParameterMode:1.0
          14,
          ACE_NTOHL (0x50617261),
          ACE_NTOHL (0x6d657465),
          ACE_NTOHL (0x724d6f64),
          ACE_NTOHL (0x65000000),  // name = ParameterMode
          3, // member count
          9,
          ACE_NTOHL (0x50415241),
          ACE_NTOHL (0x4d5f494e),
          ACE_NTOHL (0x0),  // name = PARAM_IN
          10,
          ACE_NTOHL (0x50415241),
          ACE_NTOHL (0x4d5f4f55),
          ACE_NTOHL (0x54000000),  // name = PARAM_OUT
          12,
          ACE_NTOHL (0x50415241),
          ACE_NTOHL (0x4d5f494e),
          ACE_NTOHL (0x4f555400),  // name = PARAM_INOUT


      0U,


  11,
  ACE_NTOHL (0x65786365),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e730000),  // name = exceptions
  CORBA::tk_alias, // typecode kind for typedefs
  556, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    40,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4578),
    ACE_NTOHL (0x63446573),
    ACE_NTOHL (0x63726970),
    ACE_NTOHL (0x74696f6e),
    ACE_NTOHL (0x5365713a),
    ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ExcDescriptionSeq:1.0
    18,
    ACE_NTOHL (0x45786344),
    ACE_NTOHL (0x65736372),
    ACE_NTOHL (0x69707469),
    ACE_NTOHL (0x6f6e5365),
    ACE_NTOHL (0x71000000),  // name = ExcDescriptionSeq
    CORBA::tk_sequence, // typecode kind
    476, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_struct, // typecode kind
      460, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        43,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4578),
        ACE_NTOHL (0x63657074),
        ACE_NTOHL (0x696f6e44),
        ACE_NTOHL (0x65736372),
        ACE_NTOHL (0x69707469),
        ACE_NTOHL (0x6f6e3a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDescription:1.0
        21,
        ACE_NTOHL (0x45786365),
        ACE_NTOHL (0x7074696f),
        ACE_NTOHL (0x6e446573),
        ACE_NTOHL (0x63726970),
        ACE_NTOHL (0x74696f6e),
        ACE_NTOHL (0x0),  // name = ExceptionDescription
        5, // member count
        5,
        ACE_NTOHL (0x6e616d65),
        ACE_NTOHL (0x0),  // name = name
        CORBA::tk_alias, // typecode kind for typedefs
        68, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          33,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f4964),
          ACE_NTOHL (0x656e7469),
          ACE_NTOHL (0x66696572),
          ACE_NTOHL (0x3a312e30),
          ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
          11,
          ACE_NTOHL (0x4964656e),
          ACE_NTOHL (0x74696669),
          ACE_NTOHL (0x65720000),  // name = Identifier
          CORBA::tk_string,
          0U, // string length

        3,
        ACE_NTOHL (0x69640000),  // name = id
        CORBA::tk_alias, // typecode kind for typedefs
        72, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          35,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f5265),
          ACE_NTOHL (0x706f7369),
          ACE_NTOHL (0x746f7279),
          ACE_NTOHL (0x49643a31),
          ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
          13,
          ACE_NTOHL (0x5265706f),
          ACE_NTOHL (0x7369746f),
          ACE_NTOHL (0x72794964),
          ACE_NTOHL (0x0),  // name = RepositoryId
          CORBA::tk_string,
          0U, // string length

        11,
        ACE_NTOHL (0x64656669),
        ACE_NTOHL (0x6e65645f),
        ACE_NTOHL (0x696e0000),  // name = defined_in
        CORBA::tk_alias, // typecode kind for typedefs
        72, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          35,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f5265),
          ACE_NTOHL (0x706f7369),
          ACE_NTOHL (0x746f7279),
          ACE_NTOHL (0x49643a31),
          ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
          13,
          ACE_NTOHL (0x5265706f),
          ACE_NTOHL (0x7369746f),
          ACE_NTOHL (0x72794964),
          ACE_NTOHL (0x0),  // name = RepositoryId
          CORBA::tk_string,
          0U, // string length

        8,
        ACE_NTOHL (0x76657273),
        ACE_NTOHL (0x696f6e00),  // name = version
        CORBA::tk_alias, // typecode kind for typedefs
        68, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          34,
          ACE_NTOHL (0x49444c3a),
          ACE_NTOHL (0x6f6d672e),
          ACE_NTOHL (0x6f72672f),
          ACE_NTOHL (0x434f5242),
          ACE_NTOHL (0x412f5665),
          ACE_NTOHL (0x7273696f),
          ACE_NTOHL (0x6e537065),
          ACE_NTOHL (0x633a312e),
          ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
          12,
          ACE_NTOHL (0x56657273),
          ACE_NTOHL (0x696f6e53),
          ACE_NTOHL (0x70656300),  // name = VersionSpec
          CORBA::tk_string,
          0U, // string length

        5,
        ACE_NTOHL (0x74797065),
        ACE_NTOHL (0x0),  // name = type
        CORBA::tk_TypeCode,


      0U,


};

static CORBA::TypeCode _tc_TAO_tc_CORBA_OperationDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_OperationDescription),
    (char *) &_oc_CORBA_OperationDescription,
    0,
    sizeof (CORBA::OperationDescription)
  );

static const CORBA::Long _oc_CORBA_RepositoryIdSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5265),
  ACE_NTOHL (0x706f7369),
  ACE_NTOHL (0x746f7279),
  ACE_NTOHL (0x49645365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/RepositoryIdSeq:1.0
  16,
  ACE_NTOHL (0x5265706f),
  ACE_NTOHL (0x7369746f),
  ACE_NTOHL (0x72794964),
  ACE_NTOHL (0x53657100),  // name = RepositoryIdSeq
  CORBA::tk_sequence, // typecode kind
  88, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_alias, // typecode kind for typedefs
    72, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      35,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5265),
      ACE_NTOHL (0x706f7369),
      ACE_NTOHL (0x746f7279),
      ACE_NTOHL (0x49643a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
      13,
      ACE_NTOHL (0x5265706f),
      ACE_NTOHL (0x7369746f),
      ACE_NTOHL (0x72794964),
      ACE_NTOHL (0x0),  // name = RepositoryId
      CORBA::tk_string,
      0U, // string length

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_RepositoryIdSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_RepositoryIdSeq),
    (char *) &_oc_CORBA_RepositoryIdSeq,
    0,
    sizeof (CORBA::RepositoryIdSeq)
  );

static const CORBA::Long _oc_CORBA_OpDescriptionSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  39,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4f70),
  ACE_NTOHL (0x44657363),
  ACE_NTOHL (0x72697074),
  ACE_NTOHL (0x696f6e53),
  ACE_NTOHL (0x65713a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/OpDescriptionSeq:1.0
  17,
  ACE_NTOHL (0x4f704465),
  ACE_NTOHL (0x73637269),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e536571),
  ACE_NTOHL (0x0),  // name = OpDescriptionSeq
  CORBA::tk_sequence, // typecode kind
  1956, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    1940, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      43,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4f70),
      ACE_NTOHL (0x65726174),
      ACE_NTOHL (0x696f6e44),
      ACE_NTOHL (0x65736372),
      ACE_NTOHL (0x69707469),
      ACE_NTOHL (0x6f6e3a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/OperationDescription:1.0
      21,
      ACE_NTOHL (0x4f706572),
      ACE_NTOHL (0x6174696f),
      ACE_NTOHL (0x6e446573),
      ACE_NTOHL (0x63726970),
      ACE_NTOHL (0x74696f6e),
      ACE_NTOHL (0x0),  // name = OperationDescription
      9, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      3,
      ACE_NTOHL (0x69640000),  // name = id
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      11,
      ACE_NTOHL (0x64656669),
      ACE_NTOHL (0x6e65645f),
      ACE_NTOHL (0x696e0000),  // name = defined_in
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      8,
      ACE_NTOHL (0x76657273),
      ACE_NTOHL (0x696f6e00),  // name = version
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        34,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5665),
        ACE_NTOHL (0x7273696f),
        ACE_NTOHL (0x6e537065),
        ACE_NTOHL (0x633a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
        12,
        ACE_NTOHL (0x56657273),
        ACE_NTOHL (0x696f6e53),
        ACE_NTOHL (0x70656300),  // name = VersionSpec
        CORBA::tk_string,
        0U, // string length

      7,
      ACE_NTOHL (0x72657375),
      ACE_NTOHL (0x6c740000),  // name = result
      CORBA::tk_TypeCode,

      5,
      ACE_NTOHL (0x6d6f6465),
      ACE_NTOHL (0x0),  // name = mode
      CORBA::tk_enum, // typecode kind
      100, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        36,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4f70),
        ACE_NTOHL (0x65726174),
        ACE_NTOHL (0x696f6e4d),
        ACE_NTOHL (0x6f64653a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/OperationMode:1.0
        14,
        ACE_NTOHL (0x4f706572),
        ACE_NTOHL (0x6174696f),
        ACE_NTOHL (0x6e4d6f64),
        ACE_NTOHL (0x65000000),  // name = OperationMode
        2, // member count
        10,
        ACE_NTOHL (0x4f505f4e),
        ACE_NTOHL (0x4f524d41),
        ACE_NTOHL (0x4c000000),  // name = OP_NORMAL
        10,
        ACE_NTOHL (0x4f505f4f),
        ACE_NTOHL (0x4e455741),
        ACE_NTOHL (0x59000000),  // name = OP_ONEWAY

      9,
      ACE_NTOHL (0x636f6e74),
      ACE_NTOHL (0x65787473),
      ACE_NTOHL (0x0),  // name = contexts
      CORBA::tk_alias, // typecode kind for typedefs
      236, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f436f),
        ACE_NTOHL (0x6e746578),
        ACE_NTOHL (0x74496453),
        ACE_NTOHL (0x65713a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ContextIdSeq:1.0
        13,
        ACE_NTOHL (0x436f6e74),
        ACE_NTOHL (0x65787449),
        ACE_NTOHL (0x64536571),
        ACE_NTOHL (0x0),  // name = ContextIdSeq
        CORBA::tk_sequence, // typecode kind
        164, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_alias, // typecode kind for typedefs
          148, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            40,
            ACE_NTOHL (0x49444c3a),
            ACE_NTOHL (0x6f6d672e),
            ACE_NTOHL (0x6f72672f),
            ACE_NTOHL (0x434f5242),
            ACE_NTOHL (0x412f436f),
            ACE_NTOHL (0x6e746578),
            ACE_NTOHL (0x74496465),
            ACE_NTOHL (0x6e746966),
            ACE_NTOHL (0x6965723a),
            ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ContextIdentifier:1.0
            18,
            ACE_NTOHL (0x436f6e74),
            ACE_NTOHL (0x65787449),
            ACE_NTOHL (0x64656e74),
            ACE_NTOHL (0x69666965),
            ACE_NTOHL (0x72000000),  // name = ContextIdentifier
            CORBA::tk_alias, // typecode kind for typedefs
            68, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              33,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4964),
              ACE_NTOHL (0x656e7469),
              ACE_NTOHL (0x66696572),
              ACE_NTOHL (0x3a312e30),
              ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
              11,
              ACE_NTOHL (0x4964656e),
              ACE_NTOHL (0x74696669),
              ACE_NTOHL (0x65720000),  // name = Identifier
              CORBA::tk_string,
              0U, // string length


          0U,


      11,
      ACE_NTOHL (0x70617261),
      ACE_NTOHL (0x6d657465),
      ACE_NTOHL (0x72730000),  // name = parameters
      CORBA::tk_alias, // typecode kind for typedefs
      496, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        40,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5061),
        ACE_NTOHL (0x72446573),
        ACE_NTOHL (0x63726970),
        ACE_NTOHL (0x74696f6e),
        ACE_NTOHL (0x5365713a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParDescriptionSeq:1.0
        18,
        ACE_NTOHL (0x50617244),
        ACE_NTOHL (0x65736372),
        ACE_NTOHL (0x69707469),
        ACE_NTOHL (0x6f6e5365),
        ACE_NTOHL (0x71000000),  // name = ParDescriptionSeq
        CORBA::tk_sequence, // typecode kind
        416, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_struct, // typecode kind
          400, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            43,
            ACE_NTOHL (0x49444c3a),
            ACE_NTOHL (0x6f6d672e),
            ACE_NTOHL (0x6f72672f),
            ACE_NTOHL (0x434f5242),
            ACE_NTOHL (0x412f5061),
            ACE_NTOHL (0x72616d65),
            ACE_NTOHL (0x74657244),
            ACE_NTOHL (0x65736372),
            ACE_NTOHL (0x69707469),
            ACE_NTOHL (0x6f6e3a31),
            ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ParameterDescription:1.0
            21,
            ACE_NTOHL (0x50617261),
            ACE_NTOHL (0x6d657465),
            ACE_NTOHL (0x72446573),
            ACE_NTOHL (0x63726970),
            ACE_NTOHL (0x74696f6e),
            ACE_NTOHL (0x0),  // name = ParameterDescription
            4, // member count
            5,
            ACE_NTOHL (0x6e616d65),
            ACE_NTOHL (0x0),  // name = name
            CORBA::tk_alias, // typecode kind for typedefs
            68, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              33,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4964),
              ACE_NTOHL (0x656e7469),
              ACE_NTOHL (0x66696572),
              ACE_NTOHL (0x3a312e30),
              ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
              11,
              ACE_NTOHL (0x4964656e),
              ACE_NTOHL (0x74696669),
              ACE_NTOHL (0x65720000),  // name = Identifier
              CORBA::tk_string,
              0U, // string length

            5,
            ACE_NTOHL (0x74797065),
            ACE_NTOHL (0x0),  // name = type
            CORBA::tk_TypeCode,

            9,
            ACE_NTOHL (0x74797065),
            ACE_NTOHL (0x5f646566),
            ACE_NTOHL (0x0),  // name = type_def
            CORBA::tk_objref, // typecode kind
            52, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              30,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4944),
              ACE_NTOHL (0x4c547970),
              ACE_NTOHL (0x653a312e),
              ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
              8,
              ACE_NTOHL (0x49444c54),
              ACE_NTOHL (0x79706500),  // name = IDLType

            5,
            ACE_NTOHL (0x6d6f6465),
            ACE_NTOHL (0x0),  // name = mode
            CORBA::tk_enum, // typecode kind
            116, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              36,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f5061),
              ACE_NTOHL (0x72616d65),
              ACE_NTOHL (0x7465724d),
              ACE_NTOHL (0x6f64653a),
              ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ParameterMode:1.0
              14,
              ACE_NTOHL (0x50617261),
              ACE_NTOHL (0x6d657465),
              ACE_NTOHL (0x724d6f64),
              ACE_NTOHL (0x65000000),  // name = ParameterMode
              3, // member count
              9,
              ACE_NTOHL (0x50415241),
              ACE_NTOHL (0x4d5f494e),
              ACE_NTOHL (0x0),  // name = PARAM_IN
              10,
              ACE_NTOHL (0x50415241),
              ACE_NTOHL (0x4d5f4f55),
              ACE_NTOHL (0x54000000),  // name = PARAM_OUT
              12,
              ACE_NTOHL (0x50415241),
              ACE_NTOHL (0x4d5f494e),
              ACE_NTOHL (0x4f555400),  // name = PARAM_INOUT


          0U,


      11,
      ACE_NTOHL (0x65786365),
      ACE_NTOHL (0x7074696f),
      ACE_NTOHL (0x6e730000),  // name = exceptions
      CORBA::tk_alias, // typecode kind for typedefs
      556, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        40,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4578),
        ACE_NTOHL (0x63446573),
        ACE_NTOHL (0x63726970),
        ACE_NTOHL (0x74696f6e),
        ACE_NTOHL (0x5365713a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/ExcDescriptionSeq:1.0
        18,
        ACE_NTOHL (0x45786344),
        ACE_NTOHL (0x65736372),
        ACE_NTOHL (0x69707469),
        ACE_NTOHL (0x6f6e5365),
        ACE_NTOHL (0x71000000),  // name = ExcDescriptionSeq
        CORBA::tk_sequence, // typecode kind
        476, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_struct, // typecode kind
          460, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            43,
            ACE_NTOHL (0x49444c3a),
            ACE_NTOHL (0x6f6d672e),
            ACE_NTOHL (0x6f72672f),
            ACE_NTOHL (0x434f5242),
            ACE_NTOHL (0x412f4578),
            ACE_NTOHL (0x63657074),
            ACE_NTOHL (0x696f6e44),
            ACE_NTOHL (0x65736372),
            ACE_NTOHL (0x69707469),
            ACE_NTOHL (0x6f6e3a31),
            ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ExceptionDescription:1.0
            21,
            ACE_NTOHL (0x45786365),
            ACE_NTOHL (0x7074696f),
            ACE_NTOHL (0x6e446573),
            ACE_NTOHL (0x63726970),
            ACE_NTOHL (0x74696f6e),
            ACE_NTOHL (0x0),  // name = ExceptionDescription
            5, // member count
            5,
            ACE_NTOHL (0x6e616d65),
            ACE_NTOHL (0x0),  // name = name
            CORBA::tk_alias, // typecode kind for typedefs
            68, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              33,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f4964),
              ACE_NTOHL (0x656e7469),
              ACE_NTOHL (0x66696572),
              ACE_NTOHL (0x3a312e30),
              ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
              11,
              ACE_NTOHL (0x4964656e),
              ACE_NTOHL (0x74696669),
              ACE_NTOHL (0x65720000),  // name = Identifier
              CORBA::tk_string,
              0U, // string length

            3,
            ACE_NTOHL (0x69640000),  // name = id
            CORBA::tk_alias, // typecode kind for typedefs
            72, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              35,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f5265),
              ACE_NTOHL (0x706f7369),
              ACE_NTOHL (0x746f7279),
              ACE_NTOHL (0x49643a31),
              ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
              13,
              ACE_NTOHL (0x5265706f),
              ACE_NTOHL (0x7369746f),
              ACE_NTOHL (0x72794964),
              ACE_NTOHL (0x0),  // name = RepositoryId
              CORBA::tk_string,
              0U, // string length

            11,
            ACE_NTOHL (0x64656669),
            ACE_NTOHL (0x6e65645f),
            ACE_NTOHL (0x696e0000),  // name = defined_in
            CORBA::tk_alias, // typecode kind for typedefs
            72, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              35,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f5265),
              ACE_NTOHL (0x706f7369),
              ACE_NTOHL (0x746f7279),
              ACE_NTOHL (0x49643a31),
              ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
              13,
              ACE_NTOHL (0x5265706f),
              ACE_NTOHL (0x7369746f),
              ACE_NTOHL (0x72794964),
              ACE_NTOHL (0x0),  // name = RepositoryId
              CORBA::tk_string,
              0U, // string length

            8,
            ACE_NTOHL (0x76657273),
            ACE_NTOHL (0x696f6e00),  // name = version
            CORBA::tk_alias, // typecode kind for typedefs
            68, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              34,
              ACE_NTOHL (0x49444c3a),
              ACE_NTOHL (0x6f6d672e),
              ACE_NTOHL (0x6f72672f),
              ACE_NTOHL (0x434f5242),
              ACE_NTOHL (0x412f5665),
              ACE_NTOHL (0x7273696f),
              ACE_NTOHL (0x6e537065),
              ACE_NTOHL (0x633a312e),
              ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
              12,
              ACE_NTOHL (0x56657273),
              ACE_NTOHL (0x696f6e53),
              ACE_NTOHL (0x70656300),  // name = VersionSpec
              CORBA::tk_string,
              0U, // string length

            5,
            ACE_NTOHL (0x74797065),
            ACE_NTOHL (0x0),  // name = type
            CORBA::tk_TypeCode,


          0U,



    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_OpDescriptionSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_OpDescriptionSeq),
    (char *) &_oc_CORBA_OpDescriptionSeq,
    0,
    sizeof (CORBA::OpDescriptionSeq)
  );

static const CORBA::Long _oc_CORBA_AttrDescriptionSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  41,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4174),
  ACE_NTOHL (0x74724465),
  ACE_NTOHL (0x73637269),
  ACE_NTOHL (0x7074696f),
  ACE_NTOHL (0x6e536571),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/AttrDescriptionSeq:1.0
  19,
  ACE_NTOHL (0x41747472),
  ACE_NTOHL (0x44657363),
  ACE_NTOHL (0x72697074),
  ACE_NTOHL (0x696f6e53),
  ACE_NTOHL (0x65710000),  // name = AttrDescriptionSeq
  CORBA::tk_sequence, // typecode kind
  600, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    584, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      43,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f4174),
      ACE_NTOHL (0x74726962),
      ACE_NTOHL (0x75746544),
      ACE_NTOHL (0x65736372),
      ACE_NTOHL (0x69707469),
      ACE_NTOHL (0x6f6e3a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AttributeDescription:1.0
      21,
      ACE_NTOHL (0x41747472),
      ACE_NTOHL (0x69627574),
      ACE_NTOHL (0x65446573),
      ACE_NTOHL (0x63726970),
      ACE_NTOHL (0x74696f6e),
      ACE_NTOHL (0x0),  // name = AttributeDescription
      6, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      3,
      ACE_NTOHL (0x69640000),  // name = id
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      11,
      ACE_NTOHL (0x64656669),
      ACE_NTOHL (0x6e65645f),
      ACE_NTOHL (0x696e0000),  // name = defined_in
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      8,
      ACE_NTOHL (0x76657273),
      ACE_NTOHL (0x696f6e00),  // name = version
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        34,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5665),
        ACE_NTOHL (0x7273696f),
        ACE_NTOHL (0x6e537065),
        ACE_NTOHL (0x633a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
        12,
        ACE_NTOHL (0x56657273),
        ACE_NTOHL (0x696f6e53),
        ACE_NTOHL (0x70656300),  // name = VersionSpec
        CORBA::tk_string,
        0U, // string length

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,

      5,
      ACE_NTOHL (0x6d6f6465),
      ACE_NTOHL (0x0),  // name = mode
      CORBA::tk_enum, // typecode kind
      104, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        36,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4174),
        ACE_NTOHL (0x74726962),
        ACE_NTOHL (0x7574654d),
        ACE_NTOHL (0x6f64653a),
        ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/AttributeMode:1.0
        14,
        ACE_NTOHL (0x41747472),
        ACE_NTOHL (0x69627574),
        ACE_NTOHL (0x654d6f64),
        ACE_NTOHL (0x65000000),  // name = AttributeMode
        2, // member count
        12,
        ACE_NTOHL (0x41545452),
        ACE_NTOHL (0x5f4e4f52),
        ACE_NTOHL (0x4d414c00),  // name = ATTR_NORMAL
        14,
        ACE_NTOHL (0x41545452),
        ACE_NTOHL (0x5f524541),
        ACE_NTOHL (0x444f4e4c),
        ACE_NTOHL (0x59000000),  // name = ATTR_READONLY


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AttrDescriptionSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_AttrDescriptionSeq),
    (char *) &_oc_CORBA_AttrDescriptionSeq,
    0,
    sizeof (CORBA::AttrDescriptionSeq)
  );

static const CORBA::Long _oc_CORBA_InterfaceDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f496e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636544),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/InterfaceDef:1.0
  13,
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x0),  // name = InterfaceDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_InterfaceDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_InterfaceDef),
    (char *) &_oc_CORBA_InterfaceDef,
    0,
    sizeof (CORBA::InterfaceDef)
  );

static const CORBA::Long _oc_CORBA_InterfaceDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f496e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636544),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/InterfaceDescription:1.0
  21,
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = InterfaceDescription
  5, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  16,
  ACE_NTOHL (0x62617365),
  ACE_NTOHL (0x5f696e74),
  ACE_NTOHL (0x65726661),
  ACE_NTOHL (0x63657300),  // name = base_interfaces
  CORBA::tk_alias, // typecode kind for typedefs
  164, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49645365),
    ACE_NTOHL (0x713a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/RepositoryIdSeq:1.0
    16,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x53657100),  // name = RepositoryIdSeq
    CORBA::tk_sequence, // typecode kind
    88, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      0U,


};

static CORBA::TypeCode _tc_TAO_tc_CORBA_InterfaceDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_InterfaceDescription),
    (char *) &_oc_CORBA_InterfaceDescription,
    0,
    sizeof (CORBA::InterfaceDescription)
  );

static const CORBA::Long _oc_CORBA_AbstractInterfaceDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4162),
  ACE_NTOHL (0x73747261),
  ACE_NTOHL (0x6374496e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636544),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/AbstractInterfaceDef:1.0
  21,
  ACE_NTOHL (0x41627374),
  ACE_NTOHL (0x72616374),
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x0),  // name = AbstractInterfaceDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_AbstractInterfaceDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_AbstractInterfaceDef),
    (char *) &_oc_CORBA_AbstractInterfaceDef,
    0,
    sizeof (CORBA::AbstractInterfaceDef)
  );

static const CORBA::Long _oc_CORBA_LocalInterfaceDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4c6f),
  ACE_NTOHL (0x63616c49),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x66616365),
  ACE_NTOHL (0x4465663a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CORBA/LocalInterfaceDef:1.0
  18,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x6c496e74),
  ACE_NTOHL (0x65726661),
  ACE_NTOHL (0x63654465),
  ACE_NTOHL (0x66000000),  // name = LocalInterfaceDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_LocalInterfaceDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_LocalInterfaceDef),
    (char *) &_oc_CORBA_LocalInterfaceDef,
    0,
    sizeof (CORBA::LocalInterfaceDef)
  );

static const CORBA::Long _oc_CORBA_FixedDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f4669),
  ACE_NTOHL (0x78656444),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/FixedDef:1.0
  9,
  ACE_NTOHL (0x46697865),
  ACE_NTOHL (0x64446566),
  ACE_NTOHL (0x0),  // name = FixedDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_FixedDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_FixedDef),
    (char *) &_oc_CORBA_FixedDef,
    0,
    sizeof (CORBA::FixedDef)
  );

static const CORBA::Long _oc_CORBA_ValueDefSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c756544),
  ACE_NTOHL (0x65665365),
  ACE_NTOHL (0x713a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ValueDefSeq:1.0
  12,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x53657100),  // name = ValueDefSeq
  CORBA::tk_sequence, // typecode kind
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_objref, // typecode kind
    56, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      31,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5661),
      ACE_NTOHL (0x6c756544),
      ACE_NTOHL (0x65663a31),
      ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ValueDef:1.0
      9,
      ACE_NTOHL (0x56616c75),
      ACE_NTOHL (0x65446566),
      ACE_NTOHL (0x0),  // name = ValueDef

    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueDefSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ValueDefSeq),
    (char *) &_oc_CORBA_ValueDefSeq,
    0,
    sizeof (CORBA::ValueDefSeq)
  );

static const CORBA::Long _oc_CORBA_ValueMember[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c75654d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x723a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ValueMember:1.0
  12,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x654d656d),
  ACE_NTOHL (0x62657200),  // name = ValueMember
  7, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  5,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x0),  // name = type
  CORBA::tk_TypeCode,

  9,
  ACE_NTOHL (0x74797065),
  ACE_NTOHL (0x5f646566),
  ACE_NTOHL (0x0),  // name = type_def
  CORBA::tk_objref, // typecode kind
  52, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    30,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4944),
    ACE_NTOHL (0x4c547970),
    ACE_NTOHL (0x653a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
    8,
    ACE_NTOHL (0x49444c54),
    ACE_NTOHL (0x79706500),  // name = IDLType

  7,
  ACE_NTOHL (0x61636365),
  ACE_NTOHL (0x73730000),  // name = access
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
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

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueMember (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ValueMember),
    (char *) &_oc_CORBA_ValueMember,
    0,
    sizeof (CORBA::ValueMember)
  );

static const CORBA::Long _oc_CORBA_ValueMemberSeq[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c75654d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x72536571),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/ValueMemberSeq:1.0
  15,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x654d656d),
  ACE_NTOHL (0x62657253),
  ACE_NTOHL (0x65710000),  // name = ValueMemberSeq
  CORBA::tk_sequence, // typecode kind
  616, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    600, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      34,
      ACE_NTOHL (0x49444c3a),
      ACE_NTOHL (0x6f6d672e),
      ACE_NTOHL (0x6f72672f),
      ACE_NTOHL (0x434f5242),
      ACE_NTOHL (0x412f5661),
      ACE_NTOHL (0x6c75654d),
      ACE_NTOHL (0x656d6265),
      ACE_NTOHL (0x723a312e),
      ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ValueMember:1.0
      12,
      ACE_NTOHL (0x56616c75),
      ACE_NTOHL (0x654d656d),
      ACE_NTOHL (0x62657200),  // name = ValueMember
      7, // member count
      5,
      ACE_NTOHL (0x6e616d65),
      ACE_NTOHL (0x0),  // name = name
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        33,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4964),
        ACE_NTOHL (0x656e7469),
        ACE_NTOHL (0x66696572),
        ACE_NTOHL (0x3a312e30),
        ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
        11,
        ACE_NTOHL (0x4964656e),
        ACE_NTOHL (0x74696669),
        ACE_NTOHL (0x65720000),  // name = Identifier
        CORBA::tk_string,
        0U, // string length

      3,
      ACE_NTOHL (0x69640000),  // name = id
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      11,
      ACE_NTOHL (0x64656669),
      ACE_NTOHL (0x6e65645f),
      ACE_NTOHL (0x696e0000),  // name = defined_in
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      8,
      ACE_NTOHL (0x76657273),
      ACE_NTOHL (0x696f6e00),  // name = version
      CORBA::tk_alias, // typecode kind for typedefs
      68, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        34,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5665),
        ACE_NTOHL (0x7273696f),
        ACE_NTOHL (0x6e537065),
        ACE_NTOHL (0x633a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
        12,
        ACE_NTOHL (0x56657273),
        ACE_NTOHL (0x696f6e53),
        ACE_NTOHL (0x70656300),  // name = VersionSpec
        CORBA::tk_string,
        0U, // string length

      5,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x0),  // name = type
      CORBA::tk_TypeCode,

      9,
      ACE_NTOHL (0x74797065),
      ACE_NTOHL (0x5f646566),
      ACE_NTOHL (0x0),  // name = type_def
      CORBA::tk_objref, // typecode kind
      52, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        30,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f4944),
        ACE_NTOHL (0x4c547970),
        ACE_NTOHL (0x653a312e),
        ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/IDLType:1.0
        8,
        ACE_NTOHL (0x49444c54),
        ACE_NTOHL (0x79706500),  // name = IDLType

      7,
      ACE_NTOHL (0x61636365),
      ACE_NTOHL (0x73730000),  // name = access
      CORBA::tk_alias, // typecode kind for typedefs
      64, // encapsulation length
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



    0U,

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueMemberSeq (
    CORBA::tk_alias,
    sizeof (_oc_CORBA_ValueMemberSeq),
    (char *) &_oc_CORBA_ValueMemberSeq,
    0,
    sizeof (CORBA::ValueMemberSeq)
  );

static const CORBA::Long _oc_CORBA_ValueMemberDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c75654d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x72446566),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/ValueMemberDef:1.0
  15,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x654d656d),
  ACE_NTOHL (0x62657244),
  ACE_NTOHL (0x65660000),  // name = ValueMemberDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueMemberDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ValueMemberDef),
    (char *) &_oc_CORBA_ValueMemberDef,
    0,
    sizeof (CORBA::ValueMemberDef)
  );

static const CORBA::Long _oc_CORBA_ValueDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c756544),
  ACE_NTOHL (0x65663a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ValueDef:1.0
  9,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65446566),
  ACE_NTOHL (0x0),  // name = ValueDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ValueDef),
    (char *) &_oc_CORBA_ValueDef,
    0,
    sizeof (CORBA::ValueDef)
  );

static const CORBA::Long _oc_CORBA_ValueDescription[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  39,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c756544),
  ACE_NTOHL (0x65736372),
  ACE_NTOHL (0x69707469),
  ACE_NTOHL (0x6f6e3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/ValueDescription:1.0
  17,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65446573),
  ACE_NTOHL (0x63726970),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = ValueDescription
  10, // member count
  5,
  ACE_NTOHL (0x6e616d65),
  ACE_NTOHL (0x0),  // name = name
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    33,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f4964),
    ACE_NTOHL (0x656e7469),
    ACE_NTOHL (0x66696572),
    ACE_NTOHL (0x3a312e30),
    ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/CORBA/Identifier:1.0
    11,
    ACE_NTOHL (0x4964656e),
    ACE_NTOHL (0x74696669),
    ACE_NTOHL (0x65720000),  // name = Identifier
    CORBA::tk_string,
    0U, // string length

  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  12,
  ACE_NTOHL (0x69735f61),
  ACE_NTOHL (0x62737472),
  ACE_NTOHL (0x61637400),  // name = is_abstract
  CORBA::tk_boolean,

  10,
  ACE_NTOHL (0x69735f63),
  ACE_NTOHL (0x7573746f),
  ACE_NTOHL (0x6d000000),  // name = is_custom
  CORBA::tk_boolean,

  11,
  ACE_NTOHL (0x64656669),
  ACE_NTOHL (0x6e65645f),
  ACE_NTOHL (0x696e0000),  // name = defined_in
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

  8,
  ACE_NTOHL (0x76657273),
  ACE_NTOHL (0x696f6e00),  // name = version
  CORBA::tk_alias, // typecode kind for typedefs
  68, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5665),
    ACE_NTOHL (0x7273696f),
    ACE_NTOHL (0x6e537065),
    ACE_NTOHL (0x633a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/VersionSpec:1.0
    12,
    ACE_NTOHL (0x56657273),
    ACE_NTOHL (0x696f6e53),
    ACE_NTOHL (0x70656300),  // name = VersionSpec
    CORBA::tk_string,
    0U, // string length

  21,
  ACE_NTOHL (0x73757070),
  ACE_NTOHL (0x6f727465),
  ACE_NTOHL (0x645f696e),
  ACE_NTOHL (0x74657266),
  ACE_NTOHL (0x61636573),
  ACE_NTOHL (0x0),  // name = supported_interfaces
  CORBA::tk_alias, // typecode kind for typedefs
  164, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49645365),
    ACE_NTOHL (0x713a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/RepositoryIdSeq:1.0
    16,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x53657100),  // name = RepositoryIdSeq
    CORBA::tk_sequence, // typecode kind
    88, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      0U,


  21,
  ACE_NTOHL (0x61627374),
  ACE_NTOHL (0x72616374),
  ACE_NTOHL (0x5f626173),
  ACE_NTOHL (0x655f7661),
  ACE_NTOHL (0x6c756573),
  ACE_NTOHL (0x0),  // name = abstract_base_values
  CORBA::tk_alias, // typecode kind for typedefs
  164, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    38,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49645365),
    ACE_NTOHL (0x713a312e),
    ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/RepositoryIdSeq:1.0
    16,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x53657100),  // name = RepositoryIdSeq
    CORBA::tk_sequence, // typecode kind
    88, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_alias, // typecode kind for typedefs
      72, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        35,
        ACE_NTOHL (0x49444c3a),
        ACE_NTOHL (0x6f6d672e),
        ACE_NTOHL (0x6f72672f),
        ACE_NTOHL (0x434f5242),
        ACE_NTOHL (0x412f5265),
        ACE_NTOHL (0x706f7369),
        ACE_NTOHL (0x746f7279),
        ACE_NTOHL (0x49643a31),
        ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
        13,
        ACE_NTOHL (0x5265706f),
        ACE_NTOHL (0x7369746f),
        ACE_NTOHL (0x72794964),
        ACE_NTOHL (0x0),  // name = RepositoryId
        CORBA::tk_string,
        0U, // string length

      0U,


  15,
  ACE_NTOHL (0x69735f74),
  ACE_NTOHL (0x72756e63),
  ACE_NTOHL (0x61746162),
  ACE_NTOHL (0x6c650000),  // name = is_truncatable
  CORBA::tk_boolean,

  11,
  ACE_NTOHL (0x62617365),
  ACE_NTOHL (0x5f76616c),
  ACE_NTOHL (0x75650000),  // name = base_value
  CORBA::tk_alias, // typecode kind for typedefs
  72, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    35,
    ACE_NTOHL (0x49444c3a),
    ACE_NTOHL (0x6f6d672e),
    ACE_NTOHL (0x6f72672f),
    ACE_NTOHL (0x434f5242),
    ACE_NTOHL (0x412f5265),
    ACE_NTOHL (0x706f7369),
    ACE_NTOHL (0x746f7279),
    ACE_NTOHL (0x49643a31),
    ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
    13,
    ACE_NTOHL (0x5265706f),
    ACE_NTOHL (0x7369746f),
    ACE_NTOHL (0x72794964),
    ACE_NTOHL (0x0),  // name = RepositoryId
    CORBA::tk_string,
    0U, // string length

};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueDescription (
    CORBA::tk_struct,
    sizeof (_oc_CORBA_ValueDescription),
    (char *) &_oc_CORBA_ValueDescription,
    0,
    sizeof (CORBA::ValueDescription)
  );

static const CORBA::Long _oc_CORBA_ValueBoxDef[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x434f5242),
  ACE_NTOHL (0x412f5661),
  ACE_NTOHL (0x6c756542),
  ACE_NTOHL (0x6f784465),
  ACE_NTOHL (0x663a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CORBA/ValueBoxDef:1.0
  12,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65426f78),
  ACE_NTOHL (0x44656600),  // name = ValueBoxDef
};

static CORBA::TypeCode _tc_TAO_tc_CORBA_ValueBoxDef (
    CORBA::tk_objref,
    sizeof (_oc_CORBA_ValueBoxDef),
    (char *) &_oc_CORBA_ValueBoxDef,
    0,
    sizeof (CORBA::ValueBoxDef)
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
  CORBA::_tc_DefinitionKind = &_tc_TAO_tc_CORBA_DefinitionKind;
  CORBA::_tc_VersionSpec = &_tc_TAO_tc_CORBA_VersionSpec;
  CORBA::_tc_Contained = &_tc_TAO_tc_CORBA_Contained;
  CORBA::_tc_ContainedSeq = &_tc_TAO_tc_CORBA_ContainedSeq;
  CORBA::_tc_InterfaceDefSeq = &_tc_TAO_tc_CORBA_InterfaceDefSeq;
  CORBA::_tc_AbstractInterfaceDefSeq =
    &_tc_TAO_tc_CORBA_AbstractInterfaceDefSeq;
  CORBA::_tc_LocalInterfaceDefSeq = &_tc_TAO_tc_CORBA_LocalInterfaceDefSeq;
  CORBA::_tc_StructMember = &_tc_TAO_tc_CORBA_StructMember;
  CORBA::_tc_StructMemberSeq = &_tc_TAO_tc_CORBA_StructMemberSeq;
  CORBA::_tc_Initializer = &_tc_TAO_tc_CORBA_Initializer;
  CORBA::_tc_InitializerSeq = &_tc_TAO_tc_CORBA_InitializerSeq;
  CORBA::_tc_UnionMember = &_tc_TAO_tc_CORBA_UnionMember;
  CORBA::_tc_UnionMemberSeq = &_tc_TAO_tc_CORBA_UnionMemberSeq;
  CORBA::_tc_EnumMemberSeq = &_tc_TAO_tc_CORBA_EnumMemberSeq;
  CORBA::_tc_Container = &_tc_TAO_tc_CORBA_Container;
  CORBA::_tc_IDLType = &_tc_TAO_tc_CORBA_IDLType;
  CORBA::_tc_TypedefDef = &_tc_TAO_tc_CORBA_TypedefDef;
  CORBA::_tc_TypeDescription = &_tc_TAO_tc_CORBA_TypeDescription;
  CORBA::_tc_PrimitiveKind = &_tc_TAO_tc_CORBA_PrimitiveKind;
  CORBA::_tc_Repository = &_tc_TAO_tc_CORBA_Repository;
  CORBA::_tc_ModuleDef = &_tc_TAO_tc_CORBA_ModuleDef;
  CORBA::_tc_ModuleDescription = &_tc_TAO_tc_CORBA_ModuleDescription;
  CORBA::_tc_ConstantDef = &_tc_TAO_tc_CORBA_ConstantDef;
  CORBA::_tc_ConstantDescription = &_tc_TAO_tc_CORBA_ConstantDescription;
  CORBA::_tc_StructDef = &_tc_TAO_tc_CORBA_StructDef;
  CORBA::_tc_UnionDef = &_tc_TAO_tc_CORBA_UnionDef;
  CORBA::_tc_EnumDef = &_tc_TAO_tc_CORBA_EnumDef;
  CORBA::_tc_AliasDef = &_tc_TAO_tc_CORBA_AliasDef;
  CORBA::_tc_NativeDef = &_tc_TAO_tc_CORBA_NativeDef;
  CORBA::_tc_PrimitiveDef = &_tc_TAO_tc_CORBA_PrimitiveDef;
  CORBA::_tc_StringDef = &_tc_TAO_tc_CORBA_StringDef;
  CORBA::_tc_WstringDef = &_tc_TAO_tc_CORBA_WstringDef;
  CORBA::_tc_SequenceDef = &_tc_TAO_tc_CORBA_SequenceDef;
  CORBA::_tc_ArrayDef = &_tc_TAO_tc_CORBA_ArrayDef;
  CORBA::_tc_ExceptionDef = &_tc_TAO_tc_CORBA_ExceptionDef;
  CORBA::_tc_ExceptionDescription = &_tc_TAO_tc_CORBA_ExceptionDescription;
  CORBA::_tc_ExceptionDefSeq = &_tc_TAO_tc_CORBA_ExceptionDefSeq;
  CORBA::_tc_ExcDescriptionSeq = &_tc_TAO_tc_CORBA_ExcDescriptionSeq;
  CORBA::_tc_AttributeMode = &_tc_TAO_tc_CORBA_AttributeMode;
  CORBA::_tc_AttributeDef = &_tc_TAO_tc_CORBA_AttributeDef;
  CORBA::_tc_AttributeDescription = &_tc_TAO_tc_CORBA_AttributeDescription;
  CORBA::_tc_OperationMode = &_tc_TAO_tc_CORBA_OperationMode;
  CORBA::_tc_ParameterMode = &_tc_TAO_tc_CORBA_ParameterMode;
  CORBA::_tc_ParameterDescription = &_tc_TAO_tc_CORBA_ParameterDescription;
  CORBA::_tc_ParDescriptionSeq = &_tc_TAO_tc_CORBA_ParDescriptionSeq;
  CORBA::_tc_ContextIdentifier = &_tc_TAO_tc_CORBA_ContextIdentifier;
  CORBA::_tc_ContextIdSeq = &_tc_TAO_tc_CORBA_ContextIdSeq;
  CORBA::_tc_OperationDef = &_tc_TAO_tc_CORBA_OperationDef;
  CORBA::_tc_OperationDescription = &_tc_TAO_tc_CORBA_OperationDescription;
  CORBA::_tc_RepositoryIdSeq = &_tc_TAO_tc_CORBA_RepositoryIdSeq;
  CORBA::_tc_OpDescriptionSeq = &_tc_TAO_tc_CORBA_OpDescriptionSeq;
  CORBA::_tc_AttrDescriptionSeq = &_tc_TAO_tc_CORBA_AttrDescriptionSeq;
  CORBA::_tc_InterfaceDef = &_tc_TAO_tc_CORBA_InterfaceDef;
  CORBA::_tc_InterfaceDescription = &_tc_TAO_tc_CORBA_InterfaceDescription;
  CORBA::_tc_AbstractInterfaceDef = &_tc_TAO_tc_CORBA_AbstractInterfaceDef;
  CORBA::_tc_LocalInterfaceDef = &_tc_TAO_tc_CORBA_LocalInterfaceDef;

  CORBA::_tc_FixedDef = &_tc_TAO_tc_CORBA_FixedDef;
  CORBA::_tc_ValueDefSeq = &_tc_TAO_tc_CORBA_ValueDefSeq;
  CORBA::_tc_ValueMember = &_tc_TAO_tc_CORBA_ValueMember;
  CORBA::_tc_ValueMemberSeq = &_tc_TAO_tc_CORBA_ValueMemberSeq;
  CORBA::_tc_ValueMemberDef = &_tc_TAO_tc_CORBA_ValueMemberDef;
  CORBA::_tc_ValueDef = &_tc_TAO_tc_CORBA_ValueDef;
  CORBA::_tc_ValueDescription = &_tc_TAO_tc_CORBA_ValueDescription;
  CORBA::_tc_ValueBoxDef = &_tc_TAO_tc_CORBA_ValueBoxDef;

  TAO_ORB_Core::ifr_client_adapter_name ("Concrete_IFR_Client_Adapter");

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_IFR_Client_Adapter_Impl);
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
