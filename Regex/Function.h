/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
Framework::Function

Classes:
	Func<function-type>									：函数对象

Functions:
	Curry :: (A->B) -> A -> B							：参数拆分
	Combine :: (A->B) -> (A->C) -> (B->C->D) -> (A->D)	：函数组合
***********************************************************************/
#ifndef VCZH_FUNCTION
#define VCZH_FUNCTION
#include <memory.h>
#include "Basic.h"
#include "Pointer.h"
namespace vl
{
 
/***********************************************************************
vl::Func<R(TArgs...)>
***********************************************************************/

	template<typename T>
	class Func
	{
	};

	namespace internal_invokers
	{
		template<typename R, typename ...TArgs>
		class Invoker : public Object
		{
		public:
			virtual R Invoke(TArgs&& ...args) = 0;
		};

		//------------------------------------------------------
		
		template<typename R, typename ...TArgs>
		class StaticInvoker : public Invoker<R, TArgs...>
		{
		protected:
			R(*function)(TArgs ...args);

		public:
			StaticInvoker(R(*_function)(TArgs...))
				:function(_function)
			{
			}

			R Invoke(TArgs&& ...args)override
			{
				return function(ForwardValue<TArgs>(args)...);
			}
		};

		//------------------------------------------------------
		
		template<typename C, typename R, typename ...TArgs>
		class MemberInvoker : public Invoker<R, TArgs...>
		{
		protected:
			C*							sender;
			R(C::*function)(TArgs ...args);

		public:
			MemberInvoker(C* _sender, R(C::*_function)(TArgs ...args))
				:sender(_sender)
				,function(_function)
			{
			}

			R Invoke(TArgs&& ...args)override
			{
				return (sender->*function)(ForwardValue<TArgs>(args)...);
			}
		};

		//------------------------------------------------------

		template<typename C, typename R, typename ...TArgs>
		class ObjectInvoker : public Invoker<R, TArgs...>
		{
		protected:
			C							function;

		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}

			R Invoke(TArgs&& ...args)override
			{
				return function(ForwardValue<TArgs>(args)...);
			}
		};

		//------------------------------------------------------

		template<typename C, typename ...TArgs>
		class ObjectInvoker<C, void, TArgs...> : public Invoker<void, TArgs...>
		{
		protected:
			C							function;

		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}

			void Invoke(TArgs&& ...args)override
			{
				function(ForwardValue<TArgs>(args)...);
			}
		};
	}

	template<typename R, typename ...TArgs>
	class Func<R(TArgs...)> : public Object
	{
	protected:
		Ptr<internal_invokers::Invoker<R, TArgs...>>		invoker;
	public:
		typedef R FunctionType(TArgs...);
		typedef R ResultType;

		Func()
		{
		}

		Func(const Func<R(TArgs...)>& function)
		{
			invoker=function.invoker;
		}

		Func(R(*function)(TArgs...))
		{
			invoker=new internal_invokers::StaticInvoker<R, TArgs...>(function);
		}

		template<typename C>
		Func(C* sender, R(C::*function)(TArgs...))
		{
			invoker=new internal_invokers::MemberInvoker<C, R, TArgs...>(sender, function);
		}

		template<typename C>
		Func(const C& function)
		{
			invoker=new internal_invokers::ObjectInvoker<C, R, TArgs...>(function);
		}

		R operator()(TArgs ...args)const
		{
			return invoker->Invoke(ForwardValue<TArgs>(args)...);
		}

		bool operator==(const Func<R(TArgs...)>& function)const
		{
			return invoker == function.invoker;
		}

		bool operator!=(const Func<R(TArgs...)>& function)const
		{
			return invoker != function.invoker;
		}

		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::function_lambda::LambdaRetriveType<R(TArgs...)>
***********************************************************************/
 
	namespace function_lambda
	{
		template<typename T>
		struct LambdaRetriveType
		{
			typedef vint Type;
			typedef vint FunctionType;
			typedef vint ResultType;
		};
 
		template<typename T>
		struct FunctionObjectRetriveType
		{
			typedef typename LambdaRetriveType<decltype(&T::operator())>::Type Type;
			typedef typename LambdaRetriveType<decltype(&T::operator())>::FunctionType FunctionType;
			typedef typename LambdaRetriveType<decltype(&T::operator())>::ResultType ResultType;
		};
 
		template<typename TObject, typename R, typename ...TArgs>
		struct LambdaRetriveType<R (__thiscall TObject::*)(TArgs...)const>
		{
			typedef Func<R(TArgs...)> Type;
			typedef R(FunctionType)(TArgs...);
			typedef R ResultType;
		};
 
		template<typename TObject, typename R, typename ...TArgs>
		struct LambdaRetriveType<R (__thiscall TObject::*)(TArgs...)>
		{
			typedef Func<R(TArgs...)> Type;
			typedef R(FunctionType)(TArgs...);
			typedef R ResultType;
		};
 
		template<typename R, typename ...TArgs>
		struct FunctionObjectRetriveType<R(*)(TArgs...)>
		{
			typedef Func<R(TArgs...)> Type;
			typedef R(FunctionType)(TArgs...);
			typedef R ResultType;
		}; 
 
		template<typename T>
		typename LambdaRetriveType<decltype(&T::operator())>::Type Lambda(T functionObject)
		{
			return functionObject;
		}

		template<typename T>
		typename FunctionObjectRetriveType<T>::Type ConvertToFunction(T functionObject)
		{
			return functionObject;
		}

#define LAMBDA vl::function_lambda::Lambda
#define FUNCTION vl::function_lambda::ConvertToFunction
#define FUNCTION_TYPE(T) typename vl::function_lambda::FunctionObjectRetriveType<T>::Type
#define FUNCTION_RESULT_TYPE(T) typename vl::function_lambda::FunctionObjectRetriveType<T>::ResultType
	}
 
/***********************************************************************
vl::function_binding::Binding<R(TArgs...)>
***********************************************************************/

	namespace function_binding
	{
		template<typename T>
		struct Binding
		{
		};
		 
		template<typename T>
		struct CR{typedef const T& Type;};
		template<typename T>
		struct CR<T&>{typedef T& Type;};
		template<typename T>
		struct CR<const T>{typedef const T& Type;};
		template<typename T>
		struct CR<const T&>{typedef const T& Type;};
 
		template<typename R, typename T0, typename ...TArgs>
		struct Binding<R(T0, TArgs...)>
		{
			typedef R FunctionType(T0, TArgs...);
			typedef R CurriedType(TArgs...);
			typedef T0 FirstParameterType;

			class Binder : public Object
			{
			protected:
				Func<FunctionType>				target;
				T0								firstArgument;
			public:
				Binder(const Func<FunctionType>& _target, T0 _firstArgument)
					:target(_target)
					,firstArgument(ForwardValue<T0>(_firstArgument))
				{
				}

				R operator()(TArgs ...args)const
				{
					return target(firstArgument, args...);
				}
			};

			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}

				Func<CurriedType> operator()(T0 firstArgument)const
				{
					return Binder(target, firstArgument);
				}
			};
		}; 
	}
 
	template<typename T>
	Func<Func<typename function_binding::Binding<T>::CurriedType>(typename function_binding::Binding<T>::FirstParameterType)>
	Curry(T* function)
	{
		return typename function_binding::Binding<T>::Currier(function);
	}
 
	template<typename T>
	Func<Func<typename function_binding::Binding<T>::CurriedType>(typename function_binding::Binding<T>::FirstParameterType)>
	Curry(const Func<T>& function)
	{
		return typename function_binding::Binding<T>::Currier(function);
	}

/***********************************************************************
vl::function_combining::Combining<R1(TArgs...), R2(TArgs...), R(R1,R2)>
***********************************************************************/
 
	namespace function_combining
	{
		template<typename A, typename B, typename C>
		class Combining
		{
		};
 
		template<typename R1, typename R2, typename R, typename ...TArgs>
		class Combining<R1(TArgs...), R2(TArgs...), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(TArgs...)>			function1;
			Func<R2(TArgs...)>			function2;
			Func<R(R1, R2)>				converter;
		public:
			typedef R1 FirstFunctionType(TArgs...);
			typedef R2 SecondFunctionType(TArgs...);
			typedef R ConverterFunctionType(R1, R2);
			typedef R FinalFunctionType(TArgs...);

			Combining(const Func<R1(TArgs...)>& _function1, const Func<R2(TArgs...)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}

			R operator()(TArgs&& ...args)const
			{
				return converter(function1(ForwardValue<TArgs>(args)...), function2(ForwardValue<TArgs>(args)...));
			}
		};
	}

	template<typename F1, typename F2, typename C>
	Func<typename function_combining::Combining<F1, F2, C>::FinalFunctionType>
	Combine(Func<C> converter, Func<F1> function1, Func<F2> function2)
	{
		return function_combining::Combining<F1, F2, C>(function1, function2, converter);
	}

	template<typename T>
	Func<Func<T>(Func<T>,Func<T>)> Combiner(const Func<typename Func<T>::ResultType(typename Func<T>::ResultType,typename Func<T>::ResultType)>& converter)
	{
		typedef typename Func<T>::ResultType R;
		return Curry<Func<T>(Func<R(R,R)>,Func<T>,Func<T>)>(Combine)(converter);
	}
}
#endif