/* -*- C++ -*- */
/**
 *  @file RTCORBA_Builder.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_BUILDER_H
#define TAO_NS_RTCORBA_BUILDER_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Builder.h"

/**
 * @class TAO_NS_RTCORBA_Builder
 *
 * @brief Builder for RTCORBA specific classes.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_Builder : public TAO_NS_Builder
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_Builder (void);

  /// Destructor
  ~TAO_NS_RTCORBA_Builder ();  
};

#if defined (__ACE_INLINE__)
#include "RTCORBA_Builder.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_BUILDER_H */
