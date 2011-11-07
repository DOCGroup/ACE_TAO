// $Id$

#include "ace/String_Base.h"
#include "ace/OS_NS_ctype.h"
#include "ace/INet/FTP_Request.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/FTP_Request.inl"
#endif

#include "ace/INet/INet_Log.h"
#include "ace/INet/String_IOStream.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    const ACE_CString Request::FTP_USER = "USER";
    const ACE_CString Request::FTP_PASS = "PASS";
    const ACE_CString Request::FTP_QUIT = "QUIT";
    const ACE_CString Request::FTP_TYPE = "TYPE";
    const ACE_CString Request::FTP_SYST = "SYST";
    const ACE_CString Request::FTP_PWD  = "PWD";
    const ACE_CString Request::FTP_CWD  = "CWD";
    const ACE_CString Request::FTP_CDUP = "CDUP";
    const ACE_CString Request::FTP_RNFR = "RNFR";
    const ACE_CString Request::FTP_RNTO = "RNTO";
    const ACE_CString Request::FTP_DELE = "DELE";
    const ACE_CString Request::FTP_MKD  = "MKD";
    const ACE_CString Request::FTP_RMD  = "RMD";
    const ACE_CString Request::FTP_RETR = "RETR";
    const ACE_CString Request::FTP_STOR = "STOR";
    const ACE_CString Request::FTP_LIST = "LIST";
    const ACE_CString Request::FTP_NLST = "NLST";
    const ACE_CString Request::FTP_ABOR = "ABOR";
    const ACE_CString Request::FTP_EPRT = "EPRT";
    const ACE_CString Request::FTP_PORT = "PORT";
    const ACE_CString Request::FTP_EPSV = "EPSV";
    const ACE_CString Request::FTP_PASV = "PASV";
    const ACE_CString Request::FTP_STAT = "STAT";

    const int Request::eof_ = std::char_traits<char>::eof ();

    Request::Request()
      {
      }

    Request::~Request()
      {
      }

    void Request::arguments (ACE_Array<ACE_CString> & args) const
      {
        ACE::IOS::CString_IStream sis (this->args_);

        int ch = sis.get ();
        while (ch != eof_)
          {
            // skip whitespace
            while (ACE_OS::ace_isspace (ch))  ch = sis.get ();
            if (ch != eof_)
              {
                // get arg
                ACE_Array<ACE_CString>::size_type n =
                    args.size ();
                args.size (n + 1);
                ACE_CString& arg = args[n];
                while (ch != eof_ && !ACE_OS::ace_isspace (ch))
                  {
                    arg += ch;
                    ch = sis.get ();
                  }
              }
          }
      }

    void Request::write(std::ostream& str) const
      {
        str << this->command_.c_str ();
        if (!this->args_.empty ())
          str << ' ' << this->args_.c_str ();
        str << "\r\n";

        INET_DEBUG (6, (LM_DEBUG, DLINFO
                        ACE_TEXT ("ACE_INet_FTP: --> %C %C\n"),
                        this->command_.c_str (),
                        this->command_ == FTP_PASS ?
                            "***" : this->args_.c_str ()));
      }

    bool Request::read(std::istream& str)
      {
        ACE_CString cmd (4, '\0');
        ACE_CString args (128, '\0');

        int ch =  str.peek ();
        if (ch == eof_)
          {
            str.get (); // skip to eof
            return false;
          }
        // skip whitespace
        while (ACE_OS::ace_isspace (str.peek ()))
          {
            str.get ();
          }
        // get method
        ch = str.get ();
        while (!ACE_OS::ace_isspace (ch) && ch != eof_ && cmd.length () < MAX_CMD_LENGTH)
          {
            cmd += ch;
            ch = str.get ();
          }
        if (!ACE_OS::ace_isspace (ch))
          return false; // invalid FTP command string
        if (ch != '\r' && ch != '\n')
          {
            // skip whitespace
            while (ACE_OS::ace_isspace (str.peek ()))
              {
                str.get ();
              }
            // get arguments
            ch = str.get ();
            while (ch != eof_ && ch != '\r' && ch != '\n' && args.length () < MAX_ARG_LENGTH)
              {
                args += ch;
                ch = str.get ();
              }
            if (ch != eof_ && ch != '\r' && ch != '\n')
              {
                return false; // args too long
              }
          }
        if (ch == '\r')
          {
            str.get ();
          }
        this->command (cmd);
        this->args_ = args;
        return true;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
