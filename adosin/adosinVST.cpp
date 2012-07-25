/*
 * adosinVST.cpp
 *
 * Sine wave distortion (& more) VST plugin.
 *
 * @author Adhesion
 */

#include "adosinVST.h"
#include <stdio.h>

adosinVST::adosinVST( audioMasterCallback master )
	: AudioEffectX( master, numAPrograms, numAParams )
{
	strcpy( name, "adosin" );
	strcpy( vendor, "Adhesion" );

	version = 1001;

	init( shapers );

	parameters[ shaperMethod ] = 0.0f;
	parameters[ amount ] = 1.0f;
	parameters[ pregain ] = 1.0f;
	parameters[ postgain ] = 1.0f;
	parameters[ dryWet ] = 1.0f;

	programs = new adosinProgram[ numAPrograms ];
	for( int i = 0; i < numAPrograms; i++ )
	{
		programs[ i ].parameters[ shaperMethod ] = parameters[ shaperMethod ];
		programs[ i ].parameters[ amount ] = parameters[ amount ];
		programs[ i ].parameters[ pregain ] = parameters[ pregain ];
		programs[ i ].parameters[ postgain ] = parameters[ postgain ];
		programs[ i ].parameters[ dryWet ] = parameters[ dryWet ];
		strcpy( programs[ i ].name, "Init" );
	}

	numChannels = 2;

	// see declaration to explain this nonsense
	numShapersF = numShapers - 0.001f;

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

void adosinVST::processReplacing( float** inputs, float** outputs,
	VstInt32 sampleFrames )
{
	for( VstInt32 i = 0; i < sampleFrames; i++ )
	{
		for( int j = 0; j < numChannels; j++ )
		{
			int sh = (int)parameters[ shaperMethod ];
			outputs[ j ][ i ] = shapers[ sh ].shape( inputs[ j ][ i ],
				parameters[ amount ], parameters[ pregain ], parameters[ postgain ],
				parameters[ dryWet ] );
		}
	}
}

/*
 * Note this is called by VST host and value input will be [0.0-1.0]
 */
void adosinVST::setParameter( VstInt32 index, float value )
{
	int method = (int)parameters[ shaperMethod ];
	shaper s = shapers[ method ];

	if ( index == amount )
	{
		// if amount, scale to proper range with the shaper's scale
		value = s.scale( value, s.min, s.max );
	}
	else if ( index == shaperMethod )
	{
		value = floorScale( value, 0.0f, numShapersF );
	}

	parameters[ index ] = value;
	programs[ curProgram ].parameters[ index ] = value;

	// if method changes, let's reset value back to 0.0 to prevent any scary
	// blasts of noise (ie, going from maximum in one method -> maximum in
	// expo <1)
	if ( index == shaperMethod && (int)value != method )
	{
		// note - have to use the automated variant here in order to update
		// GUI in some hosts - unfortunately doesn't work in ableton :(
		// can do updatedisplay() instead but makes click-and-drag behavior
		// feel wonky
		setParameterAutomated( amount, 0.0f );
	}
}

/*
 * Note output of this has to be [0.0-1.0] for VST interface
 */
float adosinVST::getParameter( VstInt32 index )
{
	int method = (int)parameters[ shaperMethod ];
	shaper s = shapers[ method ];

	float ret = parameters[ index ];
	if ( index == amount )
	{
		// output of this has to be 0.0-1.0 so un-scale
		ret = s.descale( ret, s.min, s.max );
	}
	else if ( index == shaperMethod )
	{
		ret = linearDescale( ret, 0.0f, numShapersF );
	}

	return ret;
}

void adosinVST::getParameterName( VstInt32 index, char* text )
{
	switch( index )
	{
	case shaperMethod:
		strcpy( text, "Method" );
		break;
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
	case shaperMethod:
		strcpy( text, shapers[ (int)parameters[ shaperMethod ] ].name );
		break;
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
