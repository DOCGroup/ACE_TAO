/**
 *  @file
 *  @ingroup ImportanceBinPacker
 *  @brief C++ implementation of the ImportanceBinPacker component.
 *
 *  @copydoc ImportanceBinPacker
 *  $Id$
 */

#include "ace/Bound_Ptr.h"
#include "ace/Null_Mutex.h"
#include <set>
#include <string>
#include <vector>
#include "ibp.h"
#include "ImportanceBinPacker.h"

// Special size used for Apps that don't have a LoadAverage!
#define DEFAULT_APP_SIZE 5

// Data Structures
namespace
{
  struct App_Record
  {
    std::string name_;
    CORBA::Double size_;
    std::string node_;
    std::string plan_uuid_;

    App_Record (const char * name,
                CORBA::Double size,
                const char * node,
                const char * plan_uuid)
      : name_ (name),
        size_ (size),
        node_ (node),
        plan_uuid_ (plan_uuid)
    {}
  };

  typedef std::vector < ACE_Strong_Bound_Ptr <App_Record, ACE_Null_Mutex> > App_List;

  struct Bin_Record
  {
    std::string name;
    CORBA::Double capacity;
    App_List apps;
  };

  typedef std::vector <Bin_Record> Bin_List;
}

// Algorithms
namespace
{
  struct App_Matcher :
    public std::binary_function <App_List::value_type, std::string, bool>
  {
    result_type operator () (const first_argument_type & x,
                             const second_argument_type & y)
      const
    {
      return x.get() && x->name_ == y;
    }
  };

  typedef std::binder2nd <App_Matcher> App_Finder;

  struct Bin_Matcher :
    public std::binary_function <Bin_Record, std::string, bool>
  {
    result_type operator () (const first_argument_type & x,
                             const second_argument_type & y)
      const
    {
      return x.name == y;
    }
  };

  typedef std::binder2nd <Bin_Matcher> Bin_Finder;
}

// More Data Structures
namespace
{
  class Plan_Record
  {
  private:
    std::string uuid_;
    mutable CORBA::Long importance_;
    ACE_Strong_Bound_Ptr < ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan_;
    ACE_Strong_Bound_Ptr < ::Deployment::PackageConfiguration, ACE_Null_Mutex> package_;
    App_List apps_;

  public:

    Plan_Record (const ::Deployment::DeploymentPlan & plan,
                 const ::Deployment::PackageConfiguration & package)
      : uuid_ (plan.UUID.in ()),
        importance_ (0),
        plan_ (new ::Deployment::DeploymentPlan (plan)),
        package_ (new ::Deployment::PackageConfiguration (package)),
        apps_ (plan.instance.length (),
               App_List::value_type (0))
    {
      // Grab the Importance
      {
        CORBA::Any importance;
        ::Deployment::ComponentPackageDescriptions & basePackage =
            this->package_->basePackage;
        if (basePackage.length()) {
          ::Deployment::ComponentPackageDescription & cpd = basePackage[0];
          ::Deployment::PackagedComponentImplementations & implementation = cpd.implementation;
          if (implementation.length()) {
            ::Deployment::PackagedComponentImplementation & first_impl = implementation[0];
            ::Deployment::ComponentImplementationDescription & cid = first_impl.referencedImplementation;
            ::Deployment::Properties & configProperties = cid.configProperty;
            for (CORBA::ULong i = 0, l = configProperties.length();
                 i < l;
                 ++i)
            {
              ::Deployment::Property & configProperty = configProperties[i];
              if (ACE_OS::strcmp(configProperty.name.in(), "Importance") == 0)
              {
                importance = configProperty.value;
              }
            }
          }
        }
        ::CORBA::TypeCode_var tcv = importance.type();
        ::CORBA::TCKind       tck = tcv->kind();
        if (tck == ::CORBA::tk_long) {
          importance >>= this->importance_;
        }
      }

      // Populate the list of apps
      ::Deployment::InstanceDeploymentDescriptions & instances =
          this->plan_->instance;
      ::Deployment::MonolithicDeploymentDescriptions & impls =
          this->plan_->implementation;

      for (CORBA::ULong i = 0;
           i < instances.length ();
           ++i)
      {
        ::Deployment::InstanceDeploymentDescription & instance = instances[i];
        ::Deployment::MonolithicDeploymentDescription & impl  = impls[instance.implementationRef];
        App_List::value_type app(new App_Record("", DEFAULT_APP_SIZE, "", uuid_.c_str()));
        CORBA::Any app_size;

        // Take application name from the name of the instance.
        if (instance.name.in() != 0)
          app->name_.assign (instance.name.in());

        // Take application size from the LoadAverage execParameter on
        // the implementation
        for (CORBA::ULong m = 0;
             m < impl.execParameter.length ();
             ++m)
        {
          if (ACE_OS::strcmp (impl.execParameter[m].name.in (),
                              "LoadAverage") == 0)
          {
            app_size = impl.execParameter[m].value;
            break;
          }
        }

        ::CORBA::TypeCode_var tcv = app_size.type();
        ::CORBA::TCKind       tck = tcv->kind();
        if (tck == ::CORBA::tk_double)
          app_size >>= app->size_;
        else if (tck == ::CORBA::tk_long) {
          CORBA::Long temp;
          app_size >>= temp;
          app->size_ = (CORBA::Double) temp;
        }

        this->apps_[App_List::size_type(i)] = app;
      }
    }

    const std::string & uuid () const
    {
      return this->uuid_;
    }

    CORBA::Long importance () const
    {
      return this->importance_;
    }

    void importance(CORBA::Long new_importance) const
    {
      // Change the Importance
      ::Deployment::ComponentPackageDescriptions & basePackage =
          this->package_->basePackage;
      if (basePackage.length()) {
        ::Deployment::ComponentPackageDescription & cpd = basePackage[0];
        ::Deployment::PackagedComponentImplementations & implementation = cpd.implementation;
        if (implementation.length()) {
          ::Deployment::PackagedComponentImplementation & first_impl = implementation[0];
          ::Deployment::ComponentImplementationDescription & cid = first_impl.referencedImplementation;
          ::Deployment::Properties & configProperties = cid.configProperty;
          for (CORBA::ULong i = 0, l = configProperties.length();
               i < l;
               ++i)
          {
            ::Deployment::Property & configProperty = configProperties[i];
            if (ACE_OS::strcmp(configProperty.name.in(), "Importance") == 0)
            {
              configProperty.value <<= new_importance;
              this->importance_ = new_importance;
            }
          }
        }
      }
    }

    template <class Visitor>
    void visit_apps (Visitor & visitor) const
    {
      const App_List & const_apps = this->apps_;
      for (App_List::const_iterator i = const_apps.begin(), e = const_apps.end();
           i < e;
           ++i)
      {
        visitor(**i);
      }
    }

    void assign_apps (std::vector<std::string> node_assignments,
                      Bin_List & bins)
    {
      while (node_assignments.size () < this->apps_.size ())
        node_assignments.push_back ("");

      std::vector<std::string>::const_iterator i2 = node_assignments.begin();
      ::CORBA::ULong i3 = 0;

      for (App_List::const_iterator i1 = this->apps_.begin(),
             e1 = this->apps_.end();
           i1 < e1;
           ++i1, ++i2, ++i3)
      {
        std::string old_node = (**i1).node_;
        Bin_List::iterator old_bin = std::find_if (bins.begin(),
                                                   bins.end(),
                                                   Bin_Finder (Bin_Matcher(), old_node));
        if (old_bin != bins.end()) {
          App_List::iterator old_position = std::find (old_bin->apps.begin(),
                                                       old_bin->apps.end(),
                                                       *i1);
          if (old_position != old_bin->apps.end())
            old_bin->apps.erase (old_position);
        }

        std::string new_node = *i2;
        (**i1).node_ = new_node;
        this->plan_->instance[i3].node = CORBA::string_dup(new_node.c_str());

        Bin_List::iterator new_bin = std::find_if (bins.begin(),
                                                   bins.end(),
                                                   Bin_Finder (Bin_Matcher(), new_node));
        if (new_bin != bins.end())
          new_bin->apps.push_back(*i1);
      }
    }

    void get_plan (::Deployment::DeploymentPlan & plan,
                   ::Deployment::PackageConfiguration & package) const
    {
      plan = *(this->plan_);
      package = *(this->package_);
    }
  };
}

// Algorithms
namespace std
{
  template<> struct less <Plan_Record>
  {
    bool operator() (const Plan_Record& x, const Plan_Record& y) const
    {
      return x.uuid() < y.uuid();
    }
  };
}

namespace
{
  struct Importance_Less :
    public std::binary_function <Plan_Record, Plan_Record, bool>
  {
    result_type operator () (const first_argument_type & x,
                             const second_argument_type & y)
      const
    {
      return x.importance() < y.importance();
    }
  };
}

// More data structures
namespace
{
  typedef std::set<Plan_Record> Plan_Set;
}

// More Algorithms
namespace
{
  struct Plan_Matcher :
    public std::binary_function <Plan_Record, std::string, bool>
  {
    result_type operator () (const first_argument_type & x,
                             const second_argument_type & y)
      const
    {
      return x.uuid() == y;
    }
  };

  typedef std::binder2nd <Plan_Matcher> Plan_Finder;

  struct Importance_Less_Than :
    public std::unary_function <Plan_Record, bool>
  {
    CORBA::Long importance_;

    Importance_Less_Than (CORBA::Long importance)
      : importance_ (importance) {}

    result_type operator () (const argument_type & x) const
    {
      return x.importance() < importance_;
    }
  };
}

struct ImportanceBinPacker::Impl
{
  Plan_Set deployed_plans;
  Bin_List bins;

  struct Transformer :
    public std::binary_function <std::string, double, Bin_Record>
  {
    result_type operator () (const first_argument_type&  x,
                             const second_argument_type& y)
    {
      Bin_Record retval;
      retval.name = x;
      retval.capacity = y;
      return retval;
    }
  };

  Impl (char ** bin_names,
        double ** bin_capacities)
  {
    std::vector<std::string> bin_name_list;

    for (char** i = bin_names;
         *i != 0;
         ++i)
    {
      bin_name_list.push_back (*i);
    }

    std::vector<double> bin_cap_list;

    for (double** i = bin_capacities;
         *i != 0;
         ++i)
    {
      bin_cap_list.push_back (**i);
    }

    while (bin_cap_list.size() < bin_name_list.size())
      bin_cap_list.push_back(1.0);

    std::transform (bin_name_list.begin(),
                    bin_name_list.end(),
                    bin_cap_list.begin(),
                    std::back_inserter (bins),
                    Transformer ());
  }

  void print_bins (std::ostream & out) const
  {
    for (Bin_List::const_iterator i (this->bins.begin()),
           e (this->bins.end());
         i < e;
         ++i)
    {
      out << "name:\t\"" << i->name << "\"\tcapacity:\t" << i->capacity << "\n";
      App_List::const_iterator ii (i->apps.begin()),
        ee (i->apps.end());
      if (ii != ee) {
        out << "{\n";
        double consumed = 0.0;
        for (;ii < ee;
             ++ii)
        {
          out << "\t" << (**ii).size_ << "\t" << (**ii).name_ << "\n";
          consumed += (**ii).size_;
        }
        out << "\t-----\n";
        out << "\t" << consumed << "\n";
        out << "}\n";
      }
    }

    if (this->deployed_plans.size())
    {
      out << "\nDeployed Plans:\n\n"
          << "Imp\tUUID\n";
      for (Plan_Set::const_iterator i (this->deployed_plans.begin()),
             e (this->deployed_plans.end());
           i != e;
           ++i)
      {
        out << i->importance() << "\t" << i->uuid() << "\n";
      }
    }
  }

private:
  bool deploy_plan (ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan_in,
                    ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> package_in,
                    std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > & plan_out,
                    std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > & package_out,
                    std::vector<Action_Enum> & action_out);

  bool teardown_plan (ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan_in,
                      ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> package_in,
                      std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > & plan_out,
                      std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > & package_out,
                      std::vector<Action_Enum> & action_out,
                      bool autoTeardown = false );

  void append (Plan_Seq & out,
               ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan,
               ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> package,
               Action_Enum action)
  {
    CORBA::ULong old_length = out.length();
    out.length(old_length + 1);
    out[old_length].plan = *plan;
    out[old_length].package = *package;
    out[old_length].command = action;
  }

public:
  bool convert_input (const Plan_Seq & in,
                      Plan_Seq & out)
  {
    bool ret_val = true;

    for (CORBA::ULong in_iter = 0, in_size = in.length();
         in_iter < in_size;
         ++in_iter)
    {

      const Plan_Struct & plan_struct = in[in_iter];
      if (plan_struct.command == Action_Types::TEARDOWN)
      {
        ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex>
          plan_for_teardown (new ::Deployment::DeploymentPlan(plan_struct.plan));
        ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex>
          package_for_teardown (new ::Deployment::PackageConfiguration(plan_struct.package));
        std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > result_plans;
        std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > result_packages;
        std::vector<Action_Enum> result_actions;
        ret_val = teardown_plan (plan_for_teardown, package_for_teardown,
                       result_plans, result_packages, result_actions);
        std::vector<Action_Enum>::size_type result_size =
          result_plans.size() < result_packages.size() ? result_plans.size() : result_packages.size();
        result_size = result_size < result_actions.size() ? result_size : result_actions.size();
        for (std::vector<Action_Enum>::size_type result_iter = 0;
             result_iter < result_size;
             ++result_iter)
        {
          append(out, result_plans[result_iter], result_packages[result_iter], result_actions[result_iter]);
        }
      }
      if (plan_struct.command == Action_Types::DEPLOY)
      {
        ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex>
          plan_for_deployment (new ::Deployment::DeploymentPlan(plan_struct.plan));
        ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex>
          package_for_deployment (new ::Deployment::PackageConfiguration(plan_struct.package));
        std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > result_plans;
        std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > result_packages;
        std::vector<Action_Enum> result_actions;
        ret_val = deploy_plan (plan_for_deployment, package_for_deployment,
                     result_plans, result_packages, result_actions);
        std::vector<Action_Enum>::size_type result_size =
          result_plans.size() < result_packages.size() ? result_plans.size() : result_packages.size();
        result_size = result_size < result_actions.size() ? result_size : result_actions.size();
        for (std::vector<Action_Enum>::size_type result_iter = 0;
             result_iter < result_size;
             ++result_iter)
        {
          append(out, result_plans[result_iter], result_packages[result_iter], result_actions[result_iter]);
        }
      }
    }
    return ret_val;
  }

  bool domain_change (const Domain_Change_List & changes,
                      char ** joined_nodes,
                      double ** joined_node_capacities,
                      Plan_Seq & out)
  {
    bool retval = true;

    // Implemented without REDAC. //

    // Add joined nodes.
    {
      std::vector<std::string> bin_name_list;

      for (char** i = joined_nodes;
           i != 0 && *i != 0;
           ++i)
      {
        bin_name_list.push_back (*i);
      }

      std::vector<double> bin_cap_list;

      for (double** i = joined_node_capacities;
           i != 0 && *i != 0;
           ++i)
      {
        bin_cap_list.push_back (**i);
      }

      while (bin_cap_list.size() < bin_name_list.size())
        bin_cap_list.push_back(100.0);

      std::transform (bin_name_list.begin(),
                      bin_name_list.end(),
                      bin_cap_list.begin(),
                      std::back_inserter (bins),
                      Transformer ());
    }

    // Find which nodes have failed.
    std::vector<std::string> failed_nodes;
    for (CORBA::ULong i = 0, l = changes.length(); i < l; ++i) {
      if (changes[i].change == Domain_Change_Types::NODE_FAILED)
        failed_nodes.push_back(changes[i].hostname.in());
    }

    // Create a set of all Plan UUIDs that need to be torn down.
    std::set<std::string> teardown_uuids;
    for (std::vector<std::string> ::iterator i (failed_nodes.begin()),
           e (failed_nodes.end());
         i != e;
         ++i)
    {
      Bin_List::iterator failed_bin = std::find_if (bins.begin(),
                                                    bins.end(),
                                                    Bin_Finder (Bin_Matcher(), *i));

      if (failed_bin != bins.end ()) {
        for (App_List::iterator ii (failed_bin->apps.begin()),
               ee (failed_bin->apps.end());
             ii != ee;
             ++ii)
          {
            teardown_uuids.insert((*ii)->plan_uuid_);
          }
      }
    }

    // Create a Plan_Seq of all actual plans that need to be torn
    // down.  The plans are torn down in order of increasing
    // importance.
    std::vector <Plan_Record> damaged_plans;
    for (std::set<std::string> ::iterator i (teardown_uuids.begin()),
           e (teardown_uuids.end());
         i != e;
         ++i)
    {
      Plan_Set::iterator damaged_plan =
        std::find_if(deployed_plans.begin(),
                     deployed_plans.end(),
                     Plan_Finder (Plan_Matcher(), *i));

      if (damaged_plan != deployed_plans.end())
        damaged_plans.push_back(*damaged_plan);
    }

    std::sort (damaged_plans.begin(), damaged_plans.end(), Importance_Less());
    Plan_Seq damaged_plans_corba(damaged_plans.size());
    damaged_plans_corba.length(0);
    for (std::vector<Plan_Record> ::iterator i (damaged_plans.begin()),
           e (damaged_plans.end());
         i != e;
         ++i)
    {
      ACE_Strong_Bound_Ptr < ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan
        (new ::Deployment::DeploymentPlan);
      ACE_Strong_Bound_Ptr < ::Deployment::PackageConfiguration, ACE_Null_Mutex> package
        (new ::Deployment::PackageConfiguration);

      i->get_plan (*plan, *package);

      this->append (damaged_plans_corba, plan, package, Action_Types::TEARDOWN);
    }

    // Tear down the damaged plans!
    retval = this->convert_input (damaged_plans_corba, out);

    // Remove failed bins
    for (std::vector<std::string> ::iterator i (failed_nodes.begin()),
           e (failed_nodes.end());
         i != e;
         ++i)
    {
      for (Bin_List::size_type ii = 0;
           ii < bins.size();
           ++ii)
      {
        if (bins[ii].name == *i) {
          for (Bin_List::size_type iii = ii;
               iii < bins.size() - 1;
               ++iii)
          {
            bins[iii] = bins[iii+1];
          }
          bins.pop_back();
        }
      }
    }

    // Try redeploying the damaged plans, in order of decreasing importance
    std::reverse (damaged_plans.begin(), damaged_plans.end());
    damaged_plans_corba.length(0);
    for (std::vector<Plan_Record> ::iterator i (damaged_plans.begin()),
           e (damaged_plans.end());
         i != e;
         ++i)
    {
      ACE_Strong_Bound_Ptr < ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan
        (new ::Deployment::DeploymentPlan);
      ACE_Strong_Bound_Ptr < ::Deployment::PackageConfiguration, ACE_Null_Mutex> package
        (new ::Deployment::PackageConfiguration);

      i->get_plan (*plan, *package);

      this->append (damaged_plans_corba, plan, package, Action_Types::DEPLOY);
    }
    retval = this->convert_input (damaged_plans_corba, out);

    // For all output plans, increment the action from
    // TEARDOWN/DEPLOY/AUTO_TEARDOWN to
    // REPLAN_TEARDOWN/REPLAN_DEPLOY/REPLAN_AUTO_TEARDOWN
    for (CORBA::ULong i = 0, l = out.length(); i < l; ++i) {
      if (out[i].command == Action_Types::TEARDOWN)
        out[i].command = Action_Types::REPLAN_TEARDOWN;
      else if (out[i].command == Action_Types::DEPLOY)
        out[i].command = Action_Types::REPLAN_DEPLOY;
      else if (out[i].command == Action_Types::AUTO_TEARDOWN)
        out[i].command = Action_Types::REPLAN_AUTO_TEARDOWN;
    }

    return retval;
  }
};

namespace
{
  struct Forward_Bin_Transformer :
    public std::unary_function<Bin_Record, SBP::bin1>
  {
    int bin_num;

    Forward_Bin_Transformer() : bin_num(0) {}

    result_type operator()(const argument_type& x)
    {
      double remaining_capacity = x.capacity;
      for (App_List::const_iterator i = x.apps.begin();
           i != x.apps.end();
           ++i)
      {
        remaining_capacity -= (**i).size_;
      }

      return SBP::bin1 (bin_num++, remaining_capacity);
    }
  };

  struct Name_Gathering_Visitor :
    public std::unary_function<App_Record, void>
  {
    std::vector<std::string> & names_;
    std::vector<SBP::item> & items_;
    int item_num;

    Name_Gathering_Visitor(std::vector<std::string> & names,
                           std::vector<SBP::item> & items) :
      names_(names),
      items_(items),
      item_num(0)
    {}

    result_type operator()(const argument_type& x)
    {
      names_.push_back(x.name_);
      items_.push_back(SBP::item(item_num++, x.size_));
    }
  };

  struct SBP_Item_Sorter :
    public std::binary_function <SBP::item, SBP::item, bool>
  {
    result_type operator() (const first_argument_type& x,
                            const second_argument_type& y)
    {
      return x.item_num < y.item_num;
    }
  };
}

bool ImportanceBinPacker::Impl::deploy_plan (ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan_in,
                                             ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> package_in,
                                             std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > & plan_out,
                                             std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > & package_out,
                                             std::vector<Action_Enum> & action_out)
{
  bool ret_val = true;
  // Search for a plan with the same UUID as the given plan
  Plan_Set::iterator uuid_check =
    std::find_if(deployed_plans.begin(),
                 deployed_plans.end(),
                 Plan_Finder (Plan_Matcher(),
                              std::string(plan_in->UUID.in())));

  if (uuid_check == deployed_plans.end())
  {
    // New plan
    Plan_Record planrec (*plan_in, *package_in);

    bool keep_going = true;
    do {
      // Populate vector of SBP::bin1
      std::vector<SBP::bin1> sbp_bins;
      std::transform (this->bins.begin(),
                      this->bins.end(),
                      std::back_inserter(sbp_bins),
                      Forward_Bin_Transformer());

      // Get apps and app names from the new plan
      std::vector<std::string> app_names;
      std::vector<SBP::item> sbp_apps;
      Name_Gathering_Visitor visitor(app_names, sbp_apps);
      planrec.visit_apps<Name_Gathering_Visitor> (visitor);

      // If packing succeeds...
      if (allocate(sbp_bins, sbp_apps))
      {
        //... get a string representation of the node assignments, in
        //    the right order!
        std::sort (sbp_apps.begin(), sbp_apps.end(), SBP_Item_Sorter());
        std::vector<std::string> node_assignments;

        for (std::vector<SBP::item>::iterator i (sbp_apps.begin()), e (sbp_apps.end());
             i != e;
             ++i)
        {
          if (0                                        <= i->allocated_to_bin &&
              Bin_List::size_type(i->allocated_to_bin) <  Bin_List::size_type(this->bins.size()))
          {
            node_assignments.push_back(this->bins[i->allocated_to_bin].name);
          }
        }

        // Assign the items in the Plan Record
        planrec.assign_apps (node_assignments, this->bins);

        // Remember the Plan Record
        this->deployed_plans.insert (planrec);

        // Put the plan and package out, along with a DEPLOY action
        ACE_Strong_Bound_Ptr < ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan
          (new ::Deployment::DeploymentPlan);
        ACE_Strong_Bound_Ptr < ::Deployment::PackageConfiguration, ACE_Null_Mutex> package
          (new ::Deployment::PackageConfiguration);

        planrec.get_plan (*plan, *package);

        plan_out.push_back    (plan);
        package_out.push_back (package);
        action_out.push_back  (Action_Types::DEPLOY);

        // No (more) need to tear down a lower priority plan.
        keep_going = false;
      }
      else
      {
        // Tear down a lower priority plan
        Plan_Set::iterator lower_priority_plan =
          std::find_if (deployed_plans.begin(),
                        deployed_plans.end(),
                        Importance_Less_Than(planrec.importance()));
        if (lower_priority_plan != deployed_plans.end()) {
          ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex>
            lpp_plan (new ::Deployment::DeploymentPlan());
          ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex>
            lpp_package (new ::Deployment::PackageConfiguration());
          lower_priority_plan->get_plan(*lpp_plan, *lpp_package);
          teardown_plan (lpp_plan, lpp_package, plan_out, package_out, action_out, true);
        } else {
          keep_going = false;
          ret_val = false;
          // *** TOM: if ultimately the plan fails, should not have torn anything down! ***
          // *** TOM: should revert to original input plan/package ***
        }
      }
    } while (keep_going);
  }
  else
  {
    // This plan is already deployed.  Just change the importance.
    CORBA::Long new_importance = uuid_check->importance();

    // Grab the Importance
    {
      CORBA::Any new_importance_any;
      ::Deployment::ComponentPackageDescriptions & basePackage =
          package_in->basePackage;
      if (basePackage.length()) {
        ::Deployment::ComponentPackageDescription & cpd = basePackage[0];
        ::Deployment::PackagedComponentImplementations & implementation = cpd.implementation;
        if (implementation.length()) {
          ::Deployment::PackagedComponentImplementation & first_impl = implementation[0];
          ::Deployment::ComponentImplementationDescription & cid = first_impl.referencedImplementation;
          ::Deployment::Properties & configProperties = cid.configProperty;
          for (CORBA::ULong i = 0, l = configProperties.length();
               i < l;
               ++i)
          {
            ::Deployment::Property & configProperty = configProperties[i];
            if (ACE_OS::strcmp(configProperty.name.in(), "Importance") == 0)
            {
              new_importance_any = configProperty.value;
            }
          }
        }
      }
      ::CORBA::TypeCode_var tcv = new_importance_any.type();
      ::CORBA::TCKind       tck = tcv->kind();
      if (tck == ::CORBA::tk_long) {
        new_importance_any >>= new_importance;
      }
    }

    uuid_check->importance(new_importance);
  }
  return ret_val;
}

bool ImportanceBinPacker::Impl::teardown_plan (ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan_in,
                                               ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> package_in,
                                               std::vector<ACE_Strong_Bound_Ptr< ::Deployment::DeploymentPlan, ACE_Null_Mutex> > & plan_out,
                                               std::vector<ACE_Strong_Bound_Ptr< ::Deployment::PackageConfiguration, ACE_Null_Mutex> > & package_out,
                                               std::vector<Action_Enum> & action_out,
                                               bool autoTeardown /*=false*/ )
{
  ACE_UNUSED_ARG (package_in);

  bool ret_val = true;
  // Search for a plan with the same UUID as the given plan
  Plan_Set::iterator uuid_check =
    std::find_if(deployed_plans.begin(),
                 deployed_plans.end(),
                 Plan_Finder (Plan_Matcher(),
                              std::string(plan_in->UUID.in())));

  if (uuid_check != deployed_plans.end())
  {
    // Found the plan with the same UUID as the given plan.
    // We now tear down the matching plan and avoid further use of the
    // given plan.
    Plan_Record planrec = *uuid_check;

    // Put the plan and package out, along with a TEARDOWN action
    ACE_Strong_Bound_Ptr < ::Deployment::DeploymentPlan, ACE_Null_Mutex> plan
      (new ::Deployment::DeploymentPlan);
    ACE_Strong_Bound_Ptr < ::Deployment::PackageConfiguration, ACE_Null_Mutex> package
      (new ::Deployment::PackageConfiguration);

    planrec.get_plan (*plan, *package);

    plan_out.push_back    (plan);
    package_out.push_back (package);
    action_out.push_back  ((autoTeardown)?Action_Types::AUTO_TEARDOWN:Action_Types::TEARDOWN);

    // Calling Plan_Record::assign_apps with an empty vector of node
    // assignments is the trick for removing this plan's apps from all
    // the bins
    std::vector<std::string> empty_node_assignments;
    planrec.assign_apps(empty_node_assignments, this->bins);

    // Remove the Plan_Record from our set of deployed plans
    deployed_plans.erase (uuid_check);
  }
  else
  {
    ret_val = false;
  }

  return ret_val;
}

ImportanceBinPacker::ImportanceBinPacker (char** bin_names,
                                          double** bin_capacities)
  : impl_(new Impl(bin_names, bin_capacities))
{}

ImportanceBinPacker::~ImportanceBinPacker ()
{
  delete this->impl_;
}

void ImportanceBinPacker::print_bins (std::ostream & out) const
{
  this->impl_->print_bins (out);
}

bool ImportanceBinPacker::convert_input (const Plan_Seq & in,
                                         Plan_Seq & out)
{
  return this->impl_->convert_input (in, out);
}

bool ImportanceBinPacker::domain_change (const Domain_Change_List & changes,
                                         char ** joined_nodes,
                                         double ** joined_node_capacities,
                                         Plan_Seq & out)
{
  return this->impl_->domain_change (changes,
                                     joined_nodes,
                                     joined_node_capacities,
                                     out);
}
