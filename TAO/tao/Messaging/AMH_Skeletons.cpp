// -*- C++ -*-
// $Id$

#include "tao/Messaging/AMH_Skeletons.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/DynamicC.h"

#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/SArg_Traits_T.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/Object_SArg_Traits.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/get_arg.h"
#include "tao/Object.h"

#include "tao/PortableInterceptor.h"
#include "tao/PortableServer/SUpcall_commands.h"
#include "tao/ORB_Core.h"

#include "tao/TAO_Server_Request.h"

void
_is_a_amh_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< ::ACE_InputCDR::to_boolean> (
      this->operation_details_,
      this->args_);

  TAO::SArg_Traits< char *>::in_arg_type arg_1 =
    TAO::Portable_Server::get_in_arg< char *> (
      this->operation_details_,
      this->args_,
      1);

  retval =
    this->servant_-> _is_a (
      arg_1);
}

void
TAO_AMH_Skeletons::_is_a_amh_skel (TAO_ServerRequest & server_request,
                                  TAO::Portable_Server::Servant_Upcall* TAO_INTERCEPTOR (servant_upcall),
                                  void * servant)
{
#if TAO_HAS_INTERCEPTORS == 1
  static ::CORBA::TypeCode_ptr const * const exceptions = 0;
  static ::CORBA::ULong const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */

  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val retval;
  TAO::SArg_Traits< char *>::in_arg_val _tao_repository_id;

  TAO::Argument * const args[] =
    {
      &retval,
      &_tao_repository_id
    };

  static size_t const nargs = 2;

  TAO_ServantBase * const impl =
    static_cast<TAO_ServantBase *> (servant);

  _is_a_amh_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);

  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         );
  server_request.tao_send_reply ();
}


#if (TAO_HAS_MINIMUM_CORBA == 0)
void
_non_existent_amh_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< ::ACE_InputCDR::to_boolean> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _non_existent ();
}

void TAO_AMH_Skeletons::_non_existent_amh_skel (TAO_ServerRequest & server_request,
                                                TAO::Portable_Server::Servant_Upcall* TAO_INTERCEPTOR (servant_upcall),
                                                void * servant)
{
#if TAO_HAS_INTERCEPTORS == 1
  static ::CORBA::TypeCode_ptr const * const exceptions = 0;
  static ::CORBA::ULong const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */

  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val retval;

  TAO::Argument * const args[] =
    {
      &retval
    };

  static size_t const nargs = 1;
  TAO_ServantBase * const impl =
    static_cast<TAO_ServantBase *> (servant);

  _non_existent_amh_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);

  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         );
  server_request.tao_send_reply ();
}


# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
void
TAO_AMH_Skeletons::_interface_amh_skel (TAO_ServerRequest & server_request,
                                        TAO::Portable_Server::Servant_Upcall* /* servant_upcall */,
                                        void * servant)
{
  TAO_IFR_Client_Adapter *_tao_adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ());

  if (!_tao_adapter)
    {
      throw ::CORBA::INTF_REPOS (::CORBA::OMGVMCID | 1, ::CORBA::COMPLETED_NO);
    }

  TAO_ServantBase * const impl =
    static_cast<TAO_ServantBase *> (servant);
  ::CORBA::InterfaceDef_ptr _tao_retval = impl->_get_interface ();
  server_request.init_reply ();
  TAO_OutputCDR &_tao_out = *server_request.outgoing ();

  ::CORBA::Boolean const _tao_result =
    _tao_adapter->interfacedef_cdr_insert (_tao_out, _tao_retval);

  _tao_adapter->dispose (_tao_retval);

  if (!_tao_result)
    {
      throw ::CORBA::MARSHAL ();
    }
}

void
_get_component_amh_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::CORBA::Object>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< ::CORBA::Object> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _get_component ();
}

void
TAO_AMH_Skeletons::_component_amh_skel (TAO_ServerRequest & server_request,
                                        TAO::Portable_Server::Servant_Upcall* TAO_INTERCEPTOR (servant_upcall),
                                        void * servant)
{
#if TAO_HAS_INTERCEPTORS == 1
  static ::CORBA::TypeCode_ptr const * const exceptions = 0;
  static ::CORBA::ULong const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */

  TAO::SArg_Traits< ::CORBA::Object>::ret_val retval;

  TAO::Argument * const args[] =
    {
      &retval
    };

  static size_t const nargs = 1;

  TAO_ServantBase * const impl =
    static_cast<TAO_ServantBase *> (servant);

  _get_component_amh_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);

  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                         );
}
# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */


void
_repository_id_amh_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< char *>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< char *> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _repository_id ();
}

void
TAO_AMH_Skeletons::_repository_id_amh_skel (TAO_ServerRequest & server_request,
                                            TAO::Portable_Server::Servant_Upcall* TAO_INTERCEPTOR (servant_upcall),
                                            void * servant)
{
#if TAO_HAS_INTERCEPTORS == 1
  static ::CORBA::TypeCode_ptr const * const exceptions = 0;
  static ::CORBA::ULong const nexceptions = 0;
#endif /* TAO_HAS_INTERCEPTORS */

  TAO::SArg_Traits< char *>::ret_val retval;

  TAO::Argument * const args[] =
    {
      &retval
    };

  static size_t const nargs = 1;

  TAO_ServantBase * const impl =
    static_cast<TAO_ServantBase *> (servant);

  _repository_id_amh_Upcall_Command command (
    impl,
    server_request.operation_details (),
    args);

  TAO::Upcall_Wrapper upcall_wrapper;
  upcall_wrapper.upcall (server_request
                         , args
                         , nargs
                         , command
#if TAO_HAS_INTERCEPTORS == 1
                         , servant_upcall
                         , exceptions
                         , nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                 );
  server_request.tao_send_reply ();
}
#endif /* TAO_HAS_MINIMUM_CORBA */


