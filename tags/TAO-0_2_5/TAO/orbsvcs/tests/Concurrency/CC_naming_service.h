// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Concurrency
//
// = FILENAME
//    CC_naming_service.h
//
// = DESCRIPTION
//      This class implements the naming services necessary to test the
//      concurrency service.
//
// = AUTHORS
//    Torben Worm <tworm@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (_CC_NAMING_SERVICE_H_)
#define _CC_NAMING_SERVICE_H_

class CC_naming_service
{
  // = TITLE
  //    Defines a class that encapsulates the necessary naming service
  //    functions for the concurrency service.
  //
  // = DESCRIPTION
  //    This class declares an interface to the naming service for the
  //    concurrency service tests.
public:
  CC_naming_service ();
  // Default constructor

  CC_naming_service (CORBA::ORB_var orb_, CORBA::Environment &_env);
  // Constructor

  ~CC_naming_service (void);
  // Default destructor.

  void Init (CORBA::ORB_var orb_, CORBA::Environment &_env);
  // Initialization method must be called after first call of Instance()
  // in order to initialize the object correctly

  CORBA::Object_var get_obj_from_name (char *c_name, char *name,
                                       CORBA::Environment &_env);
  // Lookup a name in the naming service (context, name)

  void
  bind_name (char *n,
             CORBA::Object_ptr obj,
             CORBA::Environment &_env);
  // Bind a name in the concurrency context.

  CosConcurrencyControl::LockSetFactory_var get_lock_set_factory (void);
  // Gets the cc lock set factory which is common for all tests.

  static CC_naming_service *Instance(void);
  // The naming service object is a singleton

private:
  int init_naming_service (void);
  // Function to initialize the naming service.

  CosNaming::NamingContext_var naming_context_;
  // Naming service object for this object.

  char *cc_factory_key_;
  // Key of factory obj ref.

  CORBA::ORB_var orb_;
  // Remember our orb.

  CosConcurrencyControl::LockSetFactory_var factory_;
  // factory pointer for the lock set.

  static CC_naming_service *instance_;
  // The instance of the naming service object
};
#endif /* !defined (_CC_NAMING_SERVICE_H_) */
