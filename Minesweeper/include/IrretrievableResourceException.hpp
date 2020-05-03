#ifndef IRRETRIEVABLERESOURCEEXCEPTION_HPP
#define IRRETRIEVABLERESOURCEEXCEPTION_HPP
#pragma once

#include <exception>
#include <string_view>
#include <string>

class IrretrievableResourceException : public std::exception
{
public:
	IrretrievableResourceException(std::string_view const& pathIrretrivableResource);
	virtual const char* what() const noexcept override;

private:
	std::string errorMessage{};
};

#endif // !IRRETRIEVABLERESOURCEEXCEPTION_HPP
