// $Id$
#include "File_Logger_Backend.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Record.h"
#include "ace/Log_Msg.h"
#include "Log_Macros.h"

namespace DAnCE
  {

  int
  File_Logger_Backend::open (const ACE_TCHAR *)
  {
    DANCE_DEBUG (6, (LM_DEBUG, "Setting logger's output to file \"%s\"\n", this->filename_.c_str()));
    this->fh_ = ACE_OS::fopen (this->filename_.c_str(), "a+");
    if (!this->fh_)
      {
        ACE_TString s = ACE_TEXT("Failed to open log file \"");
        s += this->filename_;
        s += ACE_TEXT("\"\n");
        throw LoggerError (s.c_str());
      }
    return 0;
  }

  int
  File_Logger_Backend::close (void)
  {
    if (this->fh_)
      {
        ACE_OS::fclose (this->fh_);
        this->fh_ = 0;
      }
    return 0;
  }

  ssize_t
  File_Logger_Backend::log (ACE_Log_Record &log_record)
  {
    int const res = log_record.print (0, ACE_Log_Msg::VERBOSE, this->fh_);
    ACE_OS::fflush (this->fh_);
    return res;
  }

}

