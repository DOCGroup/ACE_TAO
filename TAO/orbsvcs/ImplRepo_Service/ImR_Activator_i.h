// -*- C++ -*-
//=============================================================================
/**
*  @file   ImR_Activator_i.h
*
*  $Id$
*
*  @author Priyanka Gontla <gontla_p@ociweb.com>
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#ifndef IMR_ACTIVATOR_I_H
#define IMR_ACTIVATOR_I_H

#include "activator_export.h"

#include "ImR_ActivatorS.h"
#include "ImR_LocatorC.h"

#include "ace/Process_Manager.h"
#include "ace/Vector_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Bound_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Options;

/**
* @class ImR_Activator_i
*
* @brief IMR Activator Interface.
*
* This class provides the interface for the various activities
* that can be done by the ImR_Activator.
*
*/
class Activator_Export ImR_Activator_i : public POA_ImplementationRepository::AMH_Activator
{
  typedef ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var> RHList;
  typedef ACE_Strong_Bound_Ptr<RHList, ACE_Null_Mutex> RHListPtr;
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    RHListPtr,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> ServerMap;

public:
  ImR_Activator_i (void);

 void start_server (
   ImplementationRepository::AMH_ActivatorResponseHandler_ptr rh,
   const char* name,
   const char* cmdline,
   const char* dir,
   const ImplementationRepository::EnvironmentList & env ACE_ENV_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException));

  void server_status_changed(
    ImplementationRepository::AMH_ActivatorResponseHandler_ptr rh,
    const char* name ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Initialize the Server state - parsing arguments and waiting.
  int init (Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Cleans up any state created by init*.
  int fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Runs the orb.
  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:

  int init_with_orb (CORBA::ORB_ptr orb, const Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void unblock_all_servers();
  void unblock_server(RHListPtr server);

private:
  ACE_Process_Manager process_mgr_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  /// IMR Locator Object
  ImplementationRepository::Locator_var locator_;

  /// We're given a token when registering with the locator, which
  /// we must use when unregistering.
  CORBA::Long registration_token_;

  CORBA::ORB_var orb_;

  unsigned int debug_;

  ACE_CString name_;

  ServerMap waiting_servers_;
};

#endif /* IMR_ACTIVATOR_I_H */
