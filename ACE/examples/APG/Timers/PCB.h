/* -*- C++ -*- */
#if !defined(PCB_H)
#define PCB_H

// Listing 1 code/ch20
class PCB
{
public:
  PCB ();
  virtual ~PCB ();

  // Set/get the timer id that is being handled by this instance.
  void setID (long timerID);
  long getID () const;

  // Handle a timeout event, cancel, and close.
  virtual int handleEvent (const void *arg);
  virtual int handleCancel ();
  virtual int handleClose ();

private:
  long timerID_;
  int count_;
};
// Listing 1

#endif /*PCB_H*/
