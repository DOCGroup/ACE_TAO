// file      : Utility/ExH/Logic/DescriptiveException.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_LOGIC_DESCRIPTIVE_EXCEPTION_HPP
#define UTILITY_EX_H_LOGIC_DESCRIPTIVE_EXCEPTION_HPP

#include <memory>
#include <string>


#include "Utility/ExH/Logic/Exception.hpp"

namespace Utility
{
  namespace ExH
  {
    namespace Logic
    {
      class DescriptiveException : public virtual Exception
      {
      public:
        typedef Exception Base; 
        
        explicit
        DescriptiveException (char const* description) throw ();

        explicit
        DescriptiveException (std::string const& description) throw ();

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
        std::auto_ptr<std::string> description_;
      };
    }
  }
}

#include "Utility/ExH/Logic/DescriptiveException.ipp"
#include "Utility/ExH/Logic/DescriptiveException.tpp"

#endif  // UTILITY_EX_H_LOGIC_DESCRIPTIVE_EXCEPTION_HPP
//$Id$
