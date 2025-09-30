#pragma once

#include "Includes.hpp"
#include "Color.hpp"
#include "vArray.hpp"
#include "Texture.hpp"

namespace SimView
{
	enum class BlendMode
	{
		Default,
		Alpha,
		Add,
	};

	class ShaderProgram
	{
	public:
		GLuint id;
		bool instanced;

		GLint renderColorLoc;
		GLint transMatLoc;
		GLint vertexPosLoc;
		GLint vertexUVLoc;
		GLint instanceArrLoc;
		GLint vertexColorLoc;

		vArray* posArray = nullptr;
		vArray* uvArray = nullptr;
		Texture* texture = nullptr;
		vArray* instanceArray = nullptr;
		vArray* colorArray = nullptr;

		glm::mat3x3 currentMatrix;

		ShaderProgram(const char* vertexSource, const char* fragmentSource, bool instanced);
		void Use();
		void Draw(GLenum drawCall, GLint index, GLsizei count);
		void BindPosArray(vArray& array);
		void UnbindPosArray();

		void BindUVArray(vArray& array);
		void UnbindUVArray();

		void BindInstanceArray(vArray& array);
		void UnbindInstanceArray();

		void BindTexture(Texture& tex);
		void UnbindTexture();

		void BindColorArray(vArray& array);
		void UnbindColorArray();

		void SetRenderColor(Color color);

		void SetTransformMatrix(glm::mat3x3& matrix);
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

		void SetShader(ShaderProgram& shaderProgram);
		ShaderProgram GetFlatShader();
		ShaderProgram GetTexShader();
		ShaderProgram GetInstTexShader();
		ShaderProgram GetInstTexColorShader();


		// Settings functions

		void SetBlendMode(BlendMode mode);
		void SetLineWidth(int width);
		void SetPointSize(int size);


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();


		// Rendering functions

		void RenderTri(int index = 0);
		void RenderQuad(int index = 0);
		void RenderLine(int index = 0);
		void RenderLines(int index = 0, int count = 0);
		void RenderPolyline(int index = 0, int count = 0);
		void RenderPoints(int index = 0, int count = 0);
	};
}