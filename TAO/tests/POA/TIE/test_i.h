// -*- C++ -*-

//=============================================================================
/**
 *  @file     test_i.h
 *
 *   Defines implementation classes for the interfaces
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

class Tie_i
{
public:
  // constructor - takes a POA and a value parameter
  Tie_i (CORBA::Long value);

  /// Destructor.
  virtual ~Tie_i ();

  // Simple doit method
  virtual CORBA::Long doit ();

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
  virtual CORBA::Long doit ();

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA ();

protected:
  CORBA::Long value_;

  /// Default poa associated with this servant
  PortableServer::POA_var poa_;
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
    virtual CORBA::Long doit ();

    // Returns the Default POA of this Servant object
    virtual PortableServer::POA_ptr _default_POA ();

  protected:
    CORBA::Long value_;

    /// Default poa associated with this servant
    PortableServer::POA_var poa_;
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
      virtual CORBA::Long doit ();

      // Returns the Default POA of this Servant object
      virtual PortableServer::POA_ptr _default_POA ();

    protected:
      CORBA::Long value_;

      /// Default poa associated with this servant
      PortableServer::POA_var poa_;
    };
  };
};
