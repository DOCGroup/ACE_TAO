// $Id$

#include "RMCast_Header_Size.h"

#if !defined (__ACE_INLINE__)
#include "RMCast_Header_Size.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Header_Size, "$Id$")

ACE_RMCast_Header_Size::
ACE_RMCast_Header_Size (void)
  :  ACE_Message_Block (0, ACE_Message_Block::MB_PROTO)
{
}
