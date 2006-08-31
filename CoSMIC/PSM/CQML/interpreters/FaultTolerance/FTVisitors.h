#ifndef FTVISITORS_H
#define FTVISITORS_H

#include <set>
#include <map>
#include "CQML/CQML.h"

#include "common.h"

namespace CQML
  {

    class FTRequirementsVisitor : public Visitor
      {
        public:

          FTRequirementsVisitor ();
          virtual ~FTRequirementsVisitor();

          // Member functions used by FTRequirementVisitor interpreter
          virtual void Visit_FTRequirements(const FTRequirements &);
          virtual void Visit_FailOverUnit(const FailOverUnit &);
          virtual void Visit_FOU_Requirement (const FOU_Requirement &fou_req);

          std::set <std::string> get_all_monolith_components (void) const;
          std::set <std::string> get_all_assembly_components (void) const;
          FTReq get_requirements (const std::string &) const;

        private:
          int strcasecmp (const std::string &, const std::string &);
          void monolith_instance_visit (const Component &);
          void assembly_instance_visit (const Component &);
          void assembly_visit (const ComponentAssembly &);

          int current_req_replica_;
          std::map <std::string, FTReq> monolith_instance_req_map_;
          std::map <std::string, FTReq> assembly_instance_req_map_;
      };

    class SRGVisitor : public Visitor
      {
        public:
          typedef int Distance;

          SRGVisitor ();
          virtual ~SRGVisitor();
          virtual void Visit_SRGContainer (const SRGContainer &);
          virtual void Visit_NodeGroup (const NodeGroup &);
          virtual void Visit_NodeRef (const NodeRef &node_ref);
          virtual int node_count() const;
          virtual NodeRef get_noderef (int) const;
          virtual void compute_metric ();
          virtual Distance get_metric_value (int node1, int node2) const;
          virtual int get_node_number (const std::string node_name) const;
        private:
          void srg_visit (const SharedRiskGroup &);

        private:
          CQML::SRGContainer top_SRGcontainer_;
          std::vector <NodeRef> noderef_vec_;
          std::map <std::string, int> node_to_number_map_;
          std::vector <std::vector <Distance> > node_matrix_;
      };

    class NodeCollector : public Visitor
      {
        public:
          NodeCollector();
          virtual ~NodeCollector ();
          virtual void Visit_RootFolder (const RootFolder &);
          virtual void Visit_Targets(const Targets &);
          virtual void Visit_Domain(const Domain &);
          virtual void Visit_DeploymentPlan (const DeploymentPlan &);
          virtual const std::vector <Node> & get_all_nodes (void) const;
        private:
          std::vector <Node> node_vector_;
      };
  }

#endif // FTVISITORS_H

