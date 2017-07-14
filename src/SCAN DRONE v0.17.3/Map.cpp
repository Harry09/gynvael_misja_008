#include "Map.h"

#include "Common.h"
#include "ScanData.h"
#include "App.h"

#include <SFML\Graphics.hpp>

#include <filesystem>
#include <cstdlib>

Map::Map()
{
	m_pMap = std::make_shared<sf::Image>();

	m_pMap->create(1000 * App::PIXEL_SCALE, 1000 * App::PIXEL_SCALE, sf::Color::White);

	m_pMapTexture = std::make_shared<sf::Texture>();
	m_pMapTexture->loadFromImage(*m_pMap);

	m_pMapSprite = std::make_shared<sf::Sprite>(*m_pMapTexture);
}

void Map::AddPoint(const sf::Vector2f &dronePos, int angle, float length)
{
	// -1 == inf
	if (length != -1)
	{
		auto wallPos = dronePos + LenDir(length, static_cast<float>(angle));

		m_pMap->setPixel(static_cast<unsigned int>(wallPos.x * App::PIXEL_SCALE), static_cast<unsigned int>(wallPos.y * App::PIXEL_SCALE), sf::Color::Black);
	}
}

void Map::RefreshTexture()
{
	m_pMapTexture->update(*m_pMap);
}

void Map::LoadAllScans(const char *folderLocation)
{
	printf("loading...\n");

	for (auto& file : std::experimental::filesystem::directory_iterator(folderLocation))
	{
		ScanData scan;

		std::wstring wstr = file.path();

		std::string str(wstr.begin(), wstr.end());

		int backslashOffset = static_cast<int>(str.find_first_of("\\"));

		scan.Load(str.substr(backslashOffset, str.size() - backslashOffset).c_str(), str.substr(0, backslashOffset).c_str());

		auto lengths = scan.GetLengths();
		auto dronePos = scan.GetPos();

		for (auto& length : *lengths)
		{
			AddPoint(sf::Vector2f(static_cast<float>(dronePos.x), static_cast<float>(dronePos.y)), length.angle, length.length);
		}
	}

	printf("loaded!\n");

	RefreshTexture();
}

void Map::SaveAsImage(const char *imagePath)
{
	printf("Saving to '%s'...\n", imagePath);

	if (m_pMap->saveToFile(imagePath))
	{
		printf("Saved!\n");
	}
	else

	{
		printf("Cannot save!\n");
	}
}

void Map::Render(sf::RenderWindow *window)
{
	if (m_pMapSprite)
		window->draw(*m_pMapSprite);
}
