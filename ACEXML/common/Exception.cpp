// -*- C++ -*- $Id$

#include "common/Exception.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

#if !defined (__ACEXML_INLINE__)
# include "common/Exception.i"
#endif /* __ACEXML_INLINE__ */

static const ACEXML_Char ACEXML_Exception_name[] = {
  'A', 'C', 'E', 'X', 'M', 'L',
  '_', 'E', 'x', 'c', 'e', 'p',
  't', 'i', 'o', 'n', 0};
const ACEXML_Char *ACEXML_Exception::exception_name_ = ACEXML_Exception_name;

static const ACEXML_Char ACEXML_Exception_null [] = {0};
const ACEXML_Char *ACEXML_Exception::null_ = ACEXML_Exception_null;

ACEXML_Exception::ACEXML_Exception (void)
{
}

ACEXML_Exception::ACEXML_Exception (const ACEXML_Exception &)
{
}


ACEXML_Exception::~ACEXML_Exception (void)
{
}

const ACEXML_Char *
ACEXML_Exception::id (void)
{
  return ACEXML_Exception::exception_name_;
}

// @@ An example of how duplicate should be implemented.

//  ACEXML_Exception *
//  ACEXML_Exception::duplicate (void)
//  {
//    ACEXML_Exception *tmp;
//    ACE_NEW_RETURN (tmp,
//                    ACEXML_Exception (*this),
//                    // Replace ACEXML_Exception with appropriate type.
//                    0);
//  }

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
  // @@ Nanbor, I don't know how to handle the case
  //    when we define ACEXML_UTF16 as ACEXML_Char
  ACE_DEBUG ((LM_DEBUG,
              "Exception: ACEXML_Exception\n"));
}
