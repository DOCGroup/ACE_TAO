// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Vector_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of the ACE_Vector container in ACE.
//
// = AUTHOR
//    Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#include "tests/test_config.h"

#include "ace/SString.h"
#include "ace/Vector.h"

ACE_RCSID(tests, Vector_Test, "$Id$")


int
main (int, char*[])
{
  ACE_START_TEST (ASYS_TEXT ("Vector_Test"));
  // ********************************************

  ACE_CString foo ("foo");

  const size_t init_size = 5;

  ACE_Vector<ACE_CString> sv (init_size, foo);


  ACE_DEBUG ((LM_INFO, "Initial size = %d\n", init_size));

  // ********************************************
  // Test proper memory allocation
  // ********************************************
  if (sv.size () != init_size)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector size not initialized properly.\n"),
                        -1);
    }
  else
    ACE_DEBUG ((LM_INFO, "ACE_Vector properly set the initial size.\n"));

  // ********************************************
  // Test proper default value initialization
  // ********************************************
  for (size_t i = 0; i < sv.size (); ++i)
    {
      if (sv[i] != foo)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ACE_Vector default values not initialized "
                             "properly.\n"),
                            -1);
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      "ACE_Vector properly set the initial "
                      "default values.\n"));
        }
    }

  // ********************************************
  // Test that the size () method is working
  // ********************************************
  if (init_size != sv.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::size() not functioning properly.\n"
                         "Size = %d.  It should be %d.\n",
                         sv.size (),
                         init_size),
                        -1);
    }
  else
    ACE_DEBUG ((LM_INFO, "ACE_Vector::size(): %d\n", sv.size ()));

  // ********************************************
  // Test that the capacity () method is working
  // ********************************************
  if (sv.capacity () != sv.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::capacity() not functioning properly.\n"
                         "Capacity = %d.  It should be %d.\n",
                         sv.capacity (),
                         sv.size ()),
                        -1);

    }
  else
    ACE_DEBUG ((LM_INFO, "ACE_Vector::capacity(): %d\n", sv.capacity ()));

  // ********************************************
  // Test that the max_size () method is working
  // ********************************************
  if (sv.max_size () < sv.size () || sv.max_size () < sv.capacity ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::max_size() not functioning properly.\n"),
                        -1);

    }
  else
    ACE_DEBUG ((LM_INFO, "ACE_Vector::max_size (): %d\n", sv.max_size ()));

  // ********************************************
  // Test the copy constructor
  // ********************************************
  ACE_Vector<ACE_CString> sv2 (sv);
  if (sv != sv2)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector copy constructor failed or equality "
                         "operator is broken.\n"),
                        -1);
    }
  else
    {
      ACE_DEBUG ((LM_INFO,
                  "ACE_Vector copy constructor is working properly.\n"));
    }

  // ********************************************
  // Test the iterators
  // ********************************************
  ACE_Vector<ACE_CString>::iterator first = sv.begin ();
  ACE_Vector<ACE_CString>::iterator last  = sv.end ();
  if (*first != sv[0] || *(last - 1) != sv[sv.size () - 1])
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector iterators failed.\n"),
                        -1);

    }
  else
    ACE_DEBUG ((LM_INFO, "ACE_Vector iterators are working properly.\n"));

  // ********************************************
  // Test the accessor methods
  // ********************************************
  // ** Test ACE_Vector::front()
  if (sv.front () != sv[0])
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::front() failed.\n"),
                        -1);
    }

  // ** Test ACE_Vector::back()
  if (sv.back () != sv[sv.size () - 1])
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::back() failed.\n"),
                        -1);
    }

  // ** Test ACE_Vector::push_back()
  for (size_t j = 0; j < 4 * init_size; ++j)
    {
      char num[10]; // 9 digits + null terminator
      num[1] = '\0';

      ACE_OS::sprintf (num, "%d", j);

      ACE_CString bar (num);

      sv.push_back (bar);
    }

  if (sv.size () != 5 * init_size || sv.capacity () < sv.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::push_back() failed to expand the "
                         "ACE_Vector properly.\n"),
                        -1);
    }

  for (size_t k = init_size; k < sv.size (); ++k)
    if (ACE_OS::atoi ((sv[k].c_str ())) !=
        ACE_static_cast (int, k - init_size))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_Vector failed to store pushed elements "
                           "properly.\n"),
                           -1);
      }

  ACE_DEBUG ((LM_INFO,
              "ACE_Vector::push_back() successfully stored new elements.\n"));

  // ** Test ACE_Vector::pop_back()
  size_t sv_old_size = sv.size ();
  sv.pop_back ();
  if (sv_old_size != sv.size() + 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::pop_back() failed to pop the last "
                         "element.\n"),
                        -1);
    }
  else
    {
      ACE_DEBUG ((LM_INFO, "ACE_Vector::pop_back() successfully "
                  "removed the last element.\n"));
    }

  // ********************************************
  // Test the utility methods
  // ********************************************

  ACE_Vector<ACE_CString> moo;

  sv_old_size = sv.size ();
  size_t sv_old_capacity = sv.capacity ();
  size_t moo_old_size = moo.size ();
  size_t moo_old_capacity = moo.capacity ();

  // ** Test ACE_Vector::swap()

  ACE_DEBUG ((LM_INFO, "ACE_Vector 1 size before swap = %d\n", sv_old_size));
  ACE_DEBUG ((LM_INFO, "ACE_Vector 2 size before swap = %d\n", moo_old_size));

  moo.swap (sv);

  if (sv_old_size != moo.size () ||
      moo_old_size != sv.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::swap() failed to properly swap "
                         "ACE_Vector sizes. (test 1)\n"),
                        -1);

    }
  else
    {
      ACE_DEBUG ((LM_INFO, "ACE_Vector 1 size after swap = %d\n",
                  sv.size ()));
      ACE_DEBUG ((LM_INFO, "ACE_Vector 2 size after swap = %d\n",
                  moo.size ()));
    }

  for (size_t m = 0; m < init_size; ++m)
    if (moo[m] != foo)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_Vector::swap() failed to properly swap "
                           "contents of ACE_Vector. (test 2)\n"),
                          -1);
      }

  for (size_t n = init_size; n < moo.size (); ++n)
    if (ACE_OS::atoi ((sv[n].c_str ())) !=
        ACE_static_cast (int, n - init_size))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_Vector::swap() failed to properly swap "
                           "contents of ACE_Vector. (test 3)\n"),
                          -1);
      }

  ACE_DEBUG ((LM_INFO, "ACE_Vector 1 capacity before swap = %d\n",
              sv_old_capacity));
  ACE_DEBUG ((LM_INFO, "ACE_Vector 2 capacity before swap = %d\n",
              moo_old_capacity));

  if (sv.capacity () < moo_old_size || moo.capacity () < sv_old_size)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::swap() failed to allocate memory "
                         "correctly.\n"),
                        -1);
    }

  ACE_DEBUG ((LM_INFO, "ACE_Vector 1 capacity after swap = %d\n",
              sv.capacity ()));
  ACE_DEBUG ((LM_INFO, "ACE_Vector 2 capacity after swap = %d\n",
              moo.capacity ()));

  ACE_DEBUG ((LM_INFO,
              "ACE_Vector::reserve() properly increased ACE_Vector\n"
              "        capacities during the swap when necessary.\n"));

  ACE_DEBUG ((LM_INFO,
              "ACE_Vector::swap() properly swapped the contents of "
              "two vectors.\n"));

  // ** Test ACE_Vector::reserve()
  moo_old_capacity = moo.capacity ();
  moo_old_size = moo.size ();
  if (moo.reserve (0) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::reserve() failed unexpectedly.\n"),
                        -1);

    }

  if (moo_old_capacity != moo.capacity () || moo_old_size != moo.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::reserve() reduced the size of the "
                         "ACE_Vector.\n"
                         "This should never happen\n"),
                        -1);
    }
  else
    {
      ACE_DEBUG ((LM_INFO,
                  "ACE_Vector::reserve() did not reduce the capacity of\n"
                  "        the ACE_Vector.  This is the correct behavior.\n"));
    }

  ACE_DEBUG ((LM_INFO,
              "PRE-ACE_Vector::reserve() size     = %d\n",
              sv.size ()));
  ACE_DEBUG ((LM_INFO,
              "PRE-ACE_Vector::reserve() capacity = %d\n",
              sv.capacity ()));

  sv_old_size = sv.size ();
  sv_old_capacity = sv.capacity ();
  if (sv.reserve (sv.capacity () + init_size) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::reserve() was unable to allocate "
                         "memory.\n"),
                        -1);
    }

  if (sv_old_capacity >= sv.capacity ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::reserve() did not properly increase \n"
                         "         the capacity of the ACE_Vector.\n"),
                        -1);
    }

  ACE_DEBUG ((LM_INFO,
              "POST-ACE_Vector::reserve() size     = %d\n",
              sv.size ()));
  ACE_DEBUG ((LM_INFO,
              "POST-ACE_Vector::reserve() capacity = %d\n",
              sv.capacity ()));

  ACE_DEBUG ((LM_INFO, "ACE_Vector::reserve() correctly increased the\n"
              "        capacity of the ACE_Vector.\n"));
              

  if (sv_old_size != sv.size ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector::reserve() changed the size of vector.\n"
                         "This should never happen.\n"),
                        -1);
    }


  // ********************************************
  // Test assignment operator
  // ********************************************
  ACE_DEBUG ((LM_INFO,
              "ACE_Vector size before assignment     = %d\n",
              sv.size ()));
  ACE_DEBUG ((LM_INFO,
              "ACE_Vector capacity before assignment = %d\n",
              sv.capacity ()));

  sv = moo;

  if (sv != moo)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_Vector assignment operator did not work.\n"),
                        -1);
    }

  ACE_DEBUG ((LM_INFO,
              "ACE_Vector size after assignment     = %d\n",
              sv.size ()));
  ACE_DEBUG ((LM_INFO,
              "ACE_Vector capacity after assignment = %d\n",
              sv.capacity ()));

  ACE_DEBUG ((LM_INFO,
              "ACE_Vector assignment operator is working correctly.\n"));

  // ********************************************
  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Vector<ACE_CString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Vector<ACE_CString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
