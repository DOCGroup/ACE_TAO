/* -*- C++ -*- */
// $Id$

#include "IRObject_i.h"
#include "Repository_i.h"
#include "IDLType_i.h"
#include "Contained_i.h"
#include "IFR_Service_Utils.h"

#include "tao/PortableServer/POA.h"
#include "tao/ORB_Core.h"

ACE_RCSID (IFR_Service, 
           IRObject_i, 
           "$Id$")

TAO_IRObject_i::TAO_IRObject_i (TAO_Repository_i *repo)
  : repo_ (repo),
    section_key_ (ACE_Configuration_Section_Key ())
{
}

TAO_IRObject_i::~TAO_IRObject_i (void)
{
}

void
TAO_IRObject_i::section_key (ACE_Configuration_Section_Key &key)
{
  this->section_key_ = key;
}

void
TAO_IRObject_i::update_key (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_POA_Current_Impl *pc_impl =
    ACE_static_cast (TAO_POA_Current_Impl *,
                     TAO_TSS_RESOURCES::instance ()->poa_current_impl_);

  PortableServer::ObjectId object_id;
  int status = TAO_POA::parse_ir_object_key (pc_impl->object_key (),
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
                    0);

  status = 
    this->repo_->config ()->expand_path (this->repo_->root_key (),
                                         path,
                                         this->section_key_,
                                         0);

  if (status != 0)
    {
      // If we're here, destroy() has been called on us.
      ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
    }
}

