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

		GLuint shaderProgram;
		GLuint VAO;
		GLint transMatLoc;
		GLint renderColorLoc;
		GLint vertexPosLoc;
		int width;
		int height;
		double frameStartTime;
		double frameEndTime;
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
		void BindVArray(vArray& array);


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

		void RenderTri(vArray& array, int index = 0);
		void RenderLine(vArray& array, int index = 0);
		void RenderLines(vArray& array, int index = 0, int count = 0);
		void RenderPolyline(vArray& array, int index = 0, int count = 0);
		void RenderPoints(vArray& array, int index = 0, int count = 0);
	};
}