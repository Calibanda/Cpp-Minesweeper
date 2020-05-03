#include "../include/IrretrievableResourceException.hpp"

using namespace std::string_literals;

IrretrievableResourceException::IrretrievableResourceException(std::string_view const& pathIrretrivableResource) : errorMessage{ "Failed to load file: "s + pathIrretrivableResource.data() } {}

const char* IrretrievableResourceException::what() const noexcept
{
	return errorMessage.c_str(); //Convert string to char*
}
