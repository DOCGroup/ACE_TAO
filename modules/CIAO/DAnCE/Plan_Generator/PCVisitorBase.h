/* -*- C++ -*- */

//========================================================================
/**
 *  file  PCVisitorBase.h
 *
 *  $Id$
 *
 *  This file contains the virtual base class for the PackageConfiguration
 *  Visitor which is used to traverse the PackageConfiguration element
 *  defined in the PackagingData.idl. The PackageConfiguration has a
 *  number of sequence elements. This class actually implements the
 *  operations which involve sequences and delegates the calls to
 *  the operations which handle single elements from the sequence type.
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//========================================================================

#ifndef PC_VISITOR_BASE_H
#define PC_VISITOR_BASE_H

#include /**/ "ace/pre.h"
#include "DAnCE/Deployment/DeploymentC.h"

//========================================================================
/**
 *  class PCVisitorBase
 *
 *  This class is a virtual base class for the PackageConfiguration Visitor
 *  The operation which deal with sequences are implemented here by means
 *  of the visit_sequence function above, in order to make the logic of
 *  derived classes easier to write. Writers of derived classes need only
 *  overload the functions which deal with single elements. Sequences are
 *  always handled here.
 */
//========================================================================


class  PCVisitorBase
{
public:
  /// Constructor
  PCVisitorBase (void);

  /// Destructor
  virtual ~PCVisitorBase (void);

  /// Function what dispatches sequences
  template <typename SEQ>
  friend void visit_sequence (SEQ &seq, PCVisitorBase& v);

  /// A whole slew of overloaded routines for different IDL
  /// data types part of the PackageConfiguration.

  virtual
  void Visit (Deployment::PackageConfiguration &pc) = 0;
  void Visit (Deployment::PackageConfigurations &pcs);

  /// ComponentPackageDescription descendents
  virtual
  void Visit (Deployment::ComponentPackageDescription &cpd) = 0;
  void Visit (Deployment::ComponentPackageDescriptions &cpds);

  virtual
  void Visit (Deployment::ComponentInterfaceDescription &cid) = 0;
  //void Visit (Deployment::ComponentInterfaceDescriptions &cids);

  virtual
  void Visit (Deployment::PackagedComponentImplementation &pci) = 0;
  void Visit (Deployment::PackagedComponentImplementations &pcis);

  virtual
  void Visit (Deployment::ComponentImplementationDescription &cid) = 0;
  //void Visit (Deployment::ComponentImplementationDescriptions &cids);

  virtual
  void Visit (Deployment::ComponentAssemblyDescription &cad) = 0;
  void Visit (Deployment::ComponentAssemblyDescriptions &cads);

  virtual
  void Visit (Deployment::SubcomponentInstantiationDescription &sid) = 0;
  void Visit (Deployment::SubcomponentInstantiationDescriptions &sids);

  virtual
  void Visit (Deployment::MonolithicImplementationDescription &mid) = 0;
  void Visit (Deployment::MonolithicImplementationDescriptions &mids);

  virtual
  void Visit (Deployment::NamedImplementationArtifact &nia) = 0;
  void Visit (Deployment::NamedImplementationArtifacts &nias);

  virtual
  void Visit (Deployment::ImplementationArtifactDescription &iad) = 0;
  //void Visit (Deployment::ImplementationArtifactDescriptions &iads);

  //ComponentPackageReference descendents
  virtual
  void Visit (Deployment::ComponentPackageReference &cpr) = 0;
  void Visit (Deployment::ComponentPackageReferences &cprs);

  //properties
  virtual
  void Visit (Deployment::AssemblyPropertyMapping &apm) = 0;
  void Visit (Deployment::AssemblyPropertyMappings &apms);

  virtual
  void Visit (Deployment::Property &property) = 0;
  void Visit (Deployment::Properties &properties);

  //requirements & capabilities
  virtual
  void Visit (Deployment::Requirement &requirement) = 0;
  void Visit (Deployment::Requirements &requirements);

  virtual
  void Visit (Deployment::Capability &capability) = 0;
  void Visit (Deployment::Capabilities &capabilities);

  virtual
  void Visit (Deployment::ImplementationRequirement &ir) = 0;
  void Visit (Deployment::ImplementationRequirements &irs);

  virtual
  void Visit (Deployment::ImplementationDependency &id) = 0;
  void Visit (Deployment::ImplementationDependencies &ids);

  //ports and connections
  virtual
  void Visit (Deployment::AssemblyConnectionDescription &acd) = 0;
  void Visit (Deployment::AssemblyConnectionDescriptions &acds);

  virtual
  void Visit (Deployment::SubcomponentPortEndpoint &spe) = 0;
  void Visit (Deployment::SubcomponentPortEndpoints &spes);

  virtual
  void Visit (Deployment::ComponentExternalPortEndpoint &cepe) = 0;
  void Visit (Deployment::ComponentExternalPortEndpoints &cepes);
};

template <typename T>
void Accept (PCVisitorBase &v, T& element_to_visit)
{
  v.Visit (element_to_visit);
}

/**
 *  function - visit_sequence
 *
 *  This function is used to handle sequences of elements where each
 *  element takes the form of a Visitor Node.
 */
template <typename SEQ>
void visit_sequence (SEQ &seq, PCVisitorBase& v)
{
   CORBA::ULong const size = seq.length ();

   for (CORBA::ULong i = 0; i < size; ++i)
   {
     Accept(v, seq[i]);
   }
}

#if defined (__ACE_INLINE__)
#include "PCVisitorBase.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* PC_VISITOR_BASE_H */
