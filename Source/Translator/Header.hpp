#pragma once


#include <string>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <algorithm>


namespace Translator
{
	using Size = std::size_t;
	using String = std::string;
	template<class T> using Reference = std::shared_ptr<T>;
	template<class T> using Vector = std::vector<T>;
	template<class T> using List = std::list<T>;
	template<class K, class V> using Dictionary = std::map<K, V>;


	template<class A, class B> Reference<A> UpCast(const Reference<B>& b)
	{
		return std::dynamic_pointer_cast<A>(b);
	}
	template<class A, class B> Reference<const A> UpCast(const Reference<const B>& b)
	{
		return std::dynamic_pointer_cast<const A>(b);
	}
	template<class T> inline T Move(T&& t)
	{
		return std::move(t);
	}
	template<class T> inline Reference<T> MakeReference(T* t)
	{
		return Reference<T>(t);
	}
	template<class T> inline Reference<const T> MakeConstReference(const T* t)
	{
		return Reference<const T>(t);
	}


	class Named
	{
	public:
		using Name = String;
	protected:
		const Name name;
	public:
		inline Named() = default;
		inline explicit Named(const Name& name_):
			name(name_)
		{
		}
		inline Named(const Named&) = default;
		inline ~Named() = default;
		Named& operator = (const Named&) = delete;
	public:
		inline Name GetName() const
		{
			return name;
		}
	};
}


#pragma region
#pragma endregion

