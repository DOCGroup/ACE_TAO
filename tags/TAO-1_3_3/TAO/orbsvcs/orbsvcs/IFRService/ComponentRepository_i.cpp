/* -*- C++ -*- */
// $Id$

#include "ComponentRepository_i.h"
#include "ComponentDef_i.h"
#include "HomeDef_i.h"
#include "EventDef_i.h"
#include "FinderDef_i.h"
#include "FactoryDef_i.h"
#include "EmitsDef_i.h"
#include "PublishesDef_i.h"
#include "ConsumesDef_i.h"
#include "ProvidesDef_i.h"
#include "UsesDef_i.h"

ACE_RCSID (IFRService, 
           ComponentRepository_i, 
           "$Id$")

TAO_ComponentRepository_i::TAO_ComponentRepository_i (
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    ACE_Configuration *config
  )
  : TAO_IRObject_i (0),
    TAO_Container_i (0),
    TAO_Repository_i (orb, 
                      poa, 
                      config),
    TAO_ComponentContainer_i (0)
{
}

TAO_ComponentRepository_i::~TAO_ComponentRepository_i (void)
{
}

int
TAO_ComponentRepository_i::create_servants_and_poas (
    ACE_ENV_SINGLE_ARG_DECL
  )
{
  int status = 
    this->TAO_Repository_i::create_servants_and_poas (
        ACE_ENV_SINGLE_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  if (status != 0)
    {
      return -1;
    }

  CORBA::PolicyList policies (5);
  policies.length (5);

  // ID Assignment Policy.
  policies[0] =
    this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID 
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Lifespan Policy.
  policies[1] =
    this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT 
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1); 

  // Request Processing Policy.
  policies[2] =
    this->root_poa_->create_request_processing_policy (
        PortableServer::USE_DEFAULT_SERVANT 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  // Servant Retention Policy.
  policies[3] =
    this->root_poa_->create_servant_retention_policy (
        PortableServer::RETAIN 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  // Id Uniqueness Policy.
  policies[4] =
    this->root_poa_->create_id_uniqueness_policy (
        PortableServer::MULTIPLE_ID 
        ACE_ENV_ARG_PARAMETER
      );
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

#define GEN_IR_OBJECT(name) \
  this-> name ## _poa_ = \
    this->root_poa_->create_POA (#name "_poa", \
                                 poa_manager.in (), \
                                 policies \
                                 ACE_ENV_ARG_PARAMETER); \
  ACE_CHECK_RETURN (-1); \
  TAO_ ## name ## _i * name ## _impl = 0; \
  ACE_NEW_RETURN (name ## _impl, \
                  TAO_ ## name ## _i (this), \
                  -1); \
  ACE_NEW_RETURN (this-> name ## _servant_, \
                  POA_CORBA::ComponentIR:: name ## _tie<TAO_ ## name ## _i> ( \
                      name ## _impl, \
                      this-> name ## _poa_.in (), \
                      1 \
                    ), \
                  -1); \
  PortableServer::ServantBase_var name ## _safety ( \
      this-> name ## _servant_ \
    ); \
  this-> name ## _poa_->set_servant (this-> name ## _servant_ \
                                     ACE_ENV_ARG_PARAMETER); \
  ACE_CHECK_RETURN (-1);

  CONCRETE_IR_OBJECT_TYPES

#undef GEN_IR_OBJECT
#undef CONCRETE_IR_OBJECT_TYPES

  CORBA::ULong length = policies.length ();

  for (CORBA::ULong i = 0; i < length; ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

TAO_IDLType_i *
TAO_ComponentRepository_i::select_idltype (
    CORBA::DefinitionKind def_kind
  ) const
{
  switch (def_kind)
  {
    case CORBA::dk_Component:
      return this->ComponentDef_servant_->_tied_object ();
    case CORBA::dk_Home:
      return this->HomeDef_servant_->_tied_object ();
    default:
      return this->TAO_Repository_i::select_idltype (def_kind);
  }
}

TAO_Container_i *
TAO_ComponentRepository_i::select_container (
    CORBA::DefinitionKind def_kind
  ) const
{
  switch (def_kind)
  {
    case CORBA::dk_Component:
      return this->ComponentDef_servant_->_tied_object ();
    case CORBA::dk_Home:
      return this->HomeDef_servant_->_tied_object ();
    default:
      return this->TAO_Repository_i::select_container (def_kind);
  }
}

TAO_Contained_i *
TAO_ComponentRepository_i::select_contained (
    CORBA::DefinitionKind def_kind
  ) const
{
  switch (def_kind)
  {
    case CORBA::dk_Component:
      return this->ComponentDef_servant_->_tied_object ();
    case CORBA::dk_Home:
      return this->HomeDef_servant_->_tied_object ();
    case CORBA::dk_Finder:
      return this->FinderDef_servant_->_tied_object ();
    case CORBA::dk_Factory:
      return this->FactoryDef_servant_->_tied_object ();
    case CORBA::dk_Event:
      return this->EventDef_servant_->_tied_object ();
    case CORBA::dk_Emits:
      return this->EmitsDef_servant_->_tied_object ();
    case CORBA::dk_Publishes:
      return this->PublishesDef_servant_->_tied_object ();
    case CORBA::dk_Consumes:
      return this->ConsumesDef_servant_->_tied_object ();
    case CORBA::dk_Provides:
      return this->ProvidesDef_servant_->_tied_object ();
    case CORBA::dk_Uses:
      return this->UsesDef_servant_->_tied_object ();
    default:
      return this->TAO_Repository_i::select_contained (def_kind);
  }
}

PortableServer::POA_ptr
TAO_ComponentRepository_i::select_poa (
    CORBA::DefinitionKind def_kind
  ) const
{
  switch (def_kind)
  {
    case CORBA::dk_Component:
      return this->ComponentDef_poa_.in ();
    case CORBA::dk_Home:
      return this->HomeDef_poa_.in ();
    case CORBA::dk_Finder:
      return this->FinderDef_poa_.in ();
    case CORBA::dk_Factory:
      return this->FactoryDef_poa_.in ();
    case CORBA::dk_Event:
      return this->EventDef_poa_.in ();
    case CORBA::dk_Emits:
      return this->EmitsDef_poa_.in ();
    case CORBA::dk_Publishes:
      return this->PublishesDef_poa_.in ();
    case CORBA::dk_Consumes:
      return this->ConsumesDef_poa_.in ();
    case CORBA::dk_Provides:
      return this->ProvidesDef_poa_.in ();
    case CORBA::dk_Uses:
      return this->UsesDef_poa_.in ();
    default:
      return this->TAO_Repository_i::select_poa (def_kind);
  }
}


