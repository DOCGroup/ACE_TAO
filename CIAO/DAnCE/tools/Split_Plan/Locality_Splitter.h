// $Id$

/**
 * @file Locality_Splitter.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_LOCALITY_SPLITTER_H
#define DANCE_LOCALITY_SPLITTER_H

#include "ace/SStringfwd.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Split_Plan/DAnCE_Split_Plan_export.h"
#include <vector>

namespace DAnCE
{
  class DAnCE_Split_Plan_Export Locality_Splitter
  {
  public:
    typedef std::vector<CORBA::ULong>  TInstanceList;

    class DAnCE_Split_Plan_Export LocalityKey
    {
    public:
      LocalityKey ();
      LocalityKey (const LocalityKey& k);

      LocalityKey& operator =(const LocalityKey& k);

      const ACE_CString&    node () const;
      void                  node (const ACE_CString &node_name);

      const TInstanceList&  instances () const;
      bool                  has_instance (CORBA::ULong instance) const;
      void                  add_instance (CORBA::ULong instance);

      bool                  has_locality_manager () const;
      CORBA::ULong          locality_manager_instance () const;
      void                  locality_manager_instance (CORBA::ULong sub_plan_index);

      bool  operator ==(const LocalityKey &other_key) const;

      operator ACE_CString ();

    private:
      ACE_CString node_;
      TInstanceList instances_;
      CORBA::ULong loc_manager_;
    };

    typedef LocalityKey       KEY;

    typedef std::vector<ACE_CString>  TInstanceNames;

    class DAnCE_Split_Plan_Export LocalityFilter
    {
    public:
      LocalityFilter ();

      const ACE_CString&    node () const;
      void                  node (const ACE_CString &node_name);

      const TInstanceNames& included_instances () const;
      bool                  includes_instance (
                                const ACE_CString &instance_name) const;
      void                  add_instance (const ACE_CString &instance_name);

    private:
      ACE_CString node_;
      TInstanceNames included_instances_;
    };

    typedef LocalityFilter    FILTER;

    Locality_Splitter (const Deployment::DeploymentPlan &plan);

    bool    match_instance (CORBA::ULong instance,
                            const FILTER &filter) const;

    bool    match_sub_plan (CORBA::ULong instance,
                            const KEY &sub_plan_key) const;

    void    prepare_sub_plan (CORBA::ULong instance,
                              Deployment::DeploymentPlan &sub_plan,
                              KEY &sub_plan_key);

    void    finalize_sub_plan (Deployment::DeploymentPlan &sub_plan,
                               KEY &sub_plan_key);

  private:
    const Deployment::DeploymentPlan &plan_;
  };
}
#endif
