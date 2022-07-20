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

template <typename t_commandBuffer, typename t_value>
void cmdbuff_read(t_commandBuffer* const i_cmdBuff, t_value* o_value)
{
    p8 rpos = i_cmdBuff->readPtr;
    memcpy(o_value, rpos, sizeof(t_value));
    i_cmdBuff->readPtr = rpos + sizeof(t_value);
}

template <typename t_commandBuffer, typename t_value>
void cmdbuff_write(t_commandBuffer* const io_cmdBuff, const t_value& i_value)
{
    p8 wpos = io_cmdBuff->writePtr;
    memcpy(wpos, &i_value, sizeof(t_value));
    io_cmdBuff->writePtr = wpos + sizeof(t_value);
}

// ----------------------------------------------------------------------------
}
