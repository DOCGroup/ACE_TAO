/*
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#ifndef _RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H
#define _RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H

#include "ace/SOCK_Stream.h"
#include "Logging_Server.h"
#include "Thread_Per_Connection_Logging_Server.h"

class RT_Thread_Per_Connection_Logging_Server :
  public Thread_Per_Connection_Logging_Server
{
public:
  RT_Thread_Per_Connection_Logging_Server () = default;
  RT_Thread_Per_Connection_Logging_Server (RT_Thread_Per_Connection_Logging_Server const &) = delete;
  RT_Thread_Per_Connection_Logging_Server (RT_Thread_Per_Connection_Logging_Server &&) = delete;
  RT_Thread_Per_Connection_Logging_Server &operator= (RT_Thread_Per_Connection_Logging_Server const &) = delete;
  RT_Thread_Per_Connection_Logging_Server &operator= (RT_Thread_Per_Connection_Logging_Server &&) = delete;

protected:
  int open (u_short port) override;
  int handle_data (ACE_SOCK_Stream * = nullptr) override;
};

#endif /* _RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H */
