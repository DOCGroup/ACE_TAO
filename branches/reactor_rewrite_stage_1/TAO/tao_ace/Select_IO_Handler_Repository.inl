/* -*- C++ -*- */
//$Id$
ACE_INLINE int
TAO_ACE_Select_IO_Handler_Repository::max_handlep1 (void) const
{
  return this->max_handlep1_;
}

ACE_INLINE TAO_ACE_Select_Strategy_Handle_Set &
TAO_ACE_Select_IO_Handler_Repository:: wait_set (void)
{
  return this->wait_set_;
}


ACE_INLINE int
TAO_ACE_Select_IO_Handler_Repository::is_handle_valid (ACE_HANDLE h)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::is_handle_valid");

#if defined (ACE_WIN32)
  // Not exactly sure how to get the validity of Win32 handles that
  // are passed.
  if (h == ACE_INVALID_HANDLE)
#else /* !ACE_WIN32 */

  if (h < 0 || h >= this->max_size_)
#endif /* ACE_WIN32 */
    {
      errno = EINVAL;
      return -1;
    }

  return 0;
}

ACE_INLINE int
TAO_ACE_Select_IO_Handler_Repository::is_event_mask_available (ACE_HANDLE h)
{
  ACE_TRACE ("TAO_ACE_Select_IO_Handler_Repository::is_event_mask_available");

  if (this->wait_set_.rd_mask_.is_set (h) == 0
      && this->wait_set_.wr_mask_.is_set (h) == 0
      && this->wait_set_.ex_mask_.is_set (h) == 0)
    return 0;

  return 1;
}
