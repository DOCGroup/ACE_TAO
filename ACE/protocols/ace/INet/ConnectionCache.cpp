#include "ace/INet/ConnectionCache.h"

#include <memory>
#include <utility>

#if !defined (__ACE_INLINE__)
#include "ace/INet/ConnectionCache.inl"
#endif

#include "ace/INet/INet_Log.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace INet
  {
    ConnectionKey::ConnectionKey () = default;

    ConnectionKey::~ConnectionKey () = default;

    ConnectionCacheKey::ConnectionCacheKey () = default;

    ConnectionCacheKey::ConnectionCacheKey (ConnectionKey const& key)
      : key_ (&key)
      {
      }

    ConnectionCacheKey::ConnectionCacheKey (ConnectionCacheKey const& cachekey)
      {
        *this = cachekey;
      }

    ConnectionCacheKey::~ConnectionCacheKey () = default;

    ConnectionCacheKey& ConnectionCacheKey::operator = (ConnectionCacheKey const& cachekey)
      {
        if (this != std::addressof (cachekey))
          {
            std::unique_ptr<ConnectionKey> duplicate;
            if (cachekey.key_ != nullptr)
              duplicate.reset (cachekey.key_->duplicate ());
            this->owned_key_ = std::move (duplicate);
            this->key_ = this->owned_key_.get ();
          }
        return *this;
      }

    u_long ConnectionCacheKey::hash () const
      {
        return this->key_ != nullptr ? this->key ().hash () : 0;
      }

    ConnectionHolder::~ConnectionHolder () = default;
    ConnectionHolder::ConnectionHolder () = default;

    ConnectionFactory::~ConnectionFactory () = default;
    ConnectionFactory::ConnectionFactory () = default;

    ConnectionCacheValue::ConnectionCacheValue ()
      : state_ (CST_INIT)
      {
      }

    ConnectionCacheValue::ConnectionCacheValue (connection_type* connection)
      : state_ (connection != nullptr ? CST_IDLE : CST_INIT),
        connection_ (connection)
      {
      }

    ConnectionCacheValue::ConnectionCacheValue (ConnectionCacheValue const& cacheval)
      : state_ (cacheval.state_),
        connection_ (cacheval.connection_)
      {
      }

    ConnectionCacheValue& ConnectionCacheValue::operator = (ConnectionCacheValue const& cacheval)
      {
        if (this != std::addressof (cacheval))
          {
            this->state_ = cacheval.state_;
            this->connection_ = cacheval.connection_;
          }
        return *this;
      }

    ConnectionCache::ConnectionCache (size_t size)
      : condition_ (lock_),
        cache_map_ (size)
      {
      }

    ConnectionCache::~ConnectionCache ()
      {
        this->close_all_connections ();
      }

    bool ConnectionCache::find_connection (ConnectionKey const& key,
                                           ConnectionCacheValue& cacheval)
      {
        if (this->cache_map_.find (ConnectionCacheKey (key),
                                   cacheval) == 0)
          {
            return true;
          }
        return false;
      }

    bool ConnectionCache::set_connection (ConnectionKey const& key,
                                          ConnectionCacheValue const& cacheval)
      {
        return this->cache_map_.rebind (ConnectionCacheKey (key),
                                        cacheval) != -1;
      }

    bool ConnectionCache::claim_existing_connection (ConnectionKey const& key,
                                                    connection_type*& connection,
                                                    ConnectionCacheValue::State& state)
      {
        INET_TRACE ("ConnectionCache::claim_existing_connection");

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
                    INET_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("ConnectionCache::claim_existing_connection - ")
                                              ACE_TEXT ("failed to claim connection entry")));
                  }
              }
          }
        return false;
      }

    bool ConnectionCache::claim_connection (ConnectionKey const& key,
                                           connection_type*& connection,
                                           factory_type const& connection_factory,
                                           bool wait)
      {
        INET_TRACE ("ConnectionCache::claim_connection");

        while (true)
          {
            bool create_connection {};
            ConnectionCacheValue::State state {ConnectionCacheValue::CST_NONE};
            do
              {
                ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                          guard_,
                                          this->lock_,
                                          false));

                if (this->claim_existing_connection (key, connection, state))
                  {
                    INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("%P|%t) ConnectionCache::claim_connection - ")
                                             ACE_TEXT ("successfully claimed existing connection\n")));
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
                        INET_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("ConnectionCache::claim_connection - ")
                                                  ACE_TEXT ("failed to initialize connection entry")));
                        return false;
                      }

                    create_connection = true;
                  }
                else
                  {
                    INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("ConnectionCache::claim_connection - ")
                                             ACE_TEXT ("waiting for connection to become available\n")));
                    // wait for connection to become ready/free
                    if (this->condition_.wait () != 0)
                      {
                        INET_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("(%P|%t) ConnectionCache::claim_connection - ")
                                                  ACE_TEXT ("error waiting for connection condition (%p)\n")));
                        return false;
                      }
                    INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("ConnectionCache::claim_connection - ")
                                             ACE_TEXT ("awoken and retrying to claim connection\n")));
                  }
              }
            while (0);

            if (create_connection)
              {
                std::unique_ptr<connection_type> new_connection (
                  connection_factory.create_connection (key));
                if (new_connection != nullptr)
                  {
                    INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("ConnectionCache::claim_connection - ")
                                             ACE_TEXT ("successfully created new connection\n")));

                    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                              guard_,
                                              this->lock_,
                                              false));

                    ConnectionCacheValue cacheval (new_connection.get ());
                    cacheval.state (ConnectionCacheValue::CST_BUSY);
                    if (this->set_connection (key, cacheval))
                      {
                        connection = new_connection.release ();
                        return true;
                      }
                    return false;
                  }
                else
                  return false;
              }
          }
      }

    bool ConnectionCache::release_connection (ConnectionKey const& key,
                                             connection_type* connection)
      {
        INET_TRACE ("ConnectionCache::release_connection");

        INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("ConnectionCache::release_connection - ")
                                 ACE_TEXT ("releasing connection\n")));

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
                INET_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("ConnectionCache::release_connection - ")
                                          ACE_TEXT ("failed to release connection entry")));
                return false;
              }
          }
        else
          return false;
      }

    bool ConnectionCache::close_connection (ConnectionKey const& key,
                                           connection_type* connection)
      {
        INET_TRACE ("ConnectionCache::close_connection");

        INET_DEBUG (9, (LM_INFO, DLINFO ACE_TEXT ("ConnectionCache::close_connection - ")
                                 ACE_TEXT ("closing connection\n")));

        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));

        ConnectionCacheValue cacheval;
        if (this->find_connection (key, cacheval) &&
              cacheval.connection () == connection &&
              cacheval.state () == ConnectionCacheValue::CST_BUSY)
          {
            connection_type* const conn = cacheval.connection ();
            cacheval.connection (nullptr);
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
                INET_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("ConnectionCache::close_connection - ")
                                          ACE_TEXT ("failed to close connection entry")));
                return false;
              }
          }
        else
          return false;
      }

    bool ConnectionCache::has_connection (ConnectionKey const& key)
      {
        INET_TRACE ("ConnectionCache::has_connection");

        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                                  guard_,
                                  this->lock_,
                                  false));

        ConnectionCacheValue cacheval;
        return (this->find_connection (key, cacheval) &&
                  cacheval.state () != ConnectionCacheValue::CST_CLOSED);
      }

    void ConnectionCache::close_all_connections ()
      {
        INET_TRACE ("ConnectionCache::close_all_connections");

        ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX,
                           guard_,
                           this->lock_));

        map_iter_type iter = this->cache_map_.end ();
        for (iter = this->cache_map_.begin ();
             iter != this->cache_map_.end ();
             ++iter)
          {
            if ((*iter).int_id_.state () != ConnectionCacheValue::CST_CLOSED)
              {
                connection_type* conn = (*iter).int_id_.connection ();
                (*iter).int_id_.connection (nullptr);
                (*iter).int_id_.state (ConnectionCacheValue::CST_CLOSED);
                delete conn;
              }
          }
        this->cache_map_.unbind_all ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
