// file      : Utility/ExH/Compound.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_COMPOUND_HPP
#define UTILITY_EX_H_COMPOUND_HPP

#include <string>

namespace Utility
{
  namespace ExH
  {
    template <typename Type, typename _Base>
    class Compound : public virtual _Base
    {
    public:
      typedef _Base Base;
      
      explicit
      Compound (char const* description) throw ();

      explicit
      Compound (std::string const& description) throw ();

      template <typename T>
      explicit
      Compound (T const& description) throw ();

      Compound (Compound const& src) throw ();

      virtual
      ~Compound () throw ();

    public:
      Compound&
      operator= (Compound const& src) throw ();

    protected:
      Compound () throw ();
    };
  }
}

#include "Utility/ExH/Compound.tpp"

#endif  // UTILITY_EX_H_COMPOUND_HPP
//$Id$
