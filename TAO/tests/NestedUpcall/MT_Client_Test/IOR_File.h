// -*- C++ -*-

#ifndef TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H
#define TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H

#include "ace/Malloc_Base.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

struct MT_ACE_Allocator_Deleter
{
  void operator() (char *buffer) const
  {
    ACE_Allocator::instance ()->free (buffer);
  }
};

using MT_ACE_Allocated_Buffer = std::unique_ptr<char, MT_ACE_Allocator_Deleter>;
using MT_IOR_File = std::unique_ptr<FILE, int (*) (FILE *)>;

#endif /* TAO_TESTS_NESTED_UPCALL_MT_CLIENT_TEST_IOR_FILE_H */
