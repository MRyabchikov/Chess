#pragma once

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <stdexcept>


template<typename T>
struct ObjectValue
{
    T* value;
    int reference_count;
};


template<typename T>
struct AttachManager
{
    AttachManager(T* object_, Simple_window& window_);
    AttachManager(const AttachManager&);
    AttachManager& operator=(const AttachManager&);
    ~AttachManager();

    // for easier access
    void set_color(Graph_lib::Color color) const;
    // for easier access
    void set_fill_color(Graph_lib::Color color) const;
    // questionable
    void draw_lines() const;

    // for debug
    int get_reference_count() const { return value->reference_count; }

    private:
        struct ObjectValue
        {
            int reference_count;
            T* object;
            Simple_window* window;
            ObjectValue(T*, Simple_window& window_);
            ~ObjectValue();
        };

        ObjectValue* value;

        // ObjectValue<T>* object;
};

template <typename T>
AttachManager<T>::ObjectValue::ObjectValue(T* init_value, Simple_window& window_)
{
    if(init_value == nullptr)
        throw std::runtime_error("init_value is nullptr");
    object = init_value;
    window = &window_;
    window->attach(*object);
    reference_count = 1;
}

template <typename T>
AttachManager<T>::ObjectValue::~ObjectValue()
{
    window->detach(*object);
    delete object;
}

template <typename T>
AttachManager<T>::AttachManager(T* object_, Simple_window& window_)
{
    if(object_ == nullptr)
        throw std::runtime_error("object is a nullptr");
    value = new ObjectValue{object_, window_};
}

template <typename T>
AttachManager<T>::AttachManager(const AttachManager& rhs)
{
    if(rhs.value == nullptr)
        throw std::runtime_error("object has already been deleted");
    value = rhs.value;
    ++value->reference_count;
}

template <typename T>
AttachManager<T>& AttachManager<T>::operator=(const AttachManager<T>& rhs)
{
    if(value == rhs.value)
    {
        return *this;
    }
    if(--value->reference_count == 0)
    {
        delete value;
    }
    if(rhs.value == nullptr)
        throw std::runtime_error("object has already been deleted");
    value = rhs.value;
    ++value->reference_count;
    return *this;
}

template <typename T>
AttachManager<T>::~AttachManager()
{
    if (--value->reference_count == 0)
    {
        delete value;
        value = nullptr;
    }
}

template <typename T>
void AttachManager<T>::set_color(Graph_lib::Color color) const
{
    if(value == nullptr)
        throw std::runtime_error("object has already been deleted");
    value->object->set_color(color);
}

template <typename T>
void AttachManager<T>::set_fill_color(Graph_lib::Color color) const
{
    if(value == nullptr)
        throw std::runtime_error("object has already been deleted");
    value->object->set_fill_color(color);
}

template <typename T>
void AttachManager<T>::draw_lines() const
{
    if(value == nullptr)
        throw std::runtime_error("object has already been deleted");
    value->object->draw_lines();
}