# Software Security Fuzzing Assignment
## Target: FreeImage 3.8.1

### Build
/usr/local/bin/afl-g++ load_single_out_png.cpp -lfreeimage
### Run
./a.out <file>
### Example
./a.out test-dir/index.jpeg
A file with random filename and extension .png will be stored in main folder.
