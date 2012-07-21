/*
 * adosinVST.cpp
 *
 * Sine wave distortion (& more) VST plugin.
 *
 * @author Adhesion
 */

#include "adosinVST.h"
#include <math.h>
#include <stdio.h>

adosinVST::adosinVST( audioMasterCallback master )
	: AudioEffectX( master, numAPrograms, numAParams )
{
	strcpy( name, "adosin" );
	strcpy( vendor, "Adhesion" );

	version = 1000;

	parameters[ amount ] = 1.0f;
	parameters[ pregain ] = 1.0f;
	parameters[ postgain ] = 1.0f;
	parameters[ dryWet ] = 1.0f;

	programs = new adosinProgram[ numAPrograms ];
	for( int i = 0; i < numAPrograms; i++ )
	{
		programs[ i ].parameters[ amount ] = parameters[ amount ];
		programs[ i ].parameters[ pregain ] = parameters[ pregain ];
		programs[ i ].parameters[ postgain ] = parameters[ postgain ];
		programs[ i ].parameters[ dryWet ] = parameters[ dryWet ];
		strcpy( programs[ i ].name, "Init" );
	}

	numChannels = 2;

	setNumInputs( numChannels );
	setNumOutputs( numChannels );
	canProcessReplacing( true );
	//canDoubleReplacing( true ); TODO implement this!!

	setUniqueID( 'adsn' );
}

adosinVST::~adosinVST()
{
	delete[] programs;
}

void adosinVST::processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames )
{
	for( VstInt32 i = 0; i < sampleFrames; i++ )
	{
		for( int j = 0; j < numChannels; j++ )
		{
			outputs[ j ][ i ] = shape( inputs[ j ][ i ], parameters[ amount ],
				parameters[ pregain ], parameters[ postgain ],
				parameters[ dryWet ] );
		}
	}
}

float adosinVST::shape( float in, float amount, float pregain, float postgain, float dryWet )
{
	if ( in == 0.0f )
		return in;

	bool flip = ( in < 0.0f );

	in *= pregain;
	float out = pow( in * ( flip ? -1.0f : 1.0f ), amount ) * ( flip ? -1.0f : 1.0f );

	return ( ( in * ( 1.0f - dryWet ) ) + ( out * dryWet ) ) * postgain;
}

void adosinVST::setParameter( VstInt32 index, float value )
{
	if ( index == amount )
	{
		// if amount, scale to 0.0-5.0
		value *= 5.0f;
	}

	parameters[ index ] = value;
	programs[ curProgram ].parameters[ index ] = value;
}

float adosinVST::getParameter( VstInt32 index )
{
	float ret = parameters[ index ];
	if ( index == amount )
	{
		// output of this has to be 0.0-1.0 so un-scale
		ret /= 5.0f;
	}
	return ret;
}

void adosinVST::getParameterName( VstInt32 index, char* text )
{
	switch( index )
	{
	case amount:
		strcpy( text, "Amount" );
		break;
	case pregain:
		strcpy( text, "Pregain" );
		break;
	case postgain:
		strcpy( text, "Postgain" );
		break;
	case dryWet:
		strcpy( text, "Dry/Wet" );
		break;
	default:
		strcpy( text, "" );
		break;
	}
}

void adosinVST::getParameterDisplay( VstInt32 index, char* text )
{
	switch( index )
	{
	case amount:
	case dryWet:
		float2string( parameters[ index ], text, kVstMaxParamStrLen );
		break;
	case pregain:
	case postgain:
		dB2string( parameters[ index ], text, kVstMaxParamStrLen );
		break;
	default:
		strcpy( text, "" );
	}
}

void adosinVST::getParameterLabel( VstInt32 index, char* text )
{
	switch( index )
	{
	case pregain:
	case postgain:
		strcpy( text, "dB" );
		break;
	default:
		strcpy( text, "" );
	}
}

void adosinVST::setProgramName( char* name )
{
	strcpy( programs[ curProgram ].name, name );
}

void adosinVST::getProgramName( char* name )
{
	strcpy( name, programs[ curProgram ].name );
}

bool adosinVST::getEffectName( char* eName )
{
	strcpy( eName, name );
	return true;
}

bool adosinVST::getProductString( char* pString )
{
	strcpy( pString, name );
	return true;
}

bool adosinVST::getVendorString( char* vString )
{
	strcpy( vString, vendor );
	return true;
}

VstInt32 adosinVST::getVendorVersion()
{
	return version;
}
