//$Id$
static void M302 (Demux_Test_ptr objref
                  ACE_ENV_ARG_DECL)
{
  objref->M302 (ACE_ENV_SINGLE_ARG_PARAMETER);
}

static void shutdown (Demux_Test_ptr objref
                      ACE_ENV_ARG_DECL)
{
  objref->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int Demux_Test_Client::init_operation_db (void)
{
   this->op_db_[0].op_ = M302;
   this->op_db_[1].op_ = shutdown;
   return 0;
}

