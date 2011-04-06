// -*- C++ -*-

//=============================================================================
/**
 *  @file Resume_Handle_Deferred.h
 *
 *  $Id$
 *
 *  @author Iliyan Jeliazkov <jeliazkov_i@ociweb.com>
 */
//=============================================================================

#ifndef TAO_RESUME_HANDLE_DEFERRED_H
#define TAO_RESUME_HANDLE_DEFERRED_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;


/**
 * @class TAO_Resume_Handle_Deferred
 *
 * @brief A utility class that helps in resuming handlers if TAO uses
 *  a TP Reactor from ACE. In contrast with the Resume_Handle class
 *  however, the resumption is performed at the expiration of a timer,
 *  previously registered with the reactor. TAO_Resume_Handle_Deferred
 *  will also notify the reactor (reactor->notify())
 *
 * Please read the documentation in the bugzilla #575 in the bugzilla
 * database what we mean by handler resumption.
 */
class TAO_Export TAO_Resume_Handle_Deferred : public ACE_Event_Handler
{
  public:
    /// Ctor
    TAO_Resume_Handle_Deferred (TAO_ORB_Core* orbc, ACE_HANDLE h);

    /// Destructor
    ~TAO_Resume_Handle_Deferred (void);

    /// Handles the timeout event by resuming the handle and
    /// calling the reactor->notify()
    virtual int handle_timeout (const ACE_Time_Value &, const void * = 0);

    /// Disposes off this instance
    virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  private:
    /// Implements the timeout handling
    int handle_timeout_eh ();

  private:
    ACE_HANDLE h_;
    TAO_ORB_Core * orbc_;
};

#if defined (__ACE_INLINE__)
# include "Resume_Handle_Deferred.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_RESUME_HANDLE_DEFERRED_H*/
