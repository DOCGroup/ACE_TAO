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

  virtual void cube_oneway (CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Test a oneway call.

  virtual void cube_void (CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Test a twoway call.

  virtual CORBA::Octet cube_octet (CORBA::Octet o,
                                   CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an octet

  virtual CORBA::Short cube_short (CORBA::Short s,
                                   CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a short

  virtual CORBA::Long cube_long (CORBA::Long l,
                                 CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a long

  virtual Cubit::Many cube_struct (const Cubit::Many &values,
                                   CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a struct.

  virtual Cubit::oneof cube_union (const Cubit::oneof &values,
                                   CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a union.

  virtual void cube_long_sequence (const Cubit::long_seq &input,
                                   Cubit::long_seq_out output,
                                   CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a sequence.

  virtual void cube_octet_sequence (const Cubit::octet_seq &input,
                                    Cubit::octet_seq_out output,
                                    CORBA::Environment &_env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an octet sequence.

  virtual void cube_many_sequence (const Cubit::many_seq & input,
                                   Cubit::many_seq_out output,
                                   CORBA::Environment &_tao_environment)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube an Many sequence.

  virtual void cube_rti_data (const Cubit::RtiPacket &input,
                              Cubit::RtiPacket_out output,
                              CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Cube a sequence.

  virtual void shutdown (CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Shutdown routine.

protected:

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

  virtual Cubit_ptr make_cubit (CORBA::Environment &env)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Make a cubit object.

private:
  Cubit_i my_cubit_;

  int cubit_registered_;
};

#endif /* _CUBIT_I_H */
