#pragma once

#include "Includes.hpp"
#include "Color.hpp"
#include "vArray.hpp"

namespace SimView
{
	enum class BlendMode
	{
		Default,
		Add,
	};

	class Window
	{
	public:
		GLFWwindow* windowPtr;

		vArray* posArray = nullptr;

		GLuint shaderProgram;
		GLuint VAO;
		GLint transMatLoc;
		GLint renderColorLoc;
		GLint vertexPosLoc;
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
		void BindPosArray(vArray& array);
		void UnbindPosArray();


		// Settings functions

		void SetBlendMode(BlendMode mode);
		void SetRenderColor(Color color);
		void SetLineWidth(int width);
		void SetPointSize(int size);
		void SetTransformMatrix(glm::mat3x3& matrix);


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();


		// Rendering functions

		void RenderTri(int index = 0);
		void RenderLine(int index = 0);
		void RenderLines(int index = 0, int count = 0);
		void RenderPolyline(int index = 0, int count = 0);
		void RenderPoints(int index = 0, int count = 0);
	};
}