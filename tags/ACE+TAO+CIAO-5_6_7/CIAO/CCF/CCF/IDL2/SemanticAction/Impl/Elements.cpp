// file      : CCF/IDL2/SemanticAction/Impl/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Elements.hpp"

#include <iostream>

using std::cerr;
using std::endl;

namespace
{
  bool trace (false);
}

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        // @@ Need pointer iterator adapter.

        using namespace SemanticGraph;
        using SemanticGraph::Scope;

        Nameables
        resolve (TranslationUnit& tu,
                 ScopedName const& from,
                 Name const& name)
        {

          if (trace) cerr << "resolve: starting resolution process for \'"
                          << name << "\' from \'" << from << "\'" << endl;

          Nameables r;

          // Check if name is already scoped.
          //
          if (name.scoped ())
          {
            r  = tu.lookup (ScopedName (name));
          }
          else
          {
            // Obtain all nodes corresponding to this scope name.
            //

            for (ScopedName next (from);;)
            {
              Nameables scopes (tu.lookup (next));

              // Lookup the name in each scope.
              //
              for (Nameables::const_iterator
                     i (scopes.begin ()), end (scopes.end ());
                   i != end; ++i)
              {
                if (Scope* s = dynamic_cast<Scope*> (*i))
                {
                  s->lookup (name, r);
                }
              }

              if (r.begin () != r.end () || next.simple ())
              {
                // Found something -or- at the root scope.
                //
                break;
              }

              next = ScopedName (next.begin (), next.end () - 1);
            }
          }

          if (r.begin () == r.end ())
          {
            if (trace) cerr << "resolve: resolution process for \'"
                            << name << "\' from \'" << from << "\' failed."
                            << endl;

            throw NotFound ();
          }
          else
          {
            if (trace) cerr << "resolve: resolution process for \'"
                            << name << "\' from \'" << from << "\' succeeded:"
                            << endl;

            for (Nameables::const_iterator i (r.begin ()), end (r.end ());
                 i != end; ++i)
            {
              if (trace) cerr << "\t\'" << (**i).scoped_name () << "\'"
                              << endl;
            }
          }

          return r;
        }


        // Lookup *all* scopes for a simple name
        //
        //
        Nameable*
        lookup (SemanticGraph::TranslationUnit& tu,
                SemanticGraph::Scope& in,
                SemanticGraph::SimpleName const& name)
        {
          Nameables r;

          Nameables scopes (tu.lookup (in.scoped_name ()));

          for (Nameables::const_iterator
                 i (scopes.begin ()), end (scopes.end ());
               i != end; ++i)
          {
             dynamic_cast<Scope&> (**i).lookup (name, r);
          }

          if (r.size () > 1) throw NotUnique ();

          if (r.empty ()) return 0;

          return *(r.begin ());
        }
      }
    }
  }
}
