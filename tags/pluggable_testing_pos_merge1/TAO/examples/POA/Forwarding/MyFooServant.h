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
//     Irfan Pyarali and Michael Kircher
//
//=============================================================================

#ifndef MYFOOSERVANT_H
#define MYFOOSERVANT_H

#include "FooS.h"
#include "Servant_Locator.h"

class MyFirstFooServant : public POA_Foo
{
  // = TITLE
  // @@ Michael, please comment me.
public:
  // constructor - takes a POA and a value parameter
  MyFirstFooServant (CORBA::ORB_ptr orb_ptr,
                     PortableServer::POA_ptr poa_ptr,
                     CORBA::Long value,
                     CORBA::Object_ptr forward_to_ptr);

  // Destructor
  virtual ~MyFirstFooServant (void);

  //Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

  // Setup forwarding
  virtual void forward (CORBA::Environment &env);

  virtual void shutdown (CORBA::Environment &env);

protected:
  // Default poa associated with this servant
  CORBA::ORB_var orb_var_;
  PortableServer::POA_var poa_var_;
  CORBA::Long value_;
  CORBA::Object_var forward_to_var_;
};

class MySecondFooServant : public POA_Foo
{
  // = TITLE
  // @@ Michael, please comment me.
public:
  // constructor - takes a POA and a value parameter
  MySecondFooServant (CORBA::ORB_ptr orb_ptr,
                      MyFooServantLocator *locator,
                      CORBA::Long value);

  // Destructor
  virtual ~MySecondFooServant (void);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

  // Setup forwarding
  virtual void forward (CORBA::Environment &env);

  virtual void shutdown (CORBA::Environment &env);

protected:
  // Default poa associated with this servant
  CORBA::ORB_var orb_var_;
  MyFooServantLocator *locator_ptr_;
  CORBA::Long value_;
};

#endif /* MYFOOSERVANT_H */

