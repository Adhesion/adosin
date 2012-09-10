/*
 * shaper.h
 *
 * Declarations for shaping functions.
 * (Implemented c-style as an experiment - could've easily been a class w/
 * polymorphism etc.)
 *
 * @author Andrew Ford
 *
 * Copyright (c) 2012 Andrew Ford
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __SHAPER_H
#define __SHAPER_H

enum
{
	sine = 0,
	chebyshev,
	expoOver,
	expoUnder,
	softClip,
	numShapers
};

typedef float ( *shaperFunc )( float, float );
typedef float ( *scaleFunc )( float, float, float );

/*
 * Data structure for a shaper, including minimum/maximum allowed values for
 * input to the shaper function & scaling functions to go from [0.0-1.0] to
 * [min-max] (see below)
 */
typedef struct
{
	char* name;
	shaperFunc shape;
	scaleFunc scale;
	scaleFunc descale;
	float min;
	float max;
} shaper;

/* 
 * Take an array of shapers (already in memory) and set default values and
 * functions.
 */
void init( shaper* shapers );

/*
 * Scale takes input of [0.0-1.0] and scales to [min-max], descale does the
 * reverse. Inverse scaling (ie min > max) is supported.
 * Originally written as helper functions for interfacing with VST.
 */
float linearScale( float in, float min, float max );
float linearDescale( float in, float min, float max );

/*
 * Similar to the above functions, but exponential (base 2 for now)
 * Not to be confused with expoShape below which is a DSP function.
 */
float expoScale( float in, float min, float max );
float expoDescale( float in, float min, float max );

/*
 * Note we don't need a floorDescale (makes no sense since the output has to
 * be 0.0-1.0), we can just use linearDescale instead.
 */
float floorScale( float in, float min, float max );

/*
 * Audio shaping functions.
 */
float expoShape( float in, float amount );
float softClipShape( float in, float amount );
float sineShape( float in, float amount );
float chebyshevShape( float in, float amount );
float chebyshevRec( float in, int depth );

#endif
