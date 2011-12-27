// $Id$

#include "LoggerC.h"
#include "Logger_i.h"
#include "tao/debug.h"
#include "ace/ACE.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Record.h"
#include "ace/os_include/os_netdb.h"

Logger_Factory_i::Logger_Factory_i (void)
{
}

Logger_Factory_i::~Logger_Factory_i (void)
{
}

Logger_ptr
Logger_Factory_i::make_logger (const char *name)
{
  Logger_i *result = 0;
  // If name is already in the map, <find> will assign <result> to the
  // appropriate value

  if (hash_map_.find (name, result) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nMaking a new logger"));

      // This attempts to create a new Logger_i and throws an
      // exception and returns a null value if it fails
      ACE_NEW_THROW_EX (result,
                        Logger_i (name),
                        CORBA::NO_MEMORY ());
    }

  // Enter the new logger into the hash map.  Check if the <bind>
  // fails and if so, throw an UNKNOWN exception.  <result> may be
  // valid, but since it would not be properly bound, its behavior my
  // be off, so delete it to be safe.
  if (hash_map_.bind (name, result) == -1)
    {
      delete result;
      throw CORBA::UNKNOWN ();
    }
  else
    // Logger of name <name> already bound.  <result> is set
    // appropriately by <find>.  So do nothing.
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  "\nLogger name already bound"));

  // <_this> is an performance hit here, but apparently if the object
  // is already registered with the POA, it will ignore the second
  // registration attempt.
  // @@ Matt, this code doesn't seem right.  Can you please check with
  // Irfan and Carlos about whether this is the right thing to do?
  if (!result)
    return Logger::_nil ();

  return result->_this ();
}

Logger_i::Logger_i (const char *name)
  : name_ (ACE_OS::strdup (name)),
    verbosity_level_ (Logger::VERBOSE)
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
      int pval = static_cast<int> (priority);

      return static_cast<ACE_Log_Priority> (1 << pval);
      // (1 << pval) == 2^pval. <ACE_Log_Priority> are powers of 2.
    }
}

u_long
Logger_i::verbosity_conversion (Logger::Verbosity_Level verbosity_level)
{
  // This isn't very elegant, but it's because there's no simple
  // mapping from <Logger::Verbosity_Level>'s to the verbosity flags
  // specified in <ace/Log_Msg.h>
  switch (verbosity_level)
    {
    case Logger::SILENT:
      return 64;
    case Logger::VERBOSE_LITE:
      return 32;
    default:
    case Logger::VERBOSE:
      return 16;
    }
}

void
Logger_i::log (const Logger::Log_Record &log_rec)
{
  this->logv (log_rec, verbosity_level_);
}

void
Logger_i::log_twoway (const Logger::Log_Record &log_rec)
{
  this->logv (log_rec, verbosity_level_);
}

void
Logger_i::logv_twoway (const Logger::Log_Record &log_rec,
                       Logger::Verbosity_Level verbosity)
{
  this->logv (log_rec, verbosity);
}

void
Logger_i::logv (const Logger::Log_Record &log_rec,
               Logger::Verbosity_Level verbosity)
{
  // Create an <ACE_Log_Record> to leverage existing logging
  // code. Since Logger::Log_Priority enum tags don't cleanly map to
  // ACE_Log_Priority tags, <priority_conversion> is used to coerce
  // the mapping.
  ACE_Log_Record rec (this->priority_conversion (log_rec.type),
                      ACE_Time_Value (log_rec.time),
                      log_rec.app_id);

  // Create a temporary buffer for manipulating the logging message,
  // adding additional space for formatting characters..
  ACE_TCHAR msgbuf [ACE_MAXLOGMSGLEN + 4];

  // Format the message for proper display.
  ACE_OS::strcpy (msgbuf, ACE_TEXT("::"));

  // Copy the message data into the temporary buffer
  ACE_OS::strncat (msgbuf,
                   ACE_TEXT_CHAR_TO_TCHAR(log_rec.msg_data),
                   ACE_MAXLOGMSGLEN);

  // Set <ACE_Log_Record.msg_data> to the value stored in <msgbuf>.
  rec.msg_data (msgbuf);

  CORBA::Long addr = log_rec.host_addr;

  // The constructor for <ACE_INET_Addr> requires a port number, which
  // is not relevant in this context, so we give it 0.
  ACE_INET_Addr addy (static_cast<u_short> (0),
                      static_cast<ACE_UINT32> (addr));

  // Create a buffer and fill it with the host name of the logger
  ACE_TCHAR namebuf[MAXHOSTNAMELEN + 1];

  ACE_OS::strncpy (namebuf, ACE_TEXT_CHAR_TO_TCHAR(addy.get_host_addr ()), MAXHOSTNAMELEN);

  u_long verb_level = this->verbosity_conversion (verbosity);

  rec.print (namebuf,
             verb_level,
             stderr);
  // Print out the logging message to stderr with the given level of
  // verbosity
}

void
Logger_i::verbosity (Logger::Verbosity_Level level)
{
  this->verbosity_level_ = level;
}

