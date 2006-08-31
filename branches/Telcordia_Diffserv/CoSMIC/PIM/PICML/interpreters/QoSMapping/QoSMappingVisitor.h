// $Id$

#ifndef PACKAGE_VISITOR_H
#define PACKAGE_VISITOR_H

#include <memory>
#include <stack>
#include <map>
#include "PICML/PICML.h"

#include <fstream>

// Utility includes
#include "PICML/Utils.h"
#include "QoSMapping/QoSMapping_Export.h"

namespace PICML
{
  class QoSMapping_Export QoSMappingVisitor : public Visitor
  {
  public:
    QoSMappingVisitor (const std::string& outputPath);
    ~QoSMappingVisitor();

    // Lord Of the Rings..
    virtual void Visit_RootFolder(const RootFolder&);
    
    // QoS Model
    virtual void Visit_QoSDecoration(const QoSDecoration&);
    virtual void Visit_QoSOffered(const QoSOffered&);
    virtual void Visit_QoSContract(const QoSContract&);
    virtual void Visit_QoSRequired(const QoSRequired&);
    virtual void Visit_QoSModeling(const QoSModeling&);
    virtual void Visit_QoSValue(const QoSValue&);
    virtual void Visit_QoSDimensionQualifier(const QoSDimensionQualifier&);
    virtual void Visit_QoSDimensionSlot(const QoSDimensionSlot&);
    virtual void Visit_QoSContext(const QoSContext&);
    virtual void Visit_QoSParameter(const QoSParameter&);
    virtual void Visit_QoSCategory(const QoSCategory&);
    virtual void Visit_QoSDimension(const QoSDimension&);
    virtual void Visit_QoSCharacteristic(const QoSCharacteristic&);

    // QoS Decorations
    virtual void Visit_ComponentImplementations(const ComponentImplementations&);
    virtual void Visit_ComponentImplementationContainer(const ComponentImplementationContainer&);
    virtual void Visit_ComponentAssembly(const ComponentAssembly&);
    virtual void Visit_Component(const Component&);
    

    // Predefined QoSCharacterizations
    virtual void Visit_Process(const Process&);
    virtual void Visit_Concurrency(const Concurrency&);
    virtual void Visit_CPU(const CPU&);
	virtual void Visit_Connection (const Connection& conn);
    
    virtual void Visit_PredefinedTypes(const PredefinedTypes&);
    virtual void Visit_String(const String&);
  private:
    /// Initialize required data members
    void init (void);

    void create_new_context (const std::string &name);
    void prepare_qos_model (void);
    void prepare_policy_set (void);
    void prepare_res_cont (void);

    /// Generic function to call accept on kind_children.
    /// @returns the number of elements processed. 
    template <typename T>
    size_t visit_kind_children (std::set <T> &kind_set);
    
    RTModel qos_model_;
    
    std::ofstream outFile_;

    RTConfiguration qos_config_;

    // Context
    std::string ctx_comp_name_;
    
    bool ctx_qos_model_created_;
    RTModel ctx_qos_model_;

    bool ctx_res_cont_created_;
    ResourceContainer ctx_res_cont_;

    bool ctx_policy_set_created_;
    PolicySet ctx_policy_set_;
    
    enum DataTypes {
      STRING
    };

    std::map <DataTypes, PredefinedType>dt_map_;
  };
}

#endif /* PACKAGE_VISITOR_H */
