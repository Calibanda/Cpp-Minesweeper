#ifndef COORDINATES_HPP
#define COORDINATES_HPP
#pragma once

struct Coordinates
{
	explicit inline Coordinates() : i{ 0 }, j{ 0 } {};
	explicit inline Coordinates(unsigned short int const _i, unsigned short int const _j) : i{ _i }, j{ _j } {};
	unsigned short int i;
	unsigned short int j;
};

#endif // !COORDINATES_HPP
