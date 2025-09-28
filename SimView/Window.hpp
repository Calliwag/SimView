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
		GLint windowSizeLoc;
		GLint renderColorLoc;
		int width;
		int height;
		double frameStartTime;
		double frameEndTime;

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

		//float* ScreenToGLVerts(glm::vec2 points[], int count);


		// Settings functions

		void SetBlendMode(BlendMode mode);
		void SetRenderColor(Color color);
		void SetLineWidth(int width);


		// Misc. functions

		void FillScreen(Color color);
		double GetFPS();


		// Rendering functions

		void RenderTriFull2D(vArray& array);
		void RenderLine2D(vArray& array);
		void RenderLines2D(vArray& array);
	};
}