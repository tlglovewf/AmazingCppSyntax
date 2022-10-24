#include <iostream>
#include <variant>
#include <string>
using namespace std;

#pragma  region multiop
template<typename ...Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};
template<typename ...Ts>
overloaded(Ts...)->overloaded<Ts...>;
auto pris = overloaded{
	[](std::string_view str) {std::cout << str << std::endl; },
	[](int v) {std::cout << v << std::endl; }
};

void test_multiop()
{
	pris("hello world");
	pris(100);
	std::variant<std::string, int> vv;
	vv = "hello";
	std::visit(pris, vv);
}

#pragma endregion

template<typename... Args>
void print(Args... args)
{
	std::initializer_list<int>
	{
		([&]()->int
		{
			cout << args << endl;
			return 0;
		}())...
	};
}

struct Te
{
	Te() { std::cout << "te" << std::endl; }
	int pt() { return 10; }
};

#pragma region SFINAE

struct ClassA
{
	int get_id()const
	{
		return 10;
	}
};

struct ClassB
{
	std::string get_name()const
	{
		return "hello";
	}
};

template<class T, class = void>
struct has_get_name : std::false_type {};

template<class T>
struct has_get_name<T, std::void_t<decltype(std::declval<T>().get_name())>> : std::true_type {};

template<typename T>
std::string make_name(const T& cls)
{
	if constexpr (has_get_name<T>::value)
	{
		return cls.get_name();
	}
	else
	{
		return std::to_string(cls.get_id());
	}
}
template<typename T>
std::string get_value(const T& v)
{
	if constexpr(std::is_same<T, float>::value)
	{
		return "float";
	}
	else if  (std::is_same<T, ClassB>::value)
	{
		return v.get_name();
	}
	else
	{
		return "others";
	}
}

//Substitution failure is not an error  类型错误不是错误
void test_SFINAE()
{
	std::cout << has_get_name<ClassB>::value << std::endl;
	std::cout << has_get_name<ClassA>::value << std::endl;

	ClassA a;
	ClassB b;

	std::cout << make_name(a) << std::endl;
	std::cout << make_name(b) << std::endl;

	std::cout << get_value(10.3f) << std::endl;

	std::cout << get_value(b) << std::endl;
}

#pragma endregion


template<typename T>
void check(typename T::type t)
{

}

int main(int argc, char** argv)
{
	test_multiop();
	
	print("hello", "world", 1023);
	
	//declval 1.返回类型右值引用 2.在不创建对象的前提下返回函数类型
	decltype(std::declval<Te>().pt()) t;

	test_SFINAE();

	return 0;
}