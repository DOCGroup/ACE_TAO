// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_Compaction_Handler.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_TLS_COMPACTION_HANDLER_H
#define TAO_TLS_COMPACTION_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "log_serv_export.h"

// forward decls
class ACE_Reactor;
class TAO_Log_i;

/// @class TAO_Log_Compaction_Handler
/// @brief Periodically invoke remove_old_records() on Log
///
class TAO_Log_Serv_Export TAO_Log_Compaction_Handler
  : public ACE_Event_Handler
{
public:
  /// Constructor.
  TAO_Log_Compaction_Handler (ACE_Reactor* reactor,
                              TAO_Log_i* log,
                              const ACE_Time_Value& interval);

  /// Destructor.
  ~TAO_Log_Compaction_Handler ();

  /// Schedule Timer.
  void schedule ();

  /// Cancel Timer.
  void cancel ();

  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);

private:
  long                          timer_id_;
  ACE_Reactor*                  reactor_;
  TAO_Log_i*                    log_;
  const ACE_Time_Value          interval_;
};

#endif  /* TAO_LOG_COMPACTION_HANDLER_H */
