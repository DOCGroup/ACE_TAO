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
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#ifndef	_CUBIT_I_HH
#define	_CUBIT_I_HH

#include "cubitS.h"

class Cubit_i : public POA_Cubit
  // = TITLE
  //   Cubit implementation class.
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
  
  virtual void please_exit (CORBA::Environment &env);
};

#endif	// _CUBIT_I_HH
