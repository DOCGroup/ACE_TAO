// $Id$

#include "RepositoryManager_Impl.h"
#include "Update_Plan.h"
#include "DeploymentC.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"

ACE_RCSID (DAnCE,
           RepositoryManager,
           "$Id$")

using namespace Deployment;

namespace CIAO
{
  void
  traverse_package (PackageConfiguration* &pc,
                    DeploymentPlan &plan,
                    REF_MAP &ref_map,
                    REF_MAP &primary_ref_map)
  {
    // traverse the package configuration structure to get to the
    // BasePackage which consists of assemblies.
    //
    CORBA::ULong bp_len =
      pc->basePackage.length ();

    for (CORBA::ULong x = 0; x != bp_len; ++x)
      {
        CORBA::ULong impl_len =
          pc->basePackage[x].implementation.length ();

        for (CORBA::ULong y = 0;
             y != impl_len;
             ++y)
          {
            // traverse the .cpd file and get to the referenced .cid file
            //
            ComponentImplementationDescription cid =
              pc->basePackage[x].implementation[y].referencedImplementation;
            CORBA::ULong assembly_len = cid.assemblyImpl.length ();

            for (CORBA::ULong z = 0;
                 z != assembly_len;
                 ++z)
              {
                // traverse the .cid file and get to each
                // of the "assemblyImpl" tags.
                //
                ComponentAssemblyDescription assembly =
                  cid.assemblyImpl[z];
                //
                // traverse the individual assembly.
                //
                traverse_assembly (assembly, plan, ref_map, primary_ref_map);
              }
          }
      }
  }

  void
  traverse_assembly (ComponentAssemblyDescription &assembly,
                     DeploymentPlan &plan,
                     REF_MAP &ref_map, REF_MAP &primary_ref_map)
  {
    // traverse the assembly (ComponentAssemblyDescription) and
    // processes the instances and the connection within the assembly.
    //
    CORBA::ULong ins_len = assembly.instance.length ();
    for (CORBA::ULong k = 0; k < ins_len; ++k)
      {
        SubcomponentInstantiationDescription ins =
          assembly.instance[k];
        const char* in_name = ins.name;

      CORBA::ULong plan_ins_len =
        plan.instance.length ();

      for (CORBA::ULong l = 0; l < plan_ins_len; ++l)
        {
          const char* plan_name = plan.instance[l].name;
          if (strcmp (plan_name, in_name) == 0)
            {
              traverse_assembly_instance (ins, plan, l,
                                          ref_map, primary_ref_map);
            }
        }
      }

    CORBA::ULong assembly_conn_len =
      assembly.connection.length ();

    for (CORBA::ULong m = 0; m < assembly_conn_len; ++m)
      {
        AssemblyConnectionDescription
          assembly_connection = assembly.connection[m];
        traverse_assembly_connection (assembly,
                                      assembly_connection,
                                      plan);
      }
  }

  void
  traverse_assembly_connection (ComponentAssemblyDescription
                                &assembly,
                                AssemblyConnectionDescription
                                &assembly_connection,
                                DeploymentPlan &plan)
  {
    // traverse the assembly connection and get information about the
    // portName and the instances at each end of the connection.
    // Also traverse the InterfaceDescriptions for each of those instances
    // and populate the portKind information.
    //
    CORBA::ULong con_length (plan.connection.length ());
    plan.connection.length (con_length + 1);
    CORBA::ULong iepe_len = assembly_connection.internalEndpoint.length ();
    for (CORBA::ULong n = 0; n < iepe_len; ++n)
      {
        CORBA::ULong iep_len (plan.connection[con_length].
                              internalEndpoint.length ());
      plan.connection[con_length].internalEndpoint
        .length (iep_len + 1);
      plan.connection[con_length].internalEndpoint
        [iep_len].portName = assembly_connection.
        internalEndpoint[n].portName;
      CORBA::ULong ins_ref = assembly_connection.internalEndpoint[n].
        instanceRef;
      const char* ins_name = assembly.instance[ins_ref].name;
      CORBA::ULong plan_ins_len = plan.instance.length ();
      for (CORBA::ULong w = 0; w < plan_ins_len; ++w)
        {
          const char* pl_name = plan.instance[w].name;
          if (strcmp (pl_name, ins_name) == 0)
            {
              plan.connection[con_length].internalEndpoint
                [iep_len].instanceRef = w;
              break;
            }
        }
      traverse_interface (assembly.instance[ins_ref],
                          plan.connection[con_length].
                          internalEndpoint[iep_len]);
      }
  }

  void
  traverse_interface (SubcomponentInstantiationDescription
                      &instance,
                      PlanSubcomponentPortEndpoint
                      &pspe)
  {
    // traverse the InterfaceDescription of the instance and get information
    // about the portkind of the port.
    //
    CORBA::ULong pack_len = instance.package.length ();
    for (CORBA::ULong m = 0; m < pack_len; ++m)
      {
        ComponentPackageDescription
          package = instance.package[m];

        ComponentInterfaceDescription
          cid = package.realizes;

        CORBA::ULong port_len = cid.port.length ();

        for (CORBA::ULong n = 0; n < port_len; ++n)
          {
            const char* main_port_name = cid.port[n].name;
            const char* port_name = pspe.portName;
            if (strcmp (main_port_name, port_name) == 0)
              {
                pspe.kind = cid.port[n].kind;
              }
          }
      }
  }

  void
  traverse_assembly_instance (
                              SubcomponentInstantiationDescription
                              &instance,
                              DeploymentPlan &plan, int l,
                              REF_MAP &ref_map, REF_MAP &primary_ref_map)
  {
    // Each instance has a package.
    //   Each package has an implementation and their correspoding artifacts.
    // Traverse this information and populate the artifact and the
    // implementation information within the DeploymentPlan.
    //
    ART_REF_MAP art_ref_map;

    CORBA::ULong pack_len = instance.package.length ();

    for (CORBA::ULong m = 0; m < pack_len; ++m)
      {
        ComponentPackageDescription
          package = instance.package[m];
        CORBA::ULong pack_impl_len = package.implementation.length ();

        for (CORBA::ULong n = 0; n < pack_impl_len; ++n)
          {
            PackagedComponentImplementation
              impl = package.implementation[n];
            CORBA::ULong impl_length (plan.implementation.length ());
            plan.implementation.length (impl_length + 1);
            plan.implementation[impl_length].name = plan.instance[l].name;
            plan.instance[l].implementationRef = impl_length;
            CORBA::ULong mono_impl_len =
              impl.referencedImplementation.monolithicImpl.length ();

            for (CORBA::ULong p = 0; p < mono_impl_len; ++p)
              {
                MonolithicImplementationDescription
                  mid = impl.referencedImplementation.monolithicImpl[p];

                update_artifacts (mid, plan, plan.instance[l],
                                ref_map, primary_ref_map, art_ref_map,
                                  plan.implementation[impl_length]);
              }
            update_impl_config_property (impl, plan.implementation[impl_length],
                                         plan.instance[l]);
          }
      }
  }

  void
  update_artifacts (MonolithicImplementationDescription &mid,
                    DeploymentPlan &plan,
                    InstanceDeploymentDescription &instance,
                    REF_MAP &ref_map, REF_MAP &primary_ref_map,
                    ART_REF_MAP &art_ref_map,
                    MonolithicDeploymentDescription &mdd)
  {
    CORBA::ULong prim_art_len = mid.primaryArtifact.length ();
    for (CORBA::ULong q = 0; q < prim_art_len; ++q)
      {
        ImplementationArtifactDescription
          pack_iad = mid.primaryArtifact[q].referencedArtifact;
        ACE_TString artifact_name = (const char*)mid.primaryArtifact[q].name;
        int arti_len;
        CORBA::ULong art_length (plan.artifact.length ());

        if (ref_map.find (artifact_name, arti_len) != 0)
          {
            plan.artifact.length (art_length + 1);
            plan.artifact[art_length].name = mid.primaryArtifact[q].name;
            plan.artifact[art_length].node = instance.node;
            ref_map.bind (artifact_name, art_length);
            primary_ref_map.bind (artifact_name, art_length);
            CORBA::ULong art_ref_len (mdd.artifactRef.length ());
            mdd.artifactRef.length (art_ref_len + 1);
            mdd.artifactRef[art_ref_len] = art_length;
            update_artifact_location (pack_iad,
                                    plan.artifact[art_length]);
            update_artifact_property (pack_iad,
                                      plan.artifact[art_length]);
          }
        update_common_artifact_and_art_ref (pack_iad,
                                            primary_ref_map, ref_map,
                                          art_ref_map, mdd,
                                            plan, instance);
      }
  }

  void
  update_common_artifact_and_art_ref (
                                      ImplementationArtifactDescription
                                      &pack_iad,
                                      REF_MAP &primary_ref_map,
                                      REF_MAP &ref_map,
                                      ART_REF_MAP &art_ref_map,

                                      MonolithicDeploymentDescription &mid,
                                      DeploymentPlan &plan,

                                         InstanceDeploymentDescription
                                         &instance)
  {
    CORBA::ULong deps_len = pack_iad.dependsOn.length ();
    for (CORBA::ULong g = 0; g < deps_len; ++g)
      {
        ACE_TString dep_name =
          (const char*)pack_iad.dependsOn[g].name;
        int arti_len;

        if (ref_map.find (dep_name, arti_len) == 0)
          {
            if (primary_ref_map.find (dep_name, arti_len) != 0)
              {
                if (art_ref_map.find (arti_len, arti_len) != 0)
                  {
                    update_impl_art_ref (mid, arti_len);
                    art_ref_map.bind (arti_len, arti_len);
                  }
              }
          }
        else
          {
            ImplementationArtifactDescription
              depends_iad = pack_iad.dependsOn[g].
              referencedArtifact;
            CORBA::ULong new_art_length (plan.artifact.length ());
            plan.artifact.length (new_art_length + 1);
            plan.artifact[new_art_length].name =
              pack_iad.dependsOn[g].name;
            plan.artifact[new_art_length].node = instance.node;
            update_artifact_location (depends_iad,
                                      plan.artifact
                                      [new_art_length]);
            ref_map.bind (
                          (const char*)plan.artifact[new_art_length].name,
                          new_art_length);
            update_impl_art_ref (mid, new_art_length);
            art_ref_map.bind (new_art_length, new_art_length);
          }
      }
  }

  void
  update_impl_config_property (PackagedComponentImplementation
                               &impl,
                               MonolithicDeploymentDescription
                               &mid,

                                  InstanceDeploymentDescription
                               &instance)
  {
    CORBA::ULong pro_len =
    impl.referencedImplementation.configProperty.length ();

    for (CORBA::ULong x = 0; x < pro_len; ++x)
      {
        CORBA::ULong impl_pro_len (mid.execParameter.length ());
        mid.execParameter.length (impl_pro_len + 1);
        mid.execParameter[impl_pro_len]
          = impl.referencedImplementation.configProperty[x];
        CORBA::ULong ins_pro_len (instance.configProperty. length ());
        instance.configProperty.length (ins_pro_len + 1);
        instance.configProperty[ins_pro_len]
          = impl.referencedImplementation.configProperty[x];
      }
  }

  void
  update_impl_art_ref (MonolithicDeploymentDescription &mid,
                       int arti_len)
  {
    CORBA::ULong new_art_ref_len (mid.artifactRef.length ());
    mid.artifactRef.length (new_art_ref_len + 1);
    mid.artifactRef[new_art_ref_len] = arti_len;
  }

  void
  update_artifact_location (ImplementationArtifactDescription
                            &pack_iad,
                            ArtifactDeploymentDescription
                            &plan_artifact)
  {
    CORBA::ULong loc_len = pack_iad.location.length ();
    for (CORBA::ULong e = 0; e < loc_len; ++e)
      {
        CORBA::ULong art_loc_len (plan_artifact.location.length ());
        plan_artifact.location.length (art_loc_len + 1);
        plan_artifact.location[art_loc_len] = pack_iad.location[e];
      }
  }

  void
  update_artifact_property (ImplementationArtifactDescription
                            &pack_iad,
                            ArtifactDeploymentDescription
                            &plan_artifact)
  {
    CORBA::ULong para_len = pack_iad.execParameter.length ();
    for (CORBA::ULong f = 0; f < para_len; ++f)
      {
        CORBA::ULong art_pro_len (plan_artifact.execParameter.length ());
        plan_artifact.execParameter.length (art_pro_len + 1);
        plan_artifact.execParameter[art_pro_len] = pack_iad.execParameter[f];
      }
  }
}
