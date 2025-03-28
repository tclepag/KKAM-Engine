#pragma once

#include <iostream>
#include <vector>
#include <typeindex>
#include <typeindex>
#include <typeinfo>

namespace Core {
    template <typename T>
    class KECSManager;
    
    using KEntity = size_t;

    class BaseManager {
        public:
        virtual ~BaseManager() = default;
        virtual void Unregister(KEntity EntityID) = 0;
    };

    class KRegistry {
        public:

        // Entities

        KEntity CreateEntity();
        void DestroyEntity(KEntity EntityID);

        // Components

        // Registers and attaches a component to an entity
        template <typename C, typename... Args>
        KECSManager<C>& AttachComponent(KEntity EntityID, Args... Arguments);
        // Detaches and unregisters a component from an entity
        template <typename C>
        void DetachComponent(KEntity EntityID);

        // Systems

        // Registers and attaches a system to an entity
        template <typename S, typename... Args>
        KECSManager<S>& AttachSystem(KEntity EntityID, Args... Arguments);
        // Detaches and unregisters a system from an entity
        template <typename S>
        void DetachSystem(KEntity EntityID);

        private:
        KEntity Index_ = 0;

        std::vector<KEntity> Entities_;
        std::unordered_map<std::type_index, std::unique_ptr<BaseManager>> Components_;
        std::unordered_map<std::type_index, std::unique_ptr<BaseManager>> Systems_;
    };
}