
//=============================================================================
/**
 *  @file    Cubit_i.h
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee and Sergio Flores-Gaitan
 */
//=============================================================================

#ifndef _CUBIT_I_H
#define _CUBIT_I_H

#include "cubitS.h"

// Forward declarations.
class Cubit_i;

/**
 * @class Cubit_i
 *
 * @brief Illustrates how to integrate a servant with the generated
 * skeleton.
 *
 * Implementation of the cubit example at the servant side.
 * Cubes an octet, short, long, struct and union.
 */
class Cubit_i : public POA_Cubit
{
public:
  /// Constructor
  Cubit_i (CORBA::ORB_ptr orb);

  /// Destructor
  ~Cubit_i () override;

  /// Returns the default POA for this servant.
  PortableServer::POA_ptr _default_POA () override;

  /// Test a oneway call.
  void cube_oneway () override;

  /// Test a twoway call.
  void cube_void () override;

  /// Cube an octet
  CORBA::Octet cube_octet (CORBA::Octet o) override;

  /// Cube a short
  CORBA::Short cube_short (CORBA::Short s) override;

  /// Cube a long
  CORBA::Long cube_long (CORBA::Long l) override;

  /// Cube a struct.
  Cubit::Many cube_struct (const Cubit::Many &values) override;

  /// Cube a union.
  Cubit::oneof cube_union (const Cubit::oneof &values) override;

  /// Cube a sequence.
  void cube_long_sequence (const Cubit::long_seq &input,
                           Cubit::long_seq_out output) override;

  /// Cube an octet sequence.
  void cube_octet_sequence (const Cubit::octet_seq &input,
                            Cubit::octet_seq_out output) override;

  /// Cube an Many sequence.
  void cube_many_sequence (const Cubit::many_seq & input,
                           Cubit::many_seq_out output) override;

  /// Cube a sequence.
  void cube_rti_data (const Cubit::RtiPacket &input,
                      Cubit::RtiPacket_out output) override;

  /// Cube a long in an any
  CORBA::Any * cube_any (const CORBA::Any & any) override;

  /// Cube a struct in an any
  CORBA::Any * cube_any_struct (const CORBA::Any & any) override;

  /// Shutdown routine.
  void shutdown () override;

  /// A ping. Please see the idl file for details.
  void ping () override;

  /// Set default poa.
  void set_default_poa (PortableServer::POA_ptr poa);

protected:
  PortableServer::POA_var poa_;

  /// Keep a pointer to the ORB so we can shut it down.
  CORBA::ORB_var orb_;
};

/**
 * @class Cubit_Factory_i:
 *
 * @brief Cubit_Factory_i
 *
 * Factory object returning the cubit objrefs
 */
class Cubit_Factory_i: public POA_Cubit_Factory
{
public:
  /// Constructor.
  Cubit_Factory_i (CORBA::ORB_ptr orb);

  /// Destructor.
  ~Cubit_Factory_i () override;

  /// Make a cubit object.
  Cubit_ptr make_cubit () override;

  void set_default_poa (PortableServer::POA_ptr poa);

private:
  Cubit_i my_cubit_;
};

#endif /* _CUBIT_I_H */
