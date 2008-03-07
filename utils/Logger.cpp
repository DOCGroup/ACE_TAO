#include "Logger.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "RACE_utils.h"

namespace CIAO
{

  namespace RACE
  {
    Logger::Logger(const char* filename)
    {
      this->log_file_ = ACE_OS::fopen (filename, "w");
      if (this->log_file_ == 0)
      {
        ACE_DEBUG ((LM_ERROR,
          "Cannot open output log file for writing: %s\n",
          filename));
      }
    }

    Logger::~Logger(void)
    {
      if (this->log_file_ != 0)
      {
        ACE_OS::fclose (this->log_file_);
      }
    }

    void
      Logger::log (std::string msg)
    {
      if (this->log_file_ != 0)
      {
        ACE_OS::fprintf (this->log_file_, "%s", msg.c_str());
        ACE_OS::fflush (this->log_file_);
      }

      if (RACE::debug_level() > 10)
      {
        ACE_DEBUG ((LM_DEBUG, "RACE::Debug: %s", msg.c_str()));
      }
    }


  }
}
