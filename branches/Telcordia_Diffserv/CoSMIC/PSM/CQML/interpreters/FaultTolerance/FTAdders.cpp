#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>

#include "UmlExt.h"
#include "UdmUtil.h"

#include "FaultTolerance/FTAdders.h"

namespace CQML
{

  ComponentAdder::ComponentAdder (const FTRequirementsVisitor *ft_req)
    : ft_req_visitor_ (ft_req)
    {}

  ComponentAdder::~ComponentAdder ()
    {}

  void ComponentAdder::compute_new_components (void)
    {
      this->monolith_comp_map_.clear();
      std::set<std::string> comp_names = this->ft_req_visitor_->get_all_monolith_components ();
      this->populate_map (this->monolith_comp_map_, comp_names);

      this->assembly_comp_map_.clear();
      comp_names = this->ft_req_visitor_->get_all_assembly_components ();
      this->populate_map (this->assembly_comp_map_, comp_names);
    }

  std::set <std::string> ComponentAdder::give_replica_set (const std::string &comp_name) const
    {
      FTReq req = this->ft_req_visitor_->get_requirements (comp_name);
      std::set <std::string> replica_set;
      for (int i = 1; i <= req.first ; ++i)
        {
          std::ostringstream ostr;
          ostr << comp_name << this->replica_suffix() << i;
          replica_set.insert (ostr.str()); 
        }
      return replica_set;
    }

  std::map <int, std::string> ComponentAdder::give_replica_map (const std::string &comp_name) const
    {
      FTReq req = this->ft_req_visitor_->get_requirements (comp_name);
      std::map <int, std::string> replica_map;
      for (int i = 1; i <= req.first ; ++i)
        {
          std::ostringstream ostr;
          ostr << comp_name << this->replica_suffix() << i;
          replica_map.insert (std::make_pair (i, ostr.str())); 
        }
      return replica_map;
    }

  void ComponentAdder::populate_map (std::map <std::string, Component> & map, const std::set <std::string> & names)
    {
      for (std::set <std::string>::const_iterator itr = names.begin ();
           itr != names.end();
           ++itr)
        {
          FTReq req = this->ft_req_visitor_->get_requirements (*itr);
          map.insert (std::make_pair (*itr, req.second)); 

          std::set <std::string> replica_set = this->give_replica_set (*itr);

          for (std::set <std::string>::iterator itr = replica_set.begin ();
               itr != replica_set.end ();
               ++itr)
            {
              map.insert (std::make_pair (*itr, req.second)); 
            }
        }
     }

  std::string ComponentAdder::replica_suffix () const
    {
      return std::string ("_Replica");
    }

  const std::map <std::string, Component> &ComponentAdder::get_all_monolith_components (void) const
    {
      return this->monolith_comp_map_;
    }

  const std::map <std::string, Component> & ComponentAdder::get_all_assembly_components (void) const
    {
      return this->assembly_comp_map_;
    }

  bool ComponentAdder::is_monolith_instance (const std::string &str) const
    {
      return this->monolith_comp_map_.find (str) != this->monolith_comp_map_.end ();
    }

  bool ComponentAdder::is_assembly_instance (const std::string &str) const
    {
      return this->assembly_comp_map_.find (str) != this->assembly_comp_map_.end ();
    }

  ConnectionAdder::ConnectionAdder (const ComponentAdder *comp_addr)
    : comp_addr_ (comp_addr)
    {}

  ConnectionAdder::~ConnectionAdder ()
    {}

  Injector::ConnectionMap ConnectionAdder::get_new_connections (const std::string& plan_name, const Injector::ConnectionMap &conn_map)
    {
      Injector::ConnectionMap  new_map;
      for (Injector::ConnectionMap::const_iterator input_map_itr = conn_map.begin ();
            input_map_itr != conn_map.end ();
            ++input_map_itr)
        {
          const std::string &source_comp_name = input_map_itr->second.first.instance_name;
          const std::string &dest_comp_name = input_map_itr->second.second.instance_name;
          std::set <std::string> source_instances;

          // It is added in the source_instances because in the replicated
          // destination case, the two for loops in the following if block 
          // only looks at the source_instance set and nothing else
          source_instances.insert (source_comp_name);

          if (this->comp_addr_->is_assembly_instance (source_comp_name) ||
              this->comp_addr_->is_monolith_instance (source_comp_name))
            {
              // You are inside here that means the source is replicated.
              std::set <std::string> source_replica_set = this->comp_addr_->give_replica_set (source_comp_name);
              for (std::set <std::string>::iterator source_replica_itr = source_replica_set.begin ();
                   source_replica_itr != source_replica_set.end ();
                   ++source_replica_itr)
                {
                  Injector::ConnectionMap::mapped_type new_conn = input_map_itr->second;
                  std::ostringstream ostr;
                  ostr << *source_replica_itr;
                  new_conn.first.instance_name = ostr.str ();

                  // It is added in the source_instances because in the replicated
                  // destination case, the two for-loops in the following if-block 
                  // only looks at the source_instance set and nothing else
                  source_instances.insert (ostr.str()); 

                  ostr << "_" << dest_comp_name;
                  new_map.insert (std::make_pair (ostr.str (), new_conn));
                }
            }

          if (this->comp_addr_->is_assembly_instance (dest_comp_name) ||
              this->comp_addr_->is_monolith_instance (dest_comp_name))
            {
              // You are inside here that means the destination is replicated.
              std::set <std::string> dest_replica_set = this->comp_addr_->give_replica_set (dest_comp_name);
              for (std::set <std::string>::iterator dest_replica_itr = dest_replica_set.begin ();
                   dest_replica_itr != dest_replica_set.end ();
                   ++dest_replica_itr)
                {
                  // The for loop below only considers "source_instances" set.
                  // This loop does make distinction between one source instance 
                  // (i.e. primary) and more-than-one source instances 
                  // (i.e. primary + replicas) 
                  
                  for (std::set<std::string>::iterator source_instance_itr = source_instances.begin ();
                       source_instance_itr != source_instances.end();
                       ++source_instance_itr)
                    {
                      Injector::ConnectionMap::mapped_type new_conn = input_map_itr->second;
                      new_conn.first.instance_name = *source_instance_itr;
                      new_conn.first.port_kind = std::string ("IOGR-") + new_conn.first.port_kind;
                      
                      new_conn.second.instance_name = *dest_replica_itr;
                      new_conn.second.port_kind = std::string ("IOGR-") + new_conn.second.port_kind;
                      std::ostringstream ostr;
                      ostr << new_conn.first.instance_name << "_" << new_conn.second.instance_name;
                      new_map.insert (std::make_pair (ostr.str (), new_conn));
                    }
                }
            }
        }
      return new_map;
    }
  RandomNodeAssigner::RandomNodeAssigner (const NodeCollector *dv, const ComponentAdder *comp_add)
    : node_collector_ (dv),
      comp_addr_ (comp_add)
    {
    }
  
  RandomNodeAssigner::~RandomNodeAssigner()
    {}

  void RandomNodeAssigner::compute_assignment (const std::map <std::string, std::string> &known_mapping)
    {
      time_t t;
      t = time (0);
      srand (t);
      const std::vector <Node> &node_vec = this->node_collector_->get_all_nodes ();

      const std::map <std::string, Component> &comp_map = 
        this->comp_addr_->get_all_monolith_components ();
      for (std::map <std::string, Component>::const_iterator itr = 
            comp_map.begin ();
           itr != comp_map.end();
           ++itr)
        {
          int r = rand () % node_vec.size ();
          this->node_assignment_.insert (std::make_pair (itr->first, node_vec[r])); 
        }

      const std::map <std::string, Component> &assembly_comp_map = 
        this->comp_addr_->get_all_assembly_components ();
      for (std::map <std::string, Component>::const_iterator itr = 
            assembly_comp_map.begin ();
           itr != assembly_comp_map.end();
           ++itr)
        {
          int r = rand () % node_vec.size ();
          this->node_assignment_.insert (std::make_pair (itr->first, node_vec[r])); 
        }
    }

  const std::map <std::string, Node> & RandomNodeAssigner::get_node_assignment (void) const
    {
      return this->node_assignment_;
    }

  BranchNBoundNodeAssigner::BranchNBoundNodeAssigner (const SRGVisitor *srgv, 
                                                      const FTRequirementsVisitor *ft_req, 
                                                      const ComponentAdder *comp_add)
    : best_permutation_value_ (0),
      srg_visitor_ (srgv),
      ft_req_visitor_ (ft_req),
      comp_addr_ (comp_add)
    {
    }
  
  BranchNBoundNodeAssigner::~BranchNBoundNodeAssigner ()
    {}

  void BranchNBoundNodeAssigner::compute_replica_assignement_of 
    (const std::string &primary_name, const std::string &primary_node_name)
    {
      int node_count = this->srg_visitor_->node_count ();
      //std::ofstream outfile ("C:\\Documents and Settings\\sumant\\Desktop\\DOC Files\\backup\\BranchNBound_node_assignment.txt");
      this->best_permutation_.clear ();
      this->best_permutation_.resize (node_count);
      std::map <int, std::string> current_replica_group = 
        this->comp_addr_->give_replica_map (primary_name);
      current_replica_group.insert (std::make_pair (0, primary_name));
      int repg_size = current_replica_group.size ();
      assert (repg_size <= node_count);
      
      if (repg_size > 1) /// atleast one replica
        {
          std::vector <int> permutation_vector (node_count);

          /// Put numbers in the increasing order.
          for (int i = 1; i < node_count; ++i)
            {
              permutation_vector[i] = i;
              this->best_permutation_[i] = i;
            }

          this->best_permutation_value_ = 0; // to begin with

          if (0 == primary_node_name.size ())
            {
              permutation_vector[0] = 0;
              this->best_permutation_[0] = 0;
              
              /// No primary node is given therefore, no placement for primary can be
              /// made therefore, take the placement decision of the primary and replicas
              /// also.
              this->nPr (permutation_vector, 0, permutation_vector.size(), repg_size);
            }
          else
            {
              this->best_permutation_[0] = permutation_vector[0] = 
                this->srg_visitor_->get_node_number (primary_node_name);
              
              /// Primary node is given and its placement therefore, take the placement 
              /// decision of the replicas only.
              this->nPr (permutation_vector, 1, permutation_vector.size(), repg_size);
            }
        }
      else // only primary
        {
          if (0 == primary_node_name.size ())
              this->best_permutation_[0] = 0;
          else
              this->best_permutation_[0] = this->srg_visitor_->get_node_number (primary_node_name);
        }

      for (unsigned int i = 0;i < current_replica_group.size(); ++i)
        {
          int node_num = this->best_permutation_[i];
          CQML::NodeRef noderef = this->srg_visitor_->get_noderef (node_num);
          Node node = noderef.ref ();
          this->node_assignment_.insert 
            (std::make_pair (current_replica_group[i], node));
          //outfile << current_replica_group[i] << " : " 
          //  << std::string (node.name ()) << std::endl;
        }
    }

  void BranchNBoundNodeAssigner::compute_assignment (const std::map <std::string, std::string> &known_mapping)
    {
      std::set <std::string> assembly_comp_set = this->ft_req_visitor_->get_all_assembly_components ();
      std::set <std::string> primary_set = this->ft_req_visitor_->get_all_monolith_components ();
      std::copy (assembly_comp_set.begin (), assembly_comp_set.end (), 
                 std::inserter(primary_set, primary_set.begin()));
      assembly_comp_set.clear ();

      for (std::set <std::string>::iterator primary_itr = primary_set.begin ();
           primary_itr != primary_set.end ();
           ++primary_itr)
        {
          std::map <std::string, std::string>::const_iterator itr 
            = known_mapping.find(*primary_itr);

          if (itr != known_mapping.end ())
            this->compute_replica_assignement_of (*primary_itr, itr->second);
          else
            this->compute_replica_assignement_of (*primary_itr, "");
        }
    }

  void BranchNBoundNodeAssigner::nPr (std::vector<int> &array, int k, int size, int r)
    {
      this->permutations (array, k ,size, r-1);
    }

  void BranchNBoundNodeAssigner::permutations (std::vector<int> &array, int k, int size, int depth)
    {
        int i;
        if(k > depth)
        {
                this->update_best_permutation (array, depth);
                return;
        }

        vector <int> temp (size);
        temp = array;

        for(i = k; i < size; i++)
        {
                std::swap (array[k], array[i]);
                this->permutations(array, k+1, size, depth);
                array = temp;
        }
    }

  bool BranchNBoundNodeAssigner::update_best_permutation (const vector <int> &perm, int depth)
    {
      bool retval;
      double new_value = this->compute_vector_value (perm, depth);
      //std::copy (perm.begin (), perm.end (), 
      //  std::ostream_iterator <int> (this->permfile_, " "));
      //this->permfile_ << " value = " << new_value;

      if (new_value > this->best_permutation_value_)
        {
          this->best_permutation_ = perm;
          this->best_permutation_value_ = new_value;
          //this->permfile_ << " Updated ";
          retval = true;
        }
      //this->permfile_ << std::endl; 
      retval = false;
      return retval;
    }

  double BranchNBoundNodeAssigner::compute_vector_value (const std::vector <int> &perm, int depth)
    {
      double value = 0;
      int sum = 0;
      for (int i = 1;i <= depth; i++)
        {
          int distance = this->srg_visitor_->get_metric_value (perm[0], perm[i]);
          //this->permfile_ << " distance = " << distance;
          sum += distance;
          value += distance * distance;
        }
      value = ::sqrt (value);
      //this->permfile_ << " SQRT = " << value << " sum = " << sum;  
      double mean = sum / depth;
      double std_dev = 0;
      for (int i = 0;i < depth; ++i)
        {
          int distance = this->srg_visitor_->get_metric_value (perm[0], perm[i]);
          std_dev += ::pow (distance-mean, 2);
        }
      std_dev = std::sqrt (std_dev / (depth));
      //this->permfile_ << " STD_DEV = " << std_dev << "# "; 
      return value - std_dev;
    }

  const std::map <std::string, Node> & BranchNBoundNodeAssigner::get_node_assignment (void) const
    {
      return this->node_assignment_;
    }

} // namespace CQML
