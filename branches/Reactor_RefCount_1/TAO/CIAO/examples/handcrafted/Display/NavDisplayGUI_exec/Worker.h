// $Id$

#ifndef __WORKER_H
#define __WORKER_H

#include "ace/Task.h"
#include "ace/Synch.h"
#include "RootPanel.h"

class Worker : public ACE_Task_Base
{
public:
  Worker (int argc, char *argv[])
        : argc_(argc), argv_(argv) {}
  // ctor

  virtual int svc (void);
  // The thread entry point.

  void waitUntillInitialized() {init_complete_.wait();}
  // blocks the calling thread until the GUI is initialized

  RootPanel* getMainWindow() const {return main_wnd_;}
  // Returns the pointer to main application window

private:
  RootPanel *main_wnd_;
  int argc_;
  char **argv_;

  ACE_Manual_Event init_complete_;
};

#endif // __WORKER_H
