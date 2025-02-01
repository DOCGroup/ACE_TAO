// -*- C++ -*-

ACEXML_INLINE void
ACEXML_SAXPrint_Handler::inc_indent ()
{
  this->indent_ += 1;
}

ACEXML_INLINE void
ACEXML_SAXPrint_Handler::dec_indent ()
{
  this->indent_ -= 1;
}
