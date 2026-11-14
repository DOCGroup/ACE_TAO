
//=============================================================================
/**
 *  @file    CC_naming_service.h
 *
 *    This class implements the naming services necessary to test the
 *    concurrency service.
 *
 *  @author Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================

#include "orbsvcs/CosConcurrencyControlC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include <string>

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
  CC_naming_service () = default;
  CC_naming_service (CC_naming_service const &) = delete;
  CC_naming_service (CC_naming_service &&) = delete;
  CC_naming_service &operator= (CC_naming_service const &) = delete;
  CC_naming_service &operator= (CC_naming_service &&) = delete;

  explicit CC_naming_service (CORBA::ORB_ptr orb);

  ~CC_naming_service ();

  /// Initialize this object before using the singleton accessor.
  void Init (CORBA::ORB_ptr orb);

  /// Lookup a name in the naming service (context, name)
  CORBA::Object_var get_obj_from_name (std::string const &context,
                                       std::string const &name);

  /// Bind a name in the concurrency context.
  void bind_name (std::string const &name,
                  CORBA::Object_ptr obj);

  /// Gets the cc lock set factory which is common for all tests.
  CosConcurrencyControl::LockSetFactory_var get_lock_set_factory () const;

  /// The naming service object is a singleton
  static CC_naming_service *Instance();

private:
  /// Function to initialize the naming service.
  int init_naming_service ();

  /// Remember our orb.
  CORBA::ORB_var orb_ {};

  /// Name service wrapper class
  TAO_Naming_Client my_name_client_;

  /// factory pointer for the lock set.
  CosConcurrencyControl::LockSetFactory_var factory_ {};

  /// Borrowed pointer; lifetime is controlled by CC_Client or the function-local fallback.
  static CC_naming_service *instance_;
};
#endif /* !defined (_CC_NAMING_SERVICE_H_) */
