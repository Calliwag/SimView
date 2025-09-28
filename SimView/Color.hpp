#pragma once

namespace SimView
{
	class Color
	{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		static Color Black(float alpha)	{ return { 0,  0,  0,unsigned char(alpha * 255) }; };
		static Color White(float alpha)	{ return { 255,255,255,unsigned char(alpha * 255) }; };
		static Color Red(float alpha)	{ return { 255,  0,  0,unsigned char(alpha * 255) }; };
		static Color Green(float alpha)	{ return { 0,255,  0,unsigned char(alpha * 255) }; };
		static Color Blue(float alpha)	{ return { 0,  0,255,unsigned char(alpha * 255) }; };
	};
}