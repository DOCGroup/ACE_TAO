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

#if !defined (MYFOOSERVANT_H)
#define MYFOOSERVANT_H

#include "FooS.h"
#include "Servant_Locator.h"

class MyFirstFooServant : public POA_Foo
{
  // = TITLE
  // @@ Michael, please comment me.
public:
  // constructor - takes a POA and a value parameter
  MyFirstFooServant (PortableServer::POA_ptr poa,
                     CORBA::Long value,
                     CORBA::Object_ptr forward_to);
  
  // Destructor
  virtual ~MyFirstFooServant (void);
  
  //Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);
  
  // Setup forwarding
  virtual void forward (CORBA::Environment &env);
  
protected:
  // Default poa associated with this servant
  PortableServer::POA_var poa_;
  CORBA::Long value_;
  CORBA::Object_var forward_to_;
};

class MySecondFooServant : public POA_Foo
{
  // = TITLE
  // @@ Michael, please comment me.
public:
  // constructor - takes a POA and a value parameter
  MySecondFooServant (MyFooServantLocator *locator,
                      CORBA::Long value);
  
  // Destructor
  virtual ~MySecondFooServant (void);
  
  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);
  
  // Setup forwarding
  virtual void forward (CORBA::Environment &env);

protected:
  // Default poa associated with this servant
  MyFooServantLocator *locator_ptr_;
  CORBA::Long value_;
};

#endif /* MYFOOSERVANT_H */

