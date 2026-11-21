// -*- C++ -*-

#ifndef TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H
#define TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H

#include "ace/OS_NS_stdio.h"
#include "ace/Read_Buffer.h"

#include <memory>

using MT_IOR_File = std::unique_ptr<FILE, int (*) (FILE *)>;

#endif /* TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H */
