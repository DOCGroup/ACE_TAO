// $Id$

#define ACE_BUILD_SVC_DLL

#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "Logging_Strategy.h"

ACE_RCSID(lib, Logging_Strategy, "$Id$")

// Parse the string containing all the flags and set the flags accordingly
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

  ACE_Get_Opt get_opt (argc, argv, "f:s:w", 0);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'f':
	  temp = get_opt.optarg;
	  // Now tokenize the string to get all the flags
	  this->tokenize (temp);
	  break;
	case 's':
	  // Ensure that the OSTREAM flag is set
	  ACE_SET_BITS (this->flags_, ACE_Log_Msg::OSTREAM);
          ACE_OS::free ((void *) this->filename_);
	  this->filename_ = ACE_OS::strdup (get_opt.optarg);
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
  : filename_ (ACE_OS::strdup (ACE_DEFAULT_LOGFILE))
{
}

int
ACE_Logging_Strategy::fini (void)
{
  ACE_OS::free ((void *) this->filename_);
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
      ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::STDERR  
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
          ACE_Log_Msg::instance()->msg_ostream (output_file);
	}
      // Now set the flags for Log_Msg
      ACE_Log_Msg::instance()->set_flags (this->flags_);
    }

  return ACE_LOG_MSG->open ("Logging_Strategy",
                            ACE_LOG_MSG->flags (),
                            ACE_WIDE_STRING (ACE_DEFAULT_LOGGER_KEY));
}

// The following is a "Factory" used by the ACE_Service_Config and
// svc.conf file to dynamically initialize the state of the Logging_Strategy.

ACE_SVC_FACTORY_DEFINE (ACE_Logging_Strategy)
