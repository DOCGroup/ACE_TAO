// This may look like C, but it's really -*- C++ -*-
// @(#) $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Align.h
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
#define TAO_ALIGN_H

// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that "unsigned" versions of a type are the same size as the
// "signed" version of the same type.

#if     ACE_SIZEOF_VOID_P == ACE_SIZEOF_INT
typedef u_int ptr_arith_t;

#elif   ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG
typedef u_long ptr_arith_t;

#elif   ACE_SIZEOF_VOID_P == ACE_SIZEOF_LONG_LONG
typedef u_long long ptr_arith_t;

#else
#       error "Can't find a suitable type for doing pointer arithmetic."
#endif /* error */

// Efficiently align "value" up to "alignment", knowing that all such
// boundaries are binary powers and that we're using two's complement
// arithmetic.

// Since the alignment is a power of two its binary representation is:
//    alignment      = 0...010...0
//
// hence 
//
//    alignment - 1  = 0...001...1 = T1
//
// so the completent is:
//
//  ~(alignment - 1) = 1...110...0 = T2
//
// Notice that there is a multiple of <alignment> in the range 
// [<value>,<value> + T1], also notice that if
//
// X = ( <value> + T1 ) & T2
//
// then
//
// <value> <= X <= <value> + T1
//
// because the & operator only changes the last bits, and since X is a
// multiple of <alignment> (its last bits are zero) we have found the
// multiple we wanted.
// 

#define align_binary(ptr, alignment) \
    ((ptr + ((ptr_arith_t)((alignment)-1))) & (~((ptr_arith_t)((alignment)-1))))

// Efficiently round "ptr" up to an "alignment" boundary, knowing that
// all such boundaries are binary powers and that we're using two's
// complement arithmetic.
//
#define ptr_align_binary(ptr, alignment) \
        ((char *) align_binary (((ptr_arith_t) (ptr)), (alignment)))

#endif /* TAO_ALIGN_H */
