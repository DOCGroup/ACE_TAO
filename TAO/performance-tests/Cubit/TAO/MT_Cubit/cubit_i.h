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
#include "Task_Client.h"

class Cubit_i : public POA_Cubit
{
  // = TITLE
  //   Cubit implementation class.
public:
  Cubit_i (Task_State *ts);
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
private:
  Task_State *ts_;
  u_int util_started_;
};

#endif /* _CUBIT_I_HH */
