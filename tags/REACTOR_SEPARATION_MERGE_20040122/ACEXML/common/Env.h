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

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Macros.h"
#include "ACEXML/common/Exception.h"

/**
 * @class ACEXML_Env Env.h "ACEXML/common/Env.h"
 *
 * @brief ACEXML_Env
 *
 * ACEXML_Env is modeled after CORBA_Environment class. It provides a place
 * holder for callees to propagate exceptions back to callers. ACEXML does
 * not support native exceptions at this moment.
 *
 * @sa ACEXML_Exception
 */
class ACEXML_Export ACEXML_Env
{
public:
  /// Default constructor.
  ACEXML_Env (void);

  /// Copy constructor.
  ACEXML_Env (const ACEXML_Env& ACEXML_TRY_ENV);

  /// Assignment
  ACEXML_Env& operator= (const ACEXML_Env& ACEXML_TRY_ENV);

  /// Destructor.
  ~ACEXML_Env (void);

  /// Return the contained exception.
  ACEXML_Exception *exception (void) const;

  /// Set the contained exception to @arg ex
  void exception (ACEXML_Exception* ex);

  /// Clear the exception and reset the evnrionment.
  void clear (void);

private:
  /// Place holder for exception (if one occurs.)
  ACEXML_Exception *exception_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/Env.i"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_ENV_H_ */
