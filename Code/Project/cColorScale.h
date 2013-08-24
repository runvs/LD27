#pragma once

#include <map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>

namespace InterpolationFunction 
{
	enum InterpolationFunction
	{
		Linear,
		Cosinus
	};
}

namespace GradientStyle 
{
	enum GradientStyle 
	{
		Linear,
		Circle,
		Radial,
		Reflex
	};
}

class cColorScale : protected std::map<double,sf::Color>
{
public:
	cColorScale();
	bool insert(double position, sf::Color color);
	
	void fillTab(
		sf::Color* colorTab,
		int size,
		InterpolationFunction::InterpolationFunction function = InterpolationFunction::Cosinus
	) const;

	void draw(
		sf::Image&,
		const sf::Vector2f& start,
		const sf::Vector2f& end,
		GradientStyle::GradientStyle style=GradientStyle::Linear,
		int size = 500
	) const;
};