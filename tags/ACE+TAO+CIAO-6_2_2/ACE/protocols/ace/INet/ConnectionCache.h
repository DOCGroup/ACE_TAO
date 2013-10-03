// $Id$

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

              bool operator ==(const ConnectionKey& key) const;

              bool operator !=(const ConnectionKey& key) const;

              virtual u_long hash () const = 0;

              virtual ConnectionKey* duplicate () const = 0;

            protected:
              virtual bool equal (const ConnectionKey& key) const = 0;
          };

        /**
        * @class ACE_INet_ConnectionCacheKey
        *
        * @brief Holder class for connection keys.
        *
        * Implements non-copying holder object used as ext_id in
        * connection cache map.
        */
        class ConnectionCacheKey
          {
            public:
              ConnectionCacheKey ();
              ConnectionCacheKey (const ConnectionKey& key);
              ConnectionCacheKey (const ConnectionCacheKey& cachekey);

              ConnectionCacheKey& operator =(const ConnectionCacheKey& cachekey);

              bool operator ==(const ConnectionCacheKey& cachekey) const;

              bool operator !=(const ConnectionCacheKey& cachekey) const;

              u_long hash () const;

              const ConnectionKey& key () const;

            private:
              ConnectionKey* key_;
              bool delete_key_;
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

              virtual ConnectionHolder* create_connection (
                  const ConnectionKey& key) const = 0;
          };


        /**
        * @class ACE_INet_ConnectionCacheValue
        *
        * @brief Holder class for connections.
        *
        * Implements non-copying holder object maintaining
        * connection state used as int_id in connection
        * cache map.
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

              typedef ConnectionHolder connection_type;

              ConnectionCacheValue ();
              explicit ConnectionCacheValue (connection_type* connection);
              ConnectionCacheValue (const ConnectionCacheValue& cacheval);

              ConnectionCacheValue& operator =(const ConnectionCacheValue& cacheval);

              bool operator == (const ConnectionCacheValue& cacheval) const;

              bool operator != (const ConnectionCacheValue& cacheval) const;

              connection_type* connection ();

              const connection_type* connection () const;

              void connection (connection_type* conn);

              State state () const;

              void state (State st);

            private:
              State state_;
              connection_type* connection_;
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
              typedef ConnectionHolder connection_type;
              typedef ConnectionFactory factory_type;

              typedef ACE_Hash_Map_Manager_Ex <ConnectionCacheKey,
                                               ConnectionCacheValue,
                                               ACE_Hash<ConnectionCacheKey>,
                                               ACE_Equal_To<ConnectionCacheKey>,
                                               ACE_SYNCH_NULL_MUTEX> map_type;

              typedef map_type::iterator map_iter_type;

              typedef ACE_Hash_Map_Entry <ConnectionCacheKey,
                                          ConnectionCacheValue> map_entry_type;

              /// Constructor
              ConnectionCache(size_t size = ACE_DEFAULT_MAP_SIZE);

              /// Destructor
              ~ConnectionCache ();

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
              bool claim_connection(const ConnectionKey& key,
                                    connection_type*& connection,
                                    const factory_type& connection_factory,
                                    bool wait = true);

              /// Release a previously claimed connection making it
              /// available for renewed claiming.
              /// Returns true if the connection was successfully released.
              bool release_connection(const ConnectionKey& key,
                                    connection_type* connection);

              /// Close a previously claimed connection.
              /// Deletes the actual connection object and marks the cache entry
              /// as CLOSED.
              /// Returns true is the connection was successfully closed.
              bool close_connection(const ConnectionKey& key,
                                    connection_type* connection);

              /// Returns true if the cache contains a connection matching
              /// <key>. Cache entries with state CLOSED are not considered.
              /// Returns false otherwise.
              bool has_connection (const ConnectionKey& key);

              /// Unconditionally closes all active connections.
              void close_all_connections ();

              /// Returns the number of registered cache entries (including CLOSED).
              size_t current_size () const;

            private:
              /// Updates cache entry state
              bool set_connection (const ConnectionKey& key,
                                   const ConnectionCacheValue& cacheval);

              /// Attempts to claim an existing connection.
              /// Returns true and sets @a connection if successful.
              /// Returns false otherwise.
              /// Does not wait when no connection available.
              bool claim_existing_connection(const ConnectionKey& key,
                                             connection_type*& connection,
                                             ConnectionCacheValue::State& state);

              /// Looks up a matching cache entry for @a key and updates
              /// <cacheval> with the entry state if found.
              /// Returns true if found, false otherwise.
              bool find_connection (const ConnectionKey& key,
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
