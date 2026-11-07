// -*- C++ -*-

//=============================================================================
/**
 *  @file     test_i.h
 *
 *   Defines implementation classes for the interfaces
 *
 *  @author  Irfan Pyarali
 */
#include "testS.h"

class Tie_i
{
public:
  explicit Tie_i (CORBA::Long value);

  virtual ~Tie_i () = default;

  Tie_i (Tie_i const &) = delete;
  Tie_i (Tie_i &&) = delete;
  Tie_i &operator= (Tie_i const &) = delete;
  Tie_i &operator= (Tie_i &&) = delete;

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

  A_i (A_i const &) = delete;
  A_i (A_i &&) = delete;
  A_i &operator= (A_i const &) = delete;
  A_i &operator= (A_i &&) = delete;

  CORBA::Long doit () override;

  PortableServer::POA_ptr _default_POA () override;

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

    B_i (B_i const &) = delete;
    B_i (B_i &&) = delete;
    B_i &operator= (B_i const &) = delete;
    B_i &operator= (B_i &&) = delete;

    CORBA::Long doit () override;

    PortableServer::POA_ptr _default_POA () override;

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

      C_i (C_i const &) = delete;
      C_i (C_i &&) = delete;
      C_i &operator= (C_i const &) = delete;
      C_i &operator= (C_i &&) = delete;

      CORBA::Long doit () override;

      PortableServer::POA_ptr _default_POA () override;

    protected:
      CORBA::Long value_;

      /// Default poa associated with this servant
      PortableServer::POA_var poa_;
    };
  };
};
