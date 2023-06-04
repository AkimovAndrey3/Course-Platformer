#pragma once

#include "stdafx.h"

#include "ConstData.h"

struct Object
{
    int GetPropertyInt(std::string name);
    float GetPropertyFloat(std::string name);
    std::string GetPropertyString(std::string name);

    std::string name;
    std::string type;
    //!!!!!!!!!!!!!!!
    //TODO сменить на floatRect
    //sf::Rect<int> rect;
    sf::FloatRect rect;
    std::map<std::string, std::string> properties;

    ObjectId objectId;

    sf::Sprite sprite;
};

struct Layer
{
    int opacity;
    std::vector<sf::Sprite> tiles;
};


class Level
{
public:
    Level() : levelId(idCounter++) {}

    int levelNumber;
    bool LoadFromFile(std::string filename, short positionOffset);
    Object GetObj(std::string name);
    std::vector<Object> GetObjects(std::string name);
    std::vector<Object> GetObjects2(std::string name, std::string name2);
    std::vector<Object> GetAllObjects();
    void Draw(sf::RenderWindow& window);
    sf::Vector2i GetTileSize();

    sf::Vector2f GetPosition() const { return position; }

    void SetPostion(sf::Vector2f& position)
    {
        this->position = position;
    }
    void SetPostion(float x, float y)
    {
        this->position = sf::Vector2f(x, y);
    }

    void DestroyObject(ObjectId objectId)
    {
        auto itr = objectsMap.find(objectId);
        if(itr != objectsMap.end())
        {
            objectsMap.erase(itr);
        }
    }

    LevelId GetLevelId() const { return levelId; }
    void ResetIdCounter() { idCounter = 0; }

private:
    int width, height, tileWidth, tileHeight;
    int firstTileID;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;

    std::map<ObjectId, Object> objectsMap;

    sf::Vector2f position;

    LevelId levelId;
    static LevelId idCounter;
};
