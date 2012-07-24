adosin
======

VST audio effect for sine waveshaping &amp; other awesome kinds of distortion.

Requirements
============

- Visual Studio 2010
- VST 2.4 SDK

Make sure to change the VC++ include directory (in project properties) to the top-level directory for the VST SDK on your machine. Build result will be a standalone .dll in the build directory (probably Debug/)

Usage
=====

Put the .dll in your host's VST directory - should show up as a proper audio effect in any host that supports VST 2.4.
When using exponentiation < 1, BE CAREFUL! Very low values for "amount" become VERY LOUD. That's what the gain controls are for :>

Misc
====

VST PlugIn Technology by Steinberg Media Technologies GmbH.
