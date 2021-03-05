<html>
<head>
<title>6.837 Assignment 0</title>
</head>

<body background="../../images/blbkgnd.gif" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/images/blbkgnd.gif" bgcolor="#FFFFFF" text="#000000">
<h1>6.837 Intro to Computer Graphics<br>
Assignment 0: Iterated Function Systems</h1>

<p>
The goal of this assignment is to get familiar with C++ and with two
simple libraries that we will use for linear algebra and images. The
incidental goal is also to have fun with bizarre fractal objects.  IFS
are self-similar fractals: a subpart of the object is similar to the
whole. The classic example of an IFS is Barnsley's fern, where each
subpart of the fern is exactly the same as the whole fern.  IFS are
described by a set of affine transformations (rotations, translations,
scale, skew, etc.)  These transformations capture the self-similarity
of the object.  IFS can be defined in any dimension, but we will play
with two-dimensional ones.  Formally, an IFS is defined by <em>n</em>
affine transformations. Each transformation <em>f<sub>i</sub></em>
must be contractive: The distance between points must be reduced. An
<em>attractor</em> of the IFS is the object such that <em>A = U
f<sub>i</sub> (A)</em>.  <em>A</em> is unchanged by the set of
transformations: It is a fixed point.

<p>
<img src="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/fern_illustration_small.jpg">

<p>
We render an IFS by iterating the transform on random input points
from the unit square. We approximate the fixed point by applying the
transformation many times.  The algorithm is as follows:

<pre>
   for "lots" of random points (x<sub>0</sub>, y<sub>0</sub>)
       for k=0 to num_iters<sub>&nbsp;</sub>
           pick a random transform f<sub>i</sub>
           (x<sub>k+1</sub>, y<sub>k+1</sub>) = f<sub>i</sub>(x<sub>k</sub>, y<sub>k</sub>)
       display a dot at (x<sub>k</sub>, y<sub>k</sub>)
</pre>

<p>
To reduce the number of points necessary to make an image of
reasonable quality, probabilities are assigned to each transformation,
instead of choosing a transformation with uniform probability.

<h3>Tasks</h3>

<ul>
<li> Write a C++ class <tt>IFS</tt> that renders iterated function
  systems, including the interface (in a file <tt>ifs.h</tt>) and the
  implementation (<tt>ifs.C</tt>).  Your code should run under Linux
  or Windows.  The IFS class should include:

<ul>
<li> a field to store <em>n</em>, the number of transformations
<li> an array of matrices representing the <em>n</em> transformations
<li> an array of the corresponding probabilities for choosing a
transformation
<li> a constructor that creates an IFS
<li> an input method that reads the IFS description
<li> a render method that takes as input an image instance, a number
  of points and a number of iterations
<li> a destructor that frees the memory of the various arrays (using
  <tt>delete</tt>)
</ul>

<li> Write the main program <tt>main.C</tt> that creates an <tt>Image</tt>
  instance, reads an IFS description from a file, renders the IFS to
  the image, and saves the image.
<li> Use the linear algebra library for the point and transformation
  representations. 
<li> Perform proper memory management --- free memory when an object
  is destroyed.
<li> Extra credit: create a new IFS, figure out the probabilities,
  determine the bounding box, change the color scheme, anti-aliasing,
  depth-first vs. breadth-first, etc.  Include a short paragraph in
  your <tt>README.txt</tt> file describing your extensions.
</ul>

<h3>Hints</h3>

<ul>

<li> Random numbers can be obtained using the <tt>drand48()</tt> or
<tt>rand()</tt>, and <tt>RAND_MAX</tt>.  See
<tt>stdlib.h</tt>.

<li> To debug your code, set the number of iterations to one. This
will allow you to check that you got the transformations right.

<li> Be careful, arrays are indexed from 0 to <em>n-1</em> in
C++.  Reading beyond the bounds of the array will probably result in a
segmentation fault.

<li> Use <tt>assert()</tt> to check function pre-conditions, array
indices, etc.  See <tt>assert.h</tt>.

</ul>

<h3>Additional references</h3>

<ul>
<li>M.Barnsley, Fractals Everywhere, Academic Press, 1988.

<li>
<a href="javascript:if(confirm('http://spanky.triumf.ca/www/fractal-info/ifs-type.htm  \n\n���ļ��޷��� Teleport Ultra ����, ��Ϊ ����һ�����·���ⲿ������Ϊ������ʼ��ַ�ĵ�ַ��  \n\n�����ڷ������ϴ���?'))window.location='http://spanky.triumf.ca/www/fractal-info/ifs-type.htm'" tppabs="http://spanky.triumf.ca/www/fractal-info/ifs-type.htm">
http://spanky.triumf.ca/www/fractal-info/ifs-type.htm</a>

<li>
<a href="javascript:if(confirm('http://www.cut-the-knot.org/ctk/ifs.shtml  \n\n���ļ��޷��� Teleport Ultra ����, ��Ϊ ����һ�����·���ⲿ������Ϊ������ʼ��ַ�ĵ�ַ��  \n\n�����ڷ������ϴ���?'))window.location='http://www.cut-the-knot.org/ctk/ifs.shtml'" tppabs="http://www.cut-the-knot.org/ctk/ifs.shtml">
http://www.cut-the-knot.org/ctk/ifs.shtml</a>
</ul>

<h3>Image Library</h3>

The <tt>Image</tt> class is used to initialize and edit the rgb values
of images.  Be careful --- do not try to edit values outside the
bounds of the image.  The class also includes functions for loading
and saving simple <tt>.tga</tt> image files.  <tt>.tga</tt> files can
be viewed with <tt>xv</tt> or opened in Photoshop and other Windows
image viewers/editors.

<ul>
<li><a href="image.h" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/image.h">image.h</a>
<li><a href="image.C" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/image.C">image.C</a>
</ul>

<h3>Linear Algebra Library</h3>

Linear algebra support for floating point vectors with 2, 3, and 4
elements (<tt>Vec2f</tt>, <tt>Vec3f</tt>, and <tt>Vec4f</tt>) and 4x4
floating point matrices (<tt>Matrix</tt>).  For this assignment, the
<tt>void Matrix::Transform(Vec2f &v)</tt> function will be handy.

<ul>
<li><a href="javascript:if(confirm('http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/vectors.h  \n\n���ļ��޷��� Teleport Ultra ����, ��Ϊ ������, �����������, ����Ŀ����ֹͣ��  \n\n�����ڷ������ϴ���?'))window.location='http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/vectors.h'" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/vectors.h">vectors.h</a>
<li><a href="matrix.h" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/matrix.h">matrix.h</a>
<li><a href="matrix.C" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/matrix.C">matrix.C</a>
</ul>

<h3>Sample code for parsing command-line arguments and input files</h3>

Your program should take a number of command line arguments to specify
the input file, number of points, number of iterations, output image
size and output file.  Make sure the examples below work, as this is
how we will test your program.  Sample code to parse input files and
command line arguments is provided:

<ul>
<li><a href="parse_code.txt" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/parse_code.txt">parse_code.txt</a>
</ul>

<h3>Data files</h3>

The input data for an IFS is a file which contains <em>n</em>,
the number of transforms, followed by the probability of choosing each
transform and a 3x3 floating point matrix representation of the
transform.

<ul>
<li><a href="fern.txt" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/fern.txt">fern.txt</a>
<li><a href="dragon.txt" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/dragon.txt">dragon.txt</a>
<li><a href="sierpinski_triangle.txt" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle.txt">sierpinski_triangle.txt</a>
<li><a href="giant_x.txt" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/giant_x.txt">giant_x.txt</a>
</ul>

<h3>Makefile for g++ on LINUX</h3>
<ul>
<li><a href="Makefile.htm" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/Makefile">Makefile</a>
</ul>

<h3>Sample Results</h3>
<pre>
ifs -input sierpinski_triangle.txt -points 10000 -iters 0 -size 200 -output sierpinski_triangle_0.tga
ifs -input sierpinski_triangle.txt -points 10000 -iters 1 -size 200 -output sierpinski_triangle_1.tga
ifs -input sierpinski_triangle.txt -points 10000 -iters 2 -size 200 -output sierpinski_triangle_2.tga
ifs -input sierpinski_triangle.txt -points 10000 -iters 3 -size 200 -output sierpinski_triangle_3.tga
ifs -input sierpinski_triangle.txt -points 10000 -iters 4 -size 200 -output sierpinski_triangle_4.tga
ifs -input sierpinski_triangle.txt -points 10000 -iters 30 -size 200 -output sierpinski_triangle.tga
</pre>
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle_0.png">
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle_1.png">
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle_2.png"><br>
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle_3.png">
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle_4.png">
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/sierpinski_triangle.png">

<pre>
ifs -input fern.txt -points 50000 -iters 30 -size 400 -output fern.tga
</pre>
<image src = "http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/assignment0/fern.png">

<p>

See the main <a href="../index.html" tppabs="http://groups.csail.mit.edu/graphics/classes/6.837/F04/assignments/index.html">Assignments Page</a> for submission information.
<hr>
</body>
</html>