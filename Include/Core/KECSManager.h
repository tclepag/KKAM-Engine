#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "KRegistry.h"

namespace Core {
    template <typename T>
    class KECSManager final : public BaseManager {
        public:
        // Adds an ecs object by the entity to this 
        void Register(size_t EntityID, T Data);
        // Removes an ecs object by the entity from this
        void Unregister(size_t EntityID) override;
        // Returns the ecs object occupied by the entity
        // ECS Object must exist for this to work, if you are unsure use TryGet
        T Get(size_t EntityID);
        // Returns if this entity has any ecs objects here
        bool Has(size_t EntityID);
        // Returns component or nullptr
        // CALLER SHOULD NOT DELETE THIS OBJECT, PREMATURE DELETION MAY CAUSE ISSUES
        T* TryGet(size_t EntityID);

        private:
        std::vector<T> Data_;
        std::unordered_map<size_t, size_t> EntityToDataIndex_;
        std::vector<size_t> DataToEntityIndex_;
    };
}