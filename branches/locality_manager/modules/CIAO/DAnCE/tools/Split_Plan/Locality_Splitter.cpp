// $Id$

#include "Locality_Splitter.h"
#include "DAnCE/Logger/Log_Macros.h"
#include <algorithm>

namespace DAnCE
{

  Locality_Splitter::LocalityKey::LocalityKey ()
    {
    }
    
  Locality_Splitter::LocalityKey::LocalityKey (
      const Locality_Splitter::LocalityKey& k)
    : node_ (k.node ()),
      instances_ (k.instances ())
    {
    }

  Locality_Splitter::LocalityKey&
  Locality_Splitter::LocalityKey::operator =(
      const Locality_Splitter::LocalityKey& k)
    {
      this->node_ = k.node ();
      this->instances_ = k.instances ();
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

                  if (apply_constraint)
                    {
                      if (ploc.constraint == Deployment::PlanDifferentProcess)
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
                      else
                        {
                          // check to see that all instances included in the
                          // sub plan comply with this constraint
                          for (TInstanceList::const_iterator i =
                                    sub_plan_key.instances ().begin ();
                               i != sub_plan_key.instances ().end ();
                               ++i)
                            {
                              bool included = false;
                              for (CORBA::ULong j = 0;
                                  j < ploc.constrainedInstanceRef.length ();
                                  ++j)
                                {
                                  if ((*i) == ploc.constrainedInstanceRef[j])
                                    {
                                      included = true;
                                      break;
                                    }
                                }
                              if (!included)
                                {
                                  // sub plan includes an instance which is not
                                  // constraint to the same locality
                                  return false;
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
}
