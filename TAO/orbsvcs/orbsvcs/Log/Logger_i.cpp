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
priority_conversion (const Logger::Log_Priority prior1) 
{
  if (prior1 == Logger::LM_MAX)
    return LM_MAX;
  else
    {
      int pval = ACE_static_cast (int, prior1);

      return ACE_static_cast (ACE_Log_Priority,
                              ACE_POW (pval));
    }
}

void
Logger_i::log (const Logger::Log_Record &log_rec,
	       CORBA::Environment &_env)
{
  ACE_Log_Record rec (priority_conversion (log_rec.type),
		      log_rec.time,
                      log_rec.app_id);
  
  // Create a ACE_Log_Record in order to leverage existing logging
  // code.The log2() is used because we can't specify values for enum
  // tags in IDL, but ACE_Log_Priority values are powers of 2
  // priority_names_() is essentially used to cast to the enum type
  
  ASYS_TCHAR msgbuf[ACE_MAXLOGMSGLEN + 1];
  ACE_OS::strncpy (msgbuf, 
                   log_rec.msg_data,
                   ACE_MAXLOGMSGLEN);
  // I don't think this is a good way to do this. Suggestions?

  rec.msg_data (msgbuf);
  // Set ACE_Log_Record.msg_data to the value stored in msgbuf
  
#if 0
  // @@ This is currently unused.  Remove them?
  hostent rhost;
  char storage[MAXHOSTNAMELEN + 1];
#endif /* 0 */

  ACE_INET_Addr addy (ACE_static_cast (int, 0),
                      ACE_static_cast (ACE_UINT32,
                                       log_rec.host_addr));
  // The constructor for ACE_INET_Addr requires a port number, which
  // is not relevant in this context, so we give it 0.
  
  ASYS_TCHAR namebuf[MAXHOSTNAMELEN + 1];
  addy.get_host_name (namebuf,
                      MAXHOSTNAMELEN);
  
  rec.print (namebuf,
             ACE_Log_Msg::VERBOSE,
             stderr);
  // Print out the logging message to stderr.
}
