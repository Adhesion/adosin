adosin
======

VST audio effect for sine waveshaping &amp; other awesome kinds of distortion.

Usage
=====

Put the .dll in your host's VST directory - should show up as a proper audio effect in any host that supports VST 2.4.

Note that when switching methods, the shaping amount will reset to the minimum (the actual numerical value of this minimum is dependent on the selected shaping method). This is to prevent blasts of noise e.g. when switching from one method set to a high value to the exponentiation < 1 method (which can be very loud as noted below). However, the GUI may not update properly when this happens in certain hosts, notably Ableton Live (tested on version 8.2.7). Probably a good idea to subsequently click to your desired value (instead of click-and-drag) if this happens.

Methods
-------

Sine Wave:

A frequency modulation-esque effect.

Chebyshev:

A transfer function that adds higher harmonics based on the input amplitude. Note that even values for amount will exhibit DC offset, but this can be removed with a high-pass cut filter at ~20Hz.

Exponentiation:

A simple x^y method - < 1 pushes the waveform closer to a square wave, > 1 does the opposite. When using exponentiation < 1, BE CAREFUL! Very low values for "amount" (i.e., when the slider is all the way to the right) become VERY LOUD. That's what the gain controls are for :>

Also, when using > 1, be careful of input beyond 1/-1, as obviously it will exponentially clip even more.

Soft Clip:

This method will keep the input between -1 and 1 without any hard clipping.

Misc
====

VST is a trademark of Steinberg Media Technologies GmbH.
