/* -*- C++ -*- */
/**
 *  @file Direct_Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_DIRECT_CONSUMER_H
#define TAO_NS_DIRECT_CONSUMER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Periodic_Consumer.h"

/**
 * @class TAO_NS_Direct_Consumer
 *
 * @brief Does not connect to the Notification Service. The Direct Supplier supplies events directly.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Direct_Consumer : public TAO_NS_Periodic_Consumer
{
public:
  /// Constuctor
  TAO_NS_Direct_Consumer (void);

  /// Destructor
  ~TAO_NS_Direct_Consumer ();

  /// Connect using options parsed and set initial QoS.
  virtual void connect (ACE_ENV_SINGLE_ARG_DECL);
};

#include "ace/post.h"
#endif /* TAO_NS_DIRECT_CONSUMER_H */
