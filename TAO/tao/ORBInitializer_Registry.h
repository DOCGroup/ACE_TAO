// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ORBInitializer_Registry.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_ORB_INITIALIZER_REGISTRY_H
#define TAO_ORB_INITIALIZER_REGISTRY_H

#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "tao/TAO_Singleton.h"
#include "ace/Containers_T.h"


/// Global list that contains all portable interceptor ORB
/// initializers.
class TAO_Export TAO_ORBInitializer_Registry
{
  friend class TAO_Singleton<TAO_ORBInitializer_Registry, ACE_SYNCH_MUTEX>;
  friend void PortableInterceptor::register_orb_initializer (
                  PortableInterceptor::ORBInitializer_ptr init,                
				  CORBA::Environment &);
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA_Environment &);

protected:

  TAO_ORBInitializer_Registry (void);
  ///< Only allow this class to be instantiated as a singleton
  ///< instance, so declare the constructor as protected.

  ~TAO_ORBInitializer_Registry (void);
  ///< Destructor.  Releases duplicated ORBInitializer references.

  void register_orb_initializer (
    PortableInterceptor::ORBInitializer_ptr init,
    CORBA::Environment &ACE_TRY_ENV);
  ///< Register an ORBInitializer with the underlying ORBInitializer
  ///< sequence.

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info,
                 CORBA::Environment &ACE_TRY_ENV);
  ///< Begin initialization of all registered ORBInitializers before
  ///< the ORB itself is initialized.

  void post_init (PortableInterceptor::ORBInitInfo_ptr info,
                  CORBA::Environment &ACE_TRY_ENV);
  ///< Complete initialization of all registered ORBInitializers after
  ///< the ORB has been initialized.

  static TAO_ORBInitializer_Registry *instance (void);
  // Return a unique instance

private:

  /// Prevent copying
  ACE_UNIMPLEMENTED_FUNC (
    TAO_ORBInitializer_Registry (const TAO_ORBInitializer_Registry &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORBInitializer_Registry &))

private:

  ACE_Array_Base<PortableInterceptor::ORBInitializer_ptr> initializers_;
  ///< Dynamic array containing registered ORBInitializers.

};

#if defined (__ACE_INLINE__)
# include "tao/ORBInitializer_Registry.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_H */
