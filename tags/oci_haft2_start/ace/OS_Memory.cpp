// $Id$

#include "ace/OS_Memory.h"

ACE_RCSID(ace, OS_Memory, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_Memory.inl"
#else /* !ACE_HAS_INLINED_OSCALLS */
#if defined (__hpux) && !defined (__GNUG__)
static int shut_up_aCC = 0;
#endif /* HPUX && !g++ */
#endif /* !ACE_HAS_INLINED_OS_CALLS */

#include "ace/os_include/os_stdlib.h"

void *
ACE_OS_Memory::malloc (size_t nbytes)
{
  return ACE_MALLOC_FUNC (nbytes);
}

void *
ACE_OS_Memory::calloc (size_t elements, size_t sizeof_elements)
{
#if !defined (ACE_HAS_WINCE)
  return ACE_CALLOC_FUNC (elements, sizeof_elements);
#else
  // @@ This will probably not work since it doesn't consider
  // alignment properly.
  return ACE_MALLOC_FUNC (elements * sizeof_elements);
#endif /* ACE_HAS_WINCE */
}

void *
ACE_OS_Memory::realloc (void *ptr, size_t nbytes)
{
  return ACE_REALLOC_FUNC (ACE_MALLOC_T (ptr), nbytes);
}

void
ACE_OS_Memory::free (void *ptr)
{
  ACE_FREE_FUNC (ACE_MALLOC_T (ptr));
}
