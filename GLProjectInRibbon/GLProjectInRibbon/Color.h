#pragma once
class Color
{
public:
	Color();
	Color(float r, float g, float b) : R(r), G(g), B(b) {};
	Color(const Color& color){
		R = color.R;
		G = color.G;
		B = color.B;
	};
	Color& operator=(const Color& pColor)
	{
		R = pColor.R;
		G = pColor.G;
		B = pColor.B;
		return *this;
	};
	~Color();

public:
	void SetColor(float r, float g, float b);

public:
	float R;
	float G;
	float B;
};

