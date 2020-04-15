all: attack 
	gcc set-file-setuid.c -o set-file-setuid `pkg-config ext2fs --libs --cflags` \
	`pkg-config com_err --libs --cflags`

attack: attack.c
	gcc attack.c -o attack

clean:
	rm -rf set-file-setuid attack main
