// @@ Will: Modify this and the .h file.
#include <fstream>
#include <algorithm>
#include <functional>
#include "QoSMapping/QoSMappingVisitor.h"
#include "UmlExt.h"


namespace PICML
{
  QoSMappingVisitor::QoSMappingVisitor (const std::string& outputPath)
    : outFile_ ()
  {
    std::string file = outputPath + "QoSMappingDebug.log";
    outFile_.open (file.c_str ());

    outFile_ << "Begninng of debuyg...." << std::endl << std::flush;
    this->init();
  }

  QoSMappingVisitor::~QoSMappingVisitor ()
  {
  outFile_.close ();
  }

  void QoSMappingVisitor::init()
  {
  }

  void QoSMappingVisitor::Visit_RootFolder(const RootFolder& rf)
  {

    outFile_ << "Beginning visitation..." << std::endl << std::flush;

    this->qos_config_ = RTConfiguration::Create (rf,
              RootFolder::meta_RTConfiguration_children);
  this->qos_config_.name () = "RTConfiguration";

  visit_kind_children (std::set <PredefinedTypes> (rf.PredefinedTypes_kind_children ()));

    // Visit the component implementations....
    std::set<ComponentImplementations>
      impls = rf.ComponentImplementations_kind_children();
    for (std::set<ComponentImplementations>::iterator iter = impls.begin();
         iter != impls.end();
         ++iter)
      {
        ComponentImplementations folder = *iter;
        folder.Accept (*this);
      }
  }

  // QoS Model
  void QoSMappingVisitor::Visit_QoSDecoration(const QoSDecoration &qd)
  {
    outFile_ << "Entering QoSDecoration" << qd.name () << std::endl << std::flush;;

    QoSValue value = qd.srcQoSDecoration_end ();
  value.Accept (*this);
  }

  void QoSMappingVisitor::Visit_QoSOffered(const QoSOffered&)
  {
    outFile_ << "Entering QoSOffered...\n";
  }

  void QoSMappingVisitor::Visit_QoSContract(const QoSContract&)
  {
    outFile_ << "Entering QoSContract...\n";
  }

  void QoSMappingVisitor::Visit_QoSRequired(const QoSRequired&)
  {
    outFile_ << "Entering QoSRequired\n";
  }

  void QoSMappingVisitor::Visit_QoSModeling(const QoSModeling&)
  {
    outFile_ << "Entering RTModeling...\n";
  }


  void QoSMappingVisitor::Visit_QoSValue(const QoSValue &qv)
  {
    outFile_ << "Entering QoSValue:" << qv.name () << std::endl << std::flush;
    this->prepare_qos_model ();
    this->prepare_policy_set ();

    // Iterate through predefined QoS
    std::set<CPU>
      cpus = qv.CPU_kind_children();
    for (std::set<CPU>::iterator iter = cpus.begin();
         iter != cpus.end();
         ++iter)
      {
        CPU cpu = *iter;
        cpu.Accept (*this);
      }

    std::set<Concurrency>
      conc = qv.Concurrency_kind_children();
    for (std::set<Concurrency>::iterator iter = conc.begin();
         iter != conc.end();
         ++iter)
      {
        Concurrency conc = *iter;
        conc.Accept (*this);
      }

    std::set<Process>
      proc = qv.Process_kind_children();
    for (std::set<Process>::iterator iter = proc.begin();
         iter != proc.end();
         ++iter)
      {
        Process proc = *iter;
        proc.Accept (*this);
      }

    visit_kind_children(std::set<Connection> (qv.Connection_kind_children ()));

  }

  void QoSMappingVisitor::Visit_QoSDimensionQualifier(const QoSDimensionQualifier&)
  {
    outFile_ << "Entering QoSDimensionQualifier\n";
  }

  void QoSMappingVisitor::Visit_QoSDimensionSlot(const QoSDimensionSlot&)
  {
    outFile_ << "Entering QoSDimensionSlot\n";
  }

  void QoSMappingVisitor::Visit_QoSContext(const QoSContext&)
  {
    outFile_ << "Entering QoSContext\n";
  }

  void QoSMappingVisitor::Visit_QoSParameter(const QoSParameter&)
  {
    outFile_ << "Entering QoSParameter\n";
  }

  void QoSMappingVisitor::Visit_QoSCategory(const QoSCategory&)
  {
    outFile_ << "Entering QoSCategory\n";
  }

  void QoSMappingVisitor::Visit_QoSDimension(const QoSDimension&)
  {
    outFile_ << "Entering QoSDimension\n";
  }

  void QoSMappingVisitor::Visit_QoSCharacteristic(const QoSCharacteristic&)
  {
    outFile_ << "Entering QoSCharacteristic\n";
  }

  // QoS Decorations
  void QoSMappingVisitor::Visit_ComponentImplementations(const ComponentImplementations &cimpls)
  {
    outFile_ << "Entering ComponentImplementations:" << cimpls.name () << std::endl << std::flush;

    std::set<ComponentImplementationContainer>
      cics = cimpls.ComponentImplementationContainer_kind_children();
    for (std::set<ComponentImplementationContainer>::iterator
           iter = cics.begin();
         iter != cics.end();
         ++iter)
      {
        ComponentImplementationContainer cc = *iter;
        cc.Accept (*this);
      }
  }

  void QoSMappingVisitor::Visit_ComponentImplementationContainer(const ComponentImplementationContainer &cic)
  {
    outFile_ << "Entering ComponentImplementationContainer:" << cic.name () << std::endl << std::flush;

    // We are only interested in assemblies, for now.
    std::set<ComponentAssembly> asms = cic.ComponentAssembly_kind_children();
    for (std::set<ComponentAssembly>::iterator it = asms.begin();
         it != asms.end();
         ++it)
      {
        ComponentAssembly assembly = *it;
        assembly.Accept (*this);
      }
  }

  void QoSMappingVisitor::Visit_ComponentAssembly(const ComponentAssembly &ca)
  {
    outFile_ << "Entering ComponentAssembly:" << ca.name () << std::endl << std::flush;

    std::set<Component> comps = ca.Component_children ();
    for (std::set<Component>::iterator it = comps.begin ();
   it != comps.end ();
   ++it)
      {
  Component comp = *it;
  comp.Accept (*this);
      }
  }

  void QoSMappingVisitor::Visit_Component(const Component &co)
  {
    outFile_ << "Entering Component:" << co.name () << std::endl << std::flush;

    this ->create_new_context (co.name ());

    // Find all associated QoS Attributes.
    std::set<QoSDecoration> qds = co.srcQoSDecoration ();
    for (std::set<QoSDecoration>::iterator i = qds.begin ();
   i != qds.end ();
   ++i)
      {
  QoSDecoration qd = *i;
  qd.Accept (*this);
      }

    // Associate properties
    if (this->ctx_qos_model_created_)
      {
  // Create property
  Property qm = Property::Create (co.ComponentAssembly_parent (),
          ComponentAssembly::meta_Property_children);
  qm.name () = "CIAOServerResources";
  qm.DataValue () = this->ctx_qos_model_.name ();

  DataType dt = DataType::Create (qm);
  dt.ref () = this->dt_map_[STRING];

  // Connect to instance
  AssemblyConfigProperty acp = AssemblyConfigProperty::Create (co.ComponentAssembly_parent (),
                     ComponentAssembly::meta_AssemblyConfigProperty_children);
  acp.dstAssemblyConfigProperty_end () = qm;
  acp.srcAssemblyConfigProperty_end () = co;
      }
    if (this->ctx_policy_set_created_)
      {
  // Create property
  Property pol_set = Property::Create (co.ComponentAssembly_parent (),
               ComponentAssembly::meta_Property_children);
  pol_set.name () = "CIAOPolicySetId";
  pol_set.DataValue () = this->ctx_policy_set_.name ();

  DataType dt = DataType::Create (pol_set);
  dt.ref () = this->dt_map_[STRING];

  // Connect to instance
  AssemblyConfigProperty acp = AssemblyConfigProperty::Create (co.ComponentAssembly_parent (),
                     ComponentAssembly::meta_AssemblyConfigProperty_children);
  acp.dstAssemblyConfigProperty_end () = pol_set;
  acp.srcAssemblyConfigProperty_end () = co;
      }

  }

  // Predefined QoSCharacterizations
  void QoSMappingVisitor::Visit_Process(const Process &pro)
  {
    outFile_ << "Entering process:" << pro.name () << std::endl << std::flush;
    PriorityModelPolicy pmp =
      PriorityModelPolicy::Create (this->ctx_policy_set_,
           PolicySet::meta_PriorityModelPolicy_child);
  pmp.name () = this->ctx_comp_name_ + "PMP";

    pmp.Priority () = pro.ProcessPriority ();
    pmp.PriorityModel () = "SERVER_DECLARED";
  }

  void QoSMappingVisitor::Visit_Concurrency(const Concurrency &con)
  {
    outFile_ << "Entering Concurrency:" << con.name () << std::endl << std::flush;
    this->prepare_res_cont ();

    SimpleThreadPool stp = SimpleThreadPool::Create (this->ctx_res_cont_); // Simple child relationship
  stp.name () = this->ctx_comp_name_ + "STP";
    ThreadPoolLane tpl = ThreadPoolLane::Create (stp,
             SimpleThreadPool::meta_ThreadPoolLane_child);
  tpl.name () = this->ctx_comp_name_ + "TPL";

    tpl.static_threads () = static_cast <__int64> (con.MaxRequests ());

    // Attach to current policyset
    ThreadPoolRef tpr = ThreadPoolRef::Create (this->ctx_policy_set_,
                 PolicySet::meta_ThreadPoolRef_child);
  tpr.name () = this->ctx_comp_name_ + "TPR";
    tpr.ref () = stp;
  }

  void QoSMappingVisitor::Visit_CPU(const CPU &cpu)
  {
    outFile_ << "Entering CPU:" << cpu.name () << std::endl << std::flush;
  }

  void QoSMappingVisitor::Visit_Connection (const Connection& conn)
  {
    outFile_ << "Entering Connection:" << conn.name () << std::endl << std::flush;
    EnvironmentConf ec;
  std::string cmdline;
  if (static_cast <std::string> (conn.Transport ()) == "IPv6")
      {
    cmdline += "-ORBPreferIPV6Interfaces Yes";
      }

    std::set <QoSDimension> qds = conn.QoSDimension_children ();
    for (std::set <QoSDimension>::iterator i = qds.begin ();
   i != qds.end ();
   ++i)
      {
      if (static_cast <std::string> (i->name ()) == "Port")
    {
      std::set <QoSDimensionQualifier> qd_slots =
        i->srcQoSDimensionQualifier ();
      QoSDimensionQualifier qosdq = *qd_slots.begin ();
    QoSDimensionSlot qosds = qosdq.srcQoSDimensionQualifier_end ();

    cmdline += " -ORBLaneListenEndpoints iiop://localhost:";
      cmdline += qosds.value ();

    }
      }
      ec = EnvironmentConf::Create (this->ctx_qos_model_,
              RTModel::meta_EnvironmentConf_child);
  ec.CmdLineOptions () = cmdline;
  ec.name () = this->ctx_comp_name_ + "CLO";
  }

  // Datatype visitors

  void QoSMappingVisitor::Visit_PredefinedTypes(const PredefinedTypes &pdt)
  {
  if (visit_kind_children (std::set <String> (pdt.String_kind_children ())) == 0)
      this->dt_map_[STRING] = String::Create (pdt); // Simple child relationship

    // Add any additional datatypes needed....
  }

  void QoSMappingVisitor::Visit_String(const String &str)
  {
    this->dt_map_[STRING] = str;
  }

  // Private helper methods
  void QoSMappingVisitor::create_new_context (const std::string &name)
  {
    this->ctx_comp_name_ = name;
    this->ctx_qos_model_created_ = false;
    this->ctx_res_cont_created_ = false;
    this->ctx_policy_set_created_ = false;
  }

  void QoSMappingVisitor::prepare_qos_model (void)
  {
    if (this->ctx_qos_model_created_)
      return;

    this->ctx_qos_model_ = RTModel::Create (this->qos_config_,
               RTConfiguration::meta_RTModel_children);
    this->ctx_qos_model_.name () = this->ctx_comp_name_ + "QM";
    this->ctx_qos_model_created_ = true;
  }

  void QoSMappingVisitor::prepare_policy_set (void)
  {
    if (this->ctx_policy_set_created_)
      return;

    this->prepare_qos_model ();

    this->ctx_policy_set_ = PolicySet::Create (this->ctx_qos_model_,
                 RTModel::meta_PolicySet_children);
    this->ctx_policy_set_.name () = this->ctx_comp_name_ + "PS";
    this->ctx_policy_set_created_ = true;
  }

  void QoSMappingVisitor::prepare_res_cont (void)
  {
    if (this->ctx_res_cont_created_)
      return;

    this->prepare_qos_model ();

    this->ctx_res_cont_ = ResourceContainer::Create (this->ctx_qos_model_,
              RTModel::meta_ResourceContainer_child);
    this->ctx_res_cont_.name () = this->ctx_comp_name_ + "RC";
    this->ctx_res_cont_created_ = true;
  }
}

#include "QoSMappingVisitor.tpp"
