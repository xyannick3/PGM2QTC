## Project by xyannick3 and CarapaceRose


# PGM file compressor (with and without loss)

This project is the midyear C programming assignement in the university of Gustave Eiffel during the year 2024/2025. The goal was to realise a PGM (portable grey map) file compressor using the quadtree structure. The software was to be able to generate `.qtc` files from `.pgm` files (the compression algorithm requires square images of $2^n$ of width to function) as well as turning a `.qtc` file back to `.pgm`. 

The project's aim was to demonstrate our ability to use various aspects of C programming, such as modularisation, bit by bit operators, shared libraries, complex data structures and input output management. 


1. **How to compile the project :**

To compile the project, enter the main directory and execute the makefile (`make` in the terminal), this will compile the shared library as well as two executables, The first is `PGM2QTC`, the second `PGM2QTCalt`, they both serve the same purpose but one's compiled directly using the module the other using the shared library file `libPGMcompressor.so`.

2. **How to use:** 

You may find the list of arguments by executing `./bin/PGM2QTC -h`.

First of all we need to define what's a compliant file, a compliant file is any PGM file of square shape which's width is equal to $2^n$ with $n\in\mathbb{N}$. if you have a PGM file that's non compliant you'll have to use the padding module of our program. To do so `./bin/PGM2QTC -p -i file.pgm`. This will add padding to the file to make it compliant. If you do not precise an output location (`-o` )it will by default go to `PGM/out.pgm`.

**Encoding mode (no loss):**

The encoding process takes a compliant `.pgm` file in input and compresses it to a `.qtc` file, without loss. By default the created file will go to `QTC/out.qtc`, as with the padding you may change the output location using `-o`. 

Example : `./bin/PGM2QTC -c -i input.pgm` 

**Encoding mode (with loss):**

The encoding process with loss is made to function with more detailed `.pgm` files, such as pictures or non computer generated images. Indeed the quadtree compression algorithm only compresses if all the pixels of a region are identical, that's pretty rare for a picture. So this encoding process makes the quadtree go through a filter which will look at the pixels of a region and if they're similar enough they will compress the region. To do so we're using an `alpha` argument (`-a`), the argument is a float that goes from 1.0 (no compression) to 2.0 and above (2.0 in most case will compress it well enough). If you do not use `-a` for a picture it will work nonetheless, the `.qtc` file will just be bigger than the `.pgm`... Which is lame.

Example : `./bin/PGM2QTC -c -i input.pgm -a 1.8`  


**Decoding mode :**

The decoding process takes in argument a `.qtc` file, and decodes it into a `.pgm` file. By default the created file will be `PGM/out.pgm`, as usual use `-o` if you want something else. 


Exemple : `./bin/PGM2QTC -u -i input.qtc`

**Generating the segmentation grid**

The generation of the segmentation grid is made into a secondary file `PGM/grid.pgm` representing the grid. It is useful if you want to see how the filter functions. It's also pretty cool :).

Exemple : `./bin/PGM2QTC -u -i input.qtc -g`
Example : `./bin/PGM2QTC -c -i input.pgm -a 1.8 -g`  


**verbose mode:**

This is just something to display in the text output the process.


3. Documentation

The documentation is generated using Doxyfile, to access it open the file `documentation/html/index.html`, it's not much but it's probably more practical to browse than going through the code.


4. **The boring yet necessary part**

## MIT LICENSE



Copyright 2025 xyannick3 & CarapaceRose

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
