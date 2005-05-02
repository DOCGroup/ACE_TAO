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

#include /**/ "ace/pre.h"
#include "ace/Array_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CORBA_methods.h"
#include "Objref_VarOut_T.h"
#include "Environment.h"
#include "default_environment.h"

#include "ace/Thread_Mutex.h"

namespace PortableInterceptor
{
  class ORBInitializer;
  typedef ORBInitializer *ORBInitializer_ptr;

  typedef
    TAO_Objref_Var_T<
        ORBInitializer
      >
    ORBInitializer_var;

  class ORBInitInfo;
  typedef ORBInitInfo *ORBInitInfo_ptr;

  /// Register an ORBInitializer with the global ORBInitializer
  /// table.
  TAO_Export void register_orb_initializer (
      ORBInitializer_ptr init
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );
}

namespace TAO
{
  /**
   * @class ORBInitializer_Registry
   *
   * @brief Global list that contains all portable interceptor ORB
   *        initializers.
   *
   * @note This class should be instantiated via its instance()
   *       method.  Normally this would be enforced by making the
   *       constructor protected but that forces a friend declaration
   *       containing a template type (TAO_Singleton) with a static
   *       member to be introduced.  In turn, this potentially
   *       introduces problems in MS Windows DLL environments due to
   *       the occurance of multiple singleton instances.  There
   *       should only be one!
   */
  class ORBInitializer_Registry
  {
#if defined (ACE_LYNXOS_MAJOR) && (ACE_LYNXOS_MAJOR < 4)
    // Workaround for g++ (version 2.9-gnupro-98r2) on LynxOS 3.x which has
    // problems with friend declarations in namespaces 
    friend void register_orb_initializer (
        PortableInterceptor::ORBInitializer_ptr init
        ACE_ENV_ARG_DECL);

    friend CORBA::ORB_ptr ORB_init (int &,
                                    char *argv[],
                                    const char *,
                                    CORBA::Environment &);
#else
    friend void PortableInterceptor::register_orb_initializer (
                  PortableInterceptor::ORBInitializer_ptr init
                  ACE_ENV_ARG_DECL);

    friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                           char *argv[],
                                           const char *,
                                           CORBA::Environment &);
#endif /* (ACE_LYNXOS_MAJOR) && (ACE_LYNXOS_MAJOR < 4) */

  public:

    /// Only allow this class to be instantiated as a singleton
    /// instance, so declare the constructor as protected.
    /**
     * @note See the note in the class description for an explanation of
     *       why this constructor is not protected.
     */
    ORBInitializer_Registry (void);

  protected:

    /// Register an ORBInitializer with the underlying ORBInitializer
    /// array.
    void register_orb_initializer (PortableInterceptor::ORBInitializer_ptr init
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
    static ORBInitializer_Registry * instance (void);

  private:
    // Prevent copying
    ORBInitializer_Registry (const ORBInitializer_Registry &);
    void operator= (const ORBInitializer_Registry &);

  private:

    TAO_SYNCH_MUTEX lock_;

    /// Dynamic array containing registered ORBInitializers.
    ACE_Array_Base<PortableInterceptor::ORBInitializer_var> initializers_;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ORB_INITIALIZER_REGISTRY_H */
