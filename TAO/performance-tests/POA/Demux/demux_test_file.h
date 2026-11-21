#ifndef TAO_DEMUX_TEST_FILE_H
#define TAO_DEMUX_TEST_FILE_H

#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <memory>

namespace Demux_Test_Utils
{
  struct File_Closer
  {
    void operator() (FILE *file) const
    {
      if (file != nullptr)
        ACE_OS::fclose (file);
    }
  };

  using File_Handle = std::unique_ptr<FILE, File_Closer>;
}

#endif /* TAO_DEMUX_TEST_FILE_H */
