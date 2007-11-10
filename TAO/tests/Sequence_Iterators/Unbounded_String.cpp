/**
 * @file Unbounded_String.cpp
 *
 * @brief test for STL iterator behaviour of CORBA unbounded string sequence
 *
 * $Id$
 *
 * @author Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
#include "tao/StringSeqC.h"
#include "ace/Log_Msg.h"

int main(int,char*[])
{
  ::CORBA::StringSeq a;

  // test for correct behaviour for empty sequence

  ::CORBA::StringSeq::iterator a_it = a.begin ();

  if (a_it != a.end ())
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  ::CORBA::StringSeq::value_type elem0 = CORBA::string_dup ("elem0");
  ::CORBA::StringSeq::value_type elem1 = CORBA::string_dup ("elem1");
  ::CORBA::StringSeq::value_type elem2 = CORBA::string_dup ("elem2");

  a.length (3);
  a[0] = elem0;
  a[1] = elem1;
  a[3] = elem2;

  // test dereferencing

  ::CORBA::StringSeq::iterator a_it2 = a.begin ();
  
  if (ACE_OS::strcmp (*a_it2, elem0) != 0)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test increment operation

  a_it2++;

  if (a_it2 == a.begin())
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  if (ACE_OS::strcmp (*a_it2, elem1) != 0)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test pre-increment operator

  ::CORBA::StringSeq::iterator a_it3 = a.begin ();
  ::CORBA::StringSeq::iterator a_it31 = a.begin ();

  a_it3++;
  ++a_it31;

  if (a_it3 != a_it31)
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  ::CORBA::StringSeq::iterator b_it = b.begin ();

  for (::CORBA::StringSeq::iterator a_it4 = a.begin ();
       a_it4 != a.end ();
       a_it4++, b_it++)
    {
      if (ACE_OS::strcmp (*a_it4, *b_it) != 0)
	{
	  ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
	  return 1;
	}
    }

  return 0;
}
