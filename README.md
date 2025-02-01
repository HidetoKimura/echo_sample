# echo_sample

# How to build
~~~
$ mkdir build
$ cd build
$ cmake ..
$ make
~~~
# How to use
~~~
$ ./echo_sample /tmp/haltp-command &
$ echo "dtv-tuneup /tmp/haltp/dtv/channel" > /tmp/haltp-command
$ cat /tmp/haltp/dtv/channel 
$ echo "quit" > /tmp/haltp-command
[1]+  Done                    ./echo_sample /tmp/haltp-command
$ killall echo_sample
~~~