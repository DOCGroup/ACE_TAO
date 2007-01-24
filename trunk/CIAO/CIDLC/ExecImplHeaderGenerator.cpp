// file      : CIDLC/ExecImplHeaderGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ExecImplHeaderGenerator.hpp"
#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"
#include "AttributeHeaderEmitters.hpp"
#include "OperationHeaderEmitters.hpp"
#include "ParameterEmitter_T.hpp"
#include "CompositionEmitter.hpp"
#include "ModuleEmitter.hpp"
#include "InterfaceEmitter.hpp"
#include "Literals.hpp"
#include "Upcase.hpp"

#include <ostream>
#include <sstream>

#include "CCF/CodeGenerationKit/Regex.hpp"

using std::endl;

using namespace StringLiterals;
using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;

namespace
{
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
         << "  : public virtual " << i.scoped_name ().scope_name ()
         << "::CCM_" << i.name () << "," << endl
         << "    public virtual TAO_Local_RefCounted_Object" << endl
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

        AttributeHeaderEmitter attribute_emitter (ctx);
        defines_.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines_.node_traverser (operation_emitter);
        inherits_.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
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
        enclosing_type_name_emitter_ (c),
        simple_type_name_emitter_ (c),
        type_name_emitter_ (c)
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
         << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl;
    }

    virtual void
    traverse (SemanticGraph::Consumer& c)
    {
      os << "virtual void" << endl
         << "push_" << c.name () << " (" << endl;

      Traversal::ConsumerData::belongs (c, belongs_);

      os << " *ev" << endl
         << ")" << endl
         << STRS[EXCP_SNGL] << ";" << endl;
   }

  private:
    EnclosingTypeNameEmitter enclosing_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    FullTypeNameEmitter type_name_emitter_;
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
         << "  : public virtual " << exec_.name () << "," << endl
         << "    public virtual TAO_Local_RefCounted_Object" << endl
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

        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
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

        AttributeHeaderEmitter attribute_emitter (ctx);
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

        ExecPortsEmitter exec_ports_emitter (ctx);
        defines.node_traverser (exec_ports_emitter);

        component_emitter.traverse (t);
      }

      os << "// Operations from Components::SessionComponent" << endl << endl;

      os << "virtual void" << endl
         << "set_session_context (" << endl
         << STRS[COMP_SC] << "_ptr ctx" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ciao_preactivate (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ciao_postactivate (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_activate (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_passivate (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;

      os << "virtual void" << endl
         << "ccm_remove (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;
    }

    virtual void
    post (Type& t)
    {
      os << "protected:" << endl
         << t.name () << "_Context *context_;" << endl
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
         << "  : public virtual " << exec_.name () << "," << endl
         << "    public virtual TAO_Local_RefCounted_Object" << endl
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

        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        OperationEmitter operation_emitter (ctx);
        defines.node_traverser (operation_emitter);
        interface_inherits.node_traverser (interface_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        operation_emitter.edge_traverser (receives);
        operation_emitter.edge_traverser (returns);
        operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
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

        HomeOperationEmitter home_operation_emitter (ctx);
        defines.node_traverser (home_operation_emitter);

        Traversal::Receives receives;
        Traversal::Belongs returns;
        Traversal::Raises raises;
        home_operation_emitter.edge_traverser (receives);
        home_operation_emitter.edge_traverser (returns);
        home_operation_emitter.edge_traverser (raises);

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
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

        bool for_exec = true;
        FactoryOperationEmitter factory_operation_emitter (ctx, for_exec);
        FinderOperationEmitter finder_operation_emitter (ctx, for_exec);
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

        ParameterEmitter<Traversal::InParameter> in_param (ctx);
        ParameterEmitter<Traversal::InOutParameter> inout_param (ctx);
        ParameterEmitter<Traversal::OutParameter> out_param (ctx);
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

        AttributeHeaderEmitter attribute_emitter (ctx);
        defines.node_traverser (attribute_emitter);

        home_emitter.traverse (t);
      }

      os << "// Implicit operations." << endl << endl
         << "virtual " << STRS[COMP_EC] << "_ptr" << endl
         << "create (" << endl
         << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_CE] << "));" << endl;
    }

    virtual void
    post (Type& t)
    {
      os << "};";

      string name;

      // We need to escape C++ keywords before flattening the name.
      //
      {
        std::ostringstream ostr;
        ostr.pword (name_printer_index) = os.pword (name_printer_index);
        ostr << t.scoped_name ();
        name = regex::perl_s (ostr.str (), "/::/_/");
      }

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::Components::HomeExecutorBase_ptr" << endl
         << "create" << name << "_Impl (void);" << endl;
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
ExecImplHeaderEmitter::pre (TranslationUnit&)
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
                     "/^(.+?)(\\.(IDL|CIDL|CDL))?$/$1"
                     + uc_file_suffix
                     + "/");

  // Replace any remaining '.' with '_'.
  guard = regex::perl_s (guard, "/\\./_/");

  os << "#ifndef " << guard << endl
     << "#define " << guard << endl << endl
     << "#include /**/ \"ace/pre.h\"" << endl << endl;

  string file_suffix = cl_.get_value ("svnt-hdr-file-suffix",
                                      "_svnt.h");

  string svnt_file_name = regex::perl_s (file_name,
                                         "/^(.+?)(\\.(idl|cidl|cdl))?$/$1"
                                         + file_suffix
                                         + "/");

  os << "#include \"" << svnt_file_name << "\"" << endl << endl;

  os << "#if !defined (ACE_LACKS_PRAGMA_ONCE)" << endl
     << "# pragma once" << endl
     << "#endif /* ACE_LACKS_PRAGMA_ONCE */" << endl << endl;

  string default_export_include =
    regex::perl_s (file_name,
                   "/^(.+?)(\\.(idl|cidl|cdl))?$/$1_exec_export.h/");

  string export_include = cl_.get_value ("exec-export-include",
                                         default_export_include);

  if (!export_include.empty ())
  {
    os << "#include \"" << export_include << "\"" << endl;
  }

  os << "#include \"tao/LocalObject.h\"" << endl << endl;
}

void
ExecImplHeaderEmitter::generate (TranslationUnit& u)
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
ExecImplHeaderEmitter::post (TranslationUnit&)
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
                     "/^(.+?)(\\.(IDL|CIDL))?$/$1" +  uc_file_suffix + "/");

  guard = regex::perl_s (guard, "/\\./_/");

  os << "#include /**/ \"ace/post.h\"" << endl << endl
     << "#endif /* " << guard << " */"
     << endl << endl;
}

