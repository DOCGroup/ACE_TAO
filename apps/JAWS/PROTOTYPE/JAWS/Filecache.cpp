// $Id$

#include "JAWS/Filecache.h"

void
JAWS_Referenced_Filecache_Factory::destroy (ACE_Cache_Object *object)
{
  JAWS_FILE *file = (JAWS_FILE *) object->data ();
  file->close ();
  if (file->map ())
    file->map ()->close ();
  delete file;
  this->ACE_Referenced_Cache_Object_Factory::destroy (object);
}

void
JAWS_Counted_Filecache_Factory::destroy (ACE_Cache_Object *object)
{
  JAWS_FILE *file = (JAWS_FILE *) object->data ();
  file->close ();
  if (file->map ())
    file->map ()->close ();
  delete file;
  this->ACE_Counted_Cache_Object_Factory::destroy (object);
}

JAWS_Cached_FILE::JAWS_Cached_FILE (const char *const &filename,
                                    JAWS_Filecache_Proxy::Cache_Manager *cm)
  : JAWS_Filecache_Proxy (filename, cm)
{
  ACE_HANDLE handle = ACE_INVALID_HANDLE;

  if (this->data () != 0)
    handle = ACE_OS::dup (this->date ()->get_handle ());

  this->file_->set_handle (handle);
}

JAWS_Cached_FILE::JAWS_Cached_FILE (const char *const &filename,
                                    ACE_FILE_IO *&file,
                                    size_t size,
                                    JAWS_Filecache_Proxy::Cache_Manager *cm)
  : JAWS_Filecache_Proxy (filename, file, size, cm)
{
}

JAWS_Cached_FILE::~JAWS_Cached_FILE (void)
{
  this->file_.close ();
}

ACE_FILE_IO *
JAWS_Cached_FILE::file (void)
{
  return &(this->file_);
}

ACE_Mem_Map *
JAWS_Cached_FILE::mmap (void)
{
  return this->data ()->mem_map ();
}
