// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    cubit_i.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#ifndef _CUBIT_I_HH
#define _CUBIT_I_HH

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <api/api.H>
#include <api/binding.H>
#include <corba/eoa.H>
#include <corba/eorb.H>

extern COOL_CurrentCapsule* thisCapsule;

#include "cubit.H"
#include "sk_cubit.H"

// Forward declarations
class Cubit_i;

// Typedefs.
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i
  // = TITLE
  //    Illustrates how to integrate a servant with the
  //    generated skeleton.
  // = DESCRIPTION
  //    Implementation of the cubit example at the servant side.
  //    Cubes an octet, short, long, struct and union.
{
public:
  Cubit_i (const char *obj_name = 0);
  // Constructor

  ~Cubit_i (void);
  // Destructor

  virtual CORBA::Octet cube_octet (CORBA::Octet o,
                                   CORBA::Environment &env);
  // Cube an octet

  virtual CORBA::Short cube_short (CORBA::Short s,
                                   CORBA::Environment &env);
  // Cube a short

  virtual CORBA::Long cube_long (CORBA::Long l,
                                 CORBA::Environment &env);
  // Cube a long

  virtual Cubit::Many cube_struct (const Cubit::Many &values,
                                   CORBA::Environment &env);
  // Cube a struct.

  virtual Cubit::oneof cube_union (const Cubit::oneof &values,
                                   CORBA::Environment &env);
  // Cube a union.

  virtual void please_exit (CORBA::Environment &env);
  // Shutdown routine.

  const char *_get_name (void);

 private:
  const char * obj_name_;
};


class Cubit_Factory_i;

typedef Cubit_Factory_i *Cubit_Factory_i_ptr;

class Cubit_Factory_i
  // =TITLE
  //  Cubit_Factory_i
  //
  // DESCRIPTION
  //  factory object returning the cubit objrefs
{
public:
  Cubit_Factory_i (const char *key, int numobjs);
  // constructor

  ~Cubit_Factory_i (void);
  // destructor

  virtual Cubit_ptr make_cubit (const char *key, CORBA::Environment &env);
  // make the cubit object whose key is "key"

private:
  Cubit_i_ptr *my_cubit_;
  u_int numobjs_;
};

#endif /* _CUBIT_I_HH */
