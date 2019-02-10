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
	test.reccurent(B,buff.addLambda() , true, true);
	//Set the leaving function to the c state in order to save the buffer in
	//the s string.
	test.directionalLeaving(B, C, buff.getLambda(s));
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