// $Id$

ACE_INLINE

ACE_RMCast_Resend_Worker::
ACE_RMCast_Resend_Worker (ACE_RMCast_Module *next,
                          ACE_UINT32 max_sequence_number)
  : n (0)
  , next_ (next)
  , max_sequence_number_ (max_sequence_number)
{
}
