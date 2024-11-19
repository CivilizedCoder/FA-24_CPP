#include <algorithm>
#include "textures.h"

using namespace std;

map<string, Texture2D> Textures::textures;

Texture2D Textures::loadTexture(string file)
{
	if (textures.find(file) == textures.end())
	{
		Texture2D texture = LoadTexture(file.c_str());
		textures[file] = texture;
	}

	return textures[file];
}
