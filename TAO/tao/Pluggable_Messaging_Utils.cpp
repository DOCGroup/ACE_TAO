//$Id$
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
#include "tao/Pluggable_Messaging_Utils.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Pluggable_Messaging_Utils, "$Id$")

TAO_Pluggable_Reply_Params::TAO_Pluggable_Reply_Params (TAO_ORB_Core *orb_core)
  : svc_ctx_ ()
  , request_id_ (0)
  , reply_status_ (0)
  , is_dsi_ (0)
  , dsi_nvlist_align_ (0)
  , argument_flag_ (0)
  , input_cdr_ (orb_core->create_input_cdr_data_block (
                  ACE_CDR::DEFAULT_BUFSIZE),
                TAO_ENCAP_BYTE_ORDER,
                orb_core)
  , service_context_ (0)
{
}
