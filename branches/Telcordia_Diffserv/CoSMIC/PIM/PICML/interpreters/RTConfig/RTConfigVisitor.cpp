#include <algorithm>
#include <functional>
#include <stdlib.h>
#include "RTConfig/RTConfigVisitor.h"
#include "UmlExt.h"
#include "PICML/Utils.h"

using xercesc::LocalFileFormatTarget;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMException;
using xercesc::XMLUni;
using xercesc::XMLException;
using xercesc::DOMText;


//diable this annoyng warning:
//  conversion from '__int64' to 'long', possible loss of data
#pragma warning (disable: 4244) //disable warning 4244


namespace PICML
{
  RTConfigVisitor::RTConfigVisitor (const std::string& outputPath)
    : impl_ (0),
      doc_ (0),
      root_ (0),
      curr_ (0),
      resources_ (0),
      serializer_ (0),
      target_ (0),
      outputPath_ (outputPath)
  {
    this->init();
  }

  RTConfigVisitor::~RTConfigVisitor ()
  {
    if (this->doc_)
      this->doc_->release();
    delete this->target_;
  }

  void RTConfigVisitor::init()
  {
    this->impl_ = DOMImplementationRegistry::getDOMImplementation(XStr("LS"));
    this->serializer_ = ((DOMImplementationLS*)impl_)->createDOMWriter();

    // Set features if the serializer supports the feature/mode
    if (this->serializer_->canSetFeature
        (XMLUni::fgDOMWRTDiscardDefaultContent, true))
      this->serializer_->setFeature (XMLUni::fgDOMWRTDiscardDefaultContent,
                                     true);

    if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTFormatPrettyPrint,
                                          true))
      this->serializer_->setFeature (XMLUni::fgDOMWRTFormatPrettyPrint, true);

    if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTBOM, false))
      this->serializer_->setFeature (XMLUni::fgDOMWRTBOM, false);
  }

  void RTConfigVisitor::initTarget (const std::string& fileName)
  {
    if (this->target_)
      delete this->target_;
    this->target_ = new LocalFileFormatTarget (fileName.c_str());
  }

  void RTConfigVisitor::initDocument (const std::string& rootName)
  {
    if (this->doc_)
      this->doc_->release();
    // Create the document
    this->doc_ =
      this->impl_->createDocument (XStr ("http://www.dre.vanderbilt.edu/ServerResources"),
                                   XStr (rootName.c_str()),
                                   0);

  //erase all elements currently present in the element maps
  this->simpletp_.clear ();
  this->tpWithLanes_.clear ();
  this->connections_.clear ();
  }

  void RTConfigVisitor::initRootAttributes()
  {
    this->doc_->setEncoding (XStr("UTF-8"));
    this->doc_->setVersion (XStr("1.0"));
    this->root_ = this->doc_->getDocumentElement();
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:CIAO"),
                                 XStr ("http://www.dre.vanderbilt.edu/ServerResources"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xsi"),
                                 XStr ("http://www.w3.org/2001/XMLSchema-instance"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xmi"),
                                 XStr ("http://www.omg.org/XMI"));
    this->root_->setAttribute (XStr ("xsi:schemaLocation"),
                               XStr ("http://www.dre.vanderbilt.edu/ServerResources CIAOServerResources.xsd"));
  std::string id = "_";
  id += PICML::CreateUuid ();
  this->root_->setAttribute (XStr ("id"),
                 XStr (id.c_str ()));
  this->curr_ = this->root_;
  this->resources_ = 0;
  }

  void RTConfigVisitor::dumpDocument()
  {
    this->serializer_->writeNode (this->target_, *this->doc_);
  }

  void RTConfigVisitor::push()
  {
    this->curr_stack_.push (this->curr_);
  }

  void RTConfigVisitor::pop()
  {
    if (!this->curr_stack_.empty())
      {
        this->curr_ = this->curr_stack_.top();
        this->curr_stack_.pop();
      }
    else
      {
        throw(std::exception());
      }
  }

  DOMElement* RTConfigVisitor::createSimpleContent (const std::string& name,
                                                    const std::string& value)
  {
    DOMElement* pName = this->doc_->createElement (XStr (name.c_str()));
    DOMText* pValue = this->doc_->createTextNode (XStr (value.c_str()));
    pName->appendChild (pValue);
    return pName;
  }


  std::string RTConfigVisitor::ExtractName(Udm::Object ob)
  {
    Uml::Class cls= ob.type();
    set<Uml::Attribute> attrs=cls.attributes();

    // Adding parent attributes
    set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
    attrs.insert(aattrs.begin(),aattrs.end());

    for (set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++)
      {
        if (string(ai->type())=="String")
          {
            string str=ai->name();
            if (str=="name")
              {
                string value=ob.getStringAttr(*ai);
                if (value.empty())value="<empty string>";
                return value;
              }
          }
      }
    return string("<no name specified>");
  }

//==================================================================
//Entry point for the visitor

  void RTConfigVisitor::Visit_RootFolder(const RootFolder& rf)
  {
    std::set<RTConfiguration>
      qc = rf.RTConfiguration_kind_children ();
    for (std::set<RTConfiguration>::iterator iter = qc.begin ();
         iter != qc.end ();
         ++iter)
      {
        RTConfiguration folder = *iter;
        folder.Accept (*this);
      }
  }


//==================================================================
//RTConfig-related elements


  void RTConfigVisitor::Visit_RTConfiguration (const RTConfiguration& qc)
  {
    if (qc == Udm::null)
      return;

    std::set<RTModel>
      qm = qc.RTModel_kind_children ();
      for (std::set<RTModel>::iterator iter = qm.begin ();
           iter != qm.end ();
           ++iter)
      {
         RTModel pcc = *iter;
         pcc.Accept (*this);
      }
  }


  void RTConfigVisitor::Visit_RTModel(const RTModel& qm)
  {
    if (qm == Udm::null)
      return;

    std::string name = this->outputPath_ + "\\";
    name += qm.name ();
    name += ".csr";
    this->initTarget (name);
    this->initDocument ("CIAO:ServerResources");
    this->initRootAttributes ();

    this->push();

    //add the optional environment configuration parameters
    EnvironmentConf ec = qm.EnvironmentConf_child (); //*iter;
    if (ec != Udm::null)
       ec.Accept (*this);

    //now add the ORB configuration parameters

    //Create the <orbConfigs> as a child of the <ServerResources>
    DOMElement* oc = this->doc_->createElement (XStr ("orbConfigs"));
    this->curr_->appendChild (oc);
    this->curr_ = oc;

    //add the orbResources
    this->push ();

    //Create the <orbResources> as a child of the <orbConfigs>
    DOMElement* or = this->doc_->createElement (XStr ("resources"));
    this->curr_->appendChild (or);
    this->curr_ = or;
    this->resources_ = this->curr_;

    //visit the resources
    ResourceContainer rc = qm.ResourceContainer_child ();
    if (rc != Udm::null)
       rc.Accept (*this);

    //restore the current element to the ORBConfigs level
    this->pop ();

    std::set<PolicySet> ps = qm.PolicySet_children ();
    for (std::set<PolicySet>::const_iterator it = ps.begin ();
         it != ps.end ();
         it++)
    {
       PolicySet p = *it;
       p.Accept (*this);
    }

    std::set<PolicySetRef> psr = qm.PolicySetRef_children ();
    for (std::set<PolicySetRef>::const_iterator it = psr.begin ();
         it != psr.end ();
         it++)
    {
       PolicySetRef pr = *it;
       pr.Accept (*this);
    }

    // Dump out an ComponentPackageDescription file
    this->dumpDocument();
    this->pop();
  }



  void RTConfigVisitor::Visit_ResourceContainer (const ResourceContainer& rc)
  {
    if (rc == Udm::null)
      return;

    std::set<resource> rs = rc.resource_children ();
    for (std::set<resource>::const_iterator it = rs.begin ();
         it != rs.end ();
         ++it)
    {
       resource r = *it;

       if (r.type () == SimpleThreadPool::meta)
        {
           SimpleThreadPool stp = SimpleThreadPool::Cast (r);
           stp.Accept (*this);
        }
       else if (r.type () == ThreadPoolWithLanes::meta)
        {
           ThreadPoolWithLanes tpl = ThreadPoolWithLanes::Cast (r);
           tpl.Accept (*this);
        }
       else if (r.type () == BandedConnection::meta)
        {
           BandedConnection bc = BandedConnection::Cast (r);
           bc.Accept (*this);
        }
    }

  }

  void RTConfigVisitor::Visit_resource (const resource& r)
  {
    //this is an abstract class in the UML diagram
    //the documentation is incorrect
    //This cannot be used
    //the flow of control just skips this function
  }


  void RTConfigVisitor::Visit_EnvironmentConf (const EnvironmentConf& ec)
  {
    if (ec == Udm::null)
      return;

    //save current level
    this->push ();

    //Add the <cmdline> options
    DOMElement* cmd = this->doc_->createElement (XStr ("cmdline"));
    this->curr_->appendChild (cmd);
    this->curr_ = cmd;

    std::string cl = ec.CmdLineOptions ();

    //Add the <arg>
    if (!cl.empty ())
      this->curr_->appendChild (this->createSimpleContent ("arg",
                                                           cl.c_str ()));

    //return to entry level
    this->pop ();

    //save current level again
    this->push ();

    //Add the <svcconf> options
    DOMElement* svc = this->doc_->createElement (XStr ("svcconf"));
    this->curr_->appendChild (svc);
    this->curr_ = svc;

    std::string sc = ec.SvcConf ();
    //Add the <uri>
    if (!sc.empty ())
       this->curr_->appendChild (this->createSimpleContent ("uri",
                                                            sc.c_str ()));

    //return to entry level
    this->pop ();
  }


  void RTConfigVisitor::Visit_BandedConnection (const BandedConnection& bc)
  {
    if (bc == Udm::null)
      return;

    this->push ();

    //Create the <connectionBands> element
    DOMElement* e = this->doc_->createElement (XStr ("connectionBands"));
    std::string bcID (bc.name ());
    e->setAttribute (XStr ("id"), XStr (bcID));
    this->curr_->appendChild (e);
    this->curr_ = e;

    std::set<ConnectionBand> bands = bc.ConnectionBand_children ();
    for (std::set<ConnectionBand>::const_iterator it = bands.begin ();
         it != bands.end ();
         it++)
    {
       ConnectionBand band = *it;
       band.Accept (*this);
    }

    //add the banded connection to the list of elements that have been added
    this->connections_.insert (make_pair (bc, true));

    this->pop ();
  }

  void RTConfigVisitor::Visit_BandedConnectionRef (const BandedConnectionRef& bcr)
  {
    if (bcr == Udm::null)
      return;

    BandedConnection bc = bcr.ref ();

    if (bc == Udm::null)
      return;

    //save curr
    this->push ();

    //use the resource DOMElement as an entry point
    this->curr_ = this->resources_;

    map<PICML::BandedConnection, bool>::const_iterator i;
    i = this->connections_.find (bc);
    if (i == this->connections_.end ())
       bc.Accept (*this);

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_ConnectionBand (const ConnectionBand& cb)
  {
    if (cb == Udm::null)
      return;

    //save curr
    this->push ();

    //Add the <band> element
    DOMElement* band = this->doc_->createElement (XStr ("band"));
    this->curr_->appendChild (band);
    this->curr_ = band;

    //temp char array to hold the output of itoa
    char temp [256];

    //output low
    long low = cb.low ();
    itoa (low, temp, 10);
    //Add the <low>
    this->curr_->appendChild (this->createSimpleContent ("low", temp));

    //output high
    long high = cb.high ();
    itoa (high, temp, 10);
    //Add the <high>
    this->curr_->appendChild (this->createSimpleContent ("high", temp));

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_ThreadPool (const ThreadPool& tp)
  {
    //This function is not used because
    //this is an abstract base class.
    //The documentation is incorrect.
    //The control flow skips this function.
  }

  void RTConfigVisitor::Visit_ThreadPoolRef(const ThreadPoolRef& tpr)
  {
    if (tpr == Udm::null)
      return;

    ThreadPool tp = tpr.ref ();

    if (tp == Udm::null)
      return;

    //save curr
    this->push ();

    //use the resource DOMElement as an entry point
    this->curr_ = this->resources_;

    if (tp.type () == SimpleThreadPool::meta)
    {
       SimpleThreadPool stp = SimpleThreadPool::Cast (tp);
       map<PICML::SimpleThreadPool, bool>::const_iterator i;
       i = this->simpletp_.find (stp);
       if (i == this->simpletp_.end ())
         stp.Accept (*this);
    }
    else if (tp.type () == ThreadPoolWithLanes::meta)
    {
       ThreadPoolWithLanes tpwl = ThreadPoolWithLanes::Cast (tp);
       map<PICML::ThreadPoolWithLanes, bool>::const_iterator i;
       i = this->tpWithLanes_.find (tpwl);
       if (i == this->tpWithLanes_.end ())
         tpwl.Accept (*this);
    }

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_SimpleThreadPool (const SimpleThreadPool& stp)
  {
    if (stp == Udm::null)
      return;

    //save curr
    this->push ();

    //Create the <threadpool> element
    DOMElement* e = this->doc_->createElement (XStr ("threadpool"));
    std::string tpID (stp.name ());
    e->setAttribute (XStr ("id"), XStr (tpID));
    this->curr_->appendChild (e);
    this->curr_ = e;

    //temp char array to hold the output of itoa
    char temp [256];

    //output stack size
    long ss = stp.stacksize ();
    itoa (ss, temp, 10);
    //Add the <static_threads>
    this->curr_->appendChild (this->createSimpleContent ("stacksize", temp));

    //output # of static & dynamic threads and the default priority
    ThreadPoolLane lane = stp.ThreadPoolLane_child ();

    if (lane != Udm::null)
    {
      //output static threads
      long st = lane.static_threads ();
      itoa (st, temp, 10);
      //Add the <static_threads>
      this->curr_->appendChild (this->createSimpleContent ("static_threads", temp));

      //output dynamic threads
      long dt = lane.dynamic_threads ();
      itoa (dt, temp, 10);
      //Add the <dynamic_threads>
      this->curr_->appendChild (this->createSimpleContent ("dynamic_threads", temp));

      //output default priority
      long prio = lane.lane_priority ();
      itoa (prio, temp, 10);
      //Add the <default_priority>
      this->curr_->appendChild (this->createSimpleContent ("default_priority", temp));
    }

    //output allow request buffering
    bool arb = stp.allow_request_buffering ();
    //Add the <allow_borrowing>
    if (arb)
      this->curr_->appendChild
        (this->createSimpleContent ("allow_request_buffering", "true"));
    else
      this->curr_->appendChild
        (this->createSimpleContent ("allow_request_buffering", "false"));

    //output max buffered requests
    long mbr = stp.max_buffered_requests ();
    itoa (mbr, temp, 10);
    //Add the <max_buffered_requests>
    this->curr_->appendChild
      (this->createSimpleContent ("max_buffered_requests", temp));

    //output max request buffer size
    long mbrs = stp.max_buffer_size ();
    itoa (mbrs, temp, 10);
    //Add the <max_request_buffered_size>
    this->curr_->appendChild
      (this->createSimpleContent ("max_request_buffered_size", temp));

    //add the banded connection to the list of elements that have been added
    this->simpletp_.insert (make_pair (stp, true));

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_ThreadPoolWithLanes (const ThreadPoolWithLanes& tpwl)
  {
    if (tpwl == Udm::null)
      return;

    this->push ();

    //Create the <threadpoolWithLanes> element
    DOMElement* e = this->doc_->createElement (XStr ("threadpoolWithLanes"));
    std::string tpID (tpwl.name ());
    e->setAttribute (XStr ("id"), XStr (tpID));
    this->curr_->appendChild (e);
    this->curr_ = e;

    std::set<ThreadPoolLane> lanes = tpwl.ThreadPoolLane_children ();
    for (std::set<ThreadPoolLane>::const_iterator it = lanes.begin ();
         it != lanes.end ();
         it++)
    {
       ThreadPoolLane lane = *it;
       lane.Accept (*this);
    }

    //temp char array to hold the output of itoa
    char temp [256];

    //output stack size
    long ss = tpwl.stacksize ();
    itoa (ss, temp, 10);
    //Add the <static_threads>
    this->curr_->appendChild (this->createSimpleContent ("stacksize", temp));

    //output allow borrowing
    bool ab = tpwl.allow_borrowing ();
    //Add the <allow_borrowing>
    if (ab)
      this->curr_->appendChild
        (this->createSimpleContent ("allow_borrowing", "true"));
    else
      this->curr_->appendChild
        (this->createSimpleContent ("allow_borrowing", "false"));


    //output allow request buffering
    bool arb = tpwl.allow_request_buffering ();
    //Add the <allow_borrowing>
    if (arb)
      this->curr_->appendChild
        (this->createSimpleContent ("allow_request_buffering", "true"));
    else
      this->curr_->appendChild
        (this->createSimpleContent ("allow_request_buffering", "false"));

    //output max buffered requests
    long mbr = tpwl.max_buffered_requests ();
    itoa (mbr, temp, 10);
    //Add the <max_buffered_requests>
    this->curr_->appendChild
      (this->createSimpleContent ("max_buffered_requests", temp));

    //output max request buffer size
    long mbrs = tpwl.max_buffer_size ();
    itoa (mbrs, temp, 10);
    //Add the <max_request_buffered_size>
    this->curr_->appendChild
      (this->createSimpleContent ("max_request_buffered_size", temp));


    //add the banded connection to the list of elements that have been added
    this->tpWithLanes_.insert (make_pair (tpwl, true));

    this->pop ();
  }

  void RTConfigVisitor::Visit_ThreadPoolLane (const ThreadPoolLane& tpl)
  {
    if (tpl == Udm::null)
      return;

    //save curr
    this->push ();

    //Add the <threadpoolLane> element
    DOMElement* lane = this->doc_->createElement (XStr ("threadpoolLane"));
    this->curr_->appendChild (lane);
    this->curr_ = lane;

    //temp char array to hold the output of itoa
    char temp [256];

    //output static threads
    long st = tpl.static_threads ();
    itoa (st, temp, 10);
    //Add the <static_threads>
    this->curr_->appendChild (this->createSimpleContent ("static_threads", temp));

    //output dynamic threads
    long dt = tpl.dynamic_threads ();
    itoa (dt, temp, 10);
    //Add the <dynamic_threads>
    this->curr_->appendChild (this->createSimpleContent ("dynamic_threads", temp));

    //output lane priority
    long prio = tpl.lane_priority ();
    itoa (prio, temp, 10);
    //Add the <priority>
    this->curr_->appendChild (this->createSimpleContent ("priority", temp));

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_PriorityModelPolicy (const PriorityModelPolicy& pmp)
  {
    if (pmp == Udm::null)
      return;

    //save curr
    this->push ();

    //Create the <priorityModel> element
    DOMElement* e = this->doc_->createElement (XStr ("priorityModel"));
    std::string server_prio (pmp.Priority ());
    if (!server_prio.empty ())
      e->setAttribute (XStr ("server_priority"), XStr (server_prio));

    this->curr_->appendChild (e);
    this->curr_ = e;

    //Add the <priority_model>
    std::string prio_type = pmp.PriorityModel ();
    //we have a default here
    this->curr_->appendChild (
    this->createSimpleContent ("priority_model", prio_type));

    //restore curr
    this->pop ();
  }

  void RTConfigVisitor::Visit_PolicySet (const PolicySet& ps)
  {
    if (ps == Udm::null)
      return;

    //save curr
    this->push ();

    //Create the <threadpoolWithLanes> element
    DOMElement* e = this->doc_->createElement (XStr ("policySet"));
    std::string psID (ps.name ());
    e->setAttribute (XStr ("id"), XStr (psID));
    this->curr_->appendChild (e);
    this->curr_ = e;

    //add the priority model
    PriorityModelPolicy pm = ps.PriorityModelPolicy_child ();
    if (pm != Udm::null)
      pm.Accept (*this);

    //resolve the threadpool ref
    ThreadPoolRef tpr = ps.ThreadPoolRef_child ();
    if (tpr != Udm::null)
    {
      ThreadPool tp = tpr.ref ();
      if (tp != Udm::null)
      {
        if (tp.type () == SimpleThreadPool::meta)
        {
          SimpleThreadPool stp = SimpleThreadPool::Cast (tp);
          map<PICML::SimpleThreadPool, bool>::const_iterator i;
          i = this->simpletp_.find (stp);
          if (i == this->simpletp_.end ())
          {
            this->push ();
            this->curr_ = this->resources_;
            stp.Accept (*this);
            this->pop ();
          }

          std::string name = stp.name ();
          this->curr_->appendChild (
          this->createSimpleContent ("threadpool", name.c_str ()));
        }
        else if (tp.type () == ThreadPoolWithLanes::meta)
        {
          ThreadPoolWithLanes tpwl = ThreadPoolWithLanes::Cast (tp);
          map<PICML::ThreadPoolWithLanes, bool>::const_iterator i;
          i = this->tpWithLanes_.find (tpwl);
          if (i == this->tpWithLanes_.end ())
          {
            this->push ();
            this->curr_ = this->resources_;
            tpwl.Accept (*this);
            this->pop ();
          }

          std::string name = tpwl.name ();
          this->curr_->appendChild (
          this->createSimpleContent ("threadpool", name.c_str ()));
        }
      }
    }

    //resolve the bandedConnection ref
    BandedConnectionRef bcr = ps.BandedConnectionRef_child ();
    if (bcr != Udm::null)
    {
      BandedConnection bc = bcr.ref ();
      if (bc != Udm::null)
      {
        map<PICML::BandedConnection, bool>::const_iterator i;
        i = this->connections_.find (bc);
        if (i == this->connections_.end ())
        {
          this->push ();
          this->curr_ = this->resources_;
          bc.Accept (*this);
          this->pop ();
        }

        std::string name = bc.name ();
        this->curr_->appendChild (
        this->createSimpleContent ("priorityBandedConnection", name.c_str ()));
      }


    }

    //restore curr
    this->pop ();
  }


  void RTConfigVisitor::Visit_PolicySetRef (const PolicySetRef& psr)
  {
    if (psr == Udm::null)
      return;

    PolicySet ps = psr.ref ();
    if (ps != Udm::null)
      ps.Accept (*this);
  }


//========================================================================
// Predefined Types
  void RTConfigVisitor::Visit_Boolean(const Boolean& boolean)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_boolean"));
    this->pop();

  }

  void RTConfigVisitor::Visit_Byte(const Byte& byte)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_octet"));
    this->pop();
  }

  void RTConfigVisitor::Visit_String(const String& str)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_string"));
    this->pop();
  }

  void RTConfigVisitor::Visit_RealNumber(const RealNumber& real)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_double"));
    this->pop();
  }

  void RTConfigVisitor::Visit_ShortInteger(const ShortInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_short"));
    this->pop();
  }

  void RTConfigVisitor::Visit_LongInteger(const LongInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_long"));
    this->pop();
  }

}//end of namespace PICML

