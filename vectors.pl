print "extern alltraps\n";

for(my $i = 0; $i < 256; $i++){
	print "global vector$i\n";
	print "vector$i:\n";
	print "\tpush $i\n";
	print "\tjmp alltraps\n";
}


print "global vectors\n";
print "vectors:\n";
for(my $i = 0;$i<256;$i++){
	print "\tdd vector$i\n";
}
