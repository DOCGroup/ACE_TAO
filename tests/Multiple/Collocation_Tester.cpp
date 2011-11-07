// $Id$

#include "Collocation_Tester.h"
#include "ace/OS_NS_string.h"

const char *Quote::top = "Ciao";
const char *Quote::left = "Hola";
const char *Quote::right = "Hello";
const char *Quote::bottom = "Previet";

Collocation_Tester::Collocation_Tester (CORBA::Object_ptr object)
{
  this->object_ = CORBA::Object::_duplicate (object);
}

Collocation_Tester::~Collocation_Tester (void)
{
  // No-Op.
}

int
Collocation_Tester::match_answer (const char *actual_answer,
                                  const char *right_answer,
                                  const char *method_name)
{
  if (ACE_OS::strcmp (actual_answer, right_answer))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Call to <%C> returned the wrong value!\n"), method_name));
      return 1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Call to <%C> returned with success: %C\n"), method_name, actual_answer));
      return 0;
    }
}

int
Collocation_Tester::test_top (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<TOP>-------------------------------\n")));
  int failure = 0;
  int call_num = 1;
  Multiple::Top_var top =
    Multiple::Top::_narrow (this->object_.in ());

  if (CORBA::is_nil (top.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Top\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Top\n\n")));

  CORBA::String_var msg = top->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure\n"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<TOP>-------------------------------\n")));

  return failure;
}

int
Collocation_Tester::test_right (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<RIGHT>-------------------------------\n")));
  int failure = 0;
  int call_num = 2;
  Multiple::Right_var right =
    Multiple::Right::_narrow (this->object_.in ());

  if (CORBA::is_nil (right.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Right\n\n")));

  CORBA::String_var msg = right->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  msg = right->right_quote ();

  failure += this->match_answer (Quote::right, msg.in (), "right_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<RIGHT>-------------------------------\n")));

  return failure;
}


int
Collocation_Tester::test_left (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<LEFT>-------------------------------\n")));
  int failure = 0;
  int call_num = 2;

  Multiple::Left_var left =
    Multiple::Left::_narrow (this->object_.in ());

  if (CORBA::is_nil (left.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Left\n\n")));

  CORBA::String_var msg = left->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  msg = left->left_quote ();

  failure += this->match_answer (Quote::left, msg.in (), "left_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<LEFT>-------------------------------\n")));

  return failure;
}


int
Collocation_Tester::test_bottom (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n-----------------------<BOTTOM>-------------------------------\n")));
  int failure = 0;
  int call_num = 4;

  Multiple::Bottom_var bottom =
    Multiple::Bottom::_narrow (this->object_.in ());

  if (CORBA::is_nil (bottom.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Bottom\n\n")));

  CORBA::String_var msg = bottom->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  msg = bottom->left_quote ();

  failure += this->match_answer (Quote::left, msg.in (), "left_quote");

  msg = bottom->right_quote ();

  failure += this->match_answer (Quote::right, msg.in (), "right_quote");

  msg = bottom->bottom_quote ();

  failure += this->match_answer (Quote::bottom, msg.in (), "bottom_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n-----------------------<BOTTOM>-------------------------------\n")));

  return failure;
}

void
Collocation_Tester::shutdown (void)
{
  Multiple::Top_var top =
    Multiple::Top::_narrow (this->object_.in ());

  if (CORBA::is_nil (top.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Top\n")));
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling ShutDown on Top.\n\n")));

  top->shutdown ();
}

void
Collocation_Tester::run (void)
{
  int failure_num =   this->test_top ();

  failure_num += this->test_left ();

  failure_num += this->test_right ();

  failure_num += this->test_bottom ();

  if (failure_num)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("\n\nThe Test failed in %d cases."), failure_num));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\n\nThe Test Completed Successfully. Congratulations! ")));

  this->shutdown ();
}
