// $Id$

#include "ace/INet/HeaderBase.h"
#include "ace/OS_NS_stdlib.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/HeaderBase.inl"
#endif

#include "ace/INet/INet_Log.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    const int         HeaderBase::UNKNOWN_CONTENT_LENGTH     = -1;
    const ACE_CString HeaderBase::UNKNOWN_CONTENT_TYPE;
    const ACE_CString HeaderBase::CONTENT_LENGTH             = "Content-Length";
    const ACE_CString HeaderBase::CONTENT_TYPE               = "Content-Type";
    const ACE_CString HeaderBase::EMPTY;

    const int HeaderBase::eof_ = std::char_traits<char>::eof();

    HeaderBase::HeaderBase()
      {
      }

    HeaderBase::~HeaderBase()
      {
      }

    void HeaderBase::set_content_length(int length)
      {
        if (length == UNKNOWN_CONTENT_LENGTH)
          {
            this->remove (CONTENT_LENGTH);
          }
        else
          {
            char buf[32];
            this->set (CONTENT_LENGTH,
                       ACE_OS::itoa (length, buf, 10));
          }
      }

    void HeaderBase::write(std::ostream& str) const
      {
        TNVMap::ITERATOR it (const_cast<TNVMap&> (this->header_values_));
        for (it.first (); !it.done () ;it.advance ())
        {
          str << (*it).first ().c_str () << ": " << (*it).second ().c_str () << "\r\n";

          INET_DEBUG (9, (LM_DEBUG, DLINFO
                          ACE_TEXT ("ACE_INet_HTTP: +-> %C: %C\n"),
                          (*it).first ().c_str (),
                          (*it).second ().c_str ()));
        }
      }

    bool HeaderBase::read(std::istream& str)
      {

        ACE_CString name (64, '\0');
        ACE_CString value (128, '\0');
        int ch = str.peek ();
        while (ch != eof_ && ch != '\r' && ch != '\n')
          {
            name.fast_clear ();
            value.fast_clear ();
            // parse name
            ch = this->read_field (str, name, MAX_NAME_LENGTH, ':');
            if (ch == '\n')
              {
                ch = str.get ();
                continue; // ignore invalid headers
              }
            if (ch != ':')
              {
                return false; // name too long/missing colon; cannot continue
              }

            // skip leading whitespace before next field
            while (ACE_OS::ace_isspace (str.peek ()))
              {
                ch = str.get ();
              }

            // parse value
            ch = this->read_field (str, value, MAX_VALUE_LENGTH, '\r');
            if (ch == '\r')
              ch = str.get (); // get lf
            if (ch != '\n')
              return false; // value too long/no crlf found; cannot continue

            // followup lines starting with ws are continuations of the value
            // and must be appended
            ch = str.peek ();
            while (ch == ' ' || ch == '\t')
            {
              ch = this->read_field (str, value, MAX_VALUE_LENGTH, '\r');
              if (ch == '\r')
                ch = str.get (); // get lf
              if (ch != '\n')
                return false; // multiline value too long/no crlf; cannot continue

              ch = str.peek ();
            }

            this->add (name, value);

            INET_DEBUG (9, (LM_DEBUG, DLINFO
                            ACE_TEXT ("ACE_INet_HTTP: <-+ %C: %C\n"),
                            name.c_str (),
                            value.c_str ()));
          }
        return true;
      }


    void HeaderBase::set (const ACE_CString& name, const ACE_CString& value)
      {
        TNVMap::ITERATOR it (this->header_values_);
        if (this->header_values_.find (NVPair (name), it) == 0)
          {
            (*it).second (value);
          }
        else
          {
            this->header_values_.insert (NVPair (name, value));
          }
      }

    void HeaderBase::get_values(const ACE_CString& name, ACE_Array<ACE_CString> & values) const
      {
        TNVMap::ITERATOR it (const_cast<TNVMap&> (this->header_values_));
        if (this->header_values_.find (name, it) == 0)
          {
            for (; !it.done () && ((*it).second () == name) ;it.advance ())
              {
                if (values.size (values.size ()+1) == 0)
                  {
                    values.set ((*it).second (), values.size ()-1);
                  }
              }
          }
      }


    NVPair::NVPair () {}

    NVPair::NVPair (const ACE_CString& first)
      : first_ (first)
      {
      }

    NVPair::NVPair (const ACE_CString& first, const ACE_CString& second)
      : first_ (first), second_ (second)
      {
      }

    NVPair::NVPair (const NVPair& pair)
      {
        *this = pair;
      }

    NVPair::~NVPair () {}

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
