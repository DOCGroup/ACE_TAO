// $Id$

/**
 * @file FTP_Response.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_FTP_RESPONSE_H
#define ACE_FTP_RESPONSE_H

#include /**/ "ace/pre.h"

#include "ace/INet/INet_Export.h"
#include "ace/INet/Response.h"
#include "ace/SString.h"
#include "ace/Containers_T.h"
#include <iostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace FTP
      {

        /**
        * @class ACE_FTP_Response
        *
        * @brief Encapsulates FTP responses.
        *
        */
        class ACE_INET_Export Response
          : public ACE::INet::Response
          {
            public:
              Response ();

              virtual ~Response ();

              /// resets the FTP response
              void reset ();

              /// resets the FTP response and sets
              /// the response status
              Response& operator ()(int status);

              /// returns the current response status
              int status () const;

              /// sets the current response status
              void status (int status);

              /// retrieves the current response line(s)
              const ACE_Array<ACE_CString>& response () const;

              /// adds a response line
              Response& operator <<(const ACE_CString& line);

              /// Writes the FTP response to the given stream
              void write(std::ostream& str) const;

              /// Reads the FTP response from the
              /// given stream.
              bool read(std::istream& str);

              /// response types
              enum StatusType
              {
                NOSTATE = -1,
                NORESPONSE = 0,
                PRELIM_OK = 1,
                COMPLETED_OK = 2,
                INTERMEDIATE_OK = 3,
                TRANSIENT_FAIL = 4,
                PERMANENT_FAIL = 5
              };

              /// response subtypes
              enum StatusSubtype
              {
                NOSUBTYPE = -1,
                SYNTAX = 0,
                INFORMATION = 1,
                CONNECTION = 2,
                AUTHENTICATION = 3,
                NONE = 4,
                FILESYSTEM = 5
              };

              /// returns the current response type
              StatusType status_type () const;

              /// returns true if the current response type is PRELIM_OK
              bool is_preliminary_ok () const;

              /// returns true if the current response type is COMPLETED_OK
              bool is_completed_ok () const;

              /// returns true if the current response type is INTERMEDIATE_OK
              bool is_intermediate_ok () const;

              /// returns true if the current response type is TRANSIENT_FAIL
              bool is_transient_fail () const;

              /// returns true if the current response type is PERMANENT_FAIL
              bool is_permanent_fail () const;

              /// returns the current response sub type
              StatusSubtype status_sub_type () const;

              static StatusType status_type (int status);

            private:
              static const int eof_;

              int read_line (std::istream& is, std::ostream& os);

              int status_;
              ACE_Array<ACE_CString> response_;
          };

      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/FTP_Response.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_FTP_RESPONSE_H */
