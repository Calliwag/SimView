#pragma once

#include "Color.hpp"

namespace SimView
{
	class CPUTexture
	{
	public:
		Color* data = nullptr;

		CPUTexture(int width, int height, Color* data)
		{
			this->width = width;
			this->height = height;
			this->data = data;
		}
		int GetIndex(int x, int y);
	public:
		int width;
		int height;

		static CPUTexture GetColorImage(int width, int height, Color color);
		Color GetPixel(int x, int y);
		void SetPixel(int x, int y, Color color);
		~CPUTexture();
	};
}