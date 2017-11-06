#!/bin/bash

gcc rgen.c rgen.h util.c util.h base.c base.h
mv a.out rgen
