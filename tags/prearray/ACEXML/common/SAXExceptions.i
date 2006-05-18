// -*- C++ -*- $Id$

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXException::message (void)
{
  return (this->message_ == 0 ?
          ACEXML_Exception::null_ :
          this->message_);
}

ACEXML_INLINE void
ACEXML_SAXException::_raise (void)
{
  ACEXML_RAISE (*this);
}

ACEXML_INLINE void
ACEXML_SAXNotSupportedException::_raise (void)
{
  ACEXML_RAISE (*this);
}

ACEXML_INLINE void
ACEXML_SAXNotRecognizedException::_raise (void)
{
  ACEXML_RAISE (*this);
}

ACEXML_INLINE void
ACEXML_SAXParseException::_raise (void)
{
  ACEXML_RAISE (*this);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXException::id (void) const
{
  return ACEXML_SAXException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXNotSupportedException::id (void) const
{
  return ACEXML_SAXNotSupportedException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXNotRecognizedException::id (void) const
{
  return ACEXML_SAXNotRecognizedException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXParseException::id (void) const
{
  return ACEXML_SAXParseException::exception_name_;
}
