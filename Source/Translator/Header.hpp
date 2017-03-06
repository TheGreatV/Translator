#pragma once


#include <string>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <algorithm>


namespace Translator
{
	// Usings
	using Size = std::size_t;
	using String = std::string;
	template<class First, class Second> using Pair = std::pair<First, Second>;

	template<class Type> using Memory = Type*;
	template<class Type> using Pointer = std::unique_ptr<Type>;
	template<class Type> using Link = std::weak_ptr<Type>;
	template<class Type> using Reference = std::shared_ptr<Type>;

	template<class Type, Size Size> using Array = std::array<Type, Size>;
	template<class Type> using Vector = std::vector<Type>;
	template<class Type> using List = std::list<Type>;
	template<class Key, class Value> using Dictionary = std::map<Key, Value>;


	// Types
	template<class Type> class This;
	template<class Type> class Referenceable;

	class Exception;

	class Name;


	// Functions
	template<class To, class From> inline Reference<To> Cast(const Reference<From>& from_);
	template<class To, class From> inline Reference<const To> Cast(const Reference<const From>& from_);
	template<class To, class From> inline Reference<To> UpCast(const Reference<From>& from_);
	template<class To, class From> inline Reference<const To> UpCast(const Reference<const From>& from_);

	template<class Type> inline typename std::remove_reference<Type>::type&& Move(Type&& type_);
	template<class Type> inline Type&& Forward(typename std::remove_reference<Type>::type& type_);

	template<class Type> inline Pointer<Type> WrapPointer(const Type* type_);
	template<class Type, class...Arguments> inline Pointer<Type> MakePointer(Arguments&&...arguments_);

	template<class Type> inline Link<Type> MakeLink(const Reference<Type>& reference_);

	template<class Type> inline Reference<Type> WrapReference(Type* type_);
	template<class Type> inline Reference<const Type> WrapConstReference(const Type* type_);
	template<class Type> inline Reference<Type> MakeReference(const Link<Type>& link_);
	template<class Type, class...Arguments> inline Reference<Type> MakeReference(Arguments&&...arguments_);

	template<class Type> inline Memory<Type> AllocateMemory();
	template<class Type> inline void ReleaseMemory(const Memory<Type>& memory_);

	template<class Type, class...Arguments> inline Reference<Type> Make(Arguments&&...arguments_);


	template<class Basic> class SmartDestructor:
		public Basic
	{
		template<class Type, class...Arguments> friend inline Reference<Type> Make(Arguments&&...arguments_);
	protected:
		bool isConstructed;
	};


	// Classes definition
	template<class Type> class This
	{
	protected:
		Link<Type> self;
	public:
		inline This(const Reference<Type>& this_);
	public:
		template<class Child> inline Reference<Child> GetThis() const;
		inline Reference<Type> GetThis() const;
	};
	template<class Type> class Referenceable:
		public This<Referenceable<Type>>,
		public Type
	{
	public:
		template<class...Arguments> inline Referenceable(const Reference<Referenceable>& this_, Arguments&&...arguments_);
		// ???
	};

	class Exception
	{
	public:
		using Text = String;
	protected:
		const Text text = "Unknown exception";
	public:
		inline Exception() = default;
		inline Exception(const Text& errorText_);
		inline Exception(const Exception&) = default;
		inline ~Exception() = default;
	public:
		inline Exception& operator = (const Exception& source) = delete;
	public:
		inline Text GetText() const;
	};
	
	class Named
	{
	public:
		using Name = String;
	protected:
		const Name name = "";
	public:
		inline Named() = default;
		inline explicit Named(const Name& name_);
		inline Named(const Named&) = default;
		inline ~Named() = default;
		Named& operator = (const Named&) = delete;
	public:
		inline Name GetName() const;
	};
}


#pragma region Translator

#pragma region This

template<class Type>
inline Translator::This<Type>::This(const Reference<Type>& this_):
	self(this_)
{
}

template<class Type> template<class Child>
inline Translator::Reference<Child> Translator::This<Type>::GetThis() const
{
	return Cast<Child>(MakeReference(self));
}

template<class Type>
inline Translator::Reference<Type> Translator::This<Type>::GetThis() const
{
	return MakeReference(self);
}

#pragma endregion

#pragma region Referenceable

template<class Type> template<class...Arguments>
inline Translator::Referenceable<Type>::Referenceable(const Reference<Referenceable>& this_, Arguments&&...arguments_):
	This(this_),
	Type(std::forward(arguments_))
{
}

#pragma endregion

#pragma region Exception

inline Translator::Exception::Exception(const Text& errorText_):
	text(errorText_)
{
}
inline Translator::Exception::Text Translator::Exception::GetText() const
{
	return text;
}

#pragma endregion

#pragma region Named

inline Translator::Named::Named(const Name& name_):
	name(name_)
{
}
inline Translator::Named::Name Translator::Named::GetName() const
{
	return name;
}

#pragma endregion


template<class To, class From>
inline Translator::Reference<To> Translator::Cast(const Reference<From>& from_)
{
	return std::static_pointer_cast<To>(from_);
}

template<class To, class From>
inline Translator::Reference<const To> Translator::Cast(const Reference<const From>& from_)
{
	return std::static_pointer_cast<const To>(from_);
}

template<class To, class From>
inline Translator::Reference<To> Translator::UpCast(const Reference<From>& from_)
{
	return std::dynamic_pointer_cast<To>(from_);
}

template<class To, class From>
inline Translator::Reference<const To> Translator::UpCast(const Reference<const From>& from_)
{
	return std::dynamic_pointer_cast<const To>(from_);
}


template<class Type>
inline typename std::remove_reference<Type>::type&& Translator::Move(Type&& type_)
{
	return std::move(type_);
}

template<class Type> inline Type&& Translator::Forward(typename std::remove_reference<Type>::type& type_)
{
	return std::forward<Type>(type_);
}


template<class Type>
inline Translator::Pointer<Type> Translator::WrapPointer(const Type* type_)
{
	return Move(Pointer<Type>(type_));
}

template<class Type, class...Arguments>
inline Translator::Pointer<Type> Translator::MakePointer(Arguments&&...arguments_)
{
	return Move(std::make_unique<Type>(Forward<Arguments>(arguments_)...));
}

template<class Type>
inline Translator::Link<Type> Translator::MakeLink(const Reference<Type>& reference_)
{
	return Move(Link<Type>(reference_));
}

template<class Type>
inline Translator::Reference<Type> Translator::WrapReference(Type* type_)
{
	return Move(Reference<Type>(type_));
}

template<class Type>
inline Translator::Reference<const Type> Translator::WrapConstReference(const Type* type_)
{
	return Move(Reference<const Type>(type_));
}

template<class Type>
inline Translator::Reference<Type> Translator::MakeReference(const Link<Type>& link_)
{
	return Move(Reference<Type>(link_));
}

template<class Type, class...Arguments>
inline Translator::Reference<Type> Translator::MakeReference(Arguments&&...arguments_)
{
	return Move(std::make_shared<Type>(std::forward<Arguments>(arguments_)...));
}


template<class Type>
inline Translator::Memory<Type> Translator::AllocateMemory()
{
	auto rawMemory = std::malloc(sizeof(Type));
	auto memory = static_cast<Memory<Type>>(rawMemory);
	return memory;
}

template<class Type>
inline void Translator::ReleaseMemory(const Memory<Type>& memory_)
{
	std::free(memory_);
}


template<class Type, class...Arguments>
inline Translator::Reference<Type> Translator::Make(Arguments&&...arguments_)
{
	auto holder = AllocateMemory<SmartDestructor<Type>>();
	holder->isConstructed = false;

	auto shared = Move(Reference<Type>(static_cast<Type*>(holder), [](Type* memory_){
		auto holder = static_cast<SmartDestructor<Type>*>(memory_);
		
		if(holder->isConstructed)
		{
			holder->~SmartDestructor<Type>();
		}

		ReleaseMemory(holder);
	}));

	new(holder) Type(shared, Forward<Arguments>(arguments_)...);
	holder->isConstructed = true;

	return Move(shared);
}


#pragma endregion


#pragma region
#pragma endregion

