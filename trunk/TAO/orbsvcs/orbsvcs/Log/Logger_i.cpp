// $Id$

#include "ace/ACE.h"
#include "orbsvcs/LoggerC.h"
#include "orbsvcs/Log/Logger_i.h"
#include "ace/Log_Record.h"

Logger_ptr
Logger_Factory_i::make_logger (const char *name,
                               CORBA::Environment &_env)
{
  Logger_i *l;

  ACE_NEW_RETURN (l,
		  Logger_i (name),
		  0);

  return l->_this (_env);
}

Logger_Factory_i::Logger_Factory_i (void)
{
  // Do nothing.
}

Logger_i::Logger_i (const char *name)
  : name_ (ACE_OS::strdup (name))
{
  // Do nothing
}

Logger_i::~Logger_i (void)
{
  ACE_OS::free (this->name_);
}


ACE_Log_Priority
Logger_i::priority_conversion (Logger::Log_Priority priority)
{
  if (priority == Logger::LM_MAX)
    return LM_MAX;
  else
    {
      int pval = ACE_static_cast (int, priority);

      return ACE_static_cast (ACE_Log_Priority,
                              (1 << pval));
      // (1 << pval) == 2^pval. <ACE_Log_Priority> are powers of 2.
    }
}


u_long
Logger_i::verbosity_conversion (Logger::Log_Verbosity verbosity)
{
  u_long pval = ACE_static_cast (int, verbosity);
  return (1 << pval);
}

void
Logger_i::log (const Logger::Log_Record &log_rec,
	       CORBA::Environment &_env)
{
  ACE_Time_Value temp (log_rec.time);

  // Create an <ACE_Log_Record> to leverage existing logging
  // code. Since Logger::Log_Priority enum tags don't cleanly map to
  // ACE_Log_Priority tags, <priority_conversion> is used to coerce
  // the mapping.
  ACE_Log_Record rec (this->priority_conversion (log_rec.type),
		      ACE_Time_Value (log_rec.time),
                      log_rec.app_id);
  
  // Create a temporary buffer for manipulating the logging message, adding
  // additional space for formatting characters..
  ASYS_TCHAR msgbuf [ACE_MAXLOGMSGLEN + 4];

  // Format the message for proper display.
  ACE_OS::strcpy (msgbuf, "\n::");

  // Copy the message data into the temporary buffer  
  ACE_OS::strncat (msgbuf, 
		   log_rec.msg_data,
                   ACE_MAXLOGMSGLEN);

  // Set <ACE_Log_Record.msg_data> to the value stored in <msgbuf>.
  rec.msg_data (msgbuf);

  CORBA::Long addr = log_rec.host_addr;

  // The constructor for <ACE_INET_Addr> requires a port number, which
  // is not relevant in this context, so we give it 0.
  ACE_INET_Addr addy (ACE_static_cast (u_short, 0),
                      ACE_static_cast (ACE_UINT32,
                                       addr));

  // Create a buffer and fill it with the host name of the logger
  ASYS_TCHAR namebuf[MAXHOSTNAMELEN + 1];
  addy.get_host_name (namebuf,
                      MAXHOSTNAMELEN);
  
  rec.print (namebuf,
	     this->verbosity_conversion (log_rec.verbosity),
	     stderr);
  // Print out the logging message to stderr with the given level of
  // verbosity
}
