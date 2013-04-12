// $Id$

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/Repository_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/Contained_i.h"
#include "orbsvcs/IFRService/IFR_Service_Utils.h"

#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/TSS_Resources.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IRObject_i::TAO_IRObject_i (TAO_Repository_i *repo)
  : repo_ (repo)
{
}

TAO_IRObject_i::~TAO_IRObject_i (void)
{
}

CORBA::DefinitionKind
TAO_IRObject_i::def_kind (
  )
{
  // Will always be overridden by concrete classes.
  return CORBA::dk_none;
}

void
TAO_IRObject_i::section_key (ACE_Configuration_Section_Key &key)
{
  this->section_key_ = key;
}

void
TAO_IRObject_i::update_key (void)
{
  TAO::Portable_Server::POA_Current_Impl *pc_impl =
    static_cast <TAO::Portable_Server::POA_Current_Impl *>
                     (TAO_TSS_Resources::instance ()->poa_current_impl_);

  PortableServer::ObjectId object_id;
  int status = TAO_Root_POA::parse_ir_object_key (pc_impl->object_key (),
                                                  object_id);
  if (status != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "update_key - parse_ir_object_key failed\n"));
      return;
    }

  char *oid_string = TAO_IFR_Service_Utils::oid_to_string (object_id);

  if (oid_string[0U] == '\0')
    {
      this->section_key_ = this->repo_->root_key ();
      return;
    }

  ACE_TString path (oid_string,
                    0,
                    false);

  status =
    this->repo_->config ()->expand_path (this->repo_->root_key (),
                                         path,
                                         this->section_key_,
                                         0);

  if (status != 0)
    {
      // If we're here, destroy() has been called on us.
      throw CORBA::OBJECT_NOT_EXIST ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
