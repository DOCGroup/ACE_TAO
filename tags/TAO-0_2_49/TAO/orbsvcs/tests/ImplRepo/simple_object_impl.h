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
#include "../../ImplRepo_Service/PingS.h"

// Forward declarations.
class Simple_i;

// Typedefs.
typedef Simple_i *Simple_i_ptr;
typedef Simple_i_ptr Simple_i_ref;

class Simple_i: public POA_simple_object
{
  // = TITLE
  //    Simple Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that 
  //    returns the cube of a long, another that shuts down the server.
public:
  Simple_i (const char *obj_name = 0);
  // Constructor

  ~Simple_i (void);
  // Destructor

  virtual CORBA::Long simple_method (CORBA::Long l,
                                     CORBA::Environment &env);
  // Just cubes the long parameter

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown routine.

  int use_ir_;
  // Should we shutdown after each call?  If using the Impl Repo, then yes.

};

// Forward declarations.
class Ping_i;

// Typedefs.
typedef Ping_i *Ping_i_ptr;
typedef Ping_i_ptr Ping_i_ref;

class Ping_i: public POA_Ping_Object
  // = TITLE
  //    Ping Object Implementation
  //
  // = DESCRIPTION
  //    Implementation Repository uses this to check to see if the server
  //    is still running.
{
public:
  virtual void ping (CORBA::Environment &env);
};

#endif /* SIMPLE_OBJECT_IMPL_H */
