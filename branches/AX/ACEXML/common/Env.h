// -*- C++ -*-

//=============================================================================
/**
 *  @file    Env.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_ENV_H_
#define _ACEXML_ENV_H_
#include "common/Exception.h"

/**
 * @class ACEXML_Env Env.h "common/Env.h"
 *
 * @brief ACEXML_Env
 *
 * ACEXML_Env is modeled after CORBA_Environment class.  It provides
 * a place holder for callees to propagate exceptions back to
 * callers.  ACEXML does not support native exceptions at this
 * moement.
 *
 * @sa ACEXML_Exception
 */
class ACEXML_Export ACEXML_Env
{
public:
  /// Default constructor.
  ACEXML_Env ();

  /// Copy constructor.
  ACEXML_Env (const ACEXML_Env &ev);

  /// Destructor.
  ~ACEXML_Env (void);

  /// Check if exception has occured.
  int exception_occured (void) const;

  /// Clear the exception and reset the evnrionment.
  void reset (void);

  /// Return the underlying exception for examination
  ACEXML_Exception *exception (void) const;

  /// Set the underlying exception.  ACEXML_Env assumes
  /// ownership of the exception.
  void exception (ACEXML_Exception *exception);

private:
  /// Place holder for exception (if one occurs.)
  ACEXML_Exception *exception_;
};

// = These macros are used to speed up programming.  I hope to
// merge them with ACE_CHECK macros someday.
#define ACEXML_CHECK if (xmlenv.exception ()) return;
#define ACEXML_CHECK_RETURN(VAL) if (xmlenv.exception ()) return VAL;

#if defined (__ACEXML_INLINE__)
# include "common/Env.i"
#endif /* __ACEXML_INLINE__ */
#endif /* _ACEXML_ENV_H_ */
