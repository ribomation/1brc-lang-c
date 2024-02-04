use strict;
use warnings;

my $filename = "../../data/stations.txt";

my %letters;
my $max_values = 0;
my $max_values_char;
my $max_length = 0;
my $max_length_str = 0;

open my $file, '<', $filename || die "cannot open $filename: $!";
while (my $line = <$file>) {
    chomp $line;
    my ($name, $value) = split ';', $line;

    my $ch = substr $name, 0, 4;
    $letters{$ch}++;

    if ($letters{$ch} > $max_values) {
        $max_values = $letters{$ch};
        $max_values_char = $ch
    }

    if (length($name) > $max_length) {
        $max_length = length($name);
        $max_length_str = $name;
    }
}
close $file;

foreach my $ch (sort keys %letters) {
    printf "%s: %d\n", $ch, $letters{$ch};
}
printf "total elems  : %d\n", scalar( keys %letters);
printf "max elems/key: %s=%d\n", $max_values_char, $max_values;
printf "max chars/str: %s=%d\n", $max_length_str, $max_length;

