# FSMBuilder

## Description

FSMBuilder is a small library which allow to construct finite state mahcine
by defining his states and transitions.

Basically this project was created to experiment with C++11,14,17 new features
like fold expression, lambda function, structure binding and type inference;
and became usefull for some cases when I didn't want to build an enumeration
plus unreadable switch case statement just for creating a simple parser.

On the one hand this library allow you to write an FSM with a simple syntax,
on the other this will slower and take much more memory than writing your parser
by hand; so i recommend it for experimentation or creating parser when you don't
have the time to deal with switch case statement monster or even if you like to
write FSM with this syntax.

## Installation

To use this library or test the example :

```sh
git clone https://github.com/Inagaroth/FSMBuilder.git
cd FSMBuilder
mkdir bin/
mkdir bin/objects
make
```

To run the example :

```sh
./bin/Test_fsm
```

## Example

This example is provided in the repo, it show you the good syntax to
adopt while using the library.

In this example we build a simple automate to parse a "AAA" string in a sentence
full whitespace.
The automate trigger the saving state by encoutering a \" and then end when
it find an other.

In this example I use a stringAutomate provided by stringFSM.hpp,
it's just a pre-made automate for parsing string.
This module also provide some pre-made condition to set to your automate for
detecting character like the Char(char c) function which will a return
a condition function testing if the character parsed by the stringAutomate is
equel to the char c.

For more information on this module you can see the wiki if it's made at this time.

```C++
#include <iostream>
#include "./headers/stringFSM.hpp"
#include <cassert>
using namespace stringFSM;
/**
 * @brief
 * Small programm for parsing a \"AAA...\" string in a sentence and print
 * the first string found.
 * @return int 
 */
int main()
{
	//Create a string buffer and a string to receive the result
	buffer buff;
	std::string s;
	//Create a 3 states stringAutomate
	stringAutomate<3> test;
	//Get the different state that the automate can take
	auto [A, B, C] = test.getStates();
	//Define the different transition
	//this say that if we are in A, we go to B, if the actual caract is a \"
	test.transition(A, B, Char('\"'));
	test.transition(B, C, Char('\"'));
	//Add a reccurent function to the B state which will save each caracter saved
	//while parsing
	//the reccurent function is instant and will be apply one last time after the
	//transition to be able to save the \"
	test.reccurent(B, buffer::addLambda(buff), true, true);
	//Set the leaving function to the c state in order to save the buffer in
	//the s string.
	test.directionalLeaving(B, C, buffer::getLambda(buff, s));
	//define state C as the end of the automate
	test.end(C);
	//run the automate
	test.parse("     \"Hello world !\"   ");
	//print the result stored in s string.
	std::cout << "Value : " << s << std::endl;
	//test if the result of the parsing is what we expected.
	assert(s == "\"Hello world !\"" && "Error the result should be : " && "\"Hello world !\"");
	return 0;
}
```
## License

You can use this project for everything you want for.
