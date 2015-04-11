#!/usr/bin/env perl
use strict;
use warnings;
use Digest::MD5 qw( md5_hex );
use IPC::Run3 qw( run3 );

my @lines_to_process = ();

while (<>) {
  # Between start and end pattern, add lines to an array to process.
  if (/^~~~~$/../^####$/) {
      if (/^~~~~$/) {
          @lines_to_process = ();
      } elsif (/^####$/) {
          # Now process lines_to_process
          my $digest = md5_hex(@lines_to_process);
          my $stdin = join("", @lines_to_process);

          my $prog = 'NFAToDFA';
          my $conv_cmd = "~/code/$prog/dist/build/$prog/$prog";
          my $dot_cmd = "dot -Tsvg > $digest.svg";

          run3("$conv_cmd | $dot_cmd", \$stdin);

          my $prefix = "{{site.img_dir}}{{page.categories}}/{{page.date | date: '%Y-%m-%d'}}";
          print "![NFA with hash $digest]($prefix/$digest.svg)" . "\n";
            
      } else {
          push(@lines_to_process, $_);
      }

  } else {
      print;
  }
}
