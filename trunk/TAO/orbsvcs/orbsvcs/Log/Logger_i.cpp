// $Id$

#include "ace/ACE.h"
#include "orbsvcs/LoggerC.h"
#include "orbsvcs/Log/Logger_i.h"
#include "ace/Log_Record.h"

ACE_RCSID(Log, Logger_i, "$Id$")

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
#define TAO_Logger_Hash \
  ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex>

u_long
TAO_Logger_Hash::hash (const ACE_CString &ext_id)
{
  return ACE::hash_pjw (ext_id.fast_rep ());
}
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

Logger_Factory_i::Logger_Factory_i (void)
{
}

Logger_Factory_i::~Logger_Factory_i (void)
{
}

Logger_ptr
Logger_Factory_i::make_logger (const char *name,
                               CORBA::Environment &TAO_IN_ENV)
{
  Logger_i *result;
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
                        CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (Logger::_nil ());
    }

  // Enter the new logger into the hash map.  Check if the <bind>
  // fails and if so, throw an UNKNOWN exception.  <result> may be
  // valid, but since it would not be properly bound, its behavior my
  // be off, so delete it to be safe.
  if (hash_map_.bind (name, result) == -1)
    {
      delete result;
      TAO_THROW_RETURN (CORBA::UNKNOWN (CORBA::COMPLETED_NO),
                        Logger::_nil ());
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
  return result->_this (TAO_IN_ENV);
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
      int pval = ACE_static_cast (int, priority);

      return ACE_static_cast (ACE_Log_Priority,
                              1 << pval);
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
Logger_i::log (const Logger::Log_Record &log_rec,
               CORBA::Environment &TAO_IN_ENV)
{
  this->logv (log_rec, verbosity_level_, TAO_IN_ENV);
}

void
Logger_i::log2 (const Logger::Log_Record &log_rec,
               CORBA::Environment &TAO_IN_ENV)
{
  this->logv (log_rec, verbosity_level_, TAO_IN_ENV);
}

void
Logger_i::logv2 (const Logger::Log_Record &log_rec,
               Logger::Verbosity_Level verbosity,
               CORBA::Environment &TAO_IN_ENV)
{
  this->logv (log_rec, verbosity, TAO_IN_ENV);
}

void
Logger_i::logv (const Logger::Log_Record &log_rec,
               Logger::Verbosity_Level verbosity,
               CORBA::Environment &)
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
  ASYS_TCHAR msgbuf [ACE_MAXLOGMSGLEN + 4];

  // Format the message for proper display.
  ACE_OS::strcpy (msgbuf, "::");

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

  ACE_OS::strncpy (namebuf, addy.get_host_addr (), MAXHOSTNAMELEN);

  u_long verb_level = this->verbosity_conversion (verbosity);

  rec.print (namebuf,
             verb_level,
             stderr);
  // Print out the logging message to stderr with the given level of
  // verbosity
}

Logger::Verbosity_Level
Logger_i::verbosity (void) const
{
  return verbosity_level_;
}

void
Logger_i::verbosity (Logger::Verbosity_Level level, CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  verbosity_level_ = level;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, Logger_i *>;
template class ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, Logger_i *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, Logger_i *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, Logger_i *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, Logger_i *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, Logger_i *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Logger_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
