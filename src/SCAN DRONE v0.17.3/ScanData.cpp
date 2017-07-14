#include "ScanData.h"

#include "Common.h"

#include <fstream>
#include <sstream>
#include <Urlmon.h>

char ScanData::s_baseURL[] = "http://gynvael.coldwind.pl/misja008_drone_io/scans/";

bool ScanData::Load(const char *scanName, const char *baseDir)
{
	//printf("Loading scan '%s'...\n", scanName);

	CreateDirectory(baseDir, 0);

	std::string filePath = std::string(baseDir) + std::string(scanName);

	//printf("File path: %s\n", filePath.c_str());

	if (IsFileExist(filePath.c_str()))
	{
		Parse(filePath.c_str());
	}
	else
	{
		if (strcmp(scanName, "not possible") == 0)
			return false;

		std::string url = s_baseURL + std::string(scanName);

		printf("Downloading scan from '%s'...\n", url.c_str());

		int result = URLDownloadToFile(0, url.c_str(), filePath.c_str(), 0, 0);

		if (result == S_OK)
		{
			//printf("Downloaded!\n");

			if (!Parse(filePath.c_str()))
				return false;
		}
		else
		{
			printf("Cannot download '%s'. Error: %d\n", scanName, result);
			return false;
		}
	}

	return true;
}

bool ScanData::Parse(const char *filePath)
{
	m_length.clear();

	for (int i = 0; i < 4; ++i)
		m_direction[i].clear();

	//printf("Parsing scan '%s'...\n", filePath);

	std::ifstream file(filePath);

	if (file.good())
	{
		int lineNumber = 0;

		std::string data;

		while (std::getline(file, data))
		{
			lineNumber++;

			// skip header
			if (lineNumber == 1)
				continue;

			// position
			if (lineNumber == 2)
			{
				int spaceOffset = static_cast<int>(data.find_first_of(' '));

				m_pos.x = static_cast<float>(atoi(data.substr(0, spaceOffset).c_str()));
				m_pos.y = static_cast<float>(atoi(data.substr(spaceOffset, data.size() - spaceOffset).c_str()));
			}

			// length
			if (lineNumber > 2 && lineNumber < 39)
			{
				Length length;
				length.angle = (lineNumber - 3) * 10;

				if (data == "inf")
					length.length = -1;
				else
					length.length = static_cast<float>(atof(data.c_str()));

				m_length.push_back(length);
			}

			// next move
			if (lineNumber > 38 && lineNumber <= 42)
			{
				int spaceOffset = static_cast<int>(data.find_first_of(' '));

				std::string directionName = data.substr(0, spaceOffset - 1);
				
				std::string addr = data.substr(spaceOffset + 1, data.size() - spaceOffset);

				if (directionName == "MOVE_EAST")
					m_direction[EAST] = addr;
				else if (directionName == "MOVE_WEST")
					m_direction[WEST] = addr;
				else if (directionName == "MOVE_SOUTH")
					m_direction[SOUTH] = addr;
				else if (directionName == "MOVE_NORTH")
					m_direction[NORTH] = addr;
			}
		}

		file.close();
	}
	else
	{
		printf("Cannot open '%s'!", filePath);
		return false;
	}

	return true;
}

const std::string ScanData::GetMove(Direction direction)
{
	if (direction >= 0 && direction <= 3)
		return m_direction[direction];
	else
		return std::string();
}
