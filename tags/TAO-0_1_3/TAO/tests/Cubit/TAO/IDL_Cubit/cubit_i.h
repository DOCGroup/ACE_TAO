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

#if !defined (_CUBIT_I_HH)
#define	_CUBIT_I_HH

#include "cubitS.h"

// Forward declarations
class Cubit_i;

// Typedefs.
typedef Cubit_i *Cubit_i_ptr;
typedef Cubit_i_ptr Cubit_i_ref;

class Cubit_i : public POA_Cubit
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

  virtual void cube_sequence (const Cubit::vector &input,
			      Cubit::vector_out output,
			      CORBA::Environment &env);
  // Cube a sequence.

  virtual void please_exit (CORBA::Environment &env);
  // Shutdown routine.
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
  Cubit_Factory_i (void);
  // constructor

  ~Cubit_Factory_i (void);
  // destructor

  virtual Cubit_ptr make_cubit (const char *key, CORBA::Environment &env);
  // make the cubit object whose key is "key"

private:
  Cubit_i my_cubit_;
};

#endif /* _CUBIT_I_HH */
