/* -*- C++ -*- $Id$ */
#include "FT_CORBA_ServiceS.h"
#include "tao/Stub.h"

// This has been got from the generated code. So, the file may look
// messy. Need to clean it slowly -- Bala

#if (TAO_HAS_FT_CORBA == 1)

#if !defined (__ACE_INLINE__)
#include "FT_CORBA_ServiceS.i"
#endif /* !defined INLINE */

class TAO_FT_PropertyManager_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};


/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_PropertyManager_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_PropertyManager_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33,  0,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33,  0,
     33, 33, 33, 33, 33, 10, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33,  5, 33, 33, 33, 33, 33, 33,
     33, 33,  0,  0, 33, 33, 33, 33,  0, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33,
#else
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33, 33,  0, 33,  0, 33, 33,
     33, 33, 33, 10, 33, 33, 33, 33, 33, 33,
     33, 33, 33, 33,  5,  0,  0, 33, 33, 33,
     33,  0, 33, 33, 33, 33, 33, 33,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_PropertyManager_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 10,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 26,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 32,
      HASH_VALUE_RANGE = 28,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_PropertyManager::_is_a_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},
      {"_non_existent",  &FT_POA_PropertyManager::_non_existent_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"set_type_properties",   &FT_POA_PropertyManager::set_type_properties_skel},
      {"",0},{"",0},
      {"set_default_properties",        &FT_POA_PropertyManager::set_default_properties_skel},
      {"",0},
      {"get_properties",        &FT_POA_PropertyManager::get_properties_skel},
      {"",0},
      {"set_properties_dynamically",    &FT_POA_PropertyManager::set_properties_dynamically_skel},
      {"remove_type_properties",        &FT_POA_PropertyManager::remove_type_properties_skel},
      {"",0},
      {"get_type_properties",   &FT_POA_PropertyManager::get_type_properties_skel},
      {"remove_default_properties",     &FT_POA_PropertyManager::remove_default_properties_skel},
      {"",0},
      {"get_default_properties",        &FT_POA_PropertyManager::get_default_properties_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */

static TAO_FT_PropertyManager_Perfect_Hash_OpTable tao_FT_PropertyManager_optable;

FT::PropertyManager_ptr _TAO_collocation_POA_FT_PropertyManager_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::PropertyManager_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_PropertyManager (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_PropertyManager_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_PropertyManager_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_PropertyManager_Stub_Factory;

  return 0;
}


static int _TAO_collocation_POA_FT_PropertyManager_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_PropertyManager_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_PropertyManager_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_PropertyManager::FT_POA_PropertyManager (void)
{
  this->optable_ = &tao_FT_PropertyManager_optable;
}

// copy ctor
FT_POA_PropertyManager::FT_POA_PropertyManager (const FT_POA_PropertyManager& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_PropertyManager::~FT_POA_PropertyManager (void)
{
}

void FT_POA_PropertyManager::set_default_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::Properties props;
  if (!(
    (_tao_in >> props)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->set_default_properties (
        props,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_PropertyManager::get_default_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::Properties_var _tao_retval;
#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_default_properties (
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_PropertyManager::remove_default_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::Properties props;
  if (!(
    (_tao_in >> props)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->remove_default_properties (
        props,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_default_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_PropertyManager::set_type_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  CORBA::String_var type_id;
  FT::Properties overrides;
  if (!(
    (_tao_in >> type_id.out ()) &&
    (_tao_in >> overrides)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->set_type_properties (
        type_id.in (),
        overrides,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_PropertyManager::get_type_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::Properties_var _tao_retval;
  CORBA::String_var type_id;
  if (!(
    (_tao_in >> type_id.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_type_properties (
        type_id.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_PropertyManager::remove_type_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  CORBA::String_var type_id;
  FT::Properties props;
  if (!(
    (_tao_in >> type_id.out ()) &&
    (_tao_in >> props)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->remove_type_properties (
        type_id.in (),
        props,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_type_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_PropertyManager::set_properties_dynamically_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::ObjectGroup_var object_group;
  FT::Properties overrides;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> overrides)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_properties_dynamically",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->set_properties_dynamically (
        object_group.in (),
        overrides,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_properties_dynamically",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_properties_dynamically",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_PropertyManager::get_properties_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *)_tao_object_reference;

  FT::Properties_var _tao_retval;
  FT::ObjectGroup_var object_group;
  if (!(
    (_tao_in >> object_group.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_properties (
        object_group.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_properties",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_PropertyManager::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_PropertyManager::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PropertyManager *_tao_impl = (FT_POA_PropertyManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_PropertyManager::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PropertyManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_PropertyManager::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/PropertyManager:1.0") == 0)
    return ACE_static_cast (FT_POA_PropertyManager_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_PropertyManager::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_PropertyManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/PropertyManager:1.0";
}

FT::PropertyManager*
FT_POA_PropertyManager::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          FT::PropertyManager_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_PropertyManager (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return FT::PropertyManager::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_PropertyManager::FT_POA_tao_thru_poa_collocated_PropertyManager (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_PropertyManager::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_PropertyManager::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


void FT_POA_tao_thru_poa_collocated_PropertyManager::set_default_properties  (
    const FT::Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidProperty,
    FT::UnsupportedProperty
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_default_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->set_default_properties (
      props,
      ACE_TRY_ENV
    );
  return;
}

FT::Properties * FT_POA_tao_thru_poa_collocated_PropertyManager::get_default_properties  (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  FT::Properties_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_default_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->get_default_properties (
      ACE_TRY_ENV
    );

}

void FT_POA_tao_thru_poa_collocated_PropertyManager::remove_default_properties  (
    const FT::Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidProperty,
    FT::UnsupportedProperty
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "remove_default_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->remove_default_properties (
      props,
      ACE_TRY_ENV
    );
  return;
}

void FT_POA_tao_thru_poa_collocated_PropertyManager::set_type_properties  (
    const char * type_id,
    const FT::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidProperty,
    FT::UnsupportedProperty
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_type_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->set_type_properties (
      type_id,
      overrides,
      ACE_TRY_ENV
    );
  return;
}

FT::Properties * FT_POA_tao_thru_poa_collocated_PropertyManager::get_type_properties  (
    const char * type_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  FT::Properties_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_type_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->get_type_properties (
      type_id,
      ACE_TRY_ENV
    );

}

void
FT_POA_tao_thru_poa_collocated_PropertyManager::remove_type_properties  (
    const char * type_id,
    const FT::Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidProperty,
    FT::UnsupportedProperty
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "remove_type_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->remove_type_properties (
      type_id,
      props,
      ACE_TRY_ENV
    );
  return;
}

void FT_POA_tao_thru_poa_collocated_PropertyManager::set_properties_dynamically  (
    FT::ObjectGroup_ptr object_group,
    const FT::Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::InvalidProperty,
    FT::UnsupportedProperty
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_properties_dynamically",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->set_properties_dynamically (
      object_group,
      overrides,
      ACE_TRY_ENV
    );
  return;
}

FT::Properties * FT_POA_tao_thru_poa_collocated_PropertyManager::get_properties  (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound
  ))
{
  FT::Properties_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_properties",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PropertyManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PropertyManager:1.0"
        )
    )->get_properties (
      object_group,
      ACE_TRY_ENV
    );

}



class TAO_FT_ObjectGroupManager_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};

/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_ObjectGroupManager_Perfect_Hash_OpTable -N lookup  */


unsigned int
TAO_FT_ObjectGroupManager_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29,  0,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29,  0,
     29, 10,  0, 29,  0,  0, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29,  5, 29, 29,
     29, 29, 29,  0, 29, 29, 29, 29, 29, 29,
     29, 29,  0, 15, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29,
#else
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
     29, 29, 29, 29, 29,  0, 29,  0, 29, 10,
      0, 29,  0,  0, 29, 29, 29, 29,  5, 29,
     29, 29, 29, 29,  0,  0, 15, 29, 29, 29,
     29, 29, 29, 29, 29, 29, 29, 29,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_ObjectGroupManager_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 10,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 20,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 28,
      HASH_VALUE_RANGE = 24,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_ObjectGroupManager::_is_a_skel},
      {"",0},{"",0},{"",0},{"",0},
      {"add_member",    &FT_POA_ObjectGroupManager::add_member_skel},
      {"",0},{"",0},
      {"remove_member",         &FT_POA_ObjectGroupManager::remove_member_skel},
      {"get_member_ref",        &FT_POA_ObjectGroupManager::get_member_ref_skel},
      {"",0},{"",0},{"",0},
      {"set_primary_member",    &FT_POA_ObjectGroupManager::set_primary_member_skel},
      {"get_object_group_id",   &FT_POA_ObjectGroupManager::get_object_group_id_skel},
      {"get_object_group_ref",  &FT_POA_ObjectGroupManager::get_object_group_ref_skel},
      {"",0},{"",0},
      {"create_member",         &FT_POA_ObjectGroupManager::create_member_skel},
      {"",0},
      {"locations_of_members",  &FT_POA_ObjectGroupManager::locations_of_members_skel},
      {"",0},{"",0},
      {"_non_existent",  &FT_POA_ObjectGroupManager::_non_existent_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */

static TAO_FT_ObjectGroupManager_Perfect_Hash_OpTable tao_FT_ObjectGroupManager_optable;

FT::ObjectGroupManager_ptr _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::ObjectGroupManager_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_ObjectGroupManager (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_ObjectGroupManager_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_ObjectGroupManager_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_ObjectGroupManager::FT_POA_ObjectGroupManager (void)
{
  this->optable_ = &tao_FT_ObjectGroupManager_optable;
}

// copy ctor
FT_POA_ObjectGroupManager::FT_POA_ObjectGroupManager (const FT_POA_ObjectGroupManager& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_ObjectGroupManager::~FT_POA_ObjectGroupManager (void)
{
}

void FT_POA_ObjectGroupManager::create_member_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroup_var _tao_retval;
  FT::ObjectGroup_var object_group;
  FT::Location the_location;
  CORBA::String_var type_id;
  FT::Criteria the_criteria;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> the_location) &&
    (_tao_in >> type_id.out ()) &&
    (_tao_in >> the_criteria)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->create_member (
        object_group.in (),
        the_location,
        type_id.in (),
        the_criteria,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::add_member_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroup_var _tao_retval;
  FT::ObjectGroup_var object_group;
  FT::Location the_location;
  CORBA::Object_var member;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> the_location) &&
    (_tao_in >> member.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "add_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->add_member (
        object_group.in (),
        the_location,
        member.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "add_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "add_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::remove_member_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroup_var _tao_retval;
  FT::ObjectGroup_var object_group;
  FT::Location the_location;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> the_location)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->remove_member (
        object_group.in (),
        the_location,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "remove_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::set_primary_member_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroup_var _tao_retval;
  FT::ObjectGroup_var object_group;
  FT::Location the_location;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> the_location)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_primary_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->set_primary_member (
        object_group.in (),
        the_location,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_primary_member",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_primary_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::locations_of_members_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::Locations_var _tao_retval;
  FT::ObjectGroup_var object_group;
  if (!(
    (_tao_in >> object_group.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "locations_of_members",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->locations_of_members (
        object_group.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "locations_of_members",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "locations_of_members",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::get_object_group_id_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroupId _tao_retval = 0;
  FT::ObjectGroup_var object_group;
  if (!(
    (_tao_in >> object_group.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_id",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_object_group_id (
        object_group.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_id",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_id",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval)
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::get_object_group_ref_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  FT::ObjectGroup_var _tao_retval;
  FT::ObjectGroup_var object_group;
  if (!(
    (_tao_in >> object_group.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_ref",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_object_group_ref (
        object_group.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_ref",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_object_group_ref",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::get_member_ref_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *)_tao_object_reference;

  CORBA::Object_var _tao_retval;
  FT::ObjectGroup_var object_group;
  FT::Location loc;
  if (!(
    (_tao_in >> object_group.out ()) &&
    (_tao_in >> loc)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_member_ref",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_member_ref (
        object_group.in (),
        loc,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_member_ref",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_member_ref",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ObjectGroupManager::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_ObjectGroupManager::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ObjectGroupManager *_tao_impl = (FT_POA_ObjectGroupManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_ObjectGroupManager::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ObjectGroupManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_ObjectGroupManager::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/ObjectGroupManager:1.0") == 0)
    return ACE_static_cast (FT_POA_ObjectGroupManager_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_ObjectGroupManager::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_ObjectGroupManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/ObjectGroupManager:1.0";
}

FT::ObjectGroupManager*
FT_POA_ObjectGroupManager::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::ObjectGroupManager_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_ObjectGroupManager (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::ObjectGroupManager::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_ObjectGroupManager::FT_POA_tao_thru_poa_collocated_ObjectGroupManager (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_ObjectGroupManager::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_ObjectGroupManager::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


FT::ObjectGroup_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::create_member  (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    const char * type_id,
    const FT::Criteria & the_criteria,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::MemberAlreadyPresent,
    FT::NoFactory,
    FT::ObjectNotCreated,
    FT::InvalidCriteria,
    FT::CannotMeetCriteria
  ))
{
  FT::ObjectGroup_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "create_member",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->create_member (
      object_group,
      the_location,
      type_id,
      the_criteria,
      ACE_TRY_ENV
    );

}

FT::ObjectGroup_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::add_member  (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Object_ptr member,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::MemberAlreadyPresent,
    FT::ObjectNotAdded
  ))
{
  FT::ObjectGroup_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "add_member",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->add_member (
      object_group,
      the_location,
      member,
      ACE_TRY_ENV
    );

}

FT::ObjectGroup_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::remove_member  (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::MemberNotFound
  ))
{
  FT::ObjectGroup_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "remove_member",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->remove_member (
      object_group,
      the_location,
      ACE_TRY_ENV
    );

}

FT::ObjectGroup_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::set_primary_member  (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::MemberNotFound,
    FT::PrimaryNotSet,
    FT::BadReplicationStyle
  ))
{
  FT::ObjectGroup_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_primary_member",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->set_primary_member (
      object_group,
      the_location,
      ACE_TRY_ENV
    );

}

FT::Locations * FT_POA_tao_thru_poa_collocated_ObjectGroupManager::locations_of_members  (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound
  ))
{
  FT::Locations_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "locations_of_members",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->locations_of_members (
      object_group,
      ACE_TRY_ENV
    );

}

FT::ObjectGroupId FT_POA_tao_thru_poa_collocated_ObjectGroupManager::get_object_group_id  (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound
  ))
{
  FT::ObjectGroupId _tao_retval = 0;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_object_group_id",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN  (_tao_retval);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->get_object_group_id (
      object_group,
      ACE_TRY_ENV
    );

}

FT::ObjectGroup_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::get_object_group_ref  (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound
  ))
{
  FT::ObjectGroup_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_object_group_ref",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->get_object_group_ref (
      object_group,
      ACE_TRY_ENV
    );

}

CORBA::Object_ptr FT_POA_tao_thru_poa_collocated_ObjectGroupManager::get_member_ref  (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & loc,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectGroupNotFound,
    FT::MemberNotFound
  ))
{
  CORBA::Object_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_member_ref",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ObjectGroupManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ObjectGroupManager:1.0"
        )
    )->get_member_ref (
      object_group,
      loc,
      ACE_TRY_ENV
    );

}



class TAO_FT_GenericFactory_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_GenericFactory_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_GenericFactory_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16,  0,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16,  0,
     16,  1,  2, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16,  0, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16,
#else
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16,  0, 16,  0, 16,  1,
      2, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16,  0, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_GenericFactory_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 4,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 13,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 15,
      HASH_VALUE_RANGE = 11,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_GenericFactory::_is_a_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},
      {"_non_existent",  &FT_POA_GenericFactory::_non_existent_skel},
      {"create_object",         &FT_POA_GenericFactory::create_object_skel},
      {"delete_object",         &FT_POA_GenericFactory::delete_object_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */
static TAO_FT_GenericFactory_Perfect_Hash_OpTable tao_FT_GenericFactory_optable;

FT::GenericFactory_ptr _TAO_collocation_POA_FT_GenericFactory_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::GenericFactory_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_GenericFactory (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_GenericFactory_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_GenericFactory_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_GenericFactory_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_GenericFactory_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_GenericFactory_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_GenericFactory_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_GenericFactory::FT_POA_GenericFactory (void)
{
  this->optable_ = &tao_FT_GenericFactory_optable;
}

// copy ctor
FT_POA_GenericFactory::FT_POA_GenericFactory (const FT_POA_GenericFactory& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_GenericFactory::~FT_POA_GenericFactory (void)
{
}

void FT_POA_GenericFactory::create_object_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_GenericFactory *_tao_impl = (FT_POA_GenericFactory *)_tao_object_reference;

  CORBA::Object_var _tao_retval;
  CORBA::String_var type_id;
  FT::Criteria the_criteria;
  FT::GenericFactory::FactoryCreationId_var factory_creation_id;
  if (!(
    (_tao_in >> type_id.out ()) &&
    (_tao_in >> the_criteria)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_object",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->create_object (
        type_id.in (),
        the_criteria,
        factory_creation_id.out (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_object",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_object",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ()) &&
    (_tao_out << factory_creation_id.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_GenericFactory::delete_object_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_GenericFactory *_tao_impl = (FT_POA_GenericFactory *)_tao_object_reference;

  FT::GenericFactory::FactoryCreationId factory_creation_id;
  if (!(
    (_tao_in >> factory_creation_id)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "delete_object",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->delete_object (
        factory_creation_id,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "delete_object",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "delete_object",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_GenericFactory::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_GenericFactory *_tao_impl = (FT_POA_GenericFactory *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_GenericFactory::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_GenericFactory *_tao_impl = (FT_POA_GenericFactory *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_GenericFactory::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/GenericFactory:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_GenericFactory::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/GenericFactory:1.0") == 0)
    return ACE_static_cast (FT_POA_GenericFactory_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_GenericFactory::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_GenericFactory::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/GenericFactory:1.0";
}

FT::GenericFactory*
FT_POA_GenericFactory::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::GenericFactory_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_GenericFactory (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::GenericFactory::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_GenericFactory::FT_POA_tao_thru_poa_collocated_GenericFactory (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_GenericFactory::_is_a (
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_GenericFactory_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/GenericFactory:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_GenericFactory::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_GenericFactory_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/GenericFactory:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


CORBA::Object_ptr FT_POA_tao_thru_poa_collocated_GenericFactory::create_object  (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::NoFactory,
    FT::ObjectNotCreated,
    FT::InvalidCriteria,
    FT::InvalidProperty,
    FT::CannotMeetCriteria
  ))
{
  CORBA::Object_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "create_object",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_GenericFactory_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/GenericFactory:1.0"
        )
    )->create_object (
      type_id,
      the_criteria,
      factory_creation_id,
      ACE_TRY_ENV
    );

}

void FT_POA_tao_thru_poa_collocated_GenericFactory::delete_object  (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::ObjectNotFound
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "delete_object",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_GenericFactory_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/GenericFactory:1.0"
        )
    )->delete_object (
      factory_creation_id,
      ACE_TRY_ENV
    );
  return;
}



class TAO_FT_ReplicationManager_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_ReplicationManager_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_ReplicationManager_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 25,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56,  0,
     56,  0,  5, 56, 15, 20, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56,  5, 56, 56,
     56, 56, 56, 10, 56, 56, 56, 56, 56, 56,
     56, 56,  0,  0, 56, 56, 56, 56,  0, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56,
#else
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
     56, 56, 56, 56, 56, 25, 56,  0, 56,  0,
      5, 56, 15, 20, 56, 56, 56, 56,  5, 56,
     56, 56, 56, 56, 10,  0,  0, 56, 56, 56,
     56,  0, 56, 56, 56, 56, 56, 56,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_ReplicationManager_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 22,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 26,
      MIN_HASH_VALUE = 13,
      MAX_HASH_VALUE = 55,
      HASH_VALUE_RANGE = 43,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},
      {"",0},{"",0},{"",0},{"",0},
      {"create_object",         &FT_POA_ReplicationManager::create_object_skel},
      {"",0},{"",0},{"",0},{"",0},
      {"delete_object",         &FT_POA_ReplicationManager::delete_object_skel},
      {"set_type_properties",   &FT_POA_ReplicationManager::set_type_properties_skel},
      {"add_member",    &FT_POA_ReplicationManager::add_member_skel},
      {"",0},
      {"set_default_properties",        &FT_POA_ReplicationManager::set_default_properties_skel},
      {"create_member",         &FT_POA_ReplicationManager::create_member_skel},
      {"",0},
      {"locations_of_members",  &FT_POA_ReplicationManager::locations_of_members_skel},
      {"set_properties_dynamically",    &FT_POA_ReplicationManager::set_properties_dynamically_skel},
      {"",0},
      {"set_primary_member",    &FT_POA_ReplicationManager::set_primary_member_skel},
      {"",0},
      {"_is_a",  &FT_POA_ReplicationManager::_is_a_skel},
      {"",0},
      {"remove_type_properties",        &FT_POA_ReplicationManager::remove_type_properties_skel},
      {"remove_member",         &FT_POA_ReplicationManager::remove_member_skel},
      {"get_properties",        &FT_POA_ReplicationManager::get_properties_skel},
      {"remove_default_properties",     &FT_POA_ReplicationManager::remove_default_properties_skel},
      {"",0},{"",0},
      {"_non_existent",  &FT_POA_ReplicationManager::_non_existent_skel},
      {"get_type_properties",   &FT_POA_ReplicationManager::get_type_properties_skel},
      {"",0},{"",0},
      {"get_default_properties",        &FT_POA_ReplicationManager::get_default_properties_skel},
      {"register_fault_notifier",       &FT_POA_ReplicationManager::register_fault_notifier_skel},
      {"get_object_group_id",   &FT_POA_ReplicationManager::get_object_group_id_skel},
      {"",0},{"",0},{"",0},
      {"get_fault_notifier",    &FT_POA_ReplicationManager::get_fault_notifier_skel},
      {"get_member_ref",        &FT_POA_ReplicationManager::get_member_ref_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"get_object_group_ref",  &FT_POA_ReplicationManager::get_object_group_ref_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */
static TAO_FT_ReplicationManager_Perfect_Hash_OpTable tao_FT_ReplicationManager_optable;

FT::ReplicationManager_ptr _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::ReplicationManager_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_ReplicationManager (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_ReplicationManager_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_ReplicationManager_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_ReplicationManager::FT_POA_ReplicationManager (void)
{
  this->optable_ = &tao_FT_ReplicationManager_optable;
}

// copy ctor
FT_POA_ReplicationManager::FT_POA_ReplicationManager (const FT_POA_ReplicationManager& rhs)
  : FT_POA_PropertyManager (rhs),
    FT_POA_ObjectGroupManager (rhs),
    FT_POA_GenericFactory (rhs),
    TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_ReplicationManager::~FT_POA_ReplicationManager (void)
{
}

void FT_POA_ReplicationManager::register_fault_notifier_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ReplicationManager *_tao_impl = (FT_POA_ReplicationManager *)_tao_object_reference;

  FT::FaultNotifier_var fault_notifier;
  if (!(
    (_tao_in >> fault_notifier.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "register_fault_notifier",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->register_fault_notifier (
        fault_notifier.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "register_fault_notifier",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "register_fault_notifier",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_ReplicationManager::get_fault_notifier_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  FT_POA_ReplicationManager *_tao_impl = (FT_POA_ReplicationManager *)_tao_object_reference;

  FT::FaultNotifier_var _tao_retval;
#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_fault_notifier",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_fault_notifier (
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_fault_notifier",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_fault_notifier",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_ReplicationManager::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_ReplicationManager *_tao_impl = (FT_POA_ReplicationManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_ReplicationManager::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_ReplicationManager *_tao_impl = (FT_POA_ReplicationManager *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_ReplicationManager::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ReplicationManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PropertyManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ObjectGroupManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/GenericFactory:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_ReplicationManager::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/ReplicationManager:1.0") == 0)
    return ACE_static_cast (FT_POA_ReplicationManager_ptr, this);
  if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/PropertyManager:1.0") == 0)
    return ACE_static_cast (FT_POA_PropertyManager_ptr, this);
  if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/ObjectGroupManager:1.0") == 0)
    return ACE_static_cast (FT_POA_ObjectGroupManager_ptr, this);
  if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/GenericFactory:1.0") == 0)
    return ACE_static_cast (FT_POA_GenericFactory_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_ReplicationManager::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_ReplicationManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/ReplicationManager:1.0";
}

FT::ReplicationManager*
FT_POA_ReplicationManager::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::ReplicationManager_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_ReplicationManager (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return FT::ReplicationManager::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_ReplicationManager::FT_POA_tao_thru_poa_collocated_ReplicationManager (
  TAO_Stub *stub
)
  : FT_POA_tao_thru_poa_collocated_PropertyManager (stub),
    FT_POA_tao_thru_poa_collocated_ObjectGroupManager (stub),
    FT_POA_tao_thru_poa_collocated_GenericFactory (stub),
    CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_ReplicationManager::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ReplicationManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ReplicationManager:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_ReplicationManager::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ReplicationManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ReplicationManager:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


void FT_POA_tao_thru_poa_collocated_ReplicationManager::register_fault_notifier  (
    FT::FaultNotifier_ptr fault_notifier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "register_fault_notifier",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_ReplicationManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ReplicationManager:1.0"
        )
    )->register_fault_notifier (
      fault_notifier,
      ACE_TRY_ENV
    );
  return;
}

FT::FaultNotifier_ptr FT_POA_tao_thru_poa_collocated_ReplicationManager::get_fault_notifier  (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InterfaceNotFound
  ))
{
  FT::FaultNotifier_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_fault_notifier",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_ReplicationManager_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/ReplicationManager:1.0"
        )
    )->get_fault_notifier (
      ACE_TRY_ENV
    );

}



class TAO_FT_PullMonitorable_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_PullMonitorable_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_PullMonitorable_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14,  0,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14,  0,
     14, 14, 14,  0, 14, 14, 14,  0, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14,  0, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14,
#else
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14,  0, 14,  0, 14, 14,
     14,  0, 14, 14, 14,  0, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14,  0, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_PullMonitorable_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 3,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 13,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 13,
      HASH_VALUE_RANGE = 9,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_PullMonitorable::_is_a_skel},
      {"",0},{"",0},
      {"is_alive",      &FT_POA_PullMonitorable::is_alive_skel},
      {"",0},{"",0},{"",0},{"",0},
      {"_non_existent",  &FT_POA_PullMonitorable::_non_existent_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */
static TAO_FT_PullMonitorable_Perfect_Hash_OpTable tao_FT_PullMonitorable_optable;

FT::PullMonitorable_ptr _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::PullMonitorable_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_PullMonitorable (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_PullMonitorable_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_PullMonitorable_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_PullMonitorable::FT_POA_PullMonitorable (void)
{
  this->optable_ = &tao_FT_PullMonitorable_optable;
}

// copy ctor
FT_POA_PullMonitorable::FT_POA_PullMonitorable (const FT_POA_PullMonitorable& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_PullMonitorable::~FT_POA_PullMonitorable (void)
{
}

void FT_POA_PullMonitorable::is_alive_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  FT_POA_PullMonitorable *_tao_impl = (FT_POA_PullMonitorable *)_tao_object_reference;

  CORBA::Boolean _tao_retval = 0;
#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "is_alive",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->is_alive (
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "is_alive",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "is_alive",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << CORBA::Any::from_boolean (_tao_retval))
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_PullMonitorable::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_PullMonitorable *_tao_impl = (FT_POA_PullMonitorable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_PullMonitorable::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_PullMonitorable *_tao_impl = (FT_POA_PullMonitorable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_PullMonitorable::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PullMonitorable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_PullMonitorable::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/PullMonitorable:1.0") == 0)
    return ACE_static_cast (FT_POA_PullMonitorable_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_PullMonitorable::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_PullMonitorable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/PullMonitorable:1.0";
}

FT::PullMonitorable*
FT_POA_PullMonitorable::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::PullMonitorable_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_PullMonitorable (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::PullMonitorable::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_PullMonitorable::FT_POA_tao_thru_poa_collocated_PullMonitorable (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_PullMonitorable::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PullMonitorable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PullMonitorable:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_PullMonitorable::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_PullMonitorable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PullMonitorable:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_PullMonitorable::is_alive  (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  CORBA::Boolean _tao_retval = 0;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "is_alive",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN  (_tao_retval);
  return ACE_reinterpret_cast (
      FT_POA_PullMonitorable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/PullMonitorable:1.0"
        )
    )->is_alive (
      ACE_TRY_ENV
    );

}



class TAO_FT_FaultNotifier_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:35 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_FaultNotifier_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_FaultNotifier_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34,  0,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34,  0,
     34,  0,  5, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34,  0, 34,  0, 34, 34, 34, 34, 34, 34,
     34, 34, 34,  0, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34,
#else
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34, 34, 34, 34,  0, 34,  0, 34,  0,
      5, 34, 34, 34, 34, 34, 34, 34, 34, 34,
     34, 34,  0, 34,  0, 34,  0, 34, 34, 34,
     34, 34, 34, 34, 34, 34, 34, 34,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_FaultNotifier_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 8,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 33,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 33,
      HASH_VALUE_RANGE = 29,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_FaultNotifier::_is_a_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},{"",0},{"",0},
      {"_non_existent",  &FT_POA_FaultNotifier::_non_existent_skel},
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"push_sequence_fault",   &FT_POA_FaultNotifier::push_sequence_fault_skel},
      {"",0},
      {"push_structured_fault",         &FT_POA_FaultNotifier::push_structured_fault_skel},
      {"",0},{"",0},
      {"disconnect_consumer",   &FT_POA_FaultNotifier::disconnect_consumer_skel},
      {"",0},
      {"create_subscription_filter",    &FT_POA_FaultNotifier::create_subscription_filter_skel},
      {"",0},{"",0},{"",0},{"",0},
      {"connect_sequence_fault_consumer",       &FT_POA_FaultNotifier::connect_sequence_fault_consumer_skel},
      {"",0},
      {"connect_structured_fault_consumer",     &FT_POA_FaultNotifier::connect_structured_fault_consumer_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:35 */
static TAO_FT_FaultNotifier_Perfect_Hash_OpTable tao_FT_FaultNotifier_optable;

FT::FaultNotifier_ptr _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::FaultNotifier_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_FaultNotifier (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_FaultNotifier_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_FaultNotifier_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_FaultNotifier::FT_POA_FaultNotifier (void)
{
  this->optable_ = &tao_FT_FaultNotifier_optable;
}

// copy ctor
FT_POA_FaultNotifier::FT_POA_FaultNotifier (const FT_POA_FaultNotifier& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_FaultNotifier::~FT_POA_FaultNotifier (void)
{
}

void FT_POA_FaultNotifier::push_structured_fault_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  CosNotification::StructuredEvent event;
  if (!(
    (_tao_in >> event)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_structured_fault",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->push_structured_fault (
        event,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_structured_fault",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_structured_fault",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_FaultNotifier::push_sequence_fault_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  CosNotification::EventBatch events;
  if (!(
    (_tao_in >> events)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_sequence_fault",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->push_sequence_fault (
        events,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_sequence_fault",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "push_sequence_fault",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_FaultNotifier::create_subscription_filter_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  CosNotifyFilter::Filter_var _tao_retval;
  CORBA::String_var constraint_grammar;
  if (!(
    (_tao_in >> constraint_grammar.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_subscription_filter",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->create_subscription_filter (
        constraint_grammar.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_subscription_filter",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "create_subscription_filter",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_FaultNotifier::connect_structured_fault_consumer_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  FT::FaultNotifier::ConsumerId _tao_retval = 0;
  CosNotifyComm::StructuredPushConsumer_var push_consumer;
  CosNotifyFilter::Filter_var filter;
  if (!(
    (_tao_in >> push_consumer.out ()) &&
    (_tao_in >> filter.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_structured_fault_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->connect_structured_fault_consumer (
        push_consumer.in (),
        filter.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_structured_fault_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_structured_fault_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval)
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_FaultNotifier::connect_sequence_fault_consumer_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  FT::FaultNotifier::ConsumerId _tao_retval = 0;
  CosNotifyComm::SequencePushConsumer_var push_consumer;
  CosNotifyFilter::Filter_var filter;
  if (!(
    (_tao_in >> push_consumer.out ()) &&
    (_tao_in >> filter.out ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_sequence_fault_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->connect_sequence_fault_consumer (
        push_consumer.in (),
        filter.in (),
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_sequence_fault_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "connect_sequence_fault_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval)
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_FaultNotifier::disconnect_consumer_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *)_tao_object_reference;

  FT::FaultNotifier::ConsumerId connection;
  if (!(
    (_tao_in >> connection)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "disconnect_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->disconnect_consumer (
        connection,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "disconnect_consumer",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "disconnect_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_FaultNotifier::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_FaultNotifier::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_FaultNotifier *_tao_impl = (FT_POA_FaultNotifier *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_FaultNotifier::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/FaultNotifier:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_FaultNotifier::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/FaultNotifier:1.0") == 0)
    return ACE_static_cast (FT_POA_FaultNotifier_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_FaultNotifier::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_FaultNotifier::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/FaultNotifier:1.0";
}

FT::FaultNotifier*
FT_POA_FaultNotifier::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::FaultNotifier_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_FaultNotifier (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::FaultNotifier::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_FaultNotifier::FT_POA_tao_thru_poa_collocated_FaultNotifier (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_FaultNotifier::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_FaultNotifier::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


void FT_POA_tao_thru_poa_collocated_FaultNotifier::push_structured_fault  (
    const CosNotification::StructuredEvent & event,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "push_structured_fault",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->push_structured_fault (
      event,
      ACE_TRY_ENV
    );
  return;
}

void FT_POA_tao_thru_poa_collocated_FaultNotifier::push_sequence_fault  (
    const CosNotification::EventBatch & events,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "push_sequence_fault",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->push_sequence_fault (
      events,
      ACE_TRY_ENV
    );
  return;
}

CosNotifyFilter::Filter_ptr FT_POA_tao_thru_poa_collocated_FaultNotifier::create_subscription_filter  (
    const char * constraint_grammar,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidGrammar
  ))
{
  CosNotifyFilter::Filter_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "create_subscription_filter",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->create_subscription_filter (
      constraint_grammar,
      ACE_TRY_ENV
    );

}

FT::FaultNotifier::ConsumerId FT_POA_tao_thru_poa_collocated_FaultNotifier::connect_structured_fault_consumer  (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  FT::FaultNotifier::ConsumerId _tao_retval = 0;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "connect_structured_fault_consumer",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN  (_tao_retval);
  return ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->connect_structured_fault_consumer (
      push_consumer,
      filter,
      ACE_TRY_ENV
    );

}

FT::FaultNotifier::ConsumerId FT_POA_tao_thru_poa_collocated_FaultNotifier::connect_sequence_fault_consumer  (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  FT::FaultNotifier::ConsumerId _tao_retval = 0;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "connect_sequence_fault_consumer",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN  (_tao_retval);
  return ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->connect_sequence_fault_consumer (
      push_consumer,
      filter,
      ACE_TRY_ENV
    );

}

void FT_POA_tao_thru_poa_collocated_FaultNotifier::disconnect_consumer  (
    FT::FaultNotifier::ConsumerId connection,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "disconnect_consumer",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_FaultNotifier_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/FaultNotifier:1.0"
        )
    )->disconnect_consumer (
      connection,
      ACE_TRY_ENV
    );
  return;
}



class TAO_FT_Checkpointable_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:36 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_Checkpointable_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_Checkpointable_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14,  0,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14,  0,
     14, 14, 14,  0, 14,  1, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14,  0,  0, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14,
#else
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14,  0, 14,  0, 14, 14,
     14,  0, 14,  1, 14, 14, 14, 14, 14, 14,
     14, 14, 14, 14, 14,  0,  0, 14, 14, 14,
     14, 14, 14, 14, 14, 14, 14, 14,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_Checkpointable_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 4,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 13,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 13,
      HASH_VALUE_RANGE = 9,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_Checkpointable::_is_a_skel},
      {"",0},{"",0},{"",0},
      {"set_state",     &FT_POA_Checkpointable::set_state_skel},
      {"get_state",     &FT_POA_Checkpointable::get_state_skel},
      {"",0},{"",0},
      {"_non_existent",  &FT_POA_Checkpointable::_non_existent_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:36 */
static TAO_FT_Checkpointable_Perfect_Hash_OpTable tao_FT_Checkpointable_optable;

FT::Checkpointable_ptr _TAO_collocation_POA_FT_Checkpointable_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::Checkpointable_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_Checkpointable (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_Checkpointable_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_Checkpointable_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_Checkpointable_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_Checkpointable_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_Checkpointable_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_Checkpointable_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_Checkpointable::FT_POA_Checkpointable (void)
{
  this->optable_ = &tao_FT_Checkpointable_optable;
}

// copy ctor
FT_POA_Checkpointable::FT_POA_Checkpointable (const FT_POA_Checkpointable& rhs)
  :   TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_Checkpointable::~FT_POA_Checkpointable (void)
{
}

void FT_POA_Checkpointable::get_state_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  FT_POA_Checkpointable *_tao_impl = (FT_POA_Checkpointable *)_tao_object_reference;

  FT::State_var _tao_retval;
#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_state",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_state (
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_state",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_state",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_Checkpointable::set_state_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_Checkpointable *_tao_impl = (FT_POA_Checkpointable *)_tao_object_reference;

  FT::State s;
  if (!(
    (_tao_in >> s)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_state",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->set_state (
        s,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_state",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_state",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  }

void FT_POA_Checkpointable::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_Checkpointable *_tao_impl = (FT_POA_Checkpointable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_Checkpointable::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_Checkpointable *_tao_impl = (FT_POA_Checkpointable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_Checkpointable::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Checkpointable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_Checkpointable::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/Checkpointable:1.0") == 0)
    return ACE_static_cast (FT_POA_Checkpointable_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_Checkpointable::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_Checkpointable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/Checkpointable:1.0";
}

FT::Checkpointable*
FT_POA_Checkpointable::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::Checkpointable_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_Checkpointable (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::Checkpointable::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_Checkpointable::FT_POA_tao_thru_poa_collocated_Checkpointable (
  TAO_Stub *stub
)
  :  CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_Checkpointable::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Checkpointable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Checkpointable:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_Checkpointable::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Checkpointable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Checkpointable:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


FT::State * FT_POA_tao_thru_poa_collocated_Checkpointable::get_state  (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::NoStateAvailable
  ))
{
  FT::State_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_state",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Checkpointable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Checkpointable:1.0"
        )
    )->get_state (
      ACE_TRY_ENV
    );

}

void FT_POA_tao_thru_poa_collocated_Checkpointable::set_state  (
    const FT::State & s,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidState
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_state",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_Checkpointable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Checkpointable:1.0"
        )
    )->set_state (
      s,
      ACE_TRY_ENV
    );
  return;
}



class TAO_FT_Updateable_Perfect_Hash_OpTable : public TAO_Perfect_Hash_OpTable
{
private:
  unsigned int hash (const char *str, unsigned int len);
public:
 const TAO_operation_db_entry * lookup (const char *str, unsigned int len);
};
/* starting time is 15:23:36 */
/* C++ code produced by gperf version 2.8 (ACE version) */
/* Command-line: /project/acetmp/bala/ACE_wrappers/bin/gperf -m -M -J -c -C -D -E -T -f 0 -F 0 -a -o -t -p -K opname_ -L C++ -Z TAO_FT_Updateable_Perfect_Hash_OpTable -N lookup  */
unsigned int
TAO_FT_Updateable_Perfect_Hash_OpTable::hash (const char *str, unsigned int len)
{
  static const unsigned char asso_values[] =
    {
#if defined (ACE_MVS)
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16,  0,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16,  0,
     16, 16, 16,  0, 16,  5, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16,  0,  0, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16,
#else
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16,  0, 16,  0, 16, 16,
     16,  0, 16,  5, 16, 16, 16, 16, 16, 16,
     16, 16, 16, 16, 16,  0,  0, 16, 16, 16,
     16, 16, 16, 16, 16, 16, 16, 16,
#endif /* ACE_MVS */
    };
  return len + asso_values[str[len - 1]] + asso_values[str[0]];
}

const class TAO_operation_db_entry *
TAO_FT_Updateable_Perfect_Hash_OpTable::lookup (const char *str, unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 6,
      MIN_WORD_LENGTH = 5,
      MAX_WORD_LENGTH = 13,
      MIN_HASH_VALUE = 5,
      MAX_HASH_VALUE = 15,
      HASH_VALUE_RANGE = 11,
      DUPLICATES = 0
    };

  static const class TAO_operation_db_entry  wordlist[] =
    {
      {"",0},{"",0},{"",0},{"",0},{"",0},
      {"_is_a",  &FT_POA_Updateable::_is_a_skel},
      {"",0},{"",0},{"",0},
      {"set_state",     &FT_POA_Updateable::set_state_skel},
      {"set_update",    &FT_POA_Updateable::set_update_skel},
      {"",0},{"",0},
      {"_non_existent",  &FT_POA_Updateable::_non_existent_skel},
      {"get_state",     &FT_POA_Updateable::get_state_skel},
      {"get_update",    &FT_POA_Updateable::get_update_skel},
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          const char *s = wordlist[key].opname_;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
            return &wordlist[key];
        }
    }
  return 0;
}
/* ending time is 15:23:36 */
static TAO_FT_Updateable_Perfect_Hash_OpTable tao_FT_Updateable_optable;

FT::Updateable_ptr _TAO_collocation_POA_FT_Updateable_Stub_Factory (
    CORBA::Object_ptr obj
  )
{
  TAO_Stub *stub = obj->_stubobj ();

  switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
    {
    case TAO_ORB_Core::THRU_POA:
      {
      FT::Updateable_ptr retval = 0;
      ACE_NEW_RETURN (
          retval,
          FT_POA_tao_thru_poa_collocated_Updateable (stub),
          0
        );
      return retval;
      }
    case TAO_ORB_Core::DIRECT:
      break;
    default:
      break;
    }
  return 0;
}

int _TAO_collocation_POA_FT_Updateable_Stub_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_FT_Updateable_Stub_Factory_function_pointer =
    _TAO_collocation_POA_FT_Updateable_Stub_Factory;

  return 0;
}

static int _TAO_collocation_POA_FT_Updateable_Stub_Factory_Initializer_Scarecrow =
  _TAO_collocation_POA_FT_Updateable_Stub_Factory_Initializer (ACE_reinterpret_cast (long, _TAO_collocation_POA_FT_Updateable_Stub_Factory_Initializer));

// skeleton constructor
FT_POA_Updateable::FT_POA_Updateable (void)
{
  this->optable_ = &tao_FT_Updateable_optable;
}

// copy ctor
FT_POA_Updateable::FT_POA_Updateable (const FT_POA_Updateable& rhs)
  :  FT_POA_Checkpointable (rhs),
    TAO_ServantBase (rhs)
{}

// skeleton destructor
FT_POA_Updateable::~FT_POA_Updateable (void)
{
}

void FT_POA_Updateable::get_update_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  FT_POA_Updateable *_tao_impl = (FT_POA_Updateable *)_tao_object_reference;

  FT::State_var _tao_retval;
#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_update",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_retval = _tao_impl->get_update (
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_update",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "get_update",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(
    (_tao_out << _tao_retval.in ())
  ))
    ACE_THROW (CORBA::MARSHAL () );

}

void FT_POA_Updateable::set_update_skel (
    CORBA::ServerRequest &_tao_server_request,
    void *_tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
)
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_Updateable *_tao_impl = (FT_POA_Updateable *)_tao_object_reference;

  FT::State s;
  if (!(
    (_tao_in >> s)
  ))
    ACE_THROW (CORBA::MARSHAL () );

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ServerRequestInterceptor_Adapter
    _tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));
  ACE_CHECK;
  PortableInterceptor::Cookies _tao_cookies;
  CORBA::NVList_var _tao_interceptor_args;
  CORBA::Object_var _tao_objref;
  if (_tao_vfr.valid ())
    {
      _tao_server_request.orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
      ACE_CHECK;

      _tao_objref = _tao_server_request.objref (ACE_TRY_ENV);
      ACE_CHECK;
    }

  ACE_TRY
    {
      _tao_vfr.preinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_update",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

      _tao_impl->set_update (
        s,
        ACE_TRY_ENV
      );

      TAO_INTERCEPTOR_CHECK;

#if (TAO_HAS_INTERCEPTORS == 1)
      _tao_vfr.postinvoke (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_update",
          _tao_server_request.service_info (),
          _tao_interceptor_args.inout (),
          _tao_cookies,
          ACE_TRY_ENV
        );
      TAO_INTERCEPTOR_CHECK;
    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_server_request.request_id (),
          1,
          _tao_objref.in (),
          "set_update",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
}

void FT_POA_Updateable::_is_a_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  TAO_InputCDR &_tao_in = _tao_server_request.incoming ();
  FT_POA_Updateable *_tao_impl = (FT_POA_Updateable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = 0;
  CORBA::String_var value;
  if (!(_tao_in >> value.out ()))
    ACE_THROW (CORBA::MARSHAL ());

  _tao_retval = _tao_impl->_is_a (value.in (), ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

void FT_POA_Updateable::_non_existent_skel (
    CORBA::ServerRequest &_tao_server_request,
    void * _tao_object_reference,
    void * /* context */,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  FT_POA_Updateable *_tao_impl = (FT_POA_Updateable *) _tao_object_reference;
  CORBA::Boolean _tao_retval = _tao_impl->_non_existent (ACE_TRY_ENV);
  ACE_CHECK;

  _tao_server_request.init_reply (ACE_TRY_ENV);
  ACE_CHECK;
  TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();
  if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))
    ACE_THROW (CORBA::MARSHAL ());
}

CORBA::Boolean FT_POA_Updateable::_is_a (
    const char* value,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Updateable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Checkpointable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (ACE_TRY_ENV))))
      return 1;
    else
      return 0;
}

void* FT_POA_Updateable::_downcast (
    const char* logical_type_id
  )
{
if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/Updateable:1.0") == 0)
    return ACE_static_cast (FT_POA_Updateable_ptr, this);
  if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/FT/Checkpointable:1.0") == 0)
    return ACE_static_cast (FT_POA_Checkpointable_ptr, this);
    if (ACE_OS::strcmp (logical_type_id, "IDL:omg.org/CORBA/Object:1.0") == 0)
    return ACE_static_cast(PortableServer::Servant, this);
  return 0;
}

void FT_POA_Updateable::_dispatch (CORBA::ServerRequest &req, void *context, CORBA::Environment &ACE_TRY_ENV)
{
  this->synchronous_upcall_dispatch(req, context, this, ACE_TRY_ENV);
}

const char* FT_POA_Updateable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/Updateable:1.0";
}

FT::Updateable*
FT_POA_Updateable::_this (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())
    switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
      {
      case TAO_ORB_Core::THRU_POA:
        {
          ::FT::Updateable_ptr retval = 0;
          ACE_NEW_RETURN (
              retval,
              FT_POA_tao_thru_poa_collocated_Updateable (stub),
              0
            );
          return retval;
        }
      case TAO_ORB_Core::DIRECT:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      default:
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
      }
  else
    {
      // stub->_incr_refcnt ();
      CORBA::Object_ptr tmp = CORBA::Object::_nil ();
      ACE_NEW_RETURN (tmp, CORBA::Object (stub), 0);
      CORBA::Object_var obj = tmp;
      return ::FT::Updateable::_unchecked_narrow (obj.in ());
    }
}

FT_POA_tao_thru_poa_collocated_Updateable::FT_POA_tao_thru_poa_collocated_Updateable (
  TAO_Stub *stub
)
  : FT_POA_tao_thru_poa_collocated_Checkpointable (stub),
    CORBA_Object (stub, 0, 1)
{
}

CORBA::Boolean FT_POA_tao_thru_poa_collocated_Updateable::_is_a(
    const CORBA::Char *logical_type_id,
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_is_a",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Updateable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Updateable:1.0"
        )
    )->_is_a (logical_type_id, ACE_TRY_ENV);
}


CORBA::Boolean FT_POA_tao_thru_poa_collocated_Updateable::_non_existent(
    CORBA_Environment &ACE_TRY_ENV
  )

{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "_non_existent",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Updateable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Updateable:1.0"
        )
    )->_non_existent (ACE_TRY_ENV);
}


FT::State * FT_POA_tao_thru_poa_collocated_Updateable::get_update  (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::NoUpdateAvailable
  ))
{
  FT::State_var _tao_retval;
  ACE_UNUSED_ARG (_tao_retval);
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "get_update",
      ACE_TRY_ENV
    );
  ACE_CHECK_RETURN (0);
  return ACE_reinterpret_cast (
      FT_POA_Updateable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Updateable:1.0"
        )
    )->get_update (
      ACE_TRY_ENV
    );

}

void FT_POA_tao_thru_poa_collocated_Updateable::set_update  (
    const FT::State & s,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT::InvalidUpdate
  ))
{
  TAO_Object_Adapter::Servant_Upcall servant_upcall (
      *this->_stubobj ()->servant_orb_var ()->orb_core ()->object_adapter ()
    );
  servant_upcall.prepare_for_upcall (
      this->_object_key (),
      "set_update",
      ACE_TRY_ENV
    );
  ACE_CHECK;
  ACE_reinterpret_cast (
      FT_POA_Updateable_ptr,
      servant_upcall.servant ()->_downcast (
          "IDL:omg.org/FT/Updateable:1.0"
        )
    )->set_update (
      s,
      ACE_TRY_ENV
    );
  return;
}

#endif /*TAO_HAS_FT_CORBA == 1*/
