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

class TAO_POA;
// Forward decl.

class TAO_Export TAO_POA_Manager : public POA_PortableServer::POAManager
{
  friend class TAO_POA;

public:
  enum Processing_State
  {
    ACTIVE,
    DISCARDING,
    HOLDING,
    INACTIVE,
    UNKNOWN
  };

  virtual void activate (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void hold_requests (CORBA::Boolean wait_for_completion,
                              CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void discard_requests (CORBA::Boolean wait_for_completion,
                                 CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void deactivate (CORBA::Boolean etherealize_objects,
                           CORBA::Boolean wait_for_completion,
                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  TAO_POA_Manager (void);

  virtual TAO_POA_Manager *clone (void);

  virtual ~TAO_POA_Manager (void);

  virtual Processing_State state (CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

protected:

  virtual ACE_Lock &lock (void);

  virtual void remove_poa (TAO_POA *poa,
                           CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void remove_poa_i (TAO_POA *poa,
                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void register_poa (TAO_POA *poa,
                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual void register_poa_i (TAO_POA *poa,
                               CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  Processing_State state_;

  int closing_down_;

  ACE_Lock *lock_;

  typedef ACE_Unbounded_Set<TAO_POA *> POA_COLLECTION;

  POA_COLLECTION poa_collection_;
};

#if defined (__ACE_INLINE__)
# include "tao/POAManager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_POAMANAGER_H */
