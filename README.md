# echo_sample

# How to build
~~~
$ mkdir buid
$ cd build
$ cmake ..
$ make
~~~
# How to use
~~~
$ ./echo_sample &
$ echo "0" > /tmp/command
1: 0
$ echo "50" > /tmp/command
2: 50
$ echo "100" > /tmp/command
3: 100
$ echo "end" > /tmp/command
4: end
[1]+  Done                    ./echo_sample
~~~