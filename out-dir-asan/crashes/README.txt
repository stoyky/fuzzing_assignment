Command line used to find this crash:

./afl-fuzz -m 8000 -t 2000+ -i /home/azureuser/fuzzing_assignment/test-dir/all-minimized/ -o /home/azureuser/fuzzing_assignment/out-dir-asan/ /home/azureuser/fuzzing_assignment/a.out @@

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 7.81 GB.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
