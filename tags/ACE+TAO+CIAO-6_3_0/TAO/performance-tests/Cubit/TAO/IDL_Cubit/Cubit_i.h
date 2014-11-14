
//=============================================================================
/**
 *  @file    Cubit_i.h
 *
 *  $Id$
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
  ~Cubit_i (void);

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (void);

  /// Test a oneway call.
  virtual void cube_oneway (void);

  /// Test a twoway call.
  virtual void cube_void (void);

  /// Cube an octet
  virtual CORBA::Octet cube_octet (CORBA::Octet o);

  /// Cube a short
  virtual CORBA::Short cube_short (CORBA::Short s);

  /// Cube a long
  virtual CORBA::Long cube_long (CORBA::Long l);

  /// Cube a struct.
  virtual Cubit::Many cube_struct (const Cubit::Many &values);

  /// Cube a union.
  virtual Cubit::oneof cube_union (const Cubit::oneof &values);

  /// Cube a sequence.
  virtual void cube_long_sequence (const Cubit::long_seq &input,
                                   Cubit::long_seq_out output);

  /// Cube an octet sequence.
  virtual void cube_octet_sequence (const Cubit::octet_seq &input,
                                    Cubit::octet_seq_out output);

  /// Cube an Many sequence.
  virtual void cube_many_sequence (const Cubit::many_seq & input,
                                   Cubit::many_seq_out output);

  /// Cube a sequence.
  virtual void cube_rti_data (const Cubit::RtiPacket &input,
                              Cubit::RtiPacket_out output);

  /// Cube a long in an any
  virtual CORBA::Any * cube_any (const CORBA::Any & any);

  /// Cube a struct in an any
  virtual CORBA::Any * cube_any_struct (const CORBA::Any & any);

  /// Shutdown routine.
  virtual void shutdown (void);


  /// A ping. Please see the idl file for details.
  virtual void ping (void);


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
  ~Cubit_Factory_i (void);

  /// Make a cubit object.
  virtual Cubit_ptr make_cubit (void);

  void set_default_poa (PortableServer::POA_ptr poa);

private:
  Cubit_i my_cubit_;
};

#endif /* _CUBIT_I_H */
