/*
                        ::
                      :;J7, :,                        ::;7:
                      ,ivYi, ,                       ;LLLFS:
                      :iv7Yi                       :7ri;j5PL
                     ,:ivYLvr                    ,ivrrirrY2X,
                     :;r@Wwz.7r:                :ivu@kexianli.
                    :iL7::,:::iiirii:ii;::::,,irvF7rvvLujL7ur
                   ri::,:,::i:iiiiiii:i:irrv177JX7rYXqZEkvv17
                ;i:, , ::::iirrririi:i:::iiir2XXvii;L8OGJr71i
              :,, ,,:   ,::ir@mingyi.irii:i:::j1jri7ZBOS7ivv,
                 ,::,    ::rv77iiiriii:iii:i::,rvLq@huhao.Li
             ,,      ,, ,:ir7ir::,:::i;ir:::i:i::rSGGYri712:
           :::  ,v7r:: ::rrv77:, ,, ,:i7rrii:::::, ir7ri7Lri
          ,     2OBBOi,iiir;r::        ,irriiii::,, ,iv7Luur:
        ,,     i78MBBi,:,:::,:,  :7FSL: ,iriii:::i::,,:rLqXv::
        :      iuMMP: :,:::,:ii;2GY7OBB0viiii:i:iii:i:::iJqL;::
       ,     ::::i   ,,,,, ::LuBBu BBBBBErii:i:i:i:i:i:i:r77ii
      ,       :       , ,,:::rruBZ1MBBqi, :,,,:::,::::::iiriri:
     ,               ,,,,::::i:  @arqiao.       ,:,, ,:::ii;i7:
    :,       rjujLYLi   ,,:::::,:::::::::,,   ,:i,:,,,,,::i:iii
    ::      BBBBBBBBB0,    ,,::: , ,:::::: ,      ,,,, ,,:::::::
    i,  ,  ,8BMMBBBBBBi     ,,:,,     ,,, , ,   , , , :,::ii::i::
    :      iZMOMOMBBM2::::::::::,,,,     ,,,,,,:,,,::::i:irr:i:::,
    i   ,,:;u0MBMOG1L:::i::::::  ,,,::,   ,,, ::::::i:i:iirii:i:i:
    :    ,iuUuuXUkFu7i:iii:i:::, :,:,: ::::::::i:i:::::iirr7iiri::
    :     :rk@Yizero.i:::::, ,:ii:::::::i:::::i::,::::iirrriiiri::,
     :      5BMBBBBBBSr:,::rv2kuii:::iii::,:i:,, , ,,:,:i@petermu.,
          , :r50EZ8MBBBBGOBBBZP7::::i::,:::::,: :,:,::i;rrririiii::
              :jujYY7LS0ujJL7r::,::i::,::::::::::::::iirirrrrrrr:ii:
           ,:  :@kevensun.:,:,,,::::i:i:::::,,::::::iir;ii;7v77;ii;i,
           ,,,     ,,:,::::::i:iiiii:i::::,, ::::iiiir@xingjief.r;7:i,
        , , ,,,:,,::::::::iiiiiiiiii:,:,:::::::::iiir;ri7vL77rrirri::
         :,, , ::::::::i:::i:::i:i::,,,,,:,::i:i:::iir;@Secbone.ii:::
 */

/**
Classes:
	RegexString						：字符串匹配结果
	RegexMatch						：匹配结果
	Regex							：正则表达式引擎
	RegexToken						：词法记号
	RegexTokens						：词法记号表
	RegexLexer						：词法分析器
**/

#include <string>
#include <list>
#include <vector>
#include <map>

namespace regex_internal
{
	class PureResult;
	class PureInterpretor;
	class RichResult;
	class RichInterpretor;
}

namespace regex{

	/***********************************************************************
	正则表达式引擎数据结构
	***********************************************************************/

	class RegexString
	{
	protected:
		std::string										value;
		int												start;
		int												length;

	public:
		RegexString(int _start = 0);
		RegexString(const std::string& _string, int _start, int _length);

		int										Start()const;
		int										Length()const;
		const std::string&								Value()const;
		bool										operator==(const RegexString& string)const;
	};

	class RegexMatch
	{
		friend class Regex;
	public:
		typedef RegexMatch*											Ref;
		typedef std::list<Ref>										List;
		typedef std::list<RegexString>								CaptureList;
		typedef std::multimap<std::string, RegexString>				CaptureGroup;
	protected:
		std::list<RegexString>				captures;
		std::multimap<std::string, RegexString>	groups;
		bool										success;
		RegexString									result;

		RegexMatch(const std::string& _string, regex_internal::PureResult* _result);
		RegexMatch(const std::string& _string, regex_internal::RichResult* _result, regex_internal::RichInterpretor* _rich);
		RegexMatch(const RegexString& _result);
	public:

		bool										Success()const;
		const RegexString&							Result()const;
		const CaptureList&							Captures()const;
		const CaptureGroup&							Groups()const;
	};

	/***********************************************************************
	正则表达式引擎
	***********************************************************************/

	class Regex
	{
	protected:
		regex_internal::PureInterpretor*			pure;
		regex_internal::RichInterpretor*			rich;

		void										Process(const std::string& text, bool keepEmpty, bool keepSuccess, bool keepFail, RegexMatch::List& matches)const;
	public:
		Regex(const std::string& code, bool preferPure = true);
		~Regex();

		bool										IsPureMatch()const;
		bool										IsPureTest()const;

		RegexMatch::Ref								MatchHead(const std::string& text)const;
		RegexMatch::Ref								Match(const std::string& text)const;
		bool										TestHead(const std::string& text)const;
		bool										Test(const std::string& text)const;
		void										Search(const std::string& text, RegexMatch::List& matches)const;
		void										Split(const std::string& text, bool keepEmptyMatch, RegexMatch::List& matches)const;
		void										Cut(const std::string& text, bool keepEmptyMatch, RegexMatch::List& matches)const;
	};

	/***********************************************************************
	正则表达式词法分析器
	***********************************************************************/

	class RegexToken
	{
	public:
		int											start;
		int											length;
		int											token;
		const wchar_t*								reading;
		int											codeIndex;
		bool										completeToken;

		int											rowStart;
		int											columnStart;
		int											rowEnd;
		int											columnEnd;

		bool										operator==(const RegexToken& _token)const;
		bool										operator==(const wchar_t* _token)const;
	};

	class RegexTokens
	{
		friend class RegexLexer;
	protected:
		regex_internal::PureInterpretor*			pure;
		const std::vector<int>&						stateTokens;
		std::string									code;
		int											codeIndex;

		RegexTokens(regex_internal::PureInterpretor* _pure, const std::vector<int>& _stateTokens, const std::string& _code, int _codeIndex);
	public:
		RegexTokens(const RegexTokens& tokens);

		//collections::IEnumerator<RegexToken>*		CreateEnumerator()const;
		void										ReadToEnd(std::list<RegexToken>& tokens, bool(*discard)(int) = 0)const;
	};

	class RegexLexerWalker
	{
		friend class RegexLexer;
	protected:
		regex_internal::PureInterpretor*			pure;
		const std::vector<int>&						stateTokens;

		RegexLexerWalker(regex_internal::PureInterpretor* _pure, const std::vector<int>& _stateTokens);
	public:
		RegexLexerWalker(const RegexLexerWalker& walker);
		~RegexLexerWalker();

		int											GetStartState()const;
		int											GetRelatedToken(int state)const;
		void										Walk(wchar_t input, int& state, int& token, bool& finalState, bool& previousTokenStop)const;
		int											Walk(wchar_t input, int state)const;
		bool										IsClosedToken(const wchar_t* input, int length)const;
		bool										IsClosedToken(const std::string& input)const;
	};

	class RegexLexerColorizer
	{
		friend class RegexLexer;
	public:
		typedef void(*TokenProc)(void* argument, int start, int length, int token);

	protected:
		RegexLexerWalker							walker;
		int										currentState;

		RegexLexerColorizer(const RegexLexerWalker& _walker);
	public:
		RegexLexerColorizer(const RegexLexerColorizer& colorizer);
		~RegexLexerColorizer();

		void										Reset(int state);
		void										Pass(wchar_t input);
		int											GetStartState()const;
		int											GetCurrentState()const;
		void										Colorize(const wchar_t* input, int length, TokenProc tokenProc, void* tokenProcArgument);
	};

	class RegexLexer
	{
	protected:
		regex_internal::PureInterpretor*			pure;
		std::vector<int>							ids;
		std::vector<int>							stateTokens;
	public:
		RegexLexer(const collections::IEnumerable<WString>& tokens);
		~RegexLexer();

		RegexTokens									Parse(const std::string& code, int codeIndex = -1)const;
		RegexLexerWalker							Walk()const;
		RegexLexerColorizer							Colorize()const;
	};

}