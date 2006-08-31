#include <sstream>
#include <fstream>

#include "FaultTolerance/FaultToleranceVisitor.h"

namespace CQML
{

  FaultToleranceVisitor::FaultToleranceVisitor (const std::string& op)
    : dep_plan_framework_visitor_ (op),
      output_path_ (op),
      plan_injector_map_ ()
  {
  }

  FaultToleranceVisitor::~FaultToleranceVisitor ()
  {
    for(std::map <std::string, Injector *>::iterator itr 
          = this->plan_injector_map_.begin();
          itr != this->plan_injector_map_.end();
          ++itr)
    {
      delete itr->second;
    }
  }

  void FaultToleranceVisitor::Visit_RootFolder(const RootFolder& rf)
    {
      std::set<FaultTolerance>
        ft_folders = rf.FaultTolerance_kind_children();
      
      if (ft_folders.empty ())
          return;

      for (std::set<FaultTolerance>::iterator itr = ft_folders.begin();
          itr != ft_folders.end();
          ++itr)
        {
          FaultTolerance ft = *itr;
          ft.Accept (*this);
        }
/*
      for (std::map <std::string, Injector *>::iterator itr 
            = this->plan_injector_map_.begin();
            itr != this->plan_injector_map_.end();
            ++itr)
        {
          std::string full_filename = output_path_ + "\\" + itr->first + ".nodes.txt";
          std::ofstream outfile (full_filename.c_str());
          const FTInjector * ft_injector = dynamic_cast <FTInjector *> (itr->second);
          const NodeAssigner *node_assgn = ft_injector->get_node_assigner ();
          const std::map <std::string, Node> &random_assgn = node_assgn->get_node_assignment ();
          for (std::map <std::string, Node>::const_iterator itr = random_assgn.begin ();
              itr != random_assgn.end();
              ++itr)
            {
              outfile << itr->first << " : " << std::string(itr->second.name ()) << endl;
            }
         }
*/

      for (std::map <std::string, Injector *>::iterator itr 
            = this->plan_injector_map_.begin();
            itr != this->plan_injector_map_.end();
            ++itr)
        {
          this->dep_plan_framework_visitor_.add_injector (itr->first, itr->second);
        }

        this->dep_plan_framework_visitor_.Visit_RootFolder (rf);
    }

  void FaultToleranceVisitor::Visit_FaultTolerance(const FaultTolerance & ft)
  {
    std::set<FTRequirements>
      ft_requirements = ft.FTRequirements_kind_children();
    for (std::set<FTRequirements>::iterator iter = ft_requirements.begin();
         iter != ft_requirements.end();
         ++iter)
      {
        FTRequirements ft_req = *iter;
        ft_req.Accept (*this);
      }
  }

  void FaultToleranceVisitor::Visit_FTRequirements(const FTRequirements & ft_req)
  {
    std::auto_ptr <FTRequirementsVisitor> ft_req_visitor (new FTRequirementsVisitor ());
    ft_req_visitor->Visit_FTRequirements (ft_req);
    
    DeploymentPlanRef plan_ref = ft_req.DeploymentPlanRef_child ();
    DeploymentPlan plan = plan_ref.ref();
    std::string dep_plan_name = std::string (plan.name());

    SRGContainerRef srgc_ref = ft_req.SRGContainerRef_child ();

    std::auto_ptr <ComponentAdder> comp_addr (new ComponentAdder (ft_req_visitor.get()));
    comp_addr->compute_new_components ();

    std::auto_ptr <ConnectionAdder> conn_addr (new ConnectionAdder (comp_addr.get()));

    std::auto_ptr <NodeAssigner> node_assgn;
 
    if (Udm::null != srgc_ref)
      {
        SRGContainer srg_con = srgc_ref.ref();
        std::auto_ptr <SRGVisitor> srg_visitor (new SRGVisitor());
        srg_visitor->Visit_SRGContainer (srg_con);
        srg_visitor->compute_metric ();
        node_assgn = std::auto_ptr <NodeAssigner> (new BranchNBoundNodeAssigner 
                                                    (srg_visitor.release(), 
                                                     ft_req_visitor.get(), 
                                                     comp_addr.get()));
      }
    else
      {
        std::auto_ptr<NodeCollector> node_collector (new NodeCollector ());
        node_collector->Visit_DeploymentPlan (plan);
        node_assgn = std::auto_ptr <NodeAssigner> 
          (new RandomNodeAssigner (node_collector.release (), comp_addr.get()));
      }
    //node_assgn->compute_assignment ();  
    std::auto_ptr <FTInjector> ft_injector (new FTInjector (comp_addr.release(), 
                                            conn_addr.release(), node_assgn.release(), 
                                            ft_req_visitor.release()));
    this->plan_injector_map_.insert (std::make_pair (dep_plan_name, ft_injector.release()));
  }

}


/*
  Composite *SRG::add_composite (Composite *comp)
    {
      this->composites_.push_back (comp);
      comp->set_parent (this);
      return comp;
    }

  SRG::~SRG() 
    {
      for (std::vector <Composite *>::iterator itr = this->composites_.begin();
            itr != this->composites_.end ();
            ++itr)
        {
          delete *itr;
        }
    }
  
  bool SRG::is_leaf(void) const
    {
      return false;
    }

  Composite *Composite::set_parent (Composite *parent)
    {
      this->parent_ = parent;
      return parent;
    }

  SRGNode::~SRGNode()
    {}

  bool SRGNode::is_leaf(void) const
    {
      return true;
    }

  Composite * SRGNode::add_composite (Composite *comp)
    {
      return 0;
    }
*/


