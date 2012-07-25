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

	shapers[ sine ].name = "Sine Wave";
	shapers[ sine ].shape = &sineShape;
	shapers[ sine ].scale = &linearScale;
	shapers[ sine ].descale = &linearDescale;
	shapers[ sine ].min = 1.0f;
	shapers[ sine ].max = 10.0f;

	shapers[ chebyshev ].name = "Chebyshev";
	shapers[ chebyshev ].shape = &chebyshevShape;
	shapers[ chebyshev ].scale = &floorScale;
	shapers[ chebyshev ].descale = &linearDescale;
	shapers[ chebyshev ].min = 1.0f;
	shapers[ chebyshev ].max = 10.0f;
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
		ret = max + ( in * ( min - max ) );
		ret = 1.0f - ret;
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
		in = 1.0f - in;
		ret = ( in - max ) / ( min - max );
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

float expoShape( float in, float amount, float pregain, float postgain, float dryWet )
{
	if ( in == 0.0f )
		return in;

	float flip = in < 0.0f ? -1.0f : 1.0f;

	in *= pregain;
	float out = pow( in * flip, amount ) * flip;

	return ( ( in * ( 1.0f - dryWet ) ) + ( out * dryWet ) ) * postgain;
}

float softClipShape( float in, float amount, float pregain, float postgain, float dryWet )
{
	return 0.0f;
}

float sineShape( float in, float amount, float pregain, float postgain, float dryWet )
{
	return 0.0f;
}

float chebyshevShape( float in, float amount, float pregain, float postgain, float dryWet )
{
	return 0.0f;
}

float chebyshevRec( float in, int depth )
{
	return 0.0f;
}
