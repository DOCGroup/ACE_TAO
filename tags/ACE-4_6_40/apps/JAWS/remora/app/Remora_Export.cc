// $Id$

#include "Remora_Export.h"


Remora_Export::Remora_Export(const char* label, int value, int max_value)
  : remora_agent_(0)
{
  this->stat_.value_ = value;
  this->stat_.label_ = label;
  this->stat_.max_ = max_value;
  
  this->init();
}

Remora_Export::Remora_Export(const Remora_Export& export)
  : remora_agent_(export.remora_agent_)
{
  this->stat_.value_ = export.stat_.value_; 
  this->stat_.label_ = export.stat_.label_;
  this->stat_.max_ = export.stat_.max_; 
}

Remora_Export::~Remora_Export()
{
  PMCTRY
    {
      if (this->remora_agent_ != 0)
	{
	  this->remora_agent_->removeStatistic(this->stat_.label_);
	  this->remora_agent_->_release();
	}
    }
  PMCCATCH(remora::Invalid_Statistic, excp)
    {
    }
  PMCAND_CATCH(CORBA::SystemException, excp)
    {
      CORBA::release(this->remora_agent_);
    }
  PMCEND_CATCH
  
}

void
Remora_Export::init()
{
  PMCTRY
    {
      int argc = 1;
      char* argv[1];
      argv[0] = STATS_AGENT_NAME;

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      
      cout << this->stat_.label_ << " Binding to server..." << endl;
      this->remora_agent_ =
	remora::Remora_Statistics_Agent::_bind(STATS_AGENT_NAME);

      cout << this->stat_.label_ << " Registering Statistic..." << endl;
      this->remora_agent_->acceptNewStatistic(this->stat_);
    }
  PMCCATCH(CORBA::Exception, excp)
    {
      cerr << excp << endl;
    }
  PMCEND_CATCH
}

void
Remora_Export::update_value(int new_value)
{
  this->stat_.value_ = new_value;

  PMCTRY
    {
      cout << this->stat_.label_ << " Updating a value." << endl;
      if (this->remora_agent_ != 0 && (! this->remora_agent_->_non_existent()))
	this->remora_agent_->updateStatistic(this->stat_);
    }
  PMCCATCH(CORBA::SystemException, sysex)
    {
      cerr << sysex << endl;
      this->remora_agent_->_release();
      this->remora_agent_ = 0;
    }
  PMCAND_CATCH(remora::Invalid_Statistic, excp)
    {
      cerr << excp << endl;
    }
  PMCEND_CATCH
}

