#include "Select_Fair_Perf_Strategy.h"

#if !defined (__ACE_INLINE__)
#include "Select_Fair_Perf_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, Select_Fair_Perf_Strategy, "$Id$")


ACE_HANDLE
TAO_ACE_Fair_Perf_Selection_Strategy::dispatchable_io_event (
    TAO_ACE_Event_Masks::Event_Masks &t)
{
  ACE_HANDLE handle = ACE_INVALID_HANDLE;

  // Iterate through the write masks first
  handle =
    this->iterate (this->wr_mask_iter_)

  if (handle != ACE_INVALID_HANDLE)
    {
      t = TAO_ACE_Event_Masks::WRITE_MASK;
      return handle;
    }

  // Just reset the state of the <wr_mask_iter_>
  this->wr_mask_iter_.reset_state ();


  // Iterate through the exception mask next
  handle =
    this->iterate (this->ex_mask_iter_);


  if (handle != ACE_INVALID_HANDLE)
    {
      t = TAO_ACE_Event_Masks::EXCEPT_MASK;
      return handle;
    }

  // Just reset the state of the <ex_mask_iter_>
  this->ex_mask_iter_.reset_state ();


  // Iterate through the read mask next
  handle =
    this->iterate (this->rd_mask_iter_);


  if (handle != ACE_INVALID_HANDLE)
    {
      t = TAO_ACE_Event_Masks::READ_MASK;
      return handle;
    }

  // Just reset the state of the <wr_mask_iter_>
  this->rd_mask_iter_.reset_state ();

  return handle;
}




void
TAO_ACE_Select_Fair_Perf_Strategy::clear_handle (
    ACE_HANDLE handle)
{
  this->read_handle_set_.wr_mask_.clr_bit (handle);
  this->read_handle_set_.ex_mask_.clr_bit (handle);
  this->read_handle_set_.rd_mask_.clr_bit (handle);
}


ACE_HANDLE
TAO_ACE_Select_Fair_Perf_Strategy::iterate (
    ACE_Handle_Set_Iterator &handle_iter)
{
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
  while ((handle = handle_iter ()) != ACE_INVALID_HANDLE)
    {
      this->clear_bits (io_set,
                        handle);

      return handle;
    }

  return handle;
}
