// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {
    ACE_INLINE
    void Status::set_status(Code status)
      {
        this->code_ = status;
      }

    ACE_INLINE
    Status::Code Status::get_status() const
      {
        return this->code_;
      }

    ACE_INLINE
    void Status::set_reason(const ACE_CString& reason)
      {
        this->reason_ = reason;
      }

    ACE_INLINE
    const ACE_CString& Status::get_reason() const
      {
        return this->reason_;
      }

    ACE_INLINE
    void Status::set_status_and_reason(Code status)
      {
        this->reason_ = get_reason (this->code_ = status);
      }

    ACE_INLINE
    bool Status::is_valid () const
      {
        return this->code_ != INVALID;
      }

    ACE_INLINE
    bool Status::is_ok () const
      {
        return this->code_ >= HTTP_OK && this->code_ < HTTP_BAD_REQUEST;
      }

    ACE_INLINE
    Status::operator bool() const
      {
        return this->is_valid ();
      }

    ACE_INLINE
    bool Status::operator !() const
      {
        return !this->is_valid ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
