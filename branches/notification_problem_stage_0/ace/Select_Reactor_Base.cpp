// $Id$

#include "ace/Select_Reactor_Base.h"
#include "ace/Reactor.h"
#include "ace/Thread.h"
#include "ace/Synch_T.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Timer_Heap.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Select_Reactor_Base.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Select_Reactor_Base, "$Id$")

// Perform GET, CLR, SET, and ADD operations on the Handle_Sets.
//
// GET = 1, Retrieve current value
// SET = 2, Set value of bits to new mask (changes the entire mask)
// ADD = 3, Bitwise "or" the value into the mask (only changes
//          enabled bits)
// CLR = 4  Bitwise "and" the negation of the value out of the mask
//          (only changes enabled bits)
//
// Returns the original mask.  Must be called with locks held.

int
ACE_Select_Reactor_Impl::bit_ops (ACE_HANDLE handle,
                                  ACE_Reactor_Mask mask,
                                  ACE_Select_Reactor_Handle_Set &handle_set,
                                  int ops)
{
  ACE_TRACE ("ACE_Select_Reactor_Impl::bit_ops");
  if (this->handler_rep_.handle_in_range (handle) == 0)
    return -1;

#if !defined (ACE_WIN32)
  ACE_Sig_Guard sb; // Block out all signals until method returns.
#endif /* ACE_WIN32 */

  ACE_FDS_PTMF ptmf  = &ACE_Handle_Set::set_bit;
  u_long omask = ACE_Event_Handler::NULL_MASK;

  // Find the old reactor masks.  This automatically does the work of
  // the GET_MASK operation.
  if (handle_set.rd_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::READ_MASK);
  if (handle_set.wr_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::WRITE_MASK);
  if (handle_set.ex_mask_.is_set (handle))
    ACE_SET_BITS (omask, ACE_Event_Handler::EXCEPT_MASK);

  switch (ops)
    {
    case ACE_Reactor::GET_MASK:
      // The work for this operation is done in all cases at the
      // begining of the function.
      break;
    case ACE_Reactor::CLR_MASK:
      ptmf = &ACE_Handle_Set::clr_bit;
      /* FALLTHRU */
    case ACE_Reactor::SET_MASK:
      /* FALLTHRU */
    case ACE_Reactor::ADD_MASK:

      // The following code is rather subtle...  Note that if we are
      // doing a ACE_Reactor::SET_MASK then if the bit is not enabled
      // in the mask we need to clear the bit from the ACE_Handle_Set.
      // On the other hand, if we are doing a ACE_Reactor::CLR_MASK or
      // a ACE_Reactor::ADD_MASK we just carry out the operations
      // specified by the mask.

      // READ, ACCEPT, and CONNECT flag will place the handle in the
      // read set.
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::READ_MASK)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::ACCEPT_MASK)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::CONNECT_MASK))
        {
          (handle_set.rd_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.rd_mask_.clr_bit (handle);

      // WRITE and CONNECT flag will place the handle in the write set
      if (ACE_BIT_ENABLED (mask,
                           ACE_Event_Handler::WRITE_MASK)
          || ACE_BIT_ENABLED (mask,
                              ACE_Event_Handler::CONNECT_MASK))
        {
          (handle_set.wr_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.wr_mask_.clr_bit (handle);

      // EXCEPT (and CONNECT on Win32) flag will place the handle in
      // the except set.
      if (ACE_BIT_ENABLED (mask, ACE_Event_Handler::EXCEPT_MASK)
#if defined (ACE_WIN32)
          || ACE_BIT_ENABLED (mask, ACE_Event_Handler::CONNECT_MASK)
#endif /* ACE_WIN32 */
          )
        {
          (handle_set.ex_mask_.*ptmf) (handle);
        }
      else if (ops == ACE_Reactor::SET_MASK)
        handle_set.ex_mask_.clr_bit (handle);
      break;
    default:
      return -1;
    }
  return omask;
}

int
ACE_Select_Reactor_Impl::resumable_handler (void)
{
  // The select reactor has no handlers that can be resumed by the
  // application. So return 0;

  return 0;
}
