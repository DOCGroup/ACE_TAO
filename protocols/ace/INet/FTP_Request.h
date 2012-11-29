// $Id$

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
              Request& operator ()(const ACE_CString& cmd);

              /// sets the current command
              void command (const ACE_CString& cmd);

              /// returns the current command
              const ACE_CString& command () const;

              /// adds a command argument
              /// separates multiple arguments with a space
              Request& operator <<(const ACE_CString& arg);

              /// returns the arguments string
              const ACE_CString& arguments () const;

              /// retrieves the arguments
              void arguments (ACE_Array<ACE_CString> & args) const;

              /// Writes the FTP request to the given stream
              void write(std::ostream& str) const;

              /// Reads the FTP request from the
              /// given stream.
              bool read(std::istream& str);

              static const ACE_CString FTP_USER;
              static const ACE_CString FTP_PASS;
              static const ACE_CString FTP_QUIT;
              static const ACE_CString FTP_TYPE;
              static const ACE_CString FTP_SYST;
              static const ACE_CString FTP_PWD;
              static const ACE_CString FTP_CWD;
              static const ACE_CString FTP_CDUP;
              static const ACE_CString FTP_RNFR;
              static const ACE_CString FTP_RNTO;
              static const ACE_CString FTP_DELE;
              static const ACE_CString FTP_MKD;
              static const ACE_CString FTP_RMD;
              static const ACE_CString FTP_RETR;
              static const ACE_CString FTP_STOR;
              static const ACE_CString FTP_LIST;
              static const ACE_CString FTP_NLST;
              static const ACE_CString FTP_ABOR;
              static const ACE_CString FTP_EPRT;
              static const ACE_CString FTP_PORT;
              static const ACE_CString FTP_EPSV;
              static const ACE_CString FTP_PASV;
              static const ACE_CString FTP_STAT;

            private:
              static const int eof_;

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
