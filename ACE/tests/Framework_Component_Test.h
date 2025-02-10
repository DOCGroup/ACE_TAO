
//=============================================================================
/**
 *  @file    Framework_Component_Test.h
 *
 *  Define class needed for generating templates.
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
  Simple_Service ()
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Service::Simple_Service\n")));
    }
  virtual ~Simple_Service ()
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Simple_Service::~Simple_Service\n")));
    }
};

#endif /* ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H */
