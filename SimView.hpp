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

		Texture();;
		Texture(int width, int height, Color* data);

		static Texture FromBitmap(Bitmap& image);
	};

	class VArray
	{
	public:
		GLuint id;
		int count;
		int elemSize;
		bool hasArray;

		VArray();
		VArray(int elemCount, int elemSize, void* data);
		VArray(VArray& other);
		VArray& operator=(VArray&& other);
		~VArray();

		void Set(int index, int elemCount, void* data);
		void Destroy();
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

		void BindArray(VArray& array, std::string name);
		void BindColor(Color& color, std::string name);
		void BindInstanceArray(VArray& array, std::string name);
		void BindTexture(Texture& texture);
		void BindMat3x3(glm::mat3x3& matrix, std::string name);
		void BindMat4x4(glm::mat4x4& matrix, std::string name);


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
		std::map<int, bool> keyDown;
		std::map<int, bool> lastKeyDown;

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


		// Input functions

		bool IsKeyPressed(int glfwKey);
		bool IsKeyDown(int glfwKey);


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();
	};
}