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
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (_CUBIT_I_HH)
#define _CUBIT_I_HH

#include "cubitS.h"

class Cubit_i : public POA_Cubit
{
  // = TITLE
  //   Cubit implementation class.
public:
  Cubit_i (void);
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
};

class Cubit_Factory_i : public POA_Cubit_Factory
{
  // = TITLE
  //   Cubit Factory implementation class.
public:
  Cubit_Factory_i (CORBA::String *cubits,
                   u_int num_of_objs);
  // Constructor.

  ~Cubit_Factory_i (void);
  // Destructor.
  
  virtual CORBA::String create_cubit (CORBA::UShort index, 
				      CORBA::Environment &env); 
  // Make the cubit object whose priority is "priority".

private:
  CORBA::String * my_cubit_;
  // Array of cubit iors.

  u_int num_of_objs_;
  // Number of cubit objects we have ior's for.
};

#endif /* _CUBIT_I_HH */
