// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    ACE_INLINE
    void Response::reset ()
      {
        this->status_ = NORESPONSE;
        this->response_.size (0);
      }

    ACE_INLINE
    void Response::status(int status)
      {
        this->status_ = status;
      }

    ACE_INLINE
    int Response::status() const
      {
        return this->status_;
      }

    ACE_INLINE
    Response& Response::operator ()(int status)
      {
        this->reset ();
        this->status (status);
        return *this;
      }

    ACE_INLINE
    const ACE_Array<ACE_CString>& Response::response () const
      {
        return this->response_;
      }

    ACE_INLINE
    Response& Response::operator <<(const ACE_CString& line)
      {
        ACE_Array<ACE_CString>::size_type n =
            this->response_.size ();
        this->response_.size (n+1);
        this->response_[n] = line;
        return *this;
      }

    ACE_INLINE
    Response::StatusType Response::status_type (int status)
      {
        if (status == NORESPONSE)
          return NORESPONSE;
        int st = status / 100;
        if (st >= PRELIM_OK && st <= PERMANENT_FAIL)
          return static_cast<StatusType> (st);
        else
          return NOSTATE;
      }

    ACE_INLINE
    Response::StatusType Response::status_type () const
      {
        return status_type (this->status_);
      }

    ACE_INLINE
    Response::StatusSubtype Response::status_sub_type () const
      {
        StatusType st = this->status_type ();
        if (st != NOSTATE && st != NORESPONSE)
          {
            int sst = (this->status_ - (st * 100)) / 10;
            if (sst >= SYNTAX && sst <= FILESYSTEM)
              return static_cast<StatusSubtype> (sst);
          }
        return NOSUBTYPE;
      }

    ACE_INLINE
    bool Response::is_preliminary_ok () const
      {
        return this->status_type () == PRELIM_OK;
      }

    ACE_INLINE
    bool Response::is_completed_ok () const
      {
        return this->status_type () == COMPLETED_OK;
      }

    ACE_INLINE
    bool Response::is_intermediate_ok () const
      {
        return this->status_type () == INTERMEDIATE_OK;
      }

    ACE_INLINE
    bool Response::is_transient_fail () const
      {
        return this->status_type () == TRANSIENT_FAIL;
      }

    ACE_INLINE
    bool Response::is_permanent_fail () const
      {
        return this->status_type () == PERMANENT_FAIL;
      }

    ACE_INLINE
    int Response::read_line (std::istream& is, std::ostream& os)
      {
        int ch;
        for (ch = is.get ();
             ch != eof_ && ch != '\r' && ch != '\n';
             ch = is.get ())
          {
            os.put (ch);
          }
        return ch;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
