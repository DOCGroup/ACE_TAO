// -*- C++ -*- $Id$

#include "ACEXML/common/SAXExceptions.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

static const ACEXML_Char ACEXML_SAXException_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L', '_',
  'S', 'A', 'X',
  'E', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_SAXException::exception_name_ = ACEXML_SAXException_name;

static const ACEXML_Char ACEXML_SAXNotSupportedException_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L', '_',
  'S', 'A', 'X',
  'N', 'o', 't',
  'S', 'u', 'p', 'p', 'o', 'r', 't', 'e', 'd',
  'E', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_SAXNotSupportedException::exception_name_ = ACEXML_SAXNotSupportedException_name;

static const ACEXML_Char ACEXML_SAXNotRecognizedException_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L', '_',
  'S', 'A', 'X',
  'N', 'o', 't',
  'R', 'e', 'c', 'o', 'g', 'n', 'i', 'z', 'e', 'd',
  'E', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_SAXNotRecognizedException::exception_name_ = ACEXML_SAXNotRecognizedException_name;

static const ACEXML_Char ACEXML_SAXParseException_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L', '_',
  'S', 'A', 'X',
  'P', 'a', 'r', 's', 'e',
  'E', 'x', 'c', 'e', 'p', 't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_SAXParseException::exception_name_ = ACEXML_SAXParseException_name;

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/SAXExceptions.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_SAXException::ACEXML_SAXException (void)
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
  ACEXML_SAXException temp (src);
  ACEXML_Char* message = this->message_;
  this->exception_name_ = temp.exception_name_;
  this->message_ = temp.message_;
  temp.message_ = message;
  return *this;
}


ACEXML_SAXException*
ACEXML_SAXException::_downcast (ACEXML_Exception* ex)
{
  if (ex->is_a (ACE_TEXT ("ACEXML_SAXException")))
    return ACE_dynamic_cast (ACEXML_SAXException*, ex);
  return 0;
}

ACEXML_SAXException::~ACEXML_SAXException (void)
{
  delete[] this->message_;
}



ACEXML_Exception *
ACEXML_SAXException::duplicate (void) const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXException (*this), 0);
  return tmp;
}

int
ACEXML_SAXException::is_a (const ACEXML_Char *name)
{
  if (name == ACEXML_SAXException::exception_name_
      || ACE_OS::strcmp (ACEXML_Exception::exception_name_,
                         name) == 0)
    return 1;
  else
    return this->ACEXML_Exception::is_a (name);

  ACE_NOTREACHED (return 0;)
}

void
ACEXML_SAXException::print (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException (void)
{
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException (const ACEXML_SAXNotSupportedException &ex)
  : ACEXML_SAXException (ex)
{
}

ACEXML_SAXNotSupportedException::ACEXML_SAXNotSupportedException (const ACEXML_Char* msg)
  : ACEXML_SAXException (msg)
{
}

ACEXML_SAXNotSupportedException::~ACEXML_SAXNotSupportedException (void)
{
  delete[] this->message_;
}


ACEXML_Exception *
ACEXML_SAXNotSupportedException::duplicate (void) const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXNotSupportedException (*this), 0);
  return tmp;
}

int
ACEXML_SAXNotSupportedException::is_a (const ACEXML_Char *name)
{
  if (name == ACEXML_SAXNotSupportedException::exception_name_
      || ACE_OS::strcmp (ACEXML_Exception::exception_name_,
                         name) == 0)
    return 1;
  else
    return this->ACEXML_SAXException::is_a (name);

  ACE_NOTREACHED (return 0;)
}

void
ACEXML_SAXNotSupportedException::print (void)
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXNotRecognizedException::ACEXML_SAXNotRecognizedException (void)
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

ACEXML_SAXNotRecognizedException::~ACEXML_SAXNotRecognizedException (void)
{
  delete[] this->message_;
}

ACEXML_Exception *
ACEXML_SAXNotRecognizedException::duplicate (void) const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXNotRecognizedException (*this), 0);
  return tmp;
}

int
ACEXML_SAXNotRecognizedException::is_a (const ACEXML_Char *name)
{
  if (name == ACEXML_SAXNotRecognizedException::exception_name_
      || ACE_OS::strcmp (ACEXML_Exception::exception_name_,
                         name) == 0)
    return 1;
  else
    return this->ACEXML_SAXException::is_a (name);

  ACE_NOTREACHED (return 0;)
}

void
ACEXML_SAXNotRecognizedException::print (void)
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
              this->exception_name_, this->message()));
}

ACEXML_SAXParseException::ACEXML_SAXParseException (void)
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

ACEXML_SAXParseException::~ACEXML_SAXParseException (void)
{
}

ACEXML_Exception *
ACEXML_SAXParseException::duplicate (void) const
{
  ACEXML_Exception *tmp;
  ACE_NEW_RETURN (tmp, ACEXML_SAXParseException (*this), 0);
  return tmp;
}

int
ACEXML_SAXParseException::is_a (const ACEXML_Char *name)
{
  if (name == ACEXML_SAXParseException::exception_name_
      || ACE_OS::strcmp (ACEXML_Exception::exception_name_,
                         name) == 0)
    return 1;
  else
    return this->ACEXML_SAXException::is_a (name);

  ACE_NOTREACHED (return 0;)
}

void
ACEXML_SAXParseException::print (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) %s: %s\n"),
                        this->exception_name_, this->message()));
}
