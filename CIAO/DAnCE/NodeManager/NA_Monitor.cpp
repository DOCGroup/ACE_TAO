// NA_Monitor.cpp,v 1.4 2006/02/01 21:22:42 wotte Exp
//==========================================================================
/// The RSS Monitor class defination
/**
 *  @file NA_Monitor.cpp
 *
 *  @brief The Monitor which collects NA cpu util
 *         class defination
 *
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 */
//==========================================================================

#include "NA_Monitor.h"
#include "CIAO_common.h"
#include "ace/OS_NS_stdio.h"
#include "NodeManager_Impl.h"
#include "MonitorController.h"

#include <fstream>
#include <iostream>
#include <sstream>


extern "C" ACE_Proper_Export_Flag CIAO::MonitorBase * CIAO::createMonitor ()
{
  return new CIAO::NA_Monitor ();
}

CIAO::NA_Monitor::NA_Monitor ()
{
}

/// The Desctructor
CIAO::NA_Monitor::~NA_Monitor ()
{
}

int CIAO::NA_Monitor::initialize_params (
                                     ::Deployment::Domain& domain,
                                     int interval
                                     )
{
  current_domain_.reset (new ::Deployment::Domain (domain));
  this->interval_=interval;

  return 0;
}

int CIAO::NA_Monitor::start (CORBA::ORB_ptr)
{
  return 0;
}

int CIAO::NA_Monitor::stop ()
{
  return 0;
}

//::Deployment::Domain* CIAO::NA_Monitor::get_current_data ()
//{
//  //  if (CIAO::debug_level () > 9)
//    {
//      ACE_DEBUG ((LM_DEBUG ,
//                 "NA_Monitor::Inside the get_current_data of[%s]\n",
//                 current_domain_->node[0].name.in ()));
//    }
//
//  // first get the process id for the NA from the controller
//
//  NodeManager_Impl_Base::Component_Ids cids =
//    this->controller_->get_component_process ();
//
//  ACE_Unbounded_Set_Iterator<pid_t> iter (cids.process_ids_);
//
//  ACE_DEBUG ((LM_DEBUG , "The size of the Process Set is %d \n",
//	      cids.process_ids_.size ()));
//
//  size_t counter = 0;
//  // get process data for each NA process
//  for (iter = cids.process_ids_.begin ();
//       iter != cids.process_ids_.end ();
//       iter++,counter++)
//    {
//      // form the file name
//      char buffer[99];
//      memset (buffer , 0 , 99);
//
//      sprintf (buffer,"%s%d%s","/proc/",*iter,"/stat");
//
//      this->file_name_ = buffer;
//
//      CORBA::Double current_load = 0;
//
//      ACE_Unbounded_Set_Iterator<pid_data> iter_data (pid_set_);
//
//      for (iter_data = pid_set_.begin ();
//          iter_data != pid_set_.end ();
//          iter_data++)
//      {
//        pid_t pid = (*iter_data).pid_;
//        if (*iter == pid)
//          this->past_data_ = (*iter_data).prev_data_;
//      }
//
//
//      if (this->read_data (this->curr_data_) == 0)
//      {
//        current_load = ((this->curr_data_.utime + this->curr_data_.stime +
//              this->curr_data_.cutime + this->curr_data_.cstime -
//              this->past_data_.utime - this->past_data_.stime -
//              this->past_data_.cutime - this->past_data_.cstime) *
//            100.0)
//          / (this->interval_ * sysconf (_SC_CLK_TCK)) ;
//
//        //      ACE_DEBUG ((LM_INFO, "\n%3.2f\n", current_load));
//
//        std::cout << "\n The NA load is " << current_load << std::endl;
//
//        if (current_load > 100 || current_load < 0)
//          current_load = 100;
//
//        this->past_data_ = this->curr_data_;
//
//
//        if (iter_data == pid_set_.end ())
//        {
//          pid_data new_data;
//          new_data.pid_ = *iter;
//          new_data.prev_data_ = this->curr_data_;
//          pid_set_.insert_tail (new_data);
//        }
//        else
//          (*iter_data).prev_data_ = this->curr_data_;
//      }
//
//
//      CORBA::Any any;
//      any <<= current_load;
//
//      // here insert the util value, in the right position
//
//      for (unsigned int i = 0;
//          i < current_domain_->node[0].resource.length ();
//          i++)
//      {
//        if (!strcmp (current_domain_->node[0].resource[i].name, "NA_Monitor"))
//        {
//          // ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::The Resource found\n"));
//          // 	      for (unsigned int j = 0;
//          // 		   j < current_domain_->node[0].resource[i].property.length ();
//          // 		   j++)
//          // 		{
//          // 		  if (!strcmp (
//          // 			       current_domain_
//          // 			       ->node[0].resource[i].property[j].name.in (),
//          // 			       "LoadAverage"))
//          // 		    {
//          // 		      //ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::The property found\n"));
//          // 		      current_domain_->node[0].resource[i].property[j].kind =
//          // 			::Deployment::Quantity;
//          // 		      current_domain_->node[0].resource[i].property[j].value =
//          // 			any;
//          // 		    }
//          // 		}
//
//
//          if (current_domain_->node[0].resource[i].property.length () <
//              cids.process_ids_.size ())
//          {
//            current_domain_->
//              node[0].resource[i].property.length (cids.process_ids_.size ());
//          }
//
//          current_domain_->node[0].resource[i].property[counter].kind =
//            ::Deployment::Quantity;
//          current_domain_->node[0].resource[i].property[counter].value =
//            any;
//        }
//      }
//
//    }
//
//
//
//  if (CIAO::debug_level () > 9)
//    {
//      //ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::Exiting from the get_current_data function\n"));
//    }
//
//  return current_domain_.get ();
//}

::Deployment::Domain* CIAO::NA_Monitor::get_current_data ()
{
  // first get the process id for the NA from the controller

  NodeManager_Impl_Base::Component_Ids cids =
    this->controller_->get_component_process ();

  ACE_Unbounded_Set_Iterator<pid_t> iter (cids.process_ids_);

//   ACE_DEBUG ((LM_DEBUG , "The size of the Process Set is %d \n",
// 	      cids.process_ids_.size ()));

  size_t counter = 0;
  // get process data for each NA process

  for (iter = cids.process_ids_.begin ();
       iter != cids.process_ids_.end ();
       iter++,counter++)
    {
      CORBA::Double current_load = 0;

//      ACE_Unbounded_Set_Iterator<pid_data> iter_data (pid_set_);
//
//      for (iter_data = pid_set_.begin ();
//          iter_data != pid_set_.end ();
//          iter_data++)
//      {
//        pid_t pid = (*iter_data).pid_;
//        if (*iter == pid)
//          this->past_data_ = (*iter_data).prev_data_;
//      }
//
//      auto_ptr<data> curr_data = read_data (*iter);
//
//      current_load = ((curr_data->utime + curr_data->stime +
//            curr_data->cutime + curr_data->cstime -
//            past_data_.utime - past_data_.stime -
//            past_data_.cutime - past_data_.cstime) *
//          100.0)
//        / (this->interval_ * sysconf (_SC_CLK_TCK)) ;
//
//      //      ACE_DEBUG ((LM_INFO, "\n%3.2f\n", current_load));
//
//      //      std::cout << "\n The NA load is " << current_load << std::endl;
//
//      if (current_load > 100 || current_load < 0)
//        current_load = 100;
//
//      this->past_data_ = *curr_data.get ();
//
//
//      if (iter_data == pid_set_.end ())
//      {
//        pid_data new_data;
//        new_data.pid_ = *iter;
//        new_data.prev_data_ = *curr_data.get ();
//        pid_set_.insert_tail (new_data);
//      }
//      else
//        (*iter_data).prev_data_ = *curr_data.get ();


      CORBA::Any any;
      any <<= current_load;

      // here insert the util value, in the right position

      for (unsigned int i = 0;
          i < current_domain_->node[0].resource.length ();
          i++)
      {
        if (!strcmp (current_domain_->node[0].resource[i].name, "NA_Monitor"))
        {
          if (current_domain_->node[0].resource[i].property.length () <
              cids.process_ids_.size ())
          {
            current_domain_->
              node[0].resource[i].property.length (cids.process_ids_.size ());
          }

          current_domain_->node[0].resource[i].property[counter].kind =
            ::Deployment::Quantity;
          current_domain_->node[0].resource[i].property[counter].value =
            any;
        }
      }

    }



//   if (CIAO::debug_level () > 9)
//     {
//       //ACE_DEBUG ((LM_DEBUG , "CIAO::Monitor::Exiting from the get_current_data function\n"));
//     }

  return current_domain_.get ();
}

void CIAO::NA_Monitor::set_context (MonitorController* context)
{
  if (CIAO::debug_level () > 9)
    {
      ACE_DEBUG ((LM_DEBUG ,"Inside the SET CONTEXT FUNCTION\n"));
    }

  this->controller_ = context;
}


auto_ptr <CIAO::data>
CIAO::NA_Monitor::read_data (pid_t pid)
{

  // form the file name
  std::ostringstream str;

  str << "/proc/"
      << pid
      << "/stat";

  std::ifstream ifstream_;

//   ACE_DEBUG ((LM_DEBUG, "The file name is %s\n",
//               str.str ().c_str ()));

  ifstream_.open (str.str ().c_str (), std::ios_base::in);

  auto_ptr <CIAO::data> cpu_data (new CIAO::data());

  if (!ifstream_.is_open ())
  {
    ACE_DEBUG ((LM_DEBUG, "Error in opening proc/pid/stat/file"));
    cpu_data.reset ();
  }
  else
  {

    ifstream_ >> cpu_data->pid    >> cpu_data->comm  >> cpu_data->state
              >> cpu_data->ppid   >> cpu_data->pgrp  >> cpu_data->session
              >> cpu_data->tty_nr >> cpu_data->tpgid  >> cpu_data->flags
              >> cpu_data->minflt >> cpu_data->cminflt >> cpu_data->majflt
              >> cpu_data->cmajflt >> cpu_data->utime >> cpu_data->stime
              >> cpu_data->cutime  >> cpu_data->cstime;
  }

  ifstream_.close ();
  return cpu_data;
}
