adosin
======

VST audio effect for sine waveshaping &amp; other awesome kinds of distortion.

Build Requirements
==================

- Visual Studio 2010
- VST 2.4 SDK

Make sure to change the VC++ include directory (in project properties) to the top-level directory for the VST SDK on your machine. Build result will be a standalone .dll in the build directory (probably Debug/)

Usage
=====

Put the .dll in your host's VST directory - should show up as a proper audio effect in any host that supports VST 2.4.

When using exponentiation < 1, BE CAREFUL! Very low values for "amount" (i.e., when the slider is all the way to the right) become VERY LOUD. That's what the gain controls are for :>

Note that when switching methods, the shaping amount will reset to the minimum (the actual numerical value of this minimum is dependent on the selected shaping method). This is to prevent blasts of noise e.g. when switching from one method set to a high value to the exponentiation < 1 method (which can be very loud as noted above). However, the GUI may not update properly when this happens in certain hosts, notably Ableton Live (tested on version 8.2.7). Probably a good idea to subsequently click to your desired value (instead of click-and-drag) if this happens.

Misc
====

VST PlugIn Technology by Steinberg Media Technologies GmbH.
