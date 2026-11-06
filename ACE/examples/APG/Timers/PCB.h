/* -*- C++ -*- */
#if !defined(PCB_H)
#define PCB_H

// Listing 1 code/ch20
class PCB
{
public:
  PCB () = default;
  PCB (PCB const &) = delete;
  PCB (PCB &&) = delete;
  PCB &operator= (PCB const &) = delete;
  PCB &operator= (PCB &&) = delete;
  virtual ~PCB () = default;

  // Set/get the timer id that is being handled by this instance.
  void setID (long timerID);
  long getID () const;

  // Handle a timeout event, cancel, and close.
  virtual int handleEvent (void const *arg);
  virtual int handleCancel ();
  virtual int handleClose ();

private:
  long timerID_ {};
  int count_ {};
};
// Listing 1

#endif /*PCB_H*/
