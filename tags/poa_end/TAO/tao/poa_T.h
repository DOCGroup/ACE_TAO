#if !defined (POA_T_H)
#define POA_T_H

#include "tao/poa.h"

template <class ACE_LOCKING_MECHANISM>
class TAO_Locked_POA : public TAO_POA
{
public:

  TAO_Locked_POA (const char *adapter_name,
                  PortableServer::POAManager_ptr poa_manager,
                  TAO_POA_Policies &policies,
                  PortableServer::POA_ptr parent,
                  CORBA::Environment &env);

  TAO_Locked_POA (const char *adapter_name,
                  PortableServer::POAManager_ptr poa_manager,
                  TAO_POA_Policies &policies,
                  PortableServer::POA_ptr parent,
                  TAO_Object_Table &active_object_table,
                  CORBA::Environment &env);

  virtual TAO_POA *clone (const char *adapter_name,
                          PortableServer::POAManager_ptr poa_manager,
                          TAO_POA_Policies &policies,
                          PortableServer::POA_ptr parent,
                          CORBA::Environment &env);

  virtual TAO_POA *clone (const char *adapter_name,
                          PortableServer::POAManager_ptr poa_manager,
                          TAO_POA_Policies &policies,
                          PortableServer::POA_ptr parent,
                          TAO_Object_Table &active_object_table,
                          CORBA::Environment &env);

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

#endif /* POA_T_H */
