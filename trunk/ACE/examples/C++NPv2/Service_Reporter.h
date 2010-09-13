/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _SERVICE_REPORTER_H
#define _SERVICE_REPORTER_H

#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include "ace/SOCK_Acceptor.h"


class Service_Reporter : public ACE_Service_Object {
public:
  Service_Reporter (ACE_Reactor *r = ACE_Reactor::instance ())
    : ACE_Service_Object (r) {}

protected:
  // Hook methods inherited from <ACE_Service_Object>.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();
  virtual int info (ACE_TCHAR **, size_t) const;
  virtual int suspend ();
  virtual int resume ();

  // Reactor hook methods.
  virtual int handle_input (ACE_HANDLE);
  virtual ACE_HANDLE get_handle () const
  { return acceptor_.get_handle (); }

private:
  ACE_SOCK_Acceptor acceptor_; // Acceptor instance.
  enum { DEFAULT_PORT = 9411 };
};

#endif /* _SERVICE_REPORTER_H */
