// $Id$

#include "FtRtEvent_Test.h"
#include "PushConsumer.h"

ACE_RCSID (FtRtEvent,
           consumer,
           "$Id$")


int main(int argc, ACE_TCHAR** argv)
{
  FtRtEvent_Test<PushConsumer_impl> test;
  return test.run(argc, argv);
}
