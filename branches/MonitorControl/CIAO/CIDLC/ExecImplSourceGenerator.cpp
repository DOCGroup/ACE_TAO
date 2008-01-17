// file      : CIDLC/ExecImplSourceGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecImplSourceGenerator.hpp"
#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"
#include "ParameterEmitter_T.hpp"
#include "AttributeSourceEmitters_T.hpp"
#include "CompositionEmitter.hpp"
#include "ModuleEmitter.hpp"
#include "InterfaceEmitter.hpp"
#include "Literals.hpp"

#include <ostream>
#include <sstream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

namespace
{
  template <typename T>
  struct OperationEmitter : Traversal::Operation, EmitterBase
  {
    OperationEmitter (Context& c, T& scope)
      : EmitterBase (c),
        scope_ (scope)
    {}

    virtual void
    name (Type& o)
    {
      os << endl
         << scope_.name () << "_exec_i::" << o.name ();
    }

    virtual void
    receives_pre (Type&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_none (Type&)
    {
      os << " ()" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << ")" << endl;
    }

    virtual void
    raises (Type&)
    {
    }

    virtual void
    post (Type& o)
    {
      os << "{"
         << STRS[YCH] << endl;

      Traversal::Operation operation_emitter;

      Traversal::Belongs returns;
      operation_emitter.edge_traverser (returns);

      NullReturnEmitter return_emitter (ctx);
      returns.node_traverser (return_emitter);

      operation_emitter.traverse (o);

      os << "}";
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }

  protected:
    T& scope_;
  };

  struct FacetEmitter : Traversal::UnconstrainedInterface,
                        EmitterBase
  {
    FacetEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      if (i.context ().count ("facet_impl_src_gen")) return;

      os << STRS[SEP] << endl
         << "// Facet Executor Implementation Class:   "
         << i.name () << "_exec_i" << endl
         << STRS[SEP] << endl << endl;

      os << i.name () << "_exec_i::"
         << i.name () << "_exec_i (void)" << endl
         << "{"
         << "}"
         << i.name () << "_exec_i::~"
         << i.name () << "_exec_i (void)" << endl
         << "{"
         << "}"
         << "// Operations from " << i.scoped_name () << endl << endl;

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        bool exec_src = true;
        AttributeSourceEmitter<UnconstrainedInterface> attribute_emitter (
            ctx,
            i,
            exec_src
          );
        defines_.node_traverser (attribute_emitter);

        OperationEmitter<UnconstrainedInterface> operation_emitter (ctx, i);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        bool for_exec_src = true;
        ParameterEmitter<Traversal::InParameter> in_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::InOutParameter> inout_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::OutParameter> out_param (
            ctx,
            for_exec_src
          );
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        inherits (i, inherits_);
        names (i, defines_);
      }

      i.context ().set ("facet_impl_src_gen", true);
    }
  };

  struct ExecPortsEmitter : Traversal::ConsumerData,
                            Traversal::ProviderData,
                            EmitterBase
  {
    ExecPortsEmitter (Context& c, SemanticGraph::Component& scope)
      : EmitterBase (c),
        enclosing_type_name_emitter_ (c),
        simple_type_name_emitter_ (c),
        type_name_emitter_ (c),
        scope_ (scope)
    {
      enclosing_belongs_.node_traverser (enclosing_type_name_emitter_);
      simple_belongs_.node_traverser (simple_type_name_emitter_);
      belongs_.node_traverser (type_name_emitter_);
    }

    virtual void
    traverse (SemanticGraph::Provider& p)
    {
      Traversal::ProviderData::belongs (p, enclosing_belongs_);

      os << "::CCM_";

      Traversal::ProviderData::belongs (p, simple_belongs_);

      os << "_ptr" << endl
         << scope_.name () << "_exec_i::get_"
         << p.name () << " ()" << endl
         << "{"
         << STRS[YCH] << endl
         << "return ";

      Traversal::ProviderData::belongs (p, enclosing_belongs_);

      os << "::CCM_";

      Traversal::ProviderData::belongs (p, simple_belongs_);

      os << "::_nil ();" << endl
         << "}";
    }

    virtual void
    traverse (SemanticGraph::Consumer& c)
    {
      os << "void" << endl
         << scope_.name () << "_exec_i::push_"
         << c.name () << " (" << endl;

      Traversal::ConsumerData::belongs (c, belongs_);

      os << " * /* ev */"
         << ")" << endl
         << "{"
         << STRS[YCH] << endl
         << "}";
   }

  private:
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    FullTypeNameEmitter type_name_emitter_;
    Traversal::Belongs enclosing_belongs_;
    Traversal::Belongs simple_belongs_;
    Traversal::Belongs belongs_;
    SemanticGraph::Component& scope_;
  };

  struct ComponentExecImplEmitter : Traversal::Component, EmitterBase
  {
    ComponentExecImplEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      // Generate the facet executor class(es) first.
      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);
        component_emitter.edge_traverser (component_inherits);

        Traversal::Provider provider;
        defines.node_traverser (provider);

        Traversal::Belongs belongs;
        provider.edge_traverser (belongs);

        FacetEmitter facet_emitter (ctx);
        belongs.node_traverser (facet_emitter);

        component_emitter.traverse (t);
      }

      os << STRS[SEP] << endl
         << "// Component Executor Implementation Class:   "
         << t.name () << "_exec_i" << endl
         << STRS[SEP] << endl << endl;

      os << t.name () << "_exec_i::"
         << t.name () << "_exec_i (void)" << endl
         << "{"
         << "}"
         << t.name () << "_exec_i::~"
         << t.name () << "_exec_i (void)" << endl
         << "{"
         << "}";

      os << "// Supported or inherited operations." << endl << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        Traversal::Supports supports_;
        component_emitter.edge_traverser (component_inherits);
        component_emitter.edge_traverser (supports_);
        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits interface_inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (interface_inherits);

        bool exec_src = true;
        AttributeSourceEmitter<SemanticGraph::Component> attribute_emitter (
            ctx,
            t,
            exec_src
          );
        defines.node_traverser (attribute_emitter);

        OperationEmitter<SemanticGraph::Component> operation_emitter (ctx, t);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        bool for_exec_src = true;
        ParameterEmitter<Traversal::InParameter> in_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::InOutParameter> inout_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::OutParameter> out_param (
            ctx,
            for_exec_src
          );
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        component_emitter.traverse (t);
      }

      os << "// Attribute operations." << endl << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        component_emitter.edge_traverser (component_inherits);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);

        bool exec_src = true;
        AttributeSourceEmitter<SemanticGraph::Component> attribute_emitter (
            ctx,
            t,
            exec_src
          );
        defines.node_traverser (attribute_emitter);

        component_emitter.traverse (t);
      }

      os << "// Port operations." << endl << endl;

      {
        Traversal::Component component_emitter;

        Traversal::Inherits component_inherits;
        component_inherits.node_traverser (component_emitter);

        component_emitter.edge_traverser (component_inherits);

        Traversal::Defines defines;
        component_emitter.edge_traverser (defines);

        ExecPortsEmitter exec_ports_emitter (ctx, t);
        defines.node_traverser (exec_ports_emitter);

        component_emitter.traverse (t);
      }

      os << "// Operations from Components::SessionComponent"
         << endl << endl;

      os << "void" << endl
         << t.name () << "_exec_i::set_session_context (" << endl
         << STRS[COMP_SC] << "_ptr ctx)" << endl
         << "{"
         << "this->context_ =" << endl
         << "  " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_Context::_narrow ("
         << "ctx);" 
         << endl;

      os << "if (CORBA::is_nil (this->context_.in ()))" << endl
         << "{"
         << "throw ::CORBA::INTERNAL ();"
         << "}"
         << "}";

      os << "void" << endl
         << t.name () << "_exec_i::ciao_preactivate ()" << endl
         << "{"
         << STRS[YCH]
         << "}";

      os << "void" << endl
         << t.name () << "_exec_i::ciao_postactivate ()" << endl
         << "{"
         << STRS[YCH]
         << "}";

      os << "void" << endl
         << t.name () << "_exec_i::ccm_activate ()" << endl
         << "{"
         << STRS[YCH]
         << "}";

      os << "void" << endl
         << t.name () << "_exec_i::ccm_passivate ()" << endl
         << "{"
         << STRS[YCH]
         << "}";

      os << "void" << endl
         << t.name () << "_exec_i::ccm_remove ()" << endl
         << "{"
         << STRS[YCH]
         << "}";
    }

    virtual void
    post (Type&)
    {
    }
  };

  // HomeFactory and HomeFinder are tied to Operation in
  // the front end. Since we want to treat them differently
  // than regular operations in a home (we want to generate
  // base class factory operations returning the base component,
  // for example), we use this class for regular home operations
  // that overrides HomeFactory and HomeFinder traversals
  // to do nothing.
  struct HomeOperationEmitter : OperationEmitter<SemanticGraph::Home>,
                                Traversal::HomeFactory,
                                Traversal::HomeFinder
  {
    HomeOperationEmitter (Context& c, SemanticGraph::Home& scope)
      : OperationEmitter<SemanticGraph::Home> (c, scope)
    {}

    virtual void
    traverse (SemanticGraph::HomeFactory&)
    {
    }

    virtual void
    traverse (SemanticGraph::HomeFinder&)
    {
    }
  };

  struct FactoryOperationEmitter : Traversal::HomeFactory,
                                   EmitterBase
  {
    FactoryOperationEmitter (Context& c, SemanticGraph::Home& scope)
      : EmitterBase (c),
        enclosing_type_name_emitter_ (c),
        simple_type_name_emitter_ (c),
        scope_ (scope)
    {
      enclosing_returns_.node_traverser (enclosing_type_name_emitter_);
      simple_returns_.node_traverser (simple_type_name_emitter_);
    }

    virtual void
    returns (SemanticGraph::HomeFactory&)
    {
      os << STRS[COMP_EC] << "_ptr" << endl;
    }

    virtual void
    name (SemanticGraph::HomeFactory& hf)
    {
      os << scope_.name () << "_exec_i::" << hf.name ();
    }

    virtual void
    receives_none (SemanticGraph::HomeFactory&)
    {
      os << " ()" << endl;
    }

    virtual void
    receives_pre (SemanticGraph::HomeFactory&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_post (SemanticGraph::HomeFactory&)
    {
      os << endl << ")" << endl;
    }

    virtual void
    raises (SemanticGraph::HomeFactory&)
    {
    }

    virtual void
    post (SemanticGraph::HomeFactory& hf)
    {
      os << "{"
         << STRS[YCH] << endl
         << "return ";

      Traversal::HomeFactory::returns (hf, enclosing_returns_);

      os << "::CCM_";

      Traversal::HomeFactory::returns (hf, simple_returns_);

      os << "::_nil ();"
         << "}";
    }

    virtual void
    comma (SemanticGraph::HomeFactory&)
    {
      os << "," << endl;
    }

  private:
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    Traversal::Returns enclosing_returns_;
    Traversal::Returns simple_returns_;
    SemanticGraph::Home& scope_;
  };

  struct FinderOperationEmitter : Traversal::HomeFinder,
                                  EmitterBase
  {
    FinderOperationEmitter (Context& c, SemanticGraph::Home& scope)
      : EmitterBase (c),
        enclosing_type_name_emitter_ (c),
        simple_type_name_emitter_ (c),
        scope_ (scope)
    {
      enclosing_returns_.node_traverser (enclosing_type_name_emitter_);
      simple_returns_.node_traverser (simple_type_name_emitter_);
    }

    virtual void
    returns (SemanticGraph::HomeFinder&)
    {
      os << STRS[COMP_EC] << "_ptr" << endl;
    }

    virtual void
    name (SemanticGraph::HomeFinder& hf)
    {
      os << scope_.name () << "_exec_i::" << hf.name ();
    }

    virtual void
    receives_none (SemanticGraph::HomeFinder&)
    {
      os << " ()" << endl;
    }

    virtual void
    receives_pre (SemanticGraph::HomeFinder&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_post (SemanticGraph::HomeFinder&)
    {
      os << endl << ")" << endl;
    }

    virtual void
    raises (SemanticGraph::HomeFinder&)
    {
    }

    virtual void
    post (SemanticGraph::HomeFinder& hf)
    {
      os << "{"
         << STRS[YCH] << endl
         << "return ";

      Traversal::HomeFinder::returns (hf, enclosing_returns_);

      os << "::CCM_";

      Traversal::HomeFinder::returns (hf, simple_returns_);

      os << "::_nil ();"
         << "}";
    }

    virtual void
    comma (SemanticGraph::HomeFinder&)
    {
      os << "," << endl;
    }

  private:
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    Traversal::Returns enclosing_returns_;
    Traversal::Returns simple_returns_;
    SemanticGraph::Home& scope_;
  };

  struct HomeExecImplEmitter : Traversal::Home, EmitterBase
  {
    HomeExecImplEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << STRS[SEP] << endl
         << "// Home Executor Implementation Class:   "
         << t.name () << "_exec_i" << endl
         << STRS[SEP] << endl << endl;

      os << t.name () << "_exec_i::"
         << t.name () << "_exec_i (void)" << endl
         << "{"
         << "}"
         << t.name () << "_exec_i::~"
         << t.name () << "_exec_i (void)" << endl
         << "{"
         << "}";

      os << "// Supported or inherited operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);

        Traversal::Supports supports_;
        home_emitter.edge_traverser (home_inherits);
        home_emitter.edge_traverser (supports_);
        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits interface_inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (interface_inherits);

        bool exec_src = true;
        AttributeSourceEmitter<SemanticGraph::Home> attribute_emitter (
            ctx,
            t,
            exec_src
          );
        defines.node_traverser (attribute_emitter);

        OperationEmitter<SemanticGraph::Home> operation_emitter (ctx, t);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        bool for_exec_src = true;
        ParameterEmitter<Traversal::InParameter> in_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::InOutParameter> inout_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::OutParameter> out_param (
            ctx,
            for_exec_src
          );
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      os << "// Home operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);
        home_emitter.edge_traverser (home_inherits);

        HomeOperationEmitter home_operation_emitter (ctx, t);
        defines.node_traverser (home_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        home_operation_emitter.edge_traverser (receives);
        home_operation_emitter.edge_traverser (returns);
        home_operation_emitter.edge_traverser (raises);

        bool for_exec_src = true;
        ParameterEmitter<Traversal::InParameter> in_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::InOutParameter> inout_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::OutParameter> out_param (
            ctx,
            for_exec_src
          );
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      // Generate home factory operations. This is a separate traversal
      // stack because we want to generate the factory operations
      // of ancestors with the return type of the base component.
      os << "// Factory and finder operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits inherits;
        home_emitter.edge_traverser (inherits);
        inherits.node_traverser (home_emitter);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        FactoryOperationEmitter factory_operation_emitter (ctx, t);
        FinderOperationEmitter finder_operation_emitter (ctx, t);
        defines.node_traverser (factory_operation_emitter);
        defines.node_traverser (finder_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        factory_operation_emitter.edge_traverser (receives);
        factory_operation_emitter.edge_traverser (returns);
        factory_operation_emitter.edge_traverser (raises);
        finder_operation_emitter.edge_traverser (receives);
        finder_operation_emitter.edge_traverser (returns);
        finder_operation_emitter.edge_traverser (raises);

        bool for_exec_src = true;
        ParameterEmitter<Traversal::InParameter> in_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::InOutParameter> inout_param (
            ctx,
            for_exec_src
          );
        ParameterEmitter<Traversal::OutParameter> out_param (
            ctx,
            for_exec_src
          );
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (ctx);
        FullTypeNameEmitter type_name_emitter (ctx);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (ctx);
        INOUTArgTypeNameEmitter inout_arg_emitter (ctx);
        OUTArgTypeNameEmitter out_arg_emitter (ctx);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        home_emitter.traverse (t);
      }

      os << "// Attribute operations." << endl << endl;

      {
        Traversal::Home home_emitter;

        Traversal::Inherits home_inherits;
        home_inherits.node_traverser (home_emitter);

        home_emitter.edge_traverser (home_inherits);

        Traversal::Defines defines;
        home_emitter.edge_traverser (defines);

        bool exec_src = true;
        AttributeSourceEmitter<SemanticGraph::Home> attribute_emitter (
            ctx,
            t,
            exec_src
          );
        defines.node_traverser (attribute_emitter);

        home_emitter.traverse (t);
      }

      os << "// Implicit operations." << endl << endl
         << STRS[COMP_EC] << "_ptr" << endl
         << t.name () << "_exec_i::create ()" << endl
         << "{"
         << STRS[COMP_EC] << "_ptr retval =" << endl
         << "  " << STRS[COMP_EC] << "::_nil ();" << endl
         << "ACE_NEW_THROW_EX (" << endl
         << "retval," << endl;

       Traversal::Home home_emitter;

       Traversal::Manages manages_;
       home_emitter.edge_traverser (manages_);

       SimpleTypeNameEmitter manages_emitter (ctx);
       manages_.node_traverser (manages_emitter);

       home_emitter.traverse (t);

       os << "_exec_i," << endl
          << "::CORBA::NO_MEMORY ());"
          << endl
          << "return retval;"
          << "}";
    }

    virtual void
    post (Type& t)
    {
      // We need to escape C++ keywords before flattening the name.
      //
      string name;
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << t.scoped_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::Components::HomeExecutorBase_ptr" << endl
         << "create" << name << "_Impl (void)" << endl
         << "{"
         << "::Components::HomeExecutorBase_ptr retval =" << endl
         << "  ::Components::HomeExecutorBase::_nil ();" << endl
         << "ACE_NEW_RETURN (" << endl
         << "retval," << endl
         << t.name () << "_exec_i," << endl
         << "::Components::HomeExecutorBase::_nil ());" << endl
         << "return retval;"
         << "}";
    }
  };
}

ExecImplSourceEmitter::ExecImplSourceEmitter (std::ostream& os_,
                                              CommandLine const& cl,
                                              string export_macro,
                                              fs::path const& file)
  : os (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    file_ (file)
{}

void
ExecImplSourceEmitter::pre (TranslationUnit&)
{
  os << COPYRIGHT;

  string file_name ("");

  if (! file_.empty ())
  {
    file_name = file_.leaf ();
  }

  string file_suffix = cl_.get_value ("exec-hdr-file-suffix",
                                      "_exec.h");

  file_name = regex::perl_s (file_name,
                             "/^(.+?)(\\.(idl|cidl|cdl))?$/$1"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"ciao/CIAO_common.h\"" << endl << endl;
}

void
ExecImplSourceEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_, cl_);

  Traversal::TranslationUnit unit;

  // Layer 1
  //
  Traversal::ContainsPrincipal contains_principal;
  unit.edge_traverser (contains_principal);

  //--
  Traversal::TranslationRegion principal_region;
  contains_principal.node_traverser (principal_region);

  // Layer 2
  //
  Traversal::ContainsRoot contains_root;
  principal_region.edge_traverser (contains_root);

  //--
  Traversal::Root root;
  contains_root.node_traverser (root);

  // Layer 3
  //
  Traversal::Defines defines;
  root.edge_traverser (defines);

  //--
  ModuleEmitter module (c);
  CompositionEmitter composition (c);
  defines.node_traverser (module);
  defines.node_traverser (composition);

  // Layer 4
  //
  composition.edge_traverser (defines);
  module.edge_traverser (defines);

  //--
  Traversal::ComponentExecutor component_executor;
  Traversal::HomeExecutor home_executor;
  defines.node_traverser (component_executor);
  defines.node_traverser (home_executor);

  // Layer 5
  //
  Traversal::Implements implements;
  component_executor.edge_traverser (implements);
  home_executor.edge_traverser (implements);

  //--
  ComponentExecImplEmitter component_exec_impl_emitter (c);
  HomeExecImplEmitter home_exec_impl_emitter (c);
  implements.node_traverser (component_exec_impl_emitter);
  implements.node_traverser (home_exec_impl_emitter);

  unit.traverse (u);

  post (u);
}

void
ExecImplSourceEmitter::post (TranslationUnit&)
{
}

