#include "Map.h"

#include "Common.h"
#include "ScanData.h"
#include "App.h"

#include <SFML\Graphics.hpp>

#include <filesystem>
#include <cstdlib>
#include <thread>

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

		while (true)
		{
			if (m_pMapMutex.try_lock())
			{
				m_pMap->setPixel(static_cast<unsigned int>(wallPos.x * App::PIXEL_SCALE), static_cast<unsigned int>(wallPos.y * App::PIXEL_SCALE), sf::Color::Black);

				m_pMapMutex.unlock();
				break;
			}

			sf::sleep(sf::milliseconds(10));
		}
	}
}

void Map::RefreshTexture()
{
	m_pMapTexture->update(*m_pMap);
}

void Map::LoadAllScans(const char *folderLocation)
{
	printf("Loading...\n");

	std::vector<std::string> files;

	for (auto& file : std::experimental::filesystem::directory_iterator(folderLocation))
	{
		std::wstring wstr = file.path();
		std::string str(wstr.begin(), wstr.end());
		files.push_back(str);
	}

	printf("Files: %lld\n", files.size());

	int nCores = std::thread::hardware_concurrency();

	printf("Number of cores: %d\n", nCores);

	std::vector<std::thread> threads;

	int filesForCore = static_cast<int>(files.size()) / nCores;
	printf("Files per core: %d\n", filesForCore);

	for (int i = 0; i < nCores; ++i)
	{
		int start = i * filesForCore;

		int end = 0;

		if ((i + 1) == nCores)
			end = static_cast<int>(files.size());
		else
			end = (i + 1) * filesForCore;

		threads.push_back(std::thread(&Map::LoadOnThread, this, files, start, end));
	}

	for (int i = 0; i < nCores; ++i)
	{
		threads[i].join();
	}

	printf("Loaded!\n");

	RefreshTexture();
}

void Map::LoadOnThread(const std::vector<std::string>& files, int start, int end)
{
	printf("Start thread: (%d, %d)\n", start, end);

	for (int i = start; i < end; ++i)
	{
		std::string file = files.at(i);

		ScanData scan;

		int backslashOffset = static_cast<int>(file.find_first_of("\\"));

		if (scan.Load(file.substr(backslashOffset, file.size() - backslashOffset).c_str(), file.substr(0, backslashOffset).c_str()))
		{
			auto lengths = scan.GetLengths();
			auto dronePos = scan.GetPos();

			for (auto& length : *lengths)
			{
				AddPoint(sf::Vector2f(static_cast<float>(dronePos.x), static_cast<float>(dronePos.y)), length.angle, length.length);
			}
		}
	}

	printf("End thread: (%d, %d)\n", start, end);
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
