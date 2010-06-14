// $Id$

#include "Locality_Splitter.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/DAnCE/DAnCE_PropertiesC.h"
#include "DAnCE/DAnCE/DAnCE_Utility.h"
#include "ace/UUID.h"
#include <algorithm>

namespace DAnCE
{

  Locality_Splitter::LocalityKey::LocalityKey ()
    : loc_manager_ ((CORBA::ULong)ULONG_MAX)
    {
    }

  Locality_Splitter::LocalityKey::LocalityKey (
      const Locality_Splitter::LocalityKey& k)
    : node_ (k.node ()),
      instances_ (k.instances ()),
      loc_manager_ (k.loc_manager_)
    {
    }

  Locality_Splitter::LocalityKey&
  Locality_Splitter::LocalityKey::operator =(
      const Locality_Splitter::LocalityKey& k)
    {
      this->node_ = k.node ();
      this->instances_ = k.instances ();
      this->loc_manager_ = k.loc_manager_;
      return *this;
    }

  const ACE_CString& Locality_Splitter::LocalityKey::node () const
    {
      return this->node_;
    }

  void Locality_Splitter::LocalityKey::node (const ACE_CString &node_name)
    {
      this->node_ = node_name;
    }

  const Locality_Splitter::TInstanceList&
  Locality_Splitter::LocalityKey::instances () const
    {
      return this->instances_;
    }

  bool Locality_Splitter::LocalityKey::has_instance (CORBA::ULong instance) const
    {
      Locality_Splitter::TInstanceList::iterator it =
          std::find (const_cast<Locality_Splitter::LocalityKey*> (this)
                        ->instances_.begin (),
                     const_cast<Locality_Splitter::LocalityKey*> (this)
                        ->instances_.end (),
                     instance);
      return it != this->instances_.end ();
    }

  void Locality_Splitter::LocalityKey::add_instance (CORBA::ULong instance)
    {
      this->instances_.push_back (instance);
    }

  bool Locality_Splitter::LocalityKey::has_locality_manager () const
    {
      return this->loc_manager_ < this->instances_.size ();
    }

  void Locality_Splitter::LocalityKey::locality_manager_instance (CORBA::ULong instance_index)
    {
      if (instance_index < this->instances_.size ())
        this->loc_manager_ = instance_index;
    }

  CORBA::ULong Locality_Splitter::LocalityKey::locality_manager_instance () const
    {
      return this->has_locality_manager () ? this->loc_manager_ : 0;
    }

  bool Locality_Splitter::LocalityKey::operator ==(
      const Locality_Splitter::LocalityKey &other_key) const
    {
      return (this->node_ == other_key.node () &&
                  std::equal (this->instances_.begin (),
                              this->instances_.end (),
                              other_key.instances ().begin ()));
    }

  Locality_Splitter::LocalityKey::operator ACE_CString ()
    {
      char buf[16];
      ACE_CString id (this->node_);
      ACE_OS::snprintf (buf, sizeof (buf), "-%u", this);
      id += buf;
      return id;
    }

  Locality_Splitter::LocalityFilter::LocalityFilter ()
    {
    }

  const ACE_CString& Locality_Splitter::LocalityFilter::node () const
    {
      return this->node_;
    }

  void Locality_Splitter::LocalityFilter::node (const ACE_CString &node_name)
    {
      this->node_ = node_name;
    }

  const Locality_Splitter::TInstanceNames&
  Locality_Splitter::LocalityFilter::included_instances () const
    {
      return this->included_instances_;
    }

  bool Locality_Splitter::LocalityFilter::includes_instance (
      const ACE_CString &instance_name) const
    {
      Locality_Splitter::TInstanceNames::iterator it =
          std::find (const_cast<Locality_Splitter::LocalityFilter*> (this)
                        ->included_instances_.begin (),
                     const_cast<Locality_Splitter::LocalityFilter*> (this)
                        ->included_instances_.end (),
                     instance_name);
      return it != this->included_instances_.end ();
    }

  void Locality_Splitter::LocalityFilter::add_instance (
      const ACE_CString &instance_name)
    {
      this->included_instances_.push_back (instance_name);
    }

  Locality_Splitter::Locality_Splitter (const Deployment::DeploymentPlan &plan)
    : plan_ (plan)
    {
    }

  bool Locality_Splitter::match_instance (
      CORBA::ULong instance,
      const FILTER &filter) const
    {
      // check if the node for the instance matches the filter
      bool match =
          filter.node ().is_empty () ||
            (ACE_OS::strcmp (filter.node ().c_str (),
                             this->plan_.instance[instance].node.in ()) == 0);
      if (match)
        {
          // check if instance explicitly included or any allowed
          match = filter.included_instances ().empty () ||
                    filter.includes_instance (
                      this->plan_.instance[instance].node.in ());
        }
      return match;
    }

  bool Locality_Splitter::match_sub_plan (
      CORBA::ULong instance,
      const KEY &sub_plan_key) const
    {
      // check if the node for the instance matches the sub plan
      bool match =
            (ACE_OS::strcmp (sub_plan_key.node ().c_str (),
                             this->plan_.instance[instance].node.in ()) == 0);

      // if the node matches see if the instance is or can be included in the
      // sub plan
      if (match)
        {
          if (sub_plan_key.has_instance (instance))   return true; // matches

          // see if the locality constraints allow this instance to be
          // included in this sub plan
          CORBA::ULong num_localities = this->plan_.localityConstraint.length ();
          for (CORBA::ULong l = 0; l < num_localities; ++l)
            {
              const Deployment::PlanLocality &ploc = this->plan_.localityConstraint[l];
              // only interested in real constraints
              if (ploc.constraint != Deployment::PlanNoConstraint)
                {
                  // see if the instance we're matching is
                  // included in this constraint
                  bool apply_constraint = false;
                  for (CORBA::ULong i = 0;
                       i < ploc.constrainedInstanceRef.length ();
                       ++i)
                    {
                      apply_constraint = ploc.constrainedInstanceRef[i] == instance;
                      if (apply_constraint)   break;
                    }

                  if (ploc.constraint == Deployment::PlanDifferentProcess)
                    {
                      if (apply_constraint) // in case the constraint applies ...
                        {
                          // check if there are no incompatible instances already in
                          // the sub plan we're matching
                          for (CORBA::ULong i = 0;
                              i < ploc.constrainedInstanceRef.length ();
                              ++i)
                            {
                              if (ploc.constrainedInstanceRef[i] != instance)
                                {
                                  CORBA::ULong incompatible_instance =
                                      ploc.constrainedInstanceRef[i];
                                  if (sub_plan_key.has_instance (
                                                incompatible_instance))
                                    {
                                      // sub plan includes an instance which
                                      // is constrained to a different locality
                                      return false;
                                    }
                                }
                            }
                        }
                      // in case the constraint does not apply to the instance
                      // we're matching forget about it
                    }
                  else // Deployment::PlanSameProcess
                    {
                      if (apply_constraint) // in case the constraint applies ...
                        {
                          // check to see that all instances included in the
                          // sub plan also comply with this constraint
                          // since if one complies with the constraint than all should,
                          // it is enough to test the first instance in the sub plan
                          if (!sub_plan_key.instances ().empty ())
                            {
                              CORBA::ULong first_inst_nr = sub_plan_key.instances ().front ();
                              bool included = false;
                              for (CORBA::ULong j = 0;
                                  j < ploc.constrainedInstanceRef.length ();
                                  ++j)
                                {
                                  if (first_inst_nr == ploc.constrainedInstanceRef[j])
                                    {
                                      included = true;
                                      break;
                                    }
                                }
                              if (!included)
                                {
                                  // sub plan includes instance(s) which do(es) not
                                  // comply with this constraint
                                  return false;
                                }
                            }
                        }
                      else
                        {
                          // check to see that the constraint also does *not* apply to
                          // any of the instances included in the sub plan
                          // since if one complies with the constraint than all should,
                          // it is enough to test the first instance in the sub plan
                          if (!sub_plan_key.instances ().empty ())
                            {
                              CORBA::ULong first_inst_nr = sub_plan_key.instances ().front ();
                              for (CORBA::ULong j = 0;
                                  j < ploc.constrainedInstanceRef.length ();
                                  ++j)
                                {
                                  if (first_inst_nr == ploc.constrainedInstanceRef[j])
                                    {
                                      // sub plan complies with this constraint
                                      return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
      return match;
    }

  void Locality_Splitter::prepare_sub_plan (
      CORBA::ULong instance,
      Deployment::DeploymentPlan &sub_plan,
      KEY &sub_plan_key)
    {
      if (ACE_OS::strcmp (sub_plan_key.node ().c_str (),
                          this->plan_.instance[instance].node.in ()) != 0)
        {
          // set sub plan key to node name for instance
          sub_plan_key.node (this->plan_.instance[instance].node.in ());
        }
      if (!sub_plan_key.has_instance (instance))
        {
          sub_plan_key.add_instance (instance);

          if (!sub_plan_key.has_locality_manager ())
            {
              // check if this is the LocalityManager instance
              CORBA::ULong impl_idx = this->plan_.instance[instance].implementationRef;

              if (impl_idx >= this->plan_.implementation.length ())
                {
                  DANCE_ERROR (1, (LM_ERROR, DLINFO
                                  ACE_TEXT("Locality_Splitter::prepare_sub_plan - ")
                                  ACE_TEXT("Invalid implementation index\n")));
                }
              else
                {
                  const char *instance_type =
                    DAnCE::Utility::get_instance_type (plan_.implementation[impl_idx].execParameter);
                  if (instance_type)
                    {
                      if (ACE_OS::strcmp (instance_type,
                                          DANCE_LOCALITYMANAGER) == 0)
                        {
                          // mark locality manager instance offset
                          sub_plan_key.locality_manager_instance (sub_plan_key.instances ().size () - 1);

                          DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                                          ACE_TEXT ("Locality_Splitter::prepare_sub_plan - ")
                                          ACE_TEXT ("Found locality manager instance ")
                                          ACE_TEXT ("%u:%C\n"),
                                          instance,
                                          plan_.instance[instance].name.in ()
                                          ));
                        }
                    }
                }
            }
        }
      if (ACE_OS::strlen (sub_plan.label.in ()) == 0)
        {
          // derive descriptive label
          ACE_CString sub_label ("Split plan from ");
          if (this->plan_.label.in ())
            {
              sub_label += this->plan_.label.in ();
            }
          else
            {
              sub_label += this->plan_.UUID.in ();
            }
          sub_label += " for a Locality on Node ";
          sub_label += sub_plan_key.node ();
          sub_plan.label = CORBA::string_dup (sub_label.c_str ());
        }
    }

  void Locality_Splitter::finalize_sub_plan (
      Deployment::DeploymentPlan &sub_plan,
      KEY &sub_plan_key)
    {
      // check for availability of LocalityManager instance
      if (!sub_plan_key.has_locality_manager ())
        {
          // add minimal default LocalityManager instance to sub plan
          CORBA::ULong impl_index = sub_plan.implementation.length ();
          sub_plan.implementation.length (impl_index + 1);
          CORBA::ULong inst_index = sub_plan.instance.length ();
          sub_plan.instance.length (inst_index + 1);
          sub_plan.instance[inst_index].implementationRef = impl_index;

          // set correct implementation type
          Deployment::Property exec_property;
          exec_property.name = CORBA::string_dup (IMPL_TYPE);
          exec_property.value <<= DANCE_LOCALITYMANAGER;
          sub_plan.implementation[impl_index].execParameter.length (1);
          sub_plan.implementation[impl_index].execParameter[0] = exec_property;

          // create unique name for Locality Manager
          ACE_Utils::UUID uuid;
          ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);
          ACE_CString lm_name ("Locality Manager [");
          lm_name += uuid.to_string ()->c_str ();
          lm_name += "]";
          sub_plan.instance[inst_index].name = CORBA::string_dup (lm_name.c_str ());

          DANCE_DEBUG (4, (LM_DEBUG, DLINFO
                           ACE_TEXT ("Locality_Splitter::finalize_sub_plan - ")
                           ACE_TEXT ("No locality manager found, created a default locality ")
                           ACE_TEXT ("named <%C>\n"),
                           sub_plan.instance[inst_index].name.in ()
                           ));

          // add instance to sub plan key
          sub_plan_key.add_instance (inst_index);
          // mark as locality manager
          sub_plan_key.locality_manager_instance (sub_plan_key.instances ().size () - 1);
        }
    }
}
