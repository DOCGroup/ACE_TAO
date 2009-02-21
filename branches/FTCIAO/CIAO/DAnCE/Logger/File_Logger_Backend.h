// $Id$
#ifndef FILE_LOGGER_BACKEND_H_
#define FILE_LOGGER_BACKEND_H_

#include "ace/Log_Msg_Backend.h"
#include "DAnCE_Logger_Export.h"
#include "ace/SString.h"

namespace DAnCE
  {

  class DAnCE_Logger_Export LoggerError
    {
    public:
      LoggerError (const char * msg)
          : errmsg_ (msg) {};
      ACE_CString errmsg_;
    };

  class DAnCE_Logger_Export File_Logger_Backend : public ACE_Log_Msg_Backend
    {
    public:
      File_Logger_Backend (const char * fname)
          : fh_ (0), filename_ (fname) {}

      virtual ~File_Logger_Backend (void)
      {
        this->close();
      };

      virtual int open (const ACE_TCHAR *logger_key);

      virtual int reset (void)
      {
        this->close();
        return this->open (0);
      };

      virtual int close (void);

      virtual ssize_t log (ACE_Log_Record &log_record);

    private:
      FILE * fh_;
      ACE_CString filename_;
    };

} // DAnCE

#endif /*FILE_LOGGER_BACKEND_H_*/
