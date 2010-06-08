// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTTP
  {

    ACE_INLINE
    void Header::set_version(const ACE_CString& version)
      {
        this->version_ = version;
      }

    ACE_INLINE
    const ACE_CString& Header::get_version() const
      {
        return this->version_;
      }

    ACE_INLINE
    void Header::set_chunked_transfer_encoding(bool flag)
      {
        if (flag)
          {
            this->set (TRANSFER_ENCODING, CHUNKED_TRANSFER_ENCODING);
          }
        else
          {
            this->remove (TRANSFER_ENCODING);
          }
      }

    ACE_INLINE
    bool Header::has_chunked_transfer_encoding() const
      {
        ACE_CString val;
        return this->get (TRANSFER_ENCODING, val) &&
          (val == CHUNKED_TRANSFER_ENCODING);
      }

    ACE_INLINE
    void Header::set_keep_alive(bool keep_alive)
      {
        this->set (CONNECTION, keep_alive ?
                      CONNECTION_KEEP_ALIVE :
                      CONNECTION_CLOSE);
      }

    ACE_INLINE
    bool Header::has_keep_alive() const
      {
        ACE_CString val;
        return this->get (CONNECTION, val) &&
                  (val == CONNECTION_KEEP_ALIVE);
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
