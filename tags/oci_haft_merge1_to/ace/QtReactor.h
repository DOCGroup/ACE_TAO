/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    QtReactor.h
 *
 *  $Id$
 *
 *  @author Hamish Friedlander <ullexco@wave.co.nz>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_QTREACTOR_H
#define ACE_QTREACTOR_H
#include "ace/pre.h"


#include "ace/Select_Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_QT)
#include "ace/Map_Manager.h"

// Qttoolkit specific includes.
#include <qapplication.h>
#include <qobject.h>
#include <qsocketnotifier.h>
#include <qtimer.h>

/**
 * @class ACE_QtReactor
 *
 * @brief An object-oriented event demultiplexor and event handler
 * dispatcher that uses the Qt Library. This class declaration
 * also uses the extension facilities  provided by the Qt. So,
 * readers  of the class declaration should not be upset with
 * the appearence of the Keywords like Q_OBJECT, private slots
 * etc. They are specific to Qt which uses these as a call back
 * methods implementation mechanism.
 */
class ACE_Export ACE_QtReactor : public QObject, public ACE_Select_Reactor
{

  Q_OBJECT

 public:
  // = Initialization and termination methods.
  ACE_QtReactor (QApplication *qapp  = NULL,
                 size_t size = DEFAULT_SIZE,
                 int restart = 0,
                 ACE_Sig_Handler *handler = 0);

  virtual ~ACE_QtReactor (void);

  void qapplication (QApplication *qapp);

  // = Timer operations.
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const void *arg,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval);

  virtual int  cancel_timer (ACE_Event_Handler *handler,
                               int dont_call_handle_close = 1);

  virtual int  cancel_timer (long timer_id,
                             const void **arg = 0,
                             int dont_call_handle_close = 1);

 protected:

   // = Register timers/handles with Qt

  /// Register a single <handler>.
  virtual int register_handler_i (ACE_HANDLE handle,
                                  ACE_Event_Handler *handler,
                                  ACE_Reactor_Mask mask);

  /// Register a set of <handlers> with Qt.
  virtual int register_handler_i (const ACE_Handle_Set &handles,
                                  ACE_Event_Handler *handler,
                                  ACE_Reactor_Mask mask);


  /// Remove the <handler> associated with this <handle>.
  virtual int remove_handler_i (ACE_HANDLE handle,
                                ACE_Reactor_Mask mask);

  /// Remove a set of <handles>.
  virtual int remove_handler_i (const ACE_Handle_Set &handles,
                                ACE_Reactor_Mask mask);

  /// Wait for events to occur.
  virtual int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &handle_set,
                                        ACE_Time_Value *max_wait_time);

  virtual int QtWaitForMultipleEvents (int width,
                                       ACE_Select_Reactor_Handle_Set &wait_set,
                                       ACE_Time_Value *max_wait_time);

  virtual int bit_ops (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Select_Reactor_Handle_Set &handle_set,
                       int ops);

  int set_enable_flag_by_mask (int flag_value, ACE_HANDLE handle, ACE_Reactor_Mask mask);
  void create_notifiers_for_handle (ACE_HANDLE handle);
  void destroy_notifiers_for_handle (ACE_HANDLE handle);

  // Wait for Qt events to occur

  /// Some Qt stuff that we need to have
  QApplication *qapp_ ;

  /// Typedef of a map.
  typedef ACE_Map_Manager<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex> MAP;

  /// A notifier for a read
  MAP read_notifier_;

  /// A write notifier
  MAP write_notifier_;

  /// An exception notifier
  MAP exception_notifier_;

  /// The timer class that would provide timer-sgnals & single-shot timers
  QTimer *qtime_ ;

 private:
  /// This method ensures there's an Qt timeout for the first timeout
  /// in the Reactor's Timer_Queue.
  void reset_timeout (void);

  /// Deny access since member-wise won't work...
  ACE_QtReactor (const ACE_QtReactor &);
  ACE_QtReactor &operator= (const ACE_QtReactor &);

 private slots:

    // These are all part of the communication mechanism adopted in Qt.
  /// Dispatch a Read Event
  void read_event (int FD);

  /// Dispatch a Write Event
  void write_event (int FD);

  /// Dispatch an exception event
  void exception_event (int FD);

  /// Dispach a timeout event
  void timeout_event (void);
};

#endif /*ACE_HAS_QT */

#include "ace/post.h"
#endif /* ACE_QTREACTOR_H */
