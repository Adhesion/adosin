/*
 * adosinVST.h
 *
 * Sine wave distortion (& more) VST plugin.
 *
 * @author Adhesion
 */

#ifndef __ADOSIN_H
#define __ADOSIN_H

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	amount = 0,
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

class adosin : public AudioEffectX
{
public:
	adosin( audioMasterCallback master );
	~adosin();

	void processReplacing( float** inputs, float** outputs, VstInt32 sampleFrames );
	float shape( float in, float amount, float pregain, float postgain, float dryWet );
	
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

	adosinProgram* programs;
};

#endif