#ifndef FTADDERS_H
#define FTADDERS_H

#include <memory>
#include <map>
#include "CQML/CQML.h"

#include "FaultTolerance/common.h"
#include "FaultTolerance/FTVisitors.h"
#include "DeploymentPlanFramework/Injector.h"

namespace CQML
  {
    class ComponentAdder 
      {
        public:
          ComponentAdder (const FTRequirementsVisitor *);
          virtual ~ComponentAdder ();
          virtual void compute_new_components (void);
          virtual const std::map <std::string, Component> & get_all_assembly_components (void) const;
          virtual const std::map <std::string, Component> & get_all_monolith_components (void) const;
          virtual bool is_monolith_instance (const std::string &str) const;
          virtual bool is_assembly_instance (const std::string &str) const;
          virtual std::set <std::string> give_replica_set (const std::string &comp_name) const;
          virtual std::map <int, std::string> give_replica_map (const std::string &comp_name) const;
          virtual std::string replica_suffix () const;

        private:
          void populate_map (std::map <std::string, Component> &, const std::set <std::string> &);

          const FTRequirementsVisitor * ft_req_visitor_;
          std::map <std::string, Component> monolith_comp_map_;
          std::map <std::string, Component> assembly_comp_map_;
      };

    class ConnectionAdder 
      {
        public:
          ConnectionAdder (const ComponentAdder *);
          virtual ~ConnectionAdder ();
          virtual Injector::ConnectionMap get_new_connections (const std::string &, const Injector::ConnectionMap &);
        private:
          const ComponentAdder *comp_addr_;
      };


    class NodeAssigner 
    {
      public:
        virtual ~NodeAssigner () {};
        virtual void compute_assignment (const std::map <std::string, std::string> &known_mapping) {};
        virtual const std::map <std::string, Node> & get_node_assignment (void) const = 0;
    };

    class RandomNodeAssigner : public NodeAssigner
    {
      public:
        RandomNodeAssigner (const NodeCollector *, const ComponentAdder *);
        ~RandomNodeAssigner();
        virtual void compute_assignment (const std::map <std::string, std::string> &known_mapping);
        virtual const std::map <std::string, Node> & get_node_assignment (void) const;

        private:
          std::auto_ptr<const NodeCollector> node_collector_;
          const ComponentAdder * comp_addr_;
          std::map <std::string, Node> node_assignment_;
    };


    class BranchNBoundNodeAssigner : public NodeAssigner
    {
      public:
        BranchNBoundNodeAssigner (const SRGVisitor *srgv, 
                                  const FTRequirementsVisitor *ft_req, 
                                  const ComponentAdder *comp_add);

        ~BranchNBoundNodeAssigner();
        virtual void compute_assignment (const std::map <std::string, std::string> &known_mapping);
        virtual const std::map <std::string, Node> & get_node_assignment (void) const;

        private:
          void permutations (std::vector<int> &, int k, int size, int depth);
          void nPr (std::vector<int> &, int k, int size, int r);
          bool update_best_permutation (const vector <int> &array, int depth);
          double compute_vector_value (const std::vector <int> &, int depth);
          void compute_replica_assignement_of (const std::string &primary, 
                                                const std::string &node_name);

          std::vector <int> best_permutation_;
          double best_permutation_value_;
          std::auto_ptr <const SRGVisitor> srg_visitor_;
          const FTRequirementsVisitor * ft_req_visitor_;
          const ComponentAdder * comp_addr_;
          std::map <std::string, Node> node_assignment_;
    };

  }  // namespace CQML

#endif // FTADDERS_H 
