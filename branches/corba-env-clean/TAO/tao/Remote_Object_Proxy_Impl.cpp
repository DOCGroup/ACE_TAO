// $Id$

#include "tao/Remote_Object_Proxy_Impl.h"
#include "tao/Exception.h"
#include "tao/Any.h"
#include "tao/Stub.h"
#include "tao/Invocation.h"
#include "tao/IFR_Client_Adapter.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, TAO_Remote_Object_Proxy_Impl, "$Id$")

TAO_Remote_Object_Proxy_Impl::TAO_Remote_Object_Proxy_Impl (void)
{
  // No-Op
}


TAO_Remote_Object_Proxy_Impl::~TAO_Remote_Object_Proxy_Impl (void)
{
  // No-Op
}

CORBA::Boolean
TAO_Remote_Object_Proxy_Impl::_is_a (const CORBA::Object_ptr target,
                                     const CORBA::Char *logical_type_id
                                     TAO_ENV_ARG_DECL)
{
  // Here we go remote to answer the question.
  CORBA::Boolean _tao_retval = 0;

  TAO_Stub *istub = target->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EINVAL),
                        CORBA::COMPLETED_NO),
                      _tao_retval);

  TAO_GIOP_Twoway_Invocation _tao_call (istub,
                                        "_is_a",
                                        5,
                                        1,
                                        istub->orb_core ());

  // Loop until we succeed or we raise an exception.
  // @@ Nanbor: Do we still need to clear the environment variable?
  //  ACE_TRY_ENV.clear ();
  for (;;)
    {
      _tao_call.start (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (_tao_retval);

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag)
                                 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (_tao_retval);

      TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
      if (!(
            (_tao_out << logical_type_id)
            ))
        ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

      int _invoke_status =
        _tao_call.invoke (0, 0 TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (_tao_retval);

      if (_invoke_status == TAO_INVOKE_RESTART)
        continue;
      // if (_invoke_status == TAO_INVOKE_EXCEPTION)
      // cannot happen
      if (_invoke_status != TAO_INVOKE_OK)
        {
          ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            _tao_retval);

        }
      break;
    }
  TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
  if (!(_tao_in >> CORBA::Any::to_boolean (_tao_retval)))
    ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_YES),
                      _tao_retval);
  return _tao_retval;

}

#if (TAO_HAS_MINIMUM_CORBA == 0)

CORBA::Boolean
TAO_Remote_Object_Proxy_Impl::_non_existent (const CORBA::Object_ptr target
                                             TAO_ENV_ARG_DECL)
{
  CORBA::Boolean _tao_retval = 0;

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
                                            "_non_existent",
                                            13,
                                            1,
                                            istub->orb_core ());

      // ACE_TRY_ENV.clear ();
      for (;;)
        {
          _tao_call.start (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

          _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag)
                                     TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          int _invoke_status =
            _tao_call.invoke (0, 0 TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (_invoke_status == TAO_INVOKE_RESTART)
            continue;
          ACE_ASSERT (_invoke_status != TAO_INVOKE_EXCEPTION);
          if (_invoke_status != TAO_INVOKE_OK)
            {
              ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_YES),
                                _tao_retval);
            }
          break;
        }
      TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
      if (!(
            (_tao_in >> CORBA::Any::to_boolean (_tao_retval))
            ))
        ACE_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      _tao_retval = 1;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  return _tao_retval;
}

CORBA_InterfaceDef_ptr
TAO_Remote_Object_Proxy_Impl::_get_interface (const CORBA::Object_ptr target
                                              TAO_ENV_ARG_DECL)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        0);
    }

  return adapter->get_interface_remote (target
                                         TAO_ENV_ARG_PARAMETER);
}

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
