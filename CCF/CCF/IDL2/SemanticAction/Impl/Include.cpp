// file      : CCF/IDL2/SemanticAction/Impl/Include.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticAction/Impl/Include.hpp"

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
        // @@ This is left unimplemented for now. Later, code from
        // IDL3 should be copied here. Or better yet this implementation
        // should allow customization via overriding.
        //
        Include::
        Include (Context& c)
            : ctx (c)
        {
        }

        void Include::
        quote (StringLiteralPtr const& sl)
        {
          if (ctx.trace ()) cerr << "__qinclude " << sl << endl;
        }

        void Include::
        bracket (StringLiteralPtr const& sl)
        {
          if (ctx.trace ()) cerr << "__binclude " << sl << endl;
        }

        void Include::
        end ()
        {
          if (ctx.trace ()) cerr << "end" << endl;
        }
      }
    }
  }
}
