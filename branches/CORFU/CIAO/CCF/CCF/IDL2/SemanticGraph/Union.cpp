// file      : CCF/IDL2/SemanticGraph/Union.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Union.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;


      // Selects
      //
      //
      namespace
      {
        TypeInfo
        selects_init_ ()
        {
          TypeInfo ti (typeid (Selects));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo selects_ (selects_init_ ());
      }

      TypeInfo const& Selects::
      static_type_info () { return selects_; }


      // UnionMember
      //
      //
      namespace
      {
        TypeInfo
        union_member_init_ ()
        {
          TypeInfo ti (typeid (UnionMember));
          ti.add_base (Access::PUBLIC, true, Member::static_type_info ());
          return ti;
        }

        TypeInfo union_member_ (union_member_init_ ());
      }

      TypeInfo const& UnionMember::
      static_type_info () { return union_member_; }


      // Selector
      //
      //
      namespace
      {
        TypeInfo
        selector_init_ ()
        {
          TypeInfo ti (typeid (Selector));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo selector_ (selector_init_ ());
      }

      TypeInfo const& Selector::
      static_type_info () { return selector_; }

      Selector::
      ~Selector ()
      {
      }

      // DefaultSelector
      //
      //
      namespace
      {
        TypeInfo
        default_selector_init_ ()
        {
          TypeInfo ti (typeid (DefaultSelector));
          ti.add_base (Access::PUBLIC, true, Selector::static_type_info ());
          return ti;
        }

        TypeInfo default_selector_ (default_selector_init_ ());
      }

      TypeInfo const& DefaultSelector::
      static_type_info () { return default_selector_; }


      // ValueSelector
      //
      //
      namespace
      {
        TypeInfo
        value_selector_init_ ()
        {
          TypeInfo ti (typeid (ValueSelector));
          ti.add_base (Access::PUBLIC, true, Selector::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          return ti;
        }

        TypeInfo value_selector_ (value_selector_init_ ());
      }

      TypeInfo const& ValueSelector::
      static_type_info () { return value_selector_; }

      // Union
      //
      //
      namespace
      {
        TypeInfo
        union_init_ ()
        {
          TypeInfo ti (typeid (Union));

          ti.add_base (
            Access::PUBLIC, true, Specialization::static_type_info ());

          ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
          return ti;
        }

        TypeInfo union_ (union_init_ ());
      }

      TypeInfo const& Union::
      static_type_info () { return union_; }

      bool Union::
      complete () const
      {
        if (defined ())
        {
          CompilerElements::Context& ctx (
            const_cast<CompilerElements::Context&> (context ()));

          if (ctx.count ("union-complete-test"))
            return true;

          ctx.set ("union-complete-test", true);
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

          ctx.remove ("union-complete-test");
          return c;
        }

        return false;
      }
    }
  }
}
