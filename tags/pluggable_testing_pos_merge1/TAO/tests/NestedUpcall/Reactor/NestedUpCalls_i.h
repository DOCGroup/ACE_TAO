// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    NestedUpCalls_i.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#ifndef _NESTEDUPCALLS_I_H
#define _NESTEDUPCALLS_I_H

//#include "cubitS.h"

// Forward declarations.
class NestedUpCalls_i;

// Typedefs.
typedef NestedUpCalls_i *NestedUpCalls_i_ptr;
typedef NestedUpCalls_i_ptr NestedUpCalls_i_ref;

class NestedUpCalls_i : public POA
{
  // = TITLE
  //    Illustrates how to integrate a servant with the generated
  //    skeleton.
  //
  // = DESCRIPTION
  //    Implementation of the cubit example at the servant side.
  //    Cubes an octet, short, long, struct and union.
public:
  NestedUpCalls_i (const char *obj_name = 0);
  // Constructor

  ~NestedUpCalls_i (void);
  // Destructor

/*  virtual CORBA::Octet cube_octet (CORBA::Octet o,
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
*/
  virtual void shutdown (CORBA::Environment &env);
  // Shutdown routine.
};

class NestedUpCalls_Reactor_i;

typedef NestedUpCalls_Reactor_i *NestedUpCalls_Reactor_i_ptr;

class NestedUpCalls_Reactor_i: public POA_Cubit_Factory
{
  // = TITLE
  //   NestedUpCalls_Reactor_i
  //
  // = DESCRIPTION
  //   Factory object returning the cubit objrefs
public:
  NestedUpCalls_Reactor_i (void);
  // Constructor.

  ~NestedUpCalls_Reactor_i (void);
  // Destructor.

  virtual Cubit_ptr make_cubit (const char *key,
                                CORBA::Environment &env);
  // Make the cubit object whose key is "key".

private:
  NestedUpCalls_i my_cubit_;
};

#endif /* _NestedUpCalls_i_H */
