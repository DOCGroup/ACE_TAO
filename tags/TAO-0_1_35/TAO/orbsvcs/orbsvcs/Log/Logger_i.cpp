// $Id$

#include "ace/ACE.h"
#include "orbsvcs/LoggerC.h"
#include "orbsvcs/Log/Logger_i.h"
#include "ace/Log_Record.h"

Logger_ptr
Logger_Factory_i::make_logger (const char *name,
                               CORBA::Environment &_env)
{
  Logger_i *l = new Logger_i (name);

  return l->_this (_env);
}

Logger_Factory_i::Logger_Factory_i (void)
{
  // Do nothing
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

// This converts from the IDL defined Log_Priority enumerated type to
// the ACE_Log_Priority enumerated type

ACE_Log_Priority
Logger_i::priority_conversion (Logger::Log_Priority priority)
{
  if (priority == Logger::LM_MAX)
    return LM_MAX;
  else
    {
      int pval = ACE_static_cast (int, priority);

      return ACE_static_cast (ACE_Log_Priority,
                              ACE_POW (pval));
    }
}

void
Logger_i::log (const Logger::Log_Record &log_rec,
	       CORBA::Environment &_env)
{
  ACE_Log_Record rec (this->priority_conversion (log_rec.type),
		      log_rec.time,
                      log_rec.app_id);
  
  // Create an <ACE_Log_Record> to leverage existing logging code. The
  // <ACE::log2> method is used because we can't specify values for
  // enum tags in IDL, but <ACE_Log_Priority> values are powers of 2.
  // Thus, <priority_conversion> is used to transform to an enum type.
  
  ASYS_TCHAR msgbuf[ACE_MAXLOGMSGLEN + 1];
  ACE_OS::strncpy (msgbuf, 
                   log_rec.msg_data,
                   ACE_MAXLOGMSGLEN);
  // I don't think this is a good way to do this. Suggestions?  @@
  // Matt, please make sure that the size of the array <msg_data> is
  // no larger than ACE_MAXLOGMSGLEN...

  // Set <ACE_Log_Record.msg_data> to the value stored in <msgbuf>.
  rec.msg_data (msgbuf);
  
  CORBA::Long addr = log_rec.host_addr;

  // The constructor for <ACE_INET_Addr> requires a port number, which
  // is not relevant in this context, so we give it 0.
  ACE_INET_Addr addy (ACE_static_cast (u_short, 0),
                      ACE_static_cast (ACE_UINT32,
                                       addr));
  
  ASYS_TCHAR namebuf[MAXHOSTNAMELEN + 1];
  addy.get_host_name (namebuf,
                      MAXHOSTNAMELEN);
  
  rec.print (namebuf,
             ACE_Log_Msg::VERBOSE,
             stderr);
  // Print out the logging message to stderr.
}
