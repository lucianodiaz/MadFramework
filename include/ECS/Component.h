#pragma once
#include <MadFrameworkExport.h>
#include <unordered_map>
#include <memory>
#include "Entity.h"

// Generador de ID global para todos los componentes
inline size_t GenerateGlobalComponentID()
{
    static size_t lastID = 0;
    return lastID++;
}

class MAD_API IComponent
{
public:
    virtual ~IComponent() = default;
};

template <typename T>
class Component : public IComponent
{
public:
    static std::unordered_map<Entity, T> data;

    static size_t GetID()
    {
        static size_t id = GenerateGlobalComponentID(); // Usa la funci�n global
        return id;
    }
};

// Inicializaci�n del almacenamiento est�tico
template <typename T>
std::unordered_map<Entity, T> Component<T>::data;