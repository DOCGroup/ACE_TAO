// $Id$

namespace Kokyu
{
ACE_INLINE
Shutdown_Task_Command::Shutdown_Task_Command (ACE_Allocator *mb_allocator)
{
  ACE_UNUSED_ARG ((mb_allocator));
}

ACE_INLINE int
Shutdown_Task_Command::execute ()
{
  return -1;
}
}
