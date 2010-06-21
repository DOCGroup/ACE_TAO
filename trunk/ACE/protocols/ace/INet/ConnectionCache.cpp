// $Id$

#include "ace/INet/ConnectionCache.h"

#if !defined (__ACE_INLINE__)
#include "ace/INet/ConnectionCache.inl"
#endif

ACE_RCSID(NET_CLIENT,ACE_INet_ConnectionCache,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {

    ConnectionKey::ConnectionKey ()
      {}

    ConnectionKey::~ConnectionKey ()
      {}

    ConnectionCacheKey::ConnectionCacheKey ()
      : key_ (0),
        delete_key_ (false)
      {
      }

    ConnectionCacheKey::ConnectionCacheKey (const ConnectionKey& key)
      : key_ (&const_cast<ConnectionKey&> (key)),
        delete_key_ (false)
      {
      }

    ConnectionCacheKey::ConnectionCacheKey (const ConnectionCacheKey& cachekey)
      : key_ (0),
        delete_key_ (false)
      {
        *this = cachekey;
      }

    ConnectionCacheKey& ConnectionCacheKey::operator =(const ConnectionCacheKey& cachekey)
      {
        if (this != &cachekey)
          {
            if (this->key_ != 0 && this->delete_key_)
              {
                delete this->key_;
                this->delete_key_ = false;
              }

            this->key_ = cachekey.key_->duplicate ();

            if (this->key_ == 0)
              {
                this->delete_key_ = false;
              }
            else
              {
                this->delete_key_ = true;
              }
          }
        return *this;
      }

    u_long ConnectionCacheKey::hash (void) const
      {
        return this->key_ ? this->key ().hash () : 0;
      }

    ConnectionHolder::~ConnectionHolder () {}
    ConnectionHolder::ConnectionHolder () {}

    ConnectionFactory::~ConnectionFactory () {}
    ConnectionFactory::ConnectionFactory () {}

    ConnectionCacheValue::ConnectionCacheValue ()
      : state_ (CST_INIT),
        connection_ (0)
      {
      }

    ConnectionCacheValue::ConnectionCacheValue (connection_type* connection)
      : state_ (connection ? CST_IDLE : CST_INIT),
        connection_ (connection)
      {
      }

    ConnectionCacheValue::ConnectionCacheValue (const ConnectionCacheValue& cacheval)
      {
        *this = cacheval;
      }

    ConnectionCacheValue& ConnectionCacheValue::operator =(const ConnectionCacheValue& cacheval)
      {
        if (this != &cacheval)
          {
            this->state_ = cacheval.state ();
            this->connection_ = const_cast<connection_type*> (cacheval.connection ());
          }
        return *this;
      }

    ConnectionCache::ConnectionCache(size_t size)
      : condition_ (lock_),
        cache_map_ (size)
      {
      }

    ConnectionCache::~ConnectionCache ()
      {
        this->close_all_connections ();
      }

    bool ConnectionCache::find_connection (const ConnectionKey& key,
                                           ConnectionCacheValue& cacheval)
      {
        if (this->cache_map_.find (ConnectionCacheKey (key),
                                   cacheval) == 0)
          {
            return true;
          }
        return false;
      }

    bool ConnectionCache::set_connection (const ConnectionKey& key,
                                          const ConnectionCacheValue& cacheval)
      {
        return this->cache_map_.rebind (ConnectionCacheKey (key),
                                        cacheval) != -1;
      }

    bool ConnectionCache::claim_existing_connection(const ConnectionKey& key,
                                                    connection_type*& connection,
                                                    ConnectionCacheValue::State& state)
      {
        ACE_TRACE ("ConnectionCache::claim_existing_connection");

        ConnectionCacheValue cacheval;
        if (this->find_connection (key, cacheval))
          {
            state = cacheval.state ();
            if (state == ConnectionCacheValue::CST_IDLE)
              {
                cacheval.state (ConnectionCacheValue::CST_BUSY);
                if (this->set_connection (key, cacheval))
                  {
                    connection = cacheval.connection ();
                    return true;
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%P|%t) ConnectionCache::claim_existing_connection - ")
                                          ACE_TEXT ("failed to claim connection entry")));
                  }
              }
          }
        return false;
      }

    bool ConnectionCache::claim_connection(const ConnectionKey& key,
                                           connection_type*& connection,
                                           const factory_type& connection_factory,
                                           bool wait)
      {
        ACE_TRACE ("ConnectionCache::claim_connection");

        while (1)
          {
            bool create_connection = false;
            ConnectionCacheValue::State state = ConnectionCacheValue::CST_NONE;
            do
              {
                ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                          guard_,
                                          this->lock_,
                                          false));

                if (this->claim_existing_connection (key, connection, state))
                  {
                    return true;
                  }

                if ((state == ConnectionCacheValue::CST_BUSY ||
                        state == ConnectionCacheValue::CST_INIT) && !wait)
                  return false;

                if (state == ConnectionCacheValue::CST_CLOSED ||
                        state == ConnectionCacheValue::CST_NONE)
                  {
                    if (!this->set_connection (key, ConnectionCacheValue ()))
                      {
                        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%P|%t) ConnectionCache::claim_connection - ")
                                              ACE_TEXT ("failed to initialize connection entry")));
                        return false;
                      }

                    create_connection = true;
                  }
                else
                  {
                    // wait for connection to become ready/free
                    if (this->condition_.wait () != 0)
                      {
                        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%P|%t) ConnectionCache::claim_connection - ")
                                              ACE_TEXT ("error waiting for connection condition (%p)\n")));
                        return false;
                      }
                  }
              }
            while (0);

            if (create_connection)
              {
                connection = connection_factory.create_connection (key);
                if (connection)
                  {
                    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                              guard_,
                                              this->lock_,
                                              false));

                    ConnectionCacheValue cacheval (connection);
                    cacheval.state (ConnectionCacheValue::CST_BUSY);
                    return this->set_connection (key, cacheval);
                  }
              }
          }
      }

    bool ConnectionCache::release_connection(const ConnectionKey& key,
                                             connection_type* connection)
      {
        ACE_TRACE ("ConnectionCache::release_connection");

        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));

        ConnectionCacheValue cacheval;
        if (this->find_connection (key, cacheval) &&
              cacheval.connection () == connection &&
              cacheval.state () == ConnectionCacheValue::CST_BUSY)
          {
            cacheval.state (ConnectionCacheValue::CST_IDLE);
            if (this->set_connection (key, cacheval))
              {
                // signal other threads about free connection
                this->condition_.broadcast ();
                return true;
              }
            else
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("%P|%t) ConnectionCache::release_connection - ")
                                      ACE_TEXT ("failed to release connection entry")));
                return false;
              }
          }
        else
          return false;
      }

    bool ConnectionCache::close_connection(const ConnectionKey& key,
                                           connection_type* connection)
      {
        ACE_TRACE ("ConnectionCache::close_connection");

        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));

        ConnectionCacheValue cacheval;
        if (this->find_connection (key, cacheval) &&
              cacheval.connection () == connection &&
              cacheval.state () == ConnectionCacheValue::CST_IDLE)
          {
            connection_type* conn = cacheval.connection ();
            cacheval.connection (0);
            cacheval.state (ConnectionCacheValue::CST_CLOSED);
            if (this->set_connection (key, cacheval))
              {
                // signal other threads about closed connection
                this->condition_.broadcast ();
                delete conn; // clean up
                return true;
              }
            else
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("%P|%t) ConnectionCache::close_connection - ")
                                      ACE_TEXT ("failed to close connection entry")));
                return false;
              }
          }
        else
          return false;
      }

    bool ConnectionCache::has_connection(const ConnectionKey& key)
      {
        ACE_TRACE ("ConnectionCache::has_connection");

        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));

        ConnectionCacheValue cacheval;
        return (this->find_connection (key, cacheval) &&
                  cacheval.state () != ConnectionCacheValue::CST_CLOSED);
      }

    void ConnectionCache::close_all_connections()
      {
        ACE_TRACE ("ConnectionCache::close_all_connections");

        ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX,
                           guard_,
                           this->lock_));

        map_iter_type iter = this->cache_map_.end ();
        for (iter = this->cache_map_.begin ();
             iter != this->cache_map_.end ();
             ++iter)
          {
            if ((*iter).int_id_.state () == ConnectionCacheValue::CST_CLOSED)
              {
                connection_type* conn = (*iter).int_id_.connection ();
                (*iter).int_id_.connection (0);
                (*iter).int_id_.state (ConnectionCacheValue::CST_CLOSED);
                delete conn;
              }
          }
        this->cache_map_.unbind_all ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
