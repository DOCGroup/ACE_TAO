// $Id$
//=============================================================================
//
//
// = FILENAME
//     MyFooServant.h
//
// = DESCRIPTION
//     Defines MyFooServant class for the Foo interface
//
// = AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#ifndef MYFOOSERVANT_H
#define MYFOOSERVANT_H

#include "FooS.h"

class GENERIC_SERVANT_Export MyFooServant : public POA_Foo
{
public:
  MyFooServant (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                CORBA::Long value);
  // Constructor - takes a POA and a value parameter

  virtual ~MyFooServant (void);
  // Destructor

  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);
  // Returns the Default POA of this Servant object

  virtual CORBA::Long doit (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Simple doit method

  virtual void simply_doit (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Even simpler doit method

  virtual void timed_operation (CORBA::ULong timeout
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Timed operation.

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Shutdown the ORB

protected:
  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()

  CORBA::Long value_;
  // The current value.
};

#endif /* MYFOOSERVANT_H */
