/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company and Microsoft
 * Corporation make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 */

#ifndef DEFALLOC_H
#define DEFALLOC_H

#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <algobase.h>

#if 0
inline void* operator new(size_t, void* p) {return p;}
#endif

/*
 * the following template function is replaced by the following two functions
 * due to the fact that the Borland compiler doesn't change prediff_t type
 * to type long when compile with -ml or -mh.


template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
  cerr << "out of memory" << endl; 
  exit(1);
    }
    return tmp;
}
*/

/*
 * Begin change by Terris
 */
namespace std {
/*
 * End change by Terris
 */

/*
* Added by Terris (#if out code)
 */
#if 0
/*
 * Added by Terris End
 */
template <class T>
inline T* allocate(int size, T*) {
   set_new_handler(0);
   T* tmp = (T*)(::operator new((unsigned int)(size * sizeof(T))));
   if (tmp == 0) {
  cerr << "out of memory" << endl; 
  exit(1);
    }
    return tmp;
}
/*
 * Added by Terris
 */
#endif
/*
 * Added by Terris End
 */

template <class T>
inline T* allocate(int size, T*) { // Changed by Terris -- was long.
/*
 * Begin Change by Terris 
 */
//    set_new_handler(0);
//    T* tmp = (T*)(::operator new((unsigned long)(size * sizeof(T))));
//    if (tmp == 0) {
//  cerr << "out of memory" << endl; 
//  exit(1);
//    }
//    return tmp;
/*
 * End Change by Terris 
 */
  return (T*)(::operator new((unsigned long)(size * sizeof(T))));
}

template <class T>
inline void deallocate(T* buffer) {
    ::operator delete(buffer);
}

template <class T>
inline void destroy(T* pointer) {
    pointer->~T();
}

inline void destroy(char*) {}
inline void destroy(unsigned char*) {}
inline void destroy(short*) {}
inline void destroy(unsigned short*) {}
inline void destroy(int*) {}
inline void destroy(unsigned int*) {}
inline void destroy(long*) {}
inline void destroy(unsigned long*) {}
inline void destroy(float*) {}
inline void destroy(double*) {}
inline void destroy(char**) {}
inline void destroy(unsigned char**) {}
inline void destroy(short**) {}
inline void destroy(unsigned short**) {}
inline void destroy(int**) {}
inline void destroy(unsigned int**) {}
inline void destroy(long**) {}
inline void destroy(unsigned long**) {}
inline void destroy(float**) {}
inline void destroy(double**) {}

inline void destroy(char*, char*) {}
inline void destroy(unsigned char*, unsigned char*) {}
inline void destroy(short*, short*) {}
inline void destroy(unsigned short*, unsigned short*) {}
inline void destroy(int*, int*) {}
inline void destroy(unsigned int*, unsigned int*) {}
inline void destroy(long*, long*) {}
inline void destroy(unsigned long*, unsigned long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
inline void destroy(char**, char**) {}
inline void destroy(unsigned char**, unsigned char**) {}
inline void destroy(short**, short**) {}
inline void destroy(unsigned short**, unsigned short**) {}
inline void destroy(int**, int**) {}
inline void destroy(unsigned int**, unsigned int**) {}
inline void destroy(long**, long**) {}
inline void destroy(unsigned long**, unsigned long**) {}
inline void destroy(float**, float**) {}
inline void destroy(double**, double**) {}

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
    new (p) T1(value);
}

template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    
    /* 
     * Begin change by Terris 
     *
     * This removes compile-time warnings about signed/unsigned mismatch
     * and negating unsigned values.
     * typedef size_t size_type;
     */
    typedef long size_type;
    /* 
     * End change by Terris 
     */
    
    typedef ptrdiff_t difference_type;
    pointer allocate(size_type n) { 
  return std::allocate((difference_type)n, (pointer)0);
    }
    void deallocate(pointer p) { std::deallocate(p); }
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { 
  return (const_pointer)&x; 
    }
    size_type init_page_size() { 
  return max(size_type(1), size_type(4096/sizeof(T)));
    }
    size_type max_size() const { 
  return max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
    }
};

class allocator<void> {
public:
    typedef void* pointer;
};




/*
 *Added by d:\\convert.pl --begin--
 */
}
/*
 *Added by d:\\convert.pl --end--
 */

#endif
