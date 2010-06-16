// $Id$
#ifndef DEPLOYMENT_EVENTS_TPP
#define DEPLOYMENT_EVENTS_TPP

namespace DAnCE
{
  Event_Handler::Event_Handler (Functor &specific_handler)
    : outstanding_ (0),
      specific_handler_ (specific_handler)
  {
  }
    
  Event_Handler::~Event_Handler (void);

  void 
  Event_Handler::update (const Event_Result &future)
  {
    this->specific_handler_ (future);
    --this->outstanding_;
  }
    
  /// Indicate to the observer that there is an additional future
  /// it is waiting on
  void
  Event_Handler::add_outstanding (void)
  {
    ++this->outstanding_;
  }
    
  /// Return the number of still outstanding future events.
  size_t
  Event_Handler::count_outstanding (void)
  {
    return this->outstanding_.value ();
  }
}

#endif
