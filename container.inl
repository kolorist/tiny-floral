namespace floral
{
// ----------------------------------------------------------------------------

template <typename t_value>
array_t<t_value> create_array(ssize i_capacity, voidptr i_buffer)
{
    array_t<t_value> newArr;

    newArr.size = 0;
    newArr.capacity = i_capacity;
    newArr.data = (t_value*)i_buffer;

    return newArr;
}

template <typename t_value>
void array_push(array_t<t_value>* const i_arr, const t_value& i_value)
{
    i_arr->data[i_arr->size] = i_value;
    i_arr->size++;
}

template <typename t_value>
void array_empty(array_t<t_value>* const i_arr)
{
    i_arr->size = 0;
}

// ----------------------------------------------------------------------------
}
