#pragma once

#include <iostream>
#include <json/json.h>

#include "game/objects/GameObjectCache.h"
#include "ui/ObjectPicker.h"

class ObjectPickerLoader {
    private:
    static const int PICKER_WIDTH = 18;
    static const int PICKER_HEIGHT = 28;

    public:
    ObjectPickerLoader() {}

    ObjectPicker* loadObjectPicker() {
        std::string fullPath = ResourceReader::getFullPath(ResourceReader::UI, "objectPicker.json");
        std::ifstream file;
        file.open(fullPath, std::ios::in);
        if (!file.is_open()) {
            file.close();
            throw "Unable to load object picker";
        }

        Json::Value root;
        file >> root;
        file.close();

        Json::Value array = root.get("groups", Json::Value(Json::arrayValue));

        ObjectPickerGroup** groups = loadGroupList(array);

        return new ObjectPicker(groups, array.size(), PICKER_WIDTH, PICKER_HEIGHT);
    }

    ObjectPickerGroup** loadGroupList(Json::Value& array) {
        ObjectPickerGroup** groups = new ObjectPickerGroup*[array.size()];
        for (Json::Value::ArrayIndex i = 0; i < array.size(); i++) {
            groups[i] = loadGroup(array[i]);
        }
        return groups;
    }

    ObjectPickerGroup* loadGroup(Json::Value& data) {
        const char* label = data.get("label", "").asString().c_str();

        Json::Value groupsArray = data.get("groups", Json::Value(Json::arrayValue));
        ObjectPickerGroup** groups = loadGroupList(groupsArray);

        Json::Value itemsArray = data.get("items", Json::Value(Json::arrayValue));
        GameObject** items = loadItemList(itemsArray);

        return new ObjectPickerGroup(label, groups, groupsArray.size(), items, itemsArray.size());
    }

    GameObject** loadItemList(Json::Value& array) {
        Json::Value::ArrayIndex size = array.size();
        GameObject** objects = new GameObject*[size];
        for (Json::Value::ArrayIndex i = 0; i < size; i++) {
            std::string name = array[i].asString();
            objects[i] = GameObjectCache::objects.at(name.c_str());
        }
        return objects;
    }
};