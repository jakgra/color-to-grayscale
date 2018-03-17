#!/bin/sh

cc -Itoolbox -Ibstrlib -o color_to_grayscale color_to_grayscale.c bstrlib/bstrlib.c -lm
