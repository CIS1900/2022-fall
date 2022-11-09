template <typename T>
wrapper<T>::wrapper(T t) : t {t}
{}

template <typename T>
T wrapper<T>::get() const
{
    return t;
}

template <typename T>
void wrapper<T>::set(T t)
{
    this->t = t;
}
