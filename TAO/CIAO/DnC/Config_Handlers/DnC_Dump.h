//==================================================================
/**
 *  @file  DnC_Dump.h
 *
 *  $Id$
 *
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=====================================================================

#ifndef DNC_DUMP_H
#define DNC_DUMP_H
#include /**/ "ace/pre.h"

#include "../Core/DeploymentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace Deployment
{
  /**
   * @class DnC_Dump
   *
   * @brief This class defines a set of overloaded "dump" methods to print
   * the contents of various IDL data types, which passed as input.
   */
  class DnC_Dump
    {
    public:
      /// Dump functions for CORBA sequences
      static void DnC_Dump::dump (const char* caption, const ::CORBA::StringSeq &str_seq);

      template <typename SEQUENCE>
        static void dump_sequence (const char* caption, const SEQUENCE &seq);

      /// A whole slew of overloaded dump routines for different IDL data types.

      static void dump (const ::Deployment::AssemblyConnectionDescription &acd);

      static void dump (const ::Deployment::AssemblyPropertyMapping &apm);

      static void dump (const ::Deployment::ComponentPackageDescription &comppkgdesc);

      static void dump (const ::Deployment::ComponentPortDescription &compportdesc);

      static void dump (const ::Deployment::ComponentPropertyDescription &comppropdesc);

      static void dump (const ::Deployment::MonolithicImplementationDescription &mid);

      static void dump (const ::Deployment::PackageConfiguration &pc);

      static void dump (const ::Deployment::PackagedComponentImplementation &pci);

      static void dump (const ::Deployment::SubcomponentPortEndpoint &spe);

      static void dump (const ::Deployment::Requirement &requirement);

      static void dump (const ::Deployment::ComponentExternalPortEndpoint &cepe);

      static void dump (const ::Deployment::ComponentPackageReference &cpr);

      static void dump (const ::Deployment::ComponentImplementationDescription &cid);

      static void dump (const ::Deployment::SubcomponentInstantiationDescription &sid);

      static void dump (const ::Deployment::Property &property);

      static void dump (const ::Deployment::NamedImplementationArtifact &named_implementation);

      static void dump (const ::Deployment::ComponentInterfaceDescription &cid);

      static void dump (const ::Deployment::SubcomponentPropertyReference &scpr);

      static void dump (const ::Deployment::ExternalReferenceEndpoint &ere);

      static void dump (const ::Deployment::Domain &domain);

      static void dump (const ::Deployment::Capability &capability);

      static void dump (const ::Deployment::ImplementationArtifactDescription &iad);

      static void dump (const ::Deployment::ImplementationRequirement &ir);

      static void dump(const Deployment::SatisfierProperty &sp);

      static void dump(const Deployment::ResourceUsageKind &ruk);

      static void dump(const Deployment::DeploymentPlan &plan);
      ///   ... Zillions of such things ....

    private:
      static void dump (const ::CORBA::Any &any);
      // Helper function to print an Any
    };
}

#include /**/ "ace/post.h"

#endif /* DNC_DUMP_H */
