/*
 * option.hpp
 *
 * 	Created on: 14.07.2016
 * 		Author: andreas
 */

#pragma once
#include "hdrstd.hpp"
#include "monad.hpp"

namespace hdrtypes{
namespace option{
	namespace _impl{
		template<typename C>
		using MonadBind = hdrtypes::monad::MonadBind<C>;

		template<bool,typename>
		struct option;

		template<typename type>
		using some = option<true, type>;
		using none = option<false, void>;

		struct _option_monad_impl{
			template<typename T>
			struct ret {
				 	using result = some<T>;
			};

			template<typename option, typename function>
			struct bind{
			};
			template<typename function>
			struct bind<none, function>{
				using result = none;
			};
			template<typename type, typename function>
			struct bind<some<type>, function>{
				using result = typename function::template expr<type>::result;
			};
		};

		using _option_monad = MonadBind<_option_monad_impl>;

		template<typename TypeKey>
		struct _option_type { using type = hdrstd::Void; };
		template<>
		struct _option_type<hdrtypes::monad::MonadType> { using type = _option_monad; };

		template<bool existsB, typename typeT>
		struct option{
			constexpr static bool exists = existsB;
			using type = typeT;
		};
	}

	template<typename type>
	using some = _impl::some<type>;
	using none = _impl::none;

	template<typename TypeKey>
	using option_type = _impl::_option_type<TypeKey>;
} // namespace option
} //namespace hdrtypes

namespace hdrstd{
	template<bool exists, typename T>
	struct Type<hdrtypes::option::_impl::option<exists, T>> : Type<Void>{
		template<typename TypeKey>
		using Impl = typename hdrtypes::option::option_type<TypeKey>::type;
	};

	template<typename T>
	struct Printer<hdrtypes::option::some<T>>{
		static void print(){
			printf("%s", "Some<");
			Printer<T>::print();
			printf("%s", ">");
		}
	};
	template<>
	struct Printer<hdrtypes::option::none>{
		static void print(){
			printf("%s", "None");
		}
	};
}
