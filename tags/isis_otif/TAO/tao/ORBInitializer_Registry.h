// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORBInitializer_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_ORB_INITIALIZER_REGISTRY_H
#define TAO_ORB_INITIALIZER_REGISTRY_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "ace/Array_Base.h"

/**
 * @class TAO_ORBInitializer_Registry
 *
 * @brief Global list that contains all portable interceptor ORB
 *        initializers.
 *
 * @note This class should be instantiated via its instance() method.
 *       Normally this would be enforced by making the constructor
 *       protected but that forces a friend declaration containing a
 *       template type (TAO_Singleton) with a static member to be
 *       introduced.  In turn, this potentially introduces problems in
 *       MS Windows DLL environments due to the occurance of multiple
 *       singleton instances.  There should only be one!
 */
class TAO_ORBInitializer_Registry
{
  friend void PortableInterceptor::register_orb_initializer (
                  PortableInterceptor::ORBInitializer_ptr init
                  ACE_ENV_ARG_DECL);
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA::Environment &);

public:

  /// Only allow this class to be instantiated as a singleton
  /// instance, so declare the constructor as protected.
  /**
   * @note See the note in the class description for an explanation of
   *       why this constructor is not protected.
   */
  TAO_ORBInitializer_Registry (void);

protected:

  /// Register an ORBInitializer with the underlying ORBInitializer
  /// array.
  void register_orb_initializer (
    PortableInterceptor::ORBInitializer_ptr init
    ACE_ENV_ARG_DECL);

  /// Begin initialization of all registered ORBInitializers before
  /// the ORB itself is initialized.
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                 ACE_ENV_ARG_DECL);

  /// Complete initialization of all registered ORBInitializers after
  /// the ORB has been initialized.
  void post_init (PortableInterceptor::ORBInitInfo_ptr info
                  ACE_ENV_ARG_DECL);

  /// Return a unique singleton instance.
  static TAO_ORBInitializer_Registry *instance (void);

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (
    TAO_ORBInitializer_Registry (const TAO_ORBInitializer_Registry &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORBInitializer_Registry &))

private:

  /// Dynamic array containing registered ORBInitializers.
  ACE_Array_Base<PortableInterceptor::ORBInitializer_var> initializers_;

};

#include "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_H */
