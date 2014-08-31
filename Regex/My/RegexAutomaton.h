#include "RegexData.h"
#include <list>
#include <map>

namespace regex_internal
{
	class State;
	class Transition;

	// ��ʾ��������״̬�ı�
	class Transition
	{
	public:
		enum Type
		{
			Chars,				//rangeΪ�ַ���Χ
			Epsilon,
			BeginString,
			EndString,
			Nop,				//�޶�������������epsilon�������������ȼ���
			Capture,			//captureΪ����Ƶ��
			Match,				//captureΪ����Ƶ����indexΪƥ���λ�ã�-1����ƥ��Ƶ�������������Ŀ
			Positive,			//����ƥ��
			Negative,			//����ƥ��
			NegativeFail,		//����ƥ��ʧ��
			End					//Capture, Position, Negative
		};

		// Դ״̬���
		State*					source;
		
		// Ŀ��״̬���
		State*					target;
		
		// �ַ���
		CharRange				range;
		
		Type					type;
		int						capture;
		int						index;
	};

	// �Զ����е�״̬���
	class State
	{
	public:
		// ״̬�����ı߼�
		std::list<Transition*>		transitions;
		// ����״̬�ı߼�
		std::list<Transition*>		inputs;
		bool					finalState;
		void*					userData;
	};

	// �Զ�����
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