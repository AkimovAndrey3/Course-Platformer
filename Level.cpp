#include "Level.h"
#include "tinyxml.h"
#include "stdafx.h"

LevelId Level::idCounter = 0;

int Object::GetPropertyInt(std::string name)
{
    return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
    return strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string name)
{
    return properties[name];
}

bool Level::LoadFromFile(std::string filename, short positionOffset)
{
    TiXmlDocument levelFile(filename.c_str());

    // Загружаем XML-карту
    if (!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }

    // Работаем с контейнером map
    TiXmlElement* map;
    map = levelFile.FirstChildElement("map");

    width = atoi(map->Attribute("width"));
    height = atoi(map->Attribute("height"));
    tileWidth = atoi(map->Attribute("tilewidth"));
    tileHeight = atoi(map->Attribute("tileheight"));

    // Берем описание тайлсета и идентификатор первого тайла
    TiXmlElement* tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    firstTileID = atoi(tilesetElement->Attribute("firstgid"));

    // source - путь до картинки в контейнере image
    std::string imagepath = INITIAL_MAP_FILENAME_START + tilesetElement->Attribute("source");

    // Пытаемся загрузить тайлсет
    sf::Image img;

    if (!img.loadFromFile(imagepath))
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }

    // Очищаем карту от света (109, 159, 185)
    img.createMaskFromColor(sf::Color(109, 159, 185));
    // Грузим текстуру из изображения
    tilesetImage.loadFromImage(img);
    // Расплывчатость запрещена
    tilesetImage.setSmooth(false);

    // Получаем количество столбцов и строк тайлсета
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;

    // Вектор из прямоугольников изображений (TextureRect)
    std::vector<sf::Rect<int>> subRects;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            sf::Rect<int> rect;

            rect.top = y * tileHeight;
            rect.height = tileHeight;
            rect.left = x * tileWidth;
            rect.width = tileWidth;

            subRects.push_back(rect);
        }
    }

    // Работа со слоями
    TiXmlElement* layerElement;
    layerElement = map->FirstChildElement("layer");
    while (layerElement)
    {
        Layer layer;

        // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }

        // Контейнер <data>
        TiXmlElement* layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");

        if (layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }


        auto layerWidth = atoi(layerElement->Attribute("width"));
        auto layerHeight = atoi(layerElement->Attribute("height"));


        std::string dataText = layerDataElement->GetText();
        std::string tileIdStr;
        std::stringstream ss(dataText);

        auto size = layerHeight * layerWidth;
        auto tileIds = new int[size];
        int i = 0;

        const int TO_CHECK = 337;

        while (getline(ss, tileIdStr, ','))
        {
            tileIds[i] = std::stoi(tileIdStr);
            ++i;
        }

        for (auto height = 0; height < layerHeight; ++height)
        {
            for (auto width = 0; width < layerWidth; ++width)
            {
                int tileGID = 0;

                tileGID = tileIds[height * layerWidth + width];

                int subRectToUse = tileGID - firstTileID;

                if (subRectToUse >= 0)
                {
                    sf::Sprite sprite;
                    sprite.setTexture(tilesetImage);
                    sprite.setTextureRect(subRects[subRectToUse]);
                    sprite.setPosition(width * tileWidth + positionOffset, height * tileHeight);
                    sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
                        
                    sprite.setOrigin((float)tileWidth / 2, (float)tileHeight / 2);

                    layer.tiles.push_back(sprite);
                }
            }
        }

        layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }

    // Работа с объектами
    TiXmlElement* objectGroupElement;

    // Если есть слои объектов
    if (map->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            // Контейнер <object>
            TiXmlElement* objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");

            while (objectElement)
            {
                // Получаем все данные - тип, имя, позиция, etc
                std::string objectType;
                if (objectElement->Attribute("type") != NULL)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectGroupElement->Attribute("name") != NULL)
                {
                    objectName = objectGroupElement->Attribute("name");
                }

                ObjectId objectId = 0;
                if (objectElement->Attribute("id") != NULL)
                {
                    objectId = atoi(objectElement->Attribute("id"));
                }

                int x = atoi(objectElement->Attribute("x"));
                int y = atoi(objectElement->Attribute("y"));

                int width, height;

                int gid = -1;
                if (objectElement->Attribute("gid") != NULL)
                {
                    gid = atoi(objectElement->Attribute("gid"));
                }

                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);

                if (gid == -1)
                {
                    sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
                }
                else
                {
                    sprite.setTextureRect(subRects[gid - firstTileID]);
                }

                sprite.setPosition(positionOffset + x, y - tileHeight);

                if (objectElement->Attribute("width") != NULL && objectElement->Attribute("height") != NULL)
                {
                    width = std::stof(objectElement->Attribute("width"));
                    height = std::stof(objectElement->Attribute("height"));
                }
                else
                {
                    width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
                    height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
                }
                sprite.setOrigin((float)width / 2, (float)height / 2);

                // Экземпляр объекта
                Object object;
                object.name = objectName;
                object.type = objectType;
                object.objectId = objectId;
                object.sprite = sprite;

                //sf::Rect <int> objectRect;
                sf::FloatRect objectRect;

                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;

                // "Переменные" объекта
                TiXmlElement* properties;
                properties = objectElement->FirstChildElement("properties");
                if (properties != NULL)
                {
                    TiXmlElement* prop;
                    prop = properties->FirstChildElement("property");
                    if (prop != NULL)
                    {
                        while (prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");

                            object.properties[propertyName] = propertyValue;

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }

                // Пихаем объект в вектор
                objects.push_back(object);
                objectsMap.emplace(objectId, object);
                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }

    return true;
}

Object Level::GetObj(std::string name)
{
    // Только первый объект с заданным именем
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name)
{
    // Все объекты с заданным именем
    std::vector<Object> vec;
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            vec.push_back(objects[i]);

    return vec;
}

std::vector<Object> Level::GetObjects2(std::string name, std::string name2)
{
    std::vector<Object> vec;
    for (int i = 0; i < objects.size(); i++)
        if ((objects[i].name == name) || (objects[i].name == name2))
            vec.push_back(objects[i]);

    return vec;
}

std::vector<Object> Level::GetAllObjects()
{
    return objects;
}

sf::Vector2i Level::GetTileSize()
{
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow& window)
{

    for (int layer = 0; layer < layers.size(); layer++)
        for (int tile = 0; tile < layers[layer].tiles.size(); tile++)
            window.draw(layers[layer].tiles[tile]);

    for (auto& itr : objectsMap)
    {
        window.draw(itr.second.sprite);
    }
}
