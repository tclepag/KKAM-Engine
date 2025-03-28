#pragma once

#include "Core/KECSManager.h"

namespace Core {
    template <typename T>
    void KECSManager<T>::Register(size_t EntityID, T Data) {
        if (Has(EntityID)) {
            // Replace existing component
            size_t dataIndex = EntityToDataIndex_[EntityID];
            Data_[dataIndex] = Data; // Simple replacement
        } else {
            // Add new component
            Data_.push_back(Data);
            size_t newIndex = Data_.size() - 1;
            
            EntityToDataIndex_[EntityID] = newIndex;
            DataToEntityIndex_.push_back(EntityID);
        }
    }

    template <typename T>
    void KECSManager<T>::Unregister(size_t EntityID) {
        if (!Has(EntityID)) {
            return;
        }

        size_t indexToRemove = EntityToDataIndex_[EntityID];
        size_t lastIndex = Data_.size() - 1;
        // If the object isn't last then we have to swap and readjust
        if (indexToRemove != lastIndex) {
            // Swap the indices so that
            std::swap(Data_[indexToRemove], Data_[lastIndex]);
            // Get entity id that owns the last index
            size_t lastEntityID = DataToEntityIndex_[lastIndex];
            // Update it's references
            EntityToDataIndex_[lastEntityID] = indexToRemove;
            DataToEntityIndex_[indexToRemove] = lastEntityID;
        }

        // Remove and unregister
        Data_.pop_back();
        DataToEntityIndex_.pop_back();
        EntityToDataIndex_.erase(EntityID);
    }

    template <typename T>
    T KECSManager<T>::Get(size_t EntityID) {
        return Data_[EntityToDataIndex_[EntityID]];
    }

    template <typename T>
    bool KECSManager<T>::Has(size_t EntityID) {
        return EntityToDataIndex_.find(EntityID) != EntityToDataIndex_.end();
    }

    template <typename T>
    T* KECSManager<T>::TryGet(size_t EntityID) {
        if (!Has(EntityID)) {
            return nullptr;
        }
        return &Data_[EntityToDataIndex_[EntityID]];
    }
}