// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    bool ConnectionKey::operator == (ConnectionKey const& key) const
      {
        return this->equal (key);
      }

    ACE_INLINE
    bool ConnectionKey::operator != (ConnectionKey const& key) const
      {
        return !this->equal (key);
      }

    ACE_INLINE
    bool ConnectionCacheKey::operator == (ConnectionCacheKey const& cachekey) const
      {
        return this->key_ == nullptr ? cachekey.key_ == nullptr :
          cachekey.key_ != nullptr && this->key () == cachekey.key ();
      }

    ACE_INLINE
    bool ConnectionCacheKey::operator != (ConnectionCacheKey const& cachekey) const
      {
        return !(*this == cachekey);
      }

    ACE_INLINE
    ConnectionKey const& ConnectionCacheKey::key () const
      {
        return *this->key_;
      }

    ACE_INLINE
    bool ConnectionCacheValue::operator == (ConnectionCacheValue const& cacheval) const
      {
        return this->connection () == cacheval.connection () &&
                this->state () == cacheval.state ();
      }

    ACE_INLINE
    bool ConnectionCacheValue::operator != (ConnectionCacheValue const& cacheval) const
      {
        return !((*this) == cacheval);
      }

    ACE_INLINE
    ConnectionCacheValue::connection_type* ConnectionCacheValue::connection ()
      {
        return this->connection_;
      }

    ACE_INLINE
    ConnectionCacheValue::connection_type const* ConnectionCacheValue::connection () const
      {
        return this->connection_;
      }

    ACE_INLINE
    void ConnectionCacheValue::connection (connection_type* conn)
      {
        this->connection_ = conn;
      }

    ACE_INLINE
    ConnectionCacheValue::State ConnectionCacheValue::state () const
      {
        return this->state_;
      }

    ACE_INLINE
    void ConnectionCacheValue::state (State st)
      {
        this->state_ = st;
      }

    ACE_INLINE
    size_t ConnectionCache::current_size () const
      {
        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));
        return this->cache_map_.current_size ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
