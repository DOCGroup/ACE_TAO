// file      : CCF/CIDL/SemanticGraph/Executor.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticGraph/Executor.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Executor
      //
      //
      namespace
      {
        TypeInfo
        executor_init_ ()
        {
          TypeInfo ti (typeid (Executor));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          return ti;
        }

        TypeInfo executor_ (executor_init_ ());
      }

      TypeInfo const& Executor::
      static_type_info () { return executor_; }

      Executor::
      ~Executor ()
      {
      }


      // ComponentExecutor
      //
      //
      namespace
      {
        TypeInfo
        component_executorinit_ ()
        {
          TypeInfo ti (typeid (ComponentExecutor));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          return ti;
        }

        TypeInfo component_executor (component_executorinit_ ());
      }

      TypeInfo const& ComponentExecutor::
      static_type_info () { return component_executor; }


      // HomeExecutor
      //
      //
      namespace
      {
        TypeInfo
        home_executorinit_ ()
        {
          TypeInfo ti (typeid (HomeExecutor));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          return ti;
        }

        TypeInfo home_executor (home_executorinit_ ());
      }

      TypeInfo const& HomeExecutor::
      static_type_info () { return home_executor; }
    }
  }
}
