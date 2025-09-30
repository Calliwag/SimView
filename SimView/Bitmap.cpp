#include "Bitmap.hpp"
#include <algorithm>

namespace SimView
{
	Bitmap::~Bitmap()
	{
		delete[] data;
	}
	int Bitmap::GetIndex(int x, int y)
	{
		return y * width + x;
	}
	Bitmap Bitmap::GetColorImage(int width, int height, Color color)
	{
		Color* data = new Color[width * height];
		std::fill_n(data, width * height, color);
		return Bitmap(width,height,data);
	}
	Color Bitmap::GetPixel(int x, int y)
	{
		return data[GetIndex(x,y)];
	}
	void Bitmap::SetPixel(int x, int y, Color color)
	{
		data[GetIndex(x, y)] = color;
	}
}