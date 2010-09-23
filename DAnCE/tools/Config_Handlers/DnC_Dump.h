//==================================================================
/**
*  @file  DnC_Dump.h
*
* $Id$
*
*  @author Gan Deng <gan.deng@vanderbilt.edu>
*/
//=====================================================================

#ifndef DNC_DUMP_H
#define DNC_DUMP_H
#include /**/ "ace/pre.h"

#include "Deployment/Deployment_Packaging_DataC.h"
#include "Deployment/Deployment_TargetDataC.h"
#include "Config_Handlers_Export.h"

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
class Config_Handlers_Export DnC_Dump
{
public:
/// Dump functions for CORBA sequences
static void dump (const char* caption,
const ::CORBA::StringSeq &str_seq);

static void dump (const char* caption, const TAO::String_Manager& str);

static void dump (const char* caption, const CORBA::Boolean& val);

/// A whole slew of overloaded dump routines for different IDL
/// data types.

static void dump(const Deployment::SatisfierProperty &sp);

static void dump (const Deployment::SharedResource& sr);

static void dump (const Deployment::Node& node);

static void dump (const Deployment::Resource& resource);

static void dump (const Deployment::Interconnect& conn);

static void dump (const Deployment::Bridge& bridge);

static void dump (const ::Deployment::Property &property);

static void dump (const ::Deployment::AssemblyConnectionDescription &acd);

static void dump (const ::Deployment::AssemblyPropertyMapping &apm);

static void dump (const ::Deployment::ComponentPackageDescription
&comppkgdesc);

static void dump (const ::Deployment::ComponentPortDescription
&compportdesc);

static void dump (const ::Deployment::ComponentPropertyDescription
&comppropdesc);

static void dump (const ::Deployment::MonolithicImplementationDescription
&mid);

static void dump (const ::Deployment::PackageConfiguration &pc);

static void dump (const ::Deployment::PackagedComponentImplementation
&pci);

static void dump (const ::Deployment::SubcomponentPortEndpoint &spe);

static void dump (const ::Deployment::Requirement &requirement);

static void dump (const ::Deployment::ComponentExternalPortEndpoint
&cepe);

static void dump (const ::Deployment::ComponentPackageReference &cpr);

static void dump (const ::Deployment::ComponentImplementationDescription
&cid);

static void dump (const ::Deployment::SubcomponentInstantiationDescription
&sid);

static void dump (const ::Deployment::NamedImplementationArtifact
&named_implementation);

static void dump (const ::Deployment::ComponentInterfaceDescription &cid);

static void dump (const ::Deployment::SubcomponentPropertyReference
&scpr);

static void dump (const ::Deployment::ExternalReferenceEndpoint &ere);

static void dump (const ::Deployment::Domain &domain);

static void dump (const ::Deployment::Capability &capability);

static void dump (const ::Deployment::ImplementationArtifactDescription
&iad);

static void dump (const ::Deployment::ImplementationRequirement &ir);

static void dump(const Deployment::ResourceUsageKind &ruk);

static void dump(const Deployment::DeploymentPlan &plan);

static void dump(const Deployment::MonolithicDeploymentDescription &mdd);

static void dump(const Deployment::InstanceDeploymentDescription &idd);

static void dump(const Deployment::PlanConnectionDescription &pcd);

static void dump(const Deployment::PlanPropertyMapping &ppm);

static void dump(const Deployment::ImplementationDependency &id);

static void dump(const Deployment::ArtifactDeploymentDescription &add);

static void dump(const Deployment::InstanceResourceDeploymentDescription
&irdd);

static void dump(const PlanSubcomponentPortEndpoint &pspe);

static void dump(const ConnectionResourceDeploymentDescription &crdd);

static void dump(const PlanSubcomponentPropertyReference &pspr);

static void dump(const ResourceDeploymentDescription &rdd);

static void dump (const ::Deployment::ComponentAssemblyDescription& cad);

private:
static void dump (const ::CORBA::Any &any);
// Helper function to print an Any
};
}

#include /**/ "ace/post.h"

#endif /* DNC_DUMP_H */
