// @(#)cubit_i.cpp 05/14/97
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// TEST:	hand-written Cubit Implementation
//
// Modified version of Cubit Example written by Sun Microsystems Inc.
// Modified by: Brian Mendel and Andy Gokhale

#if !defined (_CUBIT_I_HH)
#define	_CUBIT_I_HH

#include "cubitS.h"

class Cubit_i;
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i : public POA_Cubit
  // = TITLE
  //    Illustrates how to integrate a servant with the
  //    generated skeleton.
{
public:
  Cubit_i (const char *obj_name = 0);
  ~Cubit_i (void);

  virtual CORBA::Octet cube_octet (CORBA::Octet o,
                                   CORBA::Environment &env);

  virtual CORBA::Short cube_short (CORBA::Short s,
                                   CORBA::Environment &env);

  virtual CORBA::Long cube_long (CORBA::Long l,
                                 CORBA::Environment &env);

  virtual Cubit::Many cube_struct (const Cubit::Many &values,
                                   CORBA::Environment &env);

  virtual Cubit::oneof cube_union (const Cubit::oneof &values,
                                   CORBA::Environment &env);

  virtual void please_exit (CORBA::Environment &env);
};

class Cubit_Factory_i;

typedef Cubit_Factory_i *Cubit_Factory_i_ptr;

class Cubit_Factory_i: public POA_Cubit_Factory
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
  int numobjs_;
};

#endif /* _CUBIT_I_HH */
