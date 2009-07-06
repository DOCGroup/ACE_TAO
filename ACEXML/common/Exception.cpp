// -*- C++ -*- $Id$

#include "ACEXML/common/Exception.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

const ACEXML_Char *ACEXML_Exception::exception_name_ = ACE_TEXT ("ACEXML_Exception");

const ACEXML_Char *ACEXML_Exception::null_ = ACE_TEXT ("");

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/common/Exception.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_Exception::ACEXML_Exception()
{
}

ACEXML_Exception::~ACEXML_Exception()
{
}

int
ACEXML_Exception::is_a (const ACEXML_Char *name)
{
  return ACE_OS::strcmp (ACEXML_Exception::exception_name_, name) == 0;
}

void
ACEXML_Exception::print (void) const
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACEXML: (%P|%t) EXCEPTION : %s\n"),
                        this->exception_name_));
}
