/* -*- c++ -*- */
// $Id$

#ifndef JAWS_FILECACHE_H
#define JAWS_FILECACHE_H

#include "JAWS/Cache_Manager.h"

class ACE_Export JAWS_Referenced_Filecache_Factory
  : public JAWS_Referenced_Cache_Object_Factory
{
public:

  virtual void destroy (ACE_Cache_Object *);

};

class ACE_Export JAWS_Counted_Filecache_Factory
  : public JAWS_Counted_Cache_Object_Factory
{
public:

  virtual void destroy (ACE_Cache_Object *);

};

typedef ACE_Cache_Manager<ACE_Strdup_String,
                          JAWS_Referenced_Filecache_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        JAWS_Referenced_Filecache_Manager;

typedef ACE_Cache_Manager<ACE_Strdup_String,
                          JAWS_Counted_Filecache_Factory,
                          ACE_String_Hash_Functor,
                          ACE_String_Equal_Functor>
        JAWS_Counted_Filecache_Manager;

class ACE_Export JAWS_Filecache_Manager
{
public:

  JAWS_Filecache_Manager (ACE_Allocator *alloc = 0,
                          JAWS_Cache_Object_Factory *cof = 0,

                          size_t hashsize = 2048,
                          size_t maxsize = 65535,

                          size_t maxobjsize = 256,
                          size_t minobjsize = 0,

                          size_t highwater = 100,
                          size_t lowwater = 50,

                          int timetolive = -1,

                          int counted = 0
                          );

  int open (ACE_Allocator *alloc = 0,
            JAWS_Cache_Object_Factory *cof = 0,

            size_t hashsize = 1024,
            size_t maxsize = 4096,

            size_t maxobjsize = 5120,
            size_t minobjsize = 0,

            size_t highwater = 50,
            size_t lowwater = 30,

            int timetolive = -1,

            int counted = 0
            );

  ~JAWS_Cache_Manager (void);

  int close (void);

  int GET (const char *const &key, JAWS_Cache_Object *&cobj);

  int PUT (const char *const &key, const void *data, size_t size,
           JAWS_Cache_Object *&obj);

  int MAKE (const void *data, size_t size, JAWS_Cache_Object *&cobj);

  int TAKE (JAWS_Cache_Object *const &cobj);

  int DROP (JAWS_Cache_Object *&cobj);

  int FLUSH (void);

  enum { JAWS_REFERENCED_FILECACHE, JAWS_COUNTED_FILECACHE };

private:

  int type_;
  JAWS_Referenced_Filecache_Manager rfm_;
  JAWS_Counted_Filecache_Manager cfm_;
};

typedef ACE_Cache_Proxy<const char *,
                        JAWS_Filecache_Object, JAWS_Filecache_Manager>
        JAWS_Filecache_Proxy;


class JAWS_Filecache : public JAWS_Filecache_Proxy
{
public:

  HTTP_Cached_MB (const char *const &url,
                 HTTP_Cache_Proxy::Cache_Manager *cm = 0);
  HTTP_Cached_MB (const char *const &url, ACE_Message_Block *&mb, size_t size,
                 HTTP_Cache_Proxy::Cache_Manager *cm = 0);
  HTTP_Cached_MB (const char *const &url, const char *data, size_t size,
                 HTTP_Cache_Proxy::Cache_Manager *cm = 0);


  const ACE_Message_Block * operator-> (void) const;

};


#endif /* HTTP_UCACHE_H */
