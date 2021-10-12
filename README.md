# Software Security Fuzzing Assignment
## Target: FreeImage 3.8.1

Install FreeImage and AFL:

1. AFL
	- Clone this AFL repository: https://github.com/google/AFL.git
	- Install AFL with `make` and `make install`
2. FreeImage
	- Clone this FreeImage repository: https://github.com/WinMerge/freeimage.git
	- Add `CC = ~/AFL/afl-gcc` and `CXX = ~/AFL/afl-g++` to Makefile.gnu
	- Do `make` and `make install` to install the C++ FreeImage library and the AFL instrumentation.
3. Fuzzing project
	- Clone this repository: https://github.com/stoyky/fuzzing_assignment.git
	- Build the example file with `~/AFL/afl-g++ load_single_out_png.cpp -lfreeimage`
	- Fuzz with `./afl-fuzz -i ~/fuzzing_assignment/test-dir/ -o ~/fuzzing_assignment/out-dir/ ~/fuzzing_assignment/a.out @@`
