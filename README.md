# soundmangle
Quick C program to mangle a WAV file

Takes a mono WAV file, PCM encoded, 16 bit signed sample at 16000 samples per second and outputs a delayed version, where the delay varies according to a sine function

# Build with:

`gcc mangle.c -o mangle -lm`

# Run with:

`./mangle test.wav out.wav 100`

includes two sample WAV files.  Use something like Goldwave to listen to them, or record others.

