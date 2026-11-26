ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    void Response::reset ()
      {
        this->clear ();
        this->set_version (EMPTY);
        this->status_ = Status ();
      }

    ACE_INLINE
    void Response::reset (Status const& status)
      {
        this->clear ();
        this->set_version (EMPTY);
        this->status_ = status;
      }

    ACE_INLINE
    void Response::reset (ACE_CString const& version, Status const& status)
      {
        this->clear ();
        this->set_version (version);
        this->status_ = status;
      }

    ACE_INLINE
    void Response::set_status(Status const& status)
      {
        this->status_ = status;
      }

    /// Return the status.
    ACE_INLINE
    Status const& Response::get_status() const
      {
        return this->status_;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
