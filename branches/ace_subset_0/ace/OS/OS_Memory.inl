// $Id$

#if ! defined (ACE_PSOS_DIAB_MIPS)
# include /**/ <stdlib.h>
#endif /* ! ACE_PSOS_DIAB_MIPS */

#if !defined (ACE_LACKS_UNISTD_H) && !defined (ACE_PSOS)
# include /**/ <unistd.h>
#endif /* !ACE_LACKS_UNISTD_H && !ACE_PSOS */

ACE_INLINE void *
ACE_OS_Memory::sbrk (int brk)
{
#if defined (ACE_LACKS_SBRK)
  ACE_UNUSED_ARG (brk);
  ACE_NOTSUP_RETURN (0);
#else
  ACE_OSCALL_RETURN (::sbrk (brk), void *, 0);
#endif /* VXWORKS */
}

ACE_INLINE void *
ACE_OS_Memory::malloc (size_t nbytes)
{
  return ACE_MALLOC_FUNC (nbytes);
}

ACE_INLINE void *
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

ACE_INLINE void *
ACE_OS_Memory::realloc (void *ptr, size_t nbytes)
{
  return ACE_REALLOC_FUNC (ACE_MALLOC_T (ptr), nbytes);
}

ACE_INLINE void
ACE_OS_Memory::free (void *ptr)
{
  ACE_FREE_FUNC (ACE_MALLOC_T (ptr));
}
