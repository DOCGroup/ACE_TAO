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
  // constructor - takes a POA and a value parameter
  MyFooServant (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                CORBA::Long value);

  // Destructor
  virtual ~MyFooServant (void);

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

  // Even simpler doit method
  virtual void simply_doit (CORBA::Environment &env);

  // Shutdown the ORB
  virtual void shutdown (CORBA::Environment &env);

protected:
  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()

  CORBA::Long value_;
  // The current value.
};

#endif /* MYFOOSERVANT_H */
