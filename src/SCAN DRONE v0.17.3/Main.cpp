#include "App.h"

#include "Map.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		// parse all files and save as image
		Map map;
		map.LoadAllScans(argv[1]);
		map.SaveAsImage("map.jpg");
	}
	else
	{
		App app;
		app.Run();
	}
}
