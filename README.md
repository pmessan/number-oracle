Please find the code solution to the homework in quiz.c

My json parser uses a library json-c

it can be installed via either of the following commands
(whichever is more applicable to your case)

sudo apt install -y libjson-c-dev

or

brew install json-c 


The executable can be compiled with the following command:

gcc -Wall -o quiz -I/usr/local/include -L/usr/local/lib -ljson-c  quiz.c