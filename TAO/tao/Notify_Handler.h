// -*- C++ -*-

//=============================================================================
/**
 *  @file Notify_Handler.h
 *
 *  $Id$
 *
 *  @author Balchanadran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_HANDLER_H
#define TAO_NOTIFY_HANDLER_H
#include "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

class TAO_Transport;
class ACE_Allocator;
class TAO_Connection_Handler;

/**
 * @class TAO_Notify_Handler
 *
 * @brief Represents the handler used by the notify calls to the
 *  reactor within the ORB.
 *
 * Cluttering the *IOP handlers with the job of handling notify calls
 * introduces problems as mentioned in [BUG 1230]. This special
 * handler can help to avoid that problem by splitting the
 * responsibilities between actual connection handling and notify
 * handling that is done underneath the ORB.
 */
class TAO_Export TAO_Notify_Handler: public ACE_Event_Handler
{
public:

  /// Dtor
  ~TAO_Notify_Handler (void);

  /// NOTE: Needs to be deprecated. Just here for backward
  /// compatibility
  static TAO_Notify_Handler *create_handler (TAO_Connection_Handler *ch,
                                             ACE_Allocator *alloc);

  /// Static method to create an instance of this object in the
  /// memory pool
  static TAO_Notify_Handler *create_handler (TAO_Transport *t,
                                             ACE_HANDLE h,
                                             ACE_Allocator *alloc);

  /// Static method to destroy an instance of this object
  static void destroy_handler (TAO_Notify_Handler *nh);

  /// The standard handle_input method, it just redirects to the
  /// connection handler
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd,
                            ACE_Reactor_Mask close_mask);

protected:

  /// NOTE: Needs to be deprecated.
  TAO_Notify_Handler (TAO_Connection_Handler *ch,
                      ACE_Allocator *alloc);

  /// Ctor
  TAO_Notify_Handler (TAO_Transport *t,
                      ACE_HANDLE h,
                      ACE_Allocator *alloc);

private:

  /// Preventing default ctor's.
  ACE_UNIMPLEMENTED_FUNC (TAO_Notify_Handler (void))

private:
  /// Our copy of the TAO_TRansport, reference count incremented
  /// and stored.
  TAO_Transport *t_;

  /// The handle that we should be concerened with
  ACE_HANDLE h_;

  /// Our allocator
  ACE_Allocator *allocator_;
};


#include "ace/post.h"
#endif /*TAO_NOTIFY_HANDLER_H*/
