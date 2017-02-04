# make sure this is the first task we have, so it becomes the default task
.PHONY : default
default: | clean build

# clang is our compiler of choice.
CC?=clang
# output folder
OUTROOT:=tmp
# sysroot for output
SYSROOT_OUT:=$(OUTROOT)/sysroot
# Default compiler:
CC?=clang
# Default CFLAGS:
CFLAGS?=-O2 -g
# Warnings we want to have
WARNINGS?= -Wall -Wextra -Wno-missing-field-initializers
# initial include paths container
INCLUDE?=
# initial copy folders directive
RECURSIVE_COPY2SYSROOT?=
# Architecture
ARCH?=x64
# stuff to define
DEFINE?= DEBUG=1
