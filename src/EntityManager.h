#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <vector>

typedef uint64_t bitmask;
const bitmask RESERVED = 0x01;

typedef class EntityManager {
private:
    template <typename C> static std::vector<C> component_vector; 
    template <typename C> static bitmask component_mask;

    static const int max_ents;
    static int end_id;
    static int component_types;
    static std::vector<bitmask> entity_mask;

public:
    static inline int new_entity(); 
    static inline int end() { return end_id; }

    template <typename... Cs>
    static inline int new_entity(Cs... c);

    template <typename C>
    static inline void remove_component(int entity);

    template <typename C>
    static inline void add_component(int entity, C c);

    template <typename C, typename... Cs>
    static inline void add_component(int entity, C c, Cs... cs);

    template <typename C>
    static inline C& get_component(int e) {
        if(!has_components<C>(e))
            exit(EXIT_FAILURE);
        return component_vector<C>[e];
    }

    template <typename C>
    static inline bool has_components(int e) {
        return !(component_mask<C> & (~entity_mask[e] | RESERVED));
    }

    template <typename C1, typename C2, typename... Cs>
    static inline bool has_components(int e) {
        return has_components<C1>(e) && has_components<C2, Cs...>(e);
    }
} EM;

#ifdef EM_MAX_ENTS
template <typename C> std::vector<C> EntityManager::component_vector;
template <typename C> bitmask EntityManager::component_mask = RESERVED;
const int EntityManager::max_ents = EM_MAX_ENTS;
int EntityManager::component_types = 0;
int EntityManager::end_id = 0;
std::vector<bitmask> EntityManager::entity_mask = std::vector<bitmask>(EM_MAX_ENTS);
#endif

/* Create a new entity, and return its ID */
int EntityManager::new_entity()
{
    int e = 0;
    while(entity_mask[e]) e++;
    if(e >= max_ents)
        exit(EXIT_FAILURE);

    entity_mask[e] = RESERVED;
    end_id = (e >= end_id) ? (e + 1) : end_id;
    return e;
}

/* Create new entity with components */
template <typename... Cs>
int EntityManager::new_entity(Cs... components) {
    int e = new_entity();
    add_component(e, components...);
    return e;
}

/* Add a new component to an entity */
template <typename C>
void EntityManager::add_component(int entity, C component)
{
    // create mask and vector on first instance of component type
    if(component_mask<C> == RESERVED) {
        component_mask<C> = 1 << ++component_types;
        component_vector<C> = std::vector<C>(max_ents);
    }
    // add component to vector, mark entity bitmask, unset reserve bit
    component_vector<C>[entity] = C(component);
    entity_mask[entity] |= component_mask<C>;
    entity_mask[entity] &= ~RESERVED;
}

/* Add multiple new components to an entity */
template <typename C, typename... Cs>
void EntityManager::add_component(int entity, C component, Cs... components) {
    add_component(entity, component);
    add_component(entity, components...);
}

/* Remove a component from an entity */
template <typename C>
void EntityManager::remove_component(int entity)
{
    entity_mask[entity] &= ~component_mask<C>;
    // TODO reset reserve bit on removal of last component?
}
