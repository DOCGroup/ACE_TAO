/* -*- C++ -*- */

// $Id$
// =================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableGroup_Loader.cpp
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =================================================================

#include "PortableGroup_Loader.h"
#include "miop.h"
#include "tao/ORB_Core.h"

ACE_RCSID(PortableGroup, PortableGroup_Loader, "$Id$")

TAO_PortableGroup_Loader::TAO_PortableGroup_Loader (void) :
  request_dispatcher_ (this)
{
}

TAO_Portable_Group_Map &
TAO_PortableGroup_Loader::group_map (void)
{
  return this->group_map_;
}

void
TAO_PortableGroup_Loader::activate (CORBA::ORB_ptr orb,
                                    int argc,
                                    char *argv[],
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  char *domain_id = "DefaultGroupDomain";
  int curarg = 0;

  for (curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-MIOPDomainId") == 0)
      {

        curarg++;
        if (curarg < argc)
          {
            domain_id = argv[curarg];
          }
      }

  // Create the MIOP factory object.
  CORBA::Object_ptr tmp_miop;
  ACE_NEW_THROW_EX (tmp_miop,
                    TAO_MOGF_impl (orb->orb_core (),
                                   domain_id),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  this->miop_ = tmp_miop;

  // Create the PortableGroup factory object...

  // Create the hooks for the POA.
  ACE_NEW_THROW_EX (this->poa_hooks_,
                    TAO_POA_Hooks (this),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  // Set a new request dispatcher in the ORB.
  orb->orb_core ()->request_dispatcher (&this->request_dispatcher_);

}


CORBA::Object_ptr
TAO_PortableGroup_Loader::miop (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return CORBA::Object::_duplicate (this->miop_.in ());
}

TAO_POA_PortableGroup_Hooks *
TAO_PortableGroup_Loader::poa_hooks (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->poa_hooks_;
}



int
TAO_PortableGroup_Loader::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_PortableGroup_Loader);

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_PortableGroup_Loader,
                       ACE_TEXT ("PortableGroup_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PortableGroup_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_PortableGroup_Loader)
