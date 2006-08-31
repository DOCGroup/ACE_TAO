// $Id$

#include "Project_Generator.h"

#if !defined (__CUTS_INLINE__)
#include "Project_Generator.inl"
#endif

#include "boost/bind.hpp"
#include "Uml.h"
#include "cuts/be/Dependency_Graph.h"
#include <algorithm>

//
// CUTS_BE_Project_Generator
//
CUTS_BE_Project_Generator::CUTS_BE_Project_Generator (void)
{

}

//
// ~CUTS_BE_Project_Generator
//
CUTS_BE_Project_Generator::~CUTS_BE_Project_Generator (void)
{

}

//
// Visit_ComponentImplementationContainer
//
void CUTS_BE_Project_Generator::
Visit_ComponentImplementationContainer (
const PICML::ComponentImplementationContainer & container)
{
  // Lets make sure we have update the <graph_>. This means
  // setting the <DNF_STUB> flag on all nodes that contain
  // a component we are implementing.
  typedef std::vector <PICML::MonolithicImplementation> Mono_Set;
  Mono_Set monos = container.MonolithicImplementation_kind_children ();

  std::for_each (monos.begin (),
                monos.end (),
                boost::bind (&Mono_Set::value_type::Accept,
                              _1,
                              boost::ref (*this)));

  // Initialize the output directory for the manager and
  // allow the manager to parse the rest of the model.
  this->manager_.init (this->outdir_);
  PICML::ComponentImplementationContainer (container).Accept (this->manager_);

  // We can now write the project for this container.
  this->write_project (container, this->manager_.preprocess_data ());
}

//
// Visit_MonolithicImplementation
//
void CUTS_BE_Project_Generator::
Visit_MonolithicImplementation (const PICML::MonolithicImplementation & mono)
{
  PICML::Implements implements = mono.dstImplements ();

  if (implements != Udm::null)
    implements.Accept (*this);
}

//
// Visit_Implements
// 
void CUTS_BE_Project_Generator::
Visit_Implements (const PICML::Implements & implements)
{
  PICML::ComponentRef ref = implements.dstImplements_end ();
  PICML::Component component = ref.ref ();

  if (component != Udm::null)
    component.Accept (*this);
}

//
// Visit_Implements
// 
void CUTS_BE_Project_Generator::
Visit_Component (const PICML::Component & component)
{
  if (this->graph_ == 0)
    return;

  PICML::MgaObject parent = component.parent ();

  while ((std::string) parent.type ().name () !=
         (std::string) PICML::File::meta.name ())
  {
    parent = PICML::MgaObject::Cast (parent.parent ());
  }

  CUTS_Dependency_Node * node = 0;
  this->graph_->find_node (parent.name (), node);
  node->flags_ |= CUTS_Dependency_Node::DNF_STUB;
}

//
// write_stub_project
//
bool CUTS_BE_Project_Generator::
write_stub_project (CUTS_Dependency_Node * node)
{
  return false;
}
