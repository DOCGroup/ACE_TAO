// $Id$

namespace ConfigFactory {

ACE_INLINE
Default_Config_Factory::Default_Config_Factory (void)
  : Config_Factory (),
    test_config_(0), //default to ECConfig
    sched_type_(EDF) //default to EDF dispatching
{
}

} //namespace ConfigFactory
