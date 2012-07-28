/*
 * shaper.cpp
 *
 * Definitions for shaping functions.
 * (Implemented c-style as an experiment - could've easily been a class w/
 * polymorphism etc.)
 *
 * @author Adhesion
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
	else if ( depth == 1 )
	{
		return in;
	}
	return ( 2.0f * in * chebyshevRec( in, depth - 1 ) ) - chebyshevRec( in, depth - 2 );
}
