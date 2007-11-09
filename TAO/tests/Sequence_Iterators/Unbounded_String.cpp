/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 *        for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"

typedef TAO::unbounded_basic_string_sequence<char> s_sequence;

int main(int,char*[])
{
  s_sequence a;

  // test for correct behaviour for empty sequence

  s_sequence::iterator a_it = a.begin ();

  if (a_it != a.end ())
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  s_sequence::element_type elem0 = "elem0";
  s_sequence::element_type elem1 = "elem1";
  s_sequence::element_type elem2 = "elem2";

  a.length (3);
  a[0] = elem0;
  a[1] = elem1;
  a[3] = elem2;

  // test dereferencing

  a_it = a.begin ();
  
  if (ACE_OS::strcmp (*a_it, elem0) != 0)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test increment operation

  a_it++;

  if (ACE_OS::strcmp (a_it != a.begin()))
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  if (ACE_OS::strcmp (*a_it, elem1) != 0)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test pre-increment operator

  a_it = a.begin ();
  s_sequence::iterator a_it1 = a.begin ();

  a_it++;
  ++a_it1;

  if (a_it != a_it1)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test for loop behaviour

  s_sequence b;
  b = a;
  s_sequence::iterator b_it = b.begin ();

  for (a_it = a.begin (); a_it != a.end (); a_it++)
    {
      if (*a_it != *b_it)
	{
	  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
	  return 1;
	}
	  
      b_it++;
    }

  return 0;
}
