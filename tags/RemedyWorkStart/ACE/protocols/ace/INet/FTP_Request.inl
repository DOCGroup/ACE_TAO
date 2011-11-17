// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace FTP
  {

    ACE_INLINE
    void Request::reset ()
      {
        this->command_.clear ();
        this->args_.clear ();
      }

    ACE_INLINE
    Request& Request::operator ()(const ACE_CString& cmd)
      {
        this->reset ();
        this->command (cmd);
        return *this;
      }

    ACE_INLINE
    void Request::command (const ACE_CString& cmd)
      {
        this->command_ = cmd;
      }

    ACE_INLINE
    const ACE_CString& Request::command () const
      {
        return this->command_;
      }

    ACE_INLINE
    Request& Request::operator <<(const ACE_CString& arg)
      {
        if (!arg.empty ())
          {
            if (!this->args_.empty ())  this->args_ += ' ' ;
            this->args_ += arg;
          }
        return *this;
      }

    ACE_INLINE
    const ACE_CString& Request::arguments () const
      {
        return this->args_;
      }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
