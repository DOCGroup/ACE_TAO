// -*- C++ -*-
// $Id$

#include "tao/PortableServer/SUpcall_commands.h"

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

void
_is_a_thru_poa_Upcall_Command::execute (void)
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
_is_a_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
    static_cast<TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val *> (this->args_[0])->arg ();

  TAO::SArg_Traits< char *>::in_arg_type arg_1 =
    static_cast<TAO::SArg_Traits< char *>::in_arg_val *> (this->args_[1])->arg ();

  retval =
    this->servant_-> _is_a (
      arg_1);
}

#if (TAO_HAS_MINIMUM_CORBA == 0)
void
_non_existent_thru_poa_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< ::ACE_InputCDR::to_boolean> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _non_existent ();
}

void
_non_existent_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_arg_type retval =
    static_cast<TAO::SArg_Traits< ::ACE_InputCDR::to_boolean>::ret_val *> (this->args_[0])->arg ();

  retval =
    this->servant_-> _non_existent ();
}


# if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
void
_get_component_thru_poa_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::CORBA::Object>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< ::CORBA::Object> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _get_component ();
}

void
_get_component_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< ::CORBA::Object>::ret_arg_type retval =
    static_cast<TAO::SArg_Traits< ::CORBA::Object>::ret_val *> (this->args_[0])->arg ();

  retval =
    this->servant_-> _get_component ();
}

# endif /* !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO) */


void
_repository_id_thru_poa_Upcall_Command::execute (void)
{
  TAO::SArg_Traits< char *>::ret_arg_type retval =
    TAO::Portable_Server::get_ret_arg< char *> (
      this->operation_details_,
      this->args_);

  retval =
    this->servant_-> _repository_id ();
}

void
_repository_id_Upcall_Command::execute(void )
{
  TAO::SArg_Traits< char *>::ret_arg_type retval =
    static_cast<TAO::SArg_Traits< char *>::ret_val *> (this->args_[0])->arg ();

  retval =
    this->servant_-> _repository_id ();
}
#endif /* TAO_HAS_MINIMUM_CORBA */

