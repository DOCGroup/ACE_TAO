/* -*- C++ -*- */
/**
 *  @file Destroy_Callback.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DESTROY_CALLBACK_H
#define TAO_NS_DESTROY_CALLBACK_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_NS_Destroy_Callback
 *
 * @brief Interface to be implemented by users of TAO_NS_Refcountable
 *
 */
class TAO_Notify_Export TAO_NS_Destroy_Callback
{
public:

  /// Called when Refcountable count drops to 0
  virtual void release (void) = 0;
};


#include "ace/post.h"
#endif /* TAO_NS_DESTROY_CALLBACK_H */
