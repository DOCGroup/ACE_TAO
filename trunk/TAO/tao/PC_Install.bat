@echo off
rem mkdir ..\proto\lib
rem copy .\Build\TAO.lib ..\proto\lib

rem copy %ACE_WRAPPERS%\ace.lib %TAO_ROOT%\proto\lib
rem copy \OFP\Workspace\libfiles\ace.lib ..\proto\lib

rem copy \bmendel\ACE_wrappers\ace\ace.lib ..\proto\lib

if not exist ..\proto\include mkdir ..\proto\include
copy align.h     ..\proto\include >NUL
copy cdr.h       ..\proto\include >NUL
copy cdr.i       ..\proto\include >NUL
copy factories.h ..\proto\include >NUL
copy factories.i ..\proto\include >NUL
copy giop.h      ..\proto\include >NUL
copy iiopobj.h   ..\proto\include >NUL
copy iioporb.h   ..\proto\include >NUL
copy roa.h       ..\proto\include >NUL
copy roa.i       ..\proto\include >NUL
copy svrrqst.i   ..\proto\include >NUL
copy params.h    ..\proto\include >NUL
copy params.i    ..\proto\include >NUL
copy connect.h   ..\proto\include >NUL
copy connect.i   ..\proto\include >NUL
copy xdr.h       ..\proto\include >NUL
copy marshal.h   ..\proto\include >NUL
copy typecode.i  ..\proto\include >NUL
copy marshal.i   ..\proto\include >NUL

if not exist ..\proto\include\corba mkdir ..\proto\include\corba
copy  any.h      ..\proto\include\corba >NUL
copy  any.i      ..\proto\include\corba >NUL
copy  boa.h      ..\proto\include\corba >NUL
copy  boa.i      ..\proto\include\corba >NUL
copy  corbacom.h ..\proto\include\corba >NUL
copy  except.h   ..\proto\include\corba >NUL
copy  nvlist.h   ..\proto\include\corba >NUL
copy  object.h   ..\proto\include\corba >NUL
copy  object.i   ..\proto\include\corba >NUL
copy  orbobj.h   ..\proto\include\corba >NUL
copy  orbobj.i   ..\proto\include\corba >NUL
copy  objtable.h ..\proto\include\corba >NUL
copy  optable.h  ..\proto\include\corba >NUL
copy  principa.h ..\proto\include\corba >NUL
copy  request.h  ..\proto\include\corba >NUL
copy  svrrqst.h  ..\proto\include\corba >NUL
copy  typecode.h ..\proto\include\corba >NUL
copy  orb.h      ..\proto\include\corba >NUL
copy  sequence.h ..\proto\include\corba >NUL
copy  stub.h     ..\proto\include\corba >NUL
copy  orbconf.h  ..\proto\include\corba >NUL
copy  debug.h    ..\proto\include\corba >NUL
