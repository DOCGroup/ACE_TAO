// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Cubit/CORBAplus/IDL_Cubit
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

#include <pbroker/corba/lifecycl/actvtr.h>
#include <pbroker/corba/lifecycl/impldecl.h>

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "cubit_s.h"

// Forward declarations
class Cubit_i;

// Typedefs.
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i : public Cubit_base_impl
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

  virtual CORBA::Octet cube_octet (CORBA::Octet o);
  // Cube an octet

  virtual CORBA::Short cube_short (CORBA::Short s);
  // Cube a short

  virtual CORBA::Long cube_long (CORBA::Long l);
  // Cube a long

  virtual Cubit::Many cube_struct (const Cubit::Many &values);
  // Cube a struct.

  virtual Cubit::oneof cube_union (const Cubit::oneof &values);
  // Cube a union.

  virtual void please_exit (void);
  // Shutdown routine.

  const char *_get_name (void);
  // get the key of the object.
 private:
  const char *key_;
};


class Cubit_Factory_i;

typedef Cubit_Factory_i *Cubit_Factory_i_ptr;

class Cubit_Factory_i: public Cubit_Factory_base_impl
  // =TITLE
  //  Cubit_Factory_i
  //
  // DESCRIPTION
  //  factory object returning the cubit objrefs
{
public:
  XPS_CAST1_DECL(Cubit_Factory_i,Cubit_Factory_base_impl)
  XPS_DEFINE_IMPLEMENTATION(Cubit_Factory_decl)

  Cubit_Factory_i (void)
    {Cubit_Factory_i ("keyone", 1);}

  Cubit_Factory_i (const CORBA_ReferenceData & refData)
    {Cubit_Factory_i ("keyone", 1);}

  Cubit_Factory_i (const char *key, int numobjs);
  // constructor

  ~Cubit_Factory_i (void);
  // destructor

  virtual Cubit_ptr make_cubit (const char *key);
  // make the cubit object whose key is "key"

private:
  Cubit_i_ptr *my_cubit_;
  u_int numobjs_;
};

#endif /* _CUBIT_I_HH */
