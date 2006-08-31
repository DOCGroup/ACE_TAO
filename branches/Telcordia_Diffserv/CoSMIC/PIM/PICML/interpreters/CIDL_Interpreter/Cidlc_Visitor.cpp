// $Id$

#include "Cidlc_Visitor.h"
#include "Uml.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace PICML
{
  //
  // Cidlc_Visitor
  //
  Cidlc_Visitor::Cidlc_Visitor (const std::string & output)
  : output_ (output)
  {

  }

  //
  // ~Cidlc_Visitor
  //
  Cidlc_Visitor::~Cidlc_Visitor (void)
  {

  }

  //
  // Visit_RootFolder
  //
  void Cidlc_Visitor::Visit_RootFolder (const PICML::RootFolder & root)
  {
    typedef std::vector <PICML::ComponentImplementations> Folder_Set;
    Folder_Set folders = root.ComponentImplementations_children ();

    for (Folder_Set::iterator iter = folders.begin ();
         iter != folders.end ();
         iter ++)
    {
      iter->Accept (*this);
    }
  }

  //
  // Visit_ComponentImplemenations
  //
  void Cidlc_Visitor::Visit_ComponentImplementations (
    const PICML::ComponentImplementations & impls)
  {
    typedef std::vector <ComponentImplementationContainer> Container_Set;
    Container_Set container = impls.ComponentImplementationContainer_children ();

    for (Container_Set::iterator iter = container.begin ();
         iter != container.end ();
         iter ++)
    {
      iter->Accept (*this);
    }
  }

  //
  // Visit_ComponentImplementationContainer
  //
  void Cidlc_Visitor::Visit_ComponentImplementationContainer (
    const PICML::ComponentImplementationContainer & container)
  {
    // Initialize the <cidl_> structure for a new container.
    this->cidl_.valid_ = false;
    this->cidl_.cidlfile_ = container.name ();

    // Now we can visit all the monolithic implemenations contained
    // in this container.
    typedef std::vector <MonolithicImplementation> Monolithic_Set;
    Monolithic_Set monolithics =
      container.MonolithicImplementation_kind_children ();

    for (Monolithic_Set::iterator iter = monolithics.begin ();
          iter != monolithics.end ();
          iter ++)
    {
      iter->Accept(*this);
    }

    // Generate the CIDL file if we have a valid entry.
    if (this->cidl_.valid_)
      this->generate_cidl_file ();
  }

  //
  // Visit_MonolithicImplementation
  //
  void Cidlc_Visitor::Visit_MonolithicImplementation (
    const PICML::MonolithicImplementation & mono)
  {
    // We need to determine the type of component we are implementing
    // before we do any code generation. If we do not know this then
    // we just bailout.
    this->cidl_.monolithic_ = mono.name ();

    PICML::Implements implements = mono.dstImplements ();

    if (implements == Udm::null)
      return;

    PICML::ComponentRef component = implements.dstImplements_end ();
    PICML::Component type = component.ref ();

    if (type == Udm::null)
      return;

    // Visit the component defined in the interface definitions.
    type.Accept (*this);
  }

  //
  // Visit_Component
  //
  void Cidlc_Visitor::Visit_Component (const PICML::Component & component)
  {
    this->cidl_.component_ = component.name ();

    // Get the <ComponentFactory> for the <component> if there
    // is one present.
    typedef std::set <PICML::ManagesComponent> Manages_Set;
    Manages_Set manages = component.srcManagesComponent ();

    if (manages.size () > 0)
    {
      (*manages.begin ()).Accept (*this);
    }
    else
    {
      // Check to see if the home is located in another file. This is
      // realized by having a <ComponentRef> connected to a
      // <ComponentFactory>.
      typedef std::set <PICML::ComponentRef> ComponentRef_Set;
      ComponentRef_Set refs = component.referedbyComponentRef ();

      for (ComponentRef_Set::iterator iter = refs.begin ();
           iter != refs.end ();
           iter ++)
      {
        // Make sure the reference is part of the interface definitions
        // aspect in PICML.
        PICML::MgaObject parent = iter->parent ();
        std::string type = parent.type ().name ();

        if ((type == (std::string)PICML::Package::meta.name ()) ||
            (type == (std::string)PICML::File::meta.name ()))
        {
          iter->Accept (*this);
        }

        // We can exit the for loop since we have found a
        // valid home.
        if (this->cidl_.valid_)
          break;
      }
    }

    if (!this->cidl_.valid_)
    {
      // Ok, so we aren't valid at this point in time either. I guess
      // We have to create a "default" home for this component.
      this->cidl_.home_ = component.name ();
      this->cidl_.home_.append ("PICMLDefaultHome");

      this->generate_scope (component);
      this->cidl_.valid_ = true;
    }
  }

  //
  // Visit_ComponentRef
  //
  void Cidlc_Visitor::Visit_ComponentRef (const PICML::ComponentRef & ref)
  {
    // Get the <ComponentFactory> for the <component> if there
    // is one present.
    typedef std::set <PICML::ManagesComponent> Manages_Set;
    Manages_Set manages = ref.srcManagesComponent ();

    if (manages.size () > 0)
    {
      (*manages.begin ()).Accept (*this);
    }
  }

  //
  // Visit_ManagesComponent
  //
  void Cidlc_Visitor::Visit_ManagesComponent (
    const PICML::ManagesComponent & manages)
  {
    PICML::ComponentFactory factory = manages.srcManagesComponent_end ();
    factory.Accept (*this);
  }

  //
  // Visit_ComponentFactory
  //
  void Cidlc_Visitor::Visit_ComponentFactory (
    const PICML::ComponentFactory &factory)
  {
    // Save the name of the factory.
    this->cidl_.home_ = factory.name ();
    this->generate_scope (factory);

    // We now have enough information to generate the
    // composition for the implemenation.
    this->cidl_.valid_ = true;
  }

  //
  // Visit_NamedType
  //
  void Cidlc_Visitor::generate_scope (const PICML::NamedType & type)
  {
    this->cidl_.scope_.clear ();
    PICML::MgaObject parent = type.parent ();

    // Generate the code of the named type. This means backtracing
    // its hierarchy all the way to a <PICML::File>.
    if ((std::string)parent.type ().name () ==
        (std::string)PICML::Package::meta.name ())
    {
      this->cidl_.scope_.insert (0, "::");
      this->cidl_.scope_.insert (0, parent.name ());

      parent = PICML::MgaObject::Cast (parent.parent ());
    }

    // We now have the IDL file of the named type.
    if ((std::string)parent.type ().name () ==
        (std::string)PICML::File::meta.name ())
    {
      this->cidl_.idlfile_ = parent.name ();
    }
  }

  //
  // generate_cidl_file
  //
  void Cidlc_Visitor::generate_cidl_file (void)
  {
    std::ostringstream filename;
    filename
      << this->output_ << "\\"
      << this->cidl_.cidlfile_ << ".cidl" << std::ends;

    ofstream outfile;
    outfile.open (filename.str ().c_str ());

    if (outfile.is_open ())
    {
      // Create the #ifdef macro for the CIDL file.
      std::string hashdef = this->cidl_.cidlfile_;

      std::transform (hashdef.begin (),
                      hashdef.end (),
                      hashdef.begin (),
                      toupper);

      hashdef.insert (0, 1, '_');
      hashdef.append ("_CIDL_");

      // Generate the preamble.
      outfile
        << "// -*- CIDL -*-" << std::endl
        << std::endl
        << "// This file was generated by" << std::endl
        << "// $Id$" << std::endl
        << std::endl

        // Generate the opening hashdef.
        << "#ifndef " << hashdef << std::endl
        << "#define " << hashdef << std::endl
        << std::endl;

      // Generate the IDL include file.
      outfile
        << "#include \"" << this->cidl_.idlfile_ << ".idl\"" << std::endl
        << std::endl

        // Generate the composition.
        << "composition session " << this->cidl_.monolithic_ << std::endl
        << "{" << std::endl
        << "  home executor " << this->cidl_.home_ << "_Exec" << std::endl
        << "  {" << std::endl
        << "    implements " << this->cidl_.scope_
        << this->cidl_.home_ << ";" << std::endl
        << "    manages " << this->cidl_.component_ << "_Exec;" << std::endl
        << "  };" << std::endl
        << "};" << std::endl

        // Generate the closing hashdef
        << std::endl
        << "#endif  // !defined " << hashdef << std::endl;
      outfile.close ();
    }
  }
}
