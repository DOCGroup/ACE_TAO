// $Id

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
  try
    {
      if (this->remora_agent_ != 0)
	{
	  this->remora_agent_->removeControl(this->control_.label_);
	  this->remora_agent_->_release();
	}
    }
  catch(remora::Invalid_Statistic& excp)
    {
      cerr << "Tried to remove invalid statistic.";
    }
  catch(CORBA::SystemException& excp)
    {
      CORBA::release(this->remora_agent_);
    }
}

void
Remora_Import::init()
{
  try
    {
      int argc = 1;
      char* argv[1];
      argv[0] = CONTROL_AGENT_NAME;

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      
      this->remora_agent_ =
	remora::Remora_Controls_Agent::_bind(CONTROL_AGENT_NAME);

      this->remora_agent_->acceptNewControl(this->control_);
    }
  catch(CORBA::Exception& excp)
    {
      cerr << excp << endl;
    }
}

CORBA::Long
Remora_Import::grab_value(void)
{
  try
    {
      if (this->remora_agent_ != 0)
	{
	  this->remora_agent_->getControlState(this->control_);
	}
      
      // C++ exception handling needs a 'finally' construct
      return this->control_.value_;
    }
  catch(CORBA::SystemException& sysex)
    {
      cerr << sysex << endl;
      this->remora_agent_->_release();
      this->remora_agent_ = 0;

      return this->control_.value_;
    }
  catch(remora::Invalid_Control& excp)
    {
      try
	{
	  this->remora_agent_->acceptNewControl(this->control_);
	}
      catch(CORBA::SystemException& sysex2)
	{
	  return this->control_.value_;
	}

      return this->control_.value_;
    }
}
