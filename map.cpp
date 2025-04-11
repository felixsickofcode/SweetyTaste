
#include "Map.h"

Map::Map(const std::string& filename) : mapFile(filename) {}
bool Map::load() {

    tson::Tileson parser;
    std::unique_ptr<tson::Map> result = parser.parse(fs::path(mapFile));

    if (result && result->getStatus() == tson::ParseStatus::OK) {
        mp = std::move(*result); // Giải dereference con unique_ptr
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

            std::cout << "Loaded texture for tile " << tile.getId() << "\n";
        }
    }
}

void Map::render(SDL_Renderer* renderer) {
    for (const auto& layer : mp.getLayers()) {
        if (layer.getType() == tson::LayerType::TileLayer) {
            for (const auto& [pos, tile] : layer.getTileData()) {
                if (tile != nullptr) {
                    int tileId = tile->getId();
                    if (tileId == 0 || tileId >= 250) continue; // Bỏ tile id không hợp lệ

                    tson::Vector2i tileSize = tile->getImageSize();


                    tileTextures[tileId].SetRect(
                        std::get<0>(pos) * 32,
                        std::get<1>(pos) * 32 - tileSize.y
                    );

                    tileTextures[tileId].Render(renderer);
                }
            }
        }
    }
}








