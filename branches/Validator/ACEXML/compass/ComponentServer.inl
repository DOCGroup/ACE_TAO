// $Id$

using namespace Deployment;

ACE_INLINE ServerActivator*
ComponentServer::get_server_activator (void)
{
  return this->activator_;
}

ACE_INLINE Containers*
ComponentServer::get_containers (void)
{
  return this->containers_;
}

ACE_INLINE const ConfigValues*
ComponentServer::get_configuration (void)
{
  return this->config_;
}

ACE_INLINE void
ComponentServer::set_configuration (ConfigValues* config)
{
  this->config_ = config;
}
