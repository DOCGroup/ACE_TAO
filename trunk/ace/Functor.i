/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor.i
//
// = DESCRIPTION
//    Inlinable method definitions for non-templatized classes
//    and template specializations implementing the GOF Command Pattern,
//    and STL-style functors.
//
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
//    and on STL-style functor implementations originally done by
//
//    Irfan Pyarali  <irfan@cs.wustl.edu>
//
// ============================================================================

//////////////////////////////////////////////////////////////
// GOF Command Pattern Classes and Template Specializations //
//////////////////////////////////////////////////////////////

// Default constructor.

ACE_INLINE
ACE_Command_Base::ACE_Command_Base (void)
{
}

// Virtaul destructor.

ACE_INLINE
ACE_Command_Base::~ACE_Command_Base (void)
{
}

////////////////////////////////////////////////////////////
// STL-style Functor Classes and Template Specializations //
////////////////////////////////////////////////////////////

ACE_INLINE u_long 
ACE_Hash<char>::operator () (char t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<signed char>::operator () (signed char t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<unsigned char>::operator () (unsigned char t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<short>::operator () (short t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<unsigned short>::operator () (unsigned short t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<int>::operator () (int t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<unsigned int>::operator () (unsigned int t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<long>::operator () (long t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<unsigned long>::operator () (unsigned long t) const
{
  return t;
}

ACE_INLINE u_long 
ACE_Hash<const char *>::operator () (const char *t) const
{
  return ACE::hash_pjw (t);
}

ACE_INLINE u_long 
ACE_Hash<char *>::operator () (const char *t) const
{
  return ACE::hash_pjw (t);
}

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
ACE_INLINE u_long 
ACE_Hash<const wchar_t *>::operator () (const wchar_t *t) const
{
  return ACE::hash_pjw (t);
}

ACE_INLINE u_long 
ACE_Hash<wchar_t *>::operator () (const wchar_t *t) const
{
  return ACE::hash_pjw (t);
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
ACE_INLINE u_long 
ACE_Hash<const ACE_USHORT16 *>::operator () (const ACE_USHORT16 *t) const
{
  return ACE::hash_pjw (t);
}

ACE_INLINE u_long 
ACE_Hash<ACE_USHORT16 *>::operator () (const ACE_USHORT16 *t) const
{
  return ACE::hash_pjw (t);
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

ACE_INLINE int 
ACE_Equal_To<const char *>::operator () (const char *lhs, const char *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}

ACE_INLINE int 
ACE_Equal_To<char *>::operator () (const char *lhs, const char *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
ACE_INLINE int 
ACE_Equal_To<const wchar_t *>::operator () (const wchar_t *lhs, const wchar_t *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}

ACE_INLINE int 
ACE_Equal_To<wchar_t *>::operator () (const wchar_t *lhs, const wchar_t *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
ACE_INLINE int 
ACE_Equal_To<const ACE_USHORT16 *>::operator () (const ACE_USHORT16 *lhs, const ACE_USHORT16 *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}

ACE_INLINE int 
ACE_Equal_To<ACE_USHORT16 *>::operator () (const ACE_USHORT16 *lhs, const ACE_USHORT16 *rhs) const
{
  return !ACE_OS::strcmp (lhs, rhs);
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

ACE_INLINE int 
ACE_Less_Than<const char *>::operator () (const char *lhs, const char *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}

ACE_INLINE int 
ACE_Less_Than<char *>::operator () (const char *lhs, const char *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
ACE_INLINE int 
ACE_Less_Than<const wchar_t *>::operator () (const wchar_t *lhs, const wchar_t *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}

ACE_INLINE int 
ACE_Less_Than<wchar_t *>::operator () (const wchar_t *lhs, const wchar_t *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
ACE_INLINE int 
ACE_Less_Than<const ACE_USHORT16 *>::operator () (const ACE_USHORT16 *lhs, const ACE_USHORT16 *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}

ACE_INLINE int 
ACE_Less_Than<ACE_USHORT16 *>::operator () (const ACE_USHORT16 *lhs, const ACE_USHORT16 *rhs) const
{
  return (ACE_OS::strcmp (lhs, rhs) < 0) ? 1 : 0;
}

#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */
