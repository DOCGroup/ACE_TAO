/* -*- c++ -*- */
#ifndef JAWS_FILECACHE_H
#define JAWS_FILECACHE_H

#include "ace/FILE_IO.h"

#include "JAWS/Cache_Manager.h"
#include "JAWS/FILE.h"
#include "JAWS/Export.h"

class JAWS_Export JAWS_Referenced_Filecache_Factory
  : public JAWS_Referenced_Cache_Object_Factory
{
public:
  void destroy (JAWS_Cache_Object *) override;
};

class JAWS_Export JAWS_Counted_Filecache_Factory
  : public JAWS_Counted_Cache_Object_Factory
{
public:
  void destroy (JAWS_Cache_Object *) override;
};

using JAWS_Referenced_Filecache_Manager =
  JAWS_Cache_Manager<JAWS_Strdup_String, JAWS_Referenced_Filecache_Factory, JAWS_String_Hash_Functor,
                     JAWS_String_Equal_Functor>;

using JAWS_Counted_Filecache_Manager =
  JAWS_Cache_Manager<JAWS_Strdup_String, JAWS_Counted_Filecache_Factory, JAWS_String_Hash_Functor,
                     JAWS_String_Equal_Functor>;

using JAWS_Filecache_Manager = JAWS_Counted_Filecache_Manager;

using JAWS_Filecache_Proxy = JAWS_Cache_Proxy<const char *, JAWS_FILE, JAWS_Filecache_Manager>;

class JAWS_Export JAWS_Cached_FILE : private JAWS_Filecache_Proxy
{
public:
  explicit JAWS_Cached_FILE (const char *const &filename,
                    JAWS_Filecache_Proxy::Cache_Manager *cm = nullptr);
  JAWS_Cached_FILE (const char *const &filename,
                    JAWS_FILE *&file,
                    size_t size,
                    JAWS_Filecache_Proxy::Cache_Manager *cm = nullptr);

  ~JAWS_Cached_FILE ();
  JAWS_Cached_FILE (JAWS_Cached_FILE const &) = delete;
  JAWS_Cached_FILE (JAWS_Cached_FILE &&) = delete;
  JAWS_Cached_FILE &operator= (JAWS_Cached_FILE const &) = delete;
  JAWS_Cached_FILE &operator= (JAWS_Cached_FILE &&) = delete;

  ACE_FILE_IO * file ();
  ACE_Mem_Map * mmap ();

private:
  ACE_FILE_IO file_;
};


#endif /* JAWS_FILECACHE_H */
