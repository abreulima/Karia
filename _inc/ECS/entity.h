#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <string>
#include <memory>

#include "component.h"

class Entity {
public:
    Entity();
    Entity(std::string name);
    bool isActive;
    std::string name;
    std::list<std::shared_ptr<Component>> components;

    template <typename T, typename... TArgs>
    std::shared_ptr<T> addComponent(TArgs&&... args) {
        //auto comp = new T();
        auto comp = std::make_shared<T>(std::forward<TArgs>(args)...);
        components.emplace_back(comp);
        return comp;
    }

    template <typename T>
    std::shared_ptr<T> getComponent() {
        for (auto &c : components) {
            auto entity = std::dynamic_pointer_cast<T>(c);
            if (entity)
                return entity;
        }
        return nullptr;
    }

    template <typename T>
    bool hasEntity() {
        for (auto &c : components) {
            auto entity = std::dynamic_pointer_cast<T>(c);
            if (entity)
                return true;
        }
        return false;
    }
};

#endif