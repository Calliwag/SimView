#include "CPUTexture.hpp"
#include <algorithm>

namespace SimView
{
	CPUTexture::~CPUTexture()
	{
		delete[] data;
	}
	int CPUTexture::GetIndex(int x, int y)
	{
		return y * width + x;
	}
	CPUTexture CPUTexture::GetColorImage(int width, int height, Color color)
	{
		Color* data = new Color[width * height];
		std::fill_n(data, width * height, color);
		return CPUTexture(width,height,data);
	}
	Color CPUTexture::GetPixel(int x, int y)
	{
		return data[GetIndex(x,y)];
	}
	void CPUTexture::SetPixel(int x, int y, Color color)
	{
		data[GetIndex(x, y)] = color;
	}
}