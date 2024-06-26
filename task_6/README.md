## Overview:
    First method may be a bit faster than
	optimized version, so probably it's better
	to just use LSM transpose and solve

## Status:
    Released (commiting about issues)

## Contains:
	src/draw/draw.c           -- using curl to send funcs to the server
	src/draw/drawer.js        -- JS POST/GET server to draw with desmos
	src/funcs/func.h          -- function from task
	src/methods/gauss.c       -- just solving SLAE
	src/methods/polynoms.c    -- vector/polynoms functions
	src/methods/normal_poly.c -- first method (LSM transpose and solve SLAE)
    src/methods/orth_poly.c   -- second method (LSE iter formula orth_poly)
    src/types/matrix.c        -- new matrix methods
	src/types/vector.c        -- refactored vector from prev tasks
	src/types/pair.h          -- simple double pair
	src/common.h              -- useful macros
	src/main.c                -- base logic and entry func

## Dependencies:
    jemalloc >= 5.3
    clang    >= 17
	libcurl  >= 8.7.1
	node     >= 21.7.3
	express  >= 4.19.2
	linux/xdg
	curl dependencies
	express dependencies

## Build:
	cd src/draw
	npm install express
    make
    make clean

## Start:
	node src/draw/drawer.js
	./build/main
