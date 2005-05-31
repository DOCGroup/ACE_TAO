// "$Id$"

#include "ORB.h"

ACE_RCSID (tao,
           ORB,
           "$Id$")

#include "ORB_Table.h"
#include "Connector_Registry.h"
#include "IOR_Parser.h"
#include "Stub.h"
#include "ORB_Core.h"
#include "TAO_Internal.h"
#include "NVList.h"
#include "Dynamic_Adapter.h"
#include "Profile.h"
#include "default_ports.h"
#include "ORBInitInfo.h"
#include "ORBInitializer_Registry.h"
#include "TAO_Singleton_Manager.h"
#include "Policy_Manager.h"
#include "Valuetype_Adapter.h"
#include "IFR_Client_Adapter.h"
#include "CodecFactory_ORBInitializer.h"
#include "TypeCodeFactory_Adapter.h"
#include "PolicyFactory_Registry_Adapter.h"
#include "debug.h"
#include "CDR.h"
#include "SystemException.h"
#include "default_environment.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "PICurrent_ORBInitializer.h"  /* @@ This should go away! */
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#if !defined (__ACE_INLINE__)
# include "ORB.i"
#endif /* ! __ACE_INLINE__ */

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Reactor.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/ACE.h"
#include "ace/Static_Object_Lock.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_ctype.h"

#if defined (ACE_HAS_EXCEPTIONS)
void TAO_unexpected_exception_handler (void)
{
  throw CORBA::UNKNOWN ();
}
#endif  /* ACE_HAS_EXCEPTIONS */


static const char ior_prefix[] = "IOR:";

// = Static initialization.

namespace
{
  // Count of the number of ORBs.
  int orb_init_count = 0;
}

// ****************************************************************

CORBA::ORB::InvalidName::InvalidName (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/ORB/InvalidName:1.0",
                          "InvalidName")
{
}

// Destructor - all members are of self managing types.
CORBA::ORB::InvalidName::~InvalidName (void)
{
}

// Copy constructor.
CORBA::ORB::InvalidName::InvalidName (const CORBA::ORB::InvalidName &exc)
  : CORBA::UserException (exc._rep_id (),
                          exc._name ())
{
}

// Assignment operator.
CORBA::ORB::InvalidName&
CORBA::ORB::InvalidName::operator= (const ::CORBA::ORB::InvalidName &_tao_excp)
{
  this->UserException::operator= (_tao_excp);
  return *this;
}

CORBA::ORB::InvalidName *
CORBA::ORB::InvalidName::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/CORBA/ORB/InvalidName:1.0",
                       exc->_rep_id ()))
    {
      return dynamic_cast<InvalidName *> (exc);
    }
  else
    {
      return 0;
    }
}

CORBA::Exception *CORBA::ORB::InvalidName::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::CORBA::ORB::InvalidName, 0);
  return retval;
}

CORBA::Exception *
CORBA::ORB::InvalidName::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (
      result,
      ::CORBA::ORB::InvalidName (*this),
      0
    );
  return result;
}

void CORBA::ORB::InvalidName::_raise (void) const
{
  TAO_RAISE (*this);
}

void CORBA::ORB::InvalidName::_tao_encode (
    TAO_OutputCDR &
    ACE_ENV_ARG_DECL
  ) const
{
  ACE_THROW (CORBA::MARSHAL ());
}

void CORBA::ORB::InvalidName::_tao_decode (
    TAO_InputCDR &
    ACE_ENV_ARG_DECL
  )
{
  ACE_THROW (CORBA::MARSHAL ());
}

// ****************************************************************

CORBA::ORB::ORB (TAO_ORB_Core *orb_core)
  : lock_ ()
  , refcount_ (1)
  , orb_core_ (orb_core)
  , use_omg_ior_format_ (1)
  , timeout_ (0)
{
}

CORBA::ORB::~ORB (void)
{
  // This destructor is only invoked when the last ORB reference (not
  // instance) is being destroyed.
}

void
CORBA::ORB::shutdown (CORBA::Boolean wait_for_completion
                     ACE_ENV_ARG_DECL)
{
  // We cannot lock the exceptions here. We need to propogate
  // BAD_INV_ORDER  exceptions if needed to the caller. Locking
  // exceptions down would render us non-compliant with the spec.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core ()->shutdown (wait_for_completion
                               ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->orb_core () == 0)
    {
      // If the ORB_Core pointer is zero, assume that the ORB_Core has
      // been destroyed.

      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::OBJECT_NOT_EXIST exception if the ORB has been
      // destroyed by the time an ORB function is called.

      ACE_THROW (CORBA::OBJECT_NOT_EXIST (0,
                                          CORBA::COMPLETED_NO));
    }

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CORBA::ORB::destroy() has been called on ORB <%s>.\n"),
                  ACE_TEXT_CHAR_TO_TCHAR (this->orb_core ()->orbid ())));
    }

  this->orb_core ()->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Now invalidate the pointer to the ORB_Core that created this
  // ORB.
  this->orb_core_ = 0;
}

void
CORBA::ORB::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->run (0 ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::run (ACE_Time_Value &tv ACE_ENV_ARG_DECL)
{
  this->run (&tv ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::run (ACE_Time_Value *tv
                 ACE_ENV_ARG_DECL)
{
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core ()->run (tv, 0 ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::perform_work (ACE_ENV_SINGLE_ARG_DECL)
{
  this->perform_work (0 ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::perform_work (ACE_Time_Value &tv ACE_ENV_ARG_DECL)
{
  this->perform_work (&tv ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::perform_work (ACE_Time_Value *tv
                         ACE_ENV_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_core ()->run (tv, 1 ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
CORBA::ORB::work_pending (ACE_Time_Value &tv ACE_ENV_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const int result = this->orb_core_->reactor ()->work_pending (tv);
  if (result == 0 || (result == -1 && errno == ETIME))
    return 0;

  if (result == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  return 1;
}

CORBA::Boolean
CORBA::ORB::work_pending (ACE_ENV_SINGLE_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const int result = this->orb_core_->reactor ()->work_pending ();
  if (result == 0)
    return 0;

  if (result == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  return 1;
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

void
CORBA::ORB::create_list (CORBA::Long count,
                         CORBA::NVList_ptr &new_list
                         ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (CORBA::ULong (count) <= UINT_MAX);

  // Create an empty list
  ACE_NEW_THROW_EX (new_list,
                    CORBA::NVList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  // If count is greater than 0, create a list of NamedValues.
  if (count != 0)
    {
      new_list->max_ = (CORBA::ULong) count;

      for (CORBA::Long i = 0; i < count; ++i)
        {
          CORBA::NamedValue_ptr nv = 0;
          ACE_NEW_THROW_EX (nv,
                            CORBA::NamedValue,
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK;

          new_list->values_.enqueue_tail (nv);
        }
    }
}

void
CORBA::ORB::create_exception_list (CORBA::ExceptionList_ptr &list
                                   ACE_ENV_ARG_DECL)
{
  TAO_Dynamic_Adapter *dynamic_adapter =
    ACE_Dynamic_Service<TAO_Dynamic_Adapter>::instance (
        TAO_ORB_Core::dynamic_adapter_name ()
      );

  dynamic_adapter->create_exception_list (list
                                          ACE_ENV_ARG_PARAMETER);
}

void
CORBA::ORB::create_operation_list (CORBA::OperationDef_ptr opDef,
                                   CORBA::NVList_ptr &result
                                   ACE_ENV_ARG_DECL)
{
  TAO_IFR_Client_Adapter *adapter =
    ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW (CORBA::INTF_REPOS ());
    }

  adapter->create_operation_list (this,
                                  opDef,
                                  result
                                  ACE_ENV_ARG_PARAMETER);
}


void
CORBA::ORB::create_environment (CORBA::Environment_ptr &environment
                                ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (environment,
                    CORBA::Environment (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

void
CORBA::ORB::create_named_value (CORBA::NamedValue_ptr &nv
                               ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (nv,
                    CORBA::NamedValue,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

// The following functions are not implemented - they just throw
// CORBA::NO_IMPLEMENT.

CORBA::Boolean
CORBA::ORB::get_service_information (
  CORBA::ServiceType /* service_type */,
  CORBA::ServiceInformation_out /* service_information */
  ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

void
CORBA::ORB::create_context_list (CORBA::ContextList_ptr &
                                ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA::ORB::get_default_context (CORBA::Context_ptr &
                                ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA::ORB::send_multiple_requests_oneway (const CORBA::ORB::RequestSeq &
                                          ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA::ORB::send_multiple_requests_deferred (const CORBA::ORB::RequestSeq &
                                            ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

void
CORBA::ORB::get_next_response (CORBA::Request_ptr &
                              ACE_ENV_ARG_DECL)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 0,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

CORBA::Boolean
CORBA::ORB::poll_next_response (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

// ****************************************************************

CORBA::TypeCode_ptr
CORBA::ORB::create_struct_tc (
    const char *id,
    const char *name,
    const CORBA::StructMemberSeq & members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_struct_tc (id,
                                    name,
                                    members
                                    ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_union_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr discriminator_type,
    const CORBA::UnionMemberSeq & members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_union_tc (id,
                                   name,
                                   discriminator_type,
                                   members
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_enum_tc (
    const char *id,
    const char *name,
    const CORBA::EnumMemberSeq & members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_enum_tc (id,
                                  name,
                                  members
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_alias_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr original_type
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_alias_tc (id,
                                   name,
                                   original_type
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_exception_tc (
    const char *id,
    const char *name,
    const CORBA::StructMemberSeq & members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_exception_tc (id,
                                       name,
                                       members
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_interface_tc (id,
                                       name
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_string_tc (
    CORBA::ULong bound
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_string_tc (bound
                                    ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_wstring_tc (
    CORBA::ULong bound
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_wstring_tc (bound
                                     ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_fixed_tc (
    CORBA::UShort digits,
    CORBA::UShort scale
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_fixed_tc (digits,
                                   scale
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_sequence_tc (
    CORBA::ULong bound,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_sequence_tc (bound,
                                      element_type
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_array_tc (
    CORBA::ULong length,
    CORBA::TypeCode_ptr element_type
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_array_tc (length,
                                   element_type
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_value_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq & members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_value_tc (id,
                                   name,
                                   type_modifier,
                                   concrete_base,
                                   members
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_value_box_tc (
    const char *id,
    const char *name,
    CORBA::TypeCode_ptr boxed_type
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_value_box_tc (id,
                                       name,
                                       boxed_type
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_native_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_native_tc (id,
                                    name
                                    ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_recursive_tc (
    const char *id
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_recursive_tc (id
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_abstract_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_abstract_interface_tc (id,
                                                name
                                                ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_local_interface_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_local_interface_tc (id,
                                             name
                                             ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_component_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_component_tc (id,
                                       name
                                       ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_home_tc (
    const char *id,
    const char *name
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_home_tc (id,
                                  name
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
CORBA::ORB::create_event_tc (
    const char *id,
    const char *name,
    CORBA::ValueModifier type_modifier,
    CORBA::TypeCode_ptr concrete_base,
    const CORBA::ValueMemberSeq &members
    ACE_ENV_ARG_DECL
  )
{
  TAO_TypeCodeFactory_Adapter *adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }

  return adapter->create_event_tc (id,
                                   name,
                                   type_modifier,
                                   concrete_base,
                                   members
                                   ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

#endif /* TAO_HAS_MINIMUM_CORBA */

CORBA::Object_ptr
CORBA::ORB::resolve_root_poa (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->orb_core_->root_poa (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
CORBA::ORB::resolve_poa_current (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return this->orb_core_->poa_current ();
}

CORBA::Object_ptr
CORBA::ORB::resolve_policy_manager (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager *policy_manager =
    this->orb_core_->policy_manager ();

  if (policy_manager == 0)
    {
      return CORBA::Object::_nil ();
    }

  return CORBA::Object::_duplicate (policy_manager);

#else

  return CORBA::Object::_nil ();

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}

CORBA::Object_ptr
CORBA::ORB::resolve_policy_current (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Current &policy_current = this->orb_core_->policy_current ();
  return CORBA::Object::_duplicate (&policy_current);

#else

  return CORBA::Object::_nil ();

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}

CORBA::Object_ptr
CORBA::ORB::resolve_service (TAO_MCAST_SERVICEID mcast_service_id
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  static const char * env_service_port[] =
  {
    "NameServicePort",
    "TradingServicePort",
    "ImplRepoServicePort",
    "InterfaceRepoServicePort"
  };

  static const unsigned short default_service_port[] =
  {
    TAO_DEFAULT_NAME_SERVER_REQUEST_PORT,
    TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT,
    TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT,
    TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT
  };

  CORBA::Object_var return_value;

  // By now, the table filled in with -ORBInitRef arguments has been
  // checked.  We only get here if the table didn't contain an initial
  // reference for the requested Service.

  // First, determine if the port was supplied on the command line
  unsigned short port =
  this->orb_core_->orb_params ()->service_port (mcast_service_id);

  if (port == 0)
  {
    // Look for the port among our environment variables.
    const char *port_number =
      ACE_OS::getenv (env_service_port[mcast_service_id]);

    if (port_number != 0)
      port = static_cast<unsigned short> (ACE_OS::atoi (port_number));
    else
      port = default_service_port[mcast_service_id];
  }

  // Set the port value in ORB_Params: modify the default mcast
  // value.
  static const char prefix[] = "mcast://:";

  char port_char[256];

  ACE_OS::itoa (port,
                port_char,
                10);

  CORBA::String_var port_ptr =
    CORBA::string_alloc (static_cast<CORBA::ULong> (
                           ACE_OS::strlen ((const char *) port_char)));

  port_ptr = (const char *) port_char;

  CORBA::String_var def_init_ref =
    CORBA::string_alloc (sizeof (prefix) +
                         static_cast<CORBA::ULong> (
                           ACE_OS::strlen (port_ptr.in ())) + 2);

  ACE_OS::strcpy (def_init_ref.inout (), prefix);
  ACE_OS::strcat (def_init_ref.inout (), port_ptr.in ());
  ACE_OS::strcat (def_init_ref.inout (), "::");

  CORBA::String_var default_init_ref =
        this->orb_core_->orb_params ()->default_init_ref ();

  static const char mcast_prefix[] = "mcast://:::";

  if ((ACE_OS::strncmp (default_init_ref.in (),
                       mcast_prefix,
                       sizeof mcast_prefix - 1) == 0))
  {
    this->orb_core_->orb_params ()->default_init_ref (def_init_ref.in ());
  }

  return CORBA::Object::_nil ();
}

CORBA::Object_ptr
CORBA::ORB::resolve_initial_references (const char *name
                                        ACE_ENV_ARG_DECL)
{
  return this->resolve_initial_references (name,
                                           0
                                           ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
CORBA::ORB::resolve_initial_references (const char *name,
                                        ACE_Time_Value *timeout
                                        ACE_ENV_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (ACE_OS::strcmp (name, TAO_OBJID_ROOTPOA) == 0)
    return this->resolve_root_poa (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POACURRENT) == 0)
    return this->resolve_poa_current (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYMANAGER) == 0)
    return this->resolve_policy_manager (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_POLICYCURRENT) == 0)
    return this->resolve_policy_current (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_IORMANIPULATION) == 0)
    return this->orb_core ()->resolve_ior_manipulation (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_IORTABLE) == 0)
    return this->orb_core ()->resolve_ior_table (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_DYNANYFACTORY) == 0)
    return this->orb_core ()->resolve_dynanyfactory (ACE_ENV_SINGLE_ARG_PARAMETER);

  else if (ACE_OS::strcmp (name, TAO_OBJID_TYPECODEFACTORY) == 0)
    return this->orb_core ()->resolve_typecodefactory (ACE_ENV_SINGLE_ARG_PARAMETER);


  // -----------------------------------------------------------------

  // Search the object reference table.  This search must occur before
  // the InitRef table search, since it may contain local objects.
  CORBA::Object_var result =
    this->orb_core ()->object_ref_table ().resolve_initial_references (
      name
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (!CORBA::is_nil (result.in ()))
    return result._retn ();

  // -----------------------------------------------------------------

  // Check ORBInitRef options.
  ACE_CString ior;
  ACE_CString object_id (name);

  // Is the service name in the IOR Table.
  if (this->orb_core_->init_ref_map ()->find (object_id, ior) == 0)
    return this->string_to_object (ior.c_str ()
                                   ACE_ENV_ARG_PARAMETER);

  // Look for an environment variable called "<name>IOR".
  //
  CORBA::String_var ior_env_var_name =
    CORBA::string_alloc (ACE_OS::strlen (name) + 3);

  ACE_OS::strcpy (ior_env_var_name.inout (),
                  name);

  ACE_OS::strcat (ior_env_var_name.inout (),
                  "IOR");

  ACE_CString service_ior = ACE_OS::getenv (ior_env_var_name.in ());

  if (ACE_OS::strcmp (service_ior.c_str (), "") != 0 )
    {
      result =
        this->string_to_object (service_ior.c_str()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      return result._retn ();
    }

  // May be trying the explicitly specified services and the well
  // known services should be tried first before falling on to default
  // services.

  // Set the timeout value.
  this->set_timeout (timeout);

  if (ACE_OS::strcmp (name, TAO_OBJID_NAMESERVICE) == 0)
    {
      result = this->resolve_service (NAMESERVICE
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (!CORBA::is_nil (result.in ()))
        return result._retn ();
    }
  else if (ACE_OS::strcmp (name, TAO_OBJID_TRADINGSERVICE) == 0)
    {
      result = this->resolve_service (TRADINGSERVICE
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (!CORBA::is_nil (result.in ()))
        return result._retn ();
    }
  else if (ACE_OS::strcmp (name, TAO_OBJID_IMPLREPOSERVICE) == 0)
    {
      result = this->resolve_service (IMPLREPOSERVICE
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (!CORBA::is_nil (result.in ()))
        return result._retn ();
    }
  else if (ACE_OS::strcmp (name, TAO_OBJID_INTERFACEREP) == 0)
    {
      result = this->resolve_service (INTERFACEREPOSERVICE
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (!CORBA::is_nil (result.in ()))
        return result._retn ();
    }

  // Is not one of the well known services, try to find it in the
  // InitRef table....check the defaultinitref values also.

  result = this->orb_core ()->resolve_rir (name ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (!CORBA::is_nil (result.in ()))
    return result._retn ();
  // -----------------------------------------------------------------

  ACE_THROW_RETURN (CORBA::ORB::InvalidName (), CORBA::Object::_nil ());
}

void
CORBA::ORB::register_initial_reference (const char * id,
                                        CORBA::Object_ptr obj
                                        ACE_ENV_ARG_DECL)
{
  TAO_Object_Ref_Table &table = this->orb_core_->object_ref_table ();

  table.register_initial_reference (id,
                                    obj
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


CORBA::ORB::ObjectIdList_ptr
CORBA::ORB::list_initial_services (ACE_ENV_SINGLE_ARG_DECL)
{
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return
    this->orb_core ()->list_initial_references (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
CORBA::ORB::check_shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->orb_core () != 0)
    {
      this->orb_core ()->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      // If the ORB_Core pointer is zero, assume that the ORB_Core has
      // been destroyed.

      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::OBJECT_NOT_EXIST exception if the ORB has been
      // destroyed by the time an ORB function is called.

      ACE_THROW (CORBA::OBJECT_NOT_EXIST (0,
                                          CORBA::COMPLETED_NO));
    }
}

// ****************************************************************

void
TAO::ORB::init_orb_globals (ACE_ENV_SINGLE_ARG_DECL)
{
  // This method should be invoked atomically.  It is the caller's
  // responsibility to ensure that this condition is satisfied.

  // Prevent multiple initializations.
  if (orb_init_count != 0)
    {
      return;
    }
  else
    {
      ++orb_init_count;
    }

#if defined (ACE_HAS_EXCEPTIONS)
  // This must be done after the system TypeCodes and Exceptions have
  // been initialized.  An unexpected exception will cause TAO's
  // unexpected exception handler to be called.  That handler
  // transforms all unexpected exceptions to CORBA::UNKNOWN, which of
  // course requires the TypeCode constants and system exceptions to
  // have been initialized.
  /**
   * @note The new TypeCode implementation obviates the above
   *       comment.  Standard CORBA exception TypeCodes are now
   *       available before CORBA::ORB_init() is ever called.
   */
  TAO_Singleton_Manager::instance ()->_set_unexpected (
    ::TAO_unexpected_exception_handler);
#endif /* ACE_HAS_EXCEPTIONS */

  // Verify some of the basic implementation requirements.  This test
  // gets optimized away by a decent compiler (or else the rest of the
  // routine does).
  //
  // NOTE:  we still "just" assume that native floating point is IEEE.
  if (   sizeof (CORBA::Boolean)    != 1
      || sizeof (CORBA::Short)      != 2
      || sizeof (CORBA::Long)       != 4
      || sizeof (CORBA::LongLong)   != 8
      || sizeof (CORBA::Float)      != 4
      || sizeof (CORBA::Double)     != 8
      || sizeof (CORBA::LongDouble) != 16
      || sizeof (CORBA::WChar)      < 2
      || sizeof (void *)            != ACE_SIZEOF_VOID_P)
    {
      ACE_ERROR ((LM_ERROR,
                  "%N; ERROR: unexpected basic type size; "
                  "b:%d s:%d l:%d ll:%d f:%d d:%d ld:%d wc:%d v:%d\n"
                  "please reconfigure TAO\n",
                  sizeof (CORBA::Boolean),
                  sizeof (CORBA::Short),
                  sizeof (CORBA::Long),
                  sizeof (CORBA::LongLong),
                  sizeof (CORBA::Float),
                  sizeof (CORBA::Double),
                  sizeof (CORBA::LongDouble),
                  sizeof (CORBA::WChar),
                  sizeof (void *)));

      ACE_THROW (CORBA::INITIALIZE ());
    }

  // Register the CodecFactory ORBInitializer.
  PortableInterceptor::ORBInitializer_ptr tmp_cf_initializer;
  ACE_NEW_THROW_EX (tmp_cf_initializer,
                    TAO_CodecFactory_ORBInitializer,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ORBInitializer_var cf_initializer =
    tmp_cf_initializer;

  PortableInterceptor::register_orb_initializer (cf_initializer.in ()
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // -------------------------------------------------------------
  // @@ These ORB initializer instantiations should go away.  They
  //    should be registered via the service configurator, for
  //    example.

#if TAO_HAS_INTERCEPTORS == 1
  PortableInterceptor::ORBInitializer_ptr temp_pi_initializer =
    PortableInterceptor::ORBInitializer::_nil ();
  PortableInterceptor::ORBInitializer_var pi_initializer;

  // Register the PICurrent ORBInitializer.
  ACE_NEW_THROW_EX (temp_pi_initializer,
                    TAO_PICurrent_ORBInitializer,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  pi_initializer = temp_pi_initializer;

  PortableInterceptor::register_orb_initializer (pi_initializer.in ()
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

const ACE_CString &
CORBA::ORB::_tao_ft_client_id (void)
{
  return this->orb_core ()->fault_tolerance_service ().client_id ();
}

void
CORBA::ORB::_tao_ft_client_id (const char *id)
{
  this->orb_core ()->fault_tolerance_service ().client_id (id);
}

CORBA::ORB_ptr
CORBA::ORB::_tao_make_ORB (TAO_ORB_Core * orb_core)
{
  CORBA::ORB_ptr orb = CORBA::ORB::_nil ();

  ACE_NEW_RETURN (orb,
                  CORBA::ORB (orb_core),
                  CORBA::ORB::_nil ());

  return orb;
}

// ****************************************************************

// ORB initialization, per OMG document 98-12-01.

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *argv[],
                 const char *orb_name)
{
  // Make sure TAO's singleton manager is initialized.
  //
  // We need to initialize before TAO_default_environment() is called
  // since that call instantiates a TAO_TSS_Singleton.
  if (TAO_Singleton_Manager::instance ()->init () == -1)
    {
      return CORBA::ORB::_nil ();
    }

  return CORBA::ORB_init (argc,
                          argv,
                          orb_name,
                          TAO_default_environment ());
}

CORBA::ORB_ptr
CORBA::ORB_init (int &argc,
                 char *argv[],
                 const char *orbid,
                 CORBA::Environment &ACE_TRY_ENV)
{
  // This ugly macro will go away once we've merged the two ORB_init's.
  ACE_UNUSED_ARG(ACE_TRY_ENV); // FUZZ: ignore check_for_ace_check

  // Use this string variable to hold the orbid
  ACE_CString orbid_string (orbid);

  // Copy command line parameter not to use original.
  ACE_Argv_Type_Converter command_line(argc, argv);

  {
    // Using ACE_Static_Object_Lock::instance() precludes ORB_init()
    // from being called within a static object CTOR.
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                              guard,
                              *ACE_Static_Object_Lock::instance (),
                              CORBA::ORB::_nil ()));

    // Make sure TAO's singleton manager is initialized.
    if (TAO_Singleton_Manager::instance ()->init () == -1)
      {
        return CORBA::ORB::_nil ();
      }

    TAO::ORB::init_orb_globals (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (CORBA::ORB::_nil ());
  }

  // Make sure the following is done after the global ORB
  // initialization since we need to have exceptions initialized.

  // It doesn't make sense for argc to be zero and argv to be
  // non-empty/zero, or for argc to be greater than zero and argv be
  // zero.
  const size_t argv0_len =
    (command_line.get_TCHAR_argv ()
     ? (*command_line.get_TCHAR_argv ()
        ? ACE_OS::strlen (*command_line.get_TCHAR_argv ())
        : 0)
     : 0);

  if ((command_line.get_argc () == 0 && argv0_len != 0)
      || (command_line.get_argc () != 0
          && (command_line.get_TCHAR_argv () == 0
              || command_line.get_TCHAR_argv ()[0] == 0)))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        CORBA::ORB::_nil ());
    }

  if (orbid_string.length () == 0)
    {
      ACE_Arg_Shifter arg_shifter (command_line.get_argc (),
                                   command_line.get_TCHAR_argv ());

      while (arg_shifter.is_anything_left ())
        {
          const ACE_TCHAR *current_arg = arg_shifter.get_current ();

          static const ACE_TCHAR orbid_opt[] = ACE_TEXT ("-ORBid");
          size_t orbid_len = ACE_OS::strlen (orbid_opt);
          if (ACE_OS::strcasecmp (current_arg,
                                  orbid_opt) == 0)
            {
              arg_shifter.consume_arg ();
              if (arg_shifter.is_parameter_next ())
                {
                  orbid_string =
                    ACE_TEXT_ALWAYS_CHAR (arg_shifter.get_current ());
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strncasecmp (current_arg, orbid_opt,
                                        orbid_len) == 0)
            {
              arg_shifter.consume_arg ();
              // The rest of the argument is the ORB id...
              // but we should skip an optional space...
              if (current_arg[orbid_len] == ' ')
                orbid_string =
                  ACE_TEXT_ALWAYS_CHAR (current_arg + orbid_len + 1);
              else
                orbid_string = ACE_TEXT_ALWAYS_CHAR (current_arg + orbid_len);
            }
          else
            arg_shifter.ignore_arg ();
        }
    }

  // Get ORB Core
  TAO_ORB_Core_Auto_Ptr oc (
    TAO::ORB_Table::instance ()->find (orbid_string.c_str ()));

  // The ORB was already initialized.  Just return that one.
  if (oc.get () != 0)
    {
      ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                                guard,
                                *ACE_Static_Object_Lock::instance (),
                                CORBA::ORB::_nil ()));

      if (oc->has_shutdown ())
        {
          // As defined by the CORBA 2.3 specification, throw a
          // CORBA::BAD_INV_ORDER exception with minor code 4 if the
          // ORB has shutdown by the time an ORB function is
          // called.

          // @@ Does the BAD_INV_ORDER exception apply here?
          //       -Ossama

          ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 4,
                                                  CORBA::COMPLETED_NO),
                            CORBA::ORB::_nil ());
        }

      return CORBA::ORB::_duplicate (oc->orb ());
    }
  else
    {
      // An ORB corresponding to the desired ORBid doesn't exist so create
      // a new one.
      TAO_ORB_Core * tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        TAO_ORB_Core (orbid_string.c_str ()),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (0,
                                                                   ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::ORB::_nil ());

      // The ORB table increases the reference count on the ORB Core
      // so do not release it here.  Allow the TAO_ORB_Core_Auto_Ptr
      // do decrease the reference on the ORB Core when it goes out of
      // scope.
      oc.reset (tmp);
    }

  // Initialize the Service Configurator.  This must occur before the
  // ORBInitializer::pre_init() method is invoked on each registered
  // ORB initializer.
  int result = TAO::ORB::open_services (command_line.get_argc (),
                                        command_line.get_TCHAR_argv ());

  // Check for errors returned from <TAO_Internal::open_services>.
  if (result != 0 && errno != ENOENT)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("Unable to initialize the ")
                  ACE_TEXT ("Service Configurator")));
      ACE_THROW_RETURN (CORBA::INITIALIZE (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        CORBA::ORB::_nil ());
    }

  // Run the registered ORB initializers, and initialize the ORB_Core.
  TAO_ORBInitInfo * orb_init_info_temp = 0;
  ACE_NEW_THROW_EX (orb_init_info_temp,
                    TAO_ORBInitInfo (oc.get (),
                                     command_line.get_argc(),
                                     command_line.get_ASCII_argv()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  // This ORBInitInfo instance is only valid for the duration of this
  // ORB's initialization.
  PortableInterceptor::ORBInitInfo_var orb_init_info =
    orb_init_info_temp;

  // Call the ORBInitializer::pre_init() on each registered ORB
  // initializer.
  TAO::ORBInitializer_Registry::instance ()->pre_init (orb_init_info.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  // Initialize the ORB Core instance.
  result = oc->init (command_line.get_argc(),
                     command_line.get_ASCII_argv()
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

  // Check for errors and return nil pseudo-reference on error.
  if (result == -1)
      ACE_THROW_RETURN (CORBA::BAD_PARAM (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        CORBA::ORB::_nil ());

  // Call the ORBInitializer::post_init() on each registered ORB
  // initializer.
  TAO::ORBInitializer_Registry::instance ()->post_init (orb_init_info.in ()
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ORB::_nil ());

#if TAO_HAS_INTERCEPTORS == 1
  oc->pi_current ()->initialize (orb_init_info_temp->slot_count ());
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  // Invalidate the ORBInitInfo instance to prevent future
  // modifications to the ORB.  This behavior complies with the
  // PortableInterceptor specification.
  orb_init_info_temp->invalidate ();

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) created new ORB <%s>\n"),
                  ACE_TEXT_CHAR_TO_TCHAR (orbid_string.c_str ())));
    }

  // Before returning remember to store the ORB into the table...
  if (TAO::ORB_Table::instance ()->bind (orbid_string.c_str (),
                                         oc.get ()) != 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (0,
                                       CORBA::COMPLETED_NO),
                      CORBA::ORB::_nil ());

  // Return a duplicate since the ORB_Core should release the last
  // reference to the ORB.
  return CORBA::ORB::_duplicate (oc->orb ());
}

// ****************************************************************

// Object reference stringification.
char *
CORBA::ORB::object_to_string (CORBA::Object_ptr obj
                              ACE_ENV_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (!CORBA::is_nil (obj) && obj->_is_local ())
    // @@ The CCM spec says one minor code, and the CORBA spec says
    //    another.  Which is the correct one?
    ACE_THROW_RETURN (CORBA::MARSHAL (CORBA::OMGVMCID | 4,
                                      CORBA::COMPLETED_NO),
                      0);


  // Application writer controls what kind of objref strings they get,
  // maybe along with other things, by how they initialize the ORB.

  if (use_omg_ior_format_)
    {
      // By default, orbs use IOR strings; these are ugly (and error
      // prone) but specified by CORBA.
      //
      // XXX there should be a simple way to reuse this code in other
      // ORB implementations ...

#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
      char buf [ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
      // Avoid the initialization overhead if not compiling with
      // support for a memory profiler.  There is no need to actually perform
      // initialization otherwise.
      char buf [ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

      TAO_OutputCDR cdr (buf,  sizeof buf,
                         TAO_ENCAP_BYTE_ORDER,
                         this->orb_core_->output_cdr_buffer_allocator (),
                         this->orb_core_->output_cdr_dblock_allocator (),
                         this->orb_core_->output_cdr_msgblock_allocator (),
                         this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                         TAO_DEF_GIOP_MAJOR,
                         TAO_DEF_GIOP_MINOR);

      // There is no translator currently available for stringifying an object
      // reference, since there is no transport with which to choose an NCS/TCS
      // pair.

      // support limited oref ACE_OS::strcmp.
      (void) ACE_OS::memset (buf, 0, sizeof (buf));

      // Marshal the objref into an encapsulation bytestream.
      (void) cdr.write_octet (TAO_ENCAP_BYTE_ORDER);
      if ((cdr << obj) == 0)
        ACE_THROW_RETURN (CORBA::MARSHAL (), 0);

      // Now hexify the encapsulated CDR data into a string, and
      // return that string.

      const size_t total_len = cdr.total_length ();

      char *cp;
      ACE_ALLOCATOR_RETURN (cp,
                            CORBA::string_alloc (
                              sizeof ior_prefix
                              + 2
                              * static_cast<CORBA::ULong> (total_len)),
                            0);

      CORBA::String_var string = cp;

      ACE_OS::strcpy (cp, ior_prefix);
      cp += sizeof (ior_prefix) - 1;

      for (const ACE_Message_Block *i = cdr.begin ();
           i != 0;
           i = i->cont ())
        {
          const char *bytes = i->rd_ptr ();
          size_t len = i->length ();

          while (len--)
            {
              *cp++ = ACE::nibble2hex ((*bytes) >> 4);
              *cp++ = ACE::nibble2hex (*bytes);
              ++bytes;
            }
        }
      // Null terminate the string..
      *cp = 0;

      return string._retn ();
    }
  else
    {
      // It is perfectly valid to marshal a nil object reference.
      // However, it is not possible to convert a nil object reference
      // to a URL IOR, so throw an exception.
      if (CORBA::is_nil (obj) || obj->_stubobj () == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Nil object reference or TAO_Stub ")
                        ACE_TEXT ("pointer is zero when converting\n")
                        ACE_TEXT ("object reference to URL IOR.\n")));

          ACE_THROW_RETURN (CORBA::MARSHAL (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            0);
        }

      TAO_MProfile &mp = obj->_stubobj ()->base_profiles ();

      if (mp.profile_count () == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Cannot stringify given ")
                        ACE_TEXT ("object.  No profiles.\n")));


          ACE_THROW_RETURN (CORBA::MARSHAL (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            0);
        }

      // For now we just use the first profile.
      TAO_Profile *profile = mp.get_profile (0);

      return profile->to_string (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}

// Destringify arbitrary objrefs.  This method is called from
// <resolve_name_service> with an IOR <multicast_to_service>.

CORBA::Object_ptr
CORBA::ORB::string_to_object (const char *str
                              ACE_ENV_ARG_DECL)
{
  // This method should not be called if the ORB has been shutdown.
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Check for NULL pointer
  if (str == 0)
    ACE_THROW_RETURN (CORBA::INV_OBJREF (
                        CORBA::SystemException::_tao_minor_code (
                          0,
                          EINVAL),
                        CORBA::COMPLETED_NO),
                      CORBA::Object::_nil ());

  TAO_IOR_Parser *ior_parser =
    this->orb_core_->parser_registry ()->match_parser (str);

  if (ior_parser != 0)
    {
      return ior_parser->parse_string (str,
                                       this
                                       ACE_ENV_ARG_PARAMETER);
    }


  if (ACE_OS::strncmp (str,
                       ior_prefix,
                       sizeof ior_prefix - 1) == 0)
    return this->ior_string_to_object (str + sizeof ior_prefix - 1
                                       ACE_ENV_ARG_PARAMETER);
  else
    return this->url_ior_string_to_object (str
                                           ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

char *
CORBA::ORB::id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CORBA::string_dup (this->orb_core_->orbid ());
}

// ****************************************************************

CORBA::Policy_ptr
CORBA::ORB::create_policy (CORBA::PolicyType type,
                           const CORBA::Any& val
                           ACE_ENV_ARG_DECL)
{
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Attempt to obtain the policy from the policy factory registry.
  return
    this->orb_core_->policy_factory_registry ()->create_policy (
      type,
      val
      ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr
CORBA::ORB::_create_policy (CORBA::PolicyType type
                            ACE_ENV_ARG_DECL)
{
  this->check_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  // Attempt to obtain the policy from the policy factory registry.
  return
    this->orb_core_->policy_factory_registry ()->_create_policy (
      type
      ACE_ENV_ARG_PARAMETER);
}

// Destringify OMG-specified "IOR" string.
//
// XXX there should be a simple way to reuse this code in other ORB
// implementations ...

CORBA::Object_ptr
CORBA::ORB::ior_string_to_object (const char *str
                                  ACE_ENV_ARG_DECL)
{
  // Unhex the bytes, and make a CDR deencapsulation stream from the
  // resulting data.
  ACE_Message_Block mb (ACE_OS::strlen (str) / 2 + 1
                        + ACE_CDR::MAX_ALIGNMENT + 1);

  ACE_CDR::mb_align (&mb);

  char *buffer = mb.rd_ptr ();
  const char *tmp = str;
  size_t len = 0;

  while (tmp [0] && tmp [1])
    {
      // Some platforms define 'byte' as a macro, solve the problem
      // here.
#undef byte
      unsigned char byte;

      if (!(isxdigit (tmp [0]) && isxdigit (tmp [1])))
        break;

      byte = (u_char) (ACE::hex2byte (tmp [0]) << 4);
      byte |= ACE::hex2byte (tmp [1]);

      buffer [len++] = byte;
      tmp += 2;
    }

  if (tmp [0] && !isspace (tmp [0]))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CORBA::Object::_nil ());
    }

  // Create deencapsulation stream ... then unmarshal objref from that
  // stream.

  int byte_order = *(mb.rd_ptr ());
  mb.rd_ptr (1);
  mb.wr_ptr (len);
  TAO_InputCDR stream (&mb,
                       byte_order,
                       TAO_DEF_GIOP_MAJOR,
                       TAO_DEF_GIOP_MINOR,
                       this->orb_core_);

  CORBA::Object_ptr objref = CORBA::Object::_nil ();
  stream >> objref;
  return objref;
}

// ****************************************************************

// Convert an URL style IOR in an object

CORBA::Object_ptr
CORBA::ORB::url_ior_string_to_object (const char* str
                                      ACE_ENV_ARG_DECL)
{
  TAO_MProfile mprofile;
  // It is safe to declare this on the stack since the contents of
  // mprofile get copied.  No memory is allocated for profile storage
  // here.  The Connector Registry will determine the exact number
  // of profiles and tell the MProfile object to allocate enough memory
  // to hold them all.

  TAO_Connector_Registry *conn_reg =
    this->orb_core_->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  int retv =
    conn_reg->make_mprofile (str,
                             mprofile
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());   // Return nil.

  if (retv != 0)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        CORBA::Object::_nil ());
    }

  // Now make the TAO_Stub.
  TAO_Stub *data = this->orb_core_->create_stub ((char *) 0,
                                                 mprofile
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_Stub_Auto_Ptr safe_objdata (data);

  // Figure out if the servant is collocated.
  CORBA::Object_ptr obj =
    this->orb_core_->create_object (safe_objdata.get ());
  if (CORBA::is_nil (obj))
    return CORBA::Object::_nil ();

  // Transfer ownership to the CORBA::Object
  (void) safe_objdata.release ();

  return obj;
}

// ****************************************************************

ACE_Time_Value *
CORBA::ORB::get_timeout (void)
{
  return this->timeout_;
}

void
CORBA::ORB::set_timeout (ACE_Time_Value *timeout)
{
  this->timeout_ = timeout;
}

// *************************************************************
// Valuetype factory operations
// *************************************************************

CORBA::ValueFactory
CORBA::ORB::register_value_factory (const char *repository_id,
                                    CORBA::ValueFactory factory
                                    ACE_ENV_ARG_DECL)
{
  // %! guard, and ACE_Null_Mutex in the map
  // do _add_ref here not in map->rebind

  if (this->orb_core ()->valuetype_adapter () == 0)
    {

     this->orb_core ()->valuetype_adapter () =
       ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
         );

      if (this->orb_core ()->valuetype_adapter () == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }
    }

  int result =
    this->orb_core ()->valuetype_adapter ()->vf_map_rebind (repository_id,
                                                            factory);

  if (result == 0)              // No previous factory found
    {
      return 0;
    }

  if (result == -1)
    {
      // Error on bind.
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);
    }

  return factory;    // previous factory was found
}

void
CORBA::ORB::unregister_value_factory (const char *repository_id
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  if (this->orb_core ()->valuetype_adapter ())
    {
      // Dont care whther it was successful or not!
      (void) this->orb_core ()->valuetype_adapter ()->vf_map_unbind (repository_id);
    }
}

CORBA::ValueFactory
CORBA::ORB::lookup_value_factory (const char *repository_id
                                  ACE_ENV_ARG_DECL)
{
  if (this->orb_core ()->valuetype_adapter () == 0)
    {
      this->orb_core ()->valuetype_adapter () =
        ACE_Dynamic_Service<TAO_Valuetype_Adapter>::instance (
            TAO_ORB_Core::valuetype_adapter_name ()
          );

      if (this->orb_core ()->valuetype_adapter () == 0)
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            0);
        }
    }

  return this->orb_core ()->valuetype_adapter ()->vf_map_find (repository_id);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Pseudo_Var_T<CORBA::ORB>;
template class TAO_Pseudo_Out_T<CORBA::ORB, CORBA::ORB_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Pseudo_Var_T<CORBA::ORB>
#pragma instantiate TAO_Pseudo_Out_T<CORBA::ORB, CORBA::ORB_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
