// -*- C++ -*-
//
// $Id$

#include "CEC_TypedEventChannel.h"
#include "CEC_Dispatching.h"
#include "CEC_TypedConsumerAdmin.h"
#include "CEC_TypedSupplierAdmin.h"
#include "CEC_ConsumerControl.h"
#include "CEC_SupplierControl.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_TypedEventChannel.i"
#endif /* __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Implementation skeleton constructor
TAO_CEC_TypedEventChannel::
TAO_CEC_TypedEventChannel (const TAO_CEC_TypedEventChannel_Attributes& attr,
                           TAO_CEC_Factory* factory,
                           int own_factory)
  : typed_supplier_poa_ (PortableServer::POA::_duplicate (attr.typed_supplier_poa)),
    typed_consumer_poa_ (PortableServer::POA::_duplicate (attr.typed_consumer_poa)),
    orb_ (CORBA::ORB::_duplicate (attr.orb)),
    interface_repository_ (CORBA::Repository::_duplicate (attr.interface_repository)),
    factory_ (factory),
    own_factory_ (own_factory),
    consumer_reconnect_ (attr.consumer_reconnect),
    supplier_reconnect_ (attr.supplier_reconnect),
    disconnect_callbacks_ (attr.disconnect_callbacks),
    destroy_on_shutdown_ (attr.destroy_on_shutdown),
    destroyed_ (0)
{
  if (this->factory_ == 0)
    {
      this->factory_ =
        ACE_Dynamic_Service<TAO_CEC_Factory>::instance ("CEC_Factory");
      this->own_factory_ = 0;
      ACE_ASSERT (this->factory_ != 0);
    }

  this->dispatching_ =
    this->factory_->create_dispatching (this);
  this->typed_consumer_admin_ =
    this->factory_->create_consumer_admin (this);
  this->typed_supplier_admin_ =
    this->factory_->create_supplier_admin (this);
  this->consumer_control_ =
    this->factory_->create_consumer_control (this);
  this->supplier_control_ =
    this->factory_->create_supplier_control (this);
}

// Implementation skeleton destructor
TAO_CEC_TypedEventChannel::~TAO_CEC_TypedEventChannel (void)
{
  this->clear_ifr_cache ();
  this->interface_description_.close ();

  this->factory_->destroy_dispatching (this->dispatching_);
  this->dispatching_ = 0;

  this->factory_->destroy_consumer_admin (this->typed_consumer_admin_);
  this->typed_consumer_admin_ = 0;
  this->factory_->destroy_supplier_admin (this->typed_supplier_admin_);
  this->typed_supplier_admin_ = 0;

  if (this->own_factory_)
    delete this->factory_;
}

void
TAO_CEC_TypedEventChannel::activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->dispatching_->activate ();
  this->consumer_control_->activate ();
  this->supplier_control_->activate ();
}

void
TAO_CEC_TypedEventChannel::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->dispatching_->shutdown ();
  this->supplier_control_->shutdown ();
  this->consumer_control_->shutdown ();

  PortableServer::POA_var typed_consumer_poa =
    this->typed_consumer_admin_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var typed_consumer_id =
    typed_consumer_poa->servant_to_id (this->typed_consumer_admin_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  typed_consumer_poa->deactivate_object (typed_consumer_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var typed_supplier_poa =
    this->typed_supplier_admin_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var typed_supplier_id =
    typed_supplier_poa->servant_to_id (this->typed_supplier_admin_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  typed_supplier_poa->deactivate_object (typed_supplier_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->typed_supplier_admin_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->typed_consumer_admin_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (destroy_on_shutdown_)
    {
      // Deactivate the Typed EC
      PortableServer::POA_var t_poa =
        this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      PortableServer::ObjectId_var t_id =
        t_poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      t_poa->deactivate_object (t_id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->orb_->shutdown(0);
    }
}

void
TAO_CEC_TypedEventChannel::connected (TAO_CEC_TypedProxyPushConsumer* consumer
                                      ACE_ENV_ARG_DECL)
{
  this->typed_supplier_admin_->connected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedEventChannel::reconnected (TAO_CEC_TypedProxyPushConsumer* consumer
                                        ACE_ENV_ARG_DECL)
{
  this->typed_supplier_admin_->reconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedEventChannel::disconnected (TAO_CEC_TypedProxyPushConsumer* consumer
                                         ACE_ENV_ARG_DECL)
{
  this->typed_supplier_admin_->disconnected (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedEventChannel::connected (TAO_CEC_ProxyPushSupplier* supplier
                                      ACE_ENV_ARG_DECL)
{
  this->typed_consumer_admin_->connected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedEventChannel::reconnected (TAO_CEC_ProxyPushSupplier* supplier
                                        ACE_ENV_ARG_DECL)
{
  this->typed_consumer_admin_->reconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_CEC_TypedEventChannel::disconnected (TAO_CEC_ProxyPushSupplier* supplier
                                         ACE_ENV_ARG_DECL)
{
  this->typed_consumer_admin_->disconnected (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Find from the ifr cache the operation and return the parameter array pointer.
TAO_CEC_Operation_Params *
TAO_CEC_TypedEventChannel::find_from_ifr_cache (const char *operation)
{
  TAO_CEC_Operation_Params *found = 0;

  this->interface_description_.find (operation, found);

  return found;
}

// Insert the operation and its parameters into the ifr cache.
int
TAO_CEC_TypedEventChannel::insert_into_ifr_cache (const char *operation_,
                                                  TAO_CEC_Operation_Params *parameters_)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (operation_ == 0 || parameters_ == 0)
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var operation = CORBA::string_dup (operation_);

  int result = this->interface_description_.bind (operation.in (), parameters_);

  if (result == 0)
    {
      // Transfer ownership to the Object InterfaceDescription map.
      (void) operation._retn ();
    }

  return result;
}

// Clear the ifr cache, freeing up all its contents.
int
TAO_CEC_TypedEventChannel::clear_ifr_cache (void)
{
  for (Iterator i = this->interface_description_.begin ();
       i != this->interface_description_.end ();
       ++i)
    {
      if (TAO_debug_level >= 10)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("***** Destroying operation %s from ifr cache *****\n"),
                      const_cast<char *> ((*i).ext_id_)));
        }

      // Deallocate the operation
      CORBA::string_free (const_cast<char *> ((*i).ext_id_));

      // Destroy the parameter
      delete ((*i).int_id_);
    }

  int result = this->interface_description_.unbind_all ();

  return result;
}


// The function performs a lookup_id of the passed interface in the IFR,
// and then obtains the FullInterfaceDescription.
// The base interfaces for the interface are stored on this class.
// All the operations and their parameters are then inserted in the ifr cache.
// Function returns 0 if successful or -1 on a failure.
int
TAO_CEC_TypedEventChannel::cache_interface_description (const char *interface_
                                                        ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      // Lookup the Interface Name in the IFR
      CORBA::Contained_var contained =
        this->interface_repository_->lookup_id (interface_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the interface
      CORBA::InterfaceDef_var interface =
        CORBA::InterfaceDef::_narrow (contained.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (interface.in () ))
        {
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** CORBA::InterfaceDef::_narrow failed for interface %s *****\n"),
                          interface_));
            }
          return -1;
        }
      else
        {
          // Obtain the full interface description
          CORBA::InterfaceDef::FullInterfaceDescription_var fid =
            interface->describe_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Obtain the base interfaces
          this->base_interfaces_ = fid->base_interfaces;
          if (TAO_debug_level >= 10)
            {
              for (CORBA::ULong base=0; base<fid->base_interfaces.length(); base++)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("***** Base interface %s found on interface %s *****\n"),
                              static_cast<char const*>(fid->base_interfaces[base]),
                              interface_ ));
                }
            }

          // Obtain the operations
          for (CORBA::ULong oper=0; oper<fid->operations.length(); oper++)
            {
              if (TAO_debug_level >= 10)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("***** Operation %s found on interface %s, num params %d *****\n"),
                              fid->operations[oper].name.in(),
                              interface_,
                              fid->operations[oper].parameters.length() ));
                }

              // Obtain the parameters
              CORBA::ULong num_params = fid->operations[oper].parameters.length();
              TAO_CEC_Operation_Params *oper_params = new TAO_CEC_Operation_Params (num_params);

              for (CORBA::ULong param=0; param<num_params; param++)
                {
                  oper_params->parameters_[param].name_ = fid->operations[oper].parameters[param].name.in();
                  oper_params->parameters_[param].type_ = fid->operations[oper].parameters[param].type;
                  switch (fid->operations[oper].parameters[param].mode)
                    {
                    case CORBA::PARAM_IN:
                      oper_params->parameters_[param].direction_ = CORBA::ARG_IN;
                      break;
                    case CORBA::PARAM_OUT:
                      oper_params->parameters_[param].direction_ = CORBA::ARG_OUT;
                      break;
                    case CORBA::PARAM_INOUT:
                      oper_params->parameters_[param].direction_ = CORBA::ARG_INOUT;
                      break;
                    }

                  if (TAO_debug_level >= 10)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("***** Parameter %s found on operation %s *****\n"),
                                  oper_params->parameters_[param].name_.in(),
                                  fid->operations[oper].name.in() ));
                    }
                }

              if (TAO_debug_level >= 10)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("***** Adding operation %s with %d parameters to the IFR cache *****\n"),
                              fid->operations[oper].name.in(),
                              oper_params->num_params_ ));
                }

              int result = insert_into_ifr_cache (fid->operations[oper].name.in(), oper_params);
              if (result != 0)
                {
                  if (TAO_debug_level >= 10)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("***** Adding operation to IFR cache failed *****\n")));
                    }
                }
            }
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (sysex, "during TAO_CEC_TypedEventChannel::cache_interface_description");
        }
      return -1;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ACE_ANY_EXCEPTION raised during TAO_CEC_TypedEventChannel::cache_interface_description");
        }
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

// A consumer is attempting to register its uses_interface.
// Note only a single interface can be registered with this version of the EC.
// For users that require more than one interface, start another EC.
// If the passed uses_interface is the same as a registered interface the function returns 0.
// If an attempt is made to register a second interface, this function will return -1
// and the TypedConsumerAdmin will throw CosTypedEventChannelAdmin::NoSuchImplementation.
// If neither a consumer nor a supplier has registered an interface,
// the function calls cache_interface_description and returns 0 if successful.
int
TAO_CEC_TypedEventChannel::consumer_register_uses_interace (const char *uses_interface
                                                            ACE_ENV_ARG_DECL)
{
  // Check if a consumer has already registered an interface with the typed EC
  if (this->uses_interface_.length() > 0)
    {
      // Check if the registered uses_interface_ == the new uses_interface
      if (this->uses_interface_ == ACE_CString (uses_interface))
        {
          return 0;
        }
      else
        {
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** different uses_interface_ already registered *****\n")));
            }
          return -1;
        }
    }

  // Check if a supplier has already registered an inerface with the typed EC
  if (this->supported_interface_.length() > 0)
    {
      // Check if the registered supported_interface_ == the new uses_interface
      if (this->supported_interface_ == ACE_CString (uses_interface))
        {
          this->uses_interface_ = uses_interface;
          return 0;
        }
      else
        {
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** different supported_interface_ already registered *****\n")));
            }
          return -1;
        }
    }
  else
    {
      // Neither a consumer nor a supplier has connected yet
      int result = cache_interface_description (uses_interface ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (result == 0)
        {
          this->uses_interface_ = uses_interface;
        }
      return result;
    }
  // Should not get here!
  return -1;
}

// A supplier is attempting to register its supported_interface.
// Note only a single interface can be registered with this version of the EC.
// For users that require more than one interface, start another EC.
// If the passed supported_interface is the same as a registered interface the function returns 0.
// If an attempt is made to register a second interface, this function will return -1
// and the TypedSupplierAdmin will throw CosTypedEventChannelAdmin::InterfaceNotSupported.
// If neither a consumer nor a supplier has registered an interface,
// the function calls cache_interface_description and returns 0 if successful.
int
TAO_CEC_TypedEventChannel::supplier_register_supported_interface (const char *supported_interface
                                                                  ACE_ENV_ARG_DECL)
{
  // Check if a supplier has already registered an interface with the typed EC
  if (this->supported_interface_.length() > 0)
    {
      // Check if the registered interface == the new supported_interface
      if (this->supported_interface_ == ACE_CString (supported_interface))
        {
          return 0;
        }
      else
        {
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** different supported_interface_ already registered *****\n")));
            }
          return -1;
        }
    }

  // Check if a consumer has already registered an inerface with the typed EC
  if (this->uses_interface_.length() > 0)
    {
      // Check if the registered uses_interface_ == the new supported_interface
      if (this->uses_interface_ == ACE_CString (supported_interface))
        {
          this->supported_interface_ = supported_interface;
          return 0;
        }
      else
        {
          if (TAO_debug_level >= 10)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** different uses_interface_ already registered *****\n")));
            }
          return -1;
        }
    }
  else
    {
      // Neither a consumer nor a supplier has connected yet
      int result = cache_interface_description (supported_interface ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (result == 0)
        {
          this->supported_interface_ = supported_interface;
        }
      return result;
    }
  // Should not get here!
  return -1;
}

// Function creates a NVList and populates it from the parameter information.
void
TAO_CEC_TypedEventChannel::create_operation_list (TAO_CEC_Operation_Params *oper_params,
                                                  CORBA::NVList_out new_list
                                                  ACE_ENV_ARG_DECL)
{
  this->orb_->create_list (0, new_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong param=0; param<oper_params->num_params_; param++)
    {

      CORBA::Any any_1;
      any_1._tao_set_typecode(oper_params->parameters_[param].type_.in ());

      new_list->add_value (oper_params->parameters_[param].name_. in (),
                           any_1,
                           oper_params->parameters_[param].direction_
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

// Function creates an empty NVList.
void
TAO_CEC_TypedEventChannel::create_list (CORBA::Long count,
                                        CORBA::NVList_out new_list
                                        ACE_ENV_ARG_DECL)
{
  this->orb_->create_list (count, new_list ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// The CosTypedEventChannelAdmin::TypedEventChannel methods...
CosTypedEventChannelAdmin::TypedConsumerAdmin_ptr
TAO_CEC_TypedEventChannel::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->typed_consumer_admin_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosTypedEventChannelAdmin::TypedSupplierAdmin_ptr
TAO_CEC_TypedEventChannel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->typed_supplier_admin_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_CEC_TypedEventChannel::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!destroyed_)
    {
      destroyed_ = 1;
      this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
