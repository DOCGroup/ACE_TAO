// $Id$

ACE_INLINE ComponentServer*
Container::get_component_server()
{
  return this->parent_;
}

ACE_INLINE CCMHomes*
Container::get_homes()
{
  return this->homes_;
}

ACE_INLINE ConfigValues*
Container::get_configuration()
{
  return this->config_;
}

ACE_INLINE void
Container::set_configuration (ConfigValues* config)
{
  this->config_ = config;
}
