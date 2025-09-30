#include <iostream>
#include "../SimView/SimView.hpp"
#include <random>

using namespace SimView;

void main()
{
    std::cout << "Hello World!\n";

	try
	{
		Core::Init();
		Window window = Window::Create(400, 400, "test_window");
		window.BeginContext();

		glm::vec2 rect[] = {
			{-1,-1},
			{-1, 1},
			{ 1, 1},
			{ 1,-1},
		};
		glm::vec2 rectUV[] = {
			{0,0},
			{0,1},
			{1,1},
			{1,0},
		};
		glm::vec2 tri[] = {
			{ -1.0, -1.0 },
			{ 1 + sqrt(2.0),-1.0 },
			{-1.0, 1 + sqrt(2.0) },
		};
		glm::vec2 triUV[] = {
			{ 0.0, 0.0 },
			{ 0.0, 1.0 + sqrt(0.5) },
			{ 1.0 + sqrt(0.5), 0.0 },
		};

		float scale = 5.0;
		for (int i = 0; i < 4; i++)
		{
			rect[i] = scale * rect[i];
		}
		for (int i = 0; i < 3; i++)
		{
			tri[i] = scale * tri[i];
		}

		int count = 1000000;
		glm::vec2* inst = new glm::vec2[count];
		std::mt19937 rd(0);
		std::uniform_real_distribution<float> dX(0, window.width);
		std::uniform_real_distribution<float> dY(0, window.height);
		for (int i = 0; i < count; i++)
		{
			inst[i] = { dX(rd),dY(rd) };
		}

		auto rectArray = vArray::Init(4, rect);
		auto rectUVArray = vArray::Init(4, rectUV);
		auto triArray = vArray::Init(3, tri);
		auto triUVArray = vArray::Init(3, triUV);
		auto instArray = vArray::Init(count, inst);

		float radius = 100;
		Bitmap image = Bitmap::GetColorImage(2 * radius, 2 * radius, Color::Black(0.0f));
		for (int x = 0; x < 2 * radius; x++)
			for (int y = 0; y < 2 * radius; y++)
			{
				if (pow(x - radius, 2) + pow(y - radius, 2) < radius * radius)
				{
					image.SetPixel(x, y, Color::White(0.1f));
				}
			}
		auto texture = Texture::FromBitmap(image);

		auto instTexShader = window.GetInstTexShader();
		window.SetShader(instTexShader);
		window.SetBlendMode(BlendMode::Alpha);
		instTexShader.SetRenderColor(Color::White(1.0));
		instTexShader.BindPosArray(triArray);
		instTexShader.BindUVArray(triUVArray);
		instTexShader.BindInstanceArray(instArray);
		instTexShader.BindTexture(texture);

		int frame = 0;
		while (!window.ShouldClose())
		{
			frame++;

			window.BeginFrame();
			instTexShader.SetTransformMatrix(window.viewMatrix);
			window.PollEvents();
			window.FillScreen(Color::Black(1.0));

			window.RenderTri();
			//window.RenderQuad();
			//window.SetPointSize(10);
			//window.RenderPoints();

			window.EndFrame();

			if(frame % 100 == 0)
				std::cout << "FPS: " << window.GetFPS() << std::endl;
		}

		window.EndContext();
		window.Destroy();
		Core::DeInit();

	}
	catch (std::runtime_error& err)
	{
		std::cout << err.what() << "\n";
		return;
	}
}