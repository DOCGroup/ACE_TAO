#include "ace_pch.h"
//$Id$

#include "ace/QtReactor.h"

#if defined (ACE_HAS_QT)

ACE_ALLOC_HOOK_DEFINE (ACE_QtReactor)

// Must be called with lock held
ACE_QtReactor::ACE_QtReactor (QApplication *qapp,
                              size_t size,
                              int restart,
                              ACE_Sig_Handler *handler)
  : ACE_Select_Reactor(size, restart, handler),
    qapp_(qapp),
    qtime_ (0)
  
{
  // When the ACE_Select_Reactor is constructed it creates the notify
  // pipe and registers it with the register_handler_i() method. The
  // QtReactor overloads this method BUT because the
  // register_handler_i occurs when constructing the base class
  // ACE_Select_Reactor, the ACE_Select_Reactor register_handler_i()
  // is called not the QtReactor register_handler_i().  This means
  // that the notify pipe is registered with the ACE_Select_Reactor
  // event handling code not the QtReactor and so notfications don't
  // work.  To get around this we simply close and re-opened the
  // notification handler in the constructor of the QtReactor.

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  this->notify_handler_->close ();

  // Patch for MS Windows: close and open doesn't clear the read
  // fd_set, so reset it manually
  this->wait_set_.rd_mask_.reset ();

  this->notify_handler_->open (this, 0);
#endif /* ACE_MT_SAFE */
}

ACE_QtReactor::~ACE_QtReactor (void)
{
  //no-op
}

void 
ACE_QtReactor::qapplication (QApplication *qapp)
{
  qapp_ = qapp ;
}

int 
ACE_QtReactor::wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &handle_set,
                                         ACE_Time_Value *max_wait_time)
{
  ACE_TRACE( "ACE_QtReactor::wait_for_multiple_events" );

  int nfound = 0; 
  do 
  {
    max_wait_time = this->timer_queue_->calculate_timeout (max_wait_time);
    size_t width = this->handler_rep_.max_handlep1 ();
    handle_set.rd_mask_ = this->wait_set_.rd_mask_;
    handle_set.wr_mask_ = this->wait_set_.wr_mask_;
    handle_set.ex_mask_ = this->wait_set_.ex_mask_;
    
    nfound = QtWaitForMultipleEvents (width, 
                                      handle_set, 
                                      max_wait_time);
    
  } while( nfound == -1 && this->handle_error () > 0 );

  if (nfound > 0)
  {
#if !defined (ACE_WIN32)
    handle_set.rd_mask_.sync (this->handler_rep_.max_handlep1 ());
    handle_set.wr_mask_.sync (this->handler_rep_.max_handlep1 ());
    handle_set.ex_mask_.sync (this->handler_rep_.max_handlep1 ());
#endif /* ACE_WIN32 */
  }
  
  return nfound; 
  // Timed out or input available
}

void 
ACE_QtReactor::timeout_event (void)
{  
  // Deal with any timer events
  ACE_Select_Reactor_Handle_Set handle_set;
  this->dispatch (0, handle_set );

  // Set next timeout signal
  this->reset_timeout ();
}

void 
ACE_QtReactor::read_event (int handle)
{
  // Send read event
  ACE_Select_Reactor_Handle_Set dispatch_set;

  dispatch_set.rd_mask_.set_bit (ACE_HANDLE(handle));
  this->dispatch (1, dispatch_set);
}

void 
ACE_QtReactor::write_event (int handle)
{
  // Send write event
  ACE_Select_Reactor_Handle_Set dispatch_set;

  dispatch_set.wr_mask_.set_bit (ACE_HANDLE(handle));
  this->dispatch (1, dispatch_set);
}

void 
ACE_QtReactor::exception_event (int handle)
{
  // Send exception event
  ACE_Select_Reactor_Handle_Set dispatch_set;

  dispatch_set.ex_mask_.set_bit(ACE_HANDLE(handle));
  dispatch (1, dispatch_set);
}



int 
ACE_QtReactor::QtWaitForMultipleEvents (int width,
                                        ACE_Select_Reactor_Handle_Set &wait_set,
                                        ACE_Time_Value */*max_wait_time*/)
{
  // Check to make sure our handle's are all usable.
  ACE_Select_Reactor_Handle_Set temp_set = wait_set;

  if (ACE_OS::select (width,
                      temp_set.rd_mask_,
                      temp_set.wr_mask_,
                      temp_set.ex_mask_,
                      (ACE_Time_Value *) &ACE_Time_Value::zero ) == -1)
    return -1; // Bad file arguments...
  
  // Qt processing.
  this->qapp_->processOneEvent ();

  // Reset the width, in case it changed during the upcalls.
  width = handler_rep_.max_handlep1 ();

  // Now actually read the result needed by the <Select_Reactor> using
  // <select>.
  return ACE_OS::select(width,
                        wait_set.rd_mask_,
                        wait_set.wr_mask_,
                        wait_set.ex_mask_,
                        (ACE_Time_Value *) &ACE_Time_Value::zero);
}

/*
New way of doing things:

1. continue to overload register_handler_i
   - always create a disabled read, write, and exception notifier for a handle
   - call regular register_handler_i -- this will eventually call
     bit_ops with ADD or SET (see #3)

2. continue to overload remove_handler_i
   - call regular remove_handler_i -- this will call bit_ops with CLR_BIT (see #3)
   - find and delete the corresponding read, write, and exception notifiers
     for the handle

2. overload bit_ops
   - call regular bit_ops
   - based on the handle_set, operation, and mask, will enable or disable the
     appropriate QSocketNotifier instance for the handle
*/

int
ACE_QtReactor::set_enable_flag_by_mask (int flag_value,
                                        ACE_HANDLE handle,
                                        ACE_Reactor_Mask mask)
{
  QSocketNotifier *qs_not;

  if (ACE_BIT_ENABLED(mask, ACE_Event_Handler::READ_MASK) ||
      ACE_BIT_ENABLED( mask, ACE_Event_Handler::ACCEPT_MASK))
    {
      // Find the current notifier
      qs_not = 0;
      if ((this->read_notifier_.find (handle, qs_not) == -1))
        return -1;
      
      qs_not->setEnabled (flag_value);
    }
  
  if (ACE_BIT_ENABLED( mask, ACE_Event_Handler::WRITE_MASK) ||
      ACE_BIT_ENABLED( mask, ACE_Event_Handler::ACCEPT_MASK) ||
      ACE_BIT_ENABLED( mask, ACE_Event_Handler::CONNECT_MASK))
    {
      qs_not = 0;
      if ((this->write_notifier_.find (handle, qs_not) == -1))
        return -1;
      
      qs_not->setEnabled (flag_value);
    }
  
  if (ACE_BIT_ENABLED( mask, 
                       ACE_Event_Handler::EXCEPT_MASK))
    {
      qs_not = 0;
      if ((this->exception_notifier_.find (handle, qs_not) == -1))
        return -1;
      
      qs_not->setEnabled (flag_value);
    }

  return 0;
}

int
ACE_QtReactor::bit_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        ACE_Select_Reactor_Handle_Set &handle_set,
                        int ops)
{
  int result;
  ACE_Select_Reactor_Handle_Set preserved_handle_set = handle_set;

  // Call regular bit_ops
  if ((result = ACE_Select_Reactor::bit_ops (handle, mask, handle_set, ops)) == -1)
    return -1;

  // disable or enable the notifiers based on handle_set and mask
  int enableFlag = -1;
  if (&handle_set == &this->suspend_set_)
    enableFlag = 0;
  else if (&handle_set == &this->wait_set_)
    enableFlag = 1;
  else
    // We have no work to do here, so just return
    return result;

  switch (ops)
    {
    case ACE_Reactor::SET_MASK:
    case ACE_Reactor::ADD_MASK:
      // Enable or disable notifiers based on the specified masks
      if (this->set_enable_flag_by_mask (enableFlag, handle, mask) == -1)
        {
          // We can't just return -1 here because we'll have half-changed things.
          // So, we need to restore the old handle_set, then return -1.
          handle_set = preserved_handle_set;
          return -1;
        }
      break;

    case ACE_Reactor::CLR_MASK:
      if (this->set_enable_flag_by_mask (!enableFlag, handle, mask) == -1)
        {
          handle_set = preserved_handle_set;
          return -1;
        }
      break;
      
    default:
      // we take no action for any other operations
      break;
    }

  return result;
}

void
ACE_QtReactor::create_notifiers_for_handle (ACE_HANDLE handle)
{
  // We check for any unused handles.
  MAP::ITERATOR iter = this->read_notifier_.end ();
  QSocketNotifier *qsock_notifier = 0;

  // We check whether we have a data against the present
  // handle. If so we need to unbind the data. 
  if ((this->read_notifier_.find (handle, 
                                  qsock_notifier) != -1))
    {
      if (qsock_notifier != (*iter).int_id_)
        {
          this->read_notifier_.unbind (handle,
                                       qsock_notifier);
          delete qsock_notifier;
        }
    }
      
  ACE_NEW (qsock_notifier,
                  QSocketNotifier (int(handle), QSocketNotifier::Read, this));
      
  this->read_notifier_.bind (handle,
                             qsock_notifier);
      
  QObject::connect (qsock_notifier, 
                    SIGNAL (activated (int)), 
                    this, 
                    SLOT (read_event (int))) ;
  // disable; it will be enabled if necessary by register_handler_i
  qsock_notifier->setEnabled (0);
  

  iter = this->write_notifier_.end ();

  // We check whether we have a data against the present
  // handle. If so we need to unbind the data. 
  if ((this->write_notifier_.find (handle, 
                                   qsock_notifier) != -1))
    {
      if (qsock_notifier != (*iter).int_id_)
        {
          this->write_notifier_.unbind (handle,
                                        qsock_notifier);
          delete qsock_notifier;
        }
    }
      
  ACE_NEW (qsock_notifier,
                  QSocketNotifier (int(handle), QSocketNotifier::Write, this));
      
  this->write_notifier_.bind (handle,
                              qsock_notifier);
      
  QObject::connect (qsock_notifier, 
                    SIGNAL (activated (int)), 
                    this, 
                    SLOT (write_event (int)));
  // disable; it will be enabled by the regular register_handler_i if
  // necessary
  qsock_notifier->setEnabled (0);
  
  // We check for any unused handles.
  iter = this->exception_notifier_.end ();
  qsock_notifier = 0;

  // We check whether we have a data against the present
  // handle. If so we need to unbind the data. 
  if ((this->exception_notifier_.find (handle, 
                                       qsock_notifier) != -1))
    {
      if (qsock_notifier != (*iter).int_id_)
        {
          this->exception_notifier_.unbind (handle,
                                            qsock_notifier);
          delete qsock_notifier;
        }
    }
      
  ACE_NEW (qsock_notifier,
                  QSocketNotifier (int(handle), QSocketNotifier::Exception, this));
      
  this->exception_notifier_.bind (handle,
                                  qsock_notifier);
      
  QObject::connect (qsock_notifier, 
                    SIGNAL (activated (int)), 
                    this, 
                    SLOT (exception_event (int))) ;
  // disable; it will be enabled by the regular register_handler_i if
  // necessary
  qsock_notifier->setEnabled (0);
}

void
ACE_QtReactor::destroy_notifiers_for_handle (ACE_HANDLE handle)
{
  QSocketNotifier *qsock_notifier = 0;

  // Looks for the handle in the maps and removes them. 
  MAP::ITERATOR iter = this->read_notifier_.end ();
  
  if ((this->read_notifier_.find (handle, 
                                  qsock_notifier) != -1))
    {
      this->read_notifier_.unbind (handle,
                                   qsock_notifier);
      delete qsock_notifier;
    }

  iter = this->write_notifier_.end ();
  if ((this->write_notifier_.find (handle, 
                                   qsock_notifier) != -1))
    {
      this->write_notifier_.unbind (handle,
                                    qsock_notifier);
      delete qsock_notifier;
    }

  iter = this->exception_notifier_.end ();
  if ((this->exception_notifier_.find (handle, 
                                       qsock_notifier) != -1))
    {
      this->exception_notifier_.unbind (handle,
                                        qsock_notifier);
      delete qsock_notifier;
    }
}

int 
ACE_QtReactor::register_handler_i (ACE_HANDLE handle ,
                                   ACE_Event_Handler *handler,
                                   ACE_Reactor_Mask mask)
{
  ACE_TRACE ("ACE_QtReactor::register_handler_i");
  
  this->create_notifiers_for_handle (handle);

  int result;
  if ((result = ACE_Select_Reactor::register_handler_i(handle, 
                                                       handler, 
                                                       mask )) 
      == -1)
    {
      this->destroy_notifiers_for_handle (handle);
      return -1;
    }

  return 0;
}

int 
ACE_QtReactor::register_handler_i (const ACE_Handle_Set &handles,
                                   ACE_Event_Handler *handler,
                                   ACE_Reactor_Mask mask)
{
  return ACE_Select_Reactor::register_handler_i(handles, 
                                                handler, 
                                                mask);
}

int ACE_QtReactor::remove_handler_i (ACE_HANDLE handle ,
                                     ACE_Reactor_Mask mask   )
{
  ACE_TRACE ("ACE_QtReactor::remove_handler_i");

  int result = ACE_Select_Reactor::remove_handler_i (handle, mask);
  this->destroy_notifiers_for_handle (handle);
  return result;
}


int 
ACE_QtReactor::remove_handler_i (const ACE_Handle_Set &handles,
                                 ACE_Reactor_Mask  mask)
{
  return ACE_Select_Reactor::remove_handler_i (handles, 
                                               mask);
}

// The following functions ensure that there is an Qt timeout for the
// first timeout in the Reactor's Timer_Queue.

void 
ACE_QtReactor::reset_timeout (void)
{
  if (this->qtime_ != 0) 
    { 
      delete this->qtime_; 
      this->qtime_ = 0; 
    }

  ACE_Time_Value *max_wait_time = 
    this->timer_queue_->calculate_timeout (0) ;

  if (max_wait_time)
  {
    ACE_NEW (this->qtime_,
             QTimer);

    QObject::connect (qtime_, 
                      SIGNAL (timeout ()), 
                      this, 
                      SLOT (timeout_event ()));
    
    qtime_->start(max_wait_time->msec(), 1); 
  }
  
}


long 
ACE_QtReactor::schedule_timer (ACE_Event_Handler *handler,
                               const void *arg,
                               const ACE_Time_Value &delay_time,
                               const ACE_Time_Value &interval)
{
  ACE_TRACE ("ACE_QtReactor::schedule_timer");
  ACE_MT (ACE_GUARD_RETURN (ACE_Select_Reactor_Token, 
                            ace_mon, 
                            this->token_, 
                            -1));

  long result;
  if ((result = ACE_Select_Reactor::schedule_timer(handler, 
                                                   arg, 
                                                   delay_time, 
                                                   interval)) == -1 ) 
    return -1;
  else
  {
    this->reset_timeout ();
    return result;
  }
}

int 
ACE_QtReactor::cancel_timer (ACE_Event_Handler *handler,
                             int dont_call_handle_close)
{
  ACE_TRACE ("ACE_QtReactor::cancel_timer");

  if (ACE_Select_Reactor::cancel_timer (handler, 
                                        dont_call_handle_close ) == -1 ) 
    return -1 ;
  else
  {
    this->reset_timeout( ) ;
    return 0 ;
  }
}

int ACE_QtReactor::cancel_timer (long  timer_id,
                                 const void **arg,
                                 int dont_call_handle_close )
{
  ACE_TRACE( "ACE_QtReactor::cancel_timer" ) ;

  if (ACE_Select_Reactor::cancel_timer (timer_id, 
                                        arg, 
                                        dont_call_handle_close ) == -1 ) 
    return -1 ;
  else
  {
    this->reset_timeout( ) ;
    return 0 ;
  }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Entry<ACE_HANDLE, QSocketNotifier *>;
template class ACE_Map_Manager<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, QSocketNotifier *>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, QSocketNotifier *, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /*ACE_HAS_QT */
