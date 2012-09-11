/*
 * shaper.cpp
 *
 * Definitions for shaping functions.
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

#include <math.h>
#include "shaper.h"

void init( shaper* shapers )
{
	shapers[ sine ].name = "Sine Wave";
	shapers[ sine ].shape = &sineShape;
	shapers[ sine ].scale = &linearScale;
	shapers[ sine ].descale = &linearDescale;
	shapers[ sine ].min = 1.0f;
	shapers[ sine ].max = 100.0f;

	shapers[ chebyshev ].name = "Chebyshev";
	shapers[ chebyshev ].shape = &chebyshevShape;
	shapers[ chebyshev ].scale = &floorScale;
	shapers[ chebyshev ].descale = &linearDescale;
	shapers[ chebyshev ].min = 1.0f;
	shapers[ chebyshev ].max = 10.0f;

	shapers[ expoOver ].name = "Exponent >1";
	shapers[ expoOver ].shape = &expoShape;
	shapers[ expoOver ].scale = &linearScale;
	shapers[ expoOver ].descale = &linearDescale;
	shapers[ expoOver ].min = 1.0f;
	shapers[ expoOver ].max = 5.0f;

	shapers[ expoUnder ].name = "Exponent <1";
	shapers[ expoUnder ].shape = &expoShape;
	shapers[ expoUnder ].scale = &linearScale;
	shapers[ expoUnder ].descale = &linearDescale;
	shapers[ expoUnder ].min = 1.0f;
	shapers[ expoUnder ].max = 0.1f;

	shapers[ softClip ].name = "Soft Clip";
	shapers[ softClip ].shape = &softClipShape;
	shapers[ softClip ].scale = &linearScale;
	shapers[ softClip ].descale = &linearDescale;
	shapers[ softClip ].min = 0.0f;
	shapers[ softClip ].max = 0.0f;
}

float linearScale( float in, float min, float max )
{
	float ret;
	if ( min == 0.0f && max == 0.0f )
	{
		ret = 0.0f;
	}
	else if ( min > max )
	{
		ret = min - ( in * ( min - max ) );
	}
	else
	{
		ret = min + ( in * ( max - min ) );
	}
	return ret;
}

float linearDescale( float in, float min, float max )
{
	float ret;
	if ( min == 0.0f && max == 0.0f )
	{
		ret = 0.0f;
	}
	else if ( min > max )
	{
		ret = ( min - in ) / ( min - max );
	}
	else
	{
		ret = ( in - min ) / ( max - min );
	}
	return ret;
}

float expoScale( float in, float min, float max )
{
	// negative log makes no sense...
	if ( min < 0.0f || max < 0.0f )
	{
		return 0.0f;
	}

	// not handling min > max (inverse) case yet

	// figure out how many "octaves" (doublings) it takes to get from min to
	// max
	// we only have log & log10 so we have to do change of base
	// note this uses + instead of * so we can handle min == 0
	float octaves = log( max - min + 1 ) / log( 2.0f );
	return ( min - 1 ) + pow( 2.0f, in * octaves );
}

float expoDescale( float in, float min, float max )
{
	// see above
	if ( min < 0.0f || max < 0.0f )
	{
		return 0.0f;
	}

	// again, not handling min > max (inverse) case yet
	
	// note this was derived by simply inverting the previous function
	float log2 = log( 2.0f );
	return ( log( in - min + 1 ) / log2 ) / ( log( max - min + 1 ) / log2 );
}

float floorScale( float in, float min, float max )
{
	if ( min > max )
	{
		return ceil( linearScale( in, min, max ) );
	}
	else
	{
		return floor( linearScale( in, min, max ) );
	}
}

float expoShape( float in, float amount )
{
	if ( in == 0.0f )
		return in;

	float flip = in < 0.0f ? -1.0f : 1.0f;

	return pow( in * flip, amount ) * flip;
}

float softClipShape( float in, float amount )
{
	return in / ( 1 + fabs( in ) );
}

float sineShape( float in, float amount )
{
	return in * cos( in * amount );
}

float chebyshevShape( float in, float amount )
{
	return chebyshevRec( in, (int)amount );
}

float chebyshevRec( float in, int depth )
{
	if ( depth == 0 )
	{
		return 1.0f;
	}

	// lastval represents C(k-1)
	float lastVal = 1.0f;
	float out = in;
	float temp;

	// depth=1 is the base case
	for( int i = 1; i < depth; i++ )
	{
		temp = out;
		out = ( 2.0f * in * out ) - lastVal;
		lastVal = temp;
	}
	return out;
}
