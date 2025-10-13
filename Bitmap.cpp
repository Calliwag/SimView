#include "SimView.hpp"
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SimView
{
	Bitmap::~Bitmap()
	{
		delete[] data;
	}
	Bitmap::Bitmap(int width, int height, Color* data)
	{
		this->width = width;
		this->height = height;
		this->data = data;
	}
	int Bitmap::GetIndex(int x, int y) const
	{
		return y * width + x;
	}
	Bitmap Bitmap::GetColorImage(int width, int height, Color color)
	{
		Color* data = new Color[width * height];
		std::fill_n(data, width * height, color);
		return Bitmap(width,height,data);
	}
	Bitmap Bitmap::FromFile(std::string path)
	{
		int x,y,n;
		unsigned char* data = stbi_load(path.c_str(), &x, &y, &n, 4);
		if (data == nullptr)
			throw std::runtime_error("Failed to load image " + path);
		return Bitmap(x, y, (Color*)data);
	}
	Color Bitmap::GetPixel(int x, int y) const
	{
		return data[GetIndex(x, y)];
	}
	void Bitmap::SetPixel(int x, int y, Color color)
	{
		data[GetIndex(x, y)] = color;
	}
	void Bitmap::DrawBitmap(int oX, int oY, const Bitmap& img)
	{
		for (int x = oX; x < glm::min(oX + img.width, width); x++)
			for (int y = oY; y < glm::min(oY + img.height, height); y++)
			{
				SetPixel(x, y, img.GetPixel(x - oX, y - oY));
			}
	}
}