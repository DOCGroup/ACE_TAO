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
      SimpleName (char const* name)
          : escaped_ (name[0] == '_'),
            name_ (escaped_ ? name + 1 : name)
      {
        if (name_.find (':') != std::string::npos)
          throw InvalidName ();
      }

      SimpleName::
      SimpleName (std::string const& name)
          : escaped_ (name[0] == '_'),
            name_ (name, escaped_ ? 1 : 0)
      {
        if (name_.find (':') != std::string::npos)
          throw InvalidName ();
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

      // Name
      //
      //
      Name::
      Name (SimpleName const& name)
          : name_cache_ (name.unescaped_str ())
      {
        name_.push_back (name);
      }

      Name::
      Name (char const* name)
      {
        init (name);
      }

      Name::
      Name (std::string const& name)
      {
        init (name);
      }

      void Name::
      init (std::string const& name)
      {
        // cerr << "parsing name \'" << name_cache_ << "\' {"<< endl;

        for (std::string::size_type pos (0), next (name.find ("::", pos));;
             next = name.find ("::", pos))
        {
          std::string simple_name (
            name,
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
          name_cache_ += (pos != 0 ? "::" : "") +
            name_.back ().unescaped_str ();

          if (next == std::string::npos)
            break;

          pos = next + 2;
        }

        // cerr << "parsing name }" << name_cache_ << endl;

        if (name_.empty ())
          throw InvalidName ();
      }


      Name::
      Name (Iterator begin, Iterator end)
      {
        for (Iterator i (begin); i != end; ++i)
        {
          name_cache_ += (i != begin ? "::" : "") + i->unescaped_str ();
          name_.push_back (*i);
        }

        if (name_.empty ())
          throw InvalidName ();
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

      // ScopedName
      //
      //

      ScopedName::
      ScopedName (char const* name)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (std::string const& name)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (Iterator begin, Iterator end)
          : Name (begin, end)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (Name const& name)
          : Name (name)
      {
        if (!scoped ()) throw InvalidName ();
      }

      ScopedName::
      ScopedName (ScopedName const& scope, Name const& name)
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
      scope_name () const
      {
        Iterator end (this->end () - 1);

        if (begin () == end) throw FileScope ();

        return ScopedName (begin (), end);
      }


      // NamePrinter
      //
      int const name_printer_index = std::ios_base::xalloc ();

      NamePrinter::~NamePrinter ()
      {
      }

      void NamePrinter::
      print (std::ostream& os, Name const& n)
      {
        for (Name::Iterator b (n.begin ()), i (b); i != n.end (); ++i)
        {
          if (i != b)
            os << "::";

          print (os, *i);
        }
      }
    }
  }
}

std::ostream&
operator << (std::ostream& o,
             CCF::IDL2::SemanticGraph::SimpleName const& name)
{
  using namespace CCF::IDL2::SemanticGraph;

  if (void* tmp = o.pword (name_printer_index))
  {
    NamePrinter* p (reinterpret_cast<NamePrinter*> (tmp));
    p->print (o, name);
    return o;
  }
  else
    return o << name.str ();
}

std::ostream&
operator << (std::ostream& o, CCF::IDL2::SemanticGraph::Name const& name)
{
  using namespace CCF::IDL2::SemanticGraph;

  if (void* tmp = o.pword (name_printer_index))
  {
    NamePrinter* p (reinterpret_cast<NamePrinter*> (tmp));
    p->print (o, name);
    return o;
  }
  else
    return o << name.str ();
}
