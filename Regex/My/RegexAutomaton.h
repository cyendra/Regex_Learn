#include "RegexData.h"
#include <list>
#include <map>

namespace regex_internal
{
	class State;
	class Transition;

	// 表示连接两个状态的边
	class Transition
	{
	public:
		enum Type
		{
			Chars,				//range为字符范围
			Epsilon,
			BeginString,
			EndString,
			Nop,				//无动作（不可消除epsilon，用来控制优先级）
			Capture,			//capture为捕获频道
			Match,				//capture为捕获频道，index为匹配的位置，-1代表匹配频道下面的所有项目
			Positive,			//正向匹配
			Negative,			//反向匹配
			NegativeFail,		//反向匹配失败
			End					//Capture, Position, Negative
		};

		// 源状态结点
		State*					source;
		
		// 目标状态结点
		State*					target;
		
		// 字符集
		CharRange				range;
		
		Type					type;
		int						capture;
		int						index;
	};

	// 自动机中的状态结点
	class State
	{
	public:
		// 状态发出的边集
		std::list<Transition*>		transitions;
		// 进入状态的边集
		std::list<Transition*>		inputs;
		bool					finalState;
		void*					userData;
	};

	// 自动机类
	class Automaton
	{
	public:
		typedef Automaton*		Ref;

		std::list<State*>		states;
		std::list<Transition*>	transitions;
		std::list<std::string>	captureNames;
		State*					startState;

		Automaton();

		State*					NewState();
		Transition*				NewTransition(State* start, State* end);
		Transition*				NewChars(State* start, State* end, CharRange range);
		Transition*				NewEpsilon(State* start, State* end);
		Transition*				NewBeginString(State* start, State* end);
		Transition*				NewEndString(State* start, State* end);
		Transition*				NewNop(State* start, State* end);
		Transition*				NewCapture(State* start, State* end, int capture);
		Transition*				NewMatch(State* start, State* end, int capture, int index = -1);
		Transition*				NewPositive(State* start, State* end);
		Transition*				NewNegative(State* start, State* end);
		Transition*				NewNegativeFail(State* start, State* end);
		Transition*				NewEnd(State* start, State* end);
	};

	extern bool					PureEpsilonChecker(Transition* transition);
	extern bool					RichEpsilonChecker(Transition* transition);
	extern bool					AreEqual(Transition* transA, Transition* transB);
	extern Automaton::Ref		EpsilonNfaToNfa(Automaton::Ref source, bool(*epsilonChecker)(Transition*), std::map<State*, State*>& nfaStateMap);
	extern Automaton::Ref		NfaToDfa(Automaton::Ref source, std::multimap<State*, State*>& dfaStateMap);
}