# Software Security Fuzzing Assignment
## Target: FreeImage 3.8.1

Install FreeImage and AFL:

1. Packages
	- Standard `sudo apt update`, `sudo apt upgrade`
	- `sudo apt install make gcc g++`
       (- `sudo apt install gcc-multilib g++-multilib` for 32-bit version)
2. AFL
	- Clone this AFL repository: https://github.com/google/AFL.git
	- Install AFL with `make` and `sudo make install`
3. FreeImage
	- Clone this FreeImage repository: https://github.com/WinMerge/freeimage.git
	- Add `CC = ~/AFL/afl-gcc` and `CXX = ~/AFL/afl-g++` to Makefile.gnu
        - (Add `CFLAGS += -m32`, `CXXFLAGS += -m32`, and `LDFLAGS += -m32` to Makefile.gnu for 32-bit version)
	- Do `make` and `sudo make install` to install the C++ FreeImage library and the AFL instrumentation.
        - (Or `AFL_USE_ASAN=1 make` and `sudo make install` for ASAN)
4. Radamsa
	- `git clone https://github.com/Hwangtaewon/radamsa.git && cd radamsa`
	- `make` and `sudo make install`
5. Test suite
	- Install pip with `sudo apt install python3-pip`
	- Install gdown with `pip3 install gdown`
	- Get corpus from Google drive with `~/.local/bin/gdown --id 149CJ2AmLqBrRnwMzVKVIW5tKurEDda_z`
	- Unzip corpus with `sudo apt install unzip` and `unzip All.zip`
	- Delete files at least 100 kB with `find All/ -type 'f' -size +99k -delete`
	- (Run Radamsa: `radamsa -o All/fuzz-%n.%s -n 10000 All/*`)
	- (Run afl-cmin: `sudo ./afl-cmin -i All/ -o All-minimized/ -m 8000 -t 2000 ~/fuzzing_assignment/a.out @@`)
6. Fuzzing project
	- Clone this repository: https://github.com/stoyky/fuzzing_assignment.git
	- Build the example file with `~/AFL/afl-g++ single_in_multiple_out.cpp -lfreeimage`
        - (Optionally with `-m32` for 32-bit version and/or `AFL_USE_ASAN=1` for ASAN)
        - Write `echo core >/proc/sys/kernel/core_pattern` inside the file _/proc/sys/kernel/core_pattern_
	- Fuzz with `./afl-fuzz -m 8000 -t 2000+ -i ~/path/to/corpus/All/ -o ~/fuzzing_assignment/out-dir/ ~/fuzzing_assignment/a.out @@`

7. Libfuzzer
	- Install clang with `sudo apt install clang`
	- Compile fuzzing target with `clang -g -O1 -fsanitize=fuzzer libfuzzer_simo.cpp -lfreeimage`
	- Run a out and specify test directory as first argument `./a.out ~/Downloads/All`
