#pragma once

#include <iostream>
#include <json/json.h>

#include "game/GameObjectCache.h"
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
        GameObject** objects = new GameObject*[array.size()];
        for (Json::Value::ArrayIndex i = 0; i < array.size(); i++) {
            const char* name = array[i].asString().c_str();
            objects[i] = GameObjectCache::objects.at(name);
        }
        return objects;
    }
};