/**
 * @brief
 *
 * @file stringFSM.hpp
 * @author Cuadros Nicolas
 * @date 2018-08-29
 */
#include "../headers/stringFSM.hpp"
namespace stringFSM
{
buffer::buffer()
{

}
void buffer::add(const char c)
{
	content = content + c;
}
void buffer::clean()
{
	content = "";
}
size_t buffer::size() const
{
	return content.size();
}
std::string buffer::get() const
{
	return content;
}
std::function<void(char c)> buffer::getLambda(std::string &saver)
{
	return [&](char c) { saver = this->get(); };
}
std::function<void(char c)> buffer::addLambda()
{
	return [&](char c) { this->add(c); };
}

charCondition Not(charCondition &&f)
{
	return [f](char c) -> bool { return !f(c); };
}

template <typename... Args>
charCondition And(const Args &&... arr)
{
	return [=](char c) {
		return (... && arr(c));
	};
}

template <typename... Args>
charCondition Or(const Args &&... conditions)
{
	return [=](char c) {
		return (... || conditions(c));
	};
}

charCondition Char(const char &&a)
{
	return [a](char c) -> bool { return c == a; };
}

charCondition range(const char &&begin, const char &&end)
{
	return [begin, end](char c) -> bool { return c >= begin && c <= end; };
}

charCondition number()
{
	return range('0', '9');
}

charCondition majLetter()
{
	return range('A', 'Z');
}

charCondition minLetter()
{
	return range('a', 'z');
}

charCondition letter()
{
	return Or(majLetter(), minLetter());
}

charCondition openingBracket()
{
	return Or(Char('{'), Char('['));
}
charCondition closingBracket()
{
	return Or(Char('}'), Char(']'));
}
charCondition whitespace()
{
	return Or(Char(' '), Char('\t'), Char('\n'));
}
}