// This may look like C, but it's really -*- C++ -*-
// @(#) $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    align.h
//
// = DESCRIPTION
//    Pointer alignment utilities
//
//    A "ptr_arith_t" type is defined for doing numerical operations
//    on pointers, such as aligning them.  Pointer sizes vary from 2
//    to 8 bytes in today's environments; a portable data type is much
//    needed.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems, Inc.
// 
// ============================================================================

#if !defined (TAO_ALIGN_H)
#define	TAO_ALIGN_H

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that "unsigned" versions of a type are the same size as the
// "signed" version of the same type.

#if	SIZEOF_VOID_P == SIZEOF_INT
typedef u_int ptr_arith_t;

#elif	SIZEOF_VOID_P == SIZEOF_LONG
typedef u_long ptr_arith_t;

#elif	SIZEOF_VOID_P == SIZEOF_LONG_LONG
typedef u_long long ptr_arith_t;

#else
#	error "Can't find a suitable type for doing pointer arithmetic."
#endif /* error */

// Efficiently align "value" up to "alignment", knowing that all such
// boundaries are binary powers and that we're using two's complement
// arithmetic.

#if 0
static inline ptr_arith_t
align_binary (const ptr_arith_t value,
	      size_t alignment)
{
  ptr_arith_t temp = alignment - 1;

  return (value + temp) & ~temp;
}
#endif /* 0 */
#define align_binary(ptr, alignment) \
    ((ptr + ((ptr_arith_t)((alignment)-1))) & (~((ptr_arith_t)((alignment)-1))))

// Efficiently round "ptr" up to an "alignment" boundary, knowing that
// all such boundaries are binary powers and that we're using two's
// complement arithmetic.
//
// XXX Returned as "byte pointer" -- CDR module would change to be
// seen as a "void *".  May want to change this to add XDR cleanly.

#if 0
static inline u_char *
ptr_align_binary (const u_char *ptr, 
		  size_t alignment)
{
  return (u_char *) align_binary ((ptr_arith_t) ptr, alignment);
}
#endif /* 0 */
#define ptr_align_binary(ptr, alignment) \
        ((u_char *) align_binary(((ptr_arith_t) (ptr)), (alignment)))

#endif /* TAO_ALIGN_H */
