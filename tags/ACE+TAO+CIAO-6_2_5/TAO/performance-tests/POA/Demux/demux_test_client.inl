//$Id$
static void M302 (Demux_Test_ptr objref)
{
  objref->M302 ();
}

//FUZZ: disable check_for_lack_ACE_OS
static void shutdown (Demux_Test_ptr objref)
{
  objref->shutdown ();
}
//FUZZ: enable check_for_lack_ACE_OS

int Demux_Test_Client::init_operation_db (void)
{
   this->op_db_[0].op_ = M302;
   this->op_db_[1].op_ = shutdown;
   return 0;
}

