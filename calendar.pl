#!/bin/perl
use strict;
use warnings;
use POSIX;

# variables
my $lmon = "";
my $lday = "";
my $contin = 0;

# is there a calendar file in the current directory?
open my $fp, "<", "calendar" or die "couldn't open calendar file";

# what time is it?
my $sdow = strftime( "%a", localtime());
my $smon = strftime( "%b", localtime());
my $nday = strftime( "%d", localtime());
my $nmon = strftime( "%m", localtime());

# get every line
while(<$fp>) {
  my $line = $_;

	# no tab, no parse
  if( $line !~ /\t/ ) {
		next;
	}

	# first tab?
	if( $line =~ /^\t/ ) {
		if( $contin == 0 ) {
			next;
		}
	}
	else {
		$contin = 0;
	}

	# nn/nn?
	if( $line =~ /\// ) {
		($lmon, $lday) = $line =~ m/([0-9]*)\/([0-9]*)/g;

		# today?
		if( ($lmon != $nmon) || ($lday != $nday) ) {
			next;
		}

		# trip the continuation flag
		$contin = 1;
 	}

  # print the line
	print $line;
}

