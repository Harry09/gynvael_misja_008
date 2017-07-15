#include "App.h"

#include "ScanData.h"
#include "Map.h"

App::App()
{
	m_pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1280, 720), "SCAN DRONE v0.17.3");
	m_baseViewSize = m_pWindow->getView().getSize();

	m_pCurrentScan = std::make_unique<ScanData>();
	m_pCurrentScan->Load("68eb1a7625837e38d55c54dc99257a17.txt");
	m_pMap = std::make_shared<Map>();

	m_pDrone = std::make_shared<sf::CircleShape>(0.5f * PIXEL_SCALE);
	m_pDrone->setFillColor(sf::Color::Black);
	//m_pDrone->setOrigin(m_pDrone->getRadius(), m_pDrone->getRadius());

	MoveView(m_pCurrentScan->GetPos());
	AddScanToMap();
}

App::~App()
{
}

void App::Run()
{
	while (m_pWindow->isOpen())
	{
		sf::Event e;
		while (m_pWindow->pollEvent(e))
		{
			if (e.type == sf::Event::Closed ||
				e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			{
				m_pWindow->close();
			}

			Event(&e);
		}

		m_pWindow->clear(sf::Color::White);
		Render();
		m_pWindow->display();

		sf::sleep(sf::milliseconds(10));
	}
}


void App::AddScanToMap()
{
	auto lengths = m_pCurrentScan->GetLengths();
	auto dronePos = m_pCurrentScan->GetPos();

	for (auto& length : *lengths)
	{
		m_pMap->AddPoint(sf::Vector2f(static_cast<float>(dronePos.x), static_cast<float>(dronePos.y)), length.angle, length.length);
	}

	m_pMap->RefreshTexture();
}

void App::MoveView(const sf::Vector2f &pos)
{
	sf::Vector2f _pos(pos * static_cast<float>(PIXEL_SCALE));

	sf::View view = m_pWindow->getView();
	view.setCenter(_pos);
	m_pWindow->setView(view);
	m_pDrone->setPosition(_pos);
}

void App::Render()
{
	if (m_pMap)
		m_pMap->Render(m_pWindow.get());

	if (m_pDrone)
		m_pWindow->draw(*m_pDrone);
}

void App::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::W:
			{
				if (m_pCurrentScan)
				{
					m_pCurrentScan->Load(m_pCurrentScan->GetMove(ScanData::NORTH).c_str());

					AddScanToMap();
					MoveView(m_pCurrentScan->GetPos());
				}
			} break;
			case sf::Keyboard::S:
			{
				if (m_pCurrentScan)
				{
					m_pCurrentScan->Load(m_pCurrentScan->GetMove(ScanData::SOUTH).c_str());

					AddScanToMap();
					MoveView(m_pCurrentScan->GetPos());
				}
			} break;
			case sf::Keyboard::A:
			{
				if (m_pCurrentScan)
				{
					m_pCurrentScan->Load(m_pCurrentScan->GetMove(ScanData::WEST).c_str());

					AddScanToMap();
					MoveView(m_pCurrentScan->GetPos());
				}
			} break;
			case sf::Keyboard::D:
			{
				if (m_pCurrentScan)
				{
					m_pCurrentScan->Load(m_pCurrentScan->GetMove(ScanData::EAST).c_str());

					AddScanToMap();
					MoveView(m_pCurrentScan->GetPos());
				}
			} break;
		}
	}
	else if (e->type == sf::Event::MouseWheelMoved)
	{
		sf::View view = m_pWindow->getView();

		if (e->mouseWheel.delta > 0 && m_scale > 0.1f)
			view.zoom(0.9f);
		else if (e->mouseWheel.delta < 0)
			view.zoom(1.1f);

		m_scale = view.getSize().x / m_baseViewSize.x;

		m_pWindow->setView(view);
	}
}
