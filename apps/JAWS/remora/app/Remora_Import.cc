// $Id$ 

#include "Remora_Import.h"

Remora_Import::Remora_Import(const char* label, int value,
			     int min_value, int max_value)
  : remora_agent_(0)
{
  this->control_.value_ = value;
  this->control_.label_ = label;
  this->control_.max_ = max_value;
  this->control_.min_ = min_value;

  this->init();
}

Remora_Import::Remora_Import(const Remora_Import& import)
  : remora_agent_(import.remora_agent_)
{
  this->control_.value_ = import.control_.value_; 
  this->control_.label_ = import.control_.label_;
  this->control_.max_ = import.control_.max_;
  this->control_.min_ = import.control_.min_;
}

Remora_Import::~Remora_Import()
{
  PMCTRY
    {
      if (this->remora_agent_ != 0)
	{
	  this->remora_agent_->removeControl(this->control_.label_);
	  this->remora_agent_->_release();
	}
    }
  PMCCATCH(remora::Invalid_Statistic, excp)
    {
      cerr << "Tried to remove invalid statistic.";
    }
  PMCAND_CATCH(CORBA::SystemException, excp)
    {
      CORBA::release(this->remora_agent_);
    }
  PMCEND_CATCH
}

void
Remora_Import::init()
{
  PMCTRY
    {
      int argc = 1;
      char* argv[1];
      argv[0] = CONTROL_AGENT_NAME;

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      
      cout << this->control_.label_ << " Binding to server..." << endl;
      this->remora_agent_ =
	remora::Remora_Controls_Agent::_bind(CONTROL_AGENT_NAME);

      cout << this->control_.label_ << " Registering Control..." << endl;
      this->remora_agent_->acceptNewControl(this->control_);
    }
  PMCCATCH(CORBA::Exception, excp)
    {
      cerr << excp << endl;
    }
  PMCEND_CATCH
}

CORBA::Long
Remora_Import::grab_value(void)
{
  PMCTRY
    {
      if (this->remora_agent_ != 0)
	{
	  cout << this->control_.label_ << " Grabbing a value from the client." << endl;
	  this->remora_agent_->getControlState(this->control_);
	}
      
      // C++ exception handling needs a 'finally' construct
      return this->control_.value_;
    }
  PMCCATCH(CORBA::SystemException, sysex)
    {
      cerr << sysex << endl;
      this->remora_agent_->_release();
      this->remora_agent_ = 0;

      return this->control_.value_;
    }
  PMCAND_CATCH(remora::Invalid_Control, excp)
    {
      PMCTRY
	{
	  this->remora_agent_->acceptNewControl(this->control_);
	}
      PMCCATCH(CORBA::SystemException, sysex2)
	{
	  return this->control_.value_;
	}
      PMCEND_CATCH

      return this->control_.value_;
    }
  PMCEND_CATCH
}
