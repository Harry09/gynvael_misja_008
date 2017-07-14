#pragma once

#include <SFML\Graphics.hpp>

#include <memory>

class ScanData;
class Map;

class App
{
private:
	std::unique_ptr<sf::RenderWindow>		m_pWindow;

	std::unique_ptr<ScanData>				m_pCurrentScan;
	std::shared_ptr<Map>					m_pMap;

	std::shared_ptr<sf::CircleShape>		m_pDrone;

	sf::Vector2f							m_baseViewSize;
	sf::Vector2f							m_cameraPos;
	float									m_scale;

public:
	static const int						PIXEL_SCALE = 3;

private:
	void AddScanToMap();
	void MoveView(const sf::Vector2f &pos);

public:
	App();
	~App();

	void Run();

	void Render();
	void Event(sf::Event *e);
};
