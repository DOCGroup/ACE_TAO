// -*- C++ -*-
//$Id$

ACE_INLINE
TAO_Pluggable_Connector_Params::TAO_Pluggable_Connector_Params (void)
  :svc_ctx_ (0),
   request_id_ (0),
   reply_status_ (0)
{
  // @@ Bala: this comment is as useful as a hole in the head.  First
  // the constructor is *not* a no-op, the initialization section was
  // non-trivial, and the constructor sets up the virtual table and a
  // bunch of other things.  Furthermore I can *see* that the function
  // is empty, the comment is just adding noise.  It reminds me of the
  // following example of what *not* to put in a comment:
  //
  // Display display; // the display
  //
  // ;-) ;-)

  //no-op
}
