/*
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _REACTOR_LOGGING_SERVER_ADAPTER_H
#define _REACTOR_LOGGING_SERVER_ADAPTER_H

#include "ace/Service_Object.h"
#include "Reactor_Logging_Server_T.h"

template <class ACCEPTOR>
class Reactor_Logging_Server_Adapter : public ACE_Service_Object {
public:
  Reactor_Logging_Server_Adapter () = default;

  Reactor_Logging_Server_Adapter (const Reactor_Logging_Server_Adapter &) = delete;
  Reactor_Logging_Server_Adapter (Reactor_Logging_Server_Adapter &&) = delete;
  Reactor_Logging_Server_Adapter &operator= (const Reactor_Logging_Server_Adapter &) = delete;
  Reactor_Logging_Server_Adapter &operator= (Reactor_Logging_Server_Adapter &&) = delete;

  // Hook methods inherited from <ACE_Service_Object>.
  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;
  int info (ACE_TCHAR **, size_t) const override;
  int suspend () override;
  int resume () override;

private:
  Reactor_Logging_Server<ACCEPTOR> *server_ = nullptr;
};

#include "Reactor_Logging_Server_Adapter.cpp"

#endif /* _REACTOR_LOGGING_SERVER_ADAPTER_H */
