// file      : Utility/ExH/System/DescriptiveException.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_SYSTEM_DESCRIPTIVE_EXCEPTION_HPP
#define UTILITY_EX_H_SYSTEM_DESCRIPTIVE_EXCEPTION_HPP

#include "Utility/ExH/System/Exception.hpp"

namespace Utility
{
  namespace ExH
  {
    namespace System
    {
      class DescriptiveException : public virtual Exception
      {
      public:
        explicit
        DescriptiveException (char const* description) throw ();

        template <typename T>
        explicit
        DescriptiveException (T const& description) throw ();

        DescriptiveException (DescriptiveException const& src) throw ();

        virtual
        ~DescriptiveException () throw ();

        DescriptiveException&
        operator= (DescriptiveException const& src) throw ();

      protected:
        DescriptiveException () throw ();

        void
        init (char const* description) throw ();

      public:
        virtual char const*
        what () const throw ();

      private:

        static unsigned long const DESCRIPTION_SIZE = 256;

        char description_ [DESCRIPTION_SIZE];
      };
    }
  }
}

#include "Utility/ExH/System/DescriptiveException.ipp"

#endif  // UTILITY_EX_H_SYSTEM_DESCRIPTIVE_EXCEPTION_HPP

