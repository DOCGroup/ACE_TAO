// $Id$

#ifndef POA_T_CPP
#define POA_T_CPP

#include "tao/poa_T.h"

ACE_RCSID(tao, poa_T, "$Id$")

template <class ACE_LOCKING_MECHANISM>
TAO_Locked_POA<ACE_LOCKING_MECHANISM>::TAO_Locked_POA (const char *adapter_name,
                                                       PortableServer::POAManager_ptr poa_manager,
                                                       TAO_POA_Policies &policies,
                                                       PortableServer::POA_ptr parent,
                                                       CORBA::Environment &env)
  : TAO_POA (adapter_name,
             poa_manager,
             policies,
             parent,
             env)
{
}

template <class ACE_LOCKING_MECHANISM> TAO_POA *
TAO_Locked_POA<ACE_LOCKING_MECHANISM>::clone (const char *adapter_name,
                                              PortableServer::POAManager_ptr poa_manager,
                                              TAO_POA_Policies &policies,
                                              PortableServer::POA_ptr parent,
                                              CORBA::Environment &env)
{
  return new SELF (adapter_name,
                   poa_manager,
                   policies,
                   parent,
                   env);
}


template <class ACE_LOCKING_MECHANISM> ACE_Lock &
TAO_Locked_POA<ACE_LOCKING_MECHANISM>::lock (void)
{
  return this->lock_;
}

template <class ACE_LOCKING_MECHANISM>
TAO_Locked_POA_Manager<ACE_LOCKING_MECHANISM>::TAO_Locked_POA_Manager (void)
  : TAO_POA_Manager ()
{
}


template <class ACE_LOCKING_MECHANISM> TAO_POA_Manager *
TAO_Locked_POA_Manager<ACE_LOCKING_MECHANISM>::clone (void)
{
  return new SELF;
}

template <class ACE_LOCKING_MECHANISM> ACE_Lock &
TAO_Locked_POA_Manager<ACE_LOCKING_MECHANISM>::lock (void)
{
  return this->lock_;
}

#endif /* POA_T_CPP */
