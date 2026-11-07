#include "Collocation_Tester.h"
#include <cstring>

Collocation_Tester::Collocation_Tester (CORBA::Object_ptr const object)
  : object_ (CORBA::Object::_duplicate (object))
{
}

int
Collocation_Tester::match_answer (char const *actual_answer,
                                  char const *right_answer,
                                  char const *method_name)
{
  if (std::strcmp (actual_answer, right_answer) != 0)
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
Collocation_Tester::test_top () const
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<TOP>-------------------------------\n")));
  int failure {};
  int const call_num {1};
  Multiple::Top_var const top =
    Multiple::Top::_narrow (this->object_.in ());

  if (CORBA::is_nil (top.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Top\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Top\n\n")));

  CORBA::String_var const msg = top->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure\n"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<TOP>-------------------------------\n")));

  return failure;
}

int
Collocation_Tester::test_right () const
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<RIGHT>-------------------------------\n")));
  int failure {};
  int const call_num {2};
  Multiple::Right_var const right =
    Multiple::Right::_narrow (this->object_.in ());

  if (CORBA::is_nil (right.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Right\n\n")));

  CORBA::String_var const msg = right->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  CORBA::String_var const right_msg = right->right_quote ();

  failure += this->match_answer (Quote::right, right_msg.in (), "right_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<RIGHT>-------------------------------\n")));

  return failure;
}


int
Collocation_Tester::test_left () const
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<LEFT>-------------------------------\n")));
  int failure {};
  int const call_num {2};

  Multiple::Left_var const left =
    Multiple::Left::_narrow (this->object_.in ());

  if (CORBA::is_nil (left.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Left\n\n")));

  CORBA::String_var const msg = left->top_quote ();

  failure += this->match_answer (Quote::top, msg.in (), "top_quote");

  CORBA::String_var const left_msg = left->left_quote ();

  failure += this->match_answer (Quote::left, left_msg.in (), "left_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n------------------------<LEFT>-------------------------------\n")));

  return failure;
}


int
Collocation_Tester::test_bottom () const
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n-----------------------<BOTTOM>-------------------------------\n")));
  int failure {};
  int const call_num {4};

  Multiple::Bottom_var const bottom =
    Multiple::Bottom::_narrow (this->object_.in ());

  if (CORBA::is_nil (bottom.in ()))
    {
      ACE_ERROR ((LM_DEBUG,
                  ACE_TEXT ("Unable to Narrow to Multiple::Right\n")));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\nCalling all method supported by the Interface Bottom\n\n")));

  CORBA::String_var const top_msg = bottom->top_quote ();

  failure += this->match_answer (Quote::top, top_msg.in (), "top_quote");

  CORBA::String_var const left_msg = bottom->left_quote ();

  failure += this->match_answer (Quote::left, left_msg.in (), "left_quote");

  CORBA::String_var const right_msg = bottom->right_quote ();

  failure += this->match_answer (Quote::right, right_msg.in (), "right_quote");

  CORBA::String_var const bottom_msg = bottom->bottom_quote ();

  failure += this->match_answer (Quote::bottom, bottom_msg.in (), "bottom_quote");

  if (failure)
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("The test had %d/%d Failure"), failure, call_num));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("\n-----------------------<BOTTOM>-------------------------------\n")));

  return failure;
}

void
Collocation_Tester::shutdown () const
{
  Multiple::Top_var const top =
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
Collocation_Tester::run () const
{
  int failure_num = this->test_top ();

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
