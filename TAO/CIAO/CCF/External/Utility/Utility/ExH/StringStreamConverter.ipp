// file      : Utility/ExH/StringStreamConverter.ipp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace ExH
  {
    template <> 
    inline char const*
    converter (std::ostringstream const& t) throw ()
    {
      return t.str ().c_str ();
    }

  }
}

