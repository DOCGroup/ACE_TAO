/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    cubit_i.h
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


#ifndef _CUBIT_I_H
#define _CUBIT_I_H

#include "cubitS.h"

/**
 * @class Cubit_i
 *
 * @brief Cubit implementation class.
 */
class Cubit_i : public POA_Cubit
{
public:
  Cubit_i (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
  ~Cubit_i ();

  virtual CORBA::Octet cube_octet (CORBA::Octet o);

  virtual CORBA::Short cube_short (CORBA::Short s);

  virtual CORBA::Long cube_long (CORBA::Long l);

  virtual Cubit::Many cube_struct (const Cubit::Many &values);

  virtual void noop ();

  virtual void shutdown ();

  /// The default POA
  virtual PortableServer::POA_ptr
      _default_POA ();

protected:
  /// Keep a pointer to the ORB so we can shut it down.
  CORBA::ORB_var orb_;

  /// The default POA..
  PortableServer::POA_var poa_;
};

#endif /* _CUBIT_I_H */
