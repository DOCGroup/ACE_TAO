// $Id$

#include "ace/config-all.h"

#if defined (ACE_WIN32) && !defined (ACE_HAS_PHARLAP)

#include "ace/Log_Msg_NT_Event_Log.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"

ACE_RCSID(ace, Log_Msg_NT_Event_Log, "$Id$")

ACE_Log_Msg_NT_Event_Log::ACE_Log_Msg_NT_Event_Log (void)
  : evlog_handle_(0)
{
}

ACE_Log_Msg_NT_Event_Log::~ACE_Log_Msg_NT_Event_Log (void)
{
  (void) this->close ();
}

int
ACE_Log_Msg_NT_Event_Log::open (const ACE_TCHAR *logger_key)
{
  // ACE's "resource module" contains the message resource required
  // for event logging.
  ACE_TCHAR msg_file [MAXPATHLEN];

  if (!ACE_TEXT_GetModuleFileName (ACE_OS::get_win32_resource_module (),
                                   msg_file,
                                   MAXPATHLEN))
    return -1;
  int msg_file_length = ACE_OS::strlen (msg_file);

  // Information is stored in the registry at a location based on the
  // program name.
  ACE_TCHAR reg_key [MAXPATHLEN];
  ACE_OS::strcpy (reg_key,
                  ACE_LIB_TEXT ("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"));
  int reg_key_length = ACE_OS::strlen(reg_key);
  ACE_OS::strncat (reg_key,
                   logger_key ? logger_key : ACE_Log_Msg::program_name (),
                   MAXPATHLEN - reg_key_length);

  // Add the event source to the registry. Note that if this fails it
  // is not fatal. The application will still be able to write entries
  // to the event log, they just won't be formatted correctly.
  HKEY hkey;
  ACE_TEXT_RegCreateKey (HKEY_LOCAL_MACHINE,
                         reg_key,
                         &hkey);
  DWORD flags = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
  ACE_TEXT_RegSetValueEx (hkey,
                          ACE_LIB_TEXT ("TypesSupported"),
                          0,
                          REG_DWORD,
                          (LPBYTE) &flags,
                          sizeof (DWORD));
  ACE_TEXT_RegSetValueEx (hkey,
                          ACE_LIB_TEXT ("EventMessageFile"),
                          0,
                          REG_SZ,
                          (LPBYTE) msg_file,
                          msg_file_length + 1);
  RegCloseKey (hkey);

  // Obtain a handle to the event source.
  this->evlog_handle_ = ACE_TEXT_RegisterEventSource (0, ACE_Log_Msg::program_name ());
  return this->evlog_handle_ ? 0 : -1;
}

int
ACE_Log_Msg_NT_Event_Log::reset (void)
{
  return this->close ();
}

int
ACE_Log_Msg_NT_Event_Log::close (void)
{
  if (DeregisterEventSource (this->evlog_handle_))
    {
      this->evlog_handle_ = 0;
      return 0;
    }
  else
    return -1;
}

int
ACE_Log_Msg_NT_Event_Log::log (ACE_Log_Record &log_record)
{
  // Make a copy of the log text and replace any newlines with
  // spaces. Newline characters do not appear correctly in the event
  // viewer.
  const ACE_TCHAR* src_msg_data = log_record.msg_data ();
  ACE_TCHAR msg_data [ACE_Log_Record::MAXLOGMSGLEN];

  for (long i = 0; i < log_record.length (); ++i)
    {
      if (src_msg_data[i] == '\n')
        msg_data[i] = ' ';
      else
        msg_data[i] = src_msg_data[i];
    }

  // Map the ACE log record type to an event log type.
  WORD event_type;
  switch (log_record.type ())
  {
  case LM_STARTUP:
  case LM_SHUTDOWN:
  case LM_TRACE:
  case LM_DEBUG:
  case LM_INFO:
    event_type = EVENTLOG_INFORMATION_TYPE;
    break;
  case LM_NOTICE:
  case LM_WARNING:
    event_type = EVENTLOG_WARNING_TYPE;
    break;
  case LM_ERROR:
  case LM_CRITICAL:
  case LM_ALERT:
  case LM_EMERGENCY:
  default:
    event_type = EVENTLOG_ERROR_TYPE;
    break;
  }

  // Send the log message to the system event log.
  const ACE_TCHAR* msgs [1];
  msgs[0] = msg_data;

  if (ACE_TEXT_ReportEvent (this->evlog_handle_,
                            event_type, 0, 0, 0, 1, 0, msgs, 0) == 0)
    return -1;
  else 
    return 0;
}

#endif /* ACE_WIN32 */
