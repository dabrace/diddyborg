CC=gcc -Wall

all: ultraborg.o ultraborg_test

ultraborg.o: ultraborg.h ultraborg.c
	$(CC) ultraborg.c -c -o ultraborg.o

ultraborg_test: ultraborg.o ultraborg_test.c
	$(CC) ultraborg_test.c ultraborg.o -o ultraborg_test

