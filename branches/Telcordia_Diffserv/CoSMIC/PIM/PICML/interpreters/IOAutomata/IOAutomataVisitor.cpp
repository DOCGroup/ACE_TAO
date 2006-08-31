#include <algorithm>
#include <functional>
#include "IOAutomata/IOAutomataVisitor.h"
#include "UmlExt.h"

using xercesc::LocalFileFormatTarget;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMException;
using xercesc::XMLUni;
using xercesc::XMLException;
using xercesc::DOMText;

namespace PICML
{
  IOAutomataVisitor::IOAutomataVisitor (const std::string& outputPath)
    : impl_ (0), doc_ (0), root_ (0), curr_ (0), serializer_ (0), target_ (0),
      outputPath_ (outputPath), curr_state_ (0), curr_state_done_ (false)

  {
    this->init();
  }

  IOAutomataVisitor::~IOAutomataVisitor ()
  {
    if (this->doc_)
      this->doc_->release();
    delete this->target_;
  }

  void IOAutomataVisitor::init()
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

  void IOAutomataVisitor::initTarget (const std::string& fileName)
  {
    if (this->target_)
      delete this->target_;
    this->target_ = new LocalFileFormatTarget (fileName.c_str());
  }

  void IOAutomataVisitor::initDocument (const std::string& rootName)
  {
    if (this->doc_)
      this->doc_->release();
    // Create the document
    this->doc_ =
      this->impl_->createDocument (XStr ("http://www.raytheon.com/ComponentWLG/WLGCharacterization"),
                                   XStr (rootName.c_str()),
                                   0);
  }

  void IOAutomataVisitor::initRootAttributes()
  {
    this->doc_->setEncoding (XStr("UTF-8"));
    this->doc_->setVersion (XStr("1.0"));
    this->doc_->setStandalone (false);
    this->root_ = this->doc_->getDocumentElement();
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xsi"),
                                 XStr ("http://www.w3.org/2001/XMLSchema-instance"));
    this->root_->setAttribute (XStr ("xsi:schemaLocation"),
                               XStr ("http://www.raytheon.com/ComponentWLG/WLGCharacterization WLGCharacterization.xsd"));
    this->curr_ = this->root_;
  }

  void IOAutomataVisitor::dumpDocument()
  {
    this->serializer_->writeNode (this->target_, *this->doc_);
  }

  void IOAutomataVisitor::push()
  {
    this->curr_stack_.push (this->curr_);
  }

  void IOAutomataVisitor::pop()
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

  void IOAutomataVisitor::Visit_RootFolder(const RootFolder& rf)
  {
    std::set<InterfaceDefinitions>
      ntrface_defs = rf.InterfaceDefinitions_kind_children();
    for (std::set<InterfaceDefinitions>::iterator iter = ntrface_defs.begin();
         iter != ntrface_defs.end();
         ++iter)
      {
        InterfaceDefinitions ntrface = *iter;
        ntrface.Accept (*this);
      }
  }

  void IOAutomataVisitor::Visit_InterfaceDefinitions(const InterfaceDefinitions& ntrface_def)
	  {
		std::set<File> files 
			= ntrface_def.File_kind_children();
		for (std::set<File>::iterator iter = files.begin();
			iter != files.end();
			++iter)
		{
			File file = *iter;
			file.Accept (*this);
		}
	  }

  void IOAutomataVisitor::Visit_File(const File& file)
	  {
		std::set<Package> packages 
			= file.Package_kind_children();
		for (std::set<Package>::iterator iter = packages.begin();
			iter != packages.end();
			++iter)
		{
			Package package = *iter;
			package.Accept (*this);
		}
      }

  void IOAutomataVisitor::Visit_Package(const Package& package)
	  {
		std::set<Component> components 
			= package.Component_kind_children();
		for (std::set<Component>::iterator iter = components.begin();
			iter != components.end();
			++iter)
		{
			Component cmp = *iter;
			cmp.Accept (*this);
		}	  
	  }


  void IOAutomataVisitor::Visit_Component(const Component& comp)
  {
    std::string name = this->outputPath_ + "\\";
    name += comp.name();
    name += ".wlgChar";
    this->initTarget (name);
    this->initDocument ("wlg:Characterization");
    this->initRootAttributes(); // this->curr_ is ROOT now.
    
    std::set<Environment> envs = comp.Environment_kind_children ();
    for (std::set<Environment>::iterator itr = envs.begin();
          itr != envs.end ();
	  ++itr)
      {
	Environment env = *itr;
	env.Accept (*this);
      }

    std::set<PeriodicAction> pacts = comp.PeriodicAction_kind_children ();
    for (std::set<PeriodicAction>::iterator itr = pacts.begin();
          itr != pacts.end ();
	  ++itr)
      {
	PeriodicAction pact = *itr;
	pact.Accept (*this);
      }

    std::set<InEventPort> eventsinks = comp.InEventPort_kind_children ();
    for (std::set<InEventPort>::iterator itr = eventsinks.begin();
          itr != eventsinks.end ();
	  ++itr)
      {
	InEventPort ieport = *itr;
	ieport.Accept (*this);
      }

    // Dump out a Workload chracterization file
    this->dumpDocument();
  }


  void IOAutomataVisitor::Visit_State(const State& state)
	  {
	        bool sequence_ends_here = check_action_sequence_end(state.dstFinish ());
          
                if (! sequence_ends_here)
                  {
                    std::set<InternalPrecondition> intern_preconds = state.dstInternalPrecondition ();
		    for (std::set<InternalPrecondition>::iterator itr = intern_preconds.begin ();
			    itr != intern_preconds.end();
			    ++itr)
		    {
			    InternalAction intern_action = itr->dstInternalPrecondition_end ();
			    intern_action.Accept(*this);
                    }
                  }
	  }

  void IOAutomataVisitor::Visit_OutputAction(const OutputAction& op_action)
	  {
		DOMElement *pub_elm = this->doc_->createElement(XStr ("PublicationAction"));
		this->curr_->appendChild (pub_elm);
		this->push (); // push workload
		this->curr_ = pub_elm;

                std::set<ActionPropertyConnection> property_assocs = op_action.dstAssociation ();
		for (std::set<ActionPropertyConnection>::iterator itr = property_assocs.begin ();
			 itr != property_assocs.end ();
			 ++itr)
		 {
                        Property p = itr->dstAssociation_end ();
			p.Accept (*this);
		 }
               Output op = op_action.dstOutput ();
               AbstractOutPort outport = op.dstOutput_end ();
               if (Udm::IsDerivedFrom (outport.type(), OutEventPort::meta))
                 {
                  OutEventPort out_event_port = OutEventPort::Cast (outport);
                  Event event = out_event_port.ref ();
                  this->curr_->setAttribute (XStr ("eventType"), XStr (std::string(event.name())));
                 }
               else
                 {
                  RequiredRequestPort reqport = RequiredRequestPort::Cast (outport);
                 }
	       this->pop(); // pop workload
	  }

    bool IOAutomataVisitor::check_action_sequence_end(const std::set<Finish> &finish_set)
    {
      for (std::set<Finish>::const_iterator itr = finish_set.begin();
            itr != finish_set.end();
            ++itr)
        {
          State state = itr->dstFinish_end ();
          if (state == *(this->curr_state_))
            {
              this->curr_state_done_ = true;
              this->curr_state_ = 0;
              break;
            }
        }
      return this->curr_state_done_;
    }

  void IOAutomataVisitor::Visit_OutputState(const OutputState& op_state)
	  {
		OutputPrecondition op_precond = op_state.dstOutputPrecondition ();
		OutputAction op_action = op_precond.dstOutputPrecondition_end ();
		op_action.Accept (*this);

                bool sequence_ends_here = check_action_sequence_end(op_state.dstFinish ());

                if (! sequence_ends_here)
                  {
                    std::set<InternalPrecondition> intern_preconds = op_state.dstInternalPrecondition ();
		    for (std::set<InternalPrecondition>::iterator itr = intern_preconds.begin ();
			    itr != intern_preconds.end();
			    ++itr)
		    {
			    InternalAction intern_action = itr->dstInternalPrecondition_end ();
			    intern_action.Accept(*this);
		    }
                  }
	  }

  void IOAutomataVisitor::Visit_InternalAction(const InternalAction& intern_action)
	  {
	    DOMElement *intern_action_elm = this->doc_->createElement 
			(XStr (std::string (intern_action.name ())));
	    this->curr_->appendChild (intern_action_elm);
	    this->push (); // push workload
	    this->curr_ = intern_action_elm;

            std::set<ActionPropertyConnection> property_assocs = intern_action.dstAssociation ();
	    for (std::set<ActionPropertyConnection>::iterator itr = property_assocs.begin ();
		      itr != property_assocs.end ();
		      ++itr)
	      {
	          Property p = itr->dstAssociation_end ();
		  p.Accept (*this);
	      }
	      this->pop(); // pop workload
              InternalEffect intern_effect = intern_action.dstInternalEffect ();
              State nextstate = intern_effect.dstInternalEffect_end ();
	      if (Udm::IsDerivedFrom (nextstate.type(), OutputState::meta))
              {
                OutputState state = OutputState::Cast (nextstate);
                state.Accept (*this);
              }
              else if (Udm::IsDerivedFrom (nextstate.type(), TimerState::meta))
              {
                TimerState state = TimerState::Cast (nextstate);
                state.Accept (*this);
              }
 	      else if (Udm::IsDerivedFrom (nextstate.type(), StartupState::meta))
              {
                StartupState state = StartupState::Cast (nextstate);
                state.Accept (*this);
              }
              else
              {
                nextstate.Accept (*this);
              }
      }

  void IOAutomataVisitor::Visit_InputState(const InputState& ip_state)
	  {
            if (0 == this->curr_state_)
              {
                this->curr_state_ = &ip_state;
                this->curr_state_done_ = false;
              }
              // A sequence may end at the first state itself.
              bool sequence_ends_here = check_action_sequence_end(ip_state.dstFinish ());
              
              if (! sequence_ends_here)
                {
                  std::set<InternalPrecondition> intern_preconds = ip_state.dstInternalPrecondition ();
		  for (std::set<InternalPrecondition>::iterator itr = intern_preconds.begin ();
			  itr != intern_preconds.end();
			  ++itr)
		  {
			  InternalAction intern_action = itr->dstInternalPrecondition_end ();
			  intern_action.Accept(*this);
		  }
                }
	  }
   
  void IOAutomataVisitor::Visit_StartupState(const StartupState& sup_state)
	  {
            if (0 == this->curr_state_)
              {
                this->curr_state_ = &sup_state;
                this->curr_state_done_ = false;
              }
              // A sequence may end at the first state itself.
              bool sequence_ends_here = check_action_sequence_end(sup_state.dstFinish ());
              
              if (! sequence_ends_here)
                {
                  std::set<InternalPrecondition> intern_preconds = sup_state.dstInternalPrecondition ();
		  for (std::set<InternalPrecondition>::iterator itr = intern_preconds.begin ();
			  itr != intern_preconds.end();
			  ++itr)
		  {
			  InternalAction intern_action = itr->dstInternalPrecondition_end ();
			  intern_action.Accept(*this);
		  }
                }
	  }

  void IOAutomataVisitor::Visit_PeriodicAction (const PeriodicAction &pact)
    {
      		DOMElement* ers_elm = this->doc_->createElement (XStr ("PeriodicActivitySpec"));
		this->curr_->appendChild (ers_elm);
		this->push(); // push whatever is curr_
		this->curr_ = ers_elm;

                std::set<TimerPropertyConnection> property_assocs = pact.dstTimerPropertyConnection ();
		for (std::set<TimerPropertyConnection>::iterator itr = property_assocs.begin ();
 		     itr != property_assocs.end ();
 		     ++itr)
		 {
                        Property p = itr->dstTimerPropertyConnection_end();
                        p.Accept (*this);
		 }

	        //this->pop(); // pop PeriodicActivitySpec
		
		DOMElement *workload_elm = this->doc_->createElement (XStr ("Workload"));
  		this->curr_->appendChild (workload_elm); // append workload to PeriodicActivitySpec
		this->push (); // push PeriodicActivitySpec
		this->curr_ = workload_elm;

                TStateConnect tstate_conn = pact.dstTStateConnect ();
                TimerState tstate = tstate_conn.dstTStateConnect_end ();
		tstate.Accept (*this);
		
		this->pop(); // pop PeriodicActivitySpec
		this->pop(); // pop and restore curr_ to whatever was there
    }

  void IOAutomataVisitor::Visit_Environment (const Environment &env)
    {
      		DOMElement* ers_elm = this->doc_->createElement (XStr ("StartupSpec"));
		this->curr_->appendChild (ers_elm);
		this->push(); // push whatever is curr_
		this->curr_ = ers_elm;

		DOMElement *workload_elm = this->doc_->createElement (XStr ("Workload"));
  		this->curr_->appendChild (workload_elm); // append workload to EventReactionSpec
		this->push (); // push Startupspec
		this->curr_ = workload_elm;

                EnvConnect env_conn = env.dstEnvConnect ();
                StartupState sup_state = env_conn.dstEnvConnect_end ();
		sup_state.Accept (*this);
		
		this->pop(); // pop StartupSpec
		this->pop(); // pop and restore curr_ to whatever was there
    }

  void IOAutomataVisitor::Visit_TimerState (const TimerState &t_state)
    {
      if (0 == this->curr_state_)
        {
          this->curr_state_ = &t_state;
          this->curr_state_done_ = false;
        }

      // A sequence may end at the first state itself.
      bool sequence_ends_here = check_action_sequence_end(t_state.dstFinish ());
      
      if (! sequence_ends_here)
        {
          std::set<InternalPrecondition> intern_preconds = t_state.dstInternalPrecondition ();
	  for (std::set<InternalPrecondition>::iterator itr = intern_preconds.begin ();
		  itr != intern_preconds.end();
		  ++itr)
	  {
		  InternalAction intern_action = itr->dstInternalPrecondition_end ();
		  intern_action.Accept(*this);
	  }
        }
    }

  void IOAutomataVisitor::Visit_Property(const Property& property)
	  {
        	 this->curr_->setAttribute (XStr (std::string (property.name())), XStr(std::string(property.DataValue())));
	  }

  void IOAutomataVisitor::Visit_InputAction(const InputAction& ip_action)
	  {
		DOMElement* ers_elm = this->doc_->createElement (XStr ("EventReactionSpec"));
		this->curr_->appendChild (ers_elm);
		this->push(); // push whatever is curr_
		this->curr_ = ers_elm;

	        DOMElement *ipevent_elm = this->doc_->createElement (XStr ("InputEvent"));
		this->curr_->appendChild (ipevent_elm);
		this->push(); // push EventReactionSpec
		this->curr_ = ipevent_elm;

                std::set<ActionPropertyConnection> property_assocs = ip_action.dstAssociation ();
		for (std::set<ActionPropertyConnection>::iterator itr = property_assocs.begin ();
 		     itr != property_assocs.end ();
 		     ++itr)
		 {
                        Property p = itr->dstAssociation_end ();
                        p.Accept (*this);
		 }
                Input ip = ip_action.srcInput ();
                AbstractInPort inport = ip.srcInput_end ();
                if (Udm::IsDerivedFrom (inport.type(), InEventPort::meta))
                 {
                  InEventPort in_event_port = InEventPort::Cast (inport);
                  Event event = in_event_port.ref ();
                  this->curr_->setAttribute (XStr ("eventType"), XStr (std::string(event.name())));
                 }
                else
                 {
                  
                 }

	        this->pop(); // pop EventReactionSpec
		
		DOMElement *workload_elm = this->doc_->createElement (XStr ("Workload"));
  		this->curr_->appendChild (workload_elm); // append workload to EventReactionSpec
		this->push (); // push EventReactionSpect
		this->curr_ = workload_elm;

		InputEffect input_effect = ip_action.dstInputEffect ();
		InputState ipstate = input_effect.dstInputEffect_end ();
		ipstate.Accept (*this);
		
		this->pop(); // pop EventReactionSpec
		this->pop(); // pop and restore curr_ to whatever was there
	  }

  void IOAutomataVisitor::Visit_InEventPort(const InEventPort& ieport)
  {
	std::set<Input> input_associations = ieport.dstInput ();
	for (std::set<Input>::iterator itr = input_associations.begin ();
             itr != input_associations.end();
	     ++itr)
	  {
	      Input input = *itr;
              InputAction input_action = input.dstInput_end ();
	      input_action.Accept (*this);
	  }
  }

}