// file      : CCF/IDL2/SemanticGraph/Struct.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Struct.hpp"
#include "CCF/IDL2/SemanticGraph/Member.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      namespace
      {
        TypeInfo
        struct_init_ ()
        {
          TypeInfo ti (typeid (Struct));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo struct_ (struct_init_ ());
      }

      TypeInfo const& Struct::
      static_type_info () { return struct_; }

      bool Struct::
      complete () const
      {
        if (defined ())
        {
          CompilerElements::Context& ctx (
            const_cast<CompilerElements::Context&> (context ()));

          if (ctx.count ("struct-complete-test"))
            return true;

          ctx.set ("struct-complete-test", true);
          bool c (true);

          for (Scope::NamesIterator i (names_begin ());
               c && i != names_end ();
               ++i)
          {
            Member const& m (dynamic_cast<Member&> ((*i)->named ()));
            Type const& t (m.belongs ().type ());

            if (!t.complete ())
              c = false;
          }

          ctx.remove ("struct-complete-test");
          return c;
        }

        return false;
      }
    }
  }
}
