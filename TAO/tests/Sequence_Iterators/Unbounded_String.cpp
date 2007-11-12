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

#include <iostream>
#include <iterator>

int main(int,char*[])
{
  // Questions:
  // Should assignment of iterators be allowed? g++ seems to allow this.
  //  I'm not sure if it makes sense.
  // What should happen to iterators when the containers are modified?

  ::CORBA::StringSeq a;

  // test for correct behaviour for empty sequence

  ::CORBA::StringSeq::iterator a_it = a.begin ();

  if (a_it != a.end ())
    {
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
      return 1;
    }

  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";
  ::CORBA::StringSeq::value_type elem0 = CORBA::string_dup (elem0_cstr);
  ::CORBA::StringSeq::value_type elem1 = CORBA::string_dup (elem1_cstr);
  ::CORBA::StringSeq::value_type elem2 = CORBA::string_dup (elem2_cstr);
  ::CORBA::StringSeq::value_type elem3 = CORBA::string_dup (elem3_cstr);

  a.length (4);
  a[0] = elem0;
  a[1] = elem1;
  a[2] = elem2;
  a[3] = elem3;


  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()
  std::copy (a.begin (),
             a.end (),
             test.begin ());

  ::CORBA::StringSeq::iterator copytest_iter = test.begin ();
  for (::CORBA::StringSeq::iterator copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      if (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Failed at %N:%l\n")));
          return 1;
        }
    }

  /// Testing - using ostream_iterator
  std::cout << "Output should be: " << std::endl
            << elem0_cstr << std::endl
            << elem1_cstr << std::endl
            << elem2_cstr << std::endl
            << elem3_cstr << std::endl
            << "End of expected output" << std::endl;

  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<CORBA::StringSeq::value_type> (std::cout,
                                                                  "\n"));
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
