/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTOR_LOGGING_SERVER_ADAPTER_H
#define _REACTOR_LOGGING_SERVER_ADAPTER_H

#include "ace/Service_Object.h"
#include "Reactor_Logging_Server.h"

template <class ACCEPTOR>
class Reactor_Logging_Server_Adapter : public ACE_Service_Object {
public:
  // Hook methods inherited from <ACE_Service_Object>.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();
  virtual int info (ACE_TCHAR **, size_t) const;
  virtual int suspend ();
  virtual int resume ();

private:
  Reactor_Logging_Server<ACCEPTOR> *server_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Reactor_Logging_Server_Adapter.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Reactor_Logging_Server_Adapter.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _REACTOR_LOGGING_SERVER_ADAPTER_H */
