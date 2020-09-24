epeg
====

An IMMENSELY FAST JPEG thumbnailer library API.

Why write this? It's a convenience library API to using libjpeg to load JPEG
images destined to be turned into thumbnails of the original, saving
information with these thumbnails, retreiving it and managing to load the image
ready for scaling with the minimum of fuss and CPU overhead.

This means it's insanely fast at loading large JPEG images and scaling them
down to tiny thumbnails. It's speedup will be proportional to the size
difference between the source image and the output thumbnail size as a
count of their pixels.

It makes use of libjpeg features of being able to load an image by only
decoding the DCT coefficients needed to reconstruct an image of the size
desired. This gives a massive speedup. If you do not try and access the pixels
in a format other than YUV (or GRAY8 if the source is grascale) then it also
avoids colorspace conversions as well.

Original Version
----------------

The epeg library was developed within the [Enlightenment](http://www.enlightenment.org) project. 
As of some unknown version the epeg library disappeared from the project or was merged into some other library.
The last version of epeg I know of is epeg [v0.9.1.042](https://github.com/mattes/epeg/archive/v0.9.1.042.zip).

**Please note that the latest master branch of this repository contains code updates and improvements.**

Bindings
--------
 * Python, https://github.com/jbaiter/epeg-cffi, https://pypi.python.org/pypi/epeg-cffi
 * Node.js, https://www.npmjs.org/package/epeg

Blog posts
----------

 * [blazing fast epeg photo resize on raspberry pi](https://web.archive.org/web/20160310234752/http://blog.sunekaae.com/2013/04/blazing-fast-epeg-photo-resize-on_3.html) by Sune Kaae



License
-------
Copyright (C) 2000 Carsten Haitzler and various contributors (see AUTHORS)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its Copyright notices. In addition publicly
documented acknowledgment must be given that this software has been used if no
source code of this software is made available publicly. This includes
acknowledgments in either Copyright notices, Manuals, Publicity and Marketing
documents or any documentation provided with any product containing this
software. This License does not apply to any software that links to the
libraries provided by this software (statically or dynamically), but only to
the software provided.

Please see the COPYING.PLAIN for a plain-english explanation of this notice
and it's intent.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
