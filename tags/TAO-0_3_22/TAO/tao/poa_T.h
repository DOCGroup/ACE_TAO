// $Id$

#ifndef (TAO_POA_T_H)
#define TAO_POA_T_H

#include "tao/poa.h"

template <class ACE_LOCKING_MECHANISM>
class TAO_Locked_POA : public TAO_POA
{
public:
  TAO_Locked_POA (const char *adapter_name,
                  PortableServer::POAManager_ptr poa_manager,
                  TAO_POA_Policies &policies,
                  PortableServer::POA_ptr parent,
                  CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

  virtual TAO_POA *clone (const char *adapter_name,
                          PortableServer::POAManager_ptr poa_manager,
                          TAO_POA_Policies &policies,
                          PortableServer::POA_ptr parent,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());

protected:

  virtual ACE_Lock &lock (void);
  ACE_Lock_Adapter<ACE_LOCKING_MECHANISM> lock_;
  typedef TAO_Locked_POA<ACE_LOCKING_MECHANISM> SELF;
};

template <class ACE_LOCKING_MECHANISM>
class TAO_Locked_POA_Manager : public TAO_POA_Manager
{
public:
  TAO_Locked_POA_Manager (void);
  virtual TAO_POA_Manager *clone (void);

protected:
  virtual ACE_Lock &lock (void);
  ACE_Lock_Adapter<ACE_LOCKING_MECHANISM> lock_;
  typedef TAO_Locked_POA_Manager<ACE_LOCKING_MECHANISM> SELF;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/poa_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("poa_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_POA_T_H */
