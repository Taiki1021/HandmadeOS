print "extern alltraps\n";
print "section .text\n";

for(my $i = 0; $i < 256; $i++){
	print "global vector$i\n";
	print "vector$i:\n";
	if(!($i==8 ||($i >= 10 && $i <= 14) || $i == 17)){
		print "\tpush 0\n";
	}
	print "\tpush $i\n";
	print "\tjmp alltraps\n";
}


print "section .data\n";
print "global vectors\n";
print "vectors:\n";
for(my $i = 0;$i<256;$i++){
	print "\tdd vector$i\n";
}
