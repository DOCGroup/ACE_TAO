// -*- C++ -*-

#ifndef TAO_TESTS_NESTED_UPCALL_TRIANGLE_TEST_IOR_FILE_H
#define TAO_TESTS_NESTED_UPCALL_TRIANGLE_TEST_IOR_FILE_H

#include "ace/OS_NS_stdio.h"

#include <memory>

using IOR_File = std::unique_ptr<FILE, int (*) (FILE *)>;

#endif /* TAO_TESTS_NESTED_UPCALL_TRIANGLE_TEST_IOR_FILE_H */
