// $Id$
//=============================================================================
//
//
// = FILENAME
//     Foo_i.h
//
// = DESCRIPTION
//     Defines Foo_i class for the Foo interface
//
// = AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#if !defined (FOO_I_H)
#define FOO_I_H

#include "FooS.h"

class Simple_Foo_i
{
public:
  // constructor - takes a POA and a value parameter
  Simple_Foo_i (CORBA::Long value);

  // Simple doit method
  virtual CORBA::Long doit (CORBA::Environment &env);

protected:

  CORBA::Long value_;
};

class Foo_i : public POA_Foo
{
public:
  // constructor - takes a POA and a value parameter
  Foo_i (CORBA::Long value,
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

  class Foo_i : public POA_Outer::Foo
  {
  public:
    // constructor - takes a POA and a value parameter
    Foo_i (CORBA::Long value,
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

    class Foo_i : public POA_Outer::Inner::Foo
    {
    public:
      // constructor - takes a POA and a value parameter
      Foo_i (CORBA::Long value,
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
