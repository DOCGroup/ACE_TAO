// $Id$
//=============================================================================
//
//
// = FILENAME
//     Foo_i.h
//
// = DESCRIPTION
//     Defines implementation classes for the interfaces
//
// = AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#ifndef FOO_I_H
#define FOO_I_H

#include "FooS.h"

class Tie_i
{
public:
  // constructor - takes a POA and a value parameter
  Tie_i (CORBA::Long value);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

protected:

  CORBA::Long value_;
};

class A_i : public POA_A
{
public:
  // constructor - takes a POA and a value parameter
  A_i (CORBA::Long value,
       PortableServer::POA_ptr poa);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

protected:

  CORBA::Long value_;

  PortableServer::POA_var poa_;
  // Default poa associated with this servant
};

class Outer_i
{
public:

  class B_i : public POA_Outer::B
  {
  public:
    // constructor - takes a POA and a value parameter
    B_i (CORBA::Long value,
         PortableServer::POA_ptr poa);

    // Simple doit method
    virtual CORBA::Long doit (CORBA::Environment &env);

    // Returns the Default POA of this Servant object
    virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  protected:

    CORBA::Long value_;

    PortableServer::POA_var poa_;
    // Default poa associated with this servant
  };

  class Inner_i
  {
  public:

    class C_i : public POA_Outer::Inner::C
    {
    public:
      // constructor - takes a POA and a value parameter
      C_i (CORBA::Long value,
           PortableServer::POA_ptr poa);

      // Simple doit method
      virtual CORBA::Long doit (CORBA::Environment &env);

      // Returns the Default POA of this Servant object
      virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

    protected:

      CORBA::Long value_;

      PortableServer::POA_var poa_;
      // Default poa associated with this servant
    };
  };
};

#endif /* FOO_I_H */
