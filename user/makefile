
a.out: user.o user.ld
	ld -T user.ld -o a.out user.o --nmagic

user.o:	user.c
	gcc -m32 user.c -c

clean:
	rm -f user.o a.out
