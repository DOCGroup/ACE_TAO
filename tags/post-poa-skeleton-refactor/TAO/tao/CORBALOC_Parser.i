// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_CORBALOC_Parser::TAO_CORBALOC_Parser (void)
{
}

ACE_INLINE
TAO_CORBALOC_Parser::parsed_endpoint::parsed_endpoint(void)
 : profile_ (0)
{
}

ACE_INLINE
TAO_CORBALOC_Parser::parsed_endpoint::~parsed_endpoint (void)
{
  if (this->profile_ != 0)
    this->profile_->_decr_refcnt();
}
