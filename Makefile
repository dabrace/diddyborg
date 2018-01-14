CC=g++ -Wall

all: ultraborg.o ultraborg_test

ultraborg.o: ultraborg.h ultraborg.cc
	$(CC) ultraborg.cc -c -o ultraborg.o

ultraborg_test: ultraborg.o ultraborg_test.cc
	$(CC) ultraborg_test.cc ultraborg.o -o ultraborg_test

