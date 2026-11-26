/**
 * @file HeaderBase.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_HEADER_BASE_H
#define ACE_INET_HEADER_BASE_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "ace/Containers_T.h"
#include "ace/OS_NS_ctype.h"
#include "ace/INet/INet_Export.h"
#include <cstdlib>
#include <iostream>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_NVPair
        *
        * @brief Name/Value pair holder class.
        *
        */
        class ACE_INET_Export NVPair
          {
            public:
              NVPair ();
              NVPair (ACE_CString const& first);
              NVPair (ACE_CString const& first, ACE_CString const& second);
              NVPair (const NVPair& pair);
              virtual ~NVPair ();
              NVPair& operator =(const NVPair& pair);
              bool operator ==(const NVPair& pair) const;
              bool operator <(const NVPair& pair) const;

              ACE_CString const& first () const;

              void first (ACE_CString const& t1);

              ACE_CString const& second () const;

              void second (ACE_CString const& t2);

            private:
              ACE_CString first_;
              ACE_CString second_;
          };

        using TNVMap = ACE_Ordered_MultiSet<NVPair>;

        /**
        * @class ACE_INet_HeaderBase
        *
        * @brief Encapsulates message header.
        *
        */
        class ACE_INET_Export HeaderBase
          {
            public:
              HeaderBase();

              virtual ~HeaderBase();

              /// Sets the Content-Length header.
              /// Removes the Content-Length header if
              /// length is UNKNOWN_CONTENT_LENGTH.
              virtual void set_content_length(int length);

              /// Returns the content length for this message.
              /// Returns UNKNOWN_CONTENT_LENGTH if no
              /// Content-Length header is present.
              virtual int get_content_length() const;

              /// Sets the content type for this message.
              /// Removes the Content-Type header if
              /// UNKNOWN_CONTENT_TYPE is specified.
              virtual void set_content_type(ACE_CString const& mime_type);

              /// Returns the content type for this message.
              /// Returns UNKNOWN_CONTENT_TYPE if
              /// no Content-Type header is present.
              virtual ACE_CString get_content_type() const;

              void clear ();

              /// Writes the headers to the given stream
              virtual void write(std::ostream& str) const;

              /// Reads the headers from the
              /// given stream.
              virtual bool read(std::istream& str);

              static int const         UNKNOWN_CONTENT_LENGTH;
              static ACE_CString const UNKNOWN_CONTENT_TYPE;

              static ACE_CString const CONTENT_LENGTH;
              static ACE_CString const CONTENT_TYPE;

              static ACE_CString const EMPTY;

              /// Sets header @a name to @a value. Overwrites existing vaues.
              void    set (ACE_CString const& name, ACE_CString const& value);

              /// Adds header @a name with @a value. Allows duplicates.
              void    add (ACE_CString const& name, ACE_CString const& value);

              /// Removes header @a name (first found).
              void    remove (ACE_CString const& name);

              /// Retrieves value for header @a name into @a value (first found).
              bool    get (ACE_CString const& name, ACE_CString& value) const;

              /// Returns true if a header @a name exists (1 or more), false otherwise.
              bool    has (ACE_CString const& name) const;

              /// Retrieves values for all headers @a name into @a values.
              void    get_values (ACE_CString const& name, ACE_Array<ACE_CString>& values) const;

            protected:
              int     read_field (std::istream& str, ACE_CString& var, size_t maxlen, char delim);
              int     read_ws_field (std::istream& str, ACE_CString& var, size_t maxlen);

              static int const eof_;

            private:
              /// Limits for reading a header
              enum Limits
                {
                  MAX_NAME_LENGTH  = 256,
                  MAX_VALUE_LENGTH = 4096
                };

              TNVMap  header_values_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/HeaderBase.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_HEADER_BASE_H */
