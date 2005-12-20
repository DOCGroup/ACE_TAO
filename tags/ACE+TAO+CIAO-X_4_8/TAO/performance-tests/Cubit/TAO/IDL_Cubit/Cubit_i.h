// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    Cubit_Client.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#ifndef _CUBIT_I_H
#define _CUBIT_I_H

#include "cubitS.h"

// Forward declarations.
class Cubit_i;

class Cubit_i : public POA_Cubit
{
  // = TITLE
  //    Illustrates how to integrate a servant with the generated
  //    skeleton.
  //
  // = DESCRIPTION
  //    Implementation of the cubit example at the servant side.
  //    Cubes an octet, short, long, struct and union.
public:
  Cubit_i (CORBA::ORB_ptr orb);
  // Constructor

  ~Cubit_i (void);
  // Destructor

  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Returns the default POA for this servant.

  virtual void cube_oneway (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Test a oneway call.

  virtual void cube_void (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Test a twoway call.

  virtual CORBA::Octet cube_octet (CORBA::Octet o
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an octet

  virtual CORBA::Short cube_short (CORBA::Short s
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a short

  virtual CORBA::Long cube_long (CORBA::Long l
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a long

  virtual Cubit::Many cube_struct (const Cubit::Many &values
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a struct.

  virtual Cubit::oneof cube_union (const Cubit::oneof &values
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a union.

  virtual void cube_long_sequence (const Cubit::long_seq &input,
                                   Cubit::long_seq_out output
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a sequence.

  virtual void cube_octet_sequence (const Cubit::octet_seq &input,
                                    Cubit::octet_seq_out output
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an octet sequence.

  virtual void cube_many_sequence (const Cubit::many_seq & input,
                                   Cubit::many_seq_out output
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an Many sequence.

  virtual void cube_rti_data (const Cubit::RtiPacket &input,
                              Cubit::RtiPacket_out output
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a sequence.

  virtual CORBA::Any * cube_any (const CORBA::Any & any
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a long in an any

  virtual CORBA::Any * cube_any_struct (const CORBA::Any & any
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a struct in an any

  /// Shutdown routine.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


  /// A ping. Please see the idl file for details.
  virtual void ping (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


  void set_default_poa (PortableServer::POA_ptr poa);
  // Set default poa.

protected:
  PortableServer::POA_var poa_;

  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.
};

class Cubit_Factory_i: public POA_Cubit_Factory
{
  // = TITLE
  //   Cubit_Factory_i
  //
  // = DESCRIPTION
  //   Factory object returning the cubit objrefs
public:
  Cubit_Factory_i (CORBA::ORB_ptr orb);
  // Constructor.

  ~Cubit_Factory_i (void);
  // Destructor.

  virtual Cubit_ptr make_cubit (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Make a cubit object.

  void set_default_poa (PortableServer::POA_ptr poa);

private:
  Cubit_i my_cubit_;
};

#endif /* _CUBIT_I_H */
