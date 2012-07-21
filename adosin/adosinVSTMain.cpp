/*
 * adosinVSTMain.cpp
 *
 * Main file for adosin VST plugin.
 *
 * @author Adhesion
 */

#include "adosinVST.h"

AudioEffect* createEffectInstance( audioMasterCallback master )
{
	return new adosinVST( master );
}
