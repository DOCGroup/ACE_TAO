// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
// 
// = FILENAME
//    simple_object_impl.h
//
// = DESCRIPTION
//    This class implements the Simple Object object.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (SIMPLE_OBJECT_IMPL_H)
#define	SIMPLE_OBJECT_IMPL_H

#include "Simple_ObjectS.h"

// Forward declarations.
class Simple_Impl;

// Typedefs.
typedef Simple_Impl *Simple_Impl_ptr;
typedef Simple_Impl_ptr Simple_Impl_ref;

class Simple_Impl: public POA_simple_object
{
  // = TITLE
  //    Simple Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that 
  //    returns the cube of a long, another that shuts down the server.
public:
  Simple_Impl (const char *obj_name = 0);
  // Constructor

  ~Simple_Impl (void);
  // Destructor

  virtual CORBA::Long simple_method (CORBA::Long l,
                                     CORBA::Environment &env);
  // Just cubes the long parameter

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown routine.
};


#endif /* SIMPLE_OBJECT_IMPL_H */
