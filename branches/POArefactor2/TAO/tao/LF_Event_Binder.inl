// $Id$

ACE_INLINE
TAO_LF_Event_Binder::TAO_LF_Event_Binder (TAO_LF_Event *event,
                                          TAO_LF_Follower *follower)
  : event_ (event)
{
  this->event_->bind (follower);
}


