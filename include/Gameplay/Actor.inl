
template<typename T, typename ...Args>
inline T& Actor::AddComponent(Args&&... args) {
    return m_ecs.template AddComponent<T>(m_entity, std::forward<Args>(args)...);
}

template<typename T, typename ...Args>
inline T& Actor::AddComponentWithName(std::string_view name, Args&&... args) {
    return m_ecs.template AddComponent<T>(m_entity, name, std::forward<Args>(args)...);
}

template<typename T>
inline T& Actor::GetComponent(std::string_view name) {
    return m_ecs.template GetComponentByName<T>(m_entity, name);
}

template<typename T>
inline T& Actor::GetComponent() {
    return m_ecs.template GetComponent<T>(m_entity);
}

template<typename T>
inline std::vector<T*> Actor::GetComponents() {
    return m_ecs.template GetComponents<T>(m_entity);
}