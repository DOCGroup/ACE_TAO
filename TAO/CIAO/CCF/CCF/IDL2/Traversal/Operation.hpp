// file      : CCF/IDL2/Traversal/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_OPERATION_HPP
#define CCF_IDL2_TRAVERSAL_OPERATION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      //
      //
      //
      class OperationTraverser : public Traverser
      {
      public:
        OperationTraverser (Dispatcher* type)
            : type_ (type)
        {
        }

      public:
        virtual void
        add_parameter_delegate (Dispatcher* d)
        {
          parameter_delegates_.push_back (d);
        }

      protected:
        virtual void
        parameter_delegate (SyntaxTree::OperationDeclPtr const& op)
        {
          SyntaxTree::CommaPtr comma (new SyntaxTree::Comma);

          for (SyntaxTree::OperationDecl::Iterator n = op->begin ();
               n != op->end ();
               n++)
          {
            bool need_comma = n + 1 != op->end ();

            if (parameter_delegates_.empty ())
            {
              dispatch (*n);
              if (need_comma) dispatch (comma);
            }
            else
            {
              for (DispatcherList::const_iterator i =
                     parameter_delegates_.begin ();
                   i != parameter_delegates_.end ();
                   i++)
              {
                (*i)->dispatch (*n);
                if (need_comma) (*i)->dispatch (comma);
              }
            }
          }
        }

      protected:
        Dispatcher* type_;
        DispatcherList parameter_delegates_;
      };


      //
      //
      //
      struct AttributeDecl : Traverser
      {
        typedef
        SyntaxTree::AttributeDeclPtr
        NodePtr;

        AttributeDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::AttributeDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::AttributeDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct OperationParameter : Traverser
      {
        typedef
        SyntaxTree::OperationParameterPtr
        NodePtr;

        OperationParameter (Dispatcher* in,
                            Dispatcher* out,
                            Dispatcher* inout)
            : in_ (in), out_ (out), inout_ (inout)
        {
          map (typeid (SyntaxTree::OperationParameter), this);
        }

        OperationParameter ()
            : in_ (0), out_ (0), inout_ (0)
        {
          map (typeid (SyntaxTree::OperationParameter), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::OperationParameter> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* in_;
        Dispatcher* out_;
        Dispatcher* inout_;
      };


      //
      //
      //
      struct OperationDecl : OperationTraverser
      {
        typedef
        SyntaxTree::OperationDeclPtr
        NodePtr;

        OperationDecl (Dispatcher* type = 0)
            : OperationTraverser (type)
        {
          map (typeid (SyntaxTree::OperationDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::OperationDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        name (NodePtr const&);

        virtual void
        parameters (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_OPERATION_HPP
