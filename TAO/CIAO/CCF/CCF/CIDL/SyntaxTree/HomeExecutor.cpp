// file      : CCF/CIDL/SyntaxTree/HomeExecutor.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/CIDL/SyntaxTree/HomeExecutor.hpp"

using namespace Introspection;

namespace CCF
{
  namespace CIDL
  {
    namespace SyntaxTree
    {
      // HomeExecutor
      //
      //
      namespace
      {
        TypeInfo
        home_executor_init_ ()
        {
          TypeInfo ti (typeid (HomeExecutor));
          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo home_executor_ (home_executor_init_ ());
      }

      TypeInfo const& HomeExecutor::
      static_type_info () { return home_executor_; }
    }
  }
}
