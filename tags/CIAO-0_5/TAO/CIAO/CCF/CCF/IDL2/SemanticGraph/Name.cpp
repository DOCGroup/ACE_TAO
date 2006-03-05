// file      : CCF/IDL2/SemanticGraph/Name.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Name.hpp"

#include <ostream>

/*
#include <iostream>

using std::cerr;
using std::endl;
*/

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {

      // SimpleName
      //
      //

      SimpleName::
      SimpleName (char const* name) throw (InvalidName)
          : name_ (name)
      {
        if (name_.find (':') != std::string::npos) throw InvalidName ();
      }

      SimpleName::
      SimpleName (std::string const& name) throw (InvalidName)
          : name_ (name)
      {
        if (name_.find (':') != std::string::npos) throw InvalidName ();
      }

      SimpleName
      operator+ (SimpleName const& name, std::string const& str)
      {
        return SimpleName (name.str () + str);
      }

      SimpleName
      operator+ (std::string const& str, SimpleName const& name)
      {
        return SimpleName (str + name.str ());
      }

      std::ostream&
      operator << (std::ostream& o, SimpleName const& name)
      {
        return o << name.str ();
      }

      // Name
      //
      //
      Name::
      Name (SimpleName const& name)
          : name_cache_ (name.str ())
      {
        name_.push_back (name);
      }

      Name::
      Name (char const* name) throw (InvalidName)
          : name_cache_ (name)
      {
        init ();
      }

      Name::
      Name (std::string const& name) throw (InvalidName)
        : name_cache_ (name)
      {
        init ();
      }

      void Name::
      init () throw (InvalidName)
      {
        // cerr << "parsing name \'" << name_cache_ << "\' {"<< endl;

        for (std::string::size_type
               pos (0), next (name_cache_.find ("::", pos));;
             next = name_cache_.find ("::", pos))
        {
          std::string simple_name (
            name_cache_,
            pos,
            next == std::string::npos ? next : next - pos);

          // cerr << "\t\'" << simple_name << '\'' << endl;

          if (simple_name.empty () && !name_.empty ())
          {
            // Empty name (file-scope) is only valid when
            // it is first.
            //
            throw InvalidName ();
          }

          name_.push_back (SimpleName (simple_name));

          if (next == std::string::npos) break;

          pos = next + 2;
        }

        // cerr << "parsing name }" << name_cache_ << endl;

        if (name_.empty ()) throw InvalidName ();
      }


      Name::
      Name (Iterator begin, Iterator end) throw (InvalidName)
      {
        for (Iterator i (begin); i != end; ++i)
        {
          name_cache_ += (i != begin ? "::" : "") + i->str ();
          name_.push_back (*i);
        }

        if (name_.empty ()) throw InvalidName ();
      }

      bool Name::
      scoped () const
      {
        return name_[0] == SimpleName ("");
      }

      bool Name::
      simple () const
      {
        return name_.size () == 1;
      }

      std::ostream&
      operator << (std::ostream& o, Name const& name)
      {
        return o << name.str ();
      }

      // ScopedName
      //
      //

      ScopedName::
      ScopedName (char const* name) throw (InvalidName)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (std::string const& name) throw (InvalidName)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (Iterator begin, Iterator end) throw (InvalidName)
          : Name (begin, end)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (Name const& name) throw (InvalidName)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (ScopedName const& scope, Name const& name)
        throw (InvalidName)
          : Name (scope.str () + "::" + name.str ())
      {
        if (!scoped ()) throw InvalidName ();
      }

      SimpleName ScopedName::
      simple_name () const
      {
        return *(end () - 1);
      }

      ScopedName ScopedName::
      scope_name () const throw (FileScope)
      {
        Iterator end (this->end () - 1);

        if (begin () == end) throw FileScope ();

        return ScopedName (begin (), end);
      }

      /*
      Name ScopedName::
      in_file_scope () const throw (FileScope)
      {
        if (name_.empty ()) throw FileScope ();

        return Name(std::string (name_.begin () + 2, name_.end ()));
      }

      */
    }
  }
}
