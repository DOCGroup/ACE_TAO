// file      : CCF/IDL2/SemanticAction/Impl/Member.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_MEMBER_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_MEMBER_HPP

#include "CCF/IDL2/SyntaxTree/Member.hpp"
#include "CCF/IDL2/SemanticAction/Member.hpp"

using namespace std;

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
        class Member : public virtual SemanticAction::Member
        {
        public:
          virtual
          ~Member () throw () {}

          Member (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current),
                type_ ("::") //@@ this is dirty
          {
          }

          virtual void
          begin (IdentifierPtr const& id)
          {
            if (trace_) cerr << "member " << id << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) const throw ()
              {
                //@@ loosing information (completeness)
                //

                return d->is_a<TypeDecl> () &&
                  d->dynamic_type<TypeDecl> ()->complete ();
              }
            } p;

            try
            {
              type_ = scope_->table ().resolve (
                name,
                scope_->name (),
                scope_->peek_order (),
                p);
            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid member declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid member declaration" << endl;
              cerr << "no complete type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
          }


          virtual void
          declarator (SimpleIdentifierPtr const& id)
          {
            if (trace_) cerr << "  " << id << endl;

            using namespace SyntaxTree;

            if (type_ != ScopedName ("::"))
            {
              MemberDeclPtr member_ (
                new MemberDecl (SimpleName (id->lexeme ()),
                                type_,
                                scope_));

              scope_->insert (member_);
            }
          }

          virtual void
          end ()
          {
            if (trace_) cerr << "end" << endl;

            // Indicate that we are done.
            //
            type_ = SyntaxTree::ScopedName ("::");
          }

        private:
          bool trace_;
          SyntaxTree::ScopePtr& scope_;
          SyntaxTree::ScopedName type_;
        };
      }
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_MEMBER_HPP
