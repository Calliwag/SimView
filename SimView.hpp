#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SSE2
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

namespace SimView
{
	typedef std::uint8_t u8;
	typedef std::uint8_t u16;
	typedef std::uint8_t u32;
	typedef std::int8_t i8;
	typedef std::int8_t i16;
	typedef std::int8_t i32;

	class Core
	{
	public:
		static void Init();
		static void DeInit();
	};

	class Color
	{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		static Color Black(float alpha) { return { 0,  0,  0,unsigned char(alpha * 255) }; };
		static Color White(float alpha) { return { 255,255,255,unsigned char(alpha * 255) }; };
		static Color Red(float alpha) { return { 255,  0,  0,unsigned char(alpha * 255) }; };
		static Color Green(float alpha) { return { 0,255,  0,unsigned char(alpha * 255) }; };
		static Color Blue(float alpha) { return { 0,  0,255,unsigned char(alpha * 255) }; };
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

		static FColor Black(float alpha) { return { 0,  0,  0, alpha }; };
		static FColor White(float alpha) { return { 1,  1,  1, alpha }; };
		static FColor Red(float alpha) { return { 1,  0,  0, alpha }; };
		static FColor Green(float alpha) { return { 0,  1,  0, alpha }; };
		static FColor Blue(float alpha) { return { 0,  0,  1, alpha }; };
	};

	class Bitmap
	{
	public:
		Color* data = nullptr;
		int width;
		int height;

		Bitmap(int width, int height, Color* data);
		int GetIndex(int x, int y) const;

		static Bitmap GetColorImage(int width, int height, Color color);
		static Bitmap FromFile(std::string path);
		Color GetPixel(int x, int y) const; 
		void SetPixel(int x, int y, Color color);
		void DrawBitmap(int oX, int oY, const Bitmap& img);
		~Bitmap();
	};

	class TextureArray
	{
	public:
		GLuint id;

		TextureArray();
		TextureArray(int width, int height, int layers, int mipLevel);
		void LayerFromBitmap(Bitmap& image, int layer);
		void GenMipmaps(float bias);
	};

	class Texture
	{
	public:
		GLuint id;

		Texture();
		Texture(GLuint id) : id(id) {};
		Texture(int width, int height, Color* data);
		void GenMipmaps(int maxLod, float bias);

		static Texture FromBitmap(Bitmap& image);
		static Texture FromTextureArray(TextureArray& texArray, int layer);
	};

	template <typename T>
	class VArray
	{
	public:
		GLuint id;
		int count;
		int elemSize;
		bool hasArray;

		VArray()
		{
			this->hasArray = false;
		}
		VArray(int elemCount, int elemSize, T* data)
		{
			glGenBuffers(1, &id);
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferData(GL_ARRAY_BUFFER, elemCount* elemSize * sizeof(T), data, GL_DYNAMIC_DRAW);
			this->count = elemCount;
			this->elemSize = elemSize;
			this->hasArray = true;
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		// Copy constructor
		VArray(const VArray& other)
		{
			if (!other.hasArray)
			{
				this->id = 0;
				this->count = 0;
				this->elemSize = 0;
				this->hasArray = false;
				return;
			}

			glGenBuffers(1, &id);
			this->count = other.count;
			this->elemSize = other.elemSize;
			this->hasArray = true;

			glBindBuffer(GL_COPY_READ_BUFFER, other.id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, id);
			glBufferData(GL_COPY_WRITE_BUFFER, count * elemSize * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, count * elemSize * sizeof(T));
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		}

		// Copy assignment
		VArray& operator=(const VArray& other)
		{
			if (this == &other)
				return *this;

			if (this->hasArray)
			{
				glDeleteBuffers(1, &id);
			}

			if (!other.hasArray)
			{
				this->id = 0;
				this->count = 0;
				this->elemSize = 0;
				this->hasArray = false;
				return *this;
			}

			glGenBuffers(1, &id);
			this->count = other.count;
			this->elemSize = other.elemSize;
			this->hasArray = true;

			glBindBuffer(GL_COPY_READ_BUFFER, other.id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, id);
			glBufferData(GL_COPY_WRITE_BUFFER, count * elemSize * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, count * elemSize * sizeof(T));
			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

			return *this;
		}

		// Move constructor
		VArray(VArray&& other) noexcept
		{
			this->id = other.id;
			this->count = other.count;
			this->elemSize = other.elemSize;
			this->hasArray = other.hasArray;

			other.hasArray = false;
			other.id = 0;
		}

		// Move assignment
		VArray& operator=(VArray&& other) noexcept
		{
			if (this != &other)
			{
				if (this->hasArray)
				{
					glDeleteBuffers(1, &id);
				}

				this->id = other.id;
				this->count = other.count;
				this->elemSize = other.elemSize;
				this->hasArray = other.hasArray;

				other.hasArray = false;
				other.id = 0;
			}
			return *this;
		}
		~VArray()
		{
			if (hasArray)
			{
				glDeleteBuffers(1, &id);
			}
		}

		void Set(int index, int elemCount, T* data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferSubData(GL_ARRAY_BUFFER, index * elemSize * sizeof(T), elemCount * elemSize * sizeof(T), data);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		void Destroy()
		{
			if (hasArray)
			{
				glDeleteBuffers(1, &id);
			}
			hasArray = false;
		}
	};

	class IndexArray
	{
	public:
		GLuint id;
		int count;
		bool hasArray;

		IndexArray();
		IndexArray(int elemCount, int* data);
		IndexArray(IndexArray& other);
		IndexArray& operator=(IndexArray&& other);
		~IndexArray();

		void Set(int index, int elemCount, int* data);
		void Destroy();
	};

	class ShaderProgram
	{
	public:
		GLuint id;
		std::map<std::string, GLint> varLocs;
		bool instanced;
		int instanceCount;
		bool hasIndexArray;

		ShaderProgram();
		ShaderProgram(const char* vertexSource, const char* fragmentSource, bool instanced, std::vector<std::string> uniforms, std::vector<std::string> attribs);
		void BindProgram();
		void Draw(GLenum drawCall, GLint index, GLsizei count);


		// Binding functions

		GLint GetVarLoc(std::string name);

		void BindArray(VArray<float>& array, GLint loc);
		void BindArray(VArray<int>& array, GLint loc);
		void BindTexture(const Texture& texture);
		void BindTextureArray(const TextureArray& textureArray);
		void BindIndexArray(IndexArray& array);
		void UnbindIndexArray();

		void SetInstanceCount(int count);
		void SetArrayDivisor(int divisor, GLint loc);

		void BindColor(Color color, GLint loc);
		void BindMat2x2(glm::mat2x2 matrix, GLint loc);
		void BindMat3x3(glm::mat3x3 matrix, GLint loc);
		void BindMat4x4(glm::mat4x4 matrix, GLint loc);
		void BindVec2(glm::vec2 vector, GLint loc);
		void BindVec3(glm::vec3 vector, GLint loc);


		// Rendering functions

		void RenderTri(int index = 0);
		void RenderTris(int count, int index = 0);
		void RenderQuad(int index = 0);
		void RenderLine(int index = 0);
		void RenderLines(int count, int index = 0);
		void RenderPolyline(int count, int index = 0);
		void RenderPoints(int count, int index = 0);
	};

	enum class BlendMode
	{
		Default,
		Alpha,
		PreMultAlpha,
		Add,
	};

	class Window
	{
	public:
		GLFWwindow* windowPtr;

		GLuint shaderProgram_flat;
		GLuint shaderProgram_texture;
		GLuint shaderProgram_instanceTexture;
		ShaderProgram* currentShader;
		GLuint VAO;
		int width;
		int height;
		double frameStartTime;
		double frameTime;

		// Inputs
		std::map<int, bool> keyDown;
		std::map<int, bool> lastKeyDown;
		std::map<int, bool> mButtonDown;
		std::map<int, bool> lastMButtonDown;
		glm::vec2 mousePos;
		glm::vec2 mouseDelta;
		glm::vec2 scrollDelta;

		Window(int width, int height, std::string title);
	public:

		// Window functions

		static Window Create(int width, int height, std::string title);
		bool ShouldClose();
		void BeginContext();
		void EndContext();
		void BeginFrame();
		void EndFrame();
		void PollEvents();
		void Destroy();


		// Settings functions

		void SetBlendMode(BlendMode mode);
		void SetLineWidth(int width);
		void SetPointSize(int size);


		// Input functions

		bool IsKeyPressed(int glfwKey);
		bool IsKeyDown(int glfwKey);
		bool IsMouseButtonPressed(int glfwMouseButton);
		bool IsMouseButtonDown(int glfwMouseButton);
		glm::vec2 GetScrollDelta();
		void LockMouse();
		void UnlockMouse();


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();
	};
}