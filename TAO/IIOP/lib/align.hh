// @(#)align.hh	1.1 95/11/04
// Copyright 1994-1995 by Sun Microsystems, Inc.
//
// Pointer alignment utilities
//
// A "ptr_arith_t" type is defined for doing numerical operations on
// pointers, such as aligning them.  Pointer sizes vary from 2 to 8
// bytes in today's environments; a portable data type is much needed.
//

#ifndef	_align_hh
#define	_align_hh

//
// Type for doing arithmetic on pointers ... as elsewhere, we assume
// that "unsigned" versions of a type are the same size as the "signed"
// version of the same type.
//

#if	SIZEOF_VOID_P == SIZEOF_INT
typedef unsigned int ptr_arith_t;

#elif	SIZEOF_VOID_P == SIZEOF_LONG
typedef unsigned long ptr_arith_t;

#elif	SIZEOF_VOID_P == SIZEOF_LONG_LONG
typedef unsigned long long ptr_arith_t;

#else
#	error "Can't find a suitable type for doing pointer arithmetic."
#endif



//
// Efficiently align "value" up to "alignment", knowing that all such
// boundaries are binary powers and that we're using two's complement
// arithmetic.
//
static inline ptr_arith_t
align_binary (const ptr_arith_t value, size_t alignment)
{
    ptr_arith_t		 temp = alignment - 1;

    return (value + temp) & ~temp;
}


//
// Efficiently round "ptr" up to an "alignment" boundary, knowing that
// all such boundaries are binary powers and that we're using two's
// complement arithmetic.
//
// XXX Returned as "byte pointer" -- CDR module would change to be seen
// as a "void *".  May want to change this to add XDR cleanly.
//
static inline unsigned char *
ptr_align_binary (const unsigned char *ptr, size_t alignment)
{
    return (unsigned char *) align_binary ((ptr_arith_t) ptr, alignment);
}

#endif	// _align_hh
