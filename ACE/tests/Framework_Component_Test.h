
//=============================================================================
/**
 *  @file    Framework_Component_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Don Hinton <dhinton@ieee.org>
 */
//=============================================================================


#ifndef ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H
#define ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H

#include "ace/Log_Msg.h"

/// This we will use to test the ACE_Framework_Repository
class Simple_Service
{
public:
  Simple_Service (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Service::Simple_Service\n")));
    }
  virtual ~Simple_Service (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Service::~Simple_Service\n")));
    }
};

#endif /* ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H */
