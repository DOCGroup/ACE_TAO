// file      : CCF/IDL2/SemanticAction/Impl/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP
#define CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP

#include <stack>

#include "CCF/IDL2/SemanticGraph/Translation.hpp"
#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

#include "CCF/IDL2/SemanticAction/Elements.hpp"

/*
  Design notes:

  (1) I am using stack-based approach to handle cases like this:

  struct A
  {
    struct B
    {
      struct C
      {
        ...
      } c;

    } b;
  };

  Even though you can't write the same let's say for interface
  I am still using stack approach for consistency and possible
  meta-coding.

*/

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
        class Context
        {
        public:
          Context (bool trace,
                   SemanticGraph::TranslationUnit& tu,
                   CompilerElements::Context& parsing_context)
              : trace_ (trace),
                tu_ (tu),
                parsing_context_ (parsing_context)
          {
          }

          bool
          trace ()
          {
            return trace_;
          }

          SemanticGraph::TranslationUnit&
          tu () const
          {
            return tu_;
          }

          SemanticGraph::Scope&
          scope () const
          {
            return *scope_;
          }

          void
          scope (SemanticGraph::Scope& scope)
          {
            scope_ = &scope;
          }

          SemanticGraph::TranslationRegion&
          region () const
          {
            return *region_;
          }

          void
          region (SemanticGraph::TranslationRegion& r)
          {
            region_ = &r;
          }

          CompilerElements::Context&
          parsing_context () const
          {
            return parsing_context_;
          }

          SemanticGraph::Path const&
          file () const
          {
            return parsing_context_.get<SemanticGraph::Path> ("file-path");
          }

        public:
          void
          int_exp_push (SemanticGraph::IntExpression& e)
          {
            int_exp_stack_.push (&e);
          }

          SemanticGraph::IntExpression&
          int_exp_pop ()
          {
            SemanticGraph::IntExpression& e (*int_exp_stack_.top ());
            int_exp_stack_.pop ();
            return e;
          }

          std::size_t
          int_exp_size ()
          {
            return int_exp_stack_.size ();
          }

          void
          int_exp_flush ()
          {
            while (int_exp_stack_.size ())
              int_exp_stack_.pop ();
          }

        private:
          bool trace_;
          SemanticGraph::TranslationUnit& tu_;
          SemanticGraph::TranslationRegion* region_;
          SemanticGraph::Scope* scope_;

          CompilerElements::Context& parsing_context_;

          std::stack<SemanticGraph::IntExpression*> int_exp_stack_;

        private:
          Context (Context const&);
          void operator= (Context const&);
        };


        //
        //
        //
        class Base
        {
        protected:
          virtual
          ~Base ()
          {
          }

          Base (Context& c)
              : ctx (c)
          {
          }

        protected:

          class Resolve {};

          template <typename Id>
          struct ExceptionTemplate : Resolve
          {
            explicit
            ExceptionTemplate (SemanticGraph::ScopedName name)
                : name_ (name)
            {
            }

            SemanticGraph::ScopedName
            name () const
            {
              return name_;
            }

          private:
            SemanticGraph::ScopedName name_;
          };


          struct NotFound : Resolve {};

          class WrongType_ {};
          typedef ExceptionTemplate<WrongType_> WrongType;

          class NotUnique : Resolve {};

          class NotDefined_ {};
          typedef ExceptionTemplate<NotDefined_> NotDefined;

          class NotComplete_ {};
          typedef ExceptionTemplate<NotComplete_> NotComplete;

          struct Flags
          {
            enum Value
            {
              none = 0x00,
              complete = 0x01,
              defined = 0x02
            };
          };

          template <typename T>
          T&
          resolve (SemanticGraph::ScopedName const& from,
                   SemanticGraph::Name const& name,
                   Flags::Value flags = Flags::none);


          class AlreadyInherited_ {};
          typedef ExceptionTemplate<AlreadyInherited_> AlreadyInherited;

          class AlreadySupported_ {};
          typedef ExceptionTemplate<AlreadySupported_> AlreadySupported;

          template<typename I>
          void
          check_inheritance (I begin, I end, SemanticGraph::Nameable& t);

          template<typename I>
          void
          check_support (I begin, I end, SemanticGraph::Nameable& t);

        protected:
          Context& ctx;
        };


        //
        //
        //
        template <typename T>
        class ScopeBase : public Base
        {
        protected:
          ScopeBase (Context& c)
              : Base (c)
          {
          }

        protected:
          T&
          now () const
          {
            return *now_;
          }

          void
          now (T& t)
          {
            now_ = &t;
          }

          void
          push (SemanticGraph::Scope& t)
          {
            stack_.push (&t);
          }

          void
          pop ()
          {
            stack_.pop ();
          }

          SemanticGraph::Scope&
          top ()
          {
            return *stack_.top ();
          }

        private:
          T* now_;
          std::stack<SemanticGraph::Scope*> stack_;
        };


        //
        //
        //
        class NotFound {};
        class NotUnique {};

        SemanticGraph::Nameables
        resolve (SemanticGraph::TranslationUnit& tu,
                 SemanticGraph::ScopedName const& from,
                 SemanticGraph::Name const& name);




        SemanticGraph::Nameable*
        lookup (SemanticGraph::TranslationUnit& tu,
                SemanticGraph::Scope& in,
                SemanticGraph::SimpleName const& name);
      }
    }
  }
}

#include "CCF/IDL2/SemanticAction/Impl/Elements.tpp"

#endif  // CCF_IDL2_SEMANTIC_ACTION_IMPL_ELEMENTS_HPP
