// file      : CIDLC/ServantSourceGenerator.cpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#include "ServantSourceGenerator.hpp"
#include "Literals.hpp"
#include "TypeNameEmitter.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"

#include <ostream>
#include <sstream>

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

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

  struct OperationExecEmitter : Traversal::Operation,
                                EmitterBase
  {
    OperationExecEmitter (Context& c)
      : EmitterBase (c)
    {}

    virtual void
    name (Type& o)
    {
      os << o.name ();
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
         << STRS[ENV_SNGL_ARG] << ");";
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_ARG] << ");";
    }
    
    /// If we don't do this, the comma() method just below
    /// gets used not only with the arguments, but also
    /// in raises(). Even though we are not generating the
    /// exception list here, the front end iterator still
    /// executes, and picks up the overridden comma(). So
    /// we override raises() itself to do nothing.
    virtual void
    raises (Type&)
    {
    }
    
    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }
  };

  struct OpExecReturnEmitter : Traversal::Type,
                               Traversal::Void
  {
    OpExecReturnEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    traverse (Void&)
    {
      os << "this->executor_->";
    }

    virtual void
    traverse (SemanticGraph::Type&)
    {
      os << "return this->executor_->";
    }

  private:
    std::ostream& os;
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

  // For generating parameter names in operation delegation.
  template <typename T>
  struct ParameterExecEmitter : T
  {
    ParameterExecEmitter (std::ostream& os_)
      : os (os_)
    {}

    virtual void
    name (typename T::Type& p)
    {
      os << p.name ();
    }

  private:
    std::ostream& os;
  };

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
         << scope_.name () << "_Servant::" << o.name ();
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
         << STRS[ENV_SNGL_SRC] << ")" << endl;
    }

    virtual void
    receives_post (Type&)
    {
      os << endl << STRS[ENV_SRC] << ")" << endl;
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
    post (Type& o)
    {
      os << "{";

      OperationExecEmitter operation_emitter (ctx);

      Traversal::Receives receives;
      Traversal::Belongs returns;
      operation_emitter.edge_traverser (receives);
      operation_emitter.edge_traverser (returns);

      ParameterExecEmitter<Traversal::Parameter> param (os);
      receives.node_traverser (param);

      OpExecReturnEmitter return_emitter (os);
      returns.node_traverser (return_emitter);

      operation_emitter.traverse (o);

      os << "}" << endl;
    }

    virtual void
    comma (Type&)
    {
      os << "," << endl;
    }

  protected:
    T& scope_;
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

  // Generates operations associated with attributes.
  // @@@ (JP) Need to support exceptions.
  template <typename T>
  struct AttributeEmitter : Traversal::ReadWriteAttribute,
                            EmitterBase
  {
    AttributeEmitter (Context& c, T& scope)
      : EmitterBase (c),
        write_type_name_emitter_ (c.os ()),
        read_type_name_emitter_ (c.os ()),
        scope_ (scope)
    {
      write_belongs_.node_traverser (write_type_name_emitter_);
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadWriteAttribute& a)
    {
      Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

      os << endl
         << scope_.name () << "_Servant::" << a.name () << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return this->executor_->" << a.name () << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl;

      os << "void" << endl
         << scope_.name () << "_Servant::" << a.name () << " (" << endl;

      Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

      os << " " << a.name () << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "this->executor_->" << a.name () << " (" << endl
         << a.name () << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl;
    }

  private:
    INArgTypeNameEmitter write_type_name_emitter_;
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs write_belongs_;
    Traversal::Belongs read_belongs_;
    T& scope_;
  };

  // Generates operations associated with readonly attributes.
  // @@@ (JP) Need to support exceptions.
  template <typename T>
  struct ReadOnlyAttributeEmitter : Traversal::ReadAttribute,
                                    EmitterBase
  {
    ReadOnlyAttributeEmitter (Context& c, T& scope)
      : EmitterBase (c),
        read_type_name_emitter_ (c.os ()),
        scope_ (scope)
    {
      read_belongs_.node_traverser (read_type_name_emitter_);
    }

    virtual void traverse (SemanticGraph::ReadAttribute& a)
    {
      Traversal::ReadAttribute::belongs (a, read_belongs_);

      os << endl
         << scope_.name () << "_Servant::" << a.name () << " (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return this->executor_->" << a.name () << " (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl;
    }

  private:
    ReturnTypeNameEmitter read_type_name_emitter_;
    Traversal::Belongs read_belongs_;
    T& scope_;
  };

  struct FacetEmitter : Traversal::UnconstrainedInterface,
                        EmitterBase
  {
    FacetEmitter (Context& c)
      : EmitterBase (c),
        scope_type_name_emitter_ (c.os ()),
        simple_type_name_emitter_ (c.os ())
    {
      scope_belongs_.node_traverser (scope_type_name_emitter_);
      simple_belongs_.node_traverser (simple_type_name_emitter_);
    }

    virtual void
    traverse (UnconstrainedInterface& i)
    {
      // Open a namespace.
      os << STRS[GLUE_NS]
         << regex::perl_s (i.scoped_name ().scope_name ().str (),
                           "/::/_/")
         << "{";

      os << i.name () << "_Servant::" << i.name ()
         << "_Servant (" << endl
         << i.scoped_name ().scope_name () << "::CCM_" << i.name ()
         << "_ptr executor," << endl
         << "::Components::CCMContext_ptr c)" << endl
         << ": executor_ (" << i.scoped_name ().scope_name () << "::CCM_"
         << i.name () << "::_duplicate (executor))," << endl
         << "ctx_ (::Components::CCMContext::_duplicate (c))" << endl
         << "{"
         << "}" << endl;

      os << i.name () << "_Servant::~" << i.name ()
         << "_Servant (void)" << endl
         << "{"
         << "}" << endl;

      {
        InterfaceEmitter interface_emitter (ctx);

        Traversal::Defines defines_;
        Traversal::Inherits inherits_;
        interface_emitter.edge_traverser (defines_);
        interface_emitter.edge_traverser (inherits_);

        AttributeEmitter<SemanticGraph::Interface> attribute_emitter (ctx, i);
        ReadOnlyAttributeEmitter<SemanticGraph::Interface> read_only_attribute_emitter (ctx, i);
        defines_.node_traverser (attribute_emitter);
        defines_.node_traverser (read_only_attribute_emitter);

        OperationEmitter<SemanticGraph::UnconstrainedInterface> operation_emitter (ctx, i);
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

      os << "CORBA::Object_ptr" << endl
         << i.name () << "_Servant::_get_component (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << "ACE_THROW_SPEC ((CORBA::SystemException))" << endl
         << "{"
         << "::Components::SessionContext_var sc =" << endl
         << "::Components::SessionContext::_narrow (" << endl
         << "this->ctx_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "if (! CORBA::is_nil (sc.in ()))" << endl
         << "{"
         << "return sc->get_CCM_object (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl
         << "::Components::EntityContext_var ec =" << endl
         << "::Components::EntityContext::_narrow (" << endl
         << "this->ctx_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "if (! CORBA::is_nil (ec.in ()))" << endl
         << "{"
         << "return ec->get_CCM_object (" << endl
         << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl
         << "ACE_THROW_RETURN (CORBA::INTERNAL (), 0);" << endl
         << "}" << endl;

      // Close the CIAO_GLUE namespace.
      os << "}" << endl;
    }

  private:
    EnclosingTypeNameEmitter scope_type_name_emitter_;
    SimpleTypeNameEmitter simple_type_name_emitter_;
    Traversal::Belongs scope_belongs_;
    Traversal::Belongs simple_belongs_;
  };

  struct ContextEmitter : Traversal::Component, EmitterBase
  {
    ContextEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by ContextEmitter.
  private:
    struct ContextPortsEmitter : Traversal::UserData,
                                 Traversal::PublisherData,
                                 Traversal::EmitterData,
                                 EmitterBase
    {
      ContextPortsEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
      }

      virtual void
      traverse (SemanticGraph::User& u)
      {
        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << u.scoped_name ().scope_name ().simple_name () << "_Context::get_connection_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_SRC_NOTUSED] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return ";

        Traversal::UserData::belongs (u, belongs_);

        os << "::_duplicate (" << endl
           << "this->ciao_uses_" << u.name ()
           << "_.in ());" << endl
           << "}" << endl;

        os << "void" << endl
           << u.scoped_name ().scope_name ().simple_name () << "_Context::connect_"
           << u.name () << " (" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "if (! CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}" << endl
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_IC] << " ());" << endl
           << "}" << endl
           << "this->ciao_uses_" << u.name () << "_ =" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "::_duplicate (c);" << endl
           << "}" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "_ptr" << endl
           << u.scoped_name ().scope_name ().simple_name () << "_Context::disconnect_"
           << u.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (this->ciao_uses_"
           << u.name () << "_.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "::_nil ());" << endl
           << "}" << endl
           << "return this->ciao_uses_" << u.name ()
           << "_._retn ();" << endl
           << "}" << endl;
      }

      virtual void
      traverse (SemanticGraph::Publisher& p)
      {
        os << "void" << endl
           << p.scoped_name ().scope_name ().simple_name () << "_Context::push_"
           << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << " *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "ACE_Active_Map_Manager<" << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator end =" << endl
           << "this->ciao_publishes_" << p.name ()
           << "_map_.end ();" << endl
           << "for (ACE_Active_Map_Manager<" << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::iterator iter =" << endl
           << "this->ciao_publishes_" << p.name ()
           << "_map_.begin ();"
           << "iter != end;"
           << "++iter)" << endl
           << "{"
           << "ACE_Active_Map_Manager<" << endl; //@@ gcc bug

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var>::ENTRY &entry = *iter;" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var c =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "entry.int_id_.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK;" << endl
           << "entry.int_id_->push_";

        Traversal::PublisherData::belongs (p, simple_belongs_);

        os << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK;" << endl
           << "}" << endl
           << "}" << endl;

        os << STRS[COMP_CK] << " *" << endl
           << p.scoped_name ().scope_name ().simple_name () << "_Context::subscribe_"
           << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);" << endl
           << "}" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os<< "Consumer_var sub =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_duplicate (c);" << endl
           << "ACE_Active_Map_Manager_Key key;"
           << "this->ciao_publishes_" << p.name ()
           << "_map_.bind (sub.in (), key);"
           << "sub._retn ();" << endl
           << STRS[COMP_CK] << "_var retv = "
           << "new ::CIAO::Map_Key_Cookie (key);"
           << "return retv._retn ();" << endl
           << "}" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << p.scoped_name ().scope_name ().simple_name () << "_Context::unsubscribe_"
           << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{";

        Traversal::PublisherData::belongs (p, belongs_);

        os<< "Consumer_var retv;"
           << "ACE_Active_Map_Manager_Key key;" << endl
           << "if (ck == 0 || ::CIAO::Map_Key_Cookie::extract (ck, key) == -1)"
           << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}" << endl << endl
           << "if (this->ciao_publishes_" << p.name ()
           << "_map_.unbind (key, retv) != 0)" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_IC] << " ()," << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}" << endl
           << "return retv._retn ();" << endl
           << "}" << endl;
      }

      virtual void
      traverse (SemanticGraph::Emitter& e)
      {
        os << "void" << endl
           << e.scoped_name ().scope_name ().simple_name () << "_Context::push_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << " *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "this->ciao_emits_" << e.name ()
           << "_consumer_->push_";

        Traversal::EmitterData::belongs (e, simple_belongs_);

        os << " (" << endl
           << "ev" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;

        os << "void" << endl
           << e.scoped_name ().scope_name ().simple_name () << "_Context::connect_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (c))" << endl
           << "{"
           << "ACE_THROW (CORBA::BAD_PARAM ());" << endl
           << "}" << endl << endl
           << "if (! CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "ACE_THROW (" << STRS[EXCP_AC] << " ());" << endl
           << "}" << endl << endl
           << "this->ciao_emits_" << e.name ()
           << "_consumer_ = c;" << endl
           << "}" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << e.scoped_name ().scope_name ().simple_name () << "_Context::disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "if (CORBA::is_nil (this->ciao_emits_" << e.name ()
           << "_consumer_.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << STRS[EXCP_NC] << " ()," << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer::_nil ());" << endl
           << "}" << endl << endl
           << "return this->ciao_emits_" << e.name ()
           << "_consumer_._retn ();" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      // CIAO has facet classes outside the context and servant classes.
      // We launch it here to generate the classes at the top of the file,
      // like the original hand-crafted examples, for easier checking.
      // This can be moved later to ServantEmitter::pre() or out on its own.
      {
        Traversal::Defines defines;

        Traversal::Provider provider;
        defines.node_traverser (provider);

        Traversal::Belongs belongs;
        provider.edge_traverser (belongs);

        FacetEmitter facet_emitter (ctx);
        belongs.node_traverser (facet_emitter);

        names (t, defines);
      }

      os << STRS[GLUE_NS]
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "{";

      os << t.name () << "_Context::"
         << t.name () << "_Context (" << endl
         << "::Components::CCMHome_ptr home," << endl
         << "::CIAO::Session_Container *c," << endl
         << t.name () << "_Servant *sv)" << endl
         << ": home_ (::Components::CCMHome::_duplicate (home))," << endl
         << "container_ (c)," << endl
         << "servant_ (sv)" << endl
         << "{"
         << "}" << endl;

      os << t.name () << "_Context::~"
         << t.name () << "_Context (void)" << endl
         << "{"
         << "}" << endl;

      os << "// Operations from ::Components::CCMContext." << endl << endl;

      os << "::Components::Principal_ptr" << endl
         << t.name () << "_Context::"
         << "get_caller_principal (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::NO_IMPLEMENT ()," << endl
         << "::Components::Principal::_nil ());" << endl
         << "}" << endl;

      os << "::Components::CCMHome_ptr" << endl
         << t.name () << "_Context::"
         << "get_CCM_home (" << endl
         << STRS[ENV_SNGL_SRC_NOTUSED] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return ::Components::CCMHome::_duplicate (this->home_.in ());"
         << endl
         << "}" << endl;

      os << "CORBA::Boolean" << endl
         << t.name () << "_Context::"
         << "get_rollback_only (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::Transaction::UserTransaction_ptr" << endl
         << t.name () << "_Context::"
         << "get_user_transaction (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::NO_IMPLEMENT ()," << endl
         << "::Components::Transaction::UserTransaction::_nil ());" << endl
         << "}" << endl;

      os << "CORBA::Boolean" << endl
         << t.name () << "_Context::"
         << "is_caller_in_role (" << endl
         << "const char * /* role */" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name () << "_Context::"
         << "set_rollback_only (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "))" << endl
         << "{"
         << "ACE_THROW (CORBA::NO_IMPLEMENT ());" << endl
         << "}" << endl;

      os << "// Operations from ::Components::SessionContext interface."
         << endl << endl;

      os << "CORBA::Object_ptr" << endl
         << t.name () << "_Context::"
         << "get_CCM_object (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IS] << "))" << endl
         << "{"
         << "if (CORBA::is_nil (this->component_.in ()))" << endl
         << "{"
         << "CORBA::Object_var obj =" << endl
         << "this->container_->get_objref (" << endl
         << "this->servant_" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "this->component_ =" << endl
         << t.scoped_name () << "::_narrow (" << endl
         << "obj.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (CORBA::Object::_nil ());" << endl
         << "if (CORBA::is_nil (this->component_.in ()))" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::INTERNAL ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl
         << "}" << endl
         << "return " << t.scoped_name () << "::_duplicate (" << endl
         << "this->component_.in ());" << endl
         << "}" << endl;

      os << "// Operations for " << t.name () << " receptacles"
         << " and event sources," << endl
         << "// defined in " << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_Context."
         << endl << endl;

      // Generate Context class operations for receptacles and event sources.
      {
        Traversal::Defines defines;
        ContextPortsEmitter ports_emitter (ctx);
        defines.node_traverser (ports_emitter);

        names (t, defines);
      }
      
      os << "// CIAO-specific." << endl << endl
         << "::CIAO::Session_Container *" << endl
         << t.name () << "_Context::"
         << "_ciao_the_Container (void) const" << endl
         << "{"
         << "return this->container_;" << endl
         << "}" << endl;
         
      os << t.name () << "_Context *" << endl
         << t.name () << "_Context::_narrow (" << endl
         << "::Components::SessionContext_ptr p" << endl
         << STRS[ENV_SRC_NOTUSED] << ")" << endl
         << "{"
         << "return dynamic_cast<" << t.name () << "_Context *> (p);"
         << endl
         << "}" << endl;
    }

    virtual void
    post (Type& t)
    {
      // Namespace closer.
      os << "}" << endl;
    }
  };

  struct ServantEmitter : Traversal::Component, EmitterBase
  {
    ServantEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by ServantEmitter.
  private:
    struct NavigationEmitter : Traversal::ProviderData,
                               EmitterBase
    {
      NavigationEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& t)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << t.name () << "\") == 0)" << endl
           << "{"
           << "return this->provide_" << t.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;
      }
    };

    struct PublishesEmitter : Traversal::PublisherData,
                              EmitterBase
    {
      PublishesEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << STRS[COMP_CK] << " *" << endl
           << p.scoped_name ().scope_name ().simple_name ()
           << "_Servant::subscribe_" << p.name () << " (" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_ECL] << "))" << endl
           << "{"
           << "return this->context_->subscribe_" << p.name ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_ptr" << endl
           << p.scoped_name ().scope_name ().simple_name ()
           << "_Servant::unsubscribe_" << p.name () << " (" << endl
           << STRS[COMP_CK] << " *ck" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "return this->context_->unsubscribe_"
           << p.name () << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct UsesConnectEmitter : Traversal::UserData,
                                EmitterBase
    {
      UsesConnectEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name () << "\") == 0)" << endl
           << "{";

        Traversal::UserData::belongs (u, belongs_);

        os << "_var _ciao_conn =" << endl;

        Traversal::UserData::belongs (u, belongs_);

        os << "::_narrow (" << endl
           << "connection" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "if (::CORBA::is_nil (_ciao_conn.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}" << endl
           << "// Simplex connect." << endl
           << "this->connect_" << u.name () << " (" << endl
           << "_ciao_conn.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "return 0;" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct UsesDisconnectEmitter : Traversal::UserData,
                                   EmitterBase
    {
      UsesDisconnectEmitter (Context& c)
        : EmitterBase (c)
      {
      }

      virtual void
      traverse (Type& u)
      {
        os << "if (ACE_OS::strcmp (name, \""
           << u.name () << "\") == 0)" << endl
           << "{"
           << "// Simplex disconnect." << endl
           << "return this->disconnect_" << u.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;
      }
    };

    struct UsesEmitter : Traversal::UserData,
                         EmitterBase
    {
      UsesEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << "void" << endl
           << p.scoped_name ().scope_name ().simple_name () << "_Servant::connect_"
           << p.name () << " (" << endl;

        Traversal::UserData::belongs (p, belongs_);

        os << "_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "," << endl
           << STRS[EXCP_IC] << "))" << endl
           << "{"
           << "this->context_->connect_" << p.name () << " ("
           << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;

        Traversal::UserData::belongs (p, belongs_);

        os << "_ptr" << endl
           << p.scoped_name ().scope_name ().simple_name () << "_Servant::disconnect_"
           << p.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_" << p.name ()
           << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;

        Traversal::UserData::belongs (p, belongs_);

        os << "_ptr" << endl
           << p.scoped_name ().scope_name ().simple_name ()
           << "_Servant::get_connection_" << p.name ()
           << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->context_->get_connection_"
           << p.name () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct ConsumesGetEmitter : Traversal::ConsumerData,
                                EmitterBase
    {
      ConsumesGetEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& c)
      {
        os << "if (ACE_OS::strcmp (sink_name, \""
           << c.name () << "\") == 0)" << endl
           << "{"
           << "return this->get_consumer_" << c.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;
      }
    };

    struct PublishesSubscribeEmitter : Traversal::PublisherData,
                                       EmitterBase
    {
      PublishesSubscribeEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name () << "\") == 0)" << endl
           << "{";

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer_var _ciao_consumer =" << endl;

        Traversal::PublisherData::belongs (p, belongs_);

        os << "Consumer::_narrow (" << endl
           << "subscribe" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (0);" << endl
           << "if (::CORBA::is_nil (_ciao_consumer.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << STRS[EXCP_IC] << " (), 0);"
           << endl
           << "}" << endl
           << "return this->subscribe_" << p.name ()
           << " (" << endl
           << "_ciao_consumer.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct PublishesUnsubscribeEmitter : Traversal::PublisherData,
                                         EmitterBase
    {
      PublishesUnsubscribeEmitter (Context& c)
        : EmitterBase (c)
      {}

      virtual void
      traverse (Type& p)
      {
        os << "if (ACE_OS::strcmp (publisher_name, \""
           << p.name () << "\") == 0)" << endl
           << "{"
           << "return this->unsubscribe_" << p.name ()
           << " (" << endl
           << "ck" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;
      }
    };

    struct EmitsEmitter : Traversal::EmitterData,
                          EmitterBase
    {
      EmitsEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
      }

      virtual void
      traverse (Type& e)
      {
        os << "void" << endl
           << e.scoped_name ().scope_name ().simple_name () << "_Servant::connect_"
           << e.name () << " (" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr c" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_AC] << "))" << endl
           << "{"
           << "this->context_->connect_" << e.name ()
           << " (" << endl
           << "c" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;

        Traversal::EmitterData::belongs (e, belongs_);

        os << "Consumer_ptr" << endl
           << e.scoped_name ().scope_name ().simple_name () << "_Servant::disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_NC] << "))" << endl
           << "{"
           << "return this->context_->disconnect_"
           << e.name () << " (" << endl
           << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      Traversal::Belongs belongs_;
    };

    struct ServantTypeNameEmitter : Traversal::Type
    {
      ServantTypeNameEmitter (std::ostream& os_)
        : os (os_)
      {}

      virtual void
      traverse (SemanticGraph::Type& t)
      {
        os << "CIAO_GLUE";

        ScopedName scope (t.scoped_name ().scope_name ());

        if (!scope.simple ())
        {
//          os << "_" << Name (scope.begin () + 1, scope.end ());
          
          for (Name::Iterator i (scope.begin () + 1); i != scope.end (); ++i)
          {
            os << "_" << i->str ();
          }
        }

        os << "::" << t.name () << "_Servant";
      }

    private:
      std::ostream& os;
    };

    struct FacetProvidesEmitter : Traversal::ProviderData,
                                  EmitterBase
    {
      FacetProvidesEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ()),
          servant_type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
        servant_belongs_.node_traverser (servant_type_name_emitter_);
      }

      virtual void
      traverse (Type& p)
      {
        Traversal::ProviderData::belongs (p, belongs_);

        os << "_ptr" << endl
           << p.scoped_name ().scope_name ().simple_name ()
           << "_Servant::provide_" << p.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (::CORBA::is_nil (this->provide_"
           << p.name () << "_.in ()))" << endl
           << "{"
           << p.scoped_name ().scope_name ().scope_name () << "::CCM_";

        Traversal::ProviderData::belongs (p, simple_belongs_);

        os << "_var fexe =" << endl
           << "this->executor_->get_" << p.name ()
           << " (" << STRS[ENV_SNGL_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());"
           << endl
           << "if (::CORBA::is_nil (fexe.in ()))" << endl
           << "{"
           << "ACE_THROW_RETURN (" << endl
           << "::CORBA::INTERNAL ()," << endl;

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());" << endl
           << "}" << endl;

        Traversal::ProviderData::belongs (p, servant_belongs_);

        os << " *svt =" << endl
           << "new ";

        Traversal::ProviderData::belongs (p, servant_belongs_);

        os << " (" << endl
           << "fexe.in ()," << endl
           << "this->context_);" << endl
           << "PortableServer::ServantBase_var safe_servant (svt);"
           << endl
           << "::CORBA::Object_var obj =" << endl
           << "this->container_->install_servant (" << endl
           << "svt" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());"
           << endl;

        Traversal::ProviderData::belongs (p, belongs_);

        os << "_var fo =" << endl;

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_nil ());"
           << endl
           << "this->provide_" << p.name () << "_ = fo;" << endl
           << "}" << endl << endl
           << "return ";

        Traversal::ProviderData::belongs (p, belongs_);

        os << "::_duplicate (this->provide_"
           << p.name () << "_.in ());" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      ServantTypeNameEmitter servant_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
      Traversal::Belongs servant_belongs_;
    };

    struct ConsumesEmitter : Traversal::ConsumerData,
                             EmitterBase
    {
      ConsumesEmitter (Context& c)
        : EmitterBase (c),
          type_name_emitter_ (c.os ()),
          simple_type_name_emitter_ (c.os ())
      {
        belongs_.node_traverser (type_name_emitter_);
        simple_belongs_.node_traverser (simple_type_name_emitter_);
      }

      virtual void
      traverse (Type& c)
      {
        os << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant (" << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name  ()
           << "_ptr executor,"
           << endl
           << c.scoped_name ().scope_name ().scope_name () << "::CCM_"
           << c.scoped_name ().scope_name ().simple_name  ()
           << "_Context_ptr c)" << endl
           << ": executor_ (" << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_" << c.scoped_name ().scope_name ().simple_name  ()
           << "::_duplicate (executor))," << endl
           << "ctx_ (" << c.scoped_name ().scope_name ().scope_name ()
           << "::CCM_" << c.scoped_name ().scope_name ().simple_name  ()
           << "_Context::_duplicate (c))" << endl
           << "{"
           << "}" << endl;

        os << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::~";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant (void)" << endl
           << "{"
           << "}" << endl;

        os << "CORBA::Object_ptr" << endl
           << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::_get_component (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "return this->ctx_->get_CCM_object "
           << "(" << STRS[ENV_SNGL_ARG] << ");" << endl
           << "}" << endl;

        os << "void" << endl
           << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << " (" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << " *evt" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "this->executor_->push_" << c.name ()
           << " (" << endl
           << "evt" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "}" << endl;

        os << "// Inherited from " << STRS[COMP_ECB] << "." << endl
           << "void" << endl
           << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant::push_event (" << endl
           << "::Components::EventBase *ev" << endl
           << STRS[ENV_SRC] << ")" << endl
           << STRS[EXCP_START] << endl
           << STRS[EXCP_SYS] << "," << endl
           << STRS[EXCP_BET] << "))" << endl
           << "{";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "_var ev_type =" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "::_downcast (ev);" << endl
           << "if (ev_type != 0)" << endl
           << "{"
           << "this->push_";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << " (" << endl
           << "ev_type.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl
           << "return;" << endl
           << "}" << endl
           << "ACE_THROW (" << STRS[EXCP_BET] << " ());" << endl
           << "}" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_ptr" << endl
           << c.scoped_name ().scope_name ().simple_name  () << "_Servant::"
           << "get_consumer_" << c.name () << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl
           << STRS[EXCP_SNGL] << endl
           << "{"
           << "if (CORBA::is_nil (this->consumes_" << c.name ()
           << "_.in ()))" << endl
           << "{"
           << c.scoped_name ().scope_name ().simple_name  () << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant *svt =" << endl
           << "new " << c.scoped_name ().scope_name ().simple_name  ()
           << "_Servant::";

        Traversal::ConsumerData::belongs (c, simple_belongs_);

        os << "Consumer_" << c.name ()
           << "_Servant (" << endl
           << "this->executor_.in ()," << endl
           << "this->context_);"
           << "PortableServer::ServantBase_var safe_servant (svt);"
           << endl
           << "CORBA::Object_var obj =" << endl
           << "this->container_->install_servant (" << endl
           << "svt" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_nil ());" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer_var eco =" << endl;

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_narrow (" << endl
           << "obj.in ()" << endl
           << STRS[ENV_ARG] << ");"
           << "ACE_CHECK_RETURN (";

        Traversal::ConsumerData::belongs (c, belongs_);

        os<< "Consumer::_nil ());" << endl
           << "this->consumes_" << c.name () << "_ = eco;" << endl
           << "}" << endl
           << "return ";

        Traversal::ConsumerData::belongs (c, belongs_);

        os << "Consumer::_duplicate (this->consumes_" << c.name ()
           << "_.in ());" << endl
           << "}" << endl;
      }

    private:
      TypeNameEmitter type_name_emitter_;
      SimpleTypeNameEmitter simple_type_name_emitter_;
      Traversal::Belongs belongs_;
      Traversal::Belongs simple_belongs_;
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << STRS[GLUE_NS]
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "{";

      // Servant Constructor
      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::Components::CCMHome_ptr h," << endl
         << "::CIAO::Session_Container *c)" << endl
         << ": executor_ (" << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "::_duplicate (exe))," << endl
         << "container_ (c)" << endl
         << "{"
         << "this->context_ = "
         << "new " << t.name () << "_Context (h, c, this);"
         << endl
         << "ACE_TRY_NEW_ENV" << endl
         << "{"
         << "::Components::SessionComponent_var scom =" << endl
         << "::Components::SessionComponent::_narrow (" << endl
         << "exe" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_TRY_CHECK;" << endl
         << "if (! ::CORBA::is_nil (scom.in ()))" << endl
         << "{"
         << "scom->set_session_context (" << endl
         << "this->context_" << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl
         << "}"
         << "ACE_CATCHANY" << endl
         << "{"
         << "}"
         << "ACE_ENDTRY;" << endl
         << "}" << endl;

      // Servant Destructor
      os << t.name () << "_Servant::~"
         << t.name () << "_Servant (void)" << endl
         << "{"
         << "ACE_TRY_NEW_ENV" << endl
         << "{"
         << "::Components::SessionComponent_var scom =" << endl
         << "::Components::SessionComponent::_narrow (" << endl
         << "this->executor_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_TRY_CHECK;" << endl
         << "if (! ::CORBA::is_nil (scom.in ()))" << endl
         << "{"
         << "scom->ccm_remove (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl
         << "}"
         << "ACE_CATCHANY" << endl
         << "{"
         << "}"
         << "ACE_ENDTRY;" << endl
         << "this->context_->_remove_ref ();" << endl
         << "}" << endl;

      // Generate provides_<facet> operation.
      {
        Traversal::Defines defines;
        FacetProvidesEmitter provides_emitter (ctx);
        defines.node_traverser (provides_emitter);

        names (t, defines);
      }

      os << "// Operations for Navigation interface." << endl << endl;

      os << "CORBA::Object_ptr" << endl
         << t.name () << "_Servant::provide_facet (" << endl
         << "const char *name" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::BAD_PARAM ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl;

      // Generate an IF block for each facet inside provide_facet().
      {
        Traversal::Defines defines;
        NavigationEmitter navigation_emitter (ctx);
        defines.node_traverser (navigation_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl;

      os << "::Components::FacetDescriptions *" << endl
         << t.name () << "_Servant::get_all_facets (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::FacetDescriptions *" << endl
         << t.name () << "_Servant::get_named_facets (" << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "CORBA::Boolean" << endl
         << t.name () << "_Servant::same_component (" << endl
         << "CORBA::Object_ptr object_ref" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "if (::CORBA::is_nil (object_ref))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::BAD_PARAM (), 0);" << endl
         << "}" << endl
         << "::CORBA::Object_var the_other =" << endl
         << "object_ref->_get_component (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "::CORBA::Object_var me =" << endl
         << "this->context_->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "return me->_is_equivalent (" << endl
         << "the_other.in ()" << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl;

      os << "::Components::EmitterDescriptions *" << endl
         << t.name () << "_Servant::get_all_emitters ("
         << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::EmitterDescriptions *" << endl
         << t.name () << "_Servant::get_named_emitters ("
         << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      // Generate subscribe_* and unsubscribe_* operations.
      {
        Traversal::Defines defines;
        PublishesEmitter publishes_emitter (ctx);
        defines.node_traverser (publishes_emitter);

        names (t, defines);
      }

      // Generate operations for nested Consumer classes.
      {
        Traversal::Defines defines;
        ConsumesEmitter consumes_emitter (ctx);
        defines.node_traverser (consumes_emitter);

        names (t, defines);
      }

      os << STRS[COMP_CK] << " *" << endl
         << t.name () << "_Servant::connect (" << endl
         << "const char *name," << endl
         << "::CORBA::Object_ptr connection" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{"
         << "// If the component has no receptacles, this will be unused."
         << endl
         << "ACE_UNUSED_ARG (connection);" << endl
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}" << endl;

      // Generate IF block in connect operation for each receptacle.
      {
        Traversal::Defines defines;
        UsesConnectEmitter uses_emitter (ctx);
        defines.node_traverser (uses_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}" << endl;

      os << "CORBA::Object_ptr" << endl
         << t.name () << "_Servant::disconnect (" << endl
         << "const char *name," << endl
         << STRS[COMP_CK] << " * /* ck */" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_CR] << "," << endl
         << STRS[EXCP_NC] << "))" << endl
         << "{"
         << "if (name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl;

      // Generate IF block in disconnect operation for each receptacle.
      {
        Traversal::Defines defines;
        UsesDisconnectEmitter uses_emitter (ctx);
        defines.node_traverser (uses_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl;

      os << "::Components::ConnectionDescriptions *" << endl
         << t.name () << "_Servant::get_connections ("
         << endl
         << "const char * /* name */" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::ReceptacleDescriptions *" << endl
         << t.name () << "_Servant::get_all_receptacles ("
         << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::ReceptacleDescriptions *" << endl
         << t.name () << "_Servant::get_named_receptacles ("
         << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      // Generate generic operations for receptacles.
      {
        Traversal::Defines defines;
        UsesEmitter uses_emitter (ctx);
        defines.node_traverser (uses_emitter);

        names (t, defines);
      }

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::get_consumer (" << endl
         << "const char *sink_name" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "if (sink_name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
         << "}" << endl;

      // Generate an IF block in for each consumer in get_consumer().
      {
        Traversal::Defines defines;
        ConsumesGetEmitter consumes_emitter (ctx);
        defines.node_traverser (consumes_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name () << "_Servant::connect_consumer ("
         << endl
         << "const char * /* emitter_name */," << endl
         << STRS[COMP_ECB] << "_ptr /*consumer*/" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_AC] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{"
         << "ACE_THROW (::CORBA::NO_IMPLEMENT ());" << endl
         << "}" << endl;

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::disconnect_consumer ("
         << endl
         << "const char * /* source_name */" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_NC] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::ConsumerDescriptions *" << endl
         << t.name () << "_Servant::get_all_consumers ("
         << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::ConsumerDescriptions *" << endl
         << t.name () << "_Servant::get_named_consumers ("
         << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << STRS[COMP_CK] << " *" << endl
         << t.name () << "_Servant::subscribe (" << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_ECB] << "_ptr subscribe" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "," << endl
         << STRS[EXCP_ECL] << "))" << endl
         << "{"
	 << "// Just in case there are no if blocks" << endl
	 << "ACE_UNUSED_ARG (subscribe);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);"
         << endl
         << "}" << endl;

      // Generate an IF block in for each publisher in subscribe().
      {
        Traversal::Defines defines;
        PublishesSubscribeEmitter publishes_emitter (ctx);
        defines.node_traverser (publishes_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << STRS[EXCP_IN] << " (), 0);" << endl
         << "}" << endl;

      os << STRS[COMP_ECB] << "_ptr" << endl
         << t.name () << "_Servant::unsubscribe ("
         << endl
         << "const char *publisher_name," << endl
         << STRS[COMP_CK] << " *ck" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "," << endl
         << STRS[EXCP_IC] << "))" << endl
         << "{"
	 << "// Just in case there are no if blocks" << endl
	 << "ACE_UNUSED_ARG (ck);" << endl
         << "if (publisher_name == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
         << "}" << endl;

      // Generate an IF block in for each publisher in unsubscribe().
      {
        Traversal::Defines defines;
        PublishesUnsubscribeEmitter publishes_emitter (ctx);
        defines.node_traverser (publishes_emitter);

        names (t, defines);
      }

      os << "ACE_THROW_RETURN (" << endl
         << STRS[EXCP_IN] << " ()," << endl
         << STRS[COMP_ECB] << "::_nil ());" << endl
         << "}" << endl;

      // @@ (diego) These are not implemented. Whenever they are,
      // they'll require a treatment as all the other ports above.

      os << "::Components::PublisherDescriptions *" << endl
         << t.name () << "_Servant::get_all_publishers ("
         << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      os << "::Components::PublisherDescriptions *" << endl
         << t.name () << "_Servant::get_named_publishers ("
         << endl
         << "const " << STRS[COMP_NAMES] << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_IN] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);" << endl
         << "}" << endl;

      // Generate connect() and disconnect() for each emits declaration.
      {
        Traversal::Defines defines;
        EmitsEmitter emits_emitter (ctx);
        defines.node_traverser (emits_emitter);

        names (t, defines);
      }

      os << "// Operations for CCMObject interface." << endl << endl;

      os << "CORBA::IRObject_ptr" << endl
         << t.name () << "_Servant::get_component_def (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::NO_IMPLEMENT ()," << endl
         << "::CORBA::IRObject::_nil ());" << endl
         << "}" << endl;

      os << "::Components::CCMHome_ptr" << endl
         << t.name () << "_Servant::get_ccm_home (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "return this->context_->get_CCM_home "
         << "(ACE_ENV_SINGLE_ARG_PARAMETER);" << endl
         << "}" << endl;

      os << "::Components::PrimaryKeyBase *" << endl
         << t.name () << "_Servant::get_primary_key (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_NKA] << "))" << endl
         << "{"
         << "ACE_THROW_RETURN (" << STRS[EXCP_NKA] << " (), 0);" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name ()
         << "_Servant::configuration_complete (" << endl
         << STRS[ENV_SNGL_SRC_NOTUSED] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_ICF] << "))" << endl
         << "{"
         << "// CIAO to-do" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name () << "_Servant::remove (" << endl
         << STRS[ENV_SNGL_SRC_NOTUSED] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_RF] << "))" << endl
         << "{"
         << "// CIAO to-do" << endl
         << "}" << endl;

      os << "::Components::ComponentPortDescription *" << endl
         << t.name () << "_Servant::get_all_ports (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "::Components::ComponentPortDescription_var retv =" << endl
         << "new OBV_Components::ComponentPortDescription;" << endl
         << "::Components::FacetDescriptions_var facets_desc =" << endl
         << "this->get_all_facets (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "::Components::ReceptacleDescriptions_var receptacle_desc ="
         << endl
         << "this->get_all_receptacles (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "::Components::ConsumerDescriptions_var consumer_desc =" << endl
         << "this->get_all_consumers (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "::Components::EmitterDescriptions_var emitter_desc =" << endl
         << "this->get_all_emitters (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "::Components::PublisherDescriptions_var publisher_desc ="
         << endl
         << "this->get_all_publishers (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "retv->facets (facets_desc.in ());"
         << "retv->receptacles (receptacle_desc.in ());"
         << "retv->consumers (consumer_desc.in ());"
         << "retv->emitters (emitter_desc.in ());"
         << "retv->publishers (publisher_desc.in ());" << endl
         << "return retv._retn ();" << endl
         << "}" << endl;

      os << "CORBA::Object_ptr" << endl
         << t.name () << "_Servant::_get_component (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "::Components::SessionContext_var sc =" << endl
         << "::Components::SessionContext::_narrow (" << endl
         << "this->context_" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (::CORBA::Object::_nil ());" << endl
         << "if (! ::CORBA::is_nil (sc.in ()))" << endl
         << "{"
         << "return sc->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
         << endl
         << "}" << endl
         << "::Components::EntityContext_var ec =" << endl
         << "::Components::EntityContext::_narrow (" << endl
         << "this->context_" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (::CORBA::Object::_nil ());" << endl
         << "if (! ::CORBA::is_nil (ec.in ()))" << endl
         << "{"
         << "return ec->get_CCM_object (" << STRS[ENV_SNGL_ARG] << ");"
         << endl
         << "}" << endl
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::INTERNAL ()," << endl
         << "::CORBA::Object::_nil ());" << endl
         << "}" << endl;

      os << "// CIAO-specific operations." << endl << endl;

      os << "void" << endl
         << t.name () << "_Servant::_ciao_activate (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "::Components::SessionComponent_var temp =" << endl
         << "::Components::SessionComponent::_narrow (" << endl
         << "this->executor_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK;" << endl
         << "if (! ::CORBA::is_nil (temp.in ()))" << endl
         << "{"
         << "temp->ccm_activate (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name () << "_Servant::_ciao_passivate (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "::Components::SessionComponent_var temp =" << endl
         << "::Components::SessionComponent::_narrow (" << endl
         << "this->executor_.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK;" << endl
         << "if (! ::CORBA::is_nil (temp.in ()))" << endl
         << "{"
         << "temp->ccm_passivate (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl
         << "}" << endl;

      os << "// Supported operations." << endl << endl;

      // Generate operations for all supported interfaces.
      {
        Traversal::Supports supports_;
        InterfaceEmitter interface_emitter (ctx);
        supports_.node_traverser (interface_emitter);

        Traversal::Defines defines;
        Traversal::Inherits inherits;
        interface_emitter.edge_traverser (defines);
        interface_emitter.edge_traverser (inherits);

        AttributeEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<SemanticGraph::Component> read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        OperationEmitter<SemanticGraph::Component> operation_emitter (ctx, t);
        defines.node_traverser (operation_emitter);
        inherits.node_traverser (interface_emitter);

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

        supports (t, supports_);
      }

      // Generate operations for component attributes.
      os << "// Component attribute operations." << endl << endl;

      {
        Traversal::Defines defines;
        AttributeEmitter<SemanticGraph::Component> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<SemanticGraph::Component> read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        names (t, defines);
      }
    }

    virtual void
    post (Type& t)
    {
      // Namespace closer.
      os << "}" << endl;
    }
  };

  struct HomeEmitter : Traversal::Home, EmitterBase
  {
    HomeEmitter (Context& c)
      : EmitterBase (c)
    {}

  // Nested classes used by this emitter.
  private:
    struct HomeOpExecReturnEmitter : Traversal::Type
    {
      HomeOpExecReturnEmitter (std::ostream& os_)
        : os (os_)
      {}

      virtual void
      traverse (SemanticGraph::Type&)
      {
        os << "::Components::EnterpriseComponent_var _ciao_ec =" << endl
           << "this->executor_->";
      }

    private:
      std::ostream& os;
    };

    struct HomeOperationEmitter : Traversal::HomeFactory,
                                  OperationEmitter<SemanticGraph::Home>,
                                  Traversal::Home
    {
      HomeOperationEmitter (Context& c, SemanticGraph::Home& home)
        : OperationEmitter<SemanticGraph::Home> (c, home)
      {}

      virtual void
      returns (SemanticGraph::HomeFactory& hf)
      {
        ReturnTypeNameEmitter manages_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (manages_emitter);

        manages (scope_, manages_);

        os << endl;
      }

      virtual void
      name (SemanticGraph::HomeFactory& hf)
      {
        os << scope_.name () << "_Servant::" << hf.name ();
      }

      virtual void
      receives_none (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl
           << STRS[ENV_SNGL_SRC] << ")" << endl;
      }

      virtual void
      receives_pre (SemanticGraph::HomeFactory&)
      {
        os << " (" << endl;
      }

      virtual void
      receives_post (SemanticGraph::HomeFactory&)
      {
        os << endl << STRS[ENV_SRC] << ")" << endl;
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
      post (SemanticGraph::HomeFactory& hf)
      {
        os << "{";

        {
          OperationExecEmitter operation_emitter (ctx);

          Traversal::Receives receives;
          Traversal::Belongs returns;
          operation_emitter.edge_traverser (receives);
          operation_emitter.edge_traverser (returns);

          ParameterExecEmitter<Traversal::InParameter> in_param (os);
          ParameterExecEmitter<Traversal::InOutParameter> inout_param (os);
          ParameterExecEmitter<Traversal::OutParameter> out_param (os);
          receives.node_traverser (in_param);
          receives.node_traverser (inout_param);
          receives.node_traverser (out_param);

          HomeOpExecReturnEmitter return_emitter (os);
          returns.node_traverser (return_emitter);

          operation_emitter.traverse (hf);
        }

        os << "ACE_CHECK_RETURN (";

        {
          TypeNameEmitter manages_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (manages_emitter);

          manages (scope_, manages_);
        }

        os << "::_nil ());" << endl;

        {
          EnclosingTypeNameEmitter name_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (name_emitter);

          manages (scope_, manages_);
        }

        os << "::CCM_";

        {
          SimpleTypeNameEmitter name_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (name_emitter);

          manages (scope_, manages_);
        }

        os << "_var _ciao_comp =" << endl;

        {
          EnclosingTypeNameEmitter name_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (name_emitter);

          manages (scope_, manages_);
        }

        os << "::CCM_";

        {
          SimpleTypeNameEmitter name_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (name_emitter);

          manages (scope_, manages_);
        }

        os << "::_narrow (" << endl
           << "_ciao_ec.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl;

        os << "ACE_CHECK_RETURN (";

        {
          TypeNameEmitter manages_emitter (os);
          Traversal::Manages manages_;
          manages_.node_traverser (manages_emitter);

          manages (scope_, manages_);
        }

        os << "::_nil ());" << endl;

        os << "return this->_ciao_activate_component (" << endl
           << "_ciao_comp.in ()" << endl
           << STRS[ENV_ARG] << ");" << endl;

        os << "}" << endl;
      }

      virtual void
      comma (SemanticGraph::HomeFactory&)
      {
        os << "," << endl;
      }
    };

  public:
    virtual void
    pre (Type& t)
    {
      os << STRS[GLUE_NS]
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "{";

      os << t.name () << "_Servant::"
         << t.name () << "_Servant (" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "_ptr exe," << endl
         << "::CIAO::Session_Container *c)" << endl
         << ": executor_ (" << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "::_duplicate (exe))," << endl
         << "container_ (c)" << endl
         << "{"
         << "}" << endl;

      os << t.name () << "_Servant::~"
         << t.name () << "_Servant (void)" << endl
         << "{"
         << "}" << endl;

      os << "// Home factory and other operations." << endl << endl;

      {
        Traversal::Defines defines;
        HomeOperationEmitter home_operation_emitter (ctx, t);
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

        names (t, defines);
      }

      // Generate operations for component attributes.
      os << "// Home attribute operations." << endl << endl;

      {
        Traversal::Defines defines;
        AttributeEmitter<SemanticGraph::Home> attribute_emitter (ctx, t);
        ReadOnlyAttributeEmitter<SemanticGraph::Home> read_only_attribute_emitter (ctx, t);
        defines.node_traverser (attribute_emitter);
        defines.node_traverser (read_only_attribute_emitter);

        names (t, defines);
      }

      os << "// Operations for keyless home interface." << endl << endl;

      os << "::Components::CCMObject_ptr" << endl
         << t.name () << "_Servant::create_component (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << "::Components::CreateFailure))" << endl
         << "{"
         << "return this->create (" << STRS[ENV_SNGL_ARG] << ");" << endl
         << "}" << endl;

      os << "// Operations for implicit home interface." << endl << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_ptr" << endl
         << t.name () << "_Servant::create (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << "::Components::CreateFailure))" << endl
         << "{"
         << "if (this->executor_.in () == 0)" << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::INTERNAL ()," << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());" << endl
         << "}" << endl
         << "Components::EnterpriseComponent_var _ciao_ec =" << endl
         << "this->executor_->create (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());" << endl;

      {
        EnclosingTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::CCM_";

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_var _ciao_comp =" << endl;

      {
        EnclosingTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::CCM_";

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_narrow (" << endl
         << "_ciao_ec.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());" << endl
         << "return this->_ciao_activate_component (" << endl
         << "_ciao_comp.in ()" << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl;

      os << "// Operations for CCMHome interface." << endl << endl;

      os << "::CORBA::IRObject_ptr" << endl
         << t.name () << "_Servant::get_component_def (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::NO_IMPLEMENT ()," << endl
         << "::CORBA::IRObject::_nil ());" << endl
         << "}" << endl;

      os << "::CORBA::IRObject_ptr" << endl
         << t.name () << "_Servant::get_home_def (" << endl
         << STRS[ENV_SNGL_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "ACE_THROW_RETURN (" << endl
         << "::CORBA::NO_IMPLEMENT ()," << endl
         << "::CORBA::IRObject::_nil ());" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name () << "_Servant::remove_component (" << endl
         << "::Components::CCMObject_ptr comp" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_START] << endl
         << STRS[EXCP_SYS] << "," << endl
         << STRS[EXCP_RF] << "))" << endl
         << "{";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_var _ciao_comp =" << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_narrow (" << endl
         << "comp" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK;" << endl
         << "if (CORBA::is_nil (_ciao_comp.in ()))" << endl
         << "{"
         << "ACE_THROW (CORBA::INTERNAL ());" << endl
         << "}" << endl
         << "_ciao_comp->remove (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK;" << endl
         << "this->_ciao_passivate_component (" << endl
         << "_ciao_comp.in ()" << endl
         << STRS[ENV_ARG] << ");" << endl
         << "}" << endl;

      os << "// CIAO-specific operations." << endl << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_ptr" << endl
         << t.name ()
         << "_Servant::_ciao_activate_component (" << endl;

      {
        EnclosingTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::CCM_";

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_ptr exe" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "::CORBA::Object_var hobj =" << endl
         << "this->container_->get_objref (" << endl
         << "this" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

       os << "::_nil ());"
         << endl
         << "::Components::CCMHome_var home =" << endl
         << "::Components::CCMHome::_narrow (" << endl
         << "hobj.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());"
         << endl;

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_Servant *svt =" << endl
         << "new ";

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_Servant ("
         << endl
         << "exe," << endl
         << "home.in ()," << endl
         << "this->container_);" << endl
         << "PortableServer::ServantBase_var safe (svt);"
         << "PortableServer::ObjectId_var oid;" << endl
         << "CORBA::Object_var objref =" << endl
         << "this->container_->install_component (" << endl
         << "svt," << endl
         << "oid.out ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());"
         << endl
         << "svt->_ciao_activate (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());"
         << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_var ho =" << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_narrow (" << endl
         << "objref.in ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (";

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "::_nil ());"
         << endl
         << "if (this->component_map_.bind (oid.in (), svt) == 0)" << endl
         << "{"
         << "safe._retn ();" << endl
         << "}" << endl
         << "return ho._retn ();" << endl
         << "}" << endl;

      os << "void" << endl
         << t.name ()
         << "_Servant::_ciao_passivate_component (" << endl;

      {
        TypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_ptr comp" << endl
         << STRS[ENV_SRC] << ")" << endl
         << STRS[EXCP_SNGL] << endl
         << "{"
         << "PortableServer::ObjectId_var oid;" << endl
         << "this->container_->uninstall_component (" << endl
         << "comp," << endl
         << "oid.out ()" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK;" << endl;

      {
        SimpleTypeNameEmitter name_emitter (os);
        Traversal::Manages manages_;
        manages_.node_traverser (name_emitter);

        manages (t, manages_);
      }

      os << "_Servant *servant = 0;"
         << endl
         << "if (this->component_map_.unbind (oid.in (), servant) == 0)"
         << endl
         << "{"
         << "PortableServer::ServantBase_var safe (servant);" << endl
         << "servant->_ciao_passivate (" << STRS[ENV_SNGL_ARG] << ");"
         << "ACE_CHECK;" << endl
         << "}" << endl
         << "}" << endl;
    }

    virtual void
    post (Type& t)
    {
      // Namespace closer.
      os << "}" << endl;

      os << "extern \"C\" " << ctx.export_macro ()
         << " ::PortableServer::Servant"
         << endl
         << "create" << t.name () << "_Servant (" << endl
         << "::Components::HomeExecutorBase_ptr p," << endl
         << "CIAO::Session_Container *c" << endl
         << STRS[ENV_SRC] << ")" << endl
         << "{"
         << "if (p == 0)" << endl
         << "{"
         << "return 0;" << endl
         << "}" << endl
         << t.scoped_name ().scope_name () << "::CCM_"
         << t.name () << "_var x =" << endl
         << t.scoped_name ().scope_name () << "::CCM_" << t.name ()
         << "::_narrow (" << endl
         << "p" << endl
         << STRS[ENV_ARG] << ");"
         << "ACE_CHECK_RETURN (0);" << endl
         << "if (::CORBA::is_nil (x.in ()))" << endl
         << "{"
         << "return 0;" << endl
         << "}" << endl
         << "return new" << endl;

      os << "CIAO_GLUE"
         << regex::perl_s (t.scoped_name ().scope_name ().str (), "/::/_/")
         << "::" << t.name () << "_Servant (" << endl
         << "x.in ()," << endl
         << "c);" << endl
         << "}" << endl;
    }
  };

  //@@ There is exactly the same code in header generator.
  //
  struct CompositionEmitter : Traversal::Composition, EmitterBase
  {
    CompositionEmitter (Context& c)
      : EmitterBase (c)
    {
    }

    virtual void
    pre (Type& t)
    {
      os << "namespace " << t.name () << "{";
    }

    virtual void
    post (Type& t)
    {
      os << "}";
    }
  };
}

ServantSourceEmitter::ServantSourceEmitter (std::ostream& os_,
                                            CommandLine const& cl,
                                            string export_macro,
                                            fs::path const& file)
  : os (os_),
    cl_ (cl),
    export_macro_ (export_macro),
    file_ (file)
{}

void
ServantSourceEmitter::pre (TranslationUnit& u)
{
  os << COPYRIGHT;

  string file_name ("");

  if (! file_.empty ())
  {
    file_name = file_.leaf ();
  }

  string file_suffix = cl_.get_value ("hdr-file-suffix",
                                      "_svnt.h");

  file_name = regex::perl_s (file_name,
                             "/(\\.(idl|cidl))?$/"
                             + file_suffix
                             + "/");

  os << "#include \"" << file_name << "\"" << endl
     << "#include \"Cookies.h\"" << endl << endl;
}

void
ServantSourceEmitter::generate (TranslationUnit& u)
{
  pre (u);

  Context c (os, export_macro_);

  Traversal::TranslationUnit unit;

  // Layer 1
  //
  Traversal::ContainsPrincipal contains_principal;
  unit.edge_traverser (contains_principal);

  //--
  Traversal::TranslationRegion region;
  contains_principal.node_traverser (region);

  // Layer 2
  //
  Traversal::ContainsRoot contains_root;
  Traversal::Includes includes;

  region.edge_traverser (includes);
  region.edge_traverser (contains_root);

  //--
  Traversal::Root root;
  includes.node_traverser (region);
  contains_root.node_traverser (root);

  // Layer 3
  //
  Traversal::Defines defines;
  root.edge_traverser (defines);

  //--
  Traversal::Module module;
  CompositionEmitter composition (c);
  defines.node_traverser (module);
  defines.node_traverser (composition);

  // Layer 4
  //
  Traversal::Defines composition_defines;
  composition.edge_traverser (composition_defines);

  //--
  Traversal::ComponentExecutor component_executor;
  Traversal::HomeExecutor home_executor;
  composition_defines.node_traverser (component_executor);
  composition_defines.node_traverser (home_executor);
  
  module.edge_traverser (defines);

  // Layer 5
  //
  Traversal::Implements implements;
  component_executor.edge_traverser (implements);
  home_executor.edge_traverser (implements);

  //--
  ContextEmitter context_emitter (c);
  ServantEmitter servant_emitter (c);
  HomeEmitter home_emitter (c);
  implements.node_traverser (context_emitter);
  implements.node_traverser (servant_emitter);
  implements.node_traverser (home_emitter);

  unit.traverse (u);
}

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
