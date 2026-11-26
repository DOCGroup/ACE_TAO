ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    int HeaderBase::get_content_length() const
      {
        ACE_CString lenstr;
        if (this->get (CONTENT_LENGTH, lenstr))
          {
            return std::atoi (lenstr.c_str ());
          }
        return UNKNOWN_CONTENT_LENGTH;
      }

    ACE_INLINE
    void HeaderBase::set_content_type(ACE_CString const& mime_type)
      {
        if (mime_type == UNKNOWN_CONTENT_TYPE)
          {
            this->remove (CONTENT_TYPE);
          }
        else
          {
            this->set (CONTENT_TYPE, UNKNOWN_CONTENT_TYPE);
          }
      }

    ACE_INLINE
    ACE_CString HeaderBase::get_content_type() const
      {
        ACE_CString val = UNKNOWN_CONTENT_TYPE;
        this->get (CONTENT_TYPE, val);
        return val;
      }

    ACE_INLINE
    void HeaderBase::clear ()
      {
        this->header_values_.reset ();
      }

    ACE_INLINE
    void HeaderBase::add (ACE_CString const& name, ACE_CString const& value)
      {
        this->header_values_.insert (NVPair (name, value));
      }

    ACE_INLINE
    void HeaderBase::remove (ACE_CString const& name)
      {
        this->header_values_.remove (NVPair (name, EMPTY));
      }

    ACE_INLINE
    bool HeaderBase::get (ACE_CString const& name, ACE_CString& value) const
      {
        TNVMap::ITERATOR it (const_cast<TNVMap&> (this->header_values_));
        if (this->header_values_.find (NVPair (name), it) == 0)
          {
            value = (*it).second ();
            return true;
          }
        return false;
      }

    ACE_INLINE
    bool HeaderBase::has (ACE_CString const& name) const
      {
        TNVMap::ITERATOR it (const_cast<TNVMap&> (this->header_values_));
        if (this->header_values_.find (NVPair (name), it) == 0)
          {
            return true;
          }
        return false;
      }

    ACE_INLINE
    int HeaderBase::read_field (std::istream& str, ACE_CString& var, size_t maxlen, char delim)
      {
        int ch = str.get ();
        while (ch != eof_ && ch != delim && ch != '\n' && var.length () < maxlen)
          {
            var += ch;
            ch = str.get ();
          }
        return ch;
      }

    ACE_INLINE
    int HeaderBase::read_ws_field (std::istream& str, ACE_CString& var, size_t maxlen)
      {
        int ch = str.get ();
        while (!ACE_OS::ace_isspace (ch) && ch != eof_ && var.length () < maxlen)
          {
            var += ch;
            ch = str.get ();
          }
        return ch;
      }

    ACE_INLINE
    NVPair& NVPair::operator =(const NVPair& pair)
      {
        this->first (pair.first ());
        this->second (pair.second ());
        return *this;
      }

    ACE_INLINE
    bool NVPair::operator ==(const NVPair& pair) const
      {
        return this->first_ == pair.first ();
      }

    ACE_INLINE
    bool NVPair::operator <(const NVPair& pair) const
      {
        return this->first_ < pair.first ();
      }

    ACE_INLINE
    ACE_CString const& NVPair::first () const
      {
        return this->first_;
      }

    ACE_INLINE
    void NVPair::first (ACE_CString const& t1)
      {
        this->first_ = t1;
      }

    ACE_INLINE
    ACE_CString const& NVPair::second () const
      {
        return this->second_;
      }

    ACE_INLINE
    void NVPair::second (ACE_CString const& t2)
      {
        this->second_ = t2;
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
