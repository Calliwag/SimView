#pragma once

#include "Color.hpp"

namespace SimView
{
	class Bitmap
	{
	public:
		Color* data = nullptr;

		Bitmap(int width, int height, Color* data)
		{
			this->width = width;
			this->height = height;
			this->data = data;
		}
		int GetIndex(int x, int y);
	public:
		int width;
		int height;

		static Bitmap GetColorImage(int width, int height, Color color);
		Color GetPixel(int x, int y);
		void SetPixel(int x, int y, Color color);
		~Bitmap();
	};
}