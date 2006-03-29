// $Id$

#include "tao/MMAP_Allocator.h"

#include "ace/Mem_Map.h"

#ifdef ACE_HAS_SENDFILE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MMAP_Allocator::TAO_MMAP_Allocator (void)
  : TAO_MMAP_Allocator_Base ()
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

  off_t const the_offset = static_cast<off_t> (off - base);

  return (the_offset < 0 ? static_cast<off_t> (-1) : the_offset);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_HAS_SENDFILE */
