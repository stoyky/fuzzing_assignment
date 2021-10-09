# Software Security Fuzzing Assignment
## Target: FreeImage 3.8.1

"load_single_out_png.cpp" takes as command line input a single file and outputs a PNG.

TODO: Write to more output files to get more path coverage.

### Build
/usr/local/bin/afl-g++ load_single_out_png.cpp -lfreeimage
### Run
./a.out filename
### Example
./a.out test-dir/index.jpeg

A file with random filename and extension .png will be stored in main folder.
### Fuzzing with AFL
afl-fuzz -i test-dir/ -o out-dir/ ./a.out @@
