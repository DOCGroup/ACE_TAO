// $Id$

namespace Kokyu {

ACE_INLINE
Dispatch_Deferrer::Dispatch_Deferrer (void)
  : msg_strat_()
  , timers_()
  , react_()
  , task_(0)
{
}

ACE_INLINE
Dispatch_Deferrer::~Dispatch_Deferrer (void)
{
  //TODO: remove all timers

  //Don't close reactor since it may still be in use
  //and we don't own it!

  this->timers_.close();
}

} //namespace Kokyu
