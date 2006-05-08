#!/usr/bin/perl

# This is a sample script that may be used to create domain specific
# unique ID's rather than using UUID. In a typical scenario, this HTID
# generator would reside in the same domain as the target server, so
# that it could be used by any clients of the server. While use of UUID
# as a unique identifier is perfectly fine, use of a remote generator
# gives additional benefits, such as the ability to collect metrics on
# identity requestors.

print "Content-type:text/html\n\n";

$htid = $ENV{'REMOTE_ADDR'};

open(IN,"counts.txt") or dienice("Can't open counts.txt for writing: $!");

flock(IN,2);           # lock the file
seek(IN,0,0);          # rewind it to the beginning

$oldtimestamp = <IN>;         # read only the first line.
$save_time = $oldtimestamp;

close(IN);


$timestamp = time;

if ($oldtimestamp == $timestamp)
  {
    open (IN2, "sec_id.txt") or
      dienice ("Can't open sec_id.txt for writing: $!");

    flock (IN2, 2);
    seek (IN2, 0, 0);

    $secondary_id = <IN2>;

    # increment the secondary id
    ++$secondary_id;


    close (IN2);
  }
else {

   $secondary_id = 0;
}

   open(OUT2,">sec_id.txt") or
     dienice("Can't open outdata.txt for writing: $!");

  flock(OUT2,2);                # lock the file
   seek(OUT2,0,0);               # rewind it to the beginning

   print OUT2 "$secondary_id";

   close(OUT2);


#save the timestamp for next time
$oldtimestamp = $timestamp;


open(OUT,">counts.txt") or dienice("Can't open outdata.txt for writing: $!");

flock(OUT,2);                # lock the file
seek(OUT,0,0);               # rewind it to the beginning
print OUT "$oldtimestamp\n";
close(OUT);


$result_string = join (".$timestamp", $htid, ".$secondary_id");

print <<EndOfHTML;

$result_string

EndOfHTML
