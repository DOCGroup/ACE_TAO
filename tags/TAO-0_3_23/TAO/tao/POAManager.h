// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    POAManager.h
//
// = DESCRIPTION
//     POAManager
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#ifndef TAO_POAMANAGER_H
#define TAO_POAMANAGER_H

#include "tao/POAS.h"
// for POA skeleton.

#include "tao/poa_macros.h"

class TAO_POA;
// Forward decl.

class TAO_Export TAO_POA_Manager : public POA_PortableServer::POAManager
{
  friend class TAO_POA;

public:

  void activate (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void hold_requests (CORBA::Boolean wait_for_completion,
                      CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void discard_requests (CORBA::Boolean wait_for_completion,
                         CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  void deactivate (CORBA::Boolean etherealize_objects,
                   CORBA::Boolean wait_for_completion,
                   CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::POAManager::State get_state (CORBA_Environment &ACE_TRY_ENV = CORBA::default_environment ());

  TAO_POA_Manager (ACE_Lock &lock);

  ~TAO_POA_Manager (void);

protected:

  void activate_i (CORBA_Environment &ACE_TRY_ENV);

#if !defined (TAO_HAS_MINIMUM_CORBA)

  void hold_requests_i (CORBA::Boolean wait_for_completion,
                        CORBA_Environment &ACE_TRY_ENV);

  void discard_requests_i (CORBA::Boolean wait_for_completion,
                           CORBA_Environment &ACE_TRY_ENV);

  void deactivate_i (CORBA::Boolean etherealize_objects,
                     CORBA::Boolean wait_for_completion,
                     CORBA_Environment &ACE_TRY_ENV);

#endif /* TAO_HAS_MINIMUM_CORBA */

  PortableServer::POAManager::State get_state_i ();

  ACE_Lock &lock (void);

  int remove_poa (TAO_POA *poa);

  int register_poa (TAO_POA *poa);

  PortableServer::POAManager::State state_;

  ACE_Lock &lock_;

  typedef ACE_Unbounded_Set<TAO_POA *> POA_COLLECTION;

  POA_COLLECTION poa_collection_;
};

#if defined (__ACE_INLINE__)
# include "tao/POAManager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_POAMANAGER_H */
