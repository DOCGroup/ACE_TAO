/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Map_Manager.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_HASH_MAP_MANAGER_H
#define ACE_HASH_MAP_MANAGER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration since we are going to specialize that template
// here. The template itself requires this file so every user of the
// template should also see the specialization.
template <class TYPE> class ACE_Hash;
template <class TYPE> class ACE_Equal_To;

class ACE_Export ACE_Hash<char>
{
  // = TITLE
  //     Function object for hashing a char
public:
  u_long operator () (char t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<signed char>
{
  // = TITLE
  //     Function object for hashing a signed char
public:
  u_long operator () (signed char t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<unsigned char>
{
  // = TITLE
  //     Function object for hashing an unsigned char
public:
  u_long operator () (unsigned char t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<short>
{
  // = TITLE
  //     Function object for hashing a short
public:
  u_long operator () (short t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<unsigned short>
{
  // = TITLE
  //     Function object for hashing an unsigned short
public:
  u_long operator () (unsigned short t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<int>
{
  // = TITLE
  //     Function object for hashing an int
public:
  u_long operator () (int t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<unsigned int>
{
  // = TITLE
  //     Function object for hashing an unsigned int
public:
  u_long operator () (unsigned int t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<long>
{
  // = TITLE
  //     Function object for hashing a long 
public:
  u_long operator () (long t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<unsigned long>
{
  // = TITLE
  //     Function object for hashing an unsigned long 
public:
  u_long operator () (unsigned long t) const;  
  // Simply returns t
};

class ACE_Export ACE_Hash<const char *>
{
  // = TITLE
  //     Function object for hashing a const string
public:
  u_long operator () (const char *t) const;  
  // Calls ACE::hash_pjw
};

class ACE_Export ACE_Hash<char *>
{
  // = TITLE
  //     Function object for hashing a string
public:
  u_long operator () (const char *t) const;  
  // Calls ACE::hash_pjw
};

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
class ACE_Export ACE_Hash<const wchar_t *>
{
  // = TITLE
  //     Function object for hashing a const wide string
public:
  u_long operator () (const wchar_t *t) const;  
  // Calls ACE::hash_pjw
};

class ACE_Export ACE_Hash<wchar_t *>
{
  // = TITLE
  //     Function object for hashing a wide string
public:
  u_long operator () (const wchar_t *t) const;  
  // Calls ACE::hash_pjw
};
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
class ACE_Export ACE_Hash<const ACE_USHORT16 *>
{
  // = TITLE
  //     Function object for hashing a const wide string
public:
  u_long operator () (const ACE_USHORT16 *t) const;  
  // Calls ACE::hash_pjw
};

class ACE_Export ACE_Hash<ACE_USHORT16 *>
{
  // = TITLE
  //     Function object for hashing a wide string
public:
  u_long operator () (const ACE_USHORT16 *t) const;  
  // Calls ACE::hash_pjw
};
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

class ACE_Export ACE_Equal_To<const char *>
{
  // = TITLE
  //     Function object for comparing two const strings
public:
  int operator () (const char *lhs,
                   const char *rhs) const;  
  // Simply calls strcmp
};

class ACE_Export ACE_Equal_To<char *>
{
  // = TITLE
  //     Function object for comparing two strings
public:
  int operator () (const char *lhs,
                   const char *rhs) const;  
  // Simply calls strcmp
};

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_CHAR)
class ACE_Export ACE_Equal_To<const wchar_t *>
{
  // = TITLE
  //     Function object for comparing two const wide strings
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const;  
  // Simply calls strcmp
};

class ACE_Export ACE_Equal_To<wchar_t *>
{
  // = TITLE
  //     Function object for comparing two wide strings
public:
  int operator () (const wchar_t *lhs,
                   const wchar_t *rhs) const;  
  // Simply calls strcmp
};
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_CHAR */

#if !defined (ACE_HAS_WCHAR_TYPEDEFS_USHORT)
class ACE_Export ACE_Equal_To<const ACE_USHORT16 *>
{
  // = TITLE
  //     Function object for comparing two const wide strings
public:
  int operator () (const ACE_USHORT16 *lhs,
                   const ACE_USHORT16 *rhs) const;  
  // Simply calls strcmp
};

class ACE_Export ACE_Equal_To<ACE_USHORT16 *>
{
  // = TITLE
  //     Function object for comparing two wide strings
public:
  int operator () (const ACE_USHORT16 *lhs,
                   const ACE_USHORT16 *rhs) const;  
  // Simply calls strcmp
};
#endif /* ! ACE_HAS_WCHAR_TYPEDEFS_USHORT */

// Include the templates here.
#include "ace/Hash_Map_Manager_T.h"

#if defined (__ACE_INLINE__)
#include "ace/Hash_Map_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HASH_MAP_MANAGER_H */
