#include "CppUnitTest.h"
#include "../SimView/SimView.hpp"
#include "bullshit.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SimView;

namespace Test
{
	TEST_CLASS(TestCore)
	{
	public:
		TEST_METHOD(InitCore)
		{
			Core::Init();
		}
		TEST_METHOD(DeInitCore)
		{
			Core::Init();
			Core::DeInit();
		}
	};

	TEST_CLASS(TestWindow)
	{
	public:
		TEST_METHOD(InitWindow)
		{
			Core::Init();
			Window window = Window::Create(100, 100, "test_window");
			window.Destroy();
			Core::DeInit();
		}
	};
	TEST_CLASS(TestRenderer)
	{
		TEST_METHOD(RenderWindow)
		{
			Core::Init();
			Window window = Window::Create(400, 400, "test_window");

			glm::vec2 tri1[] = {
				{  0,400},
				{400,  0},
				{400,400}
			};
			glm::vec2 tri2[] = {
				{  0,400},
				{400,400},
				{  0,  0}
			};
			window.BeginContext();
			auto triArray1 = vArray::Init(3, tri1);
			auto triArray2 = vArray::Init(3, tri2);

			window.BeginFrame();

			window.PollEvents();
			window.FillScreen(Color::Black(1.0));

			window.SetBlendMode(BlendMode::Add);

			window.SetRenderColor(Color::Red(0.5));
			window.RenderTriFull2D(triArray1);

			window.SetRenderColor(Color::Green(1.0));
			window.RenderTriFull2D(triArray2);

			window.EndFrame();

			window.EndContext();
			window.Destroy();
			Core::DeInit();
		}
	};
	TEST_CLASS(TestCPUTexture)
	{
	public:
		TEST_METHOD(InitCPUTexture)
		{
			CPUTexture texture = CPUTexture::GetColorImage(100, 100, Color::Black(1.0));
		}
		TEST_METHOD(ReadWriteCPUTexture)
		{
			CPUTexture texture = CPUTexture::GetColorImage(100, 100, Color::Black(1.0));
			texture.SetPixel(5, 5, Color::Red(1.0));
			Assert::AreEqual(texture.GetPixel(5, 5), Color::Red(1.0));
		}
	};
}
