// file      : CCF/IDL2/SemanticAction/Impl/Typedef.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP

#include "CCF/IDL2/SyntaxTree/Typedef.hpp"
#include "CCF/IDL2/SyntaxTree/Sequence.hpp"
#include "CCF/IDL2/SemanticAction/Typedef.hpp"

#include <sstream>

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
        class Typedef : public virtual SemanticAction::Typedef
        {
        public:
          virtual
          ~Typedef () throw () {}

          Typedef (bool trace, SyntaxTree::ScopePtr& current)
              : trace_ (trace),
                scope_ (current),
                type_ ("::") //@@ this is dirty
          {
          }

          virtual void
          begin (IdentifierPtr const& id)
          {
            if (trace_) cerr << "typedef " << id << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) const throw ()
              {
                return d->is_a<TypeDecl> ();
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
              cerr << "error: invalid typedef declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid typedef declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
          }

          virtual void
          begin_seq (IdentifierPtr const& id)
          {
            if (trace_) cerr << "typedef sequence<" << id << ">" << endl;

            using namespace SyntaxTree;

            Name name (id->lexeme ());

            //@@ can use the same predicate with begin ()
            //
            struct Predicate : public DeclarationTable::ResolvePredicate
            {
              virtual bool
              test (DeclarationPtr const& d) const throw ()
              {
                return d->is_a<TypeDecl> ();
              }
            } p;

            try
            {
              ScopedName type (scope_->table ().resolve (
                                 name,
                                 scope_->name (),
                                 scope_->peek_order (),
                                 p));

              Order order = scope_->create_order ();

              std::ostringstream ostr;
              ostr << order;

              SimpleName name (ostr.str ());

              UnboundedSequenceDeclPtr seq (
                new UnboundedSequenceDecl (name, order, scope_, type));

              scope_->insert (seq);
              
              type_ = seq->name ();

              if (trace_) cerr << "assigned anonymous sequence name "
                               << type_ << endl;

            }
            catch (DeclarationTable::NameNotFound const&)
            {
              cerr << "error: invalid sequence declaration" << endl;
              cerr << "no type with name \'"
                   << name << "\' visible from scope \'"
                   << scope_->name () << "\'" << endl;
            }
            catch (DeclarationTable::PredicateNotMet const&)
            {
              cerr << "error: invalid sequence declaration" << endl;
              cerr << "no type with name \'"
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
              TypedefDeclPtr typedef_ (
                new TypedefDecl (SimpleName (id->lexeme ()),
                                 type_,
                                 scope_));


              scope_->insert (typedef_);
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

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_TYPEDEF_HPP
