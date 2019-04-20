#pragma once
typedef unsigned char BYTE;

class Color {
public:
	Color();
	Color(unsigned int val);
	Color(BYTE r, BYTE g, BYTE b);
	Color(BYTE r, BYTE g, BYTE b, BYTE a);
	Color(const Color& src);

	Color& operator=(const Color& src);
	bool operator==(const Color& rhs) const;
	bool operator!=(const Color& rhs) const;

	constexpr BYTE GetR() const;
	void SetR(BYTE r);

	constexpr BYTE GetG() const;
	void SetG(BYTE g);

	constexpr BYTE GetB() const;
	void SetB(BYTE b);

	constexpr BYTE GetA() const;
	void SetA(BYTE a);

private:
	union {
		BYTE rgba[4];
		unsigned int color;
	};

};

namespace Colors {
	const Color UnloadedTextureColor(100, 100, 100);
	const Color UnhandledTextureColor(250, 0, 0);

	const Color White(255, 255, 255, 255);
	const Color Black(0, 0, 0, 255);
	const Color Red(255, 0, 0, 255);
	const Color Blue(0, 255, 0, 255);
	const Color Green(0, 0, 255, 255);
	const Color Yellow(255, 255, 0, 255);
	const Color Cyan(0, 255, 255, 255);
	const Color Magenta(255, 0, 255, 255);
}