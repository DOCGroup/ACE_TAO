// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Environment.h
//
// = DESCRIPTION
//   Declare the CORBA_Environment class.  Note that this header file
//   only requires a few forward declarations of CORBA classes, this
//   is *very* important because even the ORB needs to know about it;
//   make your changes with care.  It is also a good idea trying to
//   minimize cross dependencies between header files.
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENVIRONMENT_H
#define TAO_ENVIRONMENT_H

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;

class TAO_Export CORBA_Environment
{
  // = TITLE
  //   CORBA_Environment
  //
  // = DESCRIPTION
  //   A CORBA_Environment is a way to automagically ensure that
  //   exception data is freed -- the "var" class for Exceptions.  It
  //   adds just a bit of convenience function support, helping
  //   classify exceptions as well as reducing memory leakage.
  //
  //   The thread has a default environment to simplify porting
  //   between platforms that support native C++ exceptions and those
  //   that don't. This is a TSS resource (always), but with a twist:
  //   if the user creates a new environment the old one is "pushed"
  //   (actually the new one remembers it), eventually the new
  //   environment destructor pops itself from the stack and we
  //   recover the old environment.  This means that if the user
  //   create a new environment and somebody calls a function using
  //   the default one the exception will still be received in the
  //   environment created by the user.  The only drawback is that
  //   environments life time must nest properly, this shouldn't be a
  //   problem because environments are usually created on the stack,
  //   but, the spec allows their creation on the heap and/or as class
  //   members; we need to investigate the tradeoffs and take a
  //   decision.
public:
  // = Initialization and termination methods.
  CORBA_Environment (void);
  // The default constructor, the environment will hold no exceptions.

  CORBA_Environment (const CORBA_Environment &env);
  // Copy constructor.

  CORBA_Environment &operator=(const CORBA_Environment &env);
  // Assingment.

  ~CORBA_Environment (void);
  // Destructor, release the exception.

  CORBA_Exception* exception (void) const;
  // Return the exception.  Caller must call <_incr_refcnf> in order
  // to keep the ptr.

  void exception (CORBA_Exception *ex);
  // Set the exception to <ex>, taking a reference on it.

  int exception_type (void) const;
  // Return if the exception is a user exception or a system
  // exception.

  const char *exception_id (void) const;
  // return the repository ID for the exception.

  void clear (void);
  // Clear the exception.

  void print_exception (const char *info,
                        FILE *f=stdout) const;
  // Print the exception to output determined by f.  This function is
  // not CORBA compliant.

  // = Obtain a default environment to use with TAO.
  static CORBA_Environment &default_environment (void);

  // = Reference counting.
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Environment_ptr _ptr_type;
  typedef CORBA_Environment_var _var_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:

  friend class TAO_ORB_Core;
  CORBA_Environment (TAO_ORB_Core *orb_core);
  // Initialize using a well known ORB Core; this is intended for the
  // bootstraping of the ORB_Core, not for general consumption.

  CORBA_Exception *exception_;
  // Pointer to the exception object contained in the environment.

  CORBA_Environment *previous_;
  // The previous environment on the "default environment stack".

  CORBA::ULong refcount_;
  // Reference counting.

  ACE_SYNCH_MUTEX refcount_lock_;
  // Protect the reference count.
};

class TAO_Export CORBA_Environment_var
{
  // = TITLE
  //   CORBA_Environment_var
  //
  // = DESCRIPTION
  //   Provide for automatic storage deallocation on going out of
  //   scope.
public:
  CORBA_Environment_var (void);
  // default constructor
  CORBA_Environment_var (CORBA_Environment_ptr);
  CORBA_Environment_var (const CORBA_Environment_var &);
  // copy constructor
  ~CORBA_Environment_var (void);
  // destructor

  CORBA_Environment_var &operator= (CORBA_Environment_ptr);
  CORBA_Environment_var &operator= (const CORBA_Environment_var &);
  CORBA_Environment_ptr operator-> (void) const;

  operator const CORBA_Environment_ptr &() const;
  operator CORBA_Environment_ptr &();
  // in, inout, out, _retn
  CORBA_Environment_ptr in (void) const;
  CORBA_Environment_ptr &inout (void);
  CORBA_Environment_ptr &out (void);
  CORBA_Environment_ptr _retn (void);
  CORBA_Environment_ptr ptr (void) const;

private:
  CORBA_Environment_ptr ptr_;
};

class TAO_Export CORBA_Environment_out
{
  // = TITLE
  //   CORBA_Environment_out
  //
  // = DESCRIPTION
  //   The _out class for CORBA_Environment. This is used to help in
  //   managing the out parameters.
public:
  CORBA_Environment_out (CORBA_Environment_ptr &);
  CORBA_Environment_out (CORBA_Environment_var &);
  CORBA_Environment_out (const CORBA_Environment_out &);
  CORBA_Environment_out &operator= (const CORBA_Environment_out &);
  CORBA_Environment_out &operator= (CORBA_Environment_ptr);
  operator CORBA_Environment_ptr &();
  CORBA_Environment_ptr &ptr (void);
  CORBA_Environment_ptr operator-> (void);

private:
  CORBA_Environment_ptr &ptr_;

  CORBA_Environment_out &operator= (const CORBA_Environment_var &);
  // Assignment from _var not allowed.
};

#if defined (__ACE_INLINE__)
# include "tao/Environment.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ENVIRONMENT_H */
