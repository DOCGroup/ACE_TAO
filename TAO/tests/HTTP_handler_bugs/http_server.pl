#!/usr/bin/perl


# Check if we are running on Windows
my $mswin;
if($^O=~/mswin/i) { $mswin=1; }
else { $mswin=0; }


{
	package MyWebServer;
	use HTTP::Server::Simple::CGI;
	use base qw(HTTP::Server::Simple::CGI);
	
	# corbaloc:iiop:127.0.0.1:12345/GenericService
	my $IOR="IOR:010000000100000000000000010000000000000026000000010100cd0a0000003132372e302e302e310039300e00000047656e6572696353657276696365";

	sub handle_request {
		my $self = shift;
		my $cgi  = shift;
		my $path = $cgi->path_info();
		if ($path eq "/ior_with_http_response_headers") {
			# Normal response is sent, we want to check for bug where IOR string is not zero terminated
			print "HTTP/1.0 200 OK\r\n";
			print $cgi->header,
						$IOR;
		}
		elsif ($path eq "/ior_without_http_response_headers_zero_terminated") {
			# Only IOR string is sent as a response, without any HTTP headers.
			# We zero terminate IOR to avoid first non-zero-termination bug
			print $IOR.pack("C",0);
		}		
		else {
			print "HTTP/1.0 404 Not found\r\n";
			print $cgi->header,
						$cgi->start_html('Not found'),
						$cgi->h1('Not found'),
						$cgi->end_html;
		}
	}
}

# Prevent printing to STDOUT of banner from HTTP::Server::Simple
open STDOUT, '>>', '/dev/null';

# HTTP_server_port  "12345"
# $pid returned here is wrong on Windows
my $pid = MyWebServer->new(12345)->background();

sleep 300; 	# 	Sleep 300 seconds, that is enough for test to finish

if($mswin) {		# Use Windows stuff only on Windows
	require Win32::Process;
	Win32::Process->import();
	my $exitcode;
	my $mypid=Win32::Process::GetCurrentProcessID();
	Win32::Process::KillProcess($mypid, $exitcode); 	# Commit suicide
}
else {
	kill $$; 	# Commit suicide
}


