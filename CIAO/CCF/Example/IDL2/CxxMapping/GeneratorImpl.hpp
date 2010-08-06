// file      : Example/IDL2/CxxMapping/GeneratorImpl.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_IMPL_HPP
#define EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_IMPL_HPP

#include <iostream>

#include "CCF/CodeGenerationKit/IndentationCxx.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include "CCF/IDL2/SemanticGraph.hpp"
#include "CCF/IDL2/Traversal.hpp"

namespace IDL2
{
  using namespace CCF::IDL2;

  using std::cout;
  using std::endl;

  struct Inhibitor
  {
    virtual void
    operator() () const = 0;
  };

  template <typename O, typename S, typename T>
  class InhibitorTemplate : public Inhibitor
  {
  public:
    typedef void (O::*F) (S&, T&);

    InhibitorTemplate (O& o, F f, S& s, T& t)
        : o_ (o), f_ (f), s_ (s), t_ (t)
    {
    }

    virtual void
    operator() () const
    {
      (o_.*f_)(s_, t_);
    }

  private:
    O& o_;
    F f_;
    S& s_;
    T& t_;
  };

  template <typename O,
            typename S,
            typename T = Traversal::EdgeDispatcherBase>
  class Factory
  {
  public:
    typedef void (O::*F) (S&, T&);

    Factory (O& o, F f, T& t)
        : o_ (o), f_ (f), t_ (t)
    {
    }

    InhibitorTemplate<O, S, T>
    operator() (S& s) const
    {
      return InhibitorTemplate<O, S, T> (o_, f_, s, t_);
    }

  private:
    O& o_;
    F f_;
    T& t_;
  };

  std::ostream&
  operator<< (std::ostream& os, Inhibitor const& p)
  {
    p ();
    return os;
  }

  class GeneratorImpl
  {
  protected:
    // Layer 1
    //

    // Layer 2
    //

    //--

    // Layer 3
    //

    //--

    struct InterfaceHdr : Traversal::UnconstrainedInterface
    {
      virtual void
      pre (Type&)
      {
        cout << "class ";
      }

      virtual void
      name (Type& i)
      {
        cout << i.name ();
      }

      virtual void
      inherits_pre (Type&)
      {
        cout << " : ";
      }

      virtual void
      inherits_none (Type&)
      {
        cout << " : public virtual CORBA::Impl::Stub";
      }

      virtual void
      names_pre (Type&)
      {
        cout << "{"
             << "public:" << endl;
      }

      virtual void
      names_post (Type&)
      {
        cout << "}";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };


    struct Module : Traversal::Module
    {
      virtual void
      pre (Type& m)
      {
        cout << "namespace " << m.name ();
      }

      virtual void
      names_pre (Type& m)
      {
        cout << "{";
      }

      virtual void
      names_post (Type& m)
      {
        cout << "}";
      }
    };

    struct TypeName : Traversal::Nameable,
                      Traversal::FundamentalType
    {
      virtual void
      traverse (SemanticGraph::Nameable& n)
      {
        cout << n.scoped_name ();
      }

      virtual void
      traverse (SemanticGraph::FundamentalType& ft)
      {
        cout << ft.name ();
      }
    };

    // Layer 4
    //

    //--


    struct OperationHdr : Traversal::Operation
    {
      virtual void
      name (Type& o)
      {
        cout << " " << o.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        cout << " (";
      }

      virtual void
      receives_post (Type&)
      {
        cout << ")";
      }

      virtual void
      raises_pre (Type&)
      {
        //FUZZ: disable check_for_exception_sepc
        cout << " throw (";
        //FUZZ: enable check_for_exception_sepc
      }

      virtual void
      raises_post (Type&)
      {
        cout << ")";
      }

      virtual void
      post (Type&)
      {
        cout << ";";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }
    };

    struct OperationImpl : Traversal::Operation
    {
      OperationImpl ()
          : marshal (*this, &OperationImpl::receives, marshal_),
            unmarshal (*this, &OperationImpl::receives, unmarshal_),
            ret_type (*this, &OperationImpl::returns, ret_type_),
            cast_type (*this, &OperationImpl::returns, cast_type_)
      {
      }

      virtual void
      name (Type& o)
      {
        cout << " "
             << o.scoped_name ().scope_name ().simple_name ()
             << "::" << o.name ();
      }

      virtual void
      receives_pre (Type&)
      {
        cout << " (";
      }

      virtual void
      receives_post (Type&)
      {
        cout << ")";
      }

      virtual void
      raises_pre (Type&)
      {
        //FUZZ: disable check_for_exception_sepc
        cout << " throw (";
        //FUZZ: enable check_for_exception_sepc
      }

      virtual void
      raises_post (Type&)
      {
        cout << ")";
      }

      virtual void
      post (Type& o)
      {
        cout << "{"
             << "OCDRStream os;"
             << "static_cast<void> (os" << marshal (o) << ");"
             << "ICDRStream is (Stub::make_call (\""
             << o.name () << "\", os));"
             << ret_type (o) << " _retval;"
             << "is >> _retval" << unmarshal (o) << ";"
             << "return " << "static_cast<" << cast_type (o) << "> (_retval);"
             << "}";
      }

      virtual void
      comma (Type&)
      {
        cout << ", ";
      }

      struct MarshalParameter : Traversal::Receives,
                                Traversal::InParameter,
                                Traversal::InOutParameter
      {
        MarshalParameter ()
        {
          node_traverser (*this);
        }

        virtual void
        traverse (SemanticGraph::InParameter& p)
        {
          cout << " << " << p.name ();
        }

        virtual void
        traverse (SemanticGraph::InOutParameter& p)
        {
          cout << " << " << p.name ();
        }
      } marshal_;

      Factory<OperationImpl, Type> marshal;

      struct UnmarshalParameter : Traversal::Receives,
                                  Traversal::InOutParameter,
                                  Traversal::OutParameter
      {
        UnmarshalParameter ()
        {
          node_traverser (*this);
        }

        virtual void
        traverse (SemanticGraph::InOutParameter& p)
        {
          cout << " >> " << p.name ();
        }

        virtual void
        traverse (SemanticGraph::OutParameter& p)
        {
          cout << " >> " << p.name ();
        }
      } unmarshal_;

      Factory<OperationImpl, Type> unmarshal;


      struct ReturnType : Traversal::Belongs, Traversal::Void, TypeName
      {
        ReturnType ()
        {
          node_traverser (*this);
        }

        virtual void
        traverse (SemanticGraph::Void&)
        {
          // Void is a type for which do-nothing operator>> (ICDRStream&)
          // is provided. Its main purpose is to make C++ type handling
          // more regular and thus allow more streamlined code generation.
          //
          cout << "Void";
        }
      } ret_type_;

      Factory<OperationImpl, Type> ret_type;

      struct CastType : Traversal::Belongs, TypeName
      {
        CastType ()
        {
          node_traverser (*this);
        }
      } cast_type_;

      Factory<OperationImpl, Type> cast_type;
    };

    // Layer 5
    //

    //--

    struct Parameter : Traversal::Parameter
    {
      virtual void
      name (Type& p)
      {
        cout << " " << p.name ();
      }
    };

  public:

    GeneratorImpl ()
    {
      // Layer 1
      //
      unit.edge_traverser (contains_principal);

      //--

      contains_principal.node_traverser (region);

      // Layer 2
      //
      region.edge_traverser (contains_root);

      //--

      contains_root.node_traverser (root);

      // Layer 3
      //
      root.edge_traverser (defines);

      //--
      defines.node_traverser (interface_hdr);
      defines.node_traverser (interface_impl);
      defines.node_traverser (module);

      // Layer 4
      //
      module.edge_traverser (defines);

      interface_hdr.edge_traverser (inherits);
      interface_hdr.edge_traverser (interface_hdr_defines);

      interface_impl.edge_traverser (interface_impl_defines);

      //--
      inherits.node_traverser (type_name);

      interface_hdr_defines.node_traverser (operation_hdr);
      interface_impl_defines.node_traverser (operation_impl);

      // Layer 5
      //

      operation_hdr.edge_traverser (receives);
      operation_hdr.edge_traverser (returns);

      operation_impl.edge_traverser (receives);
      operation_impl.edge_traverser (returns);

      //--

      receives.node_traverser (parameter);
      returns.node_traverser (type_name);

      // Layer 6
      //
      parameter.edge_traverser (belongs);

      //--

      belongs.node_traverser (type_name);

    }

  protected:
    Traversal::TranslationUnit unit;

    // Layer 1
    //
    Traversal::ContainsPrincipal contains_principal;

    //--

    Traversal::TranslationRegion region;

    // Layer 2
    //
    Traversal::ContainsRoot contains_root;

    //--

    Traversal::Root root;

    // Layer 3
    //
    Traversal::Defines defines;

    //--
    InterfaceHdr interface_hdr;
    Traversal::UnconstrainedInterface interface_impl;

    Module module;

    TypeName type_name;

    // Layer 4
    //
    Traversal::Inherits inherits;
    Traversal::Defines interface_hdr_defines;
    Traversal::Defines interface_impl_defines;

    //--
    OperationHdr operation_hdr;
    OperationImpl operation_impl;

    // Layer 5
    //
    Traversal::Receives receives;
    Traversal::Returns returns;

    //--

    Parameter parameter;

    // Layer 6
    //
    Traversal::Belongs belongs;

  public:
    void
    generate (CCF::IDL2::SemanticGraph::TranslationUnit& tu)
    {
      // Plug automatic IDL indenter.
      //
      Indentation::Implanter<Indentation::Cxx> guard (cout);

      unit.traverse (tu);
    }
  };
}

#endif  // EXAMPLE_IDL2_CXX_MAPPING_GENERATOR_IMPL_HPP
