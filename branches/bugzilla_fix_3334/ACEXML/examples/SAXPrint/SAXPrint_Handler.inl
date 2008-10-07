// -*- C++ -*-  $Id$

ACEXML_INLINE void
ACEXML_SAXPrint_Handler::inc_indent (void)
{
  this->indent_ += 1;
}

ACEXML_INLINE void
ACEXML_SAXPrint_Handler::dec_indent (void)
{
  this->indent_ -= 1;
}
