// $Id$
#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

#include <map>

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

class Declarations
{
public:
  bool
  add (CCF::IDL3::SyntaxTree::HomeDefPtr const& h)
  {
    return homes_.insert (h).second;
  }

  bool
  add (CCF::IDL3::SyntaxTree::ComponentDefPtr const& c)
  {
    return components_.insert (c).second;
  }

  bool
  add (CCF::CIDL::SyntaxTree::CompositionPtr const& cs,
       CCF::IDL3::SyntaxTree::ComponentDefPtr const& cn)
  {
    return compositions_.insert (make_pair(cs, cn)).second;
  }

  bool
  add (CCF::IDL3::SyntaxTree::UnconstrainedInterfaceDeclPtr const& i)
  {
    return interfaces_.insert (i).second
      && interface_names_.insert (i->name ()).second;
  }

public:
  bool
  find (CCF::IDL3::SyntaxTree::HomeDefPtr const& h) const
  {
    return homes_.find (h) != homes_.end ();
  }

  bool
  find (CCF::IDL3::SyntaxTree::ComponentDefPtr const& c) const
  {
    return components_.find (c) != components_.end ();
  }

  bool
  find (CCF::CIDL::SyntaxTree::CompositionPtr const& c) const
  {
    return compositions_.find (c) != compositions_.end ();
  }

  bool
  find (CCF::IDL2::SyntaxTree::UnconstrainedInterfaceDefPtr const& i) const
  {
    return interface_names_.find (i->name ()) != interface_names_.end ();
  }

public:
  CCF::IDL3::SyntaxTree::ComponentDefPtr
  resolve (CCF::CIDL::SyntaxTree::CompositionPtr const& c) const
  {
    CompositionMap::const_iterator i = compositions_.find (c);
    if (i != compositions_.end ())
    {
      return i->second;
    }
    else
    {
      return CCF::IDL3::SyntaxTree::ComponentDefPtr (0);
    }
  }

public:
  bool
  contains_suborder (CCF::CIDL::SyntaxTree::Order const& o) const
  {
    for (CCF::IDL2::SyntaxTree::UnconstrainedInterfaceDeclSet::const_iterator i =
             interfaces_.begin ();
         i != interfaces_.end ();
         i++)
    {
      if (o.suborder ((*i)->order ())) return true;
    }

    for (CCF::IDL3::SyntaxTree::ComponentDefSet::const_iterator i = components_.begin ();
         i != components_.end ();
         i++)
    {
      if (o.suborder ((*i)->order ())) return true;
    }

    for (CCF::IDL3::SyntaxTree::HomeDefSet::const_iterator i = homes_.begin ();
         i != homes_.end ();
         i++)
    {
      if (o.suborder ((*i)->order ())) return true;
    }

    for (CompositionMap::const_iterator i = compositions_.begin ();
         i != compositions_.end ();
         i++)
    {
      if (o.suborder (i->first->order ())) return true;
    }

    return false;
  }

private:
  typedef
  std::map <CCF::CIDL::SyntaxTree::CompositionPtr,
            CCF::IDL3::SyntaxTree::ComponentDefPtr,
            CCF::CIDL::SyntaxTree::CompositionOrderComparator> CompositionMap;

  CCF::IDL3::SyntaxTree::HomeDefSet homes_;
  CCF::IDL3::SyntaxTree::ComponentDefSet components_;
  CompositionMap compositions_;
  CCF::IDL2::SyntaxTree::UnconstrainedInterfaceDeclSet interfaces_;
  CCF::IDL2::SyntaxTree::ScopedNameSet interface_names_;
};


//
//
//
class HomeCollector : public CCF::IDL3::Traversal::HomeDef
{
public:
  HomeCollector (Declarations& declarations)
    : declarations_ (declarations)
  {
  }

  virtual void
  traverse (CCF::CIDL::SyntaxTree::HomeDefPtr const& h)
  {
    CCF::CIDL::SyntaxTree::ScopedName n (h->name ());

    CCF::CIDL::SyntaxTree::ScopedName main (n.scope (), "CCM_" + n.simple ());
    CCF::CIDL::SyntaxTree::ScopedName expl (n.scope (), "CCM_" + n.simple () + "Explicit");
    CCF::CIDL::SyntaxTree::ScopedName impl (n.scope (), "CCM_" + n.simple () + "Implicit");

    // Check if mapping has already been provided.
    if (h->table ().exist (main) ||
        h->table ().exist (expl) ||
        h->table ().exist (impl)) return;

    if(declarations_.add (h))
    {
      // Note that I don't go after components that inherited home manages
      // because it will be handled by component inheritance tree.
      //
      if (h->inherits ()) traverse (h->inherits ().resolve ());
    }
  }

private:
  Declarations& declarations_;
};

//
//
//
class ComponentCollector : public CCF::IDL3::Traversal::ComponentDef
{
public:
  ComponentCollector (Declarations& declarations)
    : declarations_ (declarations)
  {
  }

  virtual void
  pre (CCF::IDL3::SyntaxTree::ComponentDefPtr const& c)
  {
    CCF::CIDL::SyntaxTree::ScopedName n (c->name ());

    CCF::CIDL::SyntaxTree::ScopedName monolith (n.scope (), "CCM_" + n.simple ());
    CCF::CIDL::SyntaxTree::ScopedName context (n.scope (), "CCM_" + n.simple () + "_Context");

    // Check if mapping has already been provided.
    if (c->table ().exist (context) ||
        c->table ().exist (monolith)) return;

    if(declarations_.add (c))
    {
      if (c->inherits ())
      {
        traverse (c->inherits ().resolve ());
      }
    }
  }

private:
  Declarations& declarations_;
};

//
//
//
class ProvidedInterfaceCollector : public CCF::IDL3::Traversal::ProvidesDecl
{
public:
  ProvidedInterfaceCollector (Declarations& declarations)
    : declarations_ (declarations)
  {
  }

  virtual void
  traverse (CCF::IDL3::SyntaxTree::ProvidesDeclPtr const& i)
  {
    //@@ CCM issue: interface should be defined at this point
    CCF::IDL2::SyntaxTree::UnconstrainedInterfaceDeclPtr def
        (i->type ()
         ->dynamic_type<CCF::CIDL::SyntaxTree::UnconstrainedInterfaceDecl> ());

    if (def != 0)
    {
      // Add to the list if it's not already there.
      declarations_.add (def);
    }
  }

private:
  Declarations& declarations_;
};


//
//
//
class HomeExecutorCollector : public CCF::CIDL::Traversal::HomeExecutor
{
public:
  HomeExecutorCollector (Declarations& declarations,
                         CCF::CIDL::Traversal::Dispatcher* home_collector,
                         CCF::CIDL::Traversal::Dispatcher* component_collector)
    : declarations_ (declarations),
      home_collector_ (home_collector),
      component_collector_ (component_collector)
  {
  }

  virtual void
  traverse (CCF::CIDL::SyntaxTree::HomeExecutorPtr const& he)
  {
    CCF::CIDL::SyntaxTree::HomeDefPtr home (he->implements ());
    home_collector_->dispatch (home);

    CCF::CIDL::SyntaxTree::ComponentDefPtr component (home->manages ());
    component_collector_->dispatch (component);

    CCF::CIDL::SyntaxTree::CompositionPtr
        composition (he->scope ()->dynamic_type<CCF::CIDL::SyntaxTree::Composition> ());
    declarations_.add (composition, component);
  }

private:
  Declarations& declarations_;

  CCF::CIDL::Traversal::Dispatcher* home_collector_;
  CCF::CIDL::Traversal::Dispatcher* component_collector_;
};

#endif // COLLECTORS_HPP

/*
 * Local Variables:
 * mode: C++
 * c-basic-offset: 2
 * End:
 */
