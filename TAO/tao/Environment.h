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
//   Declare the CORBA_Environment class.
//   Note that this header file only requires a few forward
//   declarations of CORBA classes, this is *very* important because
//   even the ORB needs to know about it; make your changes with care.
//   It is also a good idea trying to minimize cross dependencies
//   between header files.
//
// = AUTHOR
//   Carlos O'Ryan
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
  //
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
  //   recover the old environment.
  //   This means that if the user create a new environment and
  //   somebody calls a function using the default one the exception
  //   will still be received in the environment created by the user.
  //   The only drawback is that environments life time must nest
  //   properly, this shouldn't be a problem because environments are
  //   usually created on the stack, but, the spec allows their
  //   creation on the heap and/or as class members; we need to
  //   investigate the tradeoffs and take a decision.
  //
public:
  // = Initialization and termination methods.

  CORBA_Environment (void);
  // The default constructor, the environment will hold no
  // exceptions.

  CORBA_Environment (const CORBA_Environment &env);
  // Copy constructor

  CORBA_Environment &operator=(const CORBA_Environment &env);
  // Assingment

  ~CORBA_Environment (void);
  // Destructor, release the exception.

  CORBA_Exception* exception (void) const;
  // Return the exception.  Caller must call _incr_refcnf() in order
  // to keep the ptr.

  void exception (CORBA_Exception *ex);
  // Set the exception to <ex>, taking a reference on it.

  int exception_type (void) const;
  // Return if the exception is a user exception or a system
  // exception.

  const char* exception_id (void) const;
  // return the repository ID for the exception.

  void clear (void);
  // Clear the exception.

  void print_exception (const char *info,
                        FILE *f=stdout) const;
  // Print the exception to output determined by f.
  // This function is not CORBA compliant.

  // = Obtain a default environment to use with TAO.
  static CORBA_Environment &default_environment (void);

#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Environment_ptr _ptr_type;
#endif /* __GNUC__ */
  // Useful for template programming.

private:

  friend class TAO_ORB_Core;
  CORBA_Environment (TAO_ORB_Core *orb_core);
  // Initialize using a well known ORB Core; this is intended for the
  // bootstraping of the ORB_Core, not for general consumption.

  CORBA_Exception* exception_;
  // Pointer to the exception object contained in the environment.

  CORBA_Environment* previous_;
  // The previous environment on the "default environment stack".

};

#if defined (__ACE_INLINE__)
# include "tao/Environment.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ENVIRONMENT_H */
