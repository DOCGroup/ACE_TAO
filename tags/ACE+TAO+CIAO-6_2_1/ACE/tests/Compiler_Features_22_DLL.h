
//=============================================================================
/**
 *  @file    Compiler_Features_22_DLL.h
 *
 *  $Id$
 *
 *  Test DLL export with a template method specialization
 */
//=============================================================================

#ifndef ACE_TESTS_COMPILER_FEATURES_22_DLL_H
#define ACE_TESTS_COMPILER_FEATURES_22_DLL_H

#include /**/ "ace/pre.h"

#include "test_config.h"

#include "Compiler_Features_22_DLL_Export.h"

class A
{
public:

A () {};
};

typedef A* A_ptr;

template<typename T> inline bool is_nil (T x)
{
return x == 0;
}

template<>
bool
COMPILER_FEATURES_22_DLL_Export is_nil<> (A_ptr)
{
  return false;
}

#include /**/ "ace/post.h"

#endif /* ACE_TESTS_COMPILER_FEATURES_22_DLL_H */
