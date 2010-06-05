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
#include "INet_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace INet
      {

        /**
        * @class ACE_INet_ConnectionKey
        *
        * @brief
        *
        */
        class ConnectionKey
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
        * @brief
        *
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
        * @brief
        *
        */
        class ConnectionHolder
          {
            public:
              virtual ~ConnectionHolder ();
            protected:
              ConnectionHolder ();
          };


        /**
        * @class ACE_INet_ConnectionFactory
        *
        * @brief
        *
        */
        class ConnectionFactory
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
        * @brief
        *
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
        * @brief
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

              ConnectionCache(size_t size = ACE_DEFAULT_MAP_SIZE);
              ~ConnectionCache ();

              bool claim_connection(const ConnectionKey& key,
                                    connection_type*& connection,
                                    const factory_type& connection_factory,
                                    bool wait = true);

              bool release_connection(const ConnectionKey& key,
                                    connection_type* connection);

              bool close_connection(const ConnectionKey& key,
                                    connection_type* connection);

              bool has_connection (const ConnectionKey& key);

              void close_all_connections ();

              size_t current_size () const;

              size_t maximum_size () const;

            private:
              bool set_connection (const ConnectionKey& key,
                                   const ConnectionCacheValue& cacheval);

              bool claim_existing_connection(const ConnectionKey& key,
                                             connection_type*& connection,
                                             ConnectionCacheValue::State& state);

              bool find_connection (const ConnectionKey& key,
                                    ConnectionCacheValue& cacheval);


              mutable ACE_SYNCH_MUTEX lock_;
              ACE_SYNCH_CONDITION condition_;
              map_type cache_map_;
          };


      };
  };

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ConnectionCache.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_INET_CONNECTION_CACHE_H */
