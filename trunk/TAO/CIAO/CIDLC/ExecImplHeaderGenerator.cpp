// file      : CIDLC/ExecImplHeaderGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecImplHeaderGenerator.hpp"
#include "TypeNameEmitter.hpp"
#include "Literals.hpp"

#include <ostream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using std::endl;

using namespace StringLiterals;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
  // On some platforms toupper can be something other than a
  // function.
  int
  upcase (int c)
  {
    return std::toupper (c);
  }
}

namespace
{
  class Context
  {
  public:
    Context (std::ostream& os,
             string export_macro)
        : os_ (os),
          export_macro_ (export_macro)
    {
    }

  std::ostream&
  os ()
  {
    return os_;
  }

  string
  export_macro ()
  {
    return export_macro_;
  }

  private:
    std::ostream& os_;
    string export_macro_;
  };

  class EmitterBase
  {
  public:
    EmitterBase (Context& c)
      : ctx (c),
        os (ctx.os ())
    {
    }

  protected:
    Context& ctx;
    std::ostream& os;
  };

  struct ModuleEmitter : Traversal::Module, EmitterBase
  {
    ModuleEmitter (Context& c)
        : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << endl
         << "namespace " << t.name () << "{";
    }

    virtual void
    post (Type&)
    {
      os << "}";
    }
  };

  struct CompositionEmitter : Traversal::Composition, EmitterBase
  {
    CompositionEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << endl
         << "namespace " << t.name () << "{";
    }

    virtual void
    post (Type&)
    {
      os << "}";
    }
  };

  struct OperationEmitter : Traversal::Operation, EmitterBase
  {
    OperationEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    pre (Type&)
    {
      os << "virtual ";
    }

    virtual void
    name (Type& o)
    {
      os << endl << o.name ();
    }

    virtual void
    receives_pre (Type&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_none (Type&)
    {
      os << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_HDR] << ")" << endl;
    }

    virtual void
    raises_pre (Type&)
    {
      os << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl;
    }

    virtual void
    raises_none (Type&)
    {
      os << STRS[EXCP_SNGL];
    }

    virtual void
    raises_post (Type&)
    {
      os << "))";
    }

    virtual void
    post (Type&)
    {
      os << ";" << endl;
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }
  };

  // For generating parameter names.
  template <typename T>
  struct ParameterEmitter : T
  {
    ParameterEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    name (typename T::Type& p)
    {
      os << " " << p.name ();
    }

  private:
    std::ostream& os;
  };

  // Generic scoped typename emitter used by various other emitters.
  struct TypeNameEmitter : Traversal::Type
  {
    TypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.scoped_name ();
    }

  private:
    std::ostream& os;
  };

  // Generic local typename emitter used by various other emitters.
  struct SimpleTypeNameEmitter : Traversal::Type
  {
    SimpleTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.name ();
    }

  private:
    std::ostream& os;
  };

  // Generate name of type's enclosing scope.
  struct EnclosingTypeNameEmitter : Traversal::Type
  {
    EnclosingTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      os << t.scoped_name ().scope_name ();
    }

  private:
    std::ostream& os;
  };

  // Emits typename minus the leading double colon.
  struct StrippedTypeNameEmitter : Traversal::Type
  {
    StrippedTypeNameEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (SemanticGraph::Type& t)
    {
      ScopedName scoped (t.scoped_name ());
      os << Name (scoped.begin () + 1, scoped.end ());
    }

  private:
    std::ostream& os;
  };

  // Generates operations associated with attributes.
  // @@@ (JP) Need to support exceptions.
  struct AttributeEmitter : Traversal::ReadWriteAttribute,
                            EmitterBase
  {
    AttributeEmitter (Context& c)
      : EmitterBase (c),
        write_type_name_emitter_ (c.os ()),
        read_type_name_emitter_ (c.os ())
    {
      write_belongs_.node_traverser (write_type_name_emitter_);
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadWriteAttribute& a)
    {
      os << "virtual ";

      Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

      os << endl
         << a.name () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl;

      os << "virtual void" << endl
         << a.name () << " (" << endl;

      Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

      os << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl;
    }

  private:
    INArgTypeNameEmitter write_type_name_emitter_;
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs write_belongs_;
    Traversal::Belongs read_belongs_;
  };

  // Generates operations associated with readonly attributes.
  // @@@ (JP) Need to support exceptions.
  struct ReadOnlyAttributeEmitter : Traversal::ReadAttribute,
                                    EmitterBase
  {
    ReadOnlyAttributeEmitter (Context& c)
      : EmitterBase (c),
        read_type_name_emitter_ (c.os ())
    {
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadAttribute& a)
    {
      os << "virtual ";

      Traversal::ReadAttribute::belongs (a, read_belongs_);

      os << endl
         << a.name () << " (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl;
    }

  private:
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs read_belongs_;
  };

  struct InterfaceEmitter : Traversal::UnconstrainedInterface,
                            EmitterBase
  {
    InterfaceEmitter (Context& c)
      : EmitterBase (c)
    {}

    bool
    add (UnconstrainedInterface& i)
    {
      return interfaces_.insert (&i).second;
    }

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      if (add (i))
      {
        Traversal::UnconstrainedInterface::traverse (i);
      }
    }

  private:
    std::set<UnconstrainedInterface*> interfaces_;
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
      if (i.context ().count ("facet_impl_hdr_gen")) return;
    
      os << "class " << ctx.export_macro () << " "
         << i.name () << "_exec_i" << endl
         << ": public virtual " << i.scoped_name ().scope_name ()
         << "::CCM_" << i.name () << "," << endl
         << "public virtual TAO_Local_RefCounted_Object" << endl
         << "{"
         << "public:" << endl
         << i.name () << "_exec_i (void);"
         << "virtual ~" << i.name () << "_exec_i (void);" << endl
         << "// Operations from " << i.scoped_name () << endl << endl;

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines_.node_traverser (attribute_emitter);
        defines_.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
        in_belongs.node_traverser (in_arg_emitter);
        inout_belongs.node_traverser (inout_arg_emitter);
        out_belongs.node_traverser (out_arg_emitter);

        inherits (i, inherits_);
        names (i, defines_);
      }
      
      os << "};";
      
      i.context ().set ("facet_impl_hdr_gen", true);
    }
  };

  struct ExecPortsEmitter : Traversal::ConsumerData,
                            Traversal::ProviderData,
                            EmitterBase
  {
    ExecPortsEmitter (Context& c)
      : EmitterBase (c),
        enclosing_type_name_emitter_ (c.os ()),
        simple_type_name_emitter_ (c.os ()),
        type_name_emitter_ (c.os ())
    {
      enclosing_belongs_.node_traverser (enclosing_type_name_emitter_);
      simple_belongs_.node_traverser (simple_type_name_emitter_);
      belongs_.node_traverser (type_name_emitter_);
    }

    virtual void
    traverse (SemanticGraph::Provider& p)
    {
      os << "virtual ";

      Traversal::ProviderData::belongs (p, enclosing_belongs_);
      
      os << "::CCM_";

      Traversal::ProviderData::belongs (p, simple_belongs_);
      
      os << "_ptr" << endl
          << "get_" << p.name () << " (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl
          << STRS[EXCP_SNGL] << ";" << endl;
    }

    virtual void
    traverse (SemanticGraph::Consumer& c)
    {
      // @@@ (JP) May need to generate this for the eventtype's ancestors
      // as well (the spec is vague on this point). If so, we need the
      // CIDL compiler to support valuetype/eventtype inheritance.
      os << "virtual void" << endl
          << "push_" << c.name () << " (" << endl;

      Traversal::ConsumerData::belongs (c, belongs_);

      os << " *ev" << endl
          << STRS[ENV_HDR] << ")" << endl
          << STRS[EXCP_SNGL] << ";" << endl;
   }

  private:
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    TypeNameEmitter type_name_emitter_;
    Traversal::Belongs enclosing_belongs_;
    Traversal::Belongs simple_belongs_;
    Traversal::Belongs belongs_;
  };

  struct ComponentExecImplEmitter : Traversal::Component, EmitterBase
  {
    ComponentExecImplEmitter (Context& c, ComponentExecutor& exec)
      : EmitterBase (c),
        exec_ (exec)
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

      os << "class " << ctx.export_macro () << " "
         << t.name () << "_exec_i" << endl
         << ": public virtual " << exec_.name () << "," << endl
         << "public virtual TAO_Local_RefCounted_Object" << endl
         << "{"
         << "public:" << endl
         << t.name () << "_exec_i (void);"
         << "virtual ~" << t.name () << "_exec_i (void);" << endl;

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

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
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

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

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

        ExecPortsEmitter exec_ports_emitter (ctx);
        defines.node_traverser (exec_ports_emitter);

        component_emitter.traverse (t);
      }
      
      os << "// Operations from Components::SessionComponent" << endl << endl;

      os << "virtual void" << endl
         << "set_session_context (" << endl
         << STRS[COMP_SC] << "_ptr ctx" << endl
         << STRS[ENV_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ciao_preactivate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ciao_postactivate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_activate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_passivate (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_remove (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;
    }

    virtual void
    post (Type& t)
    {
      os << "protected:" << endl
         << "CIAO_GLUE"
         << regex::perl_s (t.scoped_name ().scope_name ().str (),
                           "/::/_/")
         << "::" << t.name () << "_Context *context_;" << endl
         << "};";
    }
    
  private:
    ComponentExecutor& exec_;
  };

  struct ComponentExecutorEmitter : Traversal::ComponentExecutor, EmitterBase
  {
    ComponentExecutorEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    implements (Type& i)
    {
      Traversal::ComponentExecutor component_executor;
      
      Traversal::Implements implements;
      component_executor.edge_traverser (implements);
      
      ComponentExecImplEmitter component_exec_impl_emitter (ctx, i);
      implements.node_traverser (component_exec_impl_emitter);
      
      component_executor.traverse (i);
    }
  };
  
  // HomeFactory and HomeFinder are tied to Operation in
  // the front end. Since we want to treat them differently
  // than regular operations in a home (we want to generate
  // base class factory operations returning the base component,
  // for example), we use this class for regular home operations
  // that overrides HomeFactory and HomeFinder traversals
  // to do nothing.
  struct HomeOperationEmitter : OperationEmitter,
                                Traversal::HomeFactory,
                                Traversal::HomeFinder
  {
    HomeOperationEmitter (Context& c)
      : OperationEmitter (c)
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
                                   Traversal::Home,
                                   EmitterBase
  {
    FactoryOperationEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    pre (SemanticGraph::HomeFactory&)
    {
      os << "virtual ";
    }

    virtual void
    returns (SemanticGraph::HomeFactory&)
    {
      os << STRS[COMP_EC] << "_ptr" << endl;
    }

    virtual void
    name (SemanticGraph::HomeFactory& hf)
    {
      os << hf.name ();
    }

    virtual void
    receives_none (SemanticGraph::HomeFactory&)
    {
      os << " (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl;
    }

    virtual void
    receives_pre (SemanticGraph::HomeFactory&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_post (SemanticGraph::HomeFactory&)
    {
      os << endl << STRS[ENV_HDR] << ")" << endl;
    }

    virtual void
    raises_none (SemanticGraph::HomeFactory&)
    {
      os << STRS[EXCP_SNGL];
    }

    virtual void
    raises_pre (SemanticGraph::HomeFactory&)
    {
      os << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl;
    }

    virtual void
    raises_post (SemanticGraph::HomeFactory&)
    {
      os << "))";
    }

    virtual void
    post (SemanticGraph::HomeFactory&)
    {
      os << ";" << endl;
    }

    virtual void
    comma (SemanticGraph::HomeFactory&)
    {
      os << "," << endl;
    }
  };

  struct FinderOperationEmitter : Traversal::HomeFinder,
                                  EmitterBase
  {
    FinderOperationEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    pre (SemanticGraph::HomeFinder&)
    {
      os << "virtual ";
    }

    virtual void
    returns (SemanticGraph::HomeFinder&)
    {
      os << STRS[COMP_EC] << "_ptr" << endl;
    }

    virtual void
    name (SemanticGraph::HomeFinder& t)
    {
      os << t.name ();
    }

    virtual void
    receives_none (SemanticGraph::HomeFinder&)
    {
      os << " (" << endl
          << STRS[ENV_SNGL_HDR] << ")" << endl;
    }

    virtual void
    receives_pre (SemanticGraph::HomeFinder&)
    {
      os << " (" << endl;
    }

    virtual void
    receives_post (SemanticGraph::HomeFinder&)
    {
      os << endl << STRS[ENV_HDR] << ")" << endl;
    }

    virtual void
    raises_none (SemanticGraph::HomeFinder&)
    {
      os << STRS[EXCP_SNGL];
    }

    virtual void
    raises_pre (SemanticGraph::HomeFinder&)
    {
      os << STRS[EXCP_START] << endl
          << STRS[EXCP_SYS] << "," << endl;
    }

    virtual void
    raises_post (SemanticGraph::HomeFinder&)
    {
      os << "))";
    }

    virtual void
    post (SemanticGraph::HomeFinder&)
    {
      os << ";" << endl << endl;
    }

    virtual void
    comma (SemanticGraph::HomeFinder&)
    {
      os << "," << endl;
    }
  };

  struct HomeExecImplEmitter : Traversal::Home, EmitterBase
  {
    HomeExecImplEmitter (Context& c, HomeExecutor& exec)
      : EmitterBase (c),
        exec_ (exec)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << "class " << ctx.export_macro () << " "
         << t.name () << "_exec_i" << endl
         << ": public virtual " << exec_.name () << "," << endl
         << "public virtual TAO_Local_RefCounted_Object" << endl
         << "{"
         << "public:" << endl
         << t.name () << "_exec_i (void);"
         << "virtual ~" << t.name () << "_exec_i (void);" << endl;

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

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
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

        HomeOperationEmitter home_operation_emitter (ctx);
        defines.node_traverser (home_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        home_operation_emitter.edge_traverser (receives);
        home_operation_emitter.edge_traverser (returns);
        home_operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
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

        FactoryOperationEmitter factory_operation_emitter (ctx);
        FinderOperationEmitter finder_operation_emitter (ctx);
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

        ParameterEmitter<Traversal::InParameter> in_param (os);
        ParameterEmitter<Traversal::InOutParameter> inout_param (os);
        ParameterEmitter<Traversal::OutParameter> out_param (os);
        receives.node_traverser (in_param);
        receives.node_traverser (inout_param);
        receives.node_traverser (out_param);

        ReturnTypeNameEmitter return_type_emitter (os);
        TypeNameEmitter type_name_emitter (os);
        returns.node_traverser (return_type_emitter);
        raises.node_traverser (type_name_emitter);

        Traversal::Belongs in_belongs, inout_belongs, out_belongs;
        in_param.edge_traverser (in_belongs);
        inout_param.edge_traverser (inout_belongs);
        out_param.edge_traverser (out_belongs);

        INArgTypeNameEmitter in_arg_emitter (os);
        INOUTArgTypeNameEmitter inout_arg_emitter (os);
        OUTArgTypeNameEmitter out_arg_emitter (os);
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

        AttributeEmitter attribute_emitter (ctx);
        ReadOnlyAttributeEmitter read_only_attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        home_emitter.traverse (t);
      }

      os << "// Implicit operations." << endl << endl
         << "virtual " << STRS[COMP_EC] << "_ptr" << endl
         << "create (" << endl
         << STRS[ENV_SNGL_HDR] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;
    }

    virtual void
    post (Type& t)
    {
      os << "};";

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::Components::HomeExecutorBase_ptr" << endl
         << "create" << t.name () << "_Impl (void);" << endl;
    }
    
  private:
    HomeExecutor& exec_;
  };
  
  struct HomeExecutorEmitter : Traversal::HomeExecutor, EmitterBase
  {
    HomeExecutorEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    implements (Type& i)
    {
      Traversal::HomeExecutor home_executor;
      
      Traversal::Implements implements;
      home_executor.edge_traverser (implements);
      
      HomeExecImplEmitter home_exec_impl_emitter (ctx, i);
      implements.node_traverser (home_exec_impl_emitter);
      
      home_executor.traverse (i);
    }
  };
}

ExecImplHeaderEmitter::ExecImplHeaderEmitter (std::ostream& os_,
                                              CommandLine const& cl,
                                              string export_macro,
                                              fs::path const& file)
  : os (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    file_ (file)
{}

void
ExecImplHeaderEmitter::pre (TranslationUnit& u)
{
  os << COPYRIGHT;

  string file_name ("");

  if (!file_.empty ())
  {
    file_name = file_.leaf ();
  }

  string uc_file_name = file_name;

  std::transform (uc_file_name.begin (),
                  uc_file_name.end (),
                  uc_file_name.begin (),
                  upcase);

  string uc_file_suffix = cl_.get_value ("exec-hdr-file-suffix",
                                         "_exec.h");

  std::transform (uc_file_suffix.begin (),
                  uc_file_suffix.end (),
                  uc_file_suffix.begin (),
                  upcase);

  string guard =
      "CIAO_"
      + regex::perl_s (uc_file_name,
                       "/(\\.(IDL|CIDL))?$/" +  uc_file_suffix + "/");

  // Replace any remaining '.' with '_'.
  guard = regex::perl_s (guard, "/\\./_/");

  os << "#ifndef " << guard << endl
     << "#define " << guard << endl << endl
     << "#include /**/ \"ace/pre.h\"" << endl << endl;

  string file_suffix = cl_.get_value ("svnt-hdr-file-suffix",
                                      "_svnt.h");

  file_name = regex::perl_s (file_name,
                             "/(\\.(idl|cidl|cdl))?$/"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl << endl;

  os << "#if !defined (ACE_LACKS_PRAGMA_ONCE)" << endl
     << "# pragma once" << endl
     << "#endif /* ACE_LACKS_PRAGMA_ONCE */" << endl << endl;

  string export_include = cl_.get_value ("exec-export-include", "");

  if (!export_include.empty ())
  {
    os << "#include \"" << export_include << "\"" << endl;
  }

  os << "#include \"tao/LocalObject.h\"" << endl;
}

void
ExecImplHeaderEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_);

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
  ComponentExecutorEmitter component_executor (c);
  HomeExecutorEmitter home_executor (c);
  defines.node_traverser (component_executor);
  defines.node_traverser (home_executor);

  // Layer 5
  //
  Traversal::Implements implements;
  component_executor.edge_traverser (implements);
  home_executor.edge_traverser (implements);

  unit.traverse (u);

  post (u);
}

void
ExecImplHeaderEmitter::post (TranslationUnit& u)
{
  if (file_.empty ()) return;
  
  string uc_file_name = file_.leaf ();

  std::transform (uc_file_name.begin (),
                  uc_file_name.end (),
                  uc_file_name.begin (),
                  upcase);

  string uc_file_suffix = cl_.get_value ("exec-hdr-file-suffix",
                                         "_exec.h");

  std::transform (uc_file_suffix.begin (),
                  uc_file_suffix.end (),
                  uc_file_suffix.begin (),
                  upcase);

  string guard =
    "CIAO_"
    + regex::perl_s (uc_file_name,
                     "/(\\.(IDL|CIDL))?$/" +  uc_file_suffix + "/");

  guard = regex::perl_s (guard, "/\\./_/");

  os << "#include /**/ \"ace/post.h\"" << endl << endl
     << "#endif /* " << guard << " */"
     << endl << endl;
}

