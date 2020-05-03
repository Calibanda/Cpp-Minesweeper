#ifndef SIZE_HPP
#define SIZE_HPP
#pragma once

struct Size
{
	explicit inline Size() : x{ 0 }, y{ 0 } {};
	explicit inline Size(unsigned short int const _x, unsigned short int const _y) : x{ _x }, y{ _y } {};
	unsigned short int x;
	unsigned short int y;
};

#endif // !SIZE_HPP
