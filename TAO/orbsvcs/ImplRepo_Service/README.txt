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

The IMR in TAO was first based on a paper on the subject by 
<a href="http://www.triodia.com/staff/michi-henning.html">Michi Henning</a> 
called <a href="http://www.triodia.com/staff/michi/cacm.pdf">Binding,
Migration, and Scalability in CORBA</a>. Mr. Henning later went on to
coauthor the book 
<a href="http://cseng.awl.com/bookdetail.qry?ISBN=0-201-37927-9&amp;ptype=0">
Advanced CORBA Programming in C++</a> and included a chapter on the 
Implementation Repository. The description of the IMR in this chapter is used 
as the specification for TAO's IMR.

@ref usersguide - Overall documentation on how to use the IMR in your programs

@ref future - Future Work 

@ref ntservice - Running the ImR as a NT Service

@section authors Authors

The guy currently in charge of the IMR is Darrell Brunsch
@<<a href="mailto:brunsch@cs.wustl.edu">brunsch@cs.wustl.edu</a>@>.
You can reach me by either email (which is better), or through the 
<a href="http://www.cs.wustl.edu/~schmidt/ACE-mail.html">ACE mailing list</a> 
@<<a href="mailto:ace-useres@cs.wustl.edu">ace-users@cs.wustl.edu</a>@>,
or through the <a href="news:comp.soft-sys.ace">comp.soft-sys.ace</a> 
newsgroup. The newsgroup mirrors the mailing list, but not the other way. 
Also, if you use the mailing list, it is best if you join it before sending 
a question since responses are usually just replied back to the group.

The NT Service part of the ImR was developed by Jeff Parsons
@<<a href="mailto:parsons@cs.wustl.edu">parsons@cs.wustl.edu</a>@>.
He can also be reached via the same channels.

*/

/**
@page future Future Work

As with any program, there is always a wishlist of things to do.

@subsection logicalnames Logical Server names 

In the IDL interface, but not used on the server side or fully implemented in 
tao_imr.

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
communicate with the IMR to keep it up to date on such things as the server's 
running status. These functions now are contained within the POA, so when a 
Persistent POA is used on a server that has -ORBUseIMR specified, it will 
communicate with an Implementation Repository, if one is available.

@subsection use How is the IMR used?

The main steps for the lifetime of a server that uses the IMR are generally 
the following:

<ol>
  <li>Register name and startup commands with the IMR using <b>tao_imr<br>
    </b><em>Example:<code>  </code> </em><code> tao_imr -ORBInitRef 
    ImplRepoService=file://implrepo.ior add plane -c &quot;airplane_server -i 
    -ORBInitRef ImplRepoService=file://implrepo.ior&quot;<br></code>
    <br> 
    Note that the name used to register the server is the name of the POA
    which the objects are created in. So in this example, the airplane_server 
    creates a POA called &quot;plane&quot; and activates its servants under 
    it.<br>
  <li>Start the server once to generate an IMR-ified IOR<br>
  <li>Start clients and pass them the above IOR<br>
  <li>Clients will use the IOR, which will automatically go through the IMR<br>
  <li>The IMR will start the server if it is not already running<br>
  <li>At any time when the server is not currently in use by a client, it can be shut down
    using <strong>tao_imr<br>
    </strong><em>Example:</em><code> tao_imr -ORBInitRef ImplRepoService=file://implrepo.ior shutdown
    plane<br>
    </code>
  <li>After the server isn't needed anymore, it can be removed from the IMR database using <strong>tao_imr<br>
    </strong><em>Example:<code> </em>tao_imr -ORBInitRef ImplRepoService=file://implrepo.ior remove plane</code>
</ol>

@subsection serverresponsibilities So what does the server need to do?

As of TAO 1.0.9, the Implementation Repository support on the server-side has
been integrated into the POA. Previously, the IR_Helper class needed to be
used explicitly. Now only the &quot;-ORBUseIMR 1&quot; command line
argument needs to be used.

There are a couple of restrictions on the server though. Only objects within 
a persistent POA will be supported by the IMR. Also the Implementation 
Repository will key its entries on POA name. What this means for the server 
is that each server must have unique persistent POA names.

@subsection activationmodes What are activation modes

Each server can have one of three different types of activation modes:

<ul>
  <li>NORMAL is the default.  The server can be started via tao_imr, the command line, and
      with client requests.
  <li>MANUAL specifies that the server shouldn't be  activated with a client request but can 
      be activated through tao_imr or via the command line.  
  <li>PER_CLIENT specifies that each request to the ImplRepo will result in a new server 
      process started up.  Because clients cache the forwarded reference, there is one server 
      per client (more or less).  There are some exceptions, such as if the original IOR
      is used in different threads (each thread would get a different server).  <b>Note:</b>
      The Implementation Repository doesn't store any information about the started servers
      in this mode, so it cannot be used to shut down the servers.  So the servers must 
      have an alternative way of shutting down. 
  <li>AUTO_START specifies that a server should be activated when the Implementation Repository
      is started.  tao_imr also has an autostart command to activate all servers marked 
      AUTO_START
</ul>

@subsection taoimrior Using the tao_imr ior command

First, some background.

For the longest time, there was no way with TAO's Implementation Repository 
to register a server and then start using the client immediately.  The server 
had to be started once just for the purpose of creating an IOR for the client 
to use.  The problem was that it was very difficult to create an IOR without 
running the server.

It would be nice to be able to  generate a valid IOR without requiring the 
program to be run. A valid IOR in this case requires two major things.  First 
it requires the endpoint of the IMR. This is relatively easy to get, since it 
is encoded in the IMR's IOR.  Second it also requires an object key.  At the 
least, this involves both the POA hierarchy and the object name.

So if we knew the POA and object names, then we should be able to create an 
IOR for the server. One possibility would be to have tao_imr ask for both the 
POA and the object, and then create the POA hierarchy to generate an IOR.  
Doing the generation is this manner (letting the POA create the reference) 
shields us from changes in the IOR generation schemes.  Since we are using 
the same code that the server would use, our IORs would be up to date.

It ends up there is an easier way to do this.  The Interoperable Naming 
Service is intended to be used in situations where an IOR could be created by 
hand.  Using the same information as above, it is not difficult to take the 
endpoint information from the IMR and attach the POA name.  For example, 
let's say that we are running the IMR on ringil.ece.uci.edu at port 5000.  
The endpoint would be &quot;iioploc://1.1@ringil.ece.uci.edu:5000&quot;.  If 
we are creating an IOR for the nestea server, we'd just need to attach 
&quot;/nestea_server&quot; to the end of the endpoint.  Now we have an IOR.

So what does this mean for the server?

The main issue here is that the server must be changed to support the 
simplified name. The code to do this is just one line:

<code>this->orb_->_tao_add_to_IOR_table (poa_name, server_obj.in ());</code>

This line, as taken from the nestea_server example, just uses the same 
poa_name as registered with the IMR and associates it with the server_obj 
object in the IOR table.  Because the IMR will be able to handle the 
simplified name (if it uses the POA name scheme) then this IOR will work.

Just one more thing, each object that needs an IOR needs to be registered 
with the IOR table.  But this raises the problem of uniqueness; they all 
can't have the same name.  The IMR will actually only look at the name part 
of the simplified IOR up to the first &quot;/&quot;.  So both 
&quot;iioploc://1.1@ringil:5000/nestea_server/foo&quot; and 
&quot;iioploc://1.1@ringil:5000/nestea_server/bar&quot; will be treated by 
the IMR as objects in the &quot;nestea_server&quot; server.

@subsection persistence Persistent Implementation Repository

Tao's Implementation Repository can be made persistent by doing two things:

<ul>
<li>
Always start up the Implementation Repository on the same port. This ensures that
the Implementation Repository will not have to re-IMR-ify the IORs of every server
registered to it each time it starts up. The way to accomplish this is to add<br>
-ORBEndpoint iiop://(hostname):(portnumber)<br>

 to the IMR's startup command line. The host
name can be obtained portably in C++ code with the lines<br>

	ACE_INET_addr ad;<br>
	char *hostname = ad.get_host_name ();<br>

or in a Perl script by adding<br>

	use Sys::Hostname;<br>
	$hostname = hostname();<br>

There are even specific port numbers, assigned to the OMG by the IANA, which can be used for
this purpose. They are 683 (for IIOP) and 684 (for IIOP SSL). For more information about this,
see <a href="http://www.iana.org/">http://www.iana.org/</a> and <a href="http://www.isi.edu/in-notes/iana/assignments/port-numbers">
http://www.isi.edu/in-notes/iana/assignments/port-numbers</a>.
<br><br>
<li>
Pass the IMR a filename to use for the backing store, specified by the command line option<br>

-p (filename)<br>

This option must be used the first and every subsequent time the persistent IMR is started up.

</ul>
*/

/**
@page ntservice Running as an NT service

These conditions must be met:

<ol>
<li> The user must be logged in as administrator, or have administrative 
priveleges (in order to register an NT service).
<li> ACE_ROOT must be set to the ACE_wrappers directory in the system 
environment. 
<li> ACE_ROOT/bin must be in the system path.
</ol>

To set the options for the TAO NT ImplRepo Sevice, go to the Services icon
in the Settings group under the start menu (start menu -> settings ->
services). There, highlight "TAO NT ImplRepo Service", which is the name
used by the ImplRepo Service when it is registered. After it's
highlighted, you should see at the bottom of the dialog box an area to
specify options for ImplRepo_Service.exe. Just enter the options you wish in 
that edit box and everything should just work. However, some options, such as
-ORBDebugLevel, won't work since an NT service can't write output to
standard out.

@subsetion syntax Syntax

C:@\@> NT_ImplRepo_Service [-i value] [-r] [-s] [-k] [-t n] [-d]

Optional Command-line Arguments

- -i value Install this program as an NT service, with specified startup
- -r       Remove this program from the Service Manager
- -s       Start the service
- -k       Kill the service
- -t value Set startup for an existing service
- -d       Debug; run as a regular application


Usage

To see different stages of an NT service application, you have to run the 
program several times, with different options.  Please note: run with only 
one option at a time.

First, you must initialize the service in the NT Service Control Manager 
database.  Run NT_ImplRepo_Service with -in, where n is	one of the following 
startup options:

<ul>
  <li>#define SERVICE_SYSTEM_START    0x00000001
  <li>#define SERVICE_AUTO_START      0x00000002
  <li>#define SERVICE_DEMAND_START    0x00000003
  <li>#define SERVICE_DISABLED        0x00000004
</ul>

If only -i is specified, SERVICE_DEMAND_START is default option.

Now you are ready to run the actual service. Run NT_ImplRepo_Service again, 
this time with -s option. If the service starts successfully, it will ring 
the system bell every second or so until the service is stopped.

To stop service execution, run NT_ImplRepo_Service with the -k option.

To remove the service from the Service Control Manager database, run 
NT_ImplRepo_Service with -r.

In addition, once you have initialized this service (by using the -i option) 
you can change its startup type to one of the other values above.  To do 
this, run NT_ImplRepo_Service with -tn option. n is as explained above for -i.

In order to debug the service's execution itself, use the -d option.

*/


