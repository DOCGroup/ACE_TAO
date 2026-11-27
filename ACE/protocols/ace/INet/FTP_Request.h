/**
 * @file FTP_Request.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_REQUEST_H
#define ACE_FTP_REQUEST_H

#include /**/ "ace/pre.h"

#include "ace/Containers_T.h"
#include "ace/INet/INet_Export.h"
#include "ace/INet/Request.h"
#include "ace/SString.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace FTP
      {
        /**
        * @class ACE_FTP_Request
        *
        * @brief Encapsulates FTP requests/commands.
        *
        */
        class ACE_INET_Export Request
          : public ACE::INet::Request
          {
            public:
              Request ();

              virtual ~Request ();

              /// resets the request object
              void reset ();

              /// resets the request object and sets
              /// new command
              Request& operator ()(ACE_CString const& cmd);

              /// sets the current command
              void command (ACE_CString const& cmd);

              /// returns the current command
              ACE_CString const& command () const;

              /// adds a command argument
              /// separates multiple arguments with a space
              Request& operator <<(ACE_CString const& arg);

              /// returns the arguments string
              ACE_CString const& arguments () const;

              /// retrieves the arguments
              void arguments (ACE_Array<ACE_CString> & args) const;

              /// Writes the FTP request to the given stream
              void write(std::ostream& str) const;

              /// Reads the FTP request from the
              /// given stream.
              bool read(std::istream& str);

              static ACE_CString const FTP_USER;
              static ACE_CString const FTP_PASS;
              static ACE_CString const FTP_QUIT;
              static ACE_CString const FTP_TYPE;
              static ACE_CString const FTP_SYST;
              static ACE_CString const FTP_PWD;
              static ACE_CString const FTP_CWD;
              static ACE_CString const FTP_CDUP;
              static ACE_CString const FTP_RNFR;
              static ACE_CString const FTP_RNTO;
              static ACE_CString const FTP_DELE;
              static ACE_CString const FTP_MKD;
              static ACE_CString const FTP_RMD;
              static ACE_CString const FTP_RETR;
              static ACE_CString const FTP_STOR;
              static ACE_CString const FTP_LIST;
              static ACE_CString const FTP_NLST;
              static ACE_CString const FTP_ABOR;
              static ACE_CString const FTP_EPRT;
              static ACE_CString const FTP_PORT;
              static ACE_CString const FTP_EPSV;
              static ACE_CString const FTP_PASV;
              static ACE_CString const FTP_STAT;

            private:
              static int const eof_;

              enum Limits
              {
                MAX_CMD_LENGTH  = 4,
                MAX_ARG_LENGTH  = 4096  // seems like a sensible maximum
              };

              ACE_CString command_;
              ACE_CString args_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_Request.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_FTP_REQUEST_H */
