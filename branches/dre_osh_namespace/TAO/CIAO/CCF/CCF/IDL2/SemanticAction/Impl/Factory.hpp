// file      : CCF/IDL2/SemanticAction/Impl/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_FACTORY_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_FACTORY_HPP

#include "CCF/IDL2/SemanticAction/Factory.hpp"

#include "CCF/IDL2/SemanticAction/Impl/Attribute.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Include.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Interface.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Member.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Module.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Operation.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Struct.hpp"
#include "CCF/IDL2/SemanticAction/Impl/Typedef.hpp"
#include "CCF/IDL2/SemanticAction/Impl/TypeId.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      namespace Impl
      {
        //
        //
        //
        class Factory : public virtual IDL2::SemanticAction::Factory
        {
        public:
          virtual
          ~Factory () throw () {}


          Factory (CompilerElements::Context& context,
                   Diagnostic::Stream& dout,
                   SyntaxTree::TranslationRegionPtr const& r)
              : trace_ (context.get ("idl2::semantic-action::trace", false)),

                scope_ (r->scope ()),

                attribute_ (trace_, scope_),
                include_ (trace_, *this, r, scope_),
                interface_ (trace_, scope_),
                member_ (trace_, scope_),
                module_ (trace_, scope_),
                operation_ (trace_, scope_),
                struct__ (trace_, scope_),
                typedef__ (trace_, scope_),
                type_id_ (trace_, scope_),
                type_prefix_ (trace_, scope_)
          {
          }

          virtual SemanticAction::Attribute&
          attribute ()
          {
            return attribute_;
          }

          virtual SemanticAction::Include&
          include ()
          {
            return include_;
          }

          virtual SemanticAction::Interface&
          interface ()
          {
            return interface_;
          }

          virtual SemanticAction::Member&
          member ()
          {
            return member_;
          }

          virtual SemanticAction::Module&
          module ()
          {
            return module_;
          }

          virtual SemanticAction::Operation&
          operation ()
          {
            return operation_;
          }

          virtual SemanticAction::Struct&
          struct_ ()
          {
            return struct__;
          }

          virtual SemanticAction::Typedef&
          typedef_ ()
          {
            return typedef__;
          }

          virtual SemanticAction::TypeId&
          type_id ()
          {
            return type_id_;
          }

          virtual SemanticAction::TypePrefix&
          type_prefix ()
          {
            return type_prefix_;
          }

        protected:
          bool trace_;

          SyntaxTree::ScopePtr scope_;
          SyntaxTree::TranslationRegionPtr region_;

          Attribute attribute_;
          Include include_;
          Interface interface_;
          Member member_;
          Module module_;
          Operation operation_;
          Struct struct__;
          Typedef typedef__;
          TypeId type_id_;
          TypePrefix type_prefix_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_FACTORY_HPP
