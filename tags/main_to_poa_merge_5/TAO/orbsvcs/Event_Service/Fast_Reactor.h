//
// $Id$
//
#if !defined (FAST_REACTOR_H)
#define FAST_REACTOR_H

#if defined (ACE_OLD_STYLE_REACTOR)
# if defined (ACE_WIN32)
#   include "ace/ReactorEx.h"
#   define ACE_ES_FAST_REACTOR_BASE ACE_ReactorEx 
# else 
#   include "ace/Reactor.h"
#   define ACE_ES_FAST_REACTOR_BASE ACE_Reactor 
# endif /* ACE_WIN32 */
#else
# if defined (ACE_WIN32)
#   include "ace/WFMO_Reactor.h"
#   define ACE_ES_FAST_REACTOR_BASE ACE_WFMO_Reactor
# else 
#   include "ace/Select_Reactor.h"
#   define ACE_ES_FAST_REACTOR_BASE ACE_Select_Reactor 
# endif /* ACE_WIN32 */
#endif /* ACE_OLD_STYLE_REACTOR */
//## end module.includes

class ACE_ES_Fast_Reactor : public ACE_ES_FAST_REACTOR_BASE
{
public:
  virtual int handle_events (ACE_Time_Value *max_wait_time = 0)
  {
    ACE_Time_Value timer_buf (0) ;
    ACE_Time_Value *this_timeout = &timer_buf ;

    if (this->timer_queue_->calculate_timeout (max_wait_time,
					       this_timeout) == 0) 
      {
	ACE_Time_Value t (0, 500000);
	ACE_OS::select (0, 0, 0, 0, &t);
      }
    else
      {
	ACE_OS::select (0, 0, 0, 0, this_timeout);
      }

    return this->timer_queue_->expire () == -1 ? -1 : 0;
  }
  virtual int handle_events (ACE_Time_Value &max_wait_time)
  {
    return this->handle_events (&max_wait_time);
  }
};

#endif /* FAST_REACTOR_H */
