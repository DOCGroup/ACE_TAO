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

#if !defined (MYFOOSERVANT_H)
#define MYFOOSERVANT_H

#include "FooS.h"

class GENERIC_SERVANT_Export MyFooServant : public POA_Foo
{
public:
  // constructor - takes a POA and a value parameter
  MyFooServant (PortableServer::POA_ptr poa,
                CORBA::Long value);

  // Destructor
  virtual ~MyFooServant (void);

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

  // Even simpler doit method
  virtual void simply_doit (CORBA::Environment &env);

protected:
  // Default poa associated with this servant
  PortableServer::POA_var poa_;
  CORBA::Long value_;
};

#endif /* MYFOOSERVANT_H */
