// -*- C++ -*- $Id$

ACEXML_INLINE const ACEXML_Char *
ACEXML_Exception::id (void) const
{
  return ACEXML_Exception::exception_name_;
}

ACEXML_INLINE ACEXML_Exception*
ACEXML_Exception::_downcast (ACEXML_Exception* x)
{
  return x;
}
