/**

@mainpage Implementation Repository

@section intro Introduction

The purpose of the Implementation Repository (ImR) in TAO is for the automatic
activation of a TAO server when a method is invoked but the server is not
running. It does this by working with the server to keep track of when it is
activated and stores information on how to reactivate it. Method
invocations on the server will actually be sent to the Implementation
Repository, which will then be able to start the server process if it is not
already running and forward the invocation to the real server.

@section resources Resources

The ImR in TAO was first based on a paper on the subject by 
<a href="http://www.triodia.com/staff/michi-henning.html">Michi Henning</a> 
called <a href="http://www.triodia.com/staff/michi/cacm.pdf">Binding,
Migration, and Scalability in CORBA</a>. Mr. Henning later went on to
coauthor the book 
<a href="http://cseng.awl.com/bookdetail.qry?ISBN=0-201-37927-9&amp;ptype=0">
Advanced CORBA Programming in C++</a> and included a chapter on the 
Implementation Repository. The description of the ImR in this chapter is used 
as the specification for TAO's ImR.

@ref usersguide - Overall documentation on how to use the ImR in your programs

@ref future - Future Work 

@ref ntservice - Running the ImR as a NT Service

@ref imrandnaming - Using the Naming Service with the Implmentation Repository

@section authors Authors

The guy currently in charge of the ImR is Darrell Brunsch
@<<a href="mailto:brunsch@cs.wustl.edu">brunsch@cs.wustl.edu</a>@>.
You can reach me by either email (which is better), or through the 
<a href="http://www.cs.wustl.edu/~schmidt/ACE-mail.html">ACE mailing list</a> 
@<<a href="mailto:ace-useres@cs.wustl.edu">ace-users@cs.wustl.edu</a>@>,
or through the <a href="news:comp.soft-sys.ace">comp.soft-sys.ace</a> 
newsgroup. The newsgroup mirrors the mailing list, but not the other way. 
Also, if you use the mailing list, it is best if you join it before sending 
a question since responses are usually just replied back to the group.

The NT Service part of the ImR was developed by Jeff Parsons @<<a
href="mailto:parsons@cs.wustl.edu">parsons@cs.wustl.edu</a>@>.  He can
also be reached via the same channels.

*/

/**
@page future Future Work

As with any program, there is always a wishlist of things to do.

@subsection logicalnames Logical Server names 

In the IDL interface, but not used on the server side or fully implemented in 
tao_ImR.

@subsection shutdown Server Shutdown

Only cooperative shutdown implemented right now.  Since we keep track of the
Process ID, then we could kill the server if it doesn't cooperate.

@subsection optimization Client-side Optimizations

Nothing yet.

@subsection security Server Security

Nothing really here.  There is two main things I have in mind.  

First, the security service would be useful to restrict access to some
of the Administration methods

Second, without a security service we should provide some sort of flag to
the ImR that disables the Administration methods.  That way, the database
can be set up with the servers in a controlled run of the ImR.  Then the
ImR could be run again reading in the database without Admin access.

@subsection federations Federations

Nothing yet.

@subsection dllserver DLL servers

Nothing yet.

*/

/**
@page usersguide Implementation Repository User's Guide

In order for a server to make use of the Implementation Repository, it must 
communicate with the ImR to keep it up to date on such things as the server's 
running status. These functions now are contained within the POA, so when a 
Persistent POA is used on a server that has -ORBUseImR specified, it will 
communicate with an Implementation Repository, if one is available.

@subsection use How is the ImR used?

The main steps for the lifetime of a server that uses the ImR are generally 
the following:

<ol>
  <li>Register name and startup commands with the ImR using <b>tao_ImR<br>
    </b><em>Example:<code>  </code> </em><code> tao_ImR -ORBInitRef 
    ImplRepoService=file://implrepo.ior add plane -c &quot;airplane_server -i 
    -ORBInitRef ImplRepoService=file://implrepo.ior&quot;<br></code>
    <br> 
    Note that the name used to register the server is the name of the POA
    which the objects are created in. So in this example, the airplane_server 
    creates a POA called &quot;plane&quot; and activates its servants under 
    it.<br>
  <li>Start the server once to generate an ImR-ified IOR<br>
  <li>Start clients and pass them the above IOR<br>
  <li>Clients will use the IOR, which will automatically go through the ImR<br>
  <li>The ImR will start the server if it is not already running<br>
  <li>At any time when the server is not currently in use by a client, it can be 
      shut down using <strong>tao_ImR<br></strong><em>Example:</em><code> tao_ImR 
      -ORBInitRef ImplRepoService=file://implrepo.ior shutdown plane<br>
    </code>
  <li>After the server isn't needed anymore, it can be removed from the ImR database 
      using <strong>tao_ImR<br></strong><em>Example:<code> </em>tao_ImR -ORBInitRef 
      ImplRepoService=file://implrepo.ior remove plane</code>
</ol>

@subsection serverresponsibilities So what does the server need to do?

As of TAO 1.0.9, the Implementation Repository support on the server-side has
been integrated into the POA. Previously, the IR_Helper class needed to be
used explicitly. Now only the &quot;-ORBUseImR 1&quot; command line
argument needs to be used.

There are a couple of restrictions on the server though. Only objects within 
a persistent POA will be supported by the ImR. Also the Implementation 
Repository will key its entries on POA name. What this means for the server 
is that each server must have unique persistent POA names.

@subsection defaultinitref Use of -ORBDefaultInitRef with the ImR

As mentioned in the INS documentation (in TAO/docs/INS.html), a base IOR
can be passed to the ORB.  Then when resolve_initial_reference is called,
the ORB can append the service name to the base IOR to form a full IOR.

When used with the ImR, this can be a powerful feature.  If the ImR's endpoint
is used as the base IOR, then the ImR can be used to provide many services via
the resolve_initial_reference functionality.

For example, let's say the ImR service is running on doriath on port 5555 and 
the Name Service is already registered with the ImR (in that the ImR knows how
to start the Name Service).

Now we should be able to run some client that uses the Name Service like this:

<code>client -ORBDefaultInitRef corbaloc:iiop:doriath:5555/</code>

When the client calls resolve_initial_reference("NameService"), the ORB will 
resolve that to "corbaloc:iiop:doriath:5555/NameService".  The ImR
recognizes this IOR as a pointer to the NameService, and will then
start it up if necessary.  Finally, it will forward the client to the
Name Service.

Services used in this way have two requirements:

- The server must be registered as the request_initial_reference name.  For
  example, the Name Service is registered as the "NameService", and thus also
  contains a POA with the name "NameService".
- The server must also be able to handle the INS name 
  "corbaloc:iiop:machine:port/poa_name", where the poa_name is the
  same name as above.

@subsection activationmodes What are activation modes

Each server can have one of three different types of activation modes:

<ul>
  <li>NORMAL is the default.  The server can be started via tao_ImR,
      the command line, and with client requests.
  <li>MANUAL specifies that the server shouldn't be activated with a
      client request but can be activated through tao_ImR or via the
      command line.
  <li>PER_CLIENT specifies that each request to the ImplRepo will
      result in a new server process started up.  Because clients
      cache the forwarded reference, there is one server per client
      (more or less).  There are some exceptions, such as if the
      original IOR is used in different threads (each thread would get
      a different server).  <b>Note:</b> The Implementation Repository
      doesn't store any information about the started servers in this
      mode, so it cannot be used to shut down the servers.  So the
      servers must have an alternative way of shutting down. 
  <li>AUTO_START specifies that a server should be activated when the
      Implementation Repository is started.  tao_ImR also has an
      autostart command to activate all servers marked AUTO_START.
</ul>

@subsection taoImRior Using the tao_ImR ior command

First, some background.

For the longest time, there was no way with TAO's Implementation Repository 
to register a server and then start using the client immediately.  The server 
had to be started once just for the purpose of creating an IOR for the client 
to use.  The problem was that it was very difficult to create an IOR without 
running the server.

It would be nice to be able to  generate a valid IOR without requiring the 
program to be run. A valid IOR in this case requires two major things.  First 
it requires the endpoint of the ImR. This is relatively easy to get, since it 
is encoded in the ImR's IOR.  Second it also requires an object key.  At the 
least, this involves both the POA hierarchy and the object name.

So if we knew the POA and object names, then we should be able to create an 
IOR for the server. One possibility would be to have tao_ImR ask for both the 
POA and the object, and then create the POA hierarchy to generate an IOR.  
Doing the generation is this manner (letting the POA create the reference) 
shields us from changes in the IOR generation schemes.  Since we are using 
the same code that the server would use, our IORs would be up to date.

It ends up there is an easier way to do this.  The Interoperable Naming 
Service is intended to be used in situations where an IOR could be created by 
hand.  Using the same information as above, it is not difficult to take the 
endpoint information from the ImR and attach the POA name.  For example, 
let's say that we are running the ImR on ringil.ece.uci.edu at port 5000.  
The endpoint would be
&quot;corbaloc:iiop:1.2@ringil.ece.uci.edu:5000&quot;.  If we are
creating an IOR for the nestea server, we'd just need to attach
&quot;/nestea_server&quot; to the end of the endpoint.  Now we have an
IOR.

So what does this mean for the server?

The main issue here is that the server must be changed to support the 
simplified name. This can be done by using the IORTable like this:

<CODE>
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->bind (poa_name, server_str.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
</CODE>

These lines, as taken from the nestea_server example, just uses the same 
poa_name as registered with the ImR and associates it with the server_obj 
object in the IOR table.  Because the ImR will be able to handle the 
simplified name (if it uses the POA name scheme) then this IOR will work.

Just one more thing, each object that needs an IOR needs to be registered 
with the IOR table.  But this raises the problem of uniqueness; they all 
can't have the same name.  The ImR will actually only look at the name part 
of the simplified IOR up to the first &quot;/&quot;.  So both 
&quot;corbaloc:iiop:1.2@ringil:5000/nestea_server/foo&quot; and 
&quot;corbaloc:iiop:1.2@ringil:5000/nestea_server/bar&quot; will be treated by 
the ImR as objects in the &quot;nestea_server&quot; server.

@subsection persistence Persistent Implementation Repository

Tao's Implementation Repository can be made persistent by doing two things:

<ul>
<li>
Always start up the Implementation Repository on the same port. This ensures that
the Implementation Repository will not have to re-ImR-ify the IORs of every server
registered to it each time it starts up. The way to accomplish this is to add<br>
-ORBEndpoint iiop://(hostname):(portnumber)<br>

 to the ImR's startup command line. The host
name can be obtained portably in C++ code with the lines<br>

	ACE_INET_addr ad;<br>
	char *hostname = ad.get_host_name ();<br>

or in a Perl script by adding<br>

	use Sys::Hostname;<br>
	$hostname = hostname();<br>

There are even specific port numbers, assigned to the OMG by the IANA,
which can be used for this purpose. They are 683 (for IIOP) and 684
(for IIOP over SSL).  For more information about this, see
<a href="http://www.iana.org/">http://www.iana.org/</a>
and <a href="http://www.isi.edu/in-notes/iana/assignments/port-numbers">
http://www.isi.edu/in-notes/iana/assignments/port-numbers</a>.<br><br>
<li>
Pass the ImR a filename to use for the backing store, specified by the
command line option<br>

-p (filename)<br>

This option must be used the first and every subsequent time the
persistent ImR is started up.

</ul>
*/

/**
@page ntservice Running as an NT service

The ImplRepo_Service.exe can now also function as a Windows NT
Service.  The -c option can be used to install and remove the service
(this requires Administrator access on the machine).

@note When using the ImplRepo_Service as a service, it must have all
of its required ACE/TAO DLL's in the path or in the same directory.
For example, the run_test.pl copies ImplRepo_Service.exe to the
ACE_wrappers@\bin directory before using "-c install".

The service can be then started either from the Windows NT "Services"
Admin Tool or via the "net" program on the command line:

<CODE>net start "TAO Implementation Repository"</CODE>

The Implementation Repository supports start and stop but not pause.

@subsection serviceopts Service Options

When installing the Implementation Repository as a service, there isn't an
opportunity to specify any command line options.  So if you want to specify
that the ImplRepo_Service should output to a file or set the debug level to 2,
for example, you will have to manually edit the registry to do this.

The magic key for extra options to pass to the Implementation Repository is
<B>SYSTEM\CurrentControlSet\Services\TAOImplRepo\Parameters\ORBOptions</B> 
under <B>HKEY_LOCAL_MACHINE</B>.  This value should be a MULTI_SZ value with
each parameter in separate strings.  Since it is a MULTI_SZ, you will need to
use regedt32.exe to add this, since regedit.exe doesn't support MULTI_SZ types
yet.  

So let's say you want to up the debug level to 2 and record the output to the
C:@\ImplRepo.log file.  First you will have to install the ImplRepo_Service
as a service to create the TAOImplRepo subkey under Services.  Now fire up 
regedt32 and browse to the TAOImplRepo key.  The Parameters key under 
TAOImplRepo will now need to be created.  And now you can create the 
"ORBOptions" value in Parameters.  Now when the multi-string editor pops up,
add the following on separate lines:

- -ORBLogFile
- C:@\ImplRepo.log
- -ORBDebugLevel
- 2

And when the ImplRepo_Service is run as a service (and only when it runs as
a service) it will use these options.

*/

/**
@page imrandnaming Naming Service

@subsection lowdown The Short Answer

Register the Naming Service with an Implementation Repository using the
POA name "NameService".  So for example:

<CODE>tao_imr add NameService -c "Naming_Service -ORBUseIMR 1"</CODE>

And then an IOR can be created by:

<CODE>tao_imr ior NameService</CODE>

The above commands depend on multicast (since the commands are shorter and
easier put in documents).  You might need to add "-ORBInitRef 
ImplRepoService=..." for a more robust solution.

@subsection details Various Details

The Naming Service in TAO contains one persistant POA named "NameService".  
If -ORBUseIMR 1 is passed to it, it will communicate with the ImR as any 
other persistent POA does.  Also, the Naming Service recognizes the INS
object key "NameService".  This allows you to use the "tao_imr ior"
command to create corbaloc IORs.

NameService was chosen because resolve_initial_references () uses that 
name.  And that allows us to do interesting things with ORBDefaultInitRef
(as shown in the @ref defaultinitref section).

*/
