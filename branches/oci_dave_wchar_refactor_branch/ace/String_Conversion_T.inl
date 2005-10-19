/* -*- C++ -*- */
// $Id$
\
#ifndef STRING_CONVERSION_T_I
#define STRING_CONVERSION_T_I

#include "ace/String_Conversion_T.h"
#include "ace/OS_NS_stdlib_base.h"
#include "ace/OS_NS_string_base.h"

namespace ACE
{

namespace String_Conversion
{

template < typename T >
alloc< T >::alloc( size_t len )
: str_( 0 )
{
  str_ = new T[len + 1];
}


template < typename T >
alloc< T >::operator T* () const
{
  return str_;
}


template < typename T > void
free( T* str )
{
  delete[] str;
}

template < typename DestT, typename SrcT > inline
DestT* duplicate( const SrcT* src, size_t* dest_len )
{
  DestT* dest = 0;

  if ( src == 0 )
  {
    return dest;
  }

  size_t len = ACE_OS::string_copy( static_cast<DestT*>(0), src, 0 );

  if ( dest_len != 0 )
  {
    if ( *dest_len == calc_len )
      *dest_len = len;
    else if ( *dest_len < len )
      len = *dest_len;
  }

  dest = alloc<DestT>( len );

  copy( dest, src, len );

  return dest;
}


template < typename DestT, typename SrcT > inline
void copy( DestT* dest, const SrcT* src, size_t dest_len )
{
  // Note: dest should not be null, but no assert is available.
  //       Just crash with null pointer dereference.
  if (src == 0 || dest_len == 0)
  {
    dest[0] = 0;
    return;
  }
  if ( dest_len == calc_len )
  {
    dest_len = ACE_OS::string_copy( static_cast<DestT*>(0), src, 0 );
  }
  ACE_OS::string_copy( dest, src, dest_len + 1 );
  dest[ dest_len ] = 0;
}

template < typename DestT > ACE_INLINE DestT*
Convert_Out< DestT >::c_str() const
{
  return dest_;
}


template < typename DestT > ACE_INLINE int
Convert_Out< DestT >::length() const
{
  return len_;
}

template < typename DestT, typename SrcT > ACE_INLINE
Convert_In< DestT, SrcT >::~Convert_In()
{
  free( ownstr_ );
}


template < typename DestT, typename SrcT > ACE_INLINE const DestT*
Convert_In< DestT, SrcT >::c_str( void ) const
{
  return ( str_ != 0 ? str_ : ownstr_ );
}

template < typename DestT, typename SrcT > ACE_INLINE
Convert_InOut< DestT, SrcT >::~Convert_InOut( void )
{
  if (ownstr_ != 0)
  {
    copy(orig_, ownstr_, len_);
    free( ownstr_ );
  }
}


template < typename DestT, typename SrcT > ACE_INLINE DestT*
Convert_InOut< DestT, SrcT >::c_str( void )
{
  return ( str_ != 0 ? str_ : ownstr_ );
}

} // String_Conversion

} // Ace

#endif /* STRING_CONVERSION_T_I */
