// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ACE_INLINE
    bool ConnectionKey::operator ==(const ConnectionKey& key) const
      {
        return this->equal (key);
      }

    ACE_INLINE
    bool ConnectionKey::operator !=(const ConnectionKey& key) const
      {
        return !this->equal (key);
      }

    ACE_INLINE
    bool ConnectionCacheKey::operator ==(const ConnectionCacheKey& cachekey) const
      {
        return this->key_ ? (this->key () == cachekey.key ()) : !cachekey.key_;
      }

    ACE_INLINE
    bool ConnectionCacheKey::operator !=(const ConnectionCacheKey& cachekey) const
      {
        return !(*this == cachekey);
      }

    ACE_INLINE
    const ConnectionKey& ConnectionCacheKey::key () const
      {
        return *this->key_;
      }

    ACE_INLINE
    bool ConnectionCacheValue::operator == (const ConnectionCacheValue& cacheval) const
      {
        return this->connection () == cacheval.connection () &&
                this->state () == cacheval.state ();
      }

    ACE_INLINE
    bool ConnectionCacheValue::operator != (const ConnectionCacheValue& cacheval) const
      {
        return !((*this) == cacheval);
      }

    ACE_INLINE
    ConnectionCacheValue::connection_type* ConnectionCacheValue::connection ()
      {
        return this->connection_;
      }

    ACE_INLINE
    const ConnectionCacheValue::connection_type* ConnectionCacheValue::connection () const
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
