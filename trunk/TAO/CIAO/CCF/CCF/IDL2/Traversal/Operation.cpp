// file      : CCF/IDL2/Traversal/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/Traversal/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      
      // AttributeDecl
      //
      //

      void AttributeDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void AttributeDecl::
      pre (NodePtr const&)
      {
      }

      void AttributeDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void AttributeDecl::
      post (NodePtr const&)
      {
      }


      // OperationParameter
      //
      //

      void  OperationParameter::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }

      void OperationParameter::
      pre (NodePtr const&)
      {
      }

      void OperationParameter::
      type (NodePtr const& n)
      {
        Dispatcher* d = this;

        switch (n->direction ())
        {
        case SyntaxTree::OperationParameter::Direction::IN:
          {
            if (in_) d = in_;
            break;
          }
        case SyntaxTree::OperationParameter::Direction::OUT:
          {
            if (out_) d = out_;
            break;
          }
        case SyntaxTree::OperationParameter::Direction::INOUT:
          {
            if (inout_) d = inout_;
            break;
          }
        }

        d->dispatch (n->type ());
      }

      void OperationParameter::
      post (NodePtr const&)
      {
      }

      // OperationDecl
      //
      //

      void OperationDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          name (n);
          parameters (n);
          post (n);
        }
      }


      void OperationDecl::
      pre (NodePtr const&)
      {
      }

      void OperationDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void OperationDecl::
      name (NodePtr const&)
      {
      }

      void OperationDecl::
      parameters (NodePtr const& n)
      {
        parameter_delegate (n);
      }

      void OperationDecl::
      post (NodePtr const&)
      {
      }

    }
  }
}
