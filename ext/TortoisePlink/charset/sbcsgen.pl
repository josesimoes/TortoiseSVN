#!/usr/bin/env perl -w

# This script generates sbcsdat.c (the data for all the SBCSes) from its
# source form sbcs.dat.

$infile = "sbcs.dat";
$outfile = "sbcsdat.c";

open FOO, $infile;
open BAR, ">$outfile";
select BAR;

print "/*\n";
print " * sbcsdat.c - data definitions for single-byte character sets.\n";
print " *\n";
print " * Generated by sbcsgen.pl from sbcs.dat.\n";
print " * You should edit those files rather than editing this one.\n";
print " */\n";
print "\n";
print "#ifndef ENUM_CHARSETS\n";
print "\n";
print "#include \"charset.h\"\n";
print "#include \"internal.h\"\n";
print "\n";

my $charsetname = undef;
my @vals = ();

my @charsetnames = ();
my @sortpriority = ();

while (<FOO>) {
    chomp;
    if (/^charset (.*)$/) {
	$charsetname = $1;
	@vals = ();
	@sortpriority = map { 0 } 0..255;
    } elsif (/^sortpriority ([^-]*)-([^-]*) (.*)$/) {
	for ($i = hex $1; $i <= hex $2; $i++) {
	    $sortpriority[$i] += $3;
	}
    } elsif (/^[0-9a-fA-FX]/) {
	push @vals, map { $_ eq "XXXX" ? -1 : hex $_ } split / +/, $_;
	if (scalar @vals > 256) {
	    die "$infile:$.: charset $charsetname has more than 256 values\n";
	} elsif (scalar @vals == 256) {
	    &outcharset($charsetname, \@vals, \@sortpriority);
	    push @charsetnames, $charsetname;
	    $charsetname = undef;
	    @vals = ();
	    @sortpriority = map { 0 } 0..255;
	}
    }
}

print "#else /* ENUM_CHARSETS */\n";
print "\n";

foreach $i (@charsetnames) {
    print "ENUM_CHARSET($i)\n";
}

print "\n";
print "#endif /* ENUM_CHARSETS */\n";

sub outcharset($$$) {
    my ($name, $vals, $sortpriority) = @_;
    my ($prefix, $i, @sorted);

    print "static const sbcs_data data_$name = {\n";
    print "    {\n";
    $prefix = "    ";
    @sorted = ();
    for ($i = 0; $i < 256; $i++) {
	if ($vals->[$i] < 0) {
	    printf "%sERROR ", $prefix;
	} else {
	    printf "%s0x%04x", $prefix, $vals->[$i];
	    die "ooh? $i\n" unless defined $sortpriority->[$i];
	    push @sorted, [$i, $vals->[$i], 0+$sortpriority->[$i]];
	}
	if ($i % 8 == 7) {
	    $prefix = ",\n    ";
	} else {
	    $prefix = ", ";
	}
    }
    print "\n    },\n    {\n";
    @sorted = sort { ($a->[1] == $b->[1] ?
	              $b->[2] <=> $a->[2] :
	              $a->[1] <=> $b->[1]) ||
                     $a->[0] <=> $b->[0] } @sorted;
    $prefix = "    ";
    $uval = -1;
    for ($i = $j = 0; $i < scalar @sorted; $i++) {
	next if ($uval == $sorted[$i]->[1]); # low-priority alternative
	$uval = $sorted[$i]->[1];
	printf "%s0x%02x", $prefix, $sorted[$i]->[0];
	if ($j % 8 == 7) {
	    $prefix = ",\n    ";
	} else {
	    $prefix = ", ";
	}
	$j++;
    }
    printf "\n    },\n    %d\n", $j;
    print "};\n";
    print "const charset_spec charset_$name = {\n" .
          "    $name, read_sbcs, write_sbcs, &data_$name\n};\n\n";
}
