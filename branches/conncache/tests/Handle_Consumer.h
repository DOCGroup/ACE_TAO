// $Id$

#ifndef _HANDLE_CONSUMER_H
#define _HANDLE_CONSUMER_H

#include "ace/OS.h"
#include "ace/Containers_T.h"

class Handle_Consumer
{
  // = TITLE 
  //    This class gobbles up descriptors on demand.
  //
  // = DESCRIPTION
  //     This is useful when we need to control the number of
  //     descriptors available for a process.

 public:

  ~Handle_Consumer (void);
  // Constructor.

  int consume_handles (size_t n_handles_to_keep_available);
  // The descriptors are opened continously until the process runs out
  // of them, and then only the number which have to be made available
  // are closed thereby making them usable in the future.

  int keep_available (size_t n_handles);
  // Denotes the number of handles to retain.
  
  void close_remaining_handles (void);
  // All remaining handles are relased.

 private:

  typedef ACE_Unbounded_Set<ACE_HANDLE> HANDLE_SET;
  HANDLE_SET handle_set_;
  // The container which holds the open descriptors.

};


 
Handle_Consumer::~Handle_Consumer (void)
{
  this->close_remaining_handles ();
}

int
Handle_Consumer::consume_handles (size_t n_handles_to_keep_available)
{
  int result = 0;
  ACE_HANDLE handle = ACE_INVALID_HANDLE;

  while (1)
    {
      handle = ACE_OS::open (ACE_DEV_NULL,
                             O_WRONLY);

      if (handle == ACE_INVALID_HANDLE)
        {
          if (ACE::out_of_handles (errno))
            {
              result = this->keep_available (n_handles_to_keep_available);
              break;
            }
          else
            {
              result = -1;
              break;
            }
        }

      result = this->handle_set_.insert (handle);
      if (result == -1)
        break;
    }

  return result;
}

int
Handle_Consumer::keep_available (size_t n_handles)
{
  HANDLE_SET::iterator iterator =
    this->handle_set_.begin ();

  HANDLE_SET::iterator end =
    this->handle_set_.end ();

  for (;
       iterator != end && n_handles > 0;
       ++iterator, --n_handles)
    {
      int result = ACE_OS::close (*iterator);
      if (result != 0)
        return result;
    }

  return 0;
}

void
Handle_Consumer::close_remaining_handles (void)
{
  HANDLE_SET::iterator iterator =
    this->handle_set_.begin ();

  HANDLE_SET::iterator end =
    this->handle_set_.end ();

  for (;
       iterator != end;
       ++iterator)
    {
      ACE_OS::close (*iterator);
    }
}

#endif /* _HANDLER_CONSUMER_H */
