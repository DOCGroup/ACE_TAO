/* -*- C++ -*- */
/**
 *  @file CosNotify_Initializer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_COSNOTIFY_INITIALIZER_H
#define TAO_NS_COSNOTIFY_INITIALIZER_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_NS_CosNotify_Initializer
 *
 * @brief  Helper to load the Cos Notification service into the service conf. for static links.
 *
 */
class TAO_Notify_Export TAO_NS_CosNotify_Initializer
{
public:
  /// Constuctor
  TAO_NS_CosNotify_Initializer (void);
};

static TAO_NS_CosNotify_Initializer TAO_NS_CosNotify_initializer;

#include "ace/post.h"
#endif /* TAO_NS_COSNOTIFY_INITIALIZER_H */
