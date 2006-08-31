// -*- C++ -*-

//=============================================================================
/**
 * @file        EventHandler.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_EVENT_HANDLER_H_
#define _CUTS_EVENT_HANDLER_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/SStringfwd.h"

//=============================================================================
/**
 * @class CUTS_Event_Handler
 *
 * Abstract interface for event handler types. This interface is
 * mainly used to provide consistency between event handler
 * implementations and managers of event handlers.
 */
//=============================================================================

class CUTS_Export CUTS_Event_Handler
{
public:
  /// Event handling mode.
  enum Event_Mode
  {
    /// Undefined mode.
    UNDEFINED     = 0,

    /// Synchronous mode.
    SYNCHRONOUS   = 1,

    /// Asynchronous mode.
    ASYNCHRONOUS  = 2
  };

  /// Set the priority.
  virtual void priority (int prio) = 0;

  /// Get the priority.
  virtual int priority (void) const = 0;

  /// Set the number of threads.
  virtual void thread_count (size_t n) = 0;

  /// Get the number of threads.
  virtual size_t thread_count (void) const = 0;

  /// Get the name of the event handler.
  virtual const char * name (void) const = 0;

protected:
  /// Default constructor.
  CUTS_Event_Handler (void);

  /// Destructor.
  virtual ~CUTS_Event_Handler (void);

private:
  // prevent the following operations
  CUTS_Event_Handler (const CUTS_Event_Handler &);
  const CUTS_Event_Handler & operator = (const CUTS_Event_Handler &);
};

#if defined (__CUTS_INLINE__)
#include "cuts/Event_Handler.inl"
#endif

#endif  // !defined _CUTS_EVENT_HANDLER_H_
