#pragma once
#include <vector>
class Tile;
class EmptyField;

class Creator
{
public: // poprz. private
	static const std::string data_filename;

	static std::vector<Tile> LoadTileData();
	static void SetTileAttributes(Tile&, std::vector<int>&, std::vector<int>&);
	static EmptyField** SetEmptyFields(); // Tworzy tablicê [sqf_X][sqf_Y] pustych pól
	static void ClearEmptyFields(EmptyField** &EFtab);
private:

};
