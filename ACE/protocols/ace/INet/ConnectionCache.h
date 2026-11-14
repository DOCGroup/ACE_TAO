/**
 * @file ConnectionCache.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_INET_CONNECTION_CACHE_H
#define ACE_INET_CONNECTION_CACHE_H

#include /**/ "ace/pre.h"

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/INet/INet_Export.h"
#include <memory>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {
        /**
        * @class ACE_INet_ConnectionKey
        *
        * @brief Base class for connection keys.
        *
        */
        class ACE_INET_Export ConnectionKey
          {
            public:
              ConnectionKey ();
              virtual ~ConnectionKey ();

              bool operator == (ConnectionKey const& key) const;

              bool operator != (ConnectionKey const& key) const;

              virtual u_long hash () const = 0;

              virtual ConnectionKey* duplicate () const = 0;

            protected:
              virtual bool equal (ConnectionKey const& key) const = 0;
          };

        /**
        * @class ACE_INet_ConnectionCacheKey
        *
        * @brief Holder class for connection keys.
        *
        * Borrows an input key and duplicates it when copied into the cache map.
        */
        class ConnectionCacheKey
          {
            public:
              ConnectionCacheKey ();
              explicit ConnectionCacheKey (ConnectionKey const& key);
              ConnectionCacheKey (ConnectionCacheKey const& cachekey);
              virtual ~ConnectionCacheKey ();

              ConnectionCacheKey& operator = (ConnectionCacheKey const& cachekey);
              ConnectionCacheKey (ConnectionCacheKey&&) = delete;
              ConnectionCacheKey& operator = (ConnectionCacheKey&&) = delete;

              bool operator == (ConnectionCacheKey const& cachekey) const;

              bool operator != (ConnectionCacheKey const& cachekey) const;

              u_long hash () const;

              ConnectionKey const& key () const;

            private:
              /// Owns a duplicate when this key was copied into the map.
              std::unique_ptr<ConnectionKey> owned_key_ {};

              /// Views the borrowed key or the owned duplicate.
              ConnectionKey const* key_ {};
          };

        /**
        * @class ACE_INet_ConnectionHolder
        *
        * @brief Generic base for connection wrappers.
        *
        */
        class ACE_INET_Export ConnectionHolder
          {
            public:
              virtual ~ConnectionHolder ();
            protected:
              ConnectionHolder ();
          };

        /**
        * @class ACE_INet_ConnectionFactory
        *
        * @brief Base class for connection factories.
        *
        * Derived classes create specific connections and
        * return those for caching wrapped in a connection
        * holder.
        */
        class ACE_INET_Export ConnectionFactory
          {
            public:
              ConnectionFactory ();
              virtual ~ConnectionFactory ();

              /// Returns a newly allocated connection whose ownership is
              /// transferred to the ConnectionCache on successful insertion.
              virtual ConnectionHolder* create_connection (
                  ConnectionKey const& key) const = 0;
          };

        /**
        * @class ACE_INet_ConnectionCacheValue
        *
        * @brief Holder class for connections.
        *
        * Implements a non-owning holder for connection state in the cache map.
        */
        class ConnectionCacheValue
          {
            public:
              enum State
                {
                  CST_NONE,
                  CST_INIT,
                  CST_IDLE,
                  CST_BUSY,
                  CST_CLOSED
                };

              using connection_type = ConnectionHolder;

              ConnectionCacheValue ();
              explicit ConnectionCacheValue (connection_type* connection);
              ConnectionCacheValue (ConnectionCacheValue const& cacheval);

              ConnectionCacheValue& operator = (ConnectionCacheValue const& cacheval);
              ConnectionCacheValue (ConnectionCacheValue&&) = delete;
              ConnectionCacheValue& operator = (ConnectionCacheValue&&) = delete;

              bool operator == (ConnectionCacheValue const& cacheval) const;

              bool operator != (ConnectionCacheValue const& cacheval) const;

              connection_type* connection ();

              connection_type const* connection () const;

              void connection (connection_type* conn);

              State state () const;

              void state (State st);

            private:
              State state_;
              /// Borrowed while the cache owns and manages the connection.
              connection_type* connection_ {};
          };

        /**
        * @class ACE_INet_ConnectionCache
        *
        * @brief Implements a cache for INet connection objects.
        *
        */
        class ACE_INET_Export ConnectionCache
          {
            public:
              using connection_type = ConnectionHolder;
              using factory_type = ConnectionFactory;

              using map_type =
                ACE_Hash_Map_Manager_Ex<ConnectionCacheKey,
                                         ConnectionCacheValue,
                                         ACE_Hash<ConnectionCacheKey>,
                                         ACE_Equal_To<ConnectionCacheKey>,
                                         ACE_SYNCH_NULL_MUTEX>;

              using map_iter_type = map_type::iterator;

              using map_entry_type = ACE_Hash_Map_Entry<ConnectionCacheKey, ConnectionCacheValue>;

              explicit ConnectionCache (size_t size = ACE_DEFAULT_MAP_SIZE);

              ~ConnectionCache ();

              ConnectionCache (ConnectionCache const&) = delete;
              ConnectionCache& operator = (ConnectionCache const&) = delete;
              ConnectionCache (ConnectionCache&&) = delete;
              ConnectionCache& operator = (ConnectionCache&&) = delete;

              /// Claim a connection from the cache.
              /// Creates a new connection using <connection_factory>
              /// if the cache does not contain a matching entry for
              /// <key>.
              /// If <wait> is true and the state of the matching
              /// connection is BUSY the method will block waiting for
              /// connection to become available.
              /// Returns true if a connection could be successfully
              /// claimed and sets <connection> to the claimed connection.
              /// Returns false otherwise.
              bool claim_connection (ConnectionKey const& key,
                                    connection_type*& connection,
                                    factory_type const& connection_factory,
                                    bool wait = true);

              /// Release a previously claimed connection making it
              /// available for renewed claiming.
              /// Returns true if the connection was successfully released.
              bool release_connection (ConnectionKey const& key,
                                    connection_type* connection);

              /// Close a previously claimed connection.
              /// Deletes the actual connection object and marks the cache entry
              /// as CLOSED.
              /// Returns true is the connection was successfully closed.
              bool close_connection (ConnectionKey const& key,
                                    connection_type* connection);

              /// Returns true if the cache contains a connection matching
              /// <key>. Cache entries with state CLOSED are not considered.
              /// Returns false otherwise.
              bool has_connection (ConnectionKey const& key);

              /// Unconditionally closes all active connections.
              void close_all_connections ();

              /// Returns the number of registered cache entries (including CLOSED).
              size_t current_size () const;

            private:
              /// Updates cache entry state
              bool set_connection (ConnectionKey const& key,
                                   ConnectionCacheValue const& cacheval);

              /// Attempts to claim an existing connection.
              /// Returns true and sets @a connection if successful.
              /// Returns false otherwise.
              /// Does not wait when no connection available.
              bool claim_existing_connection (ConnectionKey const& key,
                                             connection_type*& connection,
                                             ConnectionCacheValue::State& state);

              /// Looks up a matching cache entry for @a key and updates
              /// <cacheval> with the entry state if found.
              /// Returns true if found, false otherwise.
              bool find_connection (ConnectionKey const& key,
                                    ConnectionCacheValue& cacheval);

              mutable ACE_SYNCH_MUTEX lock_;
              ACE_SYNCH_CONDITION condition_;
              map_type cache_map_;
          };
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/INet/ConnectionCache.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CONNECTION_CACHE_H */
