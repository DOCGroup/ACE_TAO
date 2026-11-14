/* -*- C++ -*- */
#if !defined(CB_H)
#define CB_H

#include "ace/Event_Handler.h"

#include "TimerDispatcher.h"

// Listing 1 code/ch20
class CB : public ACE_Event_Handler
{
public:
  CB () = default;
  CB (CB const &) = delete;
  CB (CB &&) = delete;
  CB &operator= (CB const &) = delete;
  CB &operator= (CB &&) = delete;

  // Set the timer id that is being handled by this instance.
  void setID (long timerID);

  // Get the timer id.
  long getID () const;

  // Handle the timeout.
  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg = nullptr) override;

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask) override;

private:
  long timerID_ {};
  int count_ {};
};
// Listing 1

#endif /*CB_H*/
