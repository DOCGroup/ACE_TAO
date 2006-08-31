#ifndef FAULTTOLERANCE_VISITOR_H
#define FAULTTOLERANCE_VISITOR_H

#include <memory>
#include <map>
#include "CQML/CQML.h"

#include "FaultTolerance/FaultTolerance_Export.h"
#include "DeploymentPlanFramework/DeploymentPlanFrameworkVisitor.h"
#include "FaultTolerance/FTInjector.h"
#include "FaultTolerance/FTAdders.h"

namespace CQML
{
      class FaultTolerance_Export FaultToleranceVisitor: public Visitor
      {
        public:
          FaultToleranceVisitor (const std::string& outputPath);
          ~FaultToleranceVisitor();
          virtual void Visit_RootFolder(const RootFolder &);
          virtual void Visit_FaultTolerance(const FaultTolerance&);
          virtual void Visit_FTRequirements(const FTRequirements &);

        private:
          std::auto_ptr<Injector> injector_;
          DeploymentPlanFrameworkVisitor dep_plan_framework_visitor_;
          std::string output_path_;
          std::map <std::string, Injector *> plan_injector_map_;
      };


} // namespace CQML

/*
  class Composite 
    {
      public:
        virtual ~Composite() {};
        virtual bool is_leaf(void) const { return true; }
        virtual Composite *add_composite (Composite *) = 0;
        virtual Composite *set_parent (Composite *);
      private:
        const Composite *parent_; 
    };

  class SRG : public Composite
    {
      public:
        virtual ~SRG();
        virtual bool is_leaf(void) const ;
        virtual Composite *add_composite (Composite *);
      private:
        std::vector <Composite *> composites_;
    };

  class SRGNode : public Composite
    {
      public:
        virtual ~SRGNode();
        virtual bool is_leaf () const ;
        virtual Composite *add_composite (Composite *);
    };
*/



#endif /* FAULTTOLERANCE_VISITOR_H */
