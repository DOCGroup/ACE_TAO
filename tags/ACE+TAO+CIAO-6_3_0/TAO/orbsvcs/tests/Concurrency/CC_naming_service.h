
//=============================================================================
/**
 *  @file    CC_naming_service.h
 *
 *  $Id$
 *
 *    This class implements the naming services necessary to test the
 *    concurrency service.
 *
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

#ifndef _CC_NAMING_SERVICE_H_
#define _CC_NAMING_SERVICE_H_

/**
 * @class CC_naming_service
 *
 * @brief Defines a class that encapsulates the necessary naming service
 * functions for the concurrency service.
 *
 * This class declares an interface to the naming service for the
 * concurrency service tests.
 */
class CC_naming_service
{
public:
  /// Default constructor
  CC_naming_service ();

  /// Constructor
  CC_naming_service (CORBA::ORB_var orb_);

  /// Default destructor.
  ~CC_naming_service (void);

  /// Initialization method must be called after first call of Instance()
  /// in order to initialize the object correctly
  void Init (CORBA::ORB_var orb_);

  /// Lookup a name in the naming service (context, name)
  CORBA::Object_var get_obj_from_name (const char *c_name,
                                       const char *name);

  /// Bind a name in the concurrency context.
  void bind_name (const char *n,
                  CORBA::Object_ptr obj);

  /// Gets the cc lock set factory which is common for all tests.
  CosConcurrencyControl::LockSetFactory_var get_lock_set_factory (void);

  /// The naming service object is a singleton
  static CC_naming_service *Instance(void);

private:
  /// Function to initialize the naming service.
  int init_naming_service (void);

  /// Key of factory obj ref.
  char *cc_factory_key_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Name service wrapper class
  TAO_Naming_Client my_name_client_;

  /// factory pointer for the lock set.
  CosConcurrencyControl::LockSetFactory_var factory_;

  /// The instance of the naming service object
  static CC_naming_service *instance_;
};
#endif /* !defined (_CC_NAMING_SERVICE_H_) */
