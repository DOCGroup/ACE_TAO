echo on
mkdir ..\proto\lib
copy TAO_Baseline.lib ..\proto\lib
rem copy %ACE_WRAPPERS%\ace.lib %TAO_ROOT%\proto\lib
copy \OFP\Workspace\libfiles\ace.lib ..\proto\lib

mkdir ..\proto\include
copy align.hh    ..\proto\include
copy thread.hh   ..\proto\include
copy cdr.hh      ..\proto\include
copy connmgr.hh  ..\proto\include
copy giop.hh     ..\proto\include
copy iiopobj.hh  ..\proto\include
copy iioporb.hh  ..\proto\include
copy roa.hh      ..\proto\include
copy tcpoa.hh    ..\proto\include
copy xdr.hh      ..\proto\include

mkdir ..\proto\include\corba
copy  any.hh      ..\proto\include\corba
copy  corbacom.hh ..\proto\include\corba
copy  except.hh   ..\proto\include\corba
copy  nvlist.hh   ..\proto\include\corba
copy  object.hh   ..\proto\include\corba
copy  orbobj.hh   ..\proto\include\corba
copy  principa.hh ..\proto\include\corba
copy  request.hh  ..\proto\include\corba
copy  svrrqst.hh  ..\proto\include\corba
copy  toa.hh      ..\proto\include\corba
copy  typecode.hh ..\proto\include\corba
copy  orb.hh      ..\proto\include\corba
copy  sequence.hh ..\proto\include\corba
copy  stub.hh     ..\proto\include\corba
copy  orbconf.hh  ..\proto\include\corba
copy  debug.hh    ..\proto\include\corba