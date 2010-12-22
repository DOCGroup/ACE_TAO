// $Id$

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_ctype.h"
#include "ace/String_Base.h"
#include "ace/INet/FTP_Response.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/FTP_Response.inl"
#endif

#include "ace/INet/INet_Log.h"
#include "ace/INet/String_IOStream.h"



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {
    const int Response::eof_ = std::char_traits<char>::eof ();

    Response::Response()
      : status_ (NORESPONSE)
      {
      }

    Response::~Response()
      {
      }

    void Response::write(ostream& str) const
      {
        ACE_Array<ACE_CString>::size_type n = 0;
        str << this->status_;
        if (this->response_.size () > 0)
          {
            n = this->response_.size () - 1;
            str << (n > 0 ? '-' : ' ') << this->response_[0].c_str ();
          }
        str << "\r\n";
        for (ACE_Array<ACE_CString>::size_type i = 1;
             i < n;
             ++i)
          {
            str << this->response_[i].c_str () << "\r\n";
          }
        if (n > 0)
          {
            str << this->status_ << ' '
                << this->response_[n].c_str () << "\r\n";
          }
      }

    bool Response::read(istream& str)
      {
        int ch;
        str >> this->status_;
        ch = str.get ();
        if (str.bad () || this->status_type () == NOSTATE || (ch != ' ' && ch != '-'))
          {
            return false; // invalid status
          }

        bool multi_line = (ch == '-');

        ACE_Array<ACE_CString>::size_type n =
            this->response_.size ();
        this->response_.size (n+1);
        this->response_[n].clear ();
        ACE::IOS::CString_OStream sos (this->response_[n]);
        sos << this->status_;
        sos.put (ch);
        ch = this->read_line (str, sos);
        if (ch == '\r') ch = str.get ();
        sos.close (); // close the stream before resizing the array invalidates the string reference

        INET_DEBUG (6, (LM_DEBUG, DLINFO
                        ACE_TEXT ("ACE_INet_FTP: <-- %C\n"),
                        this->response_[n].c_str ()));

        if (multi_line)
          {
            while (ch != eof_)
              {
                int nxt_stat = 0;

                n = this->response_.size ();
                this->response_.size (n+1);
                this->response_[n].clear ();
                ACE::IOS::CString_OStream nxt_sos (this->response_[n]);

                if (ACE_OS::ace_isdigit (str.peek ()))
                  {
                    str >> nxt_stat;
                    ch = str.get ();
                    if (str.bad () || (nxt_stat == this->status_ && ch != ' '))
                      {
                        this->status_ = NORESPONSE;
                        return false;
                      }
                    nxt_sos << nxt_stat;
                    nxt_sos.put(ch);
                  }
                ch = this->read_line (str, nxt_sos);

                nxt_sos.close ();

                INET_DEBUG (9, (LM_DEBUG, DLINFO
                                ACE_TEXT ("ACE_INet_FTP: <-+ %C\n"),
                                this->response_[n].c_str ()));

                if (nxt_stat == this->status_)
                  {
                    return true;
                  }
              }

            this->status_ = NORESPONSE;
            return false;
          }
        else
          {
            return true;
          }
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
