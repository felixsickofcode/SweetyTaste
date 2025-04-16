
#include "Map.h"

Map::Map(const std::string& filename) : mapFile(filename) {}
bool Map::load() {

    tson::Tileson parser;
    std::unique_ptr<tson::Map> result = parser.parse(fs::path(mapFile));

    if (result && result->getStatus() == tson::ParseStatus::OK) {
        mp = std::move(*result);
        std::cout << "Map loaded successfully!\n";
    return true;
}
    else {
        std::cerr << "Failed to load map: " << mapFile << "\n";
        return false;
    }
}

void Map::loadTextures(SDL_Renderer* renderer)
{
    for (auto& tileset : mp.getTilesets())
    {
        for (auto& tile : tileset.getTiles())
        {
            const std::string& imagePath = tile.getImage().u8string();
            SDL_Surface* surface = IMG_Load(imagePath.c_str());

            if (!surface)
            {
                std::cerr << "Failed to load surface for tile " << tile.getId() << ": " << IMG_GetError() << "\n";
                continue;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            if (!texture)
            {
                std::cerr << "Failed to create texture for tile " << tile.getId() << ": " << SDL_GetError() << "\n";
                continue;
            }
            tileTextures[tile.getId()].LoadImg(imagePath.c_str(), renderer);
            //tileTextures[tile.getId()] = texture;

            //std::cout << "Loaded texture for tile " << tile.getId() << "\n";
        }
    }
}

void Map::render(SDL_Renderer* renderer) {
    for (tson::Layer& layer : mp.getLayers()) { // Duyệt từng layer
        if (layer.getType() == tson::LayerType::TileLayer) {



            int x1 = visual_map.start_x / TileSize - 18;
            int x2 = (visual_map.start_x + game_w) / TileSize + 18;

            int y1 = visual_map.start_y / TileSize - 18;
            int y2 = (visual_map.start_y + game_h) / TileSize + 18;

            for (int row = y1; row <= y2; ++row) {
                for (int col = x1; col <= x2; ++col) {

                    tson::Tile* tile = layer.getTileData(col, row);
                    if (tile != nullptr) {
                        int tileId = tile->getId();
                        if (tileId == 0 || tileId >= 250) continue;

                        tson::Vector2i tileSizeInImage = tile->getImageSize();

                        tileTextures[tileId].SetRect(
                            col * TileSize - visual_map.start_x,
                            row * TileSize - visual_map.start_y - tileSizeInImage.y
                        );

                        tileTextures[tileId].Render(renderer);
                    }
                }
            }
        }
    }
}

void Map::SaveCollision()
{
     for (tson::Layer& layer : mp.getLayers()) {
        if (layer.getType() != tson::LayerType::TileLayer || layer.getName() != "terrian") continue;
        {
            std::memset(game_map.tile, 0, sizeof(game_map.tile));

            for (const auto& [pos, tile] : layer.getTileData()) {
                if (tile != nullptr) {
                    int tileId = tile->getId();
                    if (tileId == 0 || tileId >= 250) continue;
                    tson::Vector2i tileSize = tile->getImageSize();
                    int X =  std::get<0>(pos);
                    int Y =  std::get<1>(pos);
                        game_map.tile[Y][X] = tileId;

                }
            }
        }
    }
    //printf();
}

void Map::printf()
{
    for ( int i = 0; i <= MapY; i++)
    {
        for ( int j = 0; j <= MapX; j++)
        {
            std ::cout << visual_map.tile[i][j] << ' ';
        }
        std :: cout << std :: endl;
    }

}



void Map::GetEnemyPos()
{
    auto* layer = mp.getLayer("enemy");
    if (layer == nullptr) return;
    int index = 0;
    for (auto& obj : layer->getObjects()) {
        int x = static_cast<int>(obj.getPosition().x);
        int y = static_cast<int>(obj.getPosition().y);
        spawn_e[index].posy = y;
        spawn_e[index].posx = x;

        //std :: cout << spawn_e[index].posx << ' ' << spawn_e[index].posy << '\n';
        index++;
    }
    //printf();
}







