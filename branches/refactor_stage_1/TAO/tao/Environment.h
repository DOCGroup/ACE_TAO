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
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <stdarg.h> // LynxOS requires this before stdio.h
#include <stdio.h>  // Pull in "stdout" definition.

class TAO_ORB_Core;

namespace CORBA
{
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

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
    // Useful for template programming.
    typedef CORBA::Environment_ptr _ptr_type;
    typedef CORBA::Environment_var _var_type;
#endif /* __GNUC__ */

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

  /**
   * @class Environment_var
   *
   * @brief Environment_var
   *
   * Provide for automatic storage deallocation on going out of
   * scope.
   */
  class TAO_Export Environment_var
  {
  public:
    /**
     * default constructor
     * copy constructor
     * destructor
     */
    Environment_var (void);
    Environment_var (CORBA::Environment_ptr);
    Environment_var (const Environment_var &);
    ~Environment_var (void);

    Environment_var &operator= (CORBA::Environment_ptr);
    Environment_var &operator= (const Environment_var &);
    CORBA::Environment_ptr operator-> (void) const;

    /// in, inout, out, _retn
    operator const CORBA::Environment_ptr &() const;
    operator CORBA::Environment_ptr &();
    CORBA::Environment_ptr in (void) const;
    CORBA::Environment_ptr &inout (void);
    CORBA::Environment_ptr &out (void);
    CORBA::Environment_ptr _retn (void);
    CORBA::Environment_ptr ptr (void) const;

  private:
    CORBA::Environment_ptr ptr_;
  };

  /**
   * @class Environment_out
   *
   * @brief Environment_out
   *
   * The _out class for CORBA::Environment.  This is used to help in
   * managing the out parameters.
   */
  class TAO_Export Environment_out
  {
  public:
    Environment_out (Environment_ptr &);
    Environment_out (Environment_var &);
    Environment_out (const Environment_out &);
    Environment_out &operator= (const Environment_out &);
    Environment_out &operator= (CORBA::Environment_ptr);
    operator CORBA::Environment_ptr &();
    CORBA::Environment_ptr &ptr (void);
    CORBA::Environment_ptr operator-> (void);

  private:
    Environment_ptr &ptr_;

    /// Assignment from _var not allowed.
    Environment_out &operator= (const CORBA::Environment_var &);
  };
} // End CORBA namespace

#if defined (__ACE_INLINE__)
# include "tao/Environment.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ENVIRONMENT_H */
