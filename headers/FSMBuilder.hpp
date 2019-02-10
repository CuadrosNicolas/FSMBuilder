/**
 * @brief
 *
 * @file FSMBuilder.hpp
 * @author Cuadros Nicolas
 * @date 2018-08-29
 */
#ifndef FSM
#define FSM
#include <iostream>
#include <functional>
#include <exception>
/**
 * @brief
 * Namespace defining an FSMBuilder.
 */
namespace FSMBuilder
{
/**
 * @brief
 * Exception to throw when a state called by a function doesn't exist in
 * the automate.
 */
struct unknownStateException : public std::exception
{
	const char *what() const throw()
	{
		return "Unknown State exception";
	};
};
/**
 * @brief
 * Exception to throw when a state condition need to.
 */
struct conditionException : public std::exception
{
	/**
	 * @brief Construct a new condition Exception object
	 *
	 * @param message
	 */
	conditionException(std::string message)
	{
		exceptionMessage = message;
	};
	const char *what() const throw()
	{
		return exceptionMessage.c_str();
	};
	std::string exceptionMessage;
};
template <const size_t T, typename Container, typename element>
class automate
{
  public:
	typedef std::function<bool(element c)> condition;
	typedef std::function<int(element c)> intCondition;
	typedef std::function<void(element c)> action;
	/**
	 * @brief Construct a new automate object
	 *
	 */
	automate()
	{
		for (size_t i = 0; i < T; i++)
		{
			m_states[i].num = i;
			m_states[i].condition = [i](element c) -> int { return i; };
		}
	};
	/**
	 * @brief Construct a new automate object
	 *
	 * @param autoR Define if the automate auto reset his state to the begining
	 * state after a parsing.
	 */
	automate(bool autoR) : automate()
	{
		setAutoReset(autoR);
	};
	/**
	 * @brief Set the Auto Reset object
	 * 
	 * @param autoR Define if the automate auto reset his state to the begining
	 * state after a parsing.
	 * @return automate& 
	 */
	automate &setAutoReset(bool autoR)
	{
		m_autoReset = autoR;
	};
	/**
	 * @brief
	 *Define a transition from a state to an other.
	 * @param actual State from which the transition leave.
	 * @param next State from which the transition goes.
	 * @param condition Lambda function defining the condition for the transition.
	 * @param callback Function to call when the condition of the transition is validated.
	 * @return automate& Return the automate.
	 */
	automate &transition(const int &actual, const int &next, const condition &condition, const action &callback = NULL)
	{
		if (testState(actual) && testState(next))
			m_states[actual].condition = imbricate(*this,m_states[actual].condition, condition, actual, next, callback);
		return *this;
	};
	/**
	 * @brief
	 *Define an exception to throw by a state when the condition is true.
	 * @param actual State from which the transition leave.
	 * @param next State from which the transition goes.
	 * @param condition Lambda function defining the condition for the transition.
	 * @param callback Function to call when the condition of the transition is validated.
	 * @return automate& Return the automate.
	 */
	automate &exception(const int &actual, const condition &condition,const std::string message)
	{
		if (testState(actual))
			m_states[actual].condition = imbricate_exception(m_states[actual].condition, condition, actual, message);
		return *this;
	};
	/**
	 * @brief Get the States object
	 * Return the different states possible of the automate.
	 * This function allow you to call structure binding.
	 * @return std::array<int, T>
	 */
	std::array<int, T> getStates()
	{
		std::array<int, T> num_states;
		for (size_t i = 0; i < T; i++)
		{
			num_states[i] = m_states[i].num;
			for (size_t j = 0; j < T; j++)
				m_states[i].directionalLeaving[j] = [](element c) {};
		}
		return num_states;
	};
	/**
	 * @brief
	 * Define the end state of the automate.
	 * If the end state is reach, the automate will stop.
	 * @param state State to define as the end state.
	 * @return automate&
	 */
	automate &end(const size_t &state)
	{
		if (testState(state))
			m_endState = state;
		return *this;
	};
	/**
	 * @brief
	 * Define the state from which the automate will begin.
	 * @param state
	 * @return automate& 
	 */
	automate &begin(const size_t &state)
	{
		if (testState(state))
			m_beginState = state;
		return *this;
	};
	/**
	 * @brief
	 * Define a leaving function for a state,
	 * this function will be call after leaving the state.
	 * @param state Leaving state.
	 * @param ac Function to call.
	 * @return automate& 
	 */
	automate &leaving(const size_t &state, action ac)
	{
		if (testState(state))
			m_states[state].leaving = ac;
		return *this;
	};
	/**
	 * @brief
	 * Define a directional leaving function,
	 * which is a leaving function call only in the case where the automate
	 * make a transition from the states defines.
	 * @param fromState Leaving state
	 * @param toState Entering state
	 * @param ac Function to call
	 * @return automate& 
	 */
	automate &directionalLeaving(const size_t &fromState, const size_t &toState, action ac)
	{
		if (testState(fromState) && testState(toState))
			m_states[fromState].directionalLeaving[toState] = ac;
		return *this;
	};
	/**
	 * @brief
	 * Define an entering function for a state,
	 * this function will be call when an automate enter this state.
	 * @param state State to enter.
	 * @param ac Function to call.
	 * @return automate& 
	 */
	automate &entering(const size_t &state, action ac)
	{
		if (testState(state))
			m_states[state].entering = ac;
		return *this;
	};
	/**
	 * @brief
	 * Define a reccurent function for a state,
	 * this function will be call each time the automate will stay in the current
	 * state.
	 * @param state State to set the reccurent function.
	 * @param ac Function to call.
	 * @param instantReccurent Define if the function has to be call directly after a transition.
	 * @param lastReccurent Define if the function has to be call a last time before making a transition.
	 * @return automate& 
	 */
	automate &reccurent(const size_t &state, action ac, bool instantReccurent = false, bool lastReccurent = false)
	{
		if (testState(state))
		{
			m_states[state].reccurent = ac;
			m_states[state].instantReccurent = instantReccurent;
			m_states[state].lastReccurent = lastReccurent;
		}
		return *this;
	};
	/**
	 * @brief
	 * Parse a container containing element with the rules defined by the user.
	 * @param s 
	 */
	void parse(const Container &s)
	{
		int tempState = m_actualState;
		for (auto c : s)
		{
			tempState = m_states[m_actualState].condition(c);
			if (tempState != m_actualState)
			{
				m_states[m_actualState].directionalLeaving[tempState](c);
				m_states[m_actualState].leaving(c);

				m_actualState = tempState;

				m_states[m_actualState].entering(c);
				if (m_states[m_actualState].instantReccurent)
					m_states[m_actualState].reccurent(c);
			}
			else
				m_states[m_actualState].reccurent(c);
			if (m_actualState == m_endState)
				break;
		}
		if (m_autoReset)
			m_actualState = m_beginState;
	};
	/**
	 * @brief
	 * Test if a state is available in the automate.
	 * @param state
	 * @return true 
	 * @return false 
	 */
	bool testState(const size_t state)
	{
		if (!(state >= 0 && state < T))
			throw new unknownStateException;
		return state >= 0 && state < T;
	}

  private:
	/**
   * @brief
   * Structure representing a state of the automate.
   */
	struct states_s
	{
		~states_s()
		{
		};
		/**
		 * @brief
		 * Number representing the state.
		 */
		int num;
		/**
		 * @brief
		 * Function embedding all the different condition for the transitions.
		 */
		intCondition condition;
		/**
		 * @brief
		 * Entering function
		 */
		action entering = [](element c) {};
		/**
		 * @brief
		 * Leaving function.
		 */
		action leaving = [](element c) {};
		/**
		 * @brief
		 * Array containing the different directional leaving function.
		 */
		std::array<action, T> directionalLeaving;
		/**
		 * @brief
		 * Reccurent function.
		 */
		action reccurent = [](element c) {};
		/**
		 * @brief
		 * Boolean determining if the reccurent function has to be call directly after
		 * the transition.
		 */
		bool instantReccurent = false;
		/**
		 * @brief
		 * Boolean determining if the reccurent function has to be call one last time
		 * before leaving the state.
		 */
		bool lastReccurent = false;
	};
	/**
	 * @brief
	 * Static function permitting to embedded multiple condition for a state in one function
	 * by using lambda function imbrication.
	 * @param f Function to embed in.
	 * @param newF Function to embed in f
	 * @param actual State which will have this function
	 * @param next State to go if the condition is validate.
	 * @param callback Function to call if the condition is true
	 * @return intCondition
	 */
	static intCondition imbricate(automate& aut,const intCondition &f, const condition &newF, const size_t &actual, const size_t &next, const action &callback = NULL)
	{
		auto a = [&aut,f,newF,actual,next,callback](element c) -> int {
			if (!newF(c))
			{
				return f(c);
			}
			else
			{
				if (aut.m_states[actual].lastReccurent)
					aut.m_states[actual].reccurent(c);
				if (callback != NULL)
					callback(c);
				return next;
			}
		};
		return a;
	};
	/**
	 * @brief
	 * Define a condition which will throw an exception if it's true.
	 * @param f previous function
	 * @param newF Condition to add
	 * @param actual Actual state
	 * @param message Message to throw by the exception
	 * @return intCondition
	 */
	static intCondition imbricate_exception(const intCondition &f, const condition &newF, const size_t &actual,const std::string& message)
	{
		auto a = [message,f, newF, actual](element c) -> int {
			if (!newF(c))
			{
				return f(c);
			}
			else
			{
				throw new conditionException(message);
				return actual;
			}
		};
		return a;
	};
	/**
	 * @brief
	 * Array containing the differents states of the automate.
	 */
	std::array<states_s, T> m_states;
	/**
	 * @brief
	 * Actual state of the automate.
	 */
	int m_actualState = 0;
	/**
	 * @brief
	 * end state of the automate.
	 */
	int m_endState = -1;
	/**
	 * @brief
	 * begin state of the automate.
	 */
	int m_beginState = 0;
	/**
	 * @brief
	 *Define if the automate will auto reset his state at the end of a parsing.
	 *Default value : true.
	 */
	bool m_autoReset = true;
};
} // namespace FSMBuilder
#endif