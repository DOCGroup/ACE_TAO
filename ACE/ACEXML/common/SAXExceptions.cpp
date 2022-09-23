// -*- C++ -*-

#include "ACEXML/common/SAXExceptions.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

const ACEXML_Char * ACEXML_SAXException::exception_name_ = ACE_TEXT ("ACEXML_SAXException");

const ACEXML_Char ACEXML_SAXNotSupportedException::exception_name_[] = ACE_TEXT ("ACEXML_SAXNotSupportedException");

const ACEXML_Char ACEXML_SAXNotRecognizedException::exception_name_[] = ACE_TEXT ("ACEXML_SAXNotRecognizedException");

const ACEXML_Char ACEXML_SAXParseException::exception_name_[] = ACE_TEXT ("ACEXML_SAXParseException");

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/SAXExceptions.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_SAXException::ACEXML_SAXException ()
  : message_ (0)
{
}

ACEXML_SAXException::ACEXML_SAXException (const ACEXML_Char *msg)
  : message_ (ACE::strnew (msg))
{
}

ACEXML_SAXException::ACEXML_SAXException (const ACEXML_SAXException &ex)
  : ACEXML_Exception (ex),
    message_ (ACE::strnew (ex.message_))

{
}

ACEXML_SAXException&
ACEXML_SAXException::operator= (const ACEXML_SAXException& src)
{
  if (this != &src)
    {
      ACE::strdelete (this->message_);
      this->message_ = ACE::strnew (src.message_);
    }

  return *this;
}

ACEXML_SAXException*
ACEXML_SAXException::_downcast (ACEXML_Exception* ex)
{
  return dynamic_cast<ACEXML_SAXException*> (ex);
}

ACEXML_SAXException::~ACEXML_SAXException ()
{
  ACE::strdelete (this->message_);
}

ACEXML_Exception *
ACEXML_SAXException::duplicate () const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXException (*this), 0);
  return tmp;
}

int
ACEXML_SAXException::is_a (const ACEXML_Char *name)
{
  return ACE_OS::strcmp (ACEXML_SAXException::exception_name_, name) == 0
    || this->ACEXML_Exception::is_a (name);
}

void
ACEXML_SAXException::print () const
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException ()
{
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException (const ACEXML_SAXNotSupportedException &ex)
  : ACEXML_SAXException (ex)
{
}

ACEXML_SAXNotSupportedException&
ACEXML_SAXNotSupportedException::operator= (const
                                            ACEXML_SAXNotSupportedException &ex)
{
  this->ACEXML_SAXException::operator= (ex);
  return *this;
}

ACEXML_SAXNotSupportedException*
ACEXML_SAXNotSupportedException::_downcast (ACEXML_Exception* ex)
{
  return dynamic_cast<ACEXML_SAXNotSupportedException*> (ex);
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException (const ACEXML_Char* msg)
  : ACEXML_SAXException (msg)
{
}

ACEXML_SAXNotSupportedException::~ACEXML_SAXNotSupportedException ()
{
}


ACEXML_Exception *
ACEXML_SAXNotSupportedException::duplicate () const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXNotSupportedException (*this), 0);
  return tmp;
}

int
ACEXML_SAXNotSupportedException::is_a (const ACEXML_Char *name)
{
  return ACE_OS::strcmp (ACEXML_SAXNotSupportedException::exception_name_,
                         name) == 0
    || this->ACEXML_SAXException::is_a (name);
}

void
ACEXML_SAXNotSupportedException::print () const
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXNotRecognizedException::ACEXML_SAXNotRecognizedException ()
{
}

ACEXML_SAXNotRecognizedException::ACEXML_SAXNotRecognizedException (const ACEXML_Char *msg)
  : ACEXML_SAXException (msg)
{
}

ACEXML_SAXNotRecognizedException::ACEXML_SAXNotRecognizedException (const ACEXML_SAXNotRecognizedException &ex)
  : ACEXML_SAXException (ex)
{
}

ACEXML_SAXNotRecognizedException&
ACEXML_SAXNotRecognizedException::operator= (const ACEXML_SAXNotRecognizedException &ex)
{
  this->ACEXML_SAXException::operator= (ex);
  return *this;
}

ACEXML_SAXNotRecognizedException*
ACEXML_SAXNotRecognizedException::_downcast (ACEXML_Exception* ex)
{
  return dynamic_cast<ACEXML_SAXNotRecognizedException*> (ex);
}

ACEXML_SAXNotRecognizedException::~ACEXML_SAXNotRecognizedException ()
{
}

ACEXML_Exception *
ACEXML_SAXNotRecognizedException::duplicate () const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXNotRecognizedException (*this), 0);
  return tmp;
}

int
ACEXML_SAXNotRecognizedException::is_a (const ACEXML_Char *name)
{
  return ACE_OS::strcmp (ACEXML_SAXNotRecognizedException::exception_name_,
                         name) == 0
    || this->ACEXML_SAXException::is_a (name);
}

void
ACEXML_SAXNotRecognizedException::print () const
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXParseException::ACEXML_SAXParseException ()
{
}

ACEXML_SAXParseException::ACEXML_SAXParseException (const ACEXML_Char *msg)
  : ACEXML_SAXException (msg)
{
}

ACEXML_SAXParseException::ACEXML_SAXParseException (const ACEXML_SAXParseException &ex)
  : ACEXML_SAXException (ex)
{
}

ACEXML_SAXParseException&
ACEXML_SAXParseException::operator= (const ACEXML_SAXParseException &ex)
{
  this->ACEXML_SAXException::operator= (ex);
  return *this;
}

ACEXML_SAXParseException*
ACEXML_SAXParseException::_downcast (ACEXML_Exception* ex)
{
  return dynamic_cast<ACEXML_SAXParseException*> (ex);
}

ACEXML_SAXParseException::~ACEXML_SAXParseException ()
{
}

ACEXML_Exception *
ACEXML_SAXParseException::duplicate () const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXParseException (*this), 0);
  return tmp;
}

int
ACEXML_SAXParseException::is_a (const ACEXML_Char *name)
{
  return ACE_OS::strcmp (ACEXML_SAXParseException::exception_name_,
                         name) == 0
    || this->ACEXML_SAXException::is_a (name);
}

void
ACEXML_SAXParseException::print () const
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
                        this->exception_name_, this->message()));
}
