// $Id$

// Since this is only included in Handle_Gobbler.h, these should be
// inline, not ACE_INLINE.
// FUZZ: disable check_for_inline

inline void
ACE_Handle_Gobbler::close_remaining_handles (void)
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

inline
ACE_Handle_Gobbler::~ACE_Handle_Gobbler (void)
{
  this->close_remaining_handles ();
}

inline int
ACE_Handle_Gobbler::free_handles (size_t n_handles)
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

inline int
ACE_Handle_Gobbler::consume_handles (size_t n_handles_to_keep_available)
{
  int result = 0;

#if defined(ACE_WIN32)
  // On Win32, this style of gobbling doesn't seem to work.
  ACE_UNUSED_ARG(n_handles_to_keep_available);

#else

  while (1)
    {
      ACE_HANDLE handle = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);

      if (handle == ACE_INVALID_HANDLE)
        {
          if (ACE::out_of_handles (errno))
            {
              result = this->free_handles (n_handles_to_keep_available);
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

#endif /* ACE_WIN32 */

  return result;
}
