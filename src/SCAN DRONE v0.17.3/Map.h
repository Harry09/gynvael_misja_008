#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Vertex.hpp>

#include <vector>
#include <memory>

namespace sf
{
	class RenderWindow;
	class Image;
	class Texture;
	class Sprite;
}

class Map
{
private:
	std::shared_ptr<sf::Image> m_pMap;
	std::shared_ptr<sf::Texture> m_pMapTexture;
	std::shared_ptr<sf::Sprite>	m_pMapSprite;

public:
	Map();
	~Map() = default;

	void AddPoint(const sf::Vector2f &dronePos, int angle, float length);
	void RefreshTexture();

	void LoadAllScans(const char *folderLocation);
	void SaveAsImage(const char *imagePath);

	void Render(sf::RenderWindow *window);
};

