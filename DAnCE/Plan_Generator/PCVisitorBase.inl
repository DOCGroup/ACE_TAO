/* -*- C++ -*- */

//========================================================================
/*
 *  file  PCVisitorBase.inl
 *
 * $Id$
 *
 *  This file contains the implementation of the sequence-based
 *  PackageConfiguration Visitor functions. By implementing these
 *  we relieve developers of derived classes of the burden of handling
 *  the correct traversal of sequence-based elements. This implementation
 *  used the visit_sequence templatized function to do the heavy lifting.
 *
 *  author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 */
//========================================================================


/// A whole slew of overloaded routines for different IDL
/// data types part of the PackageConfiguration.

ACE_INLINE void PCVisitorBase::Visit
(Deployment::PackageConfigurations &pcs)
{
  visit_sequence (pcs, *this);
}

//ComponentPackageDescription descendents

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ComponentPackageDescriptions &cpds)
{
  visit_sequence (cpds, *this);
}

//NOT a member of Deployment

//ACE_INLINE void PCVisitorBase::Visit
//    (Deployment::ComponentInterfaceDescriptions &cids)
//{
//  visit_sequence (cids, *this);
//}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::PackagedComponentImplementations &pcis)
{
  visit_sequence (pcis, *this);
}

//NOT a member of Deployment

//ACE_INLINE void PCVisitorBase::Visit
//    (Deployment::ComponentImplementationDescriptions &cids)
//{
//  visit_sequence (cids, *this);
//}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ComponentAssemblyDescriptions &cads)
{
  visit_sequence (cads, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::SubcomponentInstantiationDescriptions &sids)
{
  visit_sequence (sids, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::MonolithicImplementationDescriptions &mids)
{
  visit_sequence (mids, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::NamedImplementationArtifacts &nias)
{
  visit_sequence (nias, *this);
}

//ACE_INLINE void PCVisitorBase::Visit
//    (Deployment::ImplementationArtifactDescriptions &iads)
//{
//  visit_sequence (iads, *this);
//}

//ComponentPackageReference descendents

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ComponentPackageReferences &cprs)
{
  visit_sequence (cprs, *this);
}

//properties

ACE_INLINE void PCVisitorBase::Visit
(Deployment::AssemblyPropertyMappings &apms)
{
  visit_sequence (apms, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::Properties &properties)
{
  visit_sequence (properties, *this);
}

//requirements & capabilities

ACE_INLINE void PCVisitorBase::Visit
(Deployment::Requirements &requirements)
{
  visit_sequence (requirements, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::Capabilities &capabilities)
{
  visit_sequence (capabilities, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ImplementationRequirements &irs)
{
  visit_sequence (irs, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ImplementationDependencies &ids)
{
  visit_sequence (ids, *this);
}

//ports and connections

ACE_INLINE void PCVisitorBase::Visit
(Deployment::AssemblyConnectionDescriptions &acds)
{
  visit_sequence (acds, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::SubcomponentPortEndpoints &spes)
{
  visit_sequence (spes, *this);
}

ACE_INLINE void PCVisitorBase::Visit
(Deployment::ComponentExternalPortEndpoints &cepes)
{
  visit_sequence (cepes, *this);
}
