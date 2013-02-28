// $Id$

/**
 * This program checks if the compiler / platform supports aliased
 * templates
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <string>
#include <iostream>

namespace IDL
{
  template<typename _CharT, const uint32_t _Bound,
           typename _Traits = std::char_traits<_CharT>,
           typename _Alloc = std::allocator<_CharT> >
    class bounded_basic_string : public std::basic_string<_CharT, _Traits, _Alloc>
    {
      typedef std::basic_string<_CharT, _Traits, _Alloc>  _String;

    public:
      typedef _Traits                                     traits_type;
      typedef typename _Traits::char_type                 value_type;
      typedef _Alloc                                      allocator_type;
      typedef typename _String::size_type                 size_type;
      typedef typename _String::difference_type           difference_type;
      typedef typename _String::reference                 reference;
      typedef typename _String::const_reference           const_reference;
      typedef typename _String::pointer                   pointer;
      typedef typename _String::const_pointer             const_pointer;
      typedef typename _String::iterator                  iterator;
      typedef typename _String::const_iterator            const_iterator;
      typedef typename _String::const_reverse_iterator    const_reverse_iterator;
      typedef typename _String::reverse_iterator          reverse_iterator;

      static const uint32_t                               bound_ = _Bound;

      bounded_basic_string() { }

      explicit
      bounded_basic_string(const allocator_type& __a)
       : _String(__a) { }

      bounded_basic_string(const bounded_basic_string& __x)
       : _String(__x) { }

      bounded_basic_string(const _String& __x)
       : _String(__x) { }

      bounded_basic_string(const bounded_basic_string& __x, size_type __pos,
                           size_type __n = _String::npos)
       : _String(__x, __pos, __n) { }

      bounded_basic_string(const _String& __x, size_type __pos,
                           size_type __n = _String::npos)
       : _String(__x, __pos, __n) { }

      bounded_basic_string(const bounded_basic_string& __x, size_type __pos,
                           size_type __n, const _Alloc& __a)
       : _String(__x, __pos, __n, __a) { }

      bounded_basic_string(const _String& __x, size_type __pos,
                           size_type __n, const _Alloc& __a)
       : _String(__x, __pos, __n, __a) { }

      bounded_basic_string(const _CharT* __s,
                           size_type __n,
                           const _Alloc& __a = _Alloc ())
       : _String(__s, __n, __a) { }

      bounded_basic_string(const _CharT* __s,
                           const _Alloc& __a = _Alloc ())
       : _String(__s, __a) { }

      bounded_basic_string(size_type __n, _CharT __c,
                           const _Alloc& __a = _Alloc ())
       : _String(__n, __c, __a) { }

      bounded_basic_string(bounded_basic_string&& __x)
       : _String(std::move(__x)) { }

      bounded_basic_string(_String&& __x)
       : _String(std::move(__x)) { }

      bounded_basic_string(std::initializer_list<_CharT> __l,
             const _Alloc& __a = _Alloc())
      : _String(__l, __a) { }

      template<typename _InputIterator>
        bounded_basic_string(_InputIterator __beg, _InputIterator __end,
               const _Alloc& __a = _Alloc())
         : _String(__beg, __end, __a) { }

      ~bounded_basic_string() noexcept
      {}

      uint32_t bound () const
      {
        return _Bound;
      }

      bounded_basic_string&
      operator=(const _String& __x)
      { this->_String::operator = (__x); return *this; }

      bounded_basic_string&
      operator=(_String&& __x)
      { this->_String::operator = (std::move (__x)); return *this; }

      bounded_basic_string&
      operator=(const bounded_basic_string& __x)
      { this->_String::operator = (__x); return *this; }

      bounded_basic_string&
      operator=(bounded_basic_string&& __x)
      { this->_String::operator = (std::move (__x)); return *this; }

      bounded_basic_string&
      operator=(std::initializer_list<_CharT> __l)
      { this->_String::operator = (__l); return *this; }

      bounded_basic_string&
      operator+=(const bounded_basic_string& __str)
      { this->_String::operator+= (__str); return *this; }

      bounded_basic_string&
      operator+=(const _String& __str)
      { this->_String::operator+= (__str); return *this; }

      bounded_basic_string&
      operator+=(const _CharT* __s)
      { this->_String::operator+= (__s); return *this; }

      bounded_basic_string&
      operator+=(_CharT __c)
      { this->_String::operator+= (__c); return *this; }

      bounded_basic_string&
      operator+=(std::initializer_list<_CharT> __l)
      { this->_String::operator+= (__l); return *this; }

      bounded_basic_string&
      append(const bounded_basic_string& __str)
      { this->_String::append (__str); return *this; }

      bounded_basic_string&
      append(const bounded_basic_string& __str, size_type __pos, size_type __n)
      { this->_String::append (__str, __pos, __n); return *this; }

      bounded_basic_string&
      append(const _String& __str)
      { this->_String::append (__str); return *this; }

      bounded_basic_string&
      append(const _String& __str, size_type __pos, size_type __n)
      { this->_String::append (__str, __pos, __n); return *this; }

      bounded_basic_string&
      append(const _CharT* __s, size_type __n)
      { this->_String::append (__s, __n); return *this; }

      bounded_basic_string&
      append(const _CharT* __s)
      { this->_String::append (__s); return *this; }

      bounded_basic_string&
      append(size_type __n, _CharT __c)
      { this->_String::append (__n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        append(_InputIterator __first, _InputIterator __last)
        { return this->_String::append(__first, __last); }

      bounded_basic_string&
      append(std::initializer_list<_CharT> __l)
      { return this->append(__l.begin(), __l.size()); }

      bounded_basic_string&
      assign(const bounded_basic_string& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(bounded_basic_string&& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(const bounded_basic_string& __str, size_type __pos, size_type __n)
      { this->_String::assign (__str, __pos, __n); return *this; }


      bounded_basic_string&
      assign(const _String& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(_String&& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(const _String& __str, size_type __pos, size_type __n)
      { this->_String::assign (__str, __pos, __n); return *this; }

      bounded_basic_string&
      assign(const _CharT* __s, size_type __n)
      { this->_String::assign (__s, __n); return *this; }

      bounded_basic_string&
      assign(const _CharT* __s)
      { this->_String::assign (__s); return *this; }

      bounded_basic_string&
      assign(size_type __n, _CharT __c)
      { this->_String::assign (__n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        assign(_InputIterator __first, _InputIterator __last)
        { this->_String::assign (__first, __last); return *this; }

      bounded_basic_string&
      assign(std::initializer_list<_CharT> __l)
      { return this->assign(__l.begin(), __l.size()); }

      bounded_basic_string&
      insert(size_type __pos1, const bounded_basic_string& __str)
      { this->_String::insert(__pos1, __str); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const bounded_basic_string& __str,
             size_type __pos2, size_type __n)
      { this->_String::insert(__pos1, __str, __pos2, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const _String& __str)
      { this->_String::insert(__pos1, __str); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const _String& __str,
             size_type __pos2, size_type __n)
      { this->_String::insert(__pos1, __str, __pos2, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos, const _CharT* __s, size_type __n)
      { this->_String::insert(__pos, __s, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos, const _CharT* __s)
      { this->_String::insert(__pos, __s); return *this; }

      bounded_basic_string&
      insert(size_type __pos, size_type __n, _CharT __c)
      { this->_String::insert(__pos, __n, __c); return *this; }

      bounded_basic_string&
      erase(size_type __pos = 0, size_type __n = _String::npos)
      { this->_String::erase (__pos, __n); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n, const bounded_basic_string& __str)
      { this->_String::replace(__pos, __n, __str); return *this; }

      bounded_basic_string&
      replace(size_type __pos1, size_type __n1, const bounded_basic_string& __str,
              size_type __pos2, size_type __n2)
      { this->_String::replace(__pos1, __n1, __str, __pos2, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n, const _String& __str)
      { this->_String::replace(__pos, __n, __str); return *this; }

      bounded_basic_string&
      replace(size_type __pos1, size_type __n1, const _String& __str,
              size_type __pos2, size_type __n2)
      { this->_String::replace(__pos1, __n1, __str, __pos2, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s,
              size_type __n2)
      { this->_String::replace(__pos, __n1, __s, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s)
      { this->_String::replace(__pos, __n1, __s); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c)
      { this->_String::replace(__pos, __n1, __n2, __c); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const bounded_basic_string& __str)
      { this->_String::replace(__i1, __i2, __str); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _String& __str)
      { this->_String::replace(__i1, __i2, __str); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s, size_type __n)
      { this->_String::replace(__i1, __i2, __s, __n); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s)
      { this->_String::replace(__i1, __i2, __s); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, size_type __n, _CharT __c)
      { this->_String::replace(__i1, __i2, __n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        replace(iterator __i1, iterator __i2,
                _InputIterator __k1, _InputIterator __k2)
        { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, _CharT* __k1, _CharT* __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2,
              const _CharT* __k1, const _CharT* __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, iterator __k1, iterator __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2,
              const_iterator __k1, const_iterator __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string& replace(iterator __i1, iterator __i2,
                            std::initializer_list<_CharT> __l)
      { this->_String::replace(__i1, __i2, __l); return *this; }

      void
      swap(bounded_basic_string& __x)
      { this->_String::swap (__x); }

      void
      swap(_String& __x)
      { this->_String::swap (__x); }
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
              ACE_TEXT ("String %C bound %d\n"), s100.c_str(), s100.bound_));

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
