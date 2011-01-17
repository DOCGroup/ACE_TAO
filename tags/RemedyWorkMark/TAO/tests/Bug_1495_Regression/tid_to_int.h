// -*- C++ -*-

/**
 * @file tid_to_int.h
 *
 * $Id$
 *
 * Convert an ACE_thread_t to an integer in a way that doesn't rely
 * heavily on platform-specific configuration.
 *
 * @author Ossama Othman
 */

namespace
{
  template<typename thread_id_type, typename ace_thread_id_type>
  struct ACE_thread_t_to_integer_i
  {
    thread_id_type operator() (ace_thread_id_type tid)
    {
      // We assume sizeof(thread_id_type) >= sizeof(ace_thread_id_type).
      return (thread_id_type) (tid);
    }
  };

  template<typename thread_id_type, typename ace_thread_id_type>
  struct ACE_thread_t_to_integer_i<thread_id_type, ace_thread_id_type*>
  {
    thread_id_type operator() (ace_thread_id_type* tid)
    {
      // ACE_thread_t is a pointer.  Cast to an intermediate integer
      // type large enough to hold a pointer.
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
      int const tmp = reinterpret_cast<int> (tid);
#else
      intptr_t const tmp = reinterpret_cast<intptr_t> (tid);
#endif

      // We assume sizeof(thread_id_type) >= sizeof(ace_thread_id_type).
      return (thread_id_type) tmp;
    }
  };

  template<typename thread_id_type>
  thread_id_type
  ACE_thread_t_to_integer (ACE_thread_t tid)
  {
    return ACE_thread_t_to_integer_i<thread_id_type, ACE_thread_t>() (tid);
  }
}
