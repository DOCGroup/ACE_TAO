// file      : CIDLC/AttributeSourceEmitters_T.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef ATTRIBUTESOURCEEMITTERS_T_HPP
#define ATTRIBUTESOURCEEMITTERS_T_HPP

#include "CCF/CIDL/SemanticGraph.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "Literals.hpp"
#include "CorbaTypeNameEmitters.hpp"
#include "UtilityTypeNameEmitters.hpp"

using namespace CCF::CIDL;
using namespace CCF::CIDL::SemanticGraph;
using namespace StringLiterals;

template <typename T>
struct WriteAttributeSourceEmitter : Traversal::ReadWriteAttribute,
                                     EmitterBase
{
  WriteAttributeSourceEmitter (Context& c,
                               T& scope,
                               bool swapping,
                               bool exec_src)
    : EmitterBase (c),
      scope_ (scope),
      write_type_name_emitter_ (c),
      type_name_emitter_ (c),
      swapping_ (swapping),
      exec_src_ (exec_src)
  {
    write_belongs_.node_traverser (write_type_name_emitter_);
    edge_traverser (set_raises_);
    set_raises_.node_traverser (type_name_emitter_);
  }

  virtual void
  gen_swapping_set ()
  {
    os << "this->activate_component ();"
       << endl;
  }

  virtual void
  pre (SemanticGraph::ReadWriteAttribute&)
  {
    os << "void" << endl;
  }

  virtual void
  name (SemanticGraph::ReadWriteAttribute& a)
  {
    os << scope_.name ()
       << (exec_src_ ? "_exec_i::" : "_Servant::")
       << a.name () << " (";

    Traversal::ReadWriteAttribute::belongs (a, write_belongs_);

    os << (exec_src_ ? " /* " : " ")
       << a.name ()
       << (exec_src_ ? " */" : "") << ")" << endl;
  }

  virtual void
  get_raises (SemanticGraph::ReadWriteAttribute&)
  {
  }

  virtual void
  set_raises (SemanticGraph::ReadWriteAttribute&)
  {
  }

  virtual void
  post (SemanticGraph::ReadWriteAttribute& a)
  {
    os << "{";

    if (exec_src_)
      {
        os << STRS[YCH];
      }
    else
      {
        if (swapping_)
        {
          this->gen_swapping_set ();
        }

        os << "this->executor_->" << a.name ()
           << " (" << a.name () << ");" << endl;
      }

    os << "}";
  }

  virtual void
  comma (SemanticGraph::ReadWriteAttribute&)
  {
    os << "," << endl;
  }

protected:
  T& scope_;
  Traversal::Belongs write_belongs_;

private:
  INArgTypeNameEmitter write_type_name_emitter_;
  FullTypeNameEmitter type_name_emitter_;
  Traversal::SetRaises set_raises_;
  bool swapping_;
  bool exec_src_;
};

template <typename T>
struct AttributeSourceEmitter : Traversal::ReadAttribute,
                                Traversal::ReadWriteAttribute,
                                EmitterBase
{
  AttributeSourceEmitter (Context& c, T& scope, bool exec_src = false)
    : EmitterBase (c),
      scope_ (scope),
      read_type_name_emitter_ (c),
      type_name_emitter_ (c),
      return_emitter_ (c),
      exec_src_ (exec_src)
  {
    read_belongs_.node_traverser (read_type_name_emitter_);
    return_belongs_.node_traverser (return_emitter_);
    edge_traverser (get_raises_);
    get_raises_.node_traverser (type_name_emitter_);
  }

  // ReadWriteAttribute
  //

  // Overridden by facet attribute emitter in servant source generator.
  virtual void
  gen_swapping_get (SemanticGraph::ReadWriteAttribute& a)
  {
    os << "this->activate_component ();";

    Traversal::ReadWriteAttribute::belongs (a, ace_check_return_belongs_);

    os << endl;
  }

  // Overridden by facet attribute emitter in servant source generator.
  virtual void
  gen_write_operation (SemanticGraph::ReadWriteAttribute& a,
                       bool swapping)
  {
    WriteAttributeSourceEmitter<T> write_attribute_emitter (ctx,
                                                            scope_,
                                                            swapping,
                                                            exec_src_);
    write_attribute_emitter.traverse (a);
  }

  virtual void
  pre (SemanticGraph::ReadWriteAttribute& a)
  {
    Traversal::ReadWriteAttribute::belongs (a, read_belongs_);

    os << endl;
  }

  virtual void
  name (SemanticGraph::ReadWriteAttribute& a)
  {
    os << scope_.name ()
       << (exec_src_ ? "_exec_i::" : "_Servant::")
       << a.name () << " ()" << endl;
  }

  virtual void
  get_raises (SemanticGraph::ReadWriteAttribute&)
  {
  }

  virtual void
  set_raises (SemanticGraph::ReadWriteAttribute&)
  {
  }

  virtual void
  post (SemanticGraph::ReadWriteAttribute& a)
  {
    bool swapping = false;

    os << "{";

    if (exec_src_)
      {
        os << STRS[YCH] << endl;

        Traversal::ReadWriteAttribute::belongs (a, return_belongs_);
      }
    else
      {
        string swap_option = ctx.cl ().get_value ("custom-container", "");
        swapping = (swap_option == "upgradeable");

        if (swapping)
        {
          this->gen_swapping_get (a);
        }

        os << "return this->executor_->" << a.name () << " ();";
      }

    os << "}";

    this->gen_write_operation (a, swapping);
  }

  virtual void
  comma (SemanticGraph::ReadWriteAttribute&)
  {
    os << "," << endl;
  }

  // ReadAttribute
  //

  // Overridden by facet attribute emitter in servant source generator.
  virtual void
  gen_swapping_get (SemanticGraph::ReadAttribute& a)
  {
    os << "this->activate_component ();";

    Traversal::ReadAttribute::belongs (a, ace_check_return_belongs_);

    os << endl;
  }

  virtual void
  pre (SemanticGraph::ReadAttribute& a)
  {
    Traversal::ReadAttribute::belongs (a, read_belongs_);

    os << endl;
  }

  virtual void
  name (SemanticGraph::ReadAttribute& a)
  {
    os << scope_.name ()
       << (exec_src_ ? "_exec_i::" : "_Servant::")
       << a.name () << " ()" << endl;
  }

  virtual void
  get_raises (SemanticGraph::ReadAttribute&)
  {
  }

  virtual void
  set_raises (SemanticGraph::ReadAttribute&)
  {
  }

  virtual void
  post (SemanticGraph::ReadAttribute& a)
  {
    bool swapping = false;

    os << "{";

    if (exec_src_)
      {
        os << STRS[YCH] << endl;

        Traversal::ReadAttribute::belongs (a, return_belongs_);
      }
    else
      {
        string swap_option = ctx.cl ().get_value ("custom-container", "");
        swapping = (swap_option == "upgradeable");

        if (swapping)
        {
          this->gen_swapping_get (a);
        }

        os << "return this->executor_->" << a.name () << " ();";
      }

    os << "}";
  }

  virtual void
  comma (SemanticGraph::ReadAttribute&)
  {
    os << "," << endl;
  }

protected:
  T& scope_;

private:
  ReturnTypeNameEmitter read_type_name_emitter_;
  FullTypeNameEmitter type_name_emitter_;
  NullReturnEmitter return_emitter_;
  Traversal::Belongs read_belongs_;
  Traversal::Belongs return_belongs_;
  Traversal::Belongs ace_check_return_belongs_;
  Traversal::GetRaises get_raises_;
  bool exec_src_;
};

#endif  // ATTRIBUTESOURCEEMITTERS_T_HPP
