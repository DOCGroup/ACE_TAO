#include <iostream.h>
// $Id$

#include "Logger.h"

Logger::~Logger (void)
{
  // Release and free up the object reference.
  this->logref_->_release ();

  // Must use free since we used strdup(3C).
  ACE_OS::free (ACE_MALLOC_T (this->server_));
}

// Constructor takes the name of the host where the server
// is located.  If server == 0, then use the locator service.

Logger::Logger (char *server, size_t max_message_size)
  : server_ (server == 0 ? 0 : ACE_OS::strdup (server)), 
    ip_ (0), 
    pid_ (ACE_OS::getpid ())
{
  struct utsname name;

#if 0
  // Could also use sysinfo(2)...

  ACE_OS::sysinfo (SI_HOSTNAME, clienthost, MAXHOSTNAMELEN);
#endif

  ACE_OS::uname (&name);
  hostent *hp = ACE_OS::gethostbyname (name.nodename);

  if (hp != 0)
    memcpy ((void *) &this->ip_, (void *) hp->h_addr, hp->h_length);

  TRY {
    // First bind to the logger object.
    // argv[1] has the hostname (if any) of the target logger object;
    // The default is the local host:
    this->logref_ = profile_logger::_bind ("", this->server_, IT_X);
  } CATCHANY {
    // an error occurred while trying to bind to the logger object.
    cerr << "Bind to object failed" << endl;
    cerr << "Unexpected exception " << IT_X << endl;
  } ENDTRY;
  // Pre-assign certain values that don't change.
  this->log_msg_.app_id = this->pid_;
  this->log_msg_.host_addr = this->ip_;
  this->log_msg_.msg_data._maximum = max_message_size;
}

// Transmit the message to the logging server.

int
Logger::log (logger::Log_Priority priority, char message[], int length)
{
  // The following values change with every logging operation.
  this->log_msg_.type = priority;
  this->log_msg_.time = ACE_OS::time (0);
  this->log_msg_.msg_data._length = length;
  this->log_msg_.msg_data._buffer = message;

  TRY {
    // Try to log a message.
    this->logref_->log (this->log_msg_, IT_X);
  } CATCHANY {
    // an error occurred while trying to read the height and width:
    cerr << "call to log failed" << endl;
    cerr << "Unexpected exception " << IT_X << endl;
    return -1;
  } ENDTRY;
  // success.
  return 0;
}

// Get the value of verbose.

int
Logger::verbose (void)
{
  int verbosity = 0;

  TRY {
    verbosity = this->logref_->verbose ();
  } CATCHANY {
    return -1;
  } ENDTRY;
  return verbosity;
}

// Set the value of verbose.

int
Logger::verbose (int value)
{
  int verbosity = 0;

  TRY {
    this->logref_->verbose (value);
  } CATCHANY {
    return -1;
  } ENDTRY;
  return 0;
}

// Activate the timer.

int
Logger::start_timer (void)
{
  TRY {
    this->logref_->start_timer ();
  } CATCHANY {
    return -1;
  } ENDTRY;
  return 0;
}

// Deactivate the timer and return the elapsed time.

int 
Logger::stop_timer (profile_logger::Elapsed_Time &et)
{
  TRY {
    this->logref_->stop_timer (et);
  } CATCHANY {
    return -1;
  } ENDTRY;
  return 0;
}
