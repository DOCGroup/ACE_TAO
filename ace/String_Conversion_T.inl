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

template < typename T > ACE_INLINE
T*
Allocator_cpp< T >::alloc( size_t len ) const
{
  T* result = new T [len+1];
  result[ 0 ] = 0;
  return result;
}

template < typename T > ACE_INLINE 
void
Allocator_cpp< T >::free( T* str ) const
{
  delete[] str;
}

template < typename T > ACE_INLINE 
T*
Allocator_malloc< T >::alloc( size_t len ) const
: str_( 0 )
{
  T* result = static_cast<T*>::malloc((len+1)*sizeof(T));
  result[ 0 ] = 0;
  return result;
}

template < typename T > ACE_INLINE
void
Allocator_malloc< T >::free( T* str ) const
{
  ::free (str_);
}

template < typename DestT, typename SrcT, typename Allocator > ACE_INLINE
DestT*
duplicate( const SrcT* src, size_t* dest_len )
{
  DestT* dest = 0;

  if ( src == 0 )
  {
    return dest;
  }

  size_t len = ACE_OS::string_copy( static_cast<DestT*>(0), src, 0 );

  if ( dest_len != 0 )
  {
    if ( *dest_len < len && *dest_len != calc_len )
      len = *dest_len;
    else
      *dest_len = len;
  }

  dest = Allocator().alloc( len );

  copy( dest, src, len );

  return dest;
}


template < typename DestT, typename SrcT > ACE_INLINE
size_t
copy( DestT* dest, const SrcT* src, size_t dest_len )
{
  if ( dest == 0 )
  {
    return -1;
  }
  if (src == 0 || dest_len == 0)
  {
    dest[0] = 0;
    return 0;
  }
  if ( dest_len == calc_len )
  {
    dest_len = ACE_OS::string_copy( static_cast<DestT*>(0), src, 0 );
  }
  int result = ACE_OS::string_copy( dest, src, dest_len + 1 );
  dest[ dest_len ] = 0;
  return result;
}

template < typename DestT, typename Allocator > ACE_INLINE
DestT*
Convert_Out< DestT, Allocator >::c_str() const
{
  return dest_;
}


template < typename DestT, typename Allocator > ACE_INLINE
size_t
Convert_Out< DestT, Allocator >::length() const
{
  return len_;
}

template < typename DestT, typename SrcT > ACE_INLINE
Convert_In< DestT, SrcT >::~Convert_In()
{
  Allocator_cpp<DestT>().free( ownstr_ );
}


template < typename DestT, typename SrcT > ACE_INLINE
const DestT*
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
    Allocator_cpp<DestT>().free( ownstr_ );
  }
}


template < typename DestT, typename SrcT > ACE_INLINE
DestT*
Convert_InOut< DestT, SrcT >::c_str( void )
{
  return ( str_ != 0 ? str_ : ownstr_ );
}

} // String_Conversion

} // Ace

#endif /* STRING_CONVERSION_T_I */
