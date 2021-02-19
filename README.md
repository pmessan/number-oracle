# Number Oracle

My solution to [assignment sheet 7](https://cnds.jacobs-university.de/courses/os-2020/p07.pdf) of [Operating Systems Fall 2020](https://cnds.jacobs-university.de/courses/os-2020/) course.

Please find the code in [](quiz.c)

My json parser uses a library json-c

It can be installed via either of the following commands
(whichever is more applicable to your case)

`sudo apt install -y libjson-c-dev` For Debian/Ubuntu Linux users

or

`brew install json-c` For macOS users


The executable can be compiled with the following command:

```gcc -Wall -o quiz -I/usr/local/include -L/usr/local/lib -ljson-c  quiz.c```
