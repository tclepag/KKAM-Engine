#include "KRegistry.h"
#include "KECSManager.h"

namespace Core
{
    KEntity KRegistry::CreateEntity()
    {
        Entities_.push_back(Index_);
        Index_ += 1;
        return Index_ - 1;
    }

    void KRegistry::DestroyEntity(KEntity EntityID)
    {
        for (auto &[type, manager] : Components_)
        {
            manager->Unregister(EntityID);
        }
        for (auto &[type, manager] : Systems_)
        {
            manager->Unregister(EntityID);
        }

        Entities_.erase(std::remove(Entities_.begin(), Entities_.end(), EntityID), Entities_.end());
    }

    template <typename C, typename... Args>
    KECSManager<C> &KRegistry::AttachComponent(KEntity EntityID, Args... Arguments)
    {
        // Get the type index for this component type
        auto typeIndex = std::type_index(typeid(C));

        // Check if a manager for this component type already exists
        if (Components_.find(typeIndex) == Components_.end())
        {
            // Create a new manager if it doesn't exist
            Components_[typeIndex] = std::make_unique<KECSManager<C>>();
        }

        // Get the manager (with the correct type)
        KECSManager<C>* componentManager =
            static_cast<KECSManager<C>*>(Components_[typeIndex].get());

        // Register the component with the entity
        componentManager->Register(EntityID, C(Arguments...));

        // Return the manager
        return *componentManager;
    }

    template <typename C>
    void KRegistry::DetachComponent(KEntity EntityID)
    {
        // Get the type index for this component type
        auto typeIndex = std::type_index(typeid(C));

        // Check if a manager for this component type exists
        auto it = Components_.find(typeIndex);
        if (it != Components_.end())
        {
            // Get the manager (with the correct type)
            KECSManager<C>* componentManager =
                static_cast<KECSManager<C>*>(it->second.get());

            // Unregister the component from the entity
            componentManager->Unregister(EntityID);
        }
    }

    template <typename S, typename... Args>
    KECSManager<S> &KRegistry::AttachSystem(KEntity EntityID, Args... Arguments)
    {
        // Get the type index for this system type
        auto typeIndex = std::type_index(typeid(S));

        // Check if a manager for this system type already exists
        if (Systems_.find(typeIndex) == Systems_.end())
        {
            // Create a new manager if it doesn't exist
            Systems_[typeIndex] = std::make_unique<KECSManager<S>>();
        }

        // Get the manager (with the correct type)
        KECSManager<S>* systemManager =
            static_cast<KECSManager<S>*>(Systems_[typeIndex].get());

        // Register the component with the entity
        systemManager->Register(EntityID, S(Arguments...));

        // Return the manager
        return *systemManager;
    }

    template <typename S>
    void KRegistry::DetachSystem(KEntity EntityID)
    {
        // Get the type index for this component type
        auto typeIndex = std::type_index(typeid(S));

        // Check if a manager for this component type exists
        auto it = Systems_.find(typeIndex);
        if (it != Systems_.end())
        {
            // Get the manager (with the correct type)
            KECSManager<S>* systemManager =
                static_cast<KECSManager<S>*>(it->second.get());

            // Unregister the component from the entity
            systemManager->Unregister(EntityID);
        }
    }
}