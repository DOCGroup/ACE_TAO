// -*- C++ -*-
//
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/CosEvent/CEC_DynamicImplementation.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include "tao/debug.h"
#include "orbsvcs/CosEvent/CEC_TypedEvent.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_DynamicImplementation.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Destructor
TAO_CEC_DynamicImplementationServer::~TAO_CEC_DynamicImplementationServer (void)
{
}

// The DSI invoke request
void
TAO_CEC_DynamicImplementationServer::invoke (CORBA::ServerRequest_ptr request)
{
  // Trap the _is_a request
  if (ACE_OS::strcmp ("_is_a", request->operation () ) == 0)
    {
      this->is_a (request);
    }
  else
    {
      CORBA::NVList_ptr list;

      // Get the operation paramter information from the IFR cache.
      TAO_CEC_Operation_Params *oper_params =
        this->typed_event_channel_->find_from_ifr_cache (request->operation () );

      if (oper_params == 0)
        {
          if (TAO_debug_level >= 10)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** Operation not found in IFR cache *****\n")));
            }

          this->typed_event_channel_->create_list (0, list);
        }
      else
        {
          // Populate the NVList from the parameter information.
          this->typed_event_channel_->create_operation_list (oper_params, list);

          // Get the operation arguments. This ahould demarshal correctly.
          request->arguments (list);

          // Populate the TypedEvent with the list and operation name.
          TAO_CEC_TypedEvent typed_event (list, request->operation () );

          // Pass the TypedEvent to the TypedProxyPushConsumer
          this->typed_pp_consumer_->invoke (typed_event);
        }
    }
}

CORBA::RepositoryId
TAO_CEC_DynamicImplementationServer::_primary_interface (const PortableServer::ObjectId &,
                                                         PortableServer::POA_ptr)
{
  return CORBA::string_dup (repository_id_);
}

PortableServer::POA_ptr
TAO_CEC_DynamicImplementationServer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
TAO_CEC_DynamicImplementationServer::is_a (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr list;

  this->typed_event_channel_->create_list (0, list);

  CORBA::Any any_1;
  any_1._tao_set_typecode(CORBA::_tc_string);

  list->add_value ("value",
                   any_1,
                   CORBA::ARG_IN);

  request->arguments (list);

  CORBA::NamedValue_ptr nv = list->item (0);

  CORBA::Any_ptr ap = nv->value ();
  const char *value = 0;
  *ap >>= value;

  if (TAO_debug_level >= 10)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("***** TAO_CEC_DynamicImplementationServer::is_a called with value %s *****\n"),
                  value));
    }

  const char *object_id =
    CORBA::_tc_Object->id ();

  if (TAO_debug_level >= 10)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("***** is_a using Server's RepositoryId %s *****\n"),
                  this->repository_id_));
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("***** is_a using base interface %s *****\n"),
                  object_id));
    }

  CORBA::Boolean result = 0;
  if (ACE_OS::strcmp (value, this->repository_id_) == 0
      || ACE_OS::strcmp (value, object_id) == 0)
    {
      result = 1;
    }
  else
    {
      CORBA::ULong num = this->typed_event_channel_->number_of_base_interfaces ();
      for (CORBA::ULong base=0; base<num; base++)
        {
          if (TAO_debug_level >= 10)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** is_a using base interface %s *****\n"),
                          this->typed_event_channel_->base_interfaces (base) ));
            }

          if (ACE_OS::strcmp (value, this->typed_event_channel_->base_interfaces (base) ) == 0)
            {
              result = 1;
            }
        }
    }

  if (TAO_debug_level >= 10)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("***** is_a returning %d *****\n"),
                  result));
    }

  CORBA::Any result_any;
  CORBA::Any::from_boolean from_boolean (result);
  result_any <<= from_boolean;

  request->set_result (result_any);
}

TAO_END_VERSIONED_NAMESPACE_DECL
