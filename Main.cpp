/*****************************************************************************************[Main.cc]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007,      Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <errno.h>
//#include <zlib.h>
#include <fstream>

//#include "minisat22_wrapper.h"

#include "minisat/utils/System.h"
#include "minisat/utils/ParseUtils.h"
#include "minisat/utils/Options.h"
#include "minisat/core/Dimacs.h"
#include "MOLS/MOLS.h"
using namespace Minisat;

int main(int argc, char** argv)
{
	
	localsearch_minisat a;
	//a.search(10, 3, 50, 90, "D:\\LSTests\\Local_search\\ls10_3");
	string path = ".";
	if ( argc == 1 )
		cout << "Usage: [core_len] [max_solving_time]" << endl;
	if ( argc > 1 )
		a.core_len = atoi( argv[1] );
	else
		a.core_len = 128; // default;
	if ( argc > 2 )
		a.max_solving_time = atof( argv[2] );
	cout << "init core_len "         << a.core_len         << endl;
	cout << "init max_solving_time " << a.max_solving_time << endl;
	a.search(10, 3, 50, 90, path );
}