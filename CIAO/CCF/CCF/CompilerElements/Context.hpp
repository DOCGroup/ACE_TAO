// file      : CCF/CompilerElements/Context.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_COMPILER_ELEMENTS_CONTEXT_HPP
#define CCF_COMPILER_ELEMENTS_CONTEXT_HPP

#include <map>
#include <string>
#include <Utility/Hetero/Container.hpp>

namespace CCF
{
  namespace CompilerElements
  {
    class Context
    {
      typedef
      std::map<std::string, Utility::Hetero::Container>
      Map;

    public:
      Context ()
      {
      }

    public:
      class NotFound {};
      class Typing {};

      Map::size_type
      count (char const* key) const throw ()
      {
        return map_.count (key);
      }

      template <typename T>
      T const&
      get (char const* key) const
      {
        Map::const_iterator i = map_.find (key);
        if (i == map_.end ()) throw NotFound ();

        try
        {
          return i->second. template value<T> ();
        }
        catch (Utility::Hetero::Typing const&)
        {
          throw Typing ();
        }

      }

      template <typename T>
      T const&
      get (char const* key, T const& def) const
      {
        Map::const_iterator i = map_.find (key);
        if (i == map_.end ()) return def;

        try
        {
          return i->second. template value<T> ();
        }
        catch (Utility::Hetero::Typing const&)
        {
          throw Typing ();
        }
      }

      template <typename T>
      void
      set (char const* key, T const& value)
      {
        try
        {
          if (!map_.insert (std::pair<std::string,
                            Utility::Hetero::Container>(key, value)).second)
          {
            Map::iterator i = map_.find (key);
            i->second.template value <T> () = value;
          }
        }
        catch (Utility::Hetero::Typing const&)
        {
          throw Typing ();
        }
      }

      void
      remove (char const* key)
      {
        Map::iterator i (map_.find (key));

        if (i == map_.end ()) throw NotFound ();

        map_.erase (i);
      }

    private:
      Map map_;

    private:
      // Copy semanic is not supported.
      Context (Context const&) throw ();
      Context& operator= (Context const&) throw ();
    };
  }
}

#endif  // CCF_COMPILER_ELEMENTS_CONTEXT_HPP
