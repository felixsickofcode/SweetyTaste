
#include "Map.h"
#include <SDL_image.h>
#include <iostream>

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
void Map::loadTilesetsInfo()
{
    // Lấy tất cả tileset
    for (auto& tileset : mp.getTilesets())
    {
        std::cout << "Tileset name: " << tileset.getName() << "\n";
        std::cout << "Tile count: " << tileset.getTileCount() << "\n";
        std::cout << "Tile size: " << tileset.getTileSize().x << "x" << tileset.getTileSize().y << "\n";

        // Duyệt từng tile trong tileset collection
        for (auto& tile : tileset.getTiles())
        {
            std::cout << "  Tile ID: " << tile.getId() << "\n";
            std::cout << "  Tile Image Path: " << tile.getImage() << "\n";
            std::cout << "  Tile Size: " << tile.getImageSize().x << "x" << tile.getImageSize().y << "\n";
        }
    }
}
void Map::loadLayersInfo()
{
    for (const auto& layer : mp.getLayers())
    {
        std::cout << "Layer name: " << layer.getName() << "\n";
        std::cout << "Layer type: ";

        switch (layer.getType())
        {
            case tson::LayerType::TileLayer:
                std::cout << "Tile Layer\n";
                break;
            case tson::LayerType::ObjectGroup:
                std::cout << "Object Group\n";
                break;
            case tson::LayerType::ImageLayer:
                std::cout << "Image Layer\n";
                break;
            default:
                std::cout << "Other\n";
                break;
        }

        // Nếu là layer tile thì duyệt tile
        if (layer.getType() == tson::LayerType::TileLayer)
        {
            for (const auto& pair : layer.getTileData())
            {
                const auto& pos = pair.first;
                const auto& tile = pair.second;

                int x = std::get<0>(pos);
                int y = std::get<1>(pos);

                std::cout << "Tile at (" << x << ", " << y << ") ";
                if (tile != nullptr)
                std::cout << "ID: " << tile->getId() << "\n";
                else
                std::cout << "is empty\n";
            }

        }

        std::cout << "--------------------------\n";
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

            // Lưu texture vào map để lát còn render
            tileTextures[tile.getId()] = texture;

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
                    if (tileId == 0) continue; // Bỏ qua tile ID = 0

                    auto it = tileTextures.find(tileId);
                    if (it != tileTextures.end() && it->second != nullptr) {
                        SDL_Texture* texture = it->second;

                        // Lấy kích thước ảnh từ tile thay vì map
                        tson::Vector2i tileSize = tile->getImageSize();

                        SDL_Rect dstRect;
                        dstRect.x = std::get<0>(pos) * 32  ;
                        dstRect.y = (std::get<1>(pos) * 32 - tileSize.y );


                        dstRect.w = tileSize.x ;
                        dstRect.h = tileSize.y ;
//                        if (tileId == 15) {
//                            std::cout << "Tile Pos: (" << std::get<0>(pos) << ", " << std::get<1>(pos) << ")\n";
//                            std::cout << "DstRect Pos: (" << dstRect.x << ", " << dstRect.y << ")\n";
//                            int texWidth = 0, texHeight = 0;
//SDL_QueryTexture(texture, NULL, NULL, &texWidth, &texHeight);
//std::cout << "Texture Size: " << texWidth << "x" << texHeight << std::endl;
//
//                                                }

                        if (tileId == 15)
                            std::cout << "Tile Size: " << tileSize.x << "x" << tileSize.y << std::endl;

                        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
                    }
                }
            }
        }
    }
}




