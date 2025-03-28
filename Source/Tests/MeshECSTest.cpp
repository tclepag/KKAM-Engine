#include "KEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Mesh Component
struct MeshComponent {
    VertexArray Vertices;
    Indices Indices;
    std::string VertexShaderPath;
    std::string PixelShaderPath;
    DX11Geometry* Geometry;
    
    MeshComponent(
        VertexArray vertices = {},
        Indices indices = {},
        std::string vertexPath = "Content/Shaders/Default/Basic",
        std::string pixelPath = "Content/Shaders/Default/BasicPixel"
    ) : 
        Vertices(vertices), 
        Indices(indices),
        VertexShaderPath(vertexPath),
        PixelShaderPath(pixelPath),
        Geometry(nullptr) {}
};

// Transform Component
struct TransformComponent {
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
    
    TransformComponent(
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f)
    ) : 
        Position(position), 
        Rotation(rotation), 
        Scale(scale) {}
        
    glm::mat4 GetWorldMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Position);
        model = glm::rotate(model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Scale);
        return model;
    }
};

// Render System
class MeshRenderSystem {
private:
    Core::KEngine* Engine_;
    
public:
    MeshRenderSystem(Core::KEngine* engine) : Engine_(engine) {}
    
    void Initialize() {
        // Get references to component managers
        auto& meshManager = Engine_->GetWorld()->GetComponentManager<MeshComponent>();
        
        // For each entity with a mesh component, initialize its geometry
        for (auto entityID : Engine_->GetWorld()->GetEntities()) {
            if (meshManager.Has(entityID)) {
                MeshComponent* mesh = meshManager.TryGet(entityID);
                if (mesh && !mesh->Geometry) {
                    // Create DX11Geometry
                    mesh->Geometry = new DX11Geometry(
                        Engine_, 
                        Engine_->GetGraphics()->GetDevice().Get()
                    );
                    
                    mesh->Geometry->SetData(mesh->Vertices, mesh->Indices);
                    mesh->Geometry->SetVertexPath(mesh->VertexShaderPath);
                    mesh->Geometry->SetFragmentPath(mesh->PixelShaderPath);
                    
                    // Set initial transform if available
                    auto& transformManager = Engine_->GetWorld()->GetComponentManager<TransformComponent>();
                    if (transformManager.Has(entityID)) {
                        TransformComponent* transform = transformManager.TryGet(entityID);
                        if (transform) {
                            mesh->Geometry->SetWorldMatrix(transform->GetWorldMatrix());
                        }
                    }
                    
                    mesh->Geometry->Create();
                }
            }
        }
    }
    
    void Process(float deltaTime) {
        // Get references to component managers
        auto& meshManager = Engine_->GetWorld()->GetComponentManager<MeshComponent>();
        auto& transformManager = Engine_->GetWorld()->GetComponentManager<TransformComponent>();
        
        // Set up camera matrices
        float aspectRatio = static_cast<float>(Engine_->GetAppWindow()->GetWidth()) / 
                           static_cast<float>(Engine_->GetAppWindow()->GetHeight());
        glm::mat4 projMatrix = glm::perspectiveLH(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        
        // Camera position (hardcoded for testing)
        glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -2.0f);
        glm::vec3 lookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 viewMatrix = glm::lookAtLH(eyePosition, lookAtPosition, upDirection);
        
        // Render each entity with both components
        for (auto entityID : Engine_->GetWorld()->GetEntities()) {
            if (meshManager.Has(entityID) && transformManager.Has(entityID)) {
                MeshComponent* mesh = meshManager.TryGet(entityID);
                TransformComponent* transform = transformManager.TryGet(entityID);
                
                if (mesh && mesh->Geometry && transform) {
                    // Update transform
                    mesh->Geometry->SetWorldMatrix(transform->GetWorldMatrix());
                    mesh->Geometry->SetProjectionMatrix(projMatrix);
                    mesh->Geometry->SetViewMatrix(viewMatrix);
                    
                    // Draw the mesh
                    mesh->Geometry->Draw();
                }
            }
        }
    }
    
    void Cleanup() {
        // Clean up all geometry objects
        auto& meshManager = Engine_->GetWorld()->GetComponentManager<MeshComponent>();
        
        for (auto entityID : Engine_->GetWorld()->GetEntities()) {
            if (meshManager.Has(entityID)) {
                MeshComponent* mesh = meshManager.TryGet(entityID);
                if (mesh && mesh->Geometry) {
                    mesh->Geometry->Release();
                    delete mesh->Geometry;
                    mesh->Geometry = nullptr;
                }
            }
        }
    }
};

// Simple rotation system for animation
class RotationSystem {
private:
    Core::KEngine* Engine_;
    
public:
    RotationSystem(Core::KEngine* engine) : Engine_(engine) {}
    
    void Process(float deltaTime) {
        auto& transformManager = Engine_->GetWorld()->GetComponentManager<TransformComponent>();
        
        for (auto entityID : Engine_->GetWorld()->GetEntities()) {
            if (transformManager.Has(entityID)) {
                TransformComponent* transform = transformManager.TryGet(entityID);
                if (transform) {
                    // Rotate around Y axis
                    transform->Rotation.y += deltaTime * 1.0f;
                }
            }
        }
    }
};

// Test implementation
void start(std::shared_ptr<Core::KEngine> Engine) {
    // Create the rendering and rotation systems
    auto renderSystem = new MeshRenderSystem(Engine.get());
    auto rotationSystem = new RotationSystem(Engine.get());
    
    // Create an entity
    auto entity = Engine->GetWorld()->CreateEntity();
    
    // Create test pyramid geometry (same as in your KEngine::Initialize)
    VertexArray triangle = {
        // Apex/top point (red)
        {0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},

        // Base vertices (clockwise order from front)
        {0.0f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f},  // Front (green)
        {0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f}, // Right (blue)
        {-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f} // Left (yellow)
    };

    Indices indices = {
        // Front face (apex-front-right)
        0, 1, 2,

        // Right face (apex-right-left)
        0, 2, 3,

        // Left face (apex-left-front)
        0, 3, 1,

        // Base (front-left-right) - counter-clockwise for back face
        1, 3, 2
    };
    
    // Attach components to entity
    Engine->GetWorld()->AttachComponent<MeshComponent>(entity, triangle, indices);
    Engine->GetWorld()->AttachComponent<TransformComponent>(entity);
    
    // Initialize the render system
    renderSystem->Initialize();
    
    // Hook into the main loop with a custom rendering function
    Engine->SetCustomRenderFunc([renderSystem, rotationSystem](float deltaTime) {
        // Update rotation
        rotationSystem->Process(deltaTime);
        
        // Render meshes
        renderSystem->Process(deltaTime);
    });
    
    // NO FUNCTION YET
    // WILL ADD LATER
    Engine->SetShutdownCallback([renderSystem, rotationSystem]() {
        renderSystem->Cleanup();
        delete renderSystem;
        delete rotationSystem;
    });
    
    std::cout << "ECS-based mesh rendering system initialized!" << std::endl;
}