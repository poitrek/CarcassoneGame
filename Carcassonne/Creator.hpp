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
	static EmptyField** SetEmptyFields(); // Tworzy tablic� [sqf_X][sqf_Y] pustych p�l
	static void ClearEmptyFields(EmptyField** &EFtab);
private:

};
