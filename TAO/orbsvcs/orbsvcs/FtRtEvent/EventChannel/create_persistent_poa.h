// -*- C++ -*-

//=============================================================================
/**
 *  @file   create_persistent_poa.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef CREATE_PERSISTENT_POA_H
#define CREATE_PERSISTENT_POA_H
#include "tao/PortableServer/PortableServerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

PortableServer::POA_var create_persistent_poa(PortableServer::POA_var root_poa,
                                              PortableServer::POAManager_var mgr,
                                              const char* name,
                                              CORBA::PolicyList& policy_list
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);
#endif
