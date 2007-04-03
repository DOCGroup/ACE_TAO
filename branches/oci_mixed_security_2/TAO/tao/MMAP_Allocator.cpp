// $Id$

#include "tao/MMAP_Allocator.h"

#if TAO_HAS_SENDFILE == 1

#include "ace/Mem_Map.h"
#include "ace/Default_Constants.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  // Default size mmap()ed memory pool will be the sum of the control
  // block size and the default CDR buffer chosen by the user.  The
  // final size may be rounded up by the allocator to be aligned on
  // the appropriate boundary/page.
  //
  // @@ This type really should be ACE_LOFF_T but ACE's mmap()-based
  //    classes currently do not handle large file offsets.
  //        -Ossama
  off_t const the_default_buf_size =
    sizeof (ACE_Control_Block) + ACE_DEFAULT_CDR_BUFSIZE;

  ACE_MMAP_Memory_Pool_Options const the_pool_options (
    ACE_DEFAULT_BASE_ADDR,
    ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED,
    0, // No need to sync
    the_default_buf_size,
    MAP_SHARED, // Written data must be reflected in the backing store
                // file in order for sendfile() to be able to read it.
    1,
    0,
    /* 0 */ ACE_DEFAULT_FILE_PERMS,
    true); // Generate for each mmap an unqiue pool
}


// Ideally we should open the backing store with shm_open() so that we
// can avoid creating an on-disk backing store.  An on-disk backing
// store could potentially cause sendfile() to block on disk I/O,
// which is undesirable.  Unfortunately, ACE_Mem_Map currently
// provides no way to configure which "open" function to use,
// i.e. open(2) or shm_open(3).  Alternatively we could shm_open() the
// backing store file beforehand.  Unfortunately,
// ACE_{Lite_}MMAP_Memory_Pool currently doesn't provide a means to
// pass the file descriptor for the desired backing store to the
// underlying ACE_Mem_Map object.
//
// It may be tempting to mmap() /dev/zero.  That would certainly
// provide the desired transient "scratch space" memory that we can
// write and read to and from, respectively.  Unfortunately, the data
// in /dev/zero-based memory mapped buffer can only be read through
// the buffer itself, not through the file descriptor (e.g. using
// read(2)) for the open()ed /dev/zero device.  Reading through the
// /dev/zero file descriptor simply returns zero, as /dev/zero was
// designed to do.  So unfortunate. :)
TAO_MMAP_Allocator::TAO_MMAP_Allocator (void)
  : TAO_MMAP_Allocator_Base ((char const *) 0 /* pool name */,
                             0,  // No need to explicitly name the lock.
                             &the_pool_options)
{
}

TAO_MMAP_Allocator::~TAO_MMAP_Allocator (void)
{
}

// @@ Should be const but underlying allocator methods are not!
ACE_HANDLE
TAO_MMAP_Allocator::handle (void)
{
  return this->alloc ().memory_pool ().mmap ().handle ();
}

// @@ Should be const but underlying allocator methods are not!
off_t
TAO_MMAP_Allocator::offset (void * p)
{
  ACE_Mem_Map const & m = this->alloc ().memory_pool ().mmap ();

  ptrdiff_t const off  = reinterpret_cast<ptrdiff_t> (p);
  ptrdiff_t const base = reinterpret_cast<ptrdiff_t> (m.addr ());
  ptrdiff_t const end  = base + m.size ();

  // Check if p is in the range of the mmap pool, if not we return -1
  if (off < base || off > end)
    return -1;

  off_t const the_offset = static_cast<off_t> (off - base);

  return (the_offset < 0 ? static_cast<off_t> (-1) : the_offset);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_SENDFILE==1 */
