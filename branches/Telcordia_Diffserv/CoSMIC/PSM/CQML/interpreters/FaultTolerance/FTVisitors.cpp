#include "FaultTolerance/FTVisitors.h"

namespace CQML
{

// **************************************
//           FT Requirements Visitor
// **************************************
  FTRequirementsVisitor::FTRequirementsVisitor ()
    : current_req_replica_ (0),
      monolith_instance_req_map_ (),
      assembly_instance_req_map_ ()
  {
  }

  FTRequirementsVisitor::~FTRequirementsVisitor ()
  {
  }

  void FTRequirementsVisitor::Visit_FTRequirements(const FTRequirements & ft_req)
  {
    std::set<FailOverUnit>
      fous = ft_req.FailOverUnit_kind_children();
    for (std::set<FailOverUnit>::iterator iter = fous.begin();
         iter != fous.end();
         ++iter)
      {
        FailOverUnit fou = *iter;
        ReqConnection fou_connect = fou.dstReqConnection ();
        FOU_Requirement fou_req = fou_connect.dstReqConnection_end ();
        
        // Find out how many replica are required and store in 
        // this->current_req_replica
        fou_req.Accept (*this);
        
        // Populate the ref2_instance_map with the component instance ids
        // for every FOU.
        fou.Accept (*this);
      }
  }

  void FTRequirementsVisitor::Visit_FailOverUnit(const FailOverUnit & fou)
  {
    std::set<FTMonitorable>
      ft_mons = fou.FTMonitorable_kind_children();
    for (std::set<FTMonitorable>::iterator iter = ft_mons.begin();
         iter != ft_mons.end();
         ++iter)
     { 
        FTMonitorable ft_mon = *iter;
       
        if (Udm::IsDerivedFrom (ft_mon.type(), ComponentRef::meta))
          {
            ComponentRef comp_ref = ComponentRef::Cast (ft_mon);
            Component component = comp_ref.ref ();
            this->monolith_instance_visit (component);  
          }
        else
          {
            ComponentAssemblyReference assm_ref = ComponentAssemblyReference::Cast (ft_mon);
            ComponentAssembly assembly = assm_ref.ref ();
            this->assembly_visit (assembly);  
          }
        /// else shared componentes?
     }
  }

  void FTRequirementsVisitor::assembly_visit (const ComponentAssembly &assembly)
    {
        std::set <ComponentAssembly> assemblies = 
          assembly.ComponentAssembly_kind_children ();
        for (std::set <ComponentAssembly>::iterator itr = assemblies.begin();
             itr != assemblies.end ();
             ++itr)
          {
            this->assembly_visit (*itr);
          }

        std::set <Component> components = assembly.Component_kind_children ();
        for (std::set <Component>::iterator itr = components.begin();
             itr != components.end ();
             ++itr)
          {
            this->assembly_instance_visit (*itr);
          }

        std::set <ComponentAssemblyReference> assembly_refs = 
          assembly.ComponentAssemblyReference_kind_children ();
        for (std::set <ComponentAssemblyReference>::iterator itr = assembly_refs.begin();
             itr != assembly_refs.end ();
             ++itr)
          {
            this->assembly_visit (itr->ref ());
          }

        std::set <ComponentRef> component_refs = assembly.ComponentRef_kind_children ();
        for (std::set <ComponentRef>::iterator itr = component_refs.begin();
             itr != component_refs.end ();
             ++itr)
          {
            this->assembly_instance_visit (itr->ref());
          }
    }

  void FTRequirementsVisitor::monolith_instance_visit (const Component &component)
    {
        const std::string comp_name = component.getPath (".",false,true,"name",true);
        
        // Put the component primary into the replica group set.
        this->monolith_instance_req_map_.insert (std::make_pair(comp_name, 
                                                 std::make_pair (this->current_req_replica_, 
                                                 component)));
    }

  void FTRequirementsVisitor::assembly_instance_visit (const Component &component)
    {
        const std::string comp_name = component.getPath (".",false,true,"name",true);
        
        // Put the component primary into the replica group set.
        this->assembly_instance_req_map_.insert (std::make_pair(comp_name, 
                                                 std::make_pair (this->current_req_replica_, 
                                                 component)));
    }

  void FTRequirementsVisitor::Visit_FOU_Requirement (const FOU_Requirement &fou_req)
    {
      std::set <Property> property_set = fou_req.Property_kind_children ();
      for (std::set <Property>::iterator itr = property_set.begin ();
           itr != property_set.end ();
           ++itr)
        {
          const Property &property = *itr;
          if (0 == this->strcasecmp (std::string ("Replica"), std::string (property.name ())))
            {
              const std::string &value = std::string(property.DataValue());
              this->current_req_replica_ = atoi (value.c_str());
              return;
            }
        }
    }

  int FTRequirementsVisitor::strcasecmp (const std::string &s, const std::string &s2)
    {
      std::string::const_iterator p = s.begin ();
      std::string::const_iterator p2 = s2.begin ();

      while (p != s.end() && p2 != s2.end())
        { 
          if (toupper (*p) != toupper (*p2)) 
            return (toupper (*p) < toupper (*p2)) ? -1 : 1;
          ++p;
          ++p2;
        }
      return (s2.size() == s.size ()) ? 0 : (s.size() < s2.size()) ? -1 : 1;
    }

  std::set <std::string> FTRequirementsVisitor::get_all_monolith_components (void) const
    {
      std::set <std::string> comp_set;
      for (std::map <std::string, FTReq>::const_iterator itr
           = this->monolith_instance_req_map_.begin ();
           itr != this->monolith_instance_req_map_.end();
           ++itr)
        {
          comp_set.insert (itr->first);
        }
      return comp_set;
    }

  std::set <std::string> FTRequirementsVisitor::get_all_assembly_components (void) const
    {
      std::set <std::string> comp_set;
      for (std::map <std::string, FTReq>::const_iterator itr
            = this->assembly_instance_req_map_.begin ();
            itr != this->assembly_instance_req_map_.end();
            ++itr)
        {
          comp_set.insert (itr->first);
        }
      return comp_set;
    }

  FTReq FTRequirementsVisitor::get_requirements (const std::string &comp_name) const
    {
      std::map <std::string, FTReq>::const_iterator assembly_itr = 
        this->assembly_instance_req_map_.find (comp_name);
      if (assembly_itr !=  this->assembly_instance_req_map_.end())
        {
          return assembly_itr->second;
        }

      std::map <std::string, FTReq>::const_iterator monolith_itr = 
        this->monolith_instance_req_map_.find (comp_name);
      if (monolith_itr !=  this->monolith_instance_req_map_.end())
        {
          return monolith_itr->second;
        }
      throw "ERROR: Invalid component name";
    }


  NodeCollector::NodeCollector ()
    {    }

  NodeCollector::~NodeCollector ()
    {    }

  const std::vector <Node> & NodeCollector::get_all_nodes (void) const
    {
      return this->node_vector_;
    }

  void NodeCollector::Visit_RootFolder (const RootFolder &rf)
    {
        std::set <Targets> targets = rf.Targets_kind_children ();

        if (targets.empty ())
            return;

        for (std::set <Targets>::iterator itr = targets.begin ();
            itr != targets.end();
            ++itr)
          {
            Targets t = *itr;
            t.Accept (*this);
          }
    }

  void NodeCollector::Visit_Targets(const Targets &t)
    {
      std::set <Domain> domains = t.Domain_kind_children ();
      for (std::set <Domain>::iterator itr = domains.begin ();
           itr != domains.end();
           ++itr)
        {
          Domain domain = *itr;
          domain.Accept (*this);
        }
    }

  void NodeCollector::Visit_Domain(const Domain &domain)
    {
      std::set <Node> nodes = domain.Node_kind_children ();
      //std::copy(nodes.begin (), nodes.end (), this->node_vector_.begin());
      for (std::set<Node>::iterator itr = nodes.begin();
           itr != nodes.end();
           ++itr)
        {
          this->node_vector_.push_back (*itr);
        }
    }

  void NodeCollector::Visit_DeploymentPlan (const DeploymentPlan &dep_plan)
    {
      std::set <NodeReference> nodes = dep_plan.NodeReference_kind_children ();
      for (std::set<NodeReference>::iterator itr = nodes.begin();
           itr != nodes.end();
           ++itr)
        {
          this->node_vector_.push_back (itr->ref());
        }
    }


  SRGVisitor::SRGVisitor () 
    {
    }

  SRGVisitor::~SRGVisitor()
    {
    }

  void SRGVisitor::Visit_SRGContainer (const SRGContainer &srg_con)
    {
      this->top_SRGcontainer_ = srg_con;
      std::set <SharedRiskGroup> srgs = srg_con.SharedRiskGroup_kind_children ();
      for (std::set <SharedRiskGroup>::iterator itr = srgs.begin();
            itr != srgs.end();
            ++itr)
        {
          this->srg_visit (*itr);
        }
    }

  void SRGVisitor::srg_visit (const SharedRiskGroup &srg)
    {
      std::set <SharedRiskGroup> srgs = srg.SharedRiskGroup_kind_children ();
      for (std::set <SharedRiskGroup>::iterator itr = srgs.begin();
            itr != srgs.end();
            ++itr)
        {
          this->srg_visit (*itr);
        }
      std::set <NodeGroup> node_groups = srg.NodeGroup_kind_children ();
      for (std::set <NodeGroup>::iterator itr = node_groups.begin();
            itr != node_groups.end();
            ++itr)
        {
          NodeGroup node_group = *itr;
          node_group.Accept (*this);
        }
      std::set <NodeRef> nodes = srg.NodeRef_kind_children ();
      for (std::set <NodeRef>::iterator itr = nodes.begin();
            itr != nodes.end();
            ++itr)
        {
          NodeRef node_ref = *itr;
          node_ref.Accept (*this);
        }
    }

  void SRGVisitor::Visit_NodeGroup (const NodeGroup &node_group)
    {
      std::set <NodeRef> nodes = node_group.NodeRef_kind_children();
      for (std::set <NodeRef>::iterator itr = nodes.begin ();
            itr != nodes.end();
            ++itr)
        {
          NodeRef node_ref = *itr;
          node_ref.Accept (*this);
        }
    }

  void SRGVisitor::Visit_NodeRef (const NodeRef &node_ref)
    {
      Node node = node_ref.ref();
      this->noderef_vec_.push_back (node_ref); 
      this->node_to_number_map_.insert 
        (std::make_pair (node.name(), this->noderef_vec_.size() - 1));
    }

  int SRGVisitor::node_count() const
    {
      return this->noderef_vec_.size();
    }

  int SRGVisitor::get_node_number (const std::string node_name) const
    {
      std::map <std::string, int>::const_iterator itr 
        = this->node_to_number_map_.find(node_name);
      return itr->second;
    }

  NodeRef SRGVisitor::get_noderef (int index) const
    {
      return this->noderef_vec_.at(index);
    }

  void SRGVisitor::compute_metric ()
    {
      int size = this->node_count ();

      this->node_matrix_.resize (size);
      for (int i = 0; i < size; ++i)
          this->node_matrix_[i].resize (size);

      //std::ofstream outfile ("C:\\Documents and Settings\\sumant\\Desktop\\DOC Files\\backup\\matrix_debug.txt");

      for (int i = 0; i < size; ++i)
        {
          for (int j = i; j < size; ++j)
            {
              if (i == j)
                this->node_matrix_[i][j] = 0;
              else
                {
                  NodeRef i_noderef = this->noderef_vec_[i];
                  NodeRef j_noderef = this->noderef_vec_[j];
                  Node i_node = i_noderef.ref();
                  Node j_node = j_noderef.ref();
                  //outfile << "(" << i << ", " << j << ") ";
                  //outfile << "(" << std::string (i_node.name()) << ", " << std::string (j_node.name()) << ")" << std::endl;

                  bool found = false;
                  int i_dist = 1;
                  for (MgaObject i_parent = MgaObject::Cast (i_noderef.parent ());
                       (this->top_SRGcontainer_ != i_parent) && !found;
                       (i_parent = MgaObject::Cast (i_parent.parent ()), ++i_dist))
                    {
                      int j_dist = 1;
                      for (MgaObject j_parent = MgaObject::Cast (j_noderef.parent());
                           (this->top_SRGcontainer_ != j_parent) && !found;
                           (j_parent = MgaObject::Cast (j_parent.parent ()), ++j_dist))
                        {
                          if (i_parent == j_parent)
                            {
                              this->node_matrix_[j][i] = this->node_matrix_[i][j] = 
                                i_dist + j_dist - 1;
                              //outfile << this->node_matrix_[i][j] << std::endl;
                              found = true;
                            }
                        }
                    }
                  //if (!found) throw "Incomaptible SRG hierarchy.";
                }
            }
        }
    }

  SRGVisitor::Distance SRGVisitor::get_metric_value (int node1, int node2) const
    {
      return this->node_matrix_[node1][node2];
    }


}
