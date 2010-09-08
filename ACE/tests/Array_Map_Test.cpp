//=============================================================================
/**
 * @file    Array_Map_Test.cpp
 *
 * $Id$
 *
 * Regression test for ACE_Array_Map.
 *
 * @author Ossama Othman
 */
//=============================================================================


#include "test_config.h"

#include "ace/SString.h"
#include "ace/Array_Map.h"

#include <algorithm>
//#include <map>  /* For STL portability testing. */

static char const letters[] =
  { 'A', 'C', 'E', ' ', 'r', 'u', 'l', 'e', 'z', '!' };

static ACE_TString const words[] =
  {
    ACE_TEXT ("alpha"),    // A
    ACE_TEXT ("charlie"),  // C
    ACE_TEXT ("echo"),     // E
    ACE_TEXT (" "),        //
    ACE_TEXT ("romeo"),    // r
    ACE_TEXT ("uniform"),  // u
    ACE_TEXT ("lima"),     // l
    ACE_TEXT ("echo"),     // e
    ACE_TEXT ("zulu"),     // z
    ACE_TEXT ("!")         // !
  };

static size_t const letters_len = sizeof (letters) / sizeof (letters[0]);
static size_t const words_len   = sizeof (words)   / sizeof (words[0]);

// --------------------------------------------------------------

bool
insertion_removal_test (void)
{
  // Instantiate the map.
  typedef ACE_Array_Map<char, ACE_TString> Map;
  Map phonetic[2];

  ACE_ASSERT (phonetic[0] == phonetic[1]);  // Sanity check.

  static size_t const phonetic_len =
    sizeof (phonetic) / sizeof (phonetic[0]);

  unsigned int count = 1;

  // Test map insertion.
  for (Map * m = phonetic;
       m != phonetic + phonetic_len;
       ++m, ++count)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("------------- Populating map %u -------------\n"),
                  count));


      ACE_TString const * word = words;
      for (char const * i = letters;
           i != letters + letters_len;
           ++i, ++word)
        {
          std::pair<Map::iterator, bool> const result =
            m->insert (std::make_pair (*i, *word));

          if (result.second)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Inserted \"%c | %s\"\n"),
                        *i,
                        word->c_str ()));
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Insertion of \"%c | %s\" failed.\n"),
                               *i,
                               word->c_str ()),
                              false);
        }

      ACE_ASSERT (m->size () == letters_len);
    }

  // Test equality of identically populated maps.
  if (!(phonetic[0] == phonetic[1]))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Equality comparison of two identical ")
                         ACE_TEXT ("maps failed.\n")),
                        false);
    }
  else if (phonetic[0] < phonetic[1])
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Relational comparison of two identical ")
                         ACE_TEXT ("maps incorrectly passed.\n")),
                        false);
    }

  ACE_ASSERT (!phonetic[0].is_empty ()); // Sanity check.

  Map foo (phonetic[0]);  // Copy construction
  Map bar = foo;          // Assignment

  if (!(foo == bar))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Second equality comparison of two ")
                         ACE_TEXT ("identical maps failed.\n")),
                        false);
    }

  Map::value_type values[letters_len];
  size_t const values_len = sizeof (values) / sizeof (values[0]);

  ACE_TString const * word = words;
  char const * letter = letters;

  for (Map::value_type * v = values;
       v != values + values_len;
       ++v, ++letter, ++word)
    {
      *v = std::make_pair (*letter, *word);
    }

  Map A (values, values + values_len);

  if (!(A == bar))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Third equality comparison of two ")
                         ACE_TEXT ("identical maps failed.\n")),
                        false);
    }

  Map B;
  B.insert (values, values + values_len);

  if (!(A == B))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Fourth equality comparison of two ")
                         ACE_TEXT ("identical maps failed.\n")),
                        false);
    }

  // ==== Removal tests ====

  // Remove two elements from map.
  Map::iterator letter_A = A.find ('A');
  ACE_ASSERT (letter_A == A.begin ());  // Should be first map element.
  ACE_ASSERT (A.count ('A') == 1);      // Should only be one letter 'A'.

  A.erase (letter_A);
  if (A.find ('A') != A.end ()
      || A.count ('A') != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Letter '%c' not removed from map\n"),
                         (*letter_A).first),
                        false);
    }

  static char const z = 'z';
  if (A.erase (z) != 1
      || A.count (z) != 0
      || A.find (z) != A.end ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Letter '%c' not removed from map\n"),
                         z),
                        false);
    }


  // Remove range of letters from map.
  static size_t const removed_len = 3;
  Map::iterator const first = B.begin () + 2;
  Map::iterator const last  = first + removed_len;

  ACE_ASSERT (static_cast<Map::size_type> (last - first) < B.size ());
  ACE_ASSERT (last < B.end ());

  Map::value_type removed[removed_len];
  size_t rcount = 0;
  for (Map::iterator x = first; x != last; ++x)
    {
      removed[rcount++] = *x;
    }

  B.erase (first, last);

  for (size_t s = 0; s < removed_len ; ++s)
    {
      Map::key_type const key = removed[s].first;

      if (B.count (key) != 0
          || B.find (key) != B.end ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Letter '%c' from range of letters ")
                             ACE_TEXT ("not removed from map\n"),
                             key),
                            false);
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Insertion/removal test passed.\n")));

  return true;
}

// --------------------------------------------------------------

bool
index_operator_test (void)
{
  // Instantiate the map.
  typedef ACE_Array_Map<char, ACE_TString> Map;
  Map phonetic;

  ACE_ASSERT (phonetic.size () == 0 && phonetic.is_empty ());
  ACE_ASSERT (phonetic.max_size () > 1);

  // Run the same test twice, clearing the contents of the map between
  // the iterations.  The goal is to verify that the constant time
  // clear() method performs as advertised.
  for (unsigned int count = 1; count < 3; ++count)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("----- Index operator test ")
                  ACE_TEXT ("iteration %u -----\n"),
                  count));

      // Test map insertion through the index operator.
      ACE_TString const * word = words;
      for (char const * i = letters;
           i != letters + letters_len;
           ++i, ++word)
        {
          phonetic[*i] = *word;
        }

      ACE_ASSERT (phonetic.size () == letters_len);

      typedef Map::const_iterator         const_iterator;

      // Access the elements that were inserted into the map.
      char const * letter = letters;
      word = words;
      const_iterator const last = phonetic.end ();
      for (const_iterator n = phonetic.begin ();
           n != last;
           ++n, ++letter, ++word)
        {
          if ((*n).first  != *letter || (*n).second != *word)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Key/Datum mismatch:\n")
                               ACE_TEXT ("    key \"%c\" should be \"%c\"\n")
                               ACE_TEXT ("  datum \"%s\" should be \"%s\"\n"),
                               (*n).first,
                               *letter,
                               (*n).second.c_str (),
                               word->c_str ()),
                              false);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\t%c\t%s\n"),
                      (*n).first,
                      (*n).second.c_str ()));
        }

      // Now run the same test in reverse.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("... in reverse ...\n")));

      typedef Map::const_reverse_iterator const_reverse_iterator;

      letter = letters + letters_len - 1;
      word = words + words_len - 1;

      // Work around compiler / STL implementations that cannot
      // handle implicit conversions from iterator to const_iterator
      // (e.g. due to missing template constructor.)
      //
      // We don't strictly need a const Map for this test but having
      // one allows us to exercise const iterators.
      Map const & const_phonetic = phonetic;

      const_reverse_iterator const rlast = const_phonetic.rend ();
      for (const_reverse_iterator r = const_phonetic.rbegin ();
           !(r == rlast); // Sun C++ Forte doesn't support operator!=
           ++r, --letter, --word)
        {
          if ((*r).first != *letter || (*r).second != *word)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Key/Datum mismatch:\n")
                               ACE_TEXT ("    key \"%c\" should be \"%c\"\n")
                               ACE_TEXT ("  datum \"%s\" should be \"%s\"\n"),
                               (*r).first,
                               *letter,
                               (*r).second.c_str (),
                               word->c_str ()),
                              false);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\t%c\t%s\n"),
                      (*r).first,
                      (*r).second.c_str ()));
        }

      // The size should not have changed.
      ACE_ASSERT (phonetic.size () == letters_len);

      // Empty the map of its contents wholesale.
      phonetic.clear ();

      ACE_ASSERT (phonetic.size () == 0 && phonetic.is_empty ());
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Index operator test passed.\n")));

  return true;
}

// --------------------------------------------------------------

class RefCounted
{
public:

  RefCounted (void)
    : refcount_ (0)
  {
  }

  RefCounted (unsigned int * count)
    : refcount_ (count)
  {
  }

  ~RefCounted (void)
  {
    if (this->refcount_)
      --(*this->refcount_);
  }

  RefCounted (RefCounted const & r)
    : refcount_ (r.refcount_ptr ())
  {
    if (this->refcount_)
      ++(*this->refcount_);
  }

  RefCounted &
  operator= (RefCounted const & r)
  {
    RefCounted tmp (r);
    std::swap (this->refcount_, tmp.refcount_);

    return *this;
  }

  unsigned int *
  refcount_ptr (void) const
  {
    return this->refcount_;
  }

  unsigned int
  refcount (void) const
  {
    return *this->refcount_;
  }

private:

  unsigned int * refcount_;

};

// --------

bool
reference_count_test (void)
{
  typedef ACE_Array_Map<ACE_TString, RefCounted> Map;

  static Map::size_type const CAPACITY = 30;

  unsigned int ref_count = 1;

  RefCounted counted (&ref_count);

  ACE_ASSERT (counted.refcount () == 1);

  {
    Map map (CAPACITY);  // Preallocate storage for a number of
                         // elements even if they are not used to test
                         // some internals.

    map[ACE_TEXT("One")] = counted;

    ACE_ASSERT (counted.refcount () == 2);


    std::pair<Map::iterator, bool> result;

    {
      // Enter a new scope block to assure destruction of temporaries
      // on systems like Solaris / Sun C++.

      result = map.insert (std::make_pair (ACE_TString (ACE_TEXT ("Two")),
                                           counted));

      ACE_ASSERT (result.second);
    }

    ACE_ASSERT (counted.refcount () == 3);

    {
      // Enter a new scope block to assure destruction of temporaries
      // on systems like Solaris / Sun C++.

      result = map.insert (std::make_pair (ACE_TString (ACE_TEXT ("Three")),
                                           counted));

      ACE_ASSERT (result.second);
    }


    ACE_ASSERT (counted.refcount () == 4);

    Map::size_type const erased = map.erase (ACE_TEXT ("One"));

    ACE_ASSERT (erased == 1);
    ACE_ASSERT (counted.refcount () == 3);
  }

  // Map instance no longer contains any references to the "counted"
  // object so the reference count should be back to one.

  ACE_ASSERT (counted.refcount () == 1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Reference count test passed.\n")));

  return true;
}

// --------------------------------------------------------------

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Array_Map_Test"));

  ACE_ASSERT (::letters_len == ::words_len);

  bool const success =
    ::insertion_removal_test ()
    && ::index_operator_test ()
    && ::reference_count_test ();

  ACE_END_TEST;

  return (success ? 0 : -1);
}

