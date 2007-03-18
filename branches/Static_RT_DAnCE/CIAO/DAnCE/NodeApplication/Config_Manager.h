// -*- C++ -*-

//=============================================================================
/**
 *  @file    Config_Manager.h
 *
 *  $Id$
 *
 */
//=============================================================================


#ifndef CIAO_CONFIG_MANAGER_H
#define CIAO_CONFIG_MANAGER_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"

namespace CIAO
{
  /**
   */
  class Config_Manager
  {
  public:
    virtual ~Config_Manager (void) {}

    virtual int pre_orb_initialize (void) = 0;

    virtual int post_orb_initialize (CORBA::ORB_ptr o) = 0;

    virtual void init_resources (const CIAO::DAnCE::ServerResource &info) = 0;

    virtual void init (CORBA::ORB_ptr orb) = 0;

    virtual CORBA::PolicyList *find_policies_by_name (const char *name)= 0;

    virtual bool policy_exists (const char *name) = 0;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_MANAGER_H */

