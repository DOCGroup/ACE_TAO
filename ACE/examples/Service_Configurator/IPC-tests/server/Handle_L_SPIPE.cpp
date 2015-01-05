// $Id$

#include "Handle_L_SPIPE.h"

#if !defined (__ACE_INLINE__)
#include "Handle_L_SPIPE.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_STREAM_PIPES)

const ACE_TCHAR *Handle_L_SPIPE::DEFAULT_RENDEZVOUS = ACE_TEXT ("/tmp/foo_spipe");

Handle_L_SPIPE local_spipe;
ACE_Service_Object_Type lsp (&local_spipe, ACE_TEXT ("Local_SPIPE"));

#endif /* ACE_HAS_STREAM_PIPES */
