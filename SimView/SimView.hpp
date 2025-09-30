#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

namespace SimView
{
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
		int GetIndex(int x, int y);

		static Bitmap GetColorImage(int width, int height, Color color);
		Color GetPixel(int x, int y);
		void SetPixel(int x, int y, Color color);
		~Bitmap();
	};

	class Texture
	{
	public:
		GLuint id;
		int width;
		int height;

		Texture();;
		Texture(int width, int height, Color* data);

		static Texture FromBitmap(Bitmap& image);
	};

	class vArray
	{
	public:
		GLuint id;
		int count;
		int elemSize;
		bool hasArray;

		vArray();
		vArray(GLuint id, int count, int elemSize);
		vArray(vArray& other);
		~vArray();

		static vArray Init(int size, glm::vec2 points[]);
		static vArray Init(int size, Color colors[]);
		static vArray Init(int size, FColor colors[]);
		void Set(int index, int size, glm::vec2 points[]);
		void Set(int index, int size, Color colors[]);
		void Set(int index, int size, FColor colors[]);
	};

	class ShaderProgram
	{
	public:
		GLuint id;
		std::map<std::string, GLint> varLocs;
		bool instanced;
		int instanceCount;

		ShaderProgram(const char* vertexSource, const char* fragmentSource, bool instanced, std::vector<std::string> uniforms, std::vector<std::string> attribs);
		void BindProgram();
		void Draw(GLenum drawCall, GLint index, GLsizei count);


		// Binding functions

		void BindArray(vArray& array, std::string name);
		void BindColor(Color& color, std::string name);
		void BindInstanceArray(vArray& array, std::string name);
		void BindTexture(Texture& texture);
		void BindMat3x3(glm::mat3x3& matrix, std::string name);


		// Rendering functions

		void RenderTri(int index = 0);
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
		glm::mat3x3 viewMatrix;

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


		// Helper functions

		glm::mat3x3 GetViewMatrix();

		// Settings functions

		void SetBlendMode(BlendMode mode);
		void SetLineWidth(int width);
		void SetPointSize(int size);


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();
	};
}