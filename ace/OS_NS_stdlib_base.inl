/* -*- C++ -*- */
// $Id$

// Standard library includes for wide functions declared in ace_wchar.h

#ifndef OS_NS_STDLIB_BASE_INL
#define OS_NS_STDLIB_BASE_INL

namespace ACE_OS
{

template <typename DCHAR, typename SCHAR> inline
size_t string_copy (DCHAR* dest, const SCHAR* src, size_t len)
{
  // We must have a source and a valid length with a dest
  if ( src == 0 || ( dest != 0 && len < 0 ) )
  {
    return -1;
  }
  // If we perform a copy
  if ( dest != 0 )
  {
    if (len != 0)
    {
      DCHAR* d = dest;
      const SCHAR* s = src;
      do
      {
        if ((*d++ = static_cast<DCHAR>(*s++)) == 0)
        {
          // NUL pad the remaining n-1 bytes
          while (--len != 0)
            *d++ = 0;
          break;
        }
      } while (--len != 0);
      return static_cast<size_t>( d - dest );
    }
    return 0;
  }
  // Otherwise just calc length
  return ACE_OS::strlen( src );
}

#if !defined(ACE_LACKS_WCSTOMBS)
template <> inline
size_t string_copy (char* s, const wchar_t* t, size_t len)
{
#if defined(ACE_WIN32)
   // When VC6 support is dropped use wcstombs
  if ( t == 0 )
    return -1;
  int size = ::WideCharToMultiByte(
              CP_OEMCP, 0,
              t, -1,
              s, (s ? len : 0),
              0, 0 );
  // wcstombs returns len NOT size when dest == 0
  return ( s == 0 ? size - 1 : size );
#else
  return ::wcstombs( s, t, len );
#endif
}
#endif /* !ACE_LACKS_WCSTOMBS */

#if !defined(ACE_LACKS_MBSTOWCS)
template <> inline
size_t string_copy (wchar_t* s, const char* t, size_t len)
{
#if defined(ACE_WIN32)
   // When VC6 support is dropped use mbstowcs
  if ( t == 0 )
    return -1;
  int size = ::MultiByteToWideChar(
                CP_OEMCP, 0,
                t, -1,
                s, (s ? len : 0) );
  // mbstowcs returns len NOT size when dest == 0
  return ( s == 0 ? size - 1 : size );
#else
  return ::mbstowcs( s, t, len );
#endif
}
#endif

}

#endif
