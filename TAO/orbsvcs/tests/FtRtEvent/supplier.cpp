// $Id$
#include "FtRtEvent_Test.h"
#include "PushSupplier.h"

ACE_RCSID (FtRtEvent,
           supplier,
           "$Id$")

int main(int argc, ACE_TCHAR** argv)
{
  FtRtEvent_Test<PushSupplier_impl> test;
  return test.run(argc, argv);
}
