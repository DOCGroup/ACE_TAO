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

#if !defined (IR_SIMPLE_OBJECT_IMPL_H)
#define	IR_SIMPLE_OBJECT_IMPL_H

#include "Impl_RepoS.h"

// Forward declarations.
class IR_Simple_i;

// Typedefs.
typedef IR_Simple_i *IR_Simple_i_ptr;
typedef IR_Simple_i_ptr IR_Simple_i_ref;

class IR_Simple_i: public POA_simple_object
{
  // = TITLE
  //    Simple Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that 
  //    returns the cube of a long, another that shuts down the server.
public:
  IR_Simple_i (CORBA::ORB_ptr orb_ptr,
                  PortableServer::POA_ptr poa_ptr,
                  CORBA::Object_ptr forward_to_ptr);
  // Constructor

  ~IR_Simple_i (void);
  // Destructor

  virtual CORBA::Long simple_method (CORBA::Long l,
                                     CORBA::Environment &env);
  // Just cubes the long parameter

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown routine.

  CORBA::ORB_var orb_var_;
  PortableServer::POA_var poa_var_;
  CORBA::Object_var forward_to_var_;
};


#endif /* IR_SIMPLE_OBJECT_IMPL_H */
