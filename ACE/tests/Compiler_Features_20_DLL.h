
//=============================================================================
/**
 *  @file    Compiler_Features_20_DLL.h
 *
 *  Test DLL export with default constructor
 */
//=============================================================================

#ifndef ACE_TESTS_COMPILER_FEATURES_20_DLL_H
#define ACE_TESTS_COMPILER_FEATURES_20_DLL_H

#include "test_config.h"
#include "Compiler_Features_20_DLL_Export.h"

class COMPILER_FEATURES_20_DLL_Export UtcT
{
public:
  ~UtcT () = default;
};

#endif /* ACE_TESTS_COMPILER_FEATURES_20_DLL_H */
