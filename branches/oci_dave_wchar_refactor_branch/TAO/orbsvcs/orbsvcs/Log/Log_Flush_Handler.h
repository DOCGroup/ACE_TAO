// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_Flush_Handler.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_TLS_FLUSH_HANDLER_H
#define TAO_TLS_FLUSH_HANDLER_H

#include "orbsvcs/Log/log_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Log_i;

/// @class TAO_Log_Flush_Handler
/// @brief Periodically flush log records to persistent store
///
class TAO_Log_Serv_Export TAO_Log_Flush_Handler
  : public ACE_Event_Handler
{
public:
  /// Constructor.
  TAO_Log_Flush_Handler (ACE_Reactor* reactor,
                         TAO_Log_i* log,
                         const ACE_Time_Value& interval);

  /// Destructor.
  ~TAO_Log_Flush_Handler ();

  /// Schedule Timer.
  void schedule ();

  /// Cancel Timer.
  void cancel ();

  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);

private:
  long                          timer_id_;
  ACE_Reactor*                  reactor_;
  TAO_Log_i*                    log_;
  ACE_Time_Value                interval_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_LOG_FLUSH_HANDLER_H */
