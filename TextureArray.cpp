#include "SimView.hpp"

namespace SimView
{
	TextureArray::TextureArray()
	{
		id = -1;
	}
	TextureArray::TextureArray(int width, int height, int layers, int mipLevel)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevel, GL_RGBA8, width, height, layers);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
	void TextureArray::LayerFromBitmap(Bitmap& image, int layer)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, image.width, image.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
	void TextureArray::GenMipmaps(float bias)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, bias);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}
}