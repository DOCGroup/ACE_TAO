// $Id$

ACE_INLINE
TAO_LF_Event_Binder::TAO_LF_Event_Binder (TAO_LF_Event *event,
                                          TAO_Follower *follower)
  : event_ (event)
{
  this->event_->bind (follower);
}


ACE_INLINE
TAO_LF_Event_Binder::~TAO_LF_Event_Binder (void)
{
  this->event_->unbind ();
}
