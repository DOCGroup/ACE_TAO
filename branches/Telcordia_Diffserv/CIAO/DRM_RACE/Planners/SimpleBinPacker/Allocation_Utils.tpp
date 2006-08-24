#ifndef ALLOCATON_UTILS_TPP
#define ALLOCATON_UTILS_TPP

#include "Planners/SimpleBinPacker/Allocation_Utils.h"
#include "ciao/Deployment_DataC.h"


namespace CIAO
{
  namespace RACE
  {
    namespace Allocation_Utils
    {
      //      const char * cpu_resource_name = "Processor";
      //const char * cpu_resource_type = "CPULoad";
      //const char * cpu_resource_prop_name = "LoadAverage";

#define cpu_resource_name  "Processor"
#define cpu_resource_type  "CPULoad"
#define cpu_resource_prop_name  "LoadAverage"
#define instance_dynamic_assignable_name "DynamicAssignable"

      template <typename Bins,
                typename Items>
      Format_Converter<Bins, Items>::Format_Converter (const Deployment::Domain *domain,
                                                       Deployment::InstanceDeploymentDescriptions *idds,
                                                       Deployment::MonolithicDeploymentDescriptions *mdds)
      : idds_ (idds),
        domain_ (domain)
      {
        // Convert Domain information into suitable bins
        for (CORBA::ULong i = 0;
             i < domain->node.length ();
             ++i)
        {
          const ::Deployment::Node &node = domain->node[i];

          for (CORBA::ULong j = 0;
               j < node.resource.length ();
               ++j)
          {
            if (ACE_OS::strcmp (node.resource[j].name.in (),
                                cpu_resource_name) == 0)
              {
                const ::Deployment::Resource &resource = node.resource[j];

                for (CORBA::ULong m = 0;
                     m < resource.property.length ();
                     ++m)
                  {
                    if (ACE_OS::strcmp (resource.property[m].name.in (),
                                        cpu_resource_prop_name) == 0)
                      {
                        ::CORBA::Double capacity;
                        if (!(resource.property[m].value >>= capacity))
                          {
                            //                            CIAO_ERROR (0, "Allocation_Utils::Extraction from Any failed.\n");
                            break;
                          }

                        // Push bin onto the bin vector
                        this->bins_.push_back (typename Bins::value_type (i, typename Bins::value_type::capacity_type (capacity)));

                        // Great, we have found a bin.  Lets go:
                        break;
                      }
                  }
                break;
              }
          }
        }

        ACE_DEBUG ((LM_DEBUG, "IDDs\n"));

        // Convert MDD/IDD information into suitable items
        for (CORBA::ULong i = 0;
             i < idds_->length ();
             ++i)
        {
          CORBA::Boolean assignable (true); // Assume the instance is assignable unless we hear otherwise...

          // Check config property for dynamic assignable....
          for (CORBA::ULong j = 0;
               j < (*idds_)[i].configProperty.length ();
               ++j)
            {
              if (ACE_OS::strcmp ((*idds_)[i].configProperty[j].name.in (),
                                  instance_dynamic_assignable_name) == 0)
                {
                  ACE_InputCDR::to_boolean val (assignable);
                  (*idds_)[i].configProperty[j].value >>= val;

                  break; // Found what we are looking for, move along...
                }
            }

          if (!assignable) continue;

          // Make an item for the instance....
          typename Items::value_type item (i, 0);

          Deployment::MonolithicDeploymentDescription mdd
            = (*mdds)[(*idds_)[i].implementationRef];

          /*
          for (CORBA::ULong m = 0;
               m < mdd.deployRequirement.length ();
               ++m)
          {
          ACE_DEBUG ((LM_DEBUG, "\tExamining %s\n",
          mdd.deployRequirement[m].resourceType.in ()));
          ACE_DEBUG ((LM_DEBUG, "\tExamining %s\n",
                        mdd.deployRequirement[m].name.in ()));

            if ((ACE_OS::strcmp (mdd.deployRequirement[m].resourceType.in (),
                                 cpu_resource_type) == 0) &&
                (ACE_OS::strcmp (mdd.deployRequirement[m].name.in (),
                                 cpu_resource_name) == 0))
              {
                // Aha! Found what we are looking for... Now find the property...
                ::Deployment::Properties &prop = mdd.deployRequirement[m].property;

                for (CORBA::ULong j = 0;
                     j < prop.length ();
                     ++j)
                  {
                    ACE_DEBUG ((LM_DEBUG, "\t\tExamining %s\n",
                                prop[j].name.in ()));

                    if (ACE_OS::strcmp (prop[j].name.in (),
                                        cpu_resource_prop_name) == 0)
                      {
                        // extract the value and populate the bin....
                        CORBA::Double value;
                        prop[j].value >>= value;
                        item.size = value;
                        break;
                      }
                  }
                break;
              }
          }
          */

          for (CORBA::ULong m = 0;
               m < mdd.execParameter.length ();
               ++i)
            {
              if (ACE_OS::strcmp (mdd.execParameter[m].name.in (),
                                  cpu_resource_prop_name) == 0)
                {
                  // Found an interesting property...
                  CORBA::Double value;
                  mdd.execParameter[m].value >>= value;
                  item.size = value;
                  break;
                }
            }


          items_.push_back (item);
        }
      }

    template <typename Bins, typename Items>
    bool
    Format_Converter<Bins, Items>::commit_allocation (const Items &allocated_items)
    {
      for (typename Items::const_iterator i = allocated_items.begin ();
           i != allocated_items.end ();
           ++i)
      {
        if (i->allocated_to_bin == -1) return false;

        // Build the IRDD to record the allocation for the TM
        Deployment::InstanceResourceDeploymentDescription irdd;

        irdd.resourceUsage = Deployment::InstanceUsesResource;
        irdd.requirementName = cpu_resource_name;
        irdd.resourceName = cpu_resource_type;
        irdd.property.length (1);

        irdd.property[0].name = cpu_resource_prop_name;
        irdd.property[0].value <<= i->size;

        // Record the allocation for the plan
        // the item contains an index into the bin vector, which has
        // a bin number which is an index into the sequence of nodes.

        (*this->idds_)[i->item_num].node =
         (* this->domain_).node[this->bins_[i->allocated_to_bin].bin_num].name.in ();

        CORBA::ULong pos = (*this->idds_)[i->item_num].deployedResource.length ();
        (*this->idds_)[i->item_num].deployedResource.length (pos + 1);
        (*this->idds_)[i->item_num].deployedResource[pos] = irdd;

      }

      return true;
    }

    }
  }
}


#endif /* ALLOCATON_UTILS_TPP */
