#include <iostream>
#include <variant>
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



int main(int argc, char** argv)
{
	test_multiop();
	

	

	return 0;
}