// $Id$

namespace Kokyu
{

ACE_INLINE
Dispatch_Deferrer::Dispatch_Deferrer (void)
  : msg_strat_()
  , rgq_(this->msg_strat_)
  , timers_()
  , react_()
  , task_(0)
{
}

ACE_INLINE
Dispatch_Deferrer::~Dispatch_Deferrer (void)
{
  //TODO: remove all timers before closing reactor

  this->react_.close();

  this->timers_.close();
}

} //namespace Kokyu
