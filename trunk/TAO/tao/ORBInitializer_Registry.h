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

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "TAO_Singleton.h"
#include "ace/Array_Base.h"

/**
 * @class TAO_ORBInitializer_Registry
 *
 * @brief Global list that contains all portable interceptor ORB
 *        initializers.
 */
class TAO_Export TAO_ORBInitializer_Registry
{
  friend class TAO_Singleton<TAO_ORBInitializer_Registry, TAO_SYNCH_MUTEX>;
  friend void PortableInterceptor::register_orb_initializer (
                  PortableInterceptor::ORBInitializer_ptr init
                                  ACE_ENV_ARG_DECL_NOT_USED);
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA_Environment &);

protected:

  /// Only allow this class to be instantiated as a singleton
  /// instance, so declare the constructor as protected.
  TAO_ORBInitializer_Registry (void);

  /// Destructor.  Releases duplicated ORBInitializer references.
  ~TAO_ORBInitializer_Registry (void);

  ///< Register an ORBInitializer with the underlying ORBInitializer
  ///< array.
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
  ACE_Array_Base<PortableInterceptor::ORBInitializer_ptr> initializers_;

};

#if defined (__ACE_INLINE__)
# include "tao/ORBInitializer_Registry.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_H */
