// -*- C++ -*-

//=============================================================================
/**
 *  @file   Environment.h
 *
 *  $Id$
 *
 * Declare the CORBA::Environment class.  Note that this header file
 * only requires a few forward declarations of CORBA classes, this
 * is *very* important because even the ORB needs to know about it;
 * make your changes with care.  It is also a good idea trying to
 * minimize cross dependencies between header files.
 *
 * @author Carlos O'Ryan <coryan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ENVIRONMENT_H
#define TAO_ENVIRONMENT_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/CORBA_methods.h"
#include "tao/orbconf.h"
#include "tao/Pseudo_VarOut_T.h"
#include "tao/default_environment.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

namespace CORBA
{
  class Exception;

  class Environment;
  typedef Environment *Environment_ptr;
  typedef TAO_Pseudo_Var_T<Environment> Environment_var;
  typedef TAO_Pseudo_Out_T<Environment> Environment_out;

  /**
   * @class Environment
   *
   * @brief Environment
   *
   * A CORBA::Environment is a way to automagically ensure that
   * exception data is freed -- the "var" class for Exceptions.  It
   * adds just a bit of convenience function support, helping
   * classify exceptions as well as reducing memory leakage.
   * The thread has a default environment to simplify porting
   * between platforms that support native C++ exceptions and those
   * that don't. This is a TSS resource (always), but with a twist:
   * if the user creates a new environment the old one is "pushed"
   * (actually the new one remembers it), eventually the new
   * environment destructor pops itself from the stack and we
   * recover the old environment.  This means that if the user
   * create a new environment and somebody calls a function using
   * the default one the exception will still be received in the
   * environment created by the user.  The only drawback is that
   * environments life time must nest properly, this shouldn't be a
   * problem because environments are usually created on the stack,
   * but, the spec allows their creation on the heap and/or as class
   * members; we need to investigate the tradeoffs and take a
   * decision.
   */
  class TAO_Export Environment
  {
  public:
    /// The default constructor.  The environment will hold no
    /// exceptions.
    Environment (void);

    /// Copy constructor.
    Environment (const Environment &ACE_TRY_ENV);

    /// Assingment.
    Environment &operator=(const Environment &ACE_TRY_ENV);

    /// Destructor, release the exception.
    ~Environment (void);

    /// Some static methods that need to be defined in every pseudo object
    static Environment * _duplicate (Environment *);
    static Environment * _nil (void);

    /// Return the contained CORBA::Exception.
    /**
     * CORBA::Environment retains ownership of the exception, this is
     * contrary to the normal memory management rules in the C++
     * mapping, but actually mandated by the specification:
     *
     * "C++ Language Mapping" (formal/00-01-02). Section 1.27
     * Environment (page 1-113)
     */
    CORBA::Exception* exception (void) const;

    /// Set the contained CORBA::Exception to <ex>
    /**
     * CORBA::Environment assumes ownership of the exception, this is
     * contrary to the normal memory management rules in the C++
     * mapping, but actually mandated by the specification:
     *
     * "C++ Language Mapping" (formal/00-01-02). Section 1.27
     * Environment (page 1-113)
     *
     */
    void exception (CORBA::Exception *ex);

    /// Return if the exception is a user exception or a system
    /// exception.
    int exception_type (void) const;

    /// return the repository ID for the exception.
    const char *exception_id (void) const;

    /// Clear the exception.
    void clear (void);

    /// Print the exception to output determined by f.  This function
    /// is not CORBA compliant.
    void print_exception (const char *info,
                          FILE *f=stdout) const;

    // = Obtain a default environment to use with TAO.
    static CORBA::Environment &default_environment (void);

    // Useful for template programming.
    typedef CORBA::Environment_ptr _ptr_type;
    typedef CORBA::Environment_var _var_type;
    typedef CORBA::Environment_out _out_type;

  private:

    /// Initialize using a well known ORB Core; this is intended for
    /// the bootstraping of the ORB_Core, not for general
    /// consumption.
    Environment (TAO_ORB_Core *orb_core);

    /// Pointer to the exception object contained in the environment.
    CORBA::Exception *exception_;

    /// The previous environment on the "default environment stack".
    Environment *previous_;
  };

  template<>
  inline void release (Environment_ptr env)
  {
    delete env;
  }
} // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (__ACE_INLINE__)
# include "tao/Environment.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ENVIRONMENT_H */
