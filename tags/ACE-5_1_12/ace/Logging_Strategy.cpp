// $Id$

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "ace/Logging_Strategy.h"

ACE_RCSID(lib, Logging_Strategy, "$Id$")

// Parse the string containing (thread) priorities and set them accordingly.

void
ACE_Logging_Strategy::priorities (ACE_TCHAR *priority_string,
                                  ACE_Log_Msg::MASK_TYPE mask)
{
  u_long priority_mask = 0;

  // Choose priority mask to change.

  if (mask == ACE_Log_Msg::PROCESS)
    priority_mask = process_priority_mask_;
  else
    priority_mask = thread_priority_mask_;

  // Parse string and alternate priority mask.

  for (ACE_TCHAR *priority = ACE_OS::strtok (priority_string, ACE_LIB_TEXT ("|"));
       priority != 0;
       priority = ACE_OS::strtok (0, ACE_LIB_TEXT ("|")))
    {
      if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("TRACE")) == 0)
        ACE_SET_BITS (priority_mask, LM_TRACE);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~TRACE")) == 0)
        ACE_CLR_BITS (priority_mask, LM_TRACE);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("DEBUG")) == 0)
        ACE_SET_BITS (priority_mask, LM_DEBUG);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~DEBUG")) == 0)
        ACE_CLR_BITS (priority_mask, LM_DEBUG);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("INFO")) == 0)
        ACE_SET_BITS (priority_mask, LM_INFO);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~INFO")) == 0)
        ACE_CLR_BITS (priority_mask, LM_INFO);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("NOTICE")) == 0)
        ACE_SET_BITS (priority_mask, LM_NOTICE);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~NOTICE")) == 0)
        ACE_CLR_BITS (priority_mask, LM_NOTICE);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("WARNING")) == 0)
        ACE_SET_BITS (priority_mask, LM_WARNING);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~WARNING")) == 0)
        ACE_CLR_BITS (priority_mask, LM_WARNING);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("ERROR")) == 0)
        ACE_SET_BITS (priority_mask, LM_ERROR);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~ERROR")) == 0)
        ACE_CLR_BITS (priority_mask, LM_ERROR);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("CRITICAL")) == 0)
        ACE_SET_BITS (priority_mask, LM_CRITICAL);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~CRITICAL")) == 0)
        ACE_CLR_BITS (priority_mask, LM_CRITICAL);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("ALERT")) == 0)
        ACE_SET_BITS (priority_mask, LM_ALERT);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~ALERT")) == 0)
        ACE_CLR_BITS (priority_mask, LM_ALERT);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("EMERGENCY")) == 0)
        ACE_SET_BITS (priority_mask, LM_EMERGENCY);
      else if (ACE_OS::strcmp (priority, ACE_LIB_TEXT ("~EMERGENCY")) == 0)
        ACE_CLR_BITS (priority_mask, LM_EMERGENCY);
    }

  // Affect right priority mask.

  if (mask == ACE_Log_Msg::PROCESS)
    process_priority_mask_ = priority_mask;
  else
    thread_priority_mask_ = priority_mask;
}

// Parse the string containing all the flags and set the flags
// accordingly.

void
ACE_Logging_Strategy::tokenize (ACE_TCHAR *flag_string)
{
  for (ACE_TCHAR *flag = ACE_OS::strtok (flag_string, ACE_LIB_TEXT ("|"));
       flag != 0;
       flag = ACE_OS::strtok (0, ACE_LIB_TEXT ("|")))
    {
      if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("STDERR")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::STDERR);
      else if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("LOGGER")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::LOGGER);
      else if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("OSTREAM")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::OSTREAM);
      else if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("VERBOSE")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::VERBOSE);
      else if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("VERBOSE_LITE")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::VERBOSE_LITE);
      else if (ACE_OS::strcmp (flag, ACE_LIB_TEXT ("SILENT")) == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::SILENT);
    }
}

int
ACE_Logging_Strategy::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("ACE_Logging_Strategy::parse_args");
  ACE_TCHAR *temp;

  this->flags_ = 0;
  this->wipeout_logfile_ = 0;
  this->interval_ = 0;
  this->max_size_ = ACE_DEFAULT_MAX_LOGFILE_SIZE;

  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT ("f:i:m:p:s:t:w"), 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 'f':
          temp = get_opt.optarg;
          // Now tokenize the string to get all the flags
          this->tokenize (temp);
          break;
        case 'i':
          // Interval (in secs) at which logfile size is sampled.
          this->interval_ = ACE_OS::strtoul (get_opt.optarg, 0, 10);
          break;
        case 'm':
          // Maximum logfile size (in KB).  Must be a non-zero value.
          this->max_size_ = ACE_OS::strtoul (get_opt.optarg, 0, 10);
          if (this->max_size_ == 0)
            this->max_size_ = ACE_DEFAULT_MAX_LOGFILE_SIZE;
          this->max_size_ <<= 10;       // convert to KB
          break;
        case 'p':
          temp = get_opt.optarg;
          // Now tokenize the string to setup process log priority
          this->priorities (temp, ACE_Log_Msg::PROCESS);
          break;
        case 's':
          // Ensure that the OSTREAM flag is set
          ACE_SET_BITS (this->flags_, ACE_Log_Msg::OSTREAM);
          delete [] this->filename_;
          this->filename_ = ACE::strnew (get_opt.optarg);
          break;
        case 't':
          temp = get_opt.optarg;
          // Now tokenize the string to setup thread log priority
          this->priorities (temp, ACE_Log_Msg::THREAD);
          break;
        case 'w':
          // Cause the logfile to be wiped out, both on startup and on
          // reconfigure.
          this->wipeout_logfile_ = 1;
          break;
        default:
          break;
        }
    }
  return 0;
}

ACE_Logging_Strategy::ACE_Logging_Strategy (void)
{
#if defined (ACE_DEFAULT_LOGFILE)
  this->filename_ = ACE::strnew (ACE_DEFAULT_LOGFILE);
#else /* ACE_DEFAULT_LOGFILE */
  ACE_NEW (this->filename_, ACE_TCHAR[MAXPATHLEN + 1]);

  // Get the temporary directory
  if (ACE_Lib_Find::get_temp_dir (this->filename_,
                                  MAXPATHLEN - 7) == -1)  // 7 for "logfile"
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("Temporary path too long, ")
                  ACE_LIB_TEXT ("defaulting to current directory\n")));
      this->filename_[0] = 0;
    }

  // Add the filename to the end
  ACE_OS::strcat (this->filename_,
                  ACE_LIB_TEXT ("logfile"));
#endif /* ACE_DEFAULT_LOGFILE */
}

int
ACE_Logging_Strategy::fini (void)
{
  delete [] this->filename_;
  return 0;
}

int
ACE_Logging_Strategy::init (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("ACE_Logging_Strategy::init");

  // Store current priority masks for changes in <parse_args>.

  this->process_priority_mask_ =
    ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

  this->thread_priority_mask_ =
    ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::THREAD);

  // Use the options hook to parse the command line arguments.
  this->parse_args (argc, argv);

  // Setup priorities (to original if not specified on command line)

  ACE_Log_Msg::instance ()->priority_mask (thread_priority_mask_,
                                           ACE_Log_Msg::THREAD);

  ACE_Log_Msg::instance ()->priority_mask (process_priority_mask_,
                                           ACE_Log_Msg::PROCESS);

  // Check if any flags were specified. If none were specified, let
  // the default behavior take effect.
  if (this->flags_ != 0)
    {
      // Clear all flags
      ACE_Log_Msg::instance ()->clr_flags (ACE_Log_Msg::STDERR
                                          | ACE_Log_Msg::LOGGER
                                          | ACE_Log_Msg::OSTREAM
                                          | ACE_Log_Msg::VERBOSE
                                          | ACE_Log_Msg::VERBOSE_LITE
                                          | ACE_Log_Msg::SILENT);
      // Check if OSTREAM bit is set
      if (ACE_BIT_ENABLED (this->flags_,
                           ACE_Log_Msg::OSTREAM))
        {
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)
          FILE *output_file = 0;
          if (wipeout_logfile_)
            output_file = ACE_OS::fopen (this->filename_, "wt");
          else
            output_file = ACE_OS::fopen (this->filename_, "at");
          if (output_file == 0)
            return -1;
#else
          ofstream *output_file = 0;
          // Create a new ofstream to direct output to the file.
          if (wipeout_logfile_)
            ACE_NEW_RETURN (output_file,
                            ofstream (ACE_TEXT_ALWAYS_CHAR(this->filename_)),
                            -1);
          else
            ACE_NEW_RETURN (output_file,
                            ofstream (ACE_TEXT_ALWAYS_CHAR(this->filename_),
                                      ios::app | ios::out),
                            -1);
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
          // Set the <output_file> that'll be used by the rest of the
          // code.
          ACE_Log_Msg::instance ()->msg_ostream (output_file);

          // Setup a timeout handler to perform the maximum file size
          // check (if required).
          if (this->interval_ > 0)
            {
              if (this->reactor () == 0)
                this->reactor (ACE_Reactor::instance ());  // Use singleton
              this->reactor ()->schedule_timer (this, 0,
                                                ACE_Time_Value (this->interval_),
                                                ACE_Time_Value (this->interval_));
            }
        }
      // Now set the flags for Log_Msg
      ACE_Log_Msg::instance ()->set_flags (this->flags_);
    }

  return ACE_LOG_MSG->open (ACE_LIB_TEXT ("Logging_Strategy"),
                            ACE_LOG_MSG->flags (),
                            ACE_DEFAULT_LOGGER_KEY);
}

int
ACE_Logging_Strategy::handle_timeout (const ACE_Time_Value &,
                                      const void *)
{
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)
  if ((size_t) ACE_OS::fseek (ACE_LOG_MSG->msg_ostream (),
                              0,
                              SEEK_CUR) > this->max_size_)
#else
  if ((size_t) ACE_LOG_MSG->msg_ostream ()->tellp () > this->max_size_)
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
    {
      // Lock out any other logging.
      if (ACE_LOG_MSG->acquire ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("Cannot acquire lock!\n")),
                          -1);

      // Close the current ostream.
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)
      FILE *output_file = (FILE *) ACE_LOG_MSG->msg_ostream ();
      ACE_OS::fclose (output_file);
#else
      ofstream *output_file = (ofstream *) ACE_LOG_MSG->msg_ostream ();
      output_file->close ();
#endif /* ACE_LACKS_IOSTREAM_TOTALLY */
      // Save current logfile to logfile.old
      if (ACE_OS::strlen (this->filename_) + 4 <= MAXPATHLEN)   // 4 for ".old"
        {
          ACE_TCHAR backup[MAXPATHLEN+1];

          ACE_OS::strcpy (backup, this->filename_);
          ACE_OS::strcat (backup, ACE_LIB_TEXT (".old"));

          // Remove any existing .old file; ignore error as file may
          // not exist.
          ACE_OS::unlink (backup);

          // Rename the current log file to the name of the backup log
          // file.
          ACE_OS::rename (this->filename_,
                          backup);
        }
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("Backup file name too long; backup logfile not saved.\n")));

      // Open a new log file by the same name
      output_file->open (ACE_TEXT_ALWAYS_CHAR(this->filename_), ios::out);

      // Release the lock previously acquired.
      ACE_LOG_MSG->release ();
    }

  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Logging_Strategy.

ACE_FACTORY_DEFINE (ACE, ACE_Logging_Strategy)
