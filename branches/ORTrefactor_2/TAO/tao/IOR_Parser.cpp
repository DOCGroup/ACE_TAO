// $Id$

#include "tao/IOR_Parser.h"
#include "ace/Dynamic_Service.h"

#if !defined(__ACE_INLINE__)
#include "tao/IOR_Parser.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, IOR_Parser, "$Id$")

TAO_IOR_Parser::~TAO_IOR_Parser (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_IOR_Parser>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_IOR_Parser>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
