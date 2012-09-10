/*
 * adosinVST.h
 *
 * Sine wave distortion (& more) VST plugin.
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

	// note parameters are stored as their actual values, not normalized
	// 0.0-1.0 for VST (done on input/output)
	float parameters[ numAParams ];
	int numChannels;

	shaper shapers[ numShapers ];

	// float for input to scalers - numShapers minus a tiny bit to make sure
	// floor output is (integer) [0, numShapers)
	float numShapersF;

	adosinProgram* programs;
};

#endif
