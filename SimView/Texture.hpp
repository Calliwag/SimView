#pragma once

#include "Includes.hpp"
#include "Bitmap.hpp"

namespace SimView
{
	class Texture
	{
	public:
		GLuint id;
		int width;
		int height;

		Texture() {};
		Texture(int width, int height, Color* data);

		static Texture FromBitmap(Bitmap& image);
	};
}