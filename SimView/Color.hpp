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
	class FColor
	{
	public:
		float r;
		float g;
		float b;
		float a;

		static FColor Convert(Color color)
		{
			return { color.r / 255.f,color.g / 255.f ,color.b / 255.f ,color.a / 255.f };
		}

		static FColor Black(float alpha)	{ return { 0,  0,  0, alpha }; };
		static FColor White(float alpha)	{ return { 1,  1,  1, alpha }; };
		static FColor Red(float alpha)		{ return { 1,  0,  0, alpha }; };
		static FColor Green(float alpha)	{ return { 0,  1,  0, alpha }; };
		static FColor Blue(float alpha)		{ return { 0,  0,  1, alpha }; };
	};
}