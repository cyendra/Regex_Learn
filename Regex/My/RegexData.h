#include <set>

namespace regex_internal
{

	/***********************************************************************
	基础数据结构
	***********************************************************************/

	class CharRange
	{
	public:
		typedef std::set<CharRange>			List;

		wchar_t					begin;
		wchar_t					end;

		CharRange();
		CharRange(wchar_t _begin, wchar_t _end);

		bool					operator<(CharRange item)const;
		bool					operator<=(CharRange item)const;
		bool					operator>(CharRange item)const;
		bool					operator>=(CharRange item)const;
		bool					operator==(CharRange item)const;
		bool					operator!=(CharRange item)const;

		bool					operator<(wchar_t item)const;
		bool					operator<=(wchar_t item)const;
		bool					operator>(wchar_t item)const;
		bool					operator>=(wchar_t item)const;
		bool					operator==(wchar_t item)const;
		bool					operator!=(wchar_t item)const;
	};
}