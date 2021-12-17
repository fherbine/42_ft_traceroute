ft_traceroute
=======

Basically a reproduction of the `traceroute` command (Windows `tracert`), developped in C language.

Final grade: **121/100**

## Prerequisites

You'll have to install `libcurl` to run the program:
```sh
$ sudo apt install libcurl4-openssl-dev
```

## How to compile 

This project `Makefile` uses `clang` as C compiler, please make sure clang is installed,
then run:

```sh
$ make
```

## How to run

As the regular `traceroute (1)` command, you can start with:
```sh
$ ./ft_traceroute google.com
[...]
$ ./ft_traceroute 8.8.8.8
```

To see, all the available options, please type:

```sh
$ ./ft_traceroute -h
```
