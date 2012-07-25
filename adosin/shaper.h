/*
 * shaper.h
 *
 * Declarations for shaping functions.
 * (Implemented c-style as an experiment - could've easily been a class w/
 * polymorphism etc.)
 *
 * @author Adhesion
 */

#ifndef __SHAPER_H
#define __SHAPER_H

enum
{
	expoOver = 0,
	expoUnder,
	softClip,
	sine,
	chebyshev,
	numShapers
};

typedef float ( *shaperFunc )( float, float, float, float, float );
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
 * Note we don't need a floorDescale (makes no sense since the output has to
 * be 0.0-1.0), we can just use linearDescale instead.
 */
float floorScale( float in, float min, float max );

float expoShape( float in, float amount, float pregain, float postgain, float dryWet );
float softClipShape( float in, float amount, float pregain, float postgain, float dryWet );
float sineShape( float in, float amount, float pregain, float postgain, float dryWet );
float chebyshevShape( float in, float amount, float pregain, float postgain, float dryWet );
float chebyshevRec( float in, int depth );

#endif
