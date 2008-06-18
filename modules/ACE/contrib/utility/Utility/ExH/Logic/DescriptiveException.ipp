// file      : Utility/ExH/Logic/DescriptiveException.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace ExH
  {
    namespace Logic
    {
      // c-tor's & d-tor

      inline DescriptiveException::
      DescriptiveException () throw ()
      {
      }

      inline DescriptiveException::
      DescriptiveException (char const* description) throw ()
      {
        init (description);
      }

      inline DescriptiveException::
      DescriptiveException (std::string const& description) throw ()
      {
        try
        {
          init (description.c_str ());
        }
        catch (...)
        {
        }
      }

      inline DescriptiveException::
      DescriptiveException (DescriptiveException const& src) throw ()
        : std::exception (),
          Exception ()
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

      inline void
      DescriptiveException::init (char const* description) throw ()
      {
        try
        {
          if (description == 0 || description[0] == '\0')
          {
            description_.reset (0);
          }
          else
          {
            if (description_.get () != 0)
            {
              *description_ = description;
            }
            else
            {
              description_.reset (new std::string (description));
            }
          }
        }
        catch (...)
        {
          description_.reset (0);
        }
      }

      inline char const*
      DescriptiveException::what () const throw ()
      {
        try
        {
          if (description_.get () != 0)
          {
            return description_->c_str ();
          }
        }
        catch (...)
        {
        }

        return Exception::what ();
      }
    }
  }
}
//$Id$
