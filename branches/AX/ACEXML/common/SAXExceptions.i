// -*- C++ -*- $Id$

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXException::message (void)
{
  return (this->message_ == 0 ?
          ACEXML_Exception::null_ :
          this->message_);
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXException::name (void)
{
  return ACEXML_SAXException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXNotSupportedException::name (void)
{
  return ACEXML_SAXNotSupportedException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXNotRecognizedException::name (void)
{
  return ACEXML_SAXNotRecognizedException::exception_name_;
}

ACEXML_INLINE const ACEXML_Char *
ACEXML_SAXParseException::name (void)
{
  return ACEXML_SAXParseException::exception_name_;
}
