// Tests the generated perfect hash function.

// The -v option prints diagnostics as to whether a word is in the set
// or not.  Without -v the program is useful for timing.

#include "test_driver.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

#include <cstddef>

static constexpr int MAX_LEN = 80;

class Perfect_Hash
{
private:
  static size_t hash (char const *str, size_t len);
public:
  static char const *in_word_set (char const *str, size_t len);
};

int
gperf_test_main (int argc, ACE_TCHAR *argv[], Gperf_Test_Mode const test_mode)
{
  int verbose = argc > 1 && ACE_OS::strcmp (argv[1], ACE_TEXT ("-v")) == 0 ? 1 : 0;
  char buf[MAX_LEN];

  while (ACE_OS::fgets (buf, sizeof buf, stdin) != 0)
    {
      size_t len = ACE_OS::strlen (buf) - 1;
      buf[len] = '\0';
      if (Perfect_Hash::in_word_set (buf, len) && verbose)
        {
          ACE_OS::printf ("in word set %s\n", buf);
        }
      else if (verbose)
        {
          ACE_OS::printf ("NOT in word set %s\n", buf);
        }
    }

  if (test_mode == Gperf_Test_Mode::switch_prefix)
    {
      char const bounded_prefix[] = {'g', 'o', 't'};
      if (Perfect_Hash::in_word_set (bounded_prefix, 2) != nullptr)
        {
          return 1;
        }
    }

  if (test_mode == Gperf_Test_Mode::duplicate_prefix)
    {
      char const duplicate_prefix[] = {'a', 'l'};
      if (Perfect_Hash::in_word_set (duplicate_prefix, sizeof duplicate_prefix) != nullptr)
        {
          return 1;
        }
    }

  return 0;
}
