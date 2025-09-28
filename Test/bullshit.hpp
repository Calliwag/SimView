#pragma once

#include "../SimView/SimView.hpp"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<SimView::Color>(const class SimView::Color& t) { return L"Color"; }
		}
	}
}

bool operator==(const SimView::Color& color1, const SimView::Color& color2)
{
	return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}