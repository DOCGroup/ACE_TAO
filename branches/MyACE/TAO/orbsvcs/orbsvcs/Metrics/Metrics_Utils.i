// $Id$

#ifndef METRICS_UTILS_I
#define METRICS_UTILS_I

/////////////////////////////
// Class TAO_Metrics_Utils //
/////////////////////////////

// Accessor for static timeout event handle.  This handle is ignored
// by the QoS monitor, since by definition it will be dispatched
// after the end-of-frame.

ACE_INLINE
RtecScheduler::handle_t
TAO_Metrics_Utils::timeoutEvent (void)
{
  return TAO_Metrics_Utils::timeoutEvent_;
}


// Mutator for static timeout event handle.  This handle is ignored
// by the QoS monitor, since by definition it will be dispatched
// after the end-of-frame.

ACE_INLINE
void
TAO_Metrics_Utils::timeoutEvent (RtecScheduler::handle_t handle)
{
  TAO_Metrics_Utils::timeoutEvent_ = handle;
}

#endif /* METRICS_UTILS_I */
