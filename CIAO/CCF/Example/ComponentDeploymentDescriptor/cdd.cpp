// file      : CCF/Example/ComponentDeploymentDescriptor/cdd.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <iostream>

#include "CCF/CodeGenerationKit/IndentationXML.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

#include "SyntaxTree.hpp"
#include "Traversal.hpp"

using std::cout;
using std::cerr;
using std::endl;

using namespace CDD;

/*
struct CI_Emitter : public virtual Traversal::ComponentInstance
{
  virtual void
  pre (NodePtr const& n)
  {
    cout << "component instance " << n->name () << " of type "
         << n->type () << endl << "{" << endl;

  }

  virtual void
  post (NodePtr const& n)
  {
    cout << "}" << endl;
  }

};

struct PI_Emitter : public virtual Traversal::PortInstance
{
  virtual void
  traverse (NodePtr const& n)
  {
    cout << "  port " << n->name () << ";" << endl;
  }
};

struct C_Emitter : public virtual Traversal::Connection
{
  virtual void
  traverse (NodePtr const& n)
  {
    cout << "connection " << n->left ()->name ()
         << "<--->" << n->right ()->name () << ";" << endl;
  }
};

*/


struct CI_Emitter : public virtual Traversal::ComponentInstance
{
  virtual void
  pre (NodePtr const& n)
  {
    cout << "<ComponentInstance name=\"" << n->name () << "\" type=\""
         << n->type () << "\" >" << endl;

  }

  virtual void
  post (NodePtr const& n)
  {
    cout << "</ComponentInstance>" << endl << endl;
  }

};

struct PI_Emitter : public virtual Traversal::PortInstance
{
  virtual void
  traverse (NodePtr const& n)
  {
    cout << "<Port name=\"" << n->name () << "\" />" << endl;
  }
};

struct C_Emitter : public virtual Traversal::Connection
{
  virtual void
  traverse (NodePtr const& n)
  {
    cout << "<Connection left=\"" << n->left ()->name () << "\" "
         << "right=\"" << n->right ()->name () << "\" />" << endl;
  }
};

//FUZZ: disable check_for_improper_main_declaration

int
main ()
{
  using namespace SyntaxTree;

  try
  {
    // Construct tree

    DescriptorPtr d (new Descriptor);

    PortInstancePtr ap (new PortInstance ("ap"));
    ComponentInstancePtr a (new ComponentInstance ("a", "A"));
    a->insert (ap);
    d->insert (a);

    PortInstancePtr bp (new PortInstance ("bp"));
    ComponentInstancePtr b (new ComponentInstance ("b", "B"));
    b->insert (bp);
    d->insert (b);

    ConnectionPtr c (new Connection (ap, bp));
    d->insert (c);

    // Construct Traverser

    Traversal::Descriptor dt;

    CI_Emitter cie;
    PI_Emitter pie;
    cie.add_port_delegate (&pie);

    C_Emitter ce;

    dt.add_node_delegate (&cie);
    dt.add_node_delegate (&ce);

    // Set auto-indentation for cout
    Indentation::Implanter<Indentation::XML> guard (cout);

    dt.traverse (d);
  }
  catch (...)
  {
    cerr << "caught something" << endl;
  }
}
