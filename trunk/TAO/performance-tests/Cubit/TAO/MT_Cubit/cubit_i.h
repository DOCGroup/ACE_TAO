/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    cubit_i.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, Sergio Flores-Gaitan and Nagarajan Surendran.
//
// ============================================================================

#ifndef _CUBIT_I_H
#define _CUBIT_I_H

#include "cubitS.h"

class Cubit_i : public POA_Cubit
{
  // = TITLE
  //   Cubit implementation class.
public:
  // = Initialization and termination methods.
  Cubit_i (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
  ~Cubit_i (void);

  virtual CORBA::Octet cube_octet (CORBA::Octet o,
                                   CORBA::Environment &env);

  virtual CORBA::Short cube_short (CORBA::Short s,
                                   CORBA::Environment &env);

  virtual CORBA::Long cube_long (CORBA::Long l,
                                 CORBA::Environment &env);

  virtual Cubit::Many cube_struct (const Cubit::Many &values,
                                   CORBA::Environment &env);

  virtual void noop (CORBA::Environment &env);

  virtual void shutdown (CORBA::Environment &env);

  virtual PortableServer::POA_ptr
      _default_POA (CORBA::Environment &ACE_TRY_ENV);
  // The default POA

protected:
  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.

  PortableServer::POA_var poa_;
  // The default POA..
};

#endif /* _CUBIT_I_H */
