# Number Oracle

My solution to [assignment sheet 7](https://cnds.jacobs-university.de/courses/os-2020/p07.pdf) of the [Operating Systems Fall 2020](https://cnds.jacobs-university.de/courses/os-2020/) course.

Please find the code in [quiz.c](quiz.c)

## Requirements

My json parser uses the library [json-c](https://github.com/json-c/json-c)

It can be installed via either of the following commands
(whichever is more applicable to your case)

`sudo apt install -y libjson-c-dev` for Debian/Ubuntu Linux users

or

`brew install json-c` for macOS users

Refer to [this link](https://github.com/json-c/json-c#build-instructions--) for more detailed build instructions.


## Compile the Code

The executable can be compiled with the following command:

```gcc -Wall -o quiz -I/usr/local/include -L/usr/local/lib -ljson-c  quiz.c```

## Run the Code

Simply do: `./quiz`


Enjoy! ☺️ 
