/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Array_Helper.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================


#ifndef ACE_URL_ARRAY_HELPER_H
#define ACE_URL_ARRAY_HELPER_H

// ### These template functions are probably named improperly.
// You should find some way to avoid name space polution.

template <class T>
size_t ace_array_size (const T &x);

template <class T>
size_t ace_array_encode (void *buf, const T &x);

template <class T>
size_t ace_array_decode (void *buf, T &x);

#include "URL_Array_Helper.cpp"

#endif /* ACE_URL_ARRAY_HELPER_H */
