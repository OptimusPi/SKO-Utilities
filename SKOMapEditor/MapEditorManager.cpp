#include "MapEditorManager.h"

SKO_MapEditor::Manager::Manager()
{

}

SKO_MapEditor::Manager::~Manager()
{

}

// Clean up useless rectangles (<4x4 pixels) to a SKO_Map in-place.
void SKO_MapEditor::Manager::cleanupInvisibleRects(SKO_Map::Map *map)
{
	std::vector<int> invisibleRectIds;

	//Delete any collision rectangles that are too small.
	for (int i = 0; i < map->collisionRects.size(); i++)
	{
		if (map->collisionRects[i]->h < 4 || map->collisionRects[i]->w < 4)
		{
			current_rect--;
			invisibleRectIds.push_back(i);
		}
	}

	for (int i = 0; i < invisibleRectIds.size(); i++)
	{
		map->collisionRects.erase(map->collisionRects.begin() + invisibleRectIds[i]);
	}
}

void SKO_MapEditor::Manager::saveMap(std::string fileLocation)
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Save the map at the given file location
	this->map->saveMap(fileLocation);
}

void SKO_MapEditor::Manager::saveMap()
{
	// Cleanup small rects you can't see
	this->cleanupInvisibleRects(this->map);

	// Save the map to its previous file location
	this->map->saveMap();
}


void SKO_MapEditor::Manager::loadMap(std::string fileName)
{
	// Load map
	this->map = new SKO_Map::Map(fileName);

	// Edge case where accidental small rectangles are leftover, clean these up
	SKO_MapEditor::Manager::cleanupInvisibleRects(this->map);
}