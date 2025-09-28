#include <iostream>
#include "../SimView/SimView.hpp"

using namespace SimView;

int main()
{
    std::cout << "Hello World!\n";

	Core::Init();
	Window window = Window::Create(400, 400, "test_window");
	window.BeginContext();

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
	glm::vec2 lines[] = {
		{100,100},
		{300,300},
		{100,300},
		{300,100},
		{100,100},
	};

	auto triArray1 = vArray::Init(3, tri1);
	auto triArray2 = vArray::Init(3, tri2);
	auto linesArray = vArray::Init(5, lines);

	int frame = 0;
	while (!window.ShouldClose())
	{
		frame++;

		window.BeginFrame();

		window.PollEvents();
		window.FillScreen(Color::Black(1.0));

		window.SetBlendMode(BlendMode::Add);

		window.SetRenderColor(Color::Red(0.5));
		window.RenderTriFull2D(triArray1);

		window.SetRenderColor(Color::Green(1.0));
		window.RenderTriFull2D(triArray2);

		window.SetBlendMode(BlendMode::Default);
		window.SetLineWidth(3);

		window.SetRenderColor(Color::Blue(1.0));
		window.RenderLines2D(linesArray);

		window.EndFrame();

		glm::vec2 newV[] = { {frame / 100.0,400 - frame / 100.0} };
		triArray1.Set(0, 1, newV);

		std::cout << "FPS: " << window.GetFPS() << std::endl;
	}

	window.EndContext();
	window.Destroy();
	Core::DeInit();
}