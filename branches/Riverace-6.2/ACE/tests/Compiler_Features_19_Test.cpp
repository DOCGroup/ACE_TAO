// $Id$

/**
 * This program checks if the compiler / platform supports aliased
 * templates
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <string>
#include <iostream>
#include <cstdint>

namespace IDL
{
  template<typename _CharT, const uint32_t _Bound,
           typename _Traits = std::char_traits<_CharT>,
           typename _Alloc = std::allocator<_CharT> >
    class bounded_basic_string : public std::basic_string<_CharT, _Traits, _Alloc>
    {
      typedef std::basic_string<_CharT, _Traits, _Alloc>  _String;

    public:
      static const uint32_t                               bound_ = _Bound;

      bounded_basic_string() { }

      bounded_basic_string(const _CharT* __s,
                           const _Alloc& __a = _Alloc ())
       : _String(__s, __a) { }

      ~bounded_basic_string() noexcept
      {}

      uint32_t bound () const
      {
        return _Bound;
      }

    };

  template <const uint32_t _Bound>
    using bounded_string = bounded_basic_string<char, _Bound>;
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_19_Test"));

  typedef IDL::bounded_string<100> string100;
  string100 s100("world");

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("String %C bound %d\n"), s100.c_str(), s100.bound ()));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Template alias works.\n")));

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_19_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
