all:	seq_read	seq_write	rnd_read	rnd_write

seq_read:	seq_read_test.c
	gcc seq_read_test.c -o seq_read

seq_write:	seq_write_test.c
	gcc seq_write_test.c -o seq_write

rnd_read:	rnd_read_test.c
	gcc rnd_read_test.c -o rnd_read

rnd_write:	rnd_write_test.c
	gcc rnd_write_test.c -o rnd_write

clean:
	rm -f seq_read seq_write rnd_read rnd_write
