// $Id$

#include "DescriptorGenerator.hpp"
#include "Literals.hpp"

#include <ostream>

#include "CCF/CIDL/SyntaxTree.hpp"
#include "CCF/CIDL/Traversal.hpp"

#include "CCF/CodeGenerationKit/Regex.hpp"
#include "CCF/CodeGenerationKit/IndentationXML.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

using std::cout;
using std::endl;
using std::string;

using namespace CCF::CIDL;
using namespace SyntaxTree;

namespace
{
  string
  name_to_string (Name const& n)
  {
    std::ostringstream os;
    os << n;
    return os.str ();
  }

  string
  compute_repo_id (DeclarationPtr const& d)
  {
    if (d->context ().count (
          StringLiterals::STRS[StringLiterals::REPO_ID]))
    {
      return
        d->context ().get<string> (
          StringLiterals::STRS[StringLiterals::REPO_ID]);
    }

    string prefix ("");
    TypePrefixPtr tp;
    
    if (d->context ().count (
          StringLiterals::STRS[StringLiterals::TYPE_PREFIX]))
    {
      tp =
        d->context ().get<TypePrefixPtr> (
          StringLiterals::STRS[StringLiterals::TYPE_PREFIX]);
        
      prefix = tp->prefix ().str ();
    }
    else
    {
      DeclarationPtr parent = d->scope ();
        
      while (parent != 0)
      {
        if (parent->context ().count (
              StringLiterals::STRS[StringLiterals::TYPE_PREFIX]))
        {
          tp =
            parent->context ().get<TypePrefixPtr> (
              StringLiterals::STRS[StringLiterals::TYPE_PREFIX]);
            
          prefix = tp->prefix ().str ();
          break;
        }
          
        if (parent->dynamic_type<CCF::IDL2::SyntaxTree::FileScope> () != 0) 
          break;
        else parent = parent->scope ();
      }
    }
      
    if (prefix != "") prefix += "/";
    
    string scope_name =
      regex::perl_s (name_to_string (d->name ().in_file_scope ()),
                     "%::%/%",
                     '%');
                     
    string repo_id = "IDL:" + prefix + scope_name + ":1.0";
    
    // Store the repo id for possible future reference.
    d->context ().set<string> (StringLiterals::STRS[StringLiterals::REPO_ID],
                               repo_id);
    return repo_id;
  }

  typedef
  DeclarationOrderComparator<InterfaceDefPtr>
  InterfaceDefOrderComparator;

  typedef
  std::set<InterfaceDefPtr,
           InterfaceDefOrderComparator>
  InterfaceDefSet;

  class Declarations
  {
  public:
    bool
    add (UnconstrainedInterfaceDefPtr const& i)
    {
      if (! interfaces_.insert (i).second)
      {
        return false;
      }

      for (InterfaceDef::Iterator iter = i->inherits_begin ();
           iter != i->inherits_end ();
           iter++)
        {
          add (iter->resolve ()->dynamic_type<UnconstrainedInterfaceDef> ());
        }

      return true;
    }

    InterfaceDefSet& unique_ifaces ()
    {
      return this->interfaces_;
    }

  private:
    InterfaceDefSet interfaces_;
  };

  class InterfaceCollector : public Traversal::UnconstrainedInterfaceDef
  {
  public:
    InterfaceCollector (Declarations& declarations)
      : declarations_ (declarations)
    {
    }

    virtual void
    traverse (UnconstrainedInterfaceDefPtr const& i)
    {
      // Add to the list if it's not already there.
      declarations_.add (i);
    }

  private:
    Declarations& declarations_;
  };

  class ComponentCollector : public Traversal::ComponentDef
  {
  public:
    ComponentCollector (Traversal::Dispatcher* interface_collector)
        : interface_collector_ (interface_collector)
    {
    }

    virtual void
    pre (ComponentDefPtr const& c)
    {
      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        interface_collector_->dispatch (i->resolve ());
      }

      if (c->inherits ())
      {
        traverse (c->inherits ().resolve ());
      }
    }

  private:
    Traversal::Dispatcher* interface_collector_;
  };

  class HomeExecutorCollector : public Traversal::HomeExecutor
  {
  public:
    HomeExecutorCollector (Traversal::Dispatcher* component_collector)
      : component_collector_ (component_collector)
    {
    }

    virtual void
    traverse (HomeExecutorPtr const& he)
    {
      component_collector_->dispatch (he->implements ()->manages ());
    }

  private:
    Traversal::Dispatcher* component_collector_;
  };

  class EmitterBase
  {
  public:
    EmitterBase (fs::ofstream& ofs)
      : os (ofs)
    {}

  protected:
    fs::ofstream& os;
  };

  class ProvidesEmitter : public EmitterBase,
                          public Traversal::ProvidesDecl
  {
  public:
    ProvidesEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs),
        facettag_ (1U)
    {}

    virtual void
    traverse (ProvidesDeclPtr const& p)
    {
      os << "<provides" << endl
         << "providesname=\"" << p->name ().simple ()
         << "\"" << endl
         << "repid=\"" << compute_repo_id (p->type ())
         << "\"" << endl
         << "facettag=\"" << facettag_
         << "\">" << endl
         << "</provides>" << endl;

      ++facettag_;
    }

    // Called for each composition.
    void reset ()
    {
      facettag_ = 1U;
    }

  private:
    unsigned long facettag_;
  };

  class UsesEmitter : public EmitterBase,
                      public Traversal::UsesDecl
  {
  public:
    UsesEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    traverse (UsesDeclPtr const& u)
    {
      os << "<uses" << endl
         << "usesname=\"" << u->name ().simple ()
         << "\"" << endl
         << "repid=\"" << compute_repo_id (u->type ())
         << "\">" << endl
         << "</uses>" << endl;
    }
  };

  class EmitsEmitter : public EmitterBase,
                       public Traversal::EmitsDecl
  {
  public:
    EmitsEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    traverse (EmitsDeclPtr const& e)
    {
      os << "<emits" << endl
         << "emitsname=\"" << e->name ().simple ()
         << "\"" << endl
         << "eventtype=\"" << compute_repo_id (e->type ())
         << "\">" << endl
         << "<eventpolicy policy=\"normal\"/>" << endl
         << "</emits>" << endl;
    }
  };

  class PublishesEmitter : public EmitterBase,
                           public Traversal::PublishesDecl
  {
  public:
    PublishesEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    traverse (PublishesDeclPtr const& p)
    {
      os << "<publishes" << endl
         << "publishesname=\"" << p->name ().simple ()
         << "\"" << endl
         << "eventtype=\"" << compute_repo_id (p->type ())
         << "\">" << endl
         << "<eventpolicy policy=\"normal\"/>" << endl
         << "</publishes>" << endl;
    }
  };

  class ConsumesEmitter : public EmitterBase,
                          public Traversal::ConsumesDecl
  {
  public:
    ConsumesEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    traverse (ConsumesDeclPtr const& c)
    {
      os << "<consumes" << endl
         << "consumesname=\"" << c->name ().simple ()
         << "\"" << endl
         << "eventtype=\"" << compute_repo_id (c->type ())
         << "\">" << endl
         << "<eventpolicy policy=\"normal\"/>" << endl
         << "</consumes>" << endl;
    }
  };

  class ComponentEmitter : public EmitterBase,
                           public Traversal::ComponentDef
  {
  public:
    ComponentEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    pre (ComponentDefPtr const& c)
    {
      os << "<componentfeatures" << endl
         << "name=\"" << c->name ().simple ()
         << "\"" << endl
         << "repid=\"" << compute_repo_id (c)
         << "\">" << endl;

      if (c->inherits ())
      {
        os << "<inheritscomponent repid=\""
           << compute_repo_id (c->inherits ().resolve ())
           << "\"/>" << endl;
      }

      for (SyntaxTree::ComponentDef::Iterator i = c->supports_begin ();
           i != c->supports_end ();
           i++)
      {
        os << "<supportsinterface repid=\""
           << compute_repo_id (i->resolve ())
           << "\">" << endl
           << "</supportsinterface>" << endl;
      }

      os << "<ports>" << endl;
    }

    virtual void
    post (ComponentDefPtr const& c)
    {
      os << "</ports>" << endl
         << "</componentfeatures>" << endl << endl;

      // This will cause recursion as long as there are parents.
      if (c->inherits ())
      {
        dispatch (c->inherits ().resolve ());
      }
    }
  };

  class HomeEmitter : public EmitterBase,
                      public Traversal::HomeDef
  {
  public:
    HomeEmitter (fs::ofstream& ofs)
      : EmitterBase (ofs)
    {}

    virtual void
    pre (HomeDefPtr const& h)
    {
      os << "<homefeatures" << endl
        << "name=\"" << h->name ().simple () << "\"" << endl
        << "repid=\"" << compute_repo_id (h)
        << "\">" << endl;

      if (h->inherits ())
      {
        os << "<inheritshome repid=\""
           << compute_repo_id (h->inherits ().resolve ())
           << "\"/>" << endl;
      }
    }

    virtual void
    post (HomeDefPtr const& h)
    {
      os << "</homefeatures>" << endl << endl;

      // This will cause recursion as long as there are parents.
      if (h->inherits ())
      {
        dispatch (h->inherits ().resolve ());
      }
    }
  };

  class HomeExecutorEmitter : public EmitterBase,
                              public Traversal::HomeExecutor
  {
  public:
    HomeExecutorEmitter (fs::ofstream& ofs,
                         Traversal::Dispatcher* component_emitter,
                         Traversal::Dispatcher* home_emitter)
      : EmitterBase (ofs),
        manages_emitter_ (component_emitter),
        implements_emitter_ (home_emitter)
    {}

  virtual void
  pre (HomeExecutorPtr const& he)
  {
    os << "<corbacomponent>" << endl
       << "<corbaversion>3.0</corbaversion>" << endl
       << "<componentrepid repid=\""
       << compute_repo_id (he->implements ()->manages ())
       << "\"/>" << endl
       << "<homerepid repid=\""
       << compute_repo_id (he->implements ())
       << "\"/>" << endl
       << "<componentkind>" << endl;

    // This can't be 0, a home executor can be defined only in a composition.
    CompositionPtr composition =
      he->scope ()->dynamic_type<Composition> ();

    // Reasonable default values for CIAO.
    os << "<" << composition->category () << ">" << endl
       << "<servant lifetime=\"container\"/>" << endl
       << "</" << composition->category () << ">" << endl
       << "</componentkind>" << endl
       << "<threading policy=\"multithread\"/>" << endl
       << "<configurationcomplete set=\"true\"/>" << endl << endl;

    implements_emitter_->dispatch (he->implements ());
    manages_emitter_->dispatch (he->implements ()->manages ());
  }

  private:
    Traversal::Dispatcher* manages_emitter_;
    Traversal::Dispatcher* implements_emitter_;
  };

  class CompositionEmitter : public EmitterBase,
                             public Traversal::Composition
  {
  public:
    CompositionEmitter (fs::ofstream& ofs,
                        CommandLine const& cl,
                        ProvidesEmitter &pe,
                        Declarations& declarations)
      : EmitterBase (ofs),
        cl_ (cl),
        pe_ (pe),
        declarations_ (declarations)
    {}

    virtual void
    pre (CompositionPtr const& c)
    {
      string name = name_to_string (c->name ().in_file_scope ());
      configure_stream (name);

      os << "<?xml version=\"1.0\"?>" << endl
         << "<!DOCTYPE corbacomponent SYSTEM \"corbacomponent.dtd\">"
         << endl << endl;

      pe_.reset ();
    }

    virtual void
    post (CompositionPtr const&)
    {
      // Generate all the interfaces ported or supported.
      InterfaceDefSet& ifaces = declarations_.unique_ifaces ();

      for (InterfaceDefSet::const_iterator i = ifaces.begin ();
           i != ifaces.end ();
           ++i)
      {
        os << "<interface" << endl
           << "name=\"" << (*i)->name ().simple ()
           << "\"" << endl
           << "repid=\"" << compute_repo_id (*i)
           << "\">" << endl;

        for (InterfaceDef::Iterator inh = (*i)->inherits_begin ();
              inh != (*i)->inherits_end ();
              ++inh)
        {
          os << "<inheritsinterface repid=\""
             << compute_repo_id ((*inh).resolve ())
             << "\"/>" << endl;
        }

        os << "</interface>" << endl;
      }

      os << endl
         << "</corbacomponent>" << endl;
    }

  private:
    void configure_stream (string const& scoped_name)
    {
      string file_suffix = cl_.get_value ("desc-file-suffix",
                                          ".ccd");

      string desc_file_name = regex::perl_s (scoped_name, "/::/_/");

      desc_file_name += file_suffix;

      fs::path desc_file_path (desc_file_name);

      if (os.is_open ())
      {
        os.close ();
      }

      os.open (desc_file_path, std::ios_base::out);

      if (!os.is_open ())
      {
        cerr << desc_file_name
             << ": error: unable to open file in write mode"
             << endl;
      }
    }

  private:
    CommandLine const& cl_;
    ProvidesEmitter pe_;
    Declarations& declarations_;
  };
}

// ==========================================================

void
DescriptorGenerator::options (CL::Description& d)
{
  d.add_option (CL::OptionDescription (
                  "desc-file-suffix",
                  "suffix",
                  "Use provided suffix instead of default \'.ccd\' "
                  "when constructing name of descriptor file.",
                  true));

  d.add_option (CL::OptionDescription (
                  "desc-file-regex",
                  "regex",
                  "Use provided regular expression when constructing "
                  "name of descriptor file.",
                  true));
}

void
DescriptorGenerator::generate (CommandLine const& cl,
                               TranslationUnitPtr const& u)
{
  Declarations declarations;

  // Set auto-indentation for os
  Indentation::Implanter<Indentation::XML> guard (ofs_);

  {
    InterfaceCollector iface (declarations);
    Traversal::ProvidesDecl provides (&iface);
    Traversal::UsesDecl uses (&iface);

    ComponentCollector component (&iface);
    component.add_scope_delegate (&provides);
    component.add_scope_delegate (&uses);

    HomeExecutorCollector home_executor (&component);

    Traversal::Composition composition;
    composition.add_scope_delegate (&home_executor);

    Traversal::Module module;
    module.add_scope_delegate (&composition);
    module.add_scope_delegate (&module);

    Traversal::FileScope file_scope;
    file_scope.add_scope_delegate (&composition);
    file_scope.add_scope_delegate (&module);

    Traversal::TranslationRegion region (&file_scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);
  }

  {
    ProvidesEmitter provides (ofs_);
    UsesEmitter uses (ofs_);
    EmitsEmitter emits (ofs_);
    PublishesEmitter publishes (ofs_);
    ConsumesEmitter consumes (ofs_);

    ComponentEmitter component (ofs_);
    component.add_scope_delegate (&provides);
    component.add_scope_delegate (&uses);
    component.add_scope_delegate (&emits);
    component.add_scope_delegate (&publishes);
    component.add_scope_delegate (&consumes);

    HomeEmitter home (ofs_);

    HomeExecutorEmitter home_executor (ofs_, &component, &home);

    CompositionEmitter composition (ofs_, cl, provides, declarations);
    composition.add_scope_delegate (&home_executor);

    Traversal::Module module;
    module.add_scope_delegate (&composition);
    module.add_scope_delegate (&module);

    Traversal::FileScope file_scope;
    file_scope.add_scope_delegate (&module);
    file_scope.add_scope_delegate (&composition);

    Traversal::TranslationRegion region (&file_scope);

    Traversal::TranslationUnit unit;
    unit.add_content_delegate (&region);

    unit.dispatch (u);
  }
}
