#pragma once

#include <string>
#include <vector>

#include <SFML\System\Vector2.hpp>

class ScanData
{
public:
	class Length
	{
	public:
		int angle;
		float length;
	};

	enum Direction
	{
		NORTH = 0,
		EAST,
		SOUTH,
		WEST
	};

private:
	std::string					m_name;

	sf::Vector2f				m_pos;
	std::vector<Length>			m_length;

	std::string					m_direction[4];

	static char					s_baseURL[];

private:
	bool Parse(const char *filePath);

public:
	ScanData() = default;
	~ScanData() { }

	bool Load(const char *scanName, const char *baseDir = "scans\\");

	const auto GetLengths() const { return &m_length; }
	const sf::Vector2f& GetPos() const { return m_pos; }
	const std::string GetMove(Direction direction);
};

