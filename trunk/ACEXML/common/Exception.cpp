// -*- C++ -*- $Id$

#include "ACEXML/common/Exception.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

static const ACEXML_Char ACEXML_Exception_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L',
  '_', 'E', 'x', 'c', 'e', 'p',
  't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_Exception::exception_name_ = ACEXML_Exception_name;

static const ACEXML_Char ACEXML_Exception_null [] = {0};
const ACEXML_Char *ACEXML_Exception::null_ = ACEXML_Exception_null;

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/Exception.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Exception::ACEXML_Exception (void)
{
}

ACEXML_Exception::ACEXML_Exception (const ACEXML_Exception &)
{
}


ACEXML_Exception::~ACEXML_Exception (void)
{

}

int
ACEXML_Exception::is_a (const ACEXML_Char *name)
{
  if (name == ACEXML_Exception::exception_name_
      || ACE_OS::strcmp (ACEXML_Exception::exception_name_,
                         name) == 0)
    return 1;

  return 0;
}

void
ACEXML_Exception::print (void)
{
  ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) EXCEPTION : %s\n"),
                        this->exception_name_));
}
