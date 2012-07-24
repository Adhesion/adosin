/*
 * adosinVST.h
 *
 * Sine wave distortion (& more) VST plugin.
 *
 * @author Adhesion
 */

#ifndef __ADOSINVST_H
#define __ADOSINVST_H

#include "shaper.h"

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	shaperMethod = 0,
	amount,
	pregain,
	postgain,
	dryWet,
	numAParams,
	numAPrograms = 16
};

typedef struct
{
	float parameters[ numAParams ];
	char name[ kVstMaxProgNameLen ];
} adosinProgram;

class adosinVST : public AudioEffectX
{
public:
	adosinVST( audioMasterCallback master );
	~adosinVST();

	void processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames );
	
	void setParameter( VstInt32 index, float value );
	float getParameter( VstInt32 index );
	void getParameterLabel( VstInt32 index, char* text );
	void getParameterDisplay( VstInt32 index, char* text );
	void getParameterName( VstInt32 index, char* text );

	void setProgramName( char* name );
	void getProgramName( char* name );

	bool getEffectName( char* eName );
	bool getProductString( char* pString );
	bool getVendorString( char* vString );
	VstInt32 getVendorVersion();

private:
	char name[ kVstMaxEffectNameLen ];
	char vendor[ kVstMaxVendorStrLen ];
	VstInt32 version;

	float parameters[ numAParams ];
	int numChannels;

	shaper shapers[ numShapers ];

	// float for input to scalers - numShapers minus a tiny bit to make sure
	// floor output is (integer) [0, numShapers)
	float numShapersF;

	adosinProgram* programs;
};

#endif