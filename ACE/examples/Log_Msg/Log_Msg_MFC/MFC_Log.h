/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MFC_Log.h
 *
 *  @author Craig Rodrigues <crodrigu@bbn.com>
 *  @author Chris Hafey <chafey@stentor.com>
 *  @author Don Hinton <dhinton@ieee.org>
 */
//=============================================================================

#ifndef MFC_LOG_H
#define MFC_LOG_H

#include "ace/Log_Msg_Callback.h"

class ACE_Log_Record;

/**
 * @class MFC_Log
 *
 * @brief A callback class for ACE_Log_Msg which sends output to
 * Microsoft debugger
 *
 * Register this class as a MSG_CALLBACK with the ACE_Log_Msg singleton and
 * send debug output to the Microsoft debugger console window with
 * ACE_DEBUG macros.
 */
class MFC_Log  :  public virtual ACE_Log_Msg_Callback
{
public:
  MFC_Log () = default;

  /// method called by ACE_Log_Msg to log an event
  void log (ACE_Log_Record &log_record) override;

  ~MFC_Log () override = default;

  MFC_Log (MFC_Log const &) = delete;
  MFC_Log (MFC_Log &&) = delete;
  MFC_Log &operator= (MFC_Log const &) = delete;
  MFC_Log &operator= (MFC_Log &&) = delete;
};

#endif /* MFC_LOG_H */

