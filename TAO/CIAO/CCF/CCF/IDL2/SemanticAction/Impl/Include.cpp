// file      : CCF/IDL2/SemanticAction/Impl/Include.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Include.hpp"

// #include "CCF/IDL2/SemanticGraph/Translation.hpp"

#include <iostream>

using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        // @@ This is left unimplemented for now. Later code from
        // IDL3 should be copied here.
        //
        //

        // Include
        //
        //
        Include::
        ~Include () throw ()
        {
        }

        Include::
        Include (Context& c)
            : ctx (c)
        {
        }

        void Include::
        begin (StringLiteralPtr const& sl)
        {
          if (ctx.trace ()) cerr << "include " << sl << endl;
        }

        void Include::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }

        // SystemInclude
        //
        //
        SystemInclude::
        ~SystemInclude () throw ()
        {
        }

        SystemInclude::
        SystemInclude (Context& c)
            : ctx (c)
        {
        }

        void SystemInclude::
        begin (StringLiteralPtr const& sl)
        {
          if (ctx.trace ()) cerr << "include <" << sl->lexeme () << ">"
                                 << endl;
        }

        void SystemInclude::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
