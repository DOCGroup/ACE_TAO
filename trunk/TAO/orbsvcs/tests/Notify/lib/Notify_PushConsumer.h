//=============================================================================
/**
 *  @file   Notify_PushConsumer.h
 *
 *  $Id$
 *
 * This is a utility class is to be used by clients of the Notification
 * Service to implement the servant for CosNotifyComm::PushConsumer
 *
 * = HOW-TO
 * 1. Derive from Notify_ProxyConsumer and implement <push>.
 * 2. Create the servant on the heap.(refcounted servant).
 * 3. Initialize the object, provide a POA.
 * 4. To connect, call <connect>, this will activate the object in the POA.
 *     You must provide a ConsumerAdmin to <connect>.
 * 5. To disconnect, call <disconnect> this will also deactivate the object.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_PUSHCONSUMER_H
#define TAO_NOTIFY_PUSHCONSUMER_H
#include /**/ "ace/pre.h"

#include "PushConsumer.h"

typedef TAO_Notify_Tests_PushConsumer TAO_Notify_PushConsumer;

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_PUSHCONSUMER_H */
