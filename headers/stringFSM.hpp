/**
 * @brief
 *
 * @file stringFSM.hpp
 * @author Cuadros Nicolas
 * @date 2018-08-29
 */
#ifndef STRINGFSM
#define STRINGFSM
#include "./FSMBuilder.hpp"
/**
 * @brief
 * Namespace defining a specialized FSM for string parsing.
 */
namespace stringFSM
{
/**
	 * @brief
	 *Define a string automate.
	 * @tparam T Number of state.
	 */
template <const size_t T>
using stringAutomate = FSMBuilder::automate<T, std::string, char>;
/**
 * @brief
 * Define the function type for the condition.
 */
using charCondition = const std::function<bool(char c)>;
/**
 * @brief
 * Small class to manipulate a buffer with an automate.
 */
class buffer
{
  public:
	/**
   * @brief Construct a new buffer object
   *
   */
	buffer();
	/**
	 * @brief
	 * Add a character to the buffer.
	 * @param c Character to add.
	 */
	void add(const char c);
	/**
	 * @brief
	 * Empty the buffer.
	 */
	void clean();
	/**
	 * @brief
	 * Return the size of the string contained by the buffer.
	 * @return size_t 
	 */
	size_t size() const;
	/**
	 * @brief
	 * Return the string contained by the buffer.
	 * @return std::string 
	 */
	std::string get() const;
	/**
	 * @brief Get the lambda function to assign the value of the buffer
	 *to string.
	 *
	 * @param saver String to apply the content of the buffer
	 * @return std::function<void(char c)> 
	 */
	std::function<void(char c)> getLambda(std::string &saver);
	/**
	 * @brief Get the lambda function to add a character to the buffer.
	 *
	 *
	 * @return std::function<void(char c)>
	 */
	std::function<void(char c)> addLambda();

  private:
	/**
   * @brief
   * Content of the buffer.
   */
	std::string content = "";
};

/**
 * @brief
 *This function allow to an And operator between some conditions functions.
 * @tparam Args
 * @param arr Conditions function to apply an And operator
 * @return charCondition
 */
template <typename... Args>
charCondition And(const Args &&... arr);
/**
 * @brief
 *This function allow to an Or operator between some conditions functions.
 * @tparam Args
 * @param arr Conditions function to apply an Or operator
 * @return charCondition
 */
template <typename... Args>
charCondition Or(const Args &&... conditions);
/**
 * @brief
 *This function allow to an Not operator on a condition function.
 * @tparam Args
 * @param f Condition function to apply an Not operator
 * @return charCondition
 */
charCondition Not(charCondition &&f);
/**
 * @brief
 * This function return a condition function on a chararcter
 * @param a Caracter to match with.
 * @return charCondition 
 */
charCondition Char(const char &&a);
/**
 * @brief
 * Function returning a condition function which test if a char match between
 * two others like 'begin>=c>=end'.
 * @param begin Left value of the match.
 * @param end Right value of the match.
 * @return charCondition 
 */
charCondition range(const char &&begin, const char &&end);
/**
 * @brief
 * Function returning a condition function testing if a char is a number.
 * @return charCondition
 */
charCondition number();
/**
 * @brief
 * Function returning a condition function testing if a char is a upper case letter.
 * @return charCondition
 */
charCondition majLetter();
/**
 * @brief
 * Function returning a condition function testing if a char is a lower case letter.
 * @return charCondition
 */
charCondition minLetter();
/**
 * @brief
 * Function returning a condition function testing if a char is a letter.
 * @return charCondition
 */
charCondition letter();
/**
 * @brief
 * Function returning a condition function testing if a char is an opening bracket '(" or '{'.
 * @return charCondition
 */
charCondition openingBracket();
/**
 * @brief
 * Function returning a condition function testing if a char is a closing bracket ')" or '}'.
 * @return charCondition
 */
charCondition closingBracket();
/**
 * @brief
 * Function returning a condition function testing if a char is a whitespace.
 * @return charCondition
 */
charCondition whitespace();

} // namespace stringFSM
#endif