#include "Handle_L_SPIPE.h"
// $Id$


#if !defined (__ACE_INLINE__)
#include "Handle_L_SPIPE.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_STREAM_PIPES)

const char *Handle_L_SPIPE::DEFAULT_RENDEZVOUS = "/tmp/foo_spipe";

Handle_L_SPIPE local_spipe;
ACE_Service_Object_Type lsp (&local_spipe, "Local_SPIPE");

#endif /* ACE_HAS_STREAM_PIPES */
