// $Id$

#include "ACEXML/parser/parser/ParserContext.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/ParserContext.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_Parser_Context::~ACEXML_Parser_Context()
{
  delete this->instream_;
  this->instream_ = 0;
  delete this->locator_;
  this->locator_ = 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Swap <ACEXML_InputSource*>; 
template class ACE_Swap <ACEXML_LocatorImpl*>; 
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate ACE_Swap <ACEXML_InputSource*>
#  pragma instantiate ACE_Swap <ACEXML_LocatorImpl*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

