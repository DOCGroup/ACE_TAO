// file      : Utility/ExH/System/DescriptiveException.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include <cstring>

#include "Utility/ExH/Converter.hpp"

namespace Utility
{
  namespace ExH
  {
    namespace System
    {
      // c-tor's & d-tor

      inline DescriptiveException::
      DescriptiveException () throw ()
      {
        description_[0] = '\0';
      }

      inline DescriptiveException::
      DescriptiveException (char const* description) throw ()
      {
        init (description);
      }

      template <typename T>
      inline DescriptiveException::
      DescriptiveException (T const& description) throw ()
      {
        init (converter<T> (description));
      }

      inline DescriptiveException::
      DescriptiveException (DescriptiveException const& src) throw ()
      {
        init (src.what ());
      }

      inline DescriptiveException::
      ~DescriptiveException () throw ()
      {
      }

      inline DescriptiveException& DescriptiveException::
      operator= (DescriptiveException const& src) throw ()
      {
        init (src.what ());
        return *this;
      }


      // accessors / modifiers

      inline void DescriptiveException::
      init (char const* description) throw ()
      {
        if (description != 0)
        {
          std::strncpy (description_, description, DESCRIPTION_SIZE - 1);
          description_[DESCRIPTION_SIZE - 1] = '\0';
        }
        else
        {
          description_[0] = '\0';
        }
      }

      inline char const* DescriptiveException::
      what () const throw ()
      {
        if (description_[0] != '\0')
        {
          return description_;
        }
        else
        {
          return Exception::what ();
        }
      }
    }
  }
}

