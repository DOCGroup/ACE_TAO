// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "Logging_Strategy.h"

ACE_RCSID(lib, Logging_Strategy, "$Id$")

// Parse the string containing all the flags and set the flags
// accordingly.

void
ACE_Logging_Strategy::tokenize (char *flag_string)
{
  for (char *flag = ACE_OS::strtok (flag_string, "|");
       flag != 0;
       flag = ACE_OS::strtok (0, "|"))
    {
      if (ACE_OS::strcmp (flag, "STDERR") == 0)
	ACE_SET_BITS (this->flags_, ACE_Log_Msg::STDERR);
      else if (ACE_OS::strcmp (flag, "LOGGER") == 0)
	ACE_SET_BITS (this->flags_, ACE_Log_Msg::LOGGER);
      else if (ACE_OS::strcmp (flag, "OSTREAM") == 0)
	ACE_SET_BITS (this->flags_, ACE_Log_Msg::OSTREAM);
      else if (ACE_OS::strcmp (flag, "VERBOSE") == 0)
	ACE_SET_BITS (this->flags_, ACE_Log_Msg::VERBOSE);
      else if (ACE_OS::strcmp (flag, "VERBOSE_LITE") == 0)
        ACE_SET_BITS (this->flags_, ACE_Log_Msg::VERBOSE_LITE);
      else if (ACE_OS::strcmp (flag, "SILENT") == 0)
	ACE_SET_BITS (this->flags_, ACE_Log_Msg::SILENT);
    }
}

int
ACE_Logging_Strategy::parse_args (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Logging_Strategy::parse_args");
  char *temp;

  this->flags_ = 0;
  this->wipeout_logfile_ = 0;
  this->interval_ = 0;
  this->max_size_ = ACE_DEFAULT_MAX_LOGFILE_SIZE;

  ACE_Get_Opt get_opt (argc, argv, "f:i:m:s:w", 0);

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
	case 's':
	  // Ensure that the OSTREAM flag is set
	  ACE_SET_BITS (this->flags_, ACE_Log_Msg::OSTREAM);
          delete [] this->filename_;
	  this->filename_ = ACE::strnew (get_opt.optarg);
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
  ACE_NEW (this->filename_, char[MAXPATHLEN + 1]);

  // Get the temporary directory
  if (ACE::get_temp_dir (this->filename_,
                         MAXPATHLEN - 7) == -1)  // 7 for "logfile"
    {
      ACE_ERROR ((LM_ERROR,
                  "Temporary path too long, defaulting to current directory\n"));
      this->filename_[0] = 0;
    }

  // Add the filename to the end
  ACE_OS::strcat (this->filename_,
                  "logfile");
#endif /* ACE_DEFAULT_LOGFILE */
}

int
ACE_Logging_Strategy::fini (void)
{
  delete [] this->filename_;
  return 0;
}

int
ACE_Logging_Strategy::init (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Logging_Strategy::init");

  // Use the options hook to parse the command line arguments.
  this->parse_args (argc, argv);

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
          ofstream *output_file = 0;
          // Create a new ofstream to direct output to the file.
          if (wipeout_logfile_)
            ACE_NEW_RETURN (output_file,
                            ofstream (this->filename_),
                            -1);
          else
            ACE_NEW_RETURN (output_file,
                            ofstream (this->filename_,
                                      ios::app | ios::out),
                            -1);

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

  return ACE_LOG_MSG->open ("Logging_Strategy",
                            ACE_LOG_MSG->flags (),
                            ACE_TEXT_CHAR_TO_TCHAR (ACE_DEFAULT_LOGGER_KEY));
}

int
ACE_Logging_Strategy::handle_timeout (const ACE_Time_Value &,
                                      const void *)
{
  if ((size_t) ACE_LOG_MSG->msg_ostream ()->tellp () > this->max_size_)
    {
      // Lock out any other logging.
      if (ACE_LOG_MSG->acquire ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot acquire lock!\n")),
                          -1);

      // Close the current ostream.
      ofstream *output_file =
        (ofstream *) ACE_LOG_MSG->msg_ostream ();
      output_file->close ();

      // Save current logfile to logfile.old
      if (ACE_OS::strlen (this->filename_) + 4 <= MAXPATHLEN)	// 4 for ".old"
        {
          char backup[MAXPATHLEN+1];
  
          ACE_OS::strcpy (backup, this->filename_);
          ACE_OS::strcat (backup, ".old");

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
                    ACE_TEXT ("Backup file name too long; backup logfile not saved.\n")));

      // Open a new log file by the same name
      output_file->open (this->filename_, ios::out);

      // Release the lock previously acquired.
      ACE_LOG_MSG->release ();
    }

  return 0;
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Logging_Strategy.

ACE_SVC_FACTORY_DEFINE (ACE_Logging_Strategy)
