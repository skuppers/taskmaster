#!/bin/bash

clang -o sigsegv catch_segv.c
clang -o sigint catch_int.c
clang -o sigterm catch_term.c
clang -o sighup catch_hup.c
