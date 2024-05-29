## Overview:
    Work in progress
    lagrange partially ready (ready but not tested)
    newton partially ready (ready but not tested)
    quad, qube, linear splines ready for segments

## Status:
    Pre-Pre-release

## Contains:
    src/methods/gauss.c             -- solving SLAE
    src/methods/polynoms.c          -- some polynoms operations like * + 
	src/methods/lagrange.c          -- lagrange poly interpolation
    src/methods/newton.c            -- newton poly interpolation
	src/methods/linear_spline.c     -- linear spline interpolation (1 1)
    src/methods/quad_spline.c       -- spline 2 1
    src/methods/qube_spline.c       -- spline 3 2
    src/methods/penalty_spline.c    -- simplest spline

    src/types/matrix.c              -- matrix methods
	src/types/vector.c              -- vector methods
    src/types/pair.h                -- simple pair with doubles
 	
    src/draw/draw.c                 -- using curl to send funcs to the server
	src/draw/drawer.js              -- JS POST/GET server to draw with desmos

	src/funcs/funcs.h               -- functions from task
    src/common.h                    -- macros and defs
    src/test/*                      -- test dataset
	src/main.c                      -- base logic

## Dependencies:
    jemalloc >= 5.3
    clang    >= 17
	libcurl  >= 8.7.1
	node     >= 21.7.3
	express  >= 4.19.2
	linux/xdg
	curl dependencies
    express dependencies
    libc math

## Build:
    make
    make clean
